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

#include "watermark.h"
#include "imagepreview.h"
#include "openimagedialog.h"

/* QtCore */
#include <QtCore/QDir>
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFileDialog>
#include <QtGui/QFormLayout>
#include <QtGui/QLabel>
#include <QtGui/QIcon>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

/* QtDBus */
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>

Watermark::Watermark ( QWidget * parent )
    : QDialog ( parent )
{
  setObjectName ( QLatin1String ( "Watermark" ) );
  setWindowTitle ( trUtf8 ( "Watermark Filter" ) );
  setWindowIcon ( QIcon::fromTheme ( "preferences-plugin" ) );
  setSizeGripEnabled ( true );
  setMinimumSize ( 100, 100 );

  cfg = new QSettings ( QSettings::NativeFormat, QSettings::UserScope, "hjcms.de", "qx11grab", this );

  QFormLayout* layout = new QFormLayout ( this );
  layout->setObjectName ( QLatin1String ( "watermark/Layout" ) );

  m_imagePreview = new ImagePreview ( this );
  layout->addRow ( m_imagePreview  );

  m_xIndent = new QSlider ( Qt::Horizontal, this );
  m_xIndent->setRange ( 5, 100 );
  layout->addRow ( trUtf8( "Margin (Left|Right)" ), m_xIndent );

  m_yIndent = new QSlider ( Qt::Horizontal, this );
  m_yIndent->setRange ( 5, 100 );
  layout->addRow ( trUtf8( "Margin (Top|Bottom)" ), m_yIndent );

  QToolButton* btn = new QToolButton ( this );
  btn->setIcon ( QIcon::fromTheme ( "folder-open" ) );
  layout->addRow ( trUtf8( "Open Image file" ), btn );

  int i = 0;
  QIcon scIcon = QIcon::fromTheme ( "video-display" );
  m_positionComboBox = new QComboBox ( this );
  m_positionComboBox->insertItem ( i++, scIcon,  trUtf8 ( "Top Left" ), TOP_LEFT_CORNER );
  m_positionComboBox->insertItem ( i++, scIcon,  trUtf8 ( "Top Right" ), TOP_RIGHT_CORNER );
  m_positionComboBox->insertItem ( i++, scIcon,  trUtf8 ( "Bottom Left" ), BOTTOM_LEFT_CORNER );
  m_positionComboBox->insertItem ( i++, scIcon,  trUtf8 ( "Bottom Right" ), BOTTOM_RIGHT_CORNER );
  layout->addRow ( trUtf8( "Position" ), m_positionComboBox );

  m_outputEdit = new QLineEdit ( this );
  layout->addRow ( m_outputEdit );

  QDialogButtonBox* m_buttonBox = new QDialogButtonBox ( Qt::Horizontal, this );
  m_buttonBox->setObjectName ( QLatin1String ( "watermark/ButtonBox" ) );
  m_buttonBox->setStandardButtons ( ( QDialogButtonBox::Save | QDialogButtonBox::Cancel ) );
  layout->addRow ( m_buttonBox );

  setLayout ( layout );

  connect ( m_positionComboBox, SIGNAL ( currentIndexChanged ( int ) ),
            this, SLOT ( positionIndexChanged ( int ) ) );

  connect ( m_xIndent, SIGNAL ( valueChanged ( int ) ),
            this, SLOT ( setMarginX ( int ) ) );

  connect ( m_yIndent, SIGNAL ( valueChanged ( int ) ),
            this, SLOT ( setMarginY ( int ) ) );

  connect ( btn, SIGNAL ( clicked() ),
            this, SLOT ( openPixmapDialog() ) );

  connect ( m_buttonBox, SIGNAL ( accepted () ),
            this, SLOT ( accept() ) );

  connect ( m_buttonBox, SIGNAL ( rejected () ),
            this, SLOT ( reject() ) );

  loadDefaults(); // set defaults
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
  p_image = settingsValue ( "File", "/usr/share/icons/hicolor/64x64/apps/qx11grab.png" ).toString();
  p_overlay = settingsValue ( "Overlay", "10:10" ).toString();
  indentX = settingsValue ( "Indent_X", 10 ).toUInt();
  indentY = settingsValue ( "Indent_Y", 10 ).toUInt();
  m_positionComboBox->setCurrentIndex ( settingsValue ( "PositionType", 0 ).toUInt() );
  m_yIndent->setRange ( 10, m_imagePreview->rect().height() );
  m_xIndent->setRange ( 10, m_imagePreview->rect().width() );
  m_imagePreview->setImagePath ( p_image );

  update();
}

/**
* Nachrichten an die Hauptfenster Statusleiste senden!
*/
void Watermark::sendMessage ( const QString &title, const QString &text )
{
  QDBusInterface iface ( "de.hjcms.qx11grab", "/SystemTray", "de.hjcms.qx11grab.SystemTray" );
  iface.call ( "applicationMessage", title, text );
}

void Watermark::positionIndexChanged ( int index )
{
  QString _x = QString::number ( indentX );
  QString _y = QString::number ( indentY );

  switch ( m_positionComboBox->itemData ( index, Qt::UserRole ).toUInt() )
  {
    case TOP_RIGHT_CORNER:
    {
      p_overlay = QString::fromUtf8 ( "main_w-overlay_w-%1:%2" ).arg ( _x, _y );
      m_imagePreview->setAlignment ( ( Qt::AlignTop | Qt::AlignRight ) );
    }
    break;

    case BOTTOM_LEFT_CORNER:
    {
      p_overlay = QString::fromUtf8 ( "%1:main_h-overlay_h-%2" ).arg ( _x, _y );
      m_imagePreview->setAlignment ( ( Qt::AlignBottom | Qt::AlignLeft ) );
    }
    break;

    case BOTTOM_RIGHT_CORNER:
    {
      p_overlay = QString::fromUtf8 ( "main_w-overlay_w-%1:main_h-overlay_h-%2" ).arg ( _x, _y );
      m_imagePreview->setAlignment ( ( Qt::AlignBottom | Qt::AlignRight ) );
    }
    break;

    default:
    {
      p_overlay = QString::fromUtf8 ( "%1:%2" ).arg ( _x, _y );
      m_imagePreview->setAlignment ( ( Qt::AlignTop | Qt::AlignLeft ) );
    }
    break;
  };
  update();
}

void Watermark::openPixmapDialog()
{
  OpenImageDialog* d = new OpenImageDialog ( this );
  connect ( d , SIGNAL ( message ( const QString &, const QString & ) ),
            this, SLOT ( sendMessage ( const QString &, const QString & ) ) );

  if ( d->exec() == QFileDialog::Accepted )
  {
    p_image = d->filePath();
    m_imagePreview->setImagePath ( p_image );
    update();
  }
}

/** links|rechts einrücken */
void Watermark::setMarginX ( int x )
{
  indentX = x;
  positionIndexChanged ( m_positionComboBox->currentIndex() );
}

/** oben|unten einrücken */
void Watermark::setMarginY ( int y )
{
  indentY = y;
  positionIndexChanged ( m_positionComboBox->currentIndex() );
}

void Watermark::update ()
{
  QString value = QString::fromUtf8 ( "movie=%1[watermark],[in][watermark]overlay=%2[out]" )
                  .arg ( p_image, p_overlay );

  m_outputEdit->setText ( value );
}

/** Den Dialog starten */
int Watermark::start()
{
  return exec();
}

const QString Watermark::data()
{
  setSettings ( "File", p_image );
  setSettings ( "Overlay", p_overlay );
  setSettings ( "PositionType", m_positionComboBox->currentIndex() );
  setSettings ( "Indent_X", indentX );
  setSettings ( "Indent_Y", indentY );
  return m_outputEdit->text ();
}

Watermark::~Watermark()
{}

