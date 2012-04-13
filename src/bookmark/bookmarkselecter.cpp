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

#include "bookmarkselecter.h"
#include "bookmark.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QIcon>

/* QtDBus */
#include <QtDBus/QDBusConnection>

BookmarkSelecter::BookmarkSelecter ( QWidget * parent )
    : QComboBox ( parent )
    , dbusPath ( "/BookmarkSelect" )
{
  setObjectName ( QLatin1String ( "BookmarkSelecter" ) );
  QDBusConnection p_dbus = QDBusConnection::sessionBus();
  p_dbus.registerObject ( dbusPath, this, ( QDBusConnection::ExportScriptableSlots ) );
  reload();
  connect ( this, SIGNAL ( currentIndexChanged ( int ) ),
            this, SLOT ( textChanged ( int ) ) );
}

void BookmarkSelecter::textChanged ( int index )
{
  if ( itemData ( index, Qt::UserRole ).toBool() )
  {
    emit openBookmark ( itemData ( index, Qt::DisplayRole ).toString() );
    blockSignals ( true );
    setCurrentIndex ( 0 );
    blockSignals ( false );
  }
}

void BookmarkSelecter::reload()
{
  clear();
  addItem ( QIcon::fromTheme ( "bookmark-toolbar" ), trUtf8 ( "Bookmark" ), false );
  setEnabled ( false );

  Bookmark xml;
  if ( xml.open() )
  {
    foreach ( QString item, xml.entries() )
    {
      addItem ( QIcon::fromTheme ( "bookmarks" ), item, true );
    }
    setEnabled ( true );
  }
}

BookmarkSelecter::~BookmarkSelecter()
{
  QDBusConnection::sessionBus().unregisterObject ( dbusPath, QDBusConnection::UnregisterNode );
}
