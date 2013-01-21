/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann (Undefined) http://qx11grab.hjcms.de, (C) 2007-2013
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

#include "outputedit.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QMargins>
#include <QtCore/QRect>
#include <QtCore/QRegExp>

/* QtGui */
#include <QtGui/QColor>
#include <QtGui/QPalette>
#include <QtGui/QPainter>
#include <QtGui/QPen>
#include <QtGui/QStyle>
#include <QtGui/QStyleOptionFrameV3>

OutputEdit::OutputEdit ( QWidget * parent )
    : QLineEdit ( parent )
    , isValidInput ( true )
{
  setObjectName ( QLatin1String ( "OutputEdit" ) );
  setText ( QString::fromUtf8 ( "qx11grab-XXXXXX" ) );
  connect ( this, SIGNAL ( textChanged ( const QString & ) ),
            this, SLOT ( validateInput ( const QString & ) ) );

}

/**
* Suche nach dem Platzhalter für das Generieren der
* Ausgabe Datei und setzte bei falscher Eingabemaske
* die Variable \ref isValidInput auf false.
* \see MainWindow::generateOutputFile
*/
void OutputEdit::validateInput ( const QString &txt )
{
  if ( txt.contains ( "X" ) )
    isValidInput = txt.contains ( QRegExp ( "\\b[X]{6}\\b" ) );
  else
    isValidInput = true;

  emit postUpdate ( isValidInput );
}

/**
* Zeichne einen Roten Rahmen um den Text wenn die Eingabe ungültig ist.
*/
void OutputEdit::paintEvent ( QPaintEvent * event )
{
  QLineEdit::paintEvent ( event );
  if ( !isValidInput && ! ( text().isEmpty() ) )
  {
    QStyleOptionFrameV3 panel;
    initStyleOption ( &panel );
    QRect textRect = style()->subElementRect ( QStyle::SE_LineEditContents, &panel, this );
    QPainter painter ( this );
    painter.setPen ( Qt::red );
    painter.drawRect ( textRect );
  }
}

OutputEdit::~OutputEdit()
{}
