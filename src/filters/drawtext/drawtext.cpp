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
#include "fontbox.h"

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QUrl>

/* QtGui */
#include <QtGui/QColor>
#include <QtGui/QColorDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QIcon>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

drawtext::drawtext ( QWidget * parent )
    : QDialog ( parent )
{
  setObjectName ( QLatin1String ( "drawtext" ) );
  setWindowTitle ( trUtf8 ( "drawtext Filter" ) );
  setWindowIcon ( QIcon::fromTheme ( "preferences-plugin" ) );
  setSizeGripEnabled ( true );
  setMinimumSize ( 100, 100 );

  fontcolor = QString ( "#000000" );
  boxcolor = QString ( "#ffffff" );

  QVBoxLayout* layout = new QVBoxLayout ( this );
  layout->setObjectName ( QLatin1String ( "drawtext/Layout" ) );

  QLabel* info0 = new QLabel ( this );
  info0->setText ( trUtf8 ( "Draw text string into output" ) );
  layout->addWidget ( info0 );

  m_fontBox = new FontBox ( this );
  m_fontBox->setPreviewColor ( boxcolor, fontcolor );
  layout->addWidget ( m_fontBox );

  QHBoxLayout* hLayout = new QHBoxLayout();
  hLayout->addStretch ();

  QPushButton* btnForeground = new QPushButton ( trUtf8 ( "Foreground" ), this );
  btnForeground->setIcon ( QIcon::fromTheme ( "preferences-desktop-color" ) );
  /*: ToolTip */
  btnForeground->setToolTip ( trUtf8 ( "Set foreground color dialog" ) );
  hLayout->addWidget ( btnForeground );

  QPushButton* btnBackground = new QPushButton ( trUtf8 ( "Background" ), this );
  btnBackground->setIcon ( QIcon::fromTheme ( "preferences-desktop-color" ) );
  /*: ToolTip */
  btnBackground->setToolTip ( trUtf8 ( "Set background color dialog" ) );
  hLayout->addWidget ( btnBackground );

  layout->addLayout ( hLayout );

  m_lineEditOutput = new QLineEdit ( this );
  m_lineEditOutput->setText ( "drawtext=fontfile=/usr/share/fonts/truetype/FreeSans.ttf:text='Example'" );
  layout->addWidget ( m_lineEditOutput );

  QDialogButtonBox* m_buttonBox = new QDialogButtonBox ( Qt::Horizontal, this );
  m_buttonBox->setObjectName ( QLatin1String ( "drawtext/ButtonBox" ) );
  m_buttonBox->setStandardButtons ( ( QDialogButtonBox::Save | QDialogButtonBox::Cancel ) );
  layout->addWidget ( m_buttonBox );

  setLayout ( layout );

  connect ( btnBackground, SIGNAL ( clicked () ),  this, SLOT ( setBackgroundColor() ) );
  connect ( btnForeground, SIGNAL ( clicked () ),  this, SLOT ( setForegroundColor() ) );
  connect ( m_fontBox, SIGNAL ( updateFont () ),  this, SLOT ( updateFont () ) );
  connect ( m_buttonBox, SIGNAL ( accepted () ), this, SLOT ( accept() ) );
  connect ( m_buttonBox, SIGNAL ( rejected () ), this, SLOT ( reject() ) );
}

void drawtext::openColorChooser ( ColorType type )
{
  QColorDialog* d = new QColorDialog ( this );
  if ( d->exec() == QColorDialog::Accepted )
  {
    QColor color = d->selectedColor();
    switch ( type )
    {
      case BACKGROUND:
        boxcolor = color.name();
        break;

      case FOREGROUND:
        fontcolor = color.name();
        break;

      default:
        break;
    };

    m_fontBox->setPreviewColor ( boxcolor, fontcolor );
    updateFont();
  }
  delete d;
}

void drawtext::setBackgroundColor()
{
  openColorChooser ( BACKGROUND );
}

void drawtext::setForegroundColor()
{
  openColorChooser ( FOREGROUND );
}

void drawtext::updateFont ()
{
  QString color = fontcolor.replace ( "#", "0x" );
  QByteArray path = QUrl::toPercentEncoding ( m_fontBox->path() );
  QByteArray text = QUrl::toPercentEncoding ( m_fontBox->text() );
  QString value = QString ( "drawtext=\"fontfile=%1:text='%2':x=%3:y=%4:fontsize=%5:fontcolor=%6\"" )
                  .arg ( path, // File
                         text, // Text
                         QString::number ( 5 ), // _x
                         QString::number ( 5 ), // _y
                         m_fontBox->size(), // fontsize
                         color
                       );

  m_lineEditOutput->setText ( value );
}

/**
* Erst beim öffnen des Plugins den FontConfig Cache einlesen!
*/
int drawtext::start()
{
  m_fontBox->initFontConfigDatabase();
  return exec();
}

/**
* Parameter Liste
*/
const QString drawtext::value()
{
  return m_lineEditOutput->text();
}

drawtext::~drawtext()
{}

