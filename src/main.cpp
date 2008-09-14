/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2008
* Copyright: See COPYING file that comes with this distribution
**/

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QCoreApplication>
#include <QtCore/QTranslator>
#include <QtCore/QLocale>
#include <QtCore/QTextCodec>
#include <QtCore/QStringList>
#include <QtCore/QLibraryInfo>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <QtGui/QSystemTrayIcon>

#include "version.h"
#include "settings.h"
#include "qx11grab.h"

#ifdef Q_OS_UNIX
#include <QX11Info>
#endif

int main ( int argc, char *argv[] )
{
  Q_INIT_RESOURCE ( qx11grab );

  QT_REQUIRE_VERSION ( argc, argv, "4.4.0" )
  QApplication app ( argc, argv, true );
  app.setApplicationName ( "qx11grab" );
  app.setApplicationVersion ( QX11GRAB_VERSION );
  app.setOrganizationDomain ( "hjcms.de" );

  if ( ! QSystemTrayIcon::isSystemTrayAvailable() )
  {
    QMessageBox::critical ( 0, "Systray", "I couldn't detect any system tray." );
    return EXIT_FAILURE;
  }
  QApplication::setQuitOnLastWindowClosed ( false );
  QTextCodec::setCodecForLocale ( QTextCodec::codecForName ( "UTF-8" ) );

  QStringList transpaths ( QCoreApplication::applicationDirPath () );
  transpaths << QLibraryInfo::location ( QLibraryInfo::TranslationsPath );

  QTranslator translator;
  foreach ( QString d, transpaths )
  {
    if( translator.load ( QString ( "%1/qx11grab_%2" ).arg ( d, QLocale().name() ) ) )
      break;
  }
  app.installTranslator ( &translator );

  Settings *m_Settings = new Settings ( &app );

  QX11Grab grab ( m_Settings );

  if ( m_Settings->getBool ( "startMinimized" ) )
    grab.hide();
  else
    grab.show();

  return app.exec();
}
