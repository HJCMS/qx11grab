/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2008
* Copyright: See COPYING file that comes with this distribution
**/

#ifndef QX11GRAB_H
#define QX11GRAB_H

#include <QtCore/QObject>
#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <QtGui/QSystemTrayIcon>

#include "version.h"

#include "ui_qx11grabmain.h"

class QX11Grab : public QMainWindow
  , protected Ui::QX11GrabMain
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Juergen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  public:
    QX11Grab();
    ~QX11Grab();

};

#endif
