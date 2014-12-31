/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann http://qx11grab.hjcms.de, (C) 2007-2015
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
    Q_PROPERTY ( QString dimension READ dimension )
    Q_PROPERTY ( QString acodec READ audiocodec )
    Q_PROPERTY ( QString avcodec READ videocodec )
    Q_PROPERTY ( QString command READ commandline )
    Q_PROPERTY ( QString file READ output )

  public Q_SLOTS:
    /** hide/show rubberband */
    Q_NOREPLY void rubberband ();

    /** start recording  */
    Q_NOREPLY void start ();

    /** stop recording */
    Q_NOREPLY void stop ();

    /** show window */
    Q_NOREPLY void show ();

    /** hide window */
    Q_NOREPLY void hide ();

    /** send text to qx11grab messanger */
    Q_NOREPLY void message ( const QString &mess );

    /** selected codec table editor */
    const QString editorcodec();

    /** get current recording dimension */
    const QString dimension();

    /** get current audio codec name */
    const QString audiocodec();

    /** get current video codec name */
    const QString videocodec();

    /** get current commandline */
    const QString commandline();

    /** get current output filepath */
    const QString output();

  public:
     /** QDBus Adaptor */
    Adaptor ( QObject *parent = 0 );
    ~Adaptor();
};

#endif
