/**
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann http://qx11grab.hjcms.de, (C) 2007-2012
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

#ifndef COMMANDLINEEDIT_H
#define COMMANDLINEEDIT_H

#ifndef QX11GRAB_VERSION
# include "version.h"
#endif

/* QtCore */
#include <QtCore/QObject>

/* QtGui */
#include <QtGui/QContextMenuEvent>
#include <QtGui/QListWidget>
#include <QtGui/QWidget>

class CommandLineEdit : public QListWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_PROPERTY ( QStringList data READ data WRITE setData )

  private Q_SLOTS:
    void clipper();
    void remove();
    void save();

  protected:
    void contextMenuEvent ( QContextMenuEvent * );

  Q_SIGNALS:
    void dataSaved ( const QStringList & );

  public:
    CommandLineEdit ( QWidget * parent = 0 );
    const QStringList data ();
    void setData ( const QStringList & );
    virtual ~CommandLineEdit();
};

#endif
