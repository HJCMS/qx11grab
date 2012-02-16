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

#ifndef OPTIONSFINDER_H
#define OPTIONSFINDER_H

/* QtCore */
#include <QtCore/QList>
#include <QtCore/QMetaType>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>

/* QtXml */
#include <QtXml/QDomDocument>

typedef struct
{
  QString name;
  QString description;
  bool isDefault;
} VideoExtension;
Q_DECLARE_METATYPE ( VideoExtension )

class OptionsFinder : public QDomDocument
{
  private:
    QString p_codec;
    QString p_template;
    bool p_isOpen;
    QStringList opts;
    void initTemplate();

  public:
    explicit OptionsFinder ( const QString &codec );

    /** options list */
    const QStringList options();

    /** has this option predefined values */
    const QStringList values ( const QString &option );

    /** extensions list */
    const QList<VideoExtension> extensionList();

    ~OptionsFinder();
};

#endif
