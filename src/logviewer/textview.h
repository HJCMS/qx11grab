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

#ifndef TEXTVIEW_H
#define TEXTVIEW_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QContextMenuEvent>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

class Highlighter;

class TextView : public QTextEdit
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )

  private:
    Highlighter* m_highlighter;
    QString source;

  private Q_SLOTS:
    void save ();
    void gotoLine ( int );

  protected:
    /** custom Context Menu */
    void contextMenuEvent ( QContextMenuEvent * );

  Q_SIGNALS:
    /** reload View */
    void refresh();

  public:
    /** Logfile TextView Widget */
    TextView ( QWidget * parent = 0 );

    /** Insert Data into TextView */
    void insertText ( const QString & );

    ~TextView();
};

#endif
