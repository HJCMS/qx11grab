/*
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
*/

#include "defaultedit.h"
#include "optionsfinder.h"

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QRegExp>

/* QX11Grab */
#include "avoptions.h"

DefaultEdit::DefaultEdit ( QWidget * parent )
    : AbstractEdit ( parent )
{
  setObjectName ( QLatin1String ( "DefaultEdit" ) );
}

void DefaultEdit::setCodecOptions ( const QString &param )
{
  QString buffer ( param );
  QRegExp pattern ( "((^\\-{1,2})|(:\\w{1}$))" );
  QByteArray query = buffer.remove ( pattern ).toUtf8();

  QList<QX11Grab::FFOption> opts = QX11Grab::AVOptions::optionQuery ( query );
  if ( opts.size() > 0 )
  {
    setToolTip ( opts.first().help );
    setCompleters ( opts );
  }
}

void DefaultEdit::setCompleterId ( const QString &id )
{
  // Siehe "avoptions.h"
  if ( id.compare ( "-pix_fmt" ) == 0 )
  {
    setCompleters ( QX11Grab::AVOptions::pixelFormats() );
    return;
  }
  else if ( id.compare ( "-sample_fmt" ) == 0 )
  {
    setCompleters ( QX11Grab::AVOptions::sampleFormats() );
    return;
  }
  else if ( id.contains ( QRegExp ( "^\\-{1,2}\\b" ) ) )
  {
    setCodecOptions ( id );
    return;
  }
}

DefaultEdit::~DefaultEdit()
{}
