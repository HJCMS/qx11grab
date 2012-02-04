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
#include "openfontdialog.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QIcon>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

drawtext::drawtext ( QWidget * parent )
    : QDialog ( parent )
{
  setObjectName ( QLatin1String ( "drawtext" ) );
  setWindowTitle ( trUtf8 ( "drawtext Filter" ) );
  setWindowIcon ( QIcon::fromTheme ( "preferences-plugin" ) );
  setSizeGripEnabled ( true );
  setMinimumSize ( 100, 100 );

  int grow = 0;
  QGridLayout* layout = new QGridLayout ( this );
  layout->setObjectName ( QLatin1String ( "drawtext/Layout" ) );

  QLabel* info0 = new QLabel ( this );
  info0->setText ( trUtf8 ( "Draw text string into output" ) );
  layout->addWidget ( info0, grow++, 0, 1, 2, Qt::AlignLeft );

  m_lineEditTextFile = new QLineEdit ( this );
  m_lineEditTextFile->setText ( "/usr/share/fonts/truetype/FreeSans.ttf" );
  layout->addWidget ( m_lineEditTextFile, grow, 0, 1, 1 );

  QToolButton* btnFonts = new QToolButton ( this );
  btnFonts->setIcon ( QIcon::fromTheme ( "folder-open" ) );
  layout->addWidget ( btnFonts, grow++, 1, 1, 1 );

  m_lineEditTextLine = new QLineEdit ( this );
  m_lineEditTextLine->setText ( "QX11Grab Demo" );
  layout->addWidget ( m_lineEditTextLine, grow++, 0, 1, 2 );

  layout->rowStretch ( grow++ );

  m_lineEditOutput = new QLineEdit ( this );
  m_lineEditOutput->setText ( "drawtext=fontfile=/usr/share/fonts/truetype/FreeSans.ttf:text='Example'" );
  layout->addWidget ( m_lineEditOutput, grow++, 0, 1, 2 );

  QDialogButtonBox* m_buttonBox = new QDialogButtonBox ( Qt::Horizontal, this );
  m_buttonBox->setObjectName ( QLatin1String ( "drawtext/ButtonBox" ) );
  m_buttonBox->setStandardButtons ( ( QDialogButtonBox::Save | QDialogButtonBox::Cancel ) );
  layout->addWidget ( m_buttonBox, grow++, 0, 1, 2 );

  setLayout ( layout );

  connect ( btnFonts, SIGNAL ( clicked () ), this, SLOT ( openFontFile() ) );
  connect ( m_buttonBox, SIGNAL ( accepted () ), this, SLOT ( accept() ) );
  connect ( m_buttonBox, SIGNAL ( rejected () ), this, SLOT ( reject() ) );
}

void drawtext::openFontFile()
{
  OpenFontDialog* d = new OpenFontDialog ( this );
  if ( d->exec() == QFileDialog::Accepted )
    m_lineEditTextFile->setText ( d->fontPath() );

  delete d;
}

void drawtext::update ()
{
  QString value = QString ( "drawtext=fontfile=%1:text='%2':x=%3:y=%4:fontsize=%5:fontcolor=gray" )
                  .arg ( m_lineEditTextFile->text(), // File
                      m_lineEditTextLine->text(), // Text
                      QString::number ( 5 ), // _x
                      QString::number ( 5 ), // _y
                      QString::number ( 24 ) // fontsize
                  );

  m_lineEditOutput->setText ( value );
}

const QString drawtext::value()
{
  return m_lineEditOutput->text ();
}

drawtext::~drawtext()
{}

