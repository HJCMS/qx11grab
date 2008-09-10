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

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <QtGui/QSystemTrayIcon>

#include "version.h"
#include "qx11grab.h"

#ifdef Q_OS_UNIX
#include <QX11Info>
#endif

int main(int argc, char *argv[])
{
  Q_INIT_RESOURCE(qx11grab);

  QT_REQUIRE_VERSION ( argc, argv, "4.4.0" )
  QApplication app ( argc, argv, true );

  if ( ! QSystemTrayIcon::isSystemTrayAvailable() )
  {
    QMessageBox::critical( 0, "Systray", "I couldn't detect any system tray." );
    return EXIT_FAILURE;
  }
  QApplication::setQuitOnLastWindowClosed(false);

  /* FIXME Windoof mal wieder :-( */
  QTextCodec::setCodecForLocale ( QTextCodec::codecForName ( "UTF-8" ) );

  QTranslator translator ( 0 );
  translator.load ( QString ( "qx11grab_%1" ).arg ( QLocale().name() ) );
  app.installTranslator ( &translator );

  QX11Grab grab;
  grab.show();
  return app.exec();
}
