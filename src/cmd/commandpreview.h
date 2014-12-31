/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann http://qx11grab.hjcms.de, (C) 2007-2015
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

#ifndef COMMANDPREVIEW_H
#define COMMANDPREVIEW_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QLineEdit>
#include <QtGui/QWidget>

class CommandLineEdit;

class CommandPreview : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )

  private:
    CommandLineEdit* m_commandLineEdit;

  public Q_SLOTS:
    /** update Commandline Configuration */
    void setCommandLine ( QStringList & );

  Q_SIGNALS:
    /** reset all Items and read from Configuration */
    void restoreRequest();

    /** saved Commandline Configuration */
    void dataSaved ( const QStringList & );

  public:
    /** TopLevel Commandline Viewer Container */
    CommandPreview ( QWidget * parent = 0 );

    /** get Users Modified Commandline */
    const QStringList currentCommandLine();

    ~CommandPreview();
};

#endif
