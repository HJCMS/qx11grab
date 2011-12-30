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

#ifndef MESSANGER_H
#define MESSANGER_H

#ifndef QX11GRAB_VERSION
# include "version.h"
#endif

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/* QtDBus */
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>

class Messanger : public QDBusInterface
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  Q_SIGNALS:
    void message ( const QString &mess );

  public:
    typedef struct
    {
      QString fromApp;
      QVariantList contexts;
      QString title;
      QString text;
      QByteArray pixmap;
      QStringList actions;
      qint16 timeout;
      Qt::HANDLE wId; // Window ID
    } Body;

    Messanger ( const QDBusConnection &connection, QObject * parent = 0 );
    void sendMessage ( const QString &txt );
    ~Messanger();
};

namespace de
{
  namespace hjcms
  {
    typedef::Messanger qx11grab;
  }
}

#endif
