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
#include <cstdio>

/* QtCore */
#include <QtCore/QLibraryInfo>
#include <QtCore/QLocale>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QTextCodec>
#include <QtCore/QTranslator>

/* QtGui */
#include <QtGui/QMessageBox>
#include <QtGui/QSystemTrayIcon>

/* QX11Grab */
#include "application.h"

int main ( int argc, char* argv[] )
{
  /* only for debugging
  setenv ( "QT_GRAPHICSSYSTEM", "opengl", 1 );
  QApplication::setDesktopSettingsAware ( false ); */
  QApplication::setColorSpec ( QApplication::CustomColor );
  Application* app = new Application ( argc, argv );
  if ( ! app->start() )
  {
    qWarning ( "QX11Grab already Running" );
    delete app;
    return EXIT_SUCCESS;
  }

  QStringList transpaths ( app->applicationDirPath () );
  transpaths << QLibraryInfo::location ( QLibraryInfo::TranslationsPath );

  QTranslator translator;
  foreach ( QString d, transpaths )
  {
    if ( translator.load ( QString ( "%1/qx11grab_%2" ).arg ( d, QLocale().name() ) ) )
      break;
  }
  QTextCodec::setCodecForLocale ( QTextCodec::codecForName ( "UTF-8" ) );
  app->installTranslator ( &translator );

  if ( ! QSystemTrayIcon::isSystemTrayAvailable() )
  {
    QMessageBox::critical ( 0, "Systray", "I couldn't detect any system tray." );
    return EXIT_FAILURE;
  }

  app->createWindow();
  return app->exec();
}
