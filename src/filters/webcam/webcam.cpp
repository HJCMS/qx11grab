/**
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann (Undefined) http://qx11grab.hjcms.de, (C) 2007-2012
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Library General Public
* License as published by the Free Software Foundation; either
* version 2 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU Library General Public License
* along with this library; see the file COPYING.LIB.  If not, write to
* the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
* Boston, MA 02110-1301, USA.
**/

#include "webcam.h"
#include "webcampreview.h"
#include "webcamdevicechooser.h"
#include "webcamcaptureframes.h"

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QRectF>
#include <QtCore/QSizeF>
#include <QtCore/QPointF>
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFileDialog>
#include <QtGui/QFormLayout>
#include <QtGui/QLabel>
#include <QtGui/QIcon>
#include <QtGui/QToolBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

/* QtDBus */
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>

Webcam::Webcam ( QWidget * parent )
    : QDialog ( parent )
    , p_MarginX ( 20 )
    , p_MarginY ( 20 )
{
  setObjectName ( QLatin1String ( "Webcam" ) );
  /*: WindowTitle */
  setWindowTitle ( trUtf8 ( "Webcam Filter" ) );
  setWindowIcon ( QIcon::fromTheme ( "camera-web" ) );
  setContentsMargins ( 0, 0, 0, 10 );
  setSizeGripEnabled ( true );
  setMinimumSize ( 650, 550 );

  cfg = new QSettings ( QSettings::NativeFormat, QSettings::UserScope, "hjcms.de", "qx11grab", this );

  QVBoxLayout* verticalLayout = new QVBoxLayout ( this );
  verticalLayout->setContentsMargins ( 0, 5, 0, 0 );

  m_webcamPreview = new WebCamPreview ( this );
  verticalLayout->addWidget ( m_webcamPreview );


  // begin::layer {
  QWidget* layerWidget = new QWidget ( this );
  verticalLayout->addWidget ( layerWidget );

  QFormLayout* layout = new QFormLayout ( layerWidget );
  layout->setObjectName ( QLatin1String ( "webcam/Layout" ) );

  m_webcamDeviceChooser = new WebCamDeviceChooser ( layerWidget );
  m_webCamCaptureFrames = new WebCamCaptureFrames ( layerWidget );
  layout->addRow ( m_webCamCaptureFrames, m_webcamDeviceChooser );

  int i = 0;
  QIcon scIcon = QIcon::fromTheme ( "video-display" );
  m_setOverlayComboBox = new QComboBox ( layerWidget );
  m_setOverlayComboBox->insertItem ( i++, scIcon,  trUtf8 ( "Top Left" ), TOP_LEFT_CORNER );
  m_setOverlayComboBox->insertItem ( i++, scIcon,  trUtf8 ( "Top Right" ), TOP_RIGHT_CORNER );
  m_setOverlayComboBox->insertItem ( i++, scIcon,  trUtf8 ( "Bottom Left" ), BOTTOM_LEFT_CORNER );
  m_setOverlayComboBox->insertItem ( i++, scIcon,  trUtf8 ( "Bottom Right" ), BOTTOM_RIGHT_CORNER );
  layout->addRow ( trUtf8 ( "Position" ), m_setOverlayComboBox );

  // Gültige Werte sind von 160x120 bis 640x480
  m_scaleFrame = new QSlider ( Qt::Horizontal, layerWidget );
  m_scaleFrame->setRange ( 100, 640 );
  m_scaleFrame->setValue ( 160 );
  layout->addRow ( trUtf8 ( "Scale Frame" ), m_scaleFrame );

  m_xIndent = new QSlider ( Qt::Horizontal, layerWidget );
  m_xIndent->setRange ( 2, 200 );
  m_xIndent->setValue ( 2 );
  layout->addRow ( trUtf8 ( "Margin (Left|Right)" ), m_xIndent );

  m_yIndent = new QSlider ( Qt::Horizontal, layerWidget );
  m_yIndent->setRange ( 2, 200 );
  m_yIndent->setValue ( 2 );
  layout->addRow ( trUtf8 ( "Margin (Top|Bottom)" ), m_yIndent );

  m_outputEdit = new QLineEdit ( layerWidget );
  layout->addRow ( m_outputEdit );

  layerWidget->setLayout ( layout );
  // } end::layer

  QDialogButtonBox* m_buttonBox = new QDialogButtonBox ( Qt::Horizontal, this );
  m_buttonBox->setObjectName ( QLatin1String ( "webcam/ButtonBox" ) );
  m_buttonBox->setStandardButtons ( ( QDialogButtonBox::Save | QDialogButtonBox::Cancel ) );
  verticalLayout->addWidget ( m_buttonBox );

  setLayout ( verticalLayout );

  connect ( m_webcamDeviceChooser, SIGNAL ( cameraSelected ( const WebCamDeviceInfo & ) ),
            this, SLOT ( cameraDeviceChanged ( const WebCamDeviceInfo & ) ) );

  connect ( m_setOverlayComboBox, SIGNAL ( currentIndexChanged ( int ) ),
            this, SLOT ( positionIndexChanged ( int ) ) );

  connect ( m_webCamCaptureFrames, SIGNAL ( frameCaptered ( const QImage & ) ),
            m_webcamPreview, SLOT ( pixmapFromImage ( const QImage & ) ) );

  connect ( m_scaleFrame, SIGNAL ( valueChanged ( int ) ),
            this, SLOT ( setScale ( int ) ) );

  connect ( m_xIndent, SIGNAL ( valueChanged ( int ) ),
            this, SLOT ( setMarginX ( int ) ) );

  connect ( m_yIndent, SIGNAL ( valueChanged ( int ) ),
            this, SLOT ( setMarginY ( int ) ) );

  connect ( m_buttonBox, SIGNAL ( accepted() ),
            m_webcamPreview, SLOT ( restoreView() ) );

  connect ( m_buttonBox, SIGNAL ( rejected() ),
            m_webcamPreview, SLOT ( restoreView() ) );

  connect ( m_buttonBox, SIGNAL ( accepted() ),
            this, SLOT ( accept() ) );

  connect ( m_buttonBox, SIGNAL ( rejected() ),
            this, SLOT ( reject() ) );
}

void Webcam::setSettings ( const QString &key, const QVariant &value )
{
  QString path = QString::fromUtf8 ( "Filter_Webcam/%1" ).arg ( key );
  cfg->setValue ( path, value );
}

const QVariant Webcam::settingsValue ( const QString &key, const QVariant &defaultValue )
{
  QString path = QString::fromUtf8 ( "Filter_Webcam/%1" ).arg ( key );
  return cfg->value ( path, defaultValue );
}

void Webcam::loadDefaults()
{
  p_v4lDevice = settingsValue ( "Device", "/dev/video0" ).toString();
  m_webcamPreview->setDevice ( p_v4lDevice );
  m_webcamDeviceChooser->init();

  // Vorschau bereich auf Slider übertragen
  m_scaleFrame->setValue ( settingsValue ( "Scale", 100 ).toUInt() );
  m_xIndent->setValue ( settingsValue ( "Indent_X", 20 ).toUInt() );
  m_yIndent->setValue ( settingsValue ( "Indent_Y", 20 ).toUInt() );

  // Index der Combobox setzen und update auslösen
  p_Overlay = settingsValue ( "Overlay", "20:20" ).toString();
  m_setOverlayComboBox->setCurrentIndex ( settingsValue ( "PositionType", 0 ).toUInt() );
}

/**
* Kamera wurde ausgwewählt
*/
void Webcam::cameraDeviceChanged ( const WebCamDeviceInfo &dev )
{
  p_v4lDevice = dev.path;
  m_scaleFrame->setRange ( 100, qMax ( dev.size.width(), dev.size.height() ) );
  m_webCamCaptureFrames->setInterface ( dev );

  // Ausgabezeile neu Schreiben
  update();
}

void Webcam::resizeEvent ( QResizeEvent * event )
{
  // positionIndexChanged ( m_setOverlayComboBox->currentIndex() );
  QDialog::resizeEvent ( event );
}

/**
* Wird immer dann aufgerufen wenn sich eine Änderung bei den Einstellungen ergibt!
* Die Position des Bildes muss jedes mal neu gesetzt werden!
* Wir starten immer von oben Links, was zu folge hat das wir bei QPointF,
* im gegensatz zu FFmpeg, im Negativen Bereich arbeiten!
*/
void Webcam::positionIndexChanged ( int index )
{
  // Zeichenketten für FFmpeg overlay
  QString _x = QString::number ( p_MarginX );
  QString _y = QString::number ( p_MarginY );

  // Vorschau Bereich
  qreal main_w = m_webcamPreview->rect().width();
  qreal main_h = m_webcamPreview->rect().height();

  // Bild Bereich
  qreal overlay_w = m_webcamPreview->itemSize().width();
  qreal overlay_h = m_webcamPreview->itemSize().height();

  // Das Koordinaten System startet immer oben Links
  QPointF overlayTopLeft ( ( 0 - p_MarginX ), ( 0 - p_MarginY ) );

  CORNER_POSITION p = static_cast<CORNER_POSITION> ( m_setOverlayComboBox->itemData ( index, Qt::UserRole ).toUInt() );
  switch ( p )
  {
    case TOP_RIGHT_CORNER:
    {
      overlayTopLeft = QPointF ( ( 0 - ( main_w - overlay_w - p_MarginX ) ), ( 0 - p_MarginY ) );
      p_Overlay = QString::fromUtf8 ( "main_w-overlay_w-%1:%2" ).arg ( _x, _y );
    }
    break;

    case BOTTOM_LEFT_CORNER:
    {
      overlayTopLeft = QPointF ( ( 0 - p_MarginX ), ( 0 - ( main_h - overlay_h - p_MarginY ) ) );
      p_Overlay = QString::fromUtf8 ( "%1:main_h-overlay_h-%2" ).arg ( _x, _y );
    }
    break;

    case BOTTOM_RIGHT_CORNER:
    {
      overlayTopLeft = QPointF ( ( 0 - ( main_w - overlay_w - p_MarginX ) ),
                                 ( 0 - ( main_h - overlay_h - p_MarginY ) ) );
      p_Overlay = QString::fromUtf8 ( "main_w-overlay_w-%1:main_h-overlay_h-%2" ).arg ( _x, _y );
    }
    break;

    default:
    {
      overlayTopLeft = QPointF ( ( 0 - p_MarginX ), ( 0 - p_MarginY ) );
      p_Overlay = QString::fromUtf8 ( "%1:%2" ).arg ( _x, _y );
    }
    break;
  };

  // qDebug() << Q_FUNC_INFO << overlayTopLeft << m_webcamPreview->itemSize();

  QSizeF currentSizeF = m_webcamPreview->itemSize();
  // Bild verschieben
  m_webcamPreview->setSceneRect ( QRectF ( overlayTopLeft, currentSizeF ) );
  // Scaler neu setzen
  m_scaleFrame->setValue ( qMax ( currentSizeF.width(), currentSizeF.height() ) );

  // Ausgabezeile neu Schreiben
  update();
}

/** Größen Skalierung in Size */
void Webcam::setScale ( int s )
{
  qreal r = static_cast<qreal> ( s );
  QSizeF ret ( m_webcamPreview->itemSize() );
  ret.scale ( r, r, Qt::KeepAspectRatio );
  m_webcamPreview->setItemSize ( ret );
  update();
}

/** links|rechts einrücken */
void Webcam::setMarginX ( int x )
{
  if ( ( x % 2 ) == 0 )
  {
    p_MarginX = x;
    positionIndexChanged ( m_setOverlayComboBox->currentIndex() );
  }
}

/** oben|unten einrücken */
void Webcam::setMarginY ( int y )
{
  if ( ( y % 2 ) == 0 )
  {
    p_MarginY = y;
    positionIndexChanged ( m_setOverlayComboBox->currentIndex() );
  }
}

void Webcam::update()
{
  QSize scale = m_webcamPreview->itemSize().toSize();
  QString buf;
  QString value = QString::fromUtf8 ( "movie=%1:f=v4l2,scale=%2:%3[logo],[in][logo]overlay=%4,setpts=PTS-STARTPTS[out]" )
                  .arg (
                      p_v4lDevice,
                      QString::number ( scale.width() ),
                      QString::number ( scale.height() ),
                      p_Overlay
                  );

  m_outputEdit->setText ( value );
}

/** Den Dialog starten */
int Webcam::start()
{
  loadDefaults(); // set defaults
  return exec();
}

const QString Webcam::data()
{
  setSettings ( "Device", p_v4lDevice );
  setSettings ( "Overlay", p_Overlay );
  setSettings ( "PositionType", m_setOverlayComboBox->currentIndex() );
  setSettings ( "Indent_X", p_MarginX );
  setSettings ( "Indent_Y", p_MarginY );
  QSize scale = m_webcamPreview->itemSize().toSize();
  setSettings ( "Scale", qMax ( scale.width(), scale.height() ) );
  qDebug() << Q_FUNC_INFO << "TODO>>" << m_outputEdit->text ();
  return QString();
}

Webcam::~Webcam()
{}
