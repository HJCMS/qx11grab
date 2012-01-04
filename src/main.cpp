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


#include <cstdlib>

/* QtCore */
#include <QtCore/QString>

/* QX11Grab */
#include "application.h"
#include "mainwindow.h"
#include "adaptor.h"

int main ( int argc, char* argv[] )
{
  Application app ( argc, argv );
  MainWindow* window = new  MainWindow ( app.setting() );
  Adaptor* adaptor = new Adaptor ( window );
  app.bus()->registerObject ( QString ( "/" ), window, ( QDBusConnection::ExportAdaptors ) );

  if ( adaptor && app.bus()->isConnected() )
    window->registerMessanger ( app.bus() );

  if ( app.setting()->value ( "startMinimized", false ).toBool() )
    window->hide();
  else
    window->show();

  return app.exec();
}
