/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann (Undefined) http://qx11grab.hjcms.de, (C) 2007-2015
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

#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

/* QtCore */
#include <QtCore/QEvent>
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QSystemTrayIcon>
#include <QtGui/QWidget>

/* QtDBus */
#include <QtDBus/QDBusConnection>

/* QX11Grab */
#include "mainwindow.h"

class Messanger;
class Navigator;

class SystemTray : public QSystemTrayIcon
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    MainWindow* m_mainWindow;
    Messanger* m_messanger;
    Navigator* m_navigator;
    QAction* m_actionStartRecord;
    QAction* m_actionStopRecord;

  private Q_SLOTS:
    void showNavigator();

  public Q_SLOTS:
    void setActionsEnabled ( bool b = true );
    void setCustomToolTip ( const QString &txt );
    Q_SCRIPTABLE void applicationMessage ( const QString &title, const QString &txt );

  public:
    SystemTray ( MainWindow * parent );
    void setMessanger ( QDBusConnection* bus );
    void sendMessage ( const QString &title, const QString &message, QSystemTrayIcon::MessageIcon icon );
    ~SystemTray();
};

#endif
