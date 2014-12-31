/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann (Undefined) http://qx11grab.hjcms.de, (C) 2007-2015
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
*/

#include "watermark.h"
#include "imagepreview.h"
#include "openimagedialog.h"

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
#include <QtGui/QImageWriter>
#include <QtGui/QLabel>
#include <QtGui/QIcon>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>
#include <QtGui/QVBoxLayout>

/* QtDBus */
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>

Watermark::Watermark ( QWidget * parent )
    : QDialog ( parent )
    , p_MarginX ( 0 )
    , p_MarginY ( 0 )
{
  setObjectName ( QLatin1String ( "Watermark" ) );
  setWindowTitle ( trUtf8 ( "Watermark Filter" ) );
  setWindowIcon ( QIcon::fromTheme ( "preferences-plugin" ) );
  setSizeGripEnabled ( true );
  setMinimumSize ( 100, 100 );

  cfg = new QSettings ( QSettings::NativeFormat, QSettings::UserScope, "hjcms.de", "qx11grab", this );

  QVBoxLayout* layout = new QVBoxLayout ( this );
  layout->setObjectName ( QLatin1String ( "watermark/Layout" ) );

  m_imagePreview = new ImagePreview ( this );
  layout->addWidget ( m_imagePreview );

  QFormLayout* formLayout = new QFormLayout();
  m_xIndent = new QSlider ( Qt::Horizontal, this );
  m_xIndent->setRange ( 2, 200 );
  m_xIndent->setValue ( 2 );
  formLayout->addRow ( trUtf8 ( "Margin (Left|Right)" ), m_xIndent );

  m_yIndent = new QSlider ( Qt::Horizontal, this );
  m_yIndent->setRange ( 2, 200 );
  m_yIndent->setValue ( 2 );
  formLayout->addRow ( trUtf8 ( "Margin (Top|Bottom)" ), m_yIndent );

  QToolButton* btn = new QToolButton ( this );
  btn->setIcon ( QIcon::fromTheme ( "folder-open" ) );
  formLayout->addRow ( trUtf8 ( "Open Image file" ), btn );

  int i = 0;
  QIcon scIcon = QIcon::fromTheme ( "video-display" );
  m_setOverlayComboBox = new QComboBox ( this );
  m_setOverlayComboBox->insertItem ( i++, scIcon,  trUtf8 ( "Top Left" ), TOP_LEFT_CORNER );
  m_setOverlayComboBox->insertItem ( i++, scIcon,  trUtf8 ( "Top Right" ), TOP_RIGHT_CORNER );
  m_setOverlayComboBox->insertItem ( i++, scIcon,  trUtf8 ( "Bottom Left" ), BOTTOM_LEFT_CORNER );
  m_setOverlayComboBox->insertItem ( i++, scIcon,  trUtf8 ( "Bottom Right" ), BOTTOM_RIGHT_CORNER );
  formLayout->addRow ( trUtf8 ( "Position" ), m_setOverlayComboBox );

  // Image Transparency
  m_transparency = new QSlider ( Qt::Horizontal, this );
  m_transparency->setRange ( 10, 100 );
  m_transparency->setValue ( 100 );
  formLayout->addRow ( trUtf8 ( "Transparency" ), m_transparency );

  layout->addLayout ( formLayout );

  m_outputEdit = new QLineEdit ( this );
  layout->addWidget ( m_outputEdit );

  QDialogButtonBox* m_buttonBox = new QDialogButtonBox ( Qt::Horizontal, this );
  m_buttonBox->setObjectName ( QLatin1String ( "watermark/ButtonBox" ) );
  m_buttonBox->setStandardButtons ( ( QDialogButtonBox::Save | QDialogButtonBox::Cancel ) );
  layout->addWidget ( m_buttonBox );

  setLayout ( layout );

  connect ( m_setOverlayComboBox, SIGNAL ( currentIndexChanged ( int ) ),
            this, SLOT ( positionIndexChanged ( int ) ) );

  connect ( m_xIndent, SIGNAL ( valueChanged ( int ) ),
            this, SLOT ( setMarginX ( int ) ) );

  connect ( m_yIndent, SIGNAL ( valueChanged ( int ) ),
            this, SLOT ( setMarginY ( int ) ) );

  connect ( m_transparency, SIGNAL ( valueChanged ( int ) ),
            this, SLOT ( setTransparency ( int ) ) );

  connect ( btn, SIGNAL ( clicked() ),
            this, SLOT ( openPixmapDialog() ) );

  connect ( m_buttonBox, SIGNAL ( accepted () ),
            this, SLOT ( accept() ) );

  connect ( m_buttonBox, SIGNAL ( rejected () ),
            this, SLOT ( reject() ) );
}

void Watermark::setSettings ( const QString &key, const QVariant &value )
{
  QString path = QString::fromUtf8 ( "Filter_Watermark/%1" ).arg ( key );
  cfg->setValue ( path, value );
}

const QVariant Watermark::settingsValue ( const QString &key, const QVariant &defaultValue )
{
  QString path = QString::fromUtf8 ( "Filter_Watermark/%1" ).arg ( key );
  return cfg->value ( path, defaultValue );
}

void Watermark::loadDefaults()
{
  p_Overlay = settingsValue ( "Overlay", "2:2" ).toString();
  // Wir benötigen zuerst ein Bild!!
  p_Image = settingsValue ( "File", "/usr/share/icons/hicolor/64x64/apps/qx11grab.png" ).toString();
  QFileInfo file ( p_Image );
  // Grafik einfügen
  if ( file.isReadable() && m_imagePreview->setImagePath ( p_Image ) )
  {
    // Zum abschluss den Index der Combobox setzen und update auslösen
    m_setOverlayComboBox->setCurrentIndex ( settingsValue ( "PositionType", 0 ).toUInt() );

    // Vorschau bereich auf Slider übertragen
    m_xIndent->setValue ( settingsValue ( "Indent_X", 2 ).toUInt() );
    m_yIndent->setValue ( settingsValue ( "Indent_Y", 2 ).toUInt() );
    m_transparency->setValue ( settingsValue ( "Transparency", 100 ).toUInt() );
  }
}

/**
* Bei einer Transparent das Bild neu erstellen und in
* den Temporären Speicherpfad schreiben!
*/
bool Watermark::rewritePixmap ()
{
  QByteArray user = qgetenv ( "USER" );
  QString p = QString::fromUtf8 ( "%1/qx11grab_overlay_%2.png" ).arg ( QDir::tempPath(), QString ( user ) );
  if ( m_imagePreview->savePixmap ( p ) )
  {
    p_Image = p;
    return true;
  }
  return false;

}

/**
* Nachrichten an die Hauptfenster Statusleiste senden!
*/
void Watermark::sendMessage ( const QString &title, const QString &text )
{
  QDBusInterface iface ( "de.hjcms.qx11grab", "/SystemTray", "de.hjcms.qx11grab.SystemTray" );
  iface.call ( "applicationMessage", title, text );
}

/**
* Wird immer dann aufgerufen wenn sich eine Änderung bei den Einstellungen ergibt!
* Die Position des Bildes muss jedes mal neu gesetzt werden!
* Wir starten immer von oben Links, was zu folge hat das wir bei QPointF,
* im gegensatz zu FFmpeg, im Negativen Bereich arbeiten!
*/
void Watermark::positionIndexChanged ( int index )
{
  // Zeichenketten für FFmpeg overlay
  QString _x = QString::number ( p_MarginX );
  QString _y = QString::number ( p_MarginY );

  // Vorschau Bereich
  qreal main_w = m_imagePreview->rect().width();
  qreal main_h = m_imagePreview->rect().height();

  // Bild Bereich
  qreal overlay_w = m_imagePreview->itemSize().width();
  qreal overlay_h = m_imagePreview->itemSize().height();

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

  // qDebug() << Q_FUNC_INFO << overlayTopLeft << m_imagePreview->itemSize();

  // Bild verschieben
  m_imagePreview->setSceneRect ( QRectF ( overlayTopLeft, m_imagePreview->itemSize() ) );

  // Ausgabe zeile neu Schreiben
  update();
}

void Watermark::openPixmapDialog()
{
  OpenImageDialog* d = new OpenImageDialog ( this );
  connect ( d , SIGNAL ( message ( const QString &, const QString & ) ),
            this, SLOT ( sendMessage ( const QString &, const QString & ) ) );

  if ( d->exec() == QFileDialog::Accepted )
  {
    p_Image = d->filePath();
    m_imagePreview->setImagePath ( p_Image );
    // Zum abschluss den Index der Combobox setzen und update auslösen
    positionIndexChanged ( m_setOverlayComboBox->currentIndex() );
  }
}

/** links|rechts einrücken */
void Watermark::setMarginX ( int x )
{
  if ( ( x % 2 ) == 0 )
  {
    p_MarginX = x;
    positionIndexChanged ( m_setOverlayComboBox->currentIndex() );
  }
}

/** oben|unten einrücken */
void Watermark::setMarginY ( int y )
{
  if ( ( y % 2 ) == 0 )
  {
    p_MarginY = y;
    positionIndexChanged ( m_setOverlayComboBox->currentIndex() );
  }
}

/**
* Bild Transparenz einstellen
*/
void Watermark::setTransparency ( int i )
{
  m_imagePreview->setItemOpacity ( i );
  positionIndexChanged ( m_setOverlayComboBox->currentIndex() );
}

void Watermark::update ()
{
  QString value = QString::fromUtf8 ( "movie=%1[logo],[in][logo]overlay=%2,setpts=PTS-STARTPTS[out]" ).arg ( p_Image, p_Overlay );
  m_outputEdit->setText ( value );
}

void Watermark::resizeEvent ( QResizeEvent * event )
{
  positionIndexChanged ( m_setOverlayComboBox->currentIndex() );
  QDialog::resizeEvent ( event );
}

/** Den Dialog starten */
int Watermark::start()
{
  loadDefaults(); // set defaults
  return exec();
}

const QString Watermark::data()
{
  setSettings ( "File", p_Image );
  /* Wenn die Bild Transparenz gesetzt ist erstelle
  * eine Schatten Kopie und Überschreibe den BildPfad! */
  if ( m_transparency->value() != m_transparency->maximum() )
  {
    if ( rewritePixmap () )
      update();
  }
  setSettings ( "Overlay", p_Overlay );
  setSettings ( "PositionType", m_setOverlayComboBox->currentIndex() );
  setSettings ( "Indent_X", p_MarginX );
  setSettings ( "Indent_Y", p_MarginY );
  setSettings ( "Transparency", m_transparency->value() );
  return m_outputEdit->text ();
}

Watermark::~Watermark()
{}

