/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann http://qx11grab.hjcms.de, (C) 2007-2013
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

  private:
    void setOptions();
    void setItemsFlags ();
    void insertCustomItem ( const QString &data );

  private Q_SLOTS:
    void createCustomItem();
    void clipper();
    void remove();
    void save();

  protected:
    /** custom Context Menu */
    void contextMenuEvent ( QContextMenuEvent * );

  Q_SIGNALS:
    /** reset all Items and read from Configuration */
    void restoreRequest();

    /** save Commandline into Configuration */
    void dataSaved ( const QStringList & );

  public:
    /** Commandline Viewer */
    CommandLineEdit ( QWidget * parent = 0 );

    /** get commandline without binary */
    const QStringList data ();

    /** Create Command List entries */
    void setData ( const QStringList & );

    ~CommandLineEdit();
};

#endif
