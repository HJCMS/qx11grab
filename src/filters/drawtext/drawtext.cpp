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

#include "drawtext.h"
#include "fontpreview.h"
#include "dropshadowbox.h"
#include "colorpreview.h"
#include "textposition.h"

/* QX11Grab */
#include <fontconfig/fontconfig.h>
#include <fontconfig/fcfreetype.h>

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QMap>
#include <QtCore/QRegExp>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QBrush>
#include <QtGui/QClipboard>
#include <QtGui/QColor>
#include <QtGui/QColorDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QIcon>
#include <QtGui/QLabel>
#include <QtGui/QPalette>
#include <QtGui/QPushButton>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

drawtext::drawtext ( QWidget * parent )
    : QDialog ( parent )
    , fontIcon ( QIcon::fromTheme ( "addfont" ) )
    , currentFont ( parent->font() )
{
  setObjectName ( QLatin1String ( "drawtext" ) );
  setWindowTitle ( trUtf8 ( "drawtext Filter" ) );
  setWindowIcon ( QIcon::fromTheme ( "preferences-plugin" ) );
  setSizeGripEnabled ( true );
  setMinimumSize ( 100, 100 );

  QString startText = QString::fromUtf8 ( "QX11Grab" );

  QPalette pal = palette();
  fontcolor = QColor ( pal.color ( QPalette::Normal, QPalette::Text ) );
  boxcolor = QColor ( pal.color ( QPalette::Normal, QPalette::Window ) );
  shadowcolor = QColor ( 35, 35, 35 );

  int grow = 0;
  QGridLayout* layout = new QGridLayout ( this );
  layout->setObjectName ( QLatin1String ( "drawtext/Layout" ) );

  QLabel* info0 = new QLabel ( this );
  info0->setText ( trUtf8 ( "Draw text string into output" ) );
  layout->addWidget ( info0, grow++, 0, 1, 2 );

  m_fontPreview = new FontPreview ( this );
  m_fontPreview->setText ( startText );
  layout->addWidget ( m_fontPreview, grow, 0, 1, 1 );

  m_dropShadowBox = new DropShadowBox ( this );
  m_dropShadowBox->setShadowColor ( shadowcolor );
  layout->addWidget ( m_dropShadowBox, grow++, 1, 1, 1, Qt::AlignRight );

  m_fontComboBox = new  QComboBox ( this );
  layout->addWidget ( m_fontComboBox, grow++, 0, 1, 2, Qt::AlignLeft );

  m_textInputEdit = new QLineEdit ( this );
  m_textInputEdit->setText ( startText );
  layout->addWidget ( m_textInputEdit, grow++, 0, 1, 2 );

  m_sliderSize = new QSlider ( Qt::Horizontal, this );
  /*: ToolTip */
  m_sliderSize->setToolTip ( trUtf8 ( "Font Size" ) );
  m_sliderSize->setTickInterval ( 10 );
  m_sliderSize->setTickPosition ( QSlider::TicksAbove );
  m_sliderSize->setRange ( 5, 200 );
  m_sliderSize->setValue ( 24 );
  layout->addWidget ( m_sliderSize, grow++, 0, 1, 2 );

  // HorizontalLayout {
  QHBoxLayout* hLayout = new QHBoxLayout();

  m_foregroundPreview = new ColorPreview ( this );
  m_foregroundPreview->setBackgroundColor ( fontcolor );
  hLayout->addWidget ( m_foregroundPreview );

  QPushButton* btnForeground = new QPushButton ( trUtf8 ( "Foreground" ), this );
  btnForeground->setIcon ( QIcon::fromTheme ( "preferences-desktop-color" ) );
  /*: ToolTip */
  btnForeground->setToolTip ( trUtf8 ( "open foreground color dialog" ) );
  hLayout->addWidget ( btnForeground );

  m_backgroundPreview = new ColorPreview ( this );
  m_backgroundPreview->setBackgroundColor ( boxcolor );
  hLayout->addWidget ( m_backgroundPreview );

  QPushButton* btnBackground = new QPushButton ( trUtf8 ( "Background" ), this );
  btnBackground->setIcon ( QIcon::fromTheme ( "preferences-desktop-color" ) );
  /*: ToolTip */
  btnBackground->setToolTip ( trUtf8 ( "open background color dialog" ) );
  hLayout->addWidget ( btnBackground );

  hLayout->addStretch ();

  m_textPosition = new TextPosition ( this );
  hLayout->addWidget ( m_textPosition );

  layout->addLayout ( hLayout, grow++, 0, 1, 2 );
  // } HorizontalLayout

  m_lineEditOutput = new QLineEdit ( this );
  layout->addWidget ( m_lineEditOutput, grow, 0, 1, 1 );

  QToolButton* btnCopy = new QToolButton ( this );
  btnCopy->setIcon ( QIcon::fromTheme ( "edit-copy" ) );
  /*: ToolTip */
  btnCopy->setToolTip ( trUtf8 ( "Copy to Clipboard" ) );
  layout->addWidget ( btnCopy, grow++, 1, 1, 1 );

  QDialogButtonBox* m_buttonBox = new QDialogButtonBox ( Qt::Horizontal, this );
  m_buttonBox->setObjectName ( QLatin1String ( "drawtext/ButtonBox" ) );
  m_buttonBox->setStandardButtons ( ( QDialogButtonBox::Save | QDialogButtonBox::Cancel ) );
  layout->addWidget ( m_buttonBox, grow++, 0, 1, 2 );

  setLayout ( layout );

  connect ( m_textInputEdit, SIGNAL ( editingFinished () ),
            this, SLOT ( updateFont () ) );

  connect ( m_fontComboBox, SIGNAL ( currentIndexChanged ( int ) ),
            this, SLOT ( fontIndexChanged ( int ) ) );

  connect ( m_sliderSize, SIGNAL ( valueChanged ( int ) ),
            this, SLOT ( fontSizeChanged ( int ) ) );

  connect ( btnBackground, SIGNAL ( clicked () ),
            this, SLOT ( setBackgroundColor() ) );

  connect ( btnForeground, SIGNAL ( clicked () ),
            this, SLOT ( setForegroundColor() ) );

  connect ( m_textPosition, SIGNAL ( postUpdate() ),
            this, SLOT ( updateFont() ) );

  connect ( m_dropShadowBox, SIGNAL ( buttonClicked () ),
            this, SLOT ( setShadowColor() ) );

  connect ( m_dropShadowBox, SIGNAL ( offsetChanged ( int ) ),
            this, SLOT ( setShadowOffset ( int ) ) );

  connect ( m_dropShadowBox, SIGNAL ( alphaChanged ( int ) ),
            this, SLOT ( setShadowAlpha ( int ) ) );

  connect ( btnCopy, SIGNAL ( clicked() ),
            this, SLOT ( clipper() ) );

  connect ( m_buttonBox, SIGNAL ( accepted () ),
            this, SLOT ( accept() ) );

  connect ( m_buttonBox, SIGNAL ( rejected () ),
            this, SLOT ( reject() ) );
}

/**
* Weil FFmpeg nur Freetype unterstützt lese den fontconfig Cache ein
* und Suche dort alle .ttf Dateien.
* Schreibe sie zum neu sortieren in eine Map und füge die Werte
* ( FC_FAMILY, FC_FILE ) in die ComboBox ein.
*/
void drawtext::initFontConfigDatabase ()
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
            if ( info.isFile() && info.fileName().contains ( ".ttf", Qt::CaseInsensitive ) )
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

/** Farben Auswählen */
void drawtext::openColorChooser ( ColorType type )
{
  QColor seleted ( 0, 0, 0 );
  switch ( type )
  {
    case BACKGROUND:
      seleted = m_fontPreview->backgroundColor();
      break;

    case FOREGROUND:
      seleted = m_fontPreview->textColor();
      break;

    case TEXTSHADOW:
      seleted = m_fontPreview->shadowColor();
      break;

    default:
      break;
  };

  QColorDialog* d = new QColorDialog ( seleted, this );
  if ( d->exec() == QColorDialog::Accepted )
  {
    QColor color = d->selectedColor();
    switch ( type )
    {
      case BACKGROUND:
      {
        boxcolor = color;
        m_backgroundPreview->setBackgroundColor ( color );
        m_fontPreview->setBackgroundColor ( color );
      }
      break;

      case FOREGROUND:
      {
        fontcolor = color;
        m_foregroundPreview->setBackgroundColor ( color );
        m_fontPreview->setTextColor ( color );
      }
      break;

      case TEXTSHADOW:
      {
        shadowcolor = color;
        m_dropShadowBox->setShadowColor ( color );
        m_fontPreview->setShadowColor ( color );
      }
      break;

      default:
        break;
    };

    updateFont();
  }
  delete d;
}

/** Eine neue Schrift wurde ausgewählt */
void drawtext::fontIndexChanged ( int index )
{
  fontFilePath = m_fontComboBox->itemData ( index, Qt::UserRole ).toString();
  QFont font ( m_fontComboBox->itemData ( index, Qt::DisplayRole ).toString() );
  if ( font.freetypeFace() )
  {
    currentFont = font;
    updateFont();
  }
}

/** Der Schieber wurde bewegt */
void drawtext::fontSizeChanged ( int )
{
  updateFont();
}

/** Vorschau Hintergrund setzen */
void drawtext::setBackgroundColor()
{
  openColorChooser ( BACKGROUND );
}

/** Textfarbe setzen */
void drawtext::setForegroundColor()
{
  openColorChooser ( FOREGROUND );
}

/** Textschatten Farbe setzen */
void drawtext::setShadowColor()
{
  openColorChooser ( TEXTSHADOW );
}

/** Schatten versatz setzen */
void drawtext::setShadowOffset ( int i )
{
  m_fontPreview->setShadowOffset ( i );
  updateFont();
}

/** Schatten Transparenz setzen */
void drawtext::setShadowAlpha ( int i )
{
  shadowcolor.setAlpha ( i );
  m_fontPreview->setShadowColor ( shadowcolor );
  updateFont();
}

/**
* Alle gesetzten Parameter einlesen und in die Filter Zeichenkette schreiben.
* Damit QTextEdit die Änderungen bei der Schrift an nimmt
* muss der Text neu geschrieben werden.
*/
void drawtext::updateFont ()
{
  currentFont.setPointSize ( m_sliderSize->value() );
  m_fontPreview->setFont ( currentFont );
  m_fontPreview->setText ( m_textInputEdit->text() );
  m_fontPreview->setToolTip ( currentFont.key() );

  QStringList values;
  QString color = fontcolor.name().replace ( "#", "0x" );
  values << QString ( "drawtext=\"fontfile=%1:text='%2':x=%3:y=%4:fontsize=%5:fontcolor=%6" )
  .arg ( fontFilePath, // File
         m_fontPreview->text(), // Text
         m_textPosition->x(), // _x
         m_textPosition->y(), // _y
         QString::number ( currentFont.pointSize() ), // fontsize
         color // Text Color
       );

  QString offsets = QString::number ( m_fontPreview->shadowOffset() );
  color = shadowcolor.name().replace ( "#", "0x" );
  QString alphaBuffer;
  values << QString ( ":shadowx=%1:shadowy=%2:shadowcolor=%3@%4:ft_load_flags=render\"" )
  .arg ( offsets, offsets, color, alphaBuffer.sprintf ( "%.1f", shadowcolor.alphaF() ) );

  m_lineEditOutput->setText ( values.join ( "" ) );
}

/**
* Kopiere den aktuellen Listen Inhalt ins das Clipboard
*/
void drawtext::clipper()
{
  QApplication::clipboard()->setText ( m_lineEditOutput->text() );
}

/** Den FontConfig Cache einlesen und den Dialog starten */
int drawtext::start()
{
  initFontConfigDatabase();
  return exec();
}

/** Aktuelle Parameter Liste */
const QString drawtext::value()
{
  return m_lineEditOutput->text();
}

drawtext::~drawtext()
{}

