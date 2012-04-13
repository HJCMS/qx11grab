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

#ifndef PRESETSCOMBOBOX_H
#define PRESETSCOMBOBOX_H

/* QtCore */
#include <QtCore/QFileInfo>
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QComboBox>
#include <QtGui/QWidget>
#include <QtGui/QIcon>

/**
* \class PresetsComboBox
* Combo Selection for all (User/System) ffpresets
*/
class PresetsComboBox : public QComboBox
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    const QIcon p_icon;
    void insertItemData ( const QFileInfo &info );
    void load();

  private Q_SLOTS:
    void indexChanged ( int );

  Q_SIGNALS:
    /** this signal will emitted when a readable preset has changed */
    void fileChanged ( const QString & );

  public Q_SLOTS:
    void reload();

  public:
    explicit PresetsComboBox ( QWidget * parent = 0 );
    virtual ~PresetsComboBox();
};

#endif
