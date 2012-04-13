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

#ifndef QX11GRAB_OPTIONSFINDER_H
#define QX11GRAB_OPTIONSFINDER_H

/* QtCore */
#include <QtCore/QList>
#include <QtCore/QMetaType>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>

/* QtXml */
#include <QtXml/QDomDocument>

namespace QX11Grab
{
  typedef struct
  {
    QString name;
    QString description;
    bool isDefault;
  } VideoExtension;

  /**
  * this class read predifined options xml files
  * from {prefix}/usr/share/qx11grab/options/codec.xml
  */
  class OptionsFinder : public QDomDocument
  {
    private:
      QString p_codec;
      QString p_template;
      bool p_isOpen;
      QStringList opts;
      void initTemplate();

    public:
      /** \param codec Codec Name without .xml extension */
      explicit OptionsFinder ( const QString &codec );

      /** options list */
      const QStringList options();

      /** has this option predefined values */
      const QStringList values ( const QString &option );

      /** extensions list */
      const QList<VideoExtension> extensionList();

      ~OptionsFinder();
  };
}  /* eof namespace QX11Grab */

Q_DECLARE_METATYPE ( QX11Grab::VideoExtension )

#endif
