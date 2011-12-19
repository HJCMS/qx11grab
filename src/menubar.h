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

#ifndef MENUBAR_H
#define MENUBAR_H

#ifndef QX11GRAB_VERSION
# include "version.h"
#endif

/* QtCore */
#include <QtCore/QObject>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QWidget>

/* QX11Grab */
#include "qx11grab.h"

class MenuBar : public QMenuBar
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    QX11Grab* mainWindow;

  protected:
    // File Menu
    QMenu* m_menuFile;
    QAction* m_actionGrabbing;
    QAction* m_actionStartRecord;
    QAction* m_actionStopRecord;
    QAction* m_actionKillRecord;
    QAction* m_actionMinimize;
    QAction* m_actionQuit;
    // Actions Menu
    QMenu* m_menuActions;
    QAction* m_actionExport;
    QAction* m_actionAddBookmark;
    QAction* m_actionDelBookmark;
    QAction* m_actionSave;
    QAction* m_actionLoad;
    QAction* m_actionRefresh;
    // Bookmark Actions
    QAction* bookmarkCreateAction ( QMenu * parent );
    QAction* bookmarkRemoveAction ( QMenu * parent );

  protected Q_SLOTS:
    void openFFmpegHomepage();
    void openQX11GrabHomepage();

  public Q_SLOTS:
    void setActionsEnabled ( bool b = true );

  public:
    MenuBar ( QX11Grab * parent );

    // this Actions used by some other Classes
    static QAction* quitAction ( QObject * parent, bool shortcut = false );
    static QAction* grabbingAction ( QObject * parent, bool shortcut = false );
    static QAction* rubberbandAction ( QObject * parent, bool shortcut = false );
    static QAction* startRecordAction ( QObject * parent, bool shortcut = false );
    static QAction* stopRecordAction ( QObject * parent, bool shortcut = false );
    static QAction* killRecordAction ( QObject * parent, bool shortcut = false );
    static QAction* hideWindowAction ( QObject * parent, bool shortcut = false );
    static QAction* showWindowAction ( QObject * parent, bool shortcut = false );

    static QAction* exportAction ( QObject * parent, bool shortcut = false );
    static QAction* saveAction ( QObject * parent, bool shortcut = false );
    static QAction* refreshAction ( QObject * parent, bool shortcut = false );
    static QAction* viewAction ( QObject * parent, bool shortcut = false );

    ~MenuBar();
};

#endif
