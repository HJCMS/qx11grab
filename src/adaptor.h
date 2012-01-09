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

#ifndef ADAPTOR_H
#define ADAPTOR_H

#include <QtCore/QObject>
#include <QtCore/QString>

#include <QtDBus/QDBusAbstractAdaptor>

#ifndef QX11GRAB_VERSION
# include "version.h"
#endif

class Adaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )
    Q_CLASSINFO ( "D-Bus Interface", "de.hjcms.qx11grab" )

  public:
    Adaptor ( QObject *parent = 0 );
    ~Adaptor();

  public Q_SLOTS:
    Q_NOREPLY void rubberband ();
    Q_NOREPLY void start ();
    Q_NOREPLY void stop ();
    Q_NOREPLY void show ();
    Q_NOREPLY void hide ();
    Q_NOREPLY void message ( const QString &mess );
    const QString audiocodec();
    const QString videocodec();
    const QString commandline();
    const QString output();
};

#endif
