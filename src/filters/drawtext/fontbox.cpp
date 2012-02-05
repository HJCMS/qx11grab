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

#include "fontbox.h"

/* QX11Grab */
#include <fontconfig/fontconfig.h>
#include <fontconfig/fcfreetype.h>

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QMap>

/* QtGui */
#include <QtGui/QVBoxLayout>

FontBox::FontBox ( QWidget * parent )
    : QWidget ( parent )
    , fontIcon ( QIcon::fromTheme ( "addfont" ) )
    , currentFont ( parent->font() )
{
  setObjectName ( QLatin1String ( "FontBox" ) );
  setContentsMargins ( 0, 0, 0, 0 );

  QVBoxLayout* layout = new QVBoxLayout ( this );
  layout->setContentsMargins ( 0, 0, 0, 0 );

  QString startText ( "QX11Grab" );

  m_fontPreview = new FontPreview ( this );
  m_fontPreview->setText ( startText );
  layout->addWidget ( m_fontPreview );

  layout->addStretch ();

  m_fontComboBox = new  QComboBox ( this );
  layout->addWidget ( m_fontComboBox );

  m_lineEdit = new QLineEdit ( this );
  m_lineEdit->setText ( startText );
  layout->addWidget ( m_lineEdit );

  m_sliderSize = new QSlider ( Qt::Horizontal, this );
  m_sliderSize->setTickInterval ( 10 );
  m_sliderSize->setTickPosition ( QSlider::TicksAbove );
  m_sliderSize->setRange ( 5, 200 );
  m_sliderSize->setValue ( 24 );
  layout->addWidget ( m_sliderSize );

  connect ( m_lineEdit, SIGNAL ( editingFinished () ),
            this, SLOT ( updatedView () ) );

  connect ( m_fontComboBox, SIGNAL ( currentIndexChanged ( int ) ),
            this, SLOT ( fontIndexChanged ( int ) ) );

  connect ( m_sliderSize, SIGNAL ( valueChanged ( int ) ),
            this, SLOT ( fontSizeChanged ( int ) ) );

  setLayout ( layout );
}

/**
* Damit QTextEdit die Änderungen bei der Schrift an nimmt
* muss der Text neu geschrieben werden.
* Sende gleichzeitig das signal @ref updateFont an die Unterklassen.
*/
void FontBox::updatedView ()
{
  currentFont.setPointSize ( m_sliderSize->value() );
  m_fontPreview->setFont ( currentFont );
  m_fontPreview->setText ( m_lineEdit->text() );
  m_fontPreview->setToolTip ( currentFont.key() );
  emit updateFont();
}

/**
* Eine neue Schrift wurde ausgewählt
*/
void FontBox::fontIndexChanged ( int index )
{
  fontFilePath = m_fontComboBox->itemData ( index, Qt::UserRole ).toString();
  QFont font ( m_fontComboBox->itemData ( index, Qt::DisplayRole ).toString() );
  if ( font.freetypeFace() )
  {
    currentFont = font;
    updatedView();
  }
}

/** Der Schieber wurde bewegt */
void FontBox::fontSizeChanged ( int )
{
  updatedView();
}

/**
* Weil FFmpeg nur Freetype unterstützt lese den fontconfig Cache ein
* und Suche dort alle ttf Dateien.
* Schreibe sie zum neu sortieren in eine Map und füge die Werte
* ( FC_FAMILY, FC_FILE ) in die ComboBox ein.
*/
void FontBox::initFontConfigDatabase ()
{
  if ( FcInit() )
  {
    FcPattern* fc_pattern = FcPatternCreate ();
    FcObjectSet* fc_object = FcObjectSetBuild ( FC_FAMILY, FC_FILE, NULL );
    if ( fc_pattern && fc_object )
    {
      FcFontSet* fc_fontset = FcFontList ( 0, fc_pattern, fc_object );
      FcObjectSetDestroy ( fc_object );
      FcPatternDestroy ( fc_pattern );
      if ( fc_fontset )
      {
        QMap<QString,QString> map;
        for ( int j = 0; j < fc_fontset->nfont; ++j )
        {
          FcChar8* fc_file;
          if ( FcPatternGetString ( fc_fontset->fonts[j], FC_FILE, 0, &fc_file ) == FcResultMatch )
          {
            QString buf;
            QFileInfo info ( buf.sprintf ( "%s", fc_file ) );
            if ( info.isFile() && info.fileName().contains ( "ttf", Qt::CaseInsensitive ) )
            {
              QString fc_family ( buf.sprintf ( "%s", FcNameUnparse ( fc_fontset->fonts[j] ) ) );
              map.insert ( fc_family, info.absoluteFilePath() );
              // FcPatternPrint ( fc_fontset->fonts[j] );
            }
          }
        }
        FcFontSetDestroy ( fc_fontset );

        QMapIterator<QString,QString> it ( map );
        while ( it.hasNext() )
        {
          it.next();
          m_fontComboBox->addItem ( fontIcon, it.key(), it.value() );
        }
        map.clear();
      }
    }
  }
}

/** Textfareb und Hintergrund setzen */
void FontBox::setPreviewColor ( const QString &bg, const QString &fg )
{
  QString style = QString::fromUtf8 ( "color:%1;background:%2;" ).arg ( fg, bg );
  m_fontPreview->setStyleSheet ( style );
}

/** Aktuell verwendete Schrift */
const QFont FontBox::font()
{
  return currentFont;
}

/** TTF Datei Pfad */
const QString FontBox::path()
{
  if ( fontFilePath.isEmpty() )
    return QLatin1String ( "/usr/share/fonts/truetype/FreeSerif.ttf" );

  return fontFilePath;
}

/** Eingegebene Text */
const QString FontBox::text()
{
  return m_lineEdit->text();
}

const QString FontBox::size()
{
  return QString::number ( currentFont.pointSize() );
}

FontBox::~FontBox()
{}
