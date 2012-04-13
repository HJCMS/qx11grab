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

#ifndef QX11GRAB_PLUGINS_H
#define QX11GRAB_PLUGINS_H

/* QtCore */
#include <QtCore/QDir>
#include <QtCore/QGlobalStatic>
#include <QtCore/QList>
#include <QtCore/QMetaObject>
#include <QtCore/QMetaType>
#include <QtCore/QObject>
#include <QtCore/QPluginLoader>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QtPlugin>

/* QtGui */
#include <QtGui/QWidget>

/** Plugins Delimiter */
#ifndef QX11GRAB_DELIMITER
# define QX11GRAB_DELIMITER QLatin1String(",")
#endif

namespace QX11Grab
{
  class Interface;

  class Q_DECL_EXPORT Plugins : public QObject
  {
      Q_OBJECT
      Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
      Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

    private:
      QDir p_dir;

    protected:
      /** complete plugins path list */
      const QStringList files();

    public:
      explicit Plugins ( QObject * parent = 0 );

      /** Search filter plugin by name */
      bool find ( const QString &libName );

      /** get plugins with given name */
      QX11Grab::Interface* get ( const QString &name, QWidget * parent );

      /** read all plugins in a list */
      const QList<QX11Grab::Interface*> getAll ( QWidget * parent );

      ~Plugins();
  };

} /* eof namespace qx11grab */

#endif
