/**
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2012
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

#ifndef QX11GRAB_INTERFACE_H
#define QX11GRAB_INTERFACE_H

/** QX11GRAB Interface Version */
#ifndef QX11GRAB_VERSION
# define QX11GRAB_VERSION    "@QX11GRAB_VERSION_MAJOR@.@QX11GRAB_VERSION_MINOR@.@QX11GRAB_VERSION_RELEASE@"
#endif

/* QtCore */
#include <QtCore/QMetaObject>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QtPlugin>
#include <QtCore/QUrl>

/* QtGui */
#include <QtGui/QDialog>
#include <QtGui/QWidget>

namespace QX11Grab
{
  /**
  * \class Interface
  * \short qx11grab Plugin Interface Class.
  */
  class Q_DECL_EXPORT Interface : public QObject
  {
      Q_OBJECT
      Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
      Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )
      Q_CLASSINFO ( "Description", "QX11Grab Plugin Interface" )

    public:
      /** Plugin Initialisation */
      virtual bool create ( QWidget * parent ) = 0;

      /** Plugin Dialog Instance */
      virtual bool exec() = 0;

      /** Full Plugin Name */
      virtual const QString pluginName() = 0;

      /** Plugin Filter Title */
      virtual const QString title() = 0;

      /** Short Plugin Filter Description */
      virtual const QString description() = 0;

      /** Generated Filter Argument */
      virtual const QString data() = 0;

  }; /* eof interface */

} /* eof namespace qx11grab */

Q_DECLARE_INTERFACE ( QX11Grab::Interface, "de.hjcms.qx11grab.Interface" )

#endif
