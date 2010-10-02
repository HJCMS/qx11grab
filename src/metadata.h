/**
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann http://qx11grab.hjcms.de, (C) 2007-2010
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

#ifndef METADATA_H
#define METADATA_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QSettings>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QDateTimeEdit>
#include <QtGui/QLineEdit>
#include <QtGui/QWidget>

class MetaData : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )

  private:
    QLineEdit* metadata_INAM;
    QLineEdit* metadata_IART;
    QLineEdit* metadata_ICOP;
    QLineEdit* metadata_ISBJ;
    QLineEdit* metadata_ICMT;
    QLineEdit* metadata_IMED;
    QDateTimeEdit* metadata_ICRD;

  public Q_SLOTS:
    void load ( QSettings * cfg );
    void save ( QSettings * cfg );

  public:
    MetaData ( QWidget * parent = 0 );
    const QStringList getCmd();
    ~MetaData ();
};

#endif
