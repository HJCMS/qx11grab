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

#ifndef BOOKMARKENTRY_H
#define BOOKMARKENTRY_H

/* QtCore */
#include <QtCore/QGlobalStatic>
#include <QtCore/QHash>
#include <QtCore/QMetaType>
#include <QtCore/QString>
#include <QtCore/QVariant>

/* QtXml */
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

/**
* @class BookmarkEntry
* @short Primary Boomark Entries
* @code
* <entry title="QString">
*   <vcodec id="QString">
*     <param>
*       <argument><![CDATA[QString]]></argument>
*       <value><![CDATA[QString]]></value>
*     </param>
*   </vcodec>
*   <acodec id="QString">
*     <param>
*       <argument><![CDATA[QString]]></argument>
*       <value><![CDATA[QString]]></value>
*     </param>
*   </acodec>
* </entry>
* @endcode
*/
class BookmarkEntry : private QDomElement
{
  private:
    QDomDocument doc;

    bool elementExists ( const QString &nodeName );

    QDomElement vcodecNode();
    void setVCodecChildNodes ( const QDomElement &node );

    QDomElement acodecNode();
    void setACodecChildNodes ( const QDomElement &node );

  protected:
    BookmarkEntry& operator= ( const BookmarkEntry &p );

  public:
    /** which codec node type */
    enum BTYPE { VCODEC, ACODEC };

    /**
    * Initial Bookmark Entry
    * @param entry  document childNode (nodeName=entry with attribute title)
    */
    explicit BookmarkEntry ( const QDomElement &entry );

    /** bookmark has title and codec definitions ? */
    bool isValid();

    /**
    * Read Codec Name e.g. -vcodec or -acodec
    * @param  type  CodecNodeName (vcodec|acodec)
    */
    const QString getCodecName ( BTYPE type );

    /**
    * Set Codec Options e.g. -vcodec or -acodec
    * @note If \ref codecName already exists it will overwriting!
    * @param  type        Codec Type (vcodec|acodec)
    * @param  codecName   Add Options for Codec Name
    * @param  hash        Options
    */
    void setCodecOptions ( BTYPE t, const QString &codecName,
                           const QHash<QString,QVariant> &hash );

    /**
    * read Codec Options e.g. -vcodec or -acodec
    * @param  type        Codec Type (vcodec|acodec)
    */
    const QHash<QString,QVariant> getCodecOptions ( BTYPE t );
};

Q_DECLARE_METATYPE ( BookmarkEntry::BTYPE )

#endif
