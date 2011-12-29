/**
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann http://qx11grab.hjcms.de, (C) 2007-2012
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

#include "qx11grabadaptor.h"
#include "mainwindow.h"

#include <QtCore/QDebug>
#include <QtCore/QGenericReturnArgument>

QX11GrabAdaptor::QX11GrabAdaptor ( QObject *parent )
    : QDBusAbstractAdaptor ( parent )
{
  setAutoRelaySignals ( false );
}

void QX11GrabAdaptor::rubberband()
{
  QMetaObject::invokeMethod ( parent(), "swapRubberBand" );
}

void QX11GrabAdaptor::show ()
{
  QMetaObject::invokeMethod ( parent(), "showNormal" );
}

void QX11GrabAdaptor::hide ()
{
  QMetaObject::invokeMethod ( parent(), "hide" );
}

void QX11GrabAdaptor::message ( const QString &mess )
{
  QMetaObject::invokeMethod ( parent(), "statusBarMessage", Q_ARG ( QString, mess ), Q_ARG ( int, 10000 ) );
}

const QString QX11GrabAdaptor::getAudioCodec()
{
  QString str;
  QMetaObject::invokeMethod ( parent(), "audioCodec", Q_RETURN_ARG ( QString, str ) );
  return str;
}

const QString QX11GrabAdaptor::getVideoCodec()
{
  QString str;
  QMetaObject::invokeMethod ( parent(), "videoCodec", Q_RETURN_ARG ( QString, str ) );
  return str;
}

const QString QX11GrabAdaptor::getCommand()
{
  QString str;
  QMetaObject::invokeMethod ( parent(), "currentCommandLine", Q_RETURN_ARG ( QString, str ) );
  return str;
}

const QString QX11GrabAdaptor::getOutputFile()
{
  QString str;
  QMetaObject::invokeMethod ( parent(), "outputFile", Q_RETURN_ARG ( QString, str ) );
  return str;
}

QX11GrabAdaptor::~QX11GrabAdaptor()
{}
