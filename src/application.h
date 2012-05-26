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
**/

#ifndef QX11GRAB_MAIN_H
#define QX11GRAB_MAIN_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QEvent>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QSessionManager>

/* QtDBus */
#include <QtDBus/QDBusConnection>

/* QX11Grab */
class Settings;
class MainWindow;

/**
* \class Application
* \code
* #include <cstdlib>
* #include <QtCore/QDebug>
* #include "application.h"
*
* int main ( int argc, char* argv[] )
* {
*   Application* app = new Application ( argc, argv );
*   if ( ! app->start() )
*   {
*     qWarning ( "Application already Running" );
*     delete app;
*     return EXIT_SUCCESS;
*   }
*   // ...
*   app->createWindow();
*   return app->exec();
* \endcode
*/
class Application : public QApplication
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    bool connected;
    Settings* m_settings;
    QDBusConnection* m_dbus;
    MainWindow* m_window;

  private Q_SLOTS:
    virtual void commitData ( QSessionManager &manager );

  public:
    explicit Application ( int &argc, char **argv );
    /** Start Application */
    bool start();
    /** If Application connected open Window */
    void createWindow();
    virtual ~Application();
};

#endif
