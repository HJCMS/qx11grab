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
    Q_ENUMS ( TYPE )

  private:
    QDomElement vcodecNode;
    QDomElement acodecNode;

  public:
    enum TYPE { VCODEC, ACODEC };
    explicit BookmarkEntry ( Bookmark * doc, const QString &id );
    explicit BookmarkEntry ( QDomElement &rootNode );

    /**
    * Read Codec Name e.g. -vcodec or -acodec
    * @param  type  CodecNodeName (vcodec|acodec)
    */
    const QString getCodecName ( TYPE type );

    /**
    * Set Codec Options e.g. -vcodec or -acodec
    * @param  type        Codec Type (vcodec|acodec)
    * @param  codecName   Add Options for Codec Name
    * @param  hash        Options
    */
    void setCodecOptions ( TYPE t, const QString &codecName, const QHash<QString,QVariant> &hash );

    /**
    * read Codec Options e.g. -vcodec or -acodec
    * @param  type        Codec Type (vcodec|acodec)
    */
    const QHash<QString,QVariant> getCodecOptions ( TYPE t );
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
    bool removeEntryById ( const QString &id );

    const QString implode ( const QStringList &data ) const;
    const QStringList explode ( const QString &data ) const;

    bool save () const;
    bool save ( QDomDocument * xml ) const;
    ~Bookmark();
};

#endif
