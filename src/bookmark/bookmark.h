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

class Bookmark;

class BookmarkEntry : public QDomElement
{
  private:
    QDomElement vcodecNode;
    QDomElement acodecNode;

  public:
    explicit BookmarkEntry ( Bookmark * doc, const QString &id );
    explicit BookmarkEntry ( const QDomElement &parent );
    void addVCodecs ( const QString &id, const QHash<QString,QVariant> &hash );
    void addACodecs ( const QString &id, const QHash<QString,QVariant> &hash );
};

class Bookmark : public QDomDocument
{
  private:
    Q_DISABLE_COPY ( Bookmark )

  public:
    explicit Bookmark ();
    bool open ();

    /**
    * List with all titled Bookmarks
    */
    const QStringList entries();

    /**
    * Open Bookmark Entry
    * if not exists create a new empty Entry with id
    */
    const BookmarkEntry entry ( const QString &id );

    bool save () const;
    bool save ( QDomDocument * xml ) const;
    ~Bookmark();
};

#endif
