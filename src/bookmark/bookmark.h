/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann (Undefined) http://qx11grab.hjcms.de, (C) 2007-2015
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

#ifndef BOOKMARK_H
#define BOOKMARK_H

/* QtCore */
#include <QtCore/QHash>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/* QtXml */
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

/* QX11Grab */
#include "bookmarkentry.h"

/*
* @class Bookmark
* @short Bookmark Document
* @code
* <?xml version='1.0' encoding='utf-8'?>
* <bookmark>
*   <!-- BookmarkEntries -->
* </bookmark>
* @endcode
*/
class Bookmark : public QDomDocument
{
  private:
    Q_DISABLE_COPY ( Bookmark )

  public:
    explicit Bookmark ();

    /**
    * Open Bookmark Document
    */
    bool open ();

    /**
    * List with all titled Bookmarks
    */
    const QStringList entries();

    /**
    * Open Bookmark Entry with given id
    * if not exists create a new empty Entry with id
    * @param id  e.g. &lt;entry title="id"/&gt; Attributes
    */
    BookmarkEntry entry ( const QString &id );

    /**
    * Remove Bookmark Entry with given id
    * @param id  e.g. &lt;entry title="id"/&gt; Attributes
    */
    bool removeEntryById ( const QString &id );

    /**
    * Save Current Document
    */
    bool save () const;

    /**
    * Override with Document Copy
    */
    bool save ( QDomDocument * xml ) const;

    ~Bookmark();
};

#endif
