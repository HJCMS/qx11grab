/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2008
* Copyright: See COPYING file that comes with this distribution
**/

#ifndef QX11GRAB_H
#define QX11GRAB_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QRect>

/* QtGui */
#include <QtGui/QMainWindow>
#include <QtGui/QWidget>
#include <QtGui/QStatusBar>
#include <QtGui/QSystemTrayIcon>
#include <QtGui/QShowEvent>
#include <QtGui/QHideEvent>
#include <QtGui/QCloseEvent>

#include "version.h"
#include "ui_qx11grabmain.h"

class QAction;
class QMenu;
class Settings;
class DesktopInfo;
class RubberBand;
class FFProcess;

#ifdef HAVE_DBUS

class QX11GrabAdaptor;

#endif

class QX11Grab : public QMainWindow
      , protected Ui::QX11GrabMain
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Juergen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    int TimeOutMessages;
    Settings *cfg;
    DesktopInfo *m_DesktopInfo;
    RubberBand *m_RubberBand;
    FFProcess *m_FFProcess;
    QMenu *systemTrayMenu;
    QAction *grabActionFromWindow,
    *showRubberbandWindow,
    *stopRecordingWindow,
    *startRecordingWindow,
    *minimizeWindowAction,
    *displayWindowAction,
    *quitWindowAction;
    QSystemTrayIcon *systemTrayIcon;
    const QIcon getIcon ( const QString &name, const QString &group = QString::fromUtf8 ( "/" ) );
    void createActions();
    void createEnviroment();
    void createSystemTrayIcon();
    void loadStats();
    void saveStats();

#ifdef HAVE_DBUS

    QX11GrabAdaptor *m_QX11GrabAdaptor;

#endif

  public:
    QX11Grab( Settings *settings = 0 );
    ~QX11Grab();

#ifdef HAVE_DBUS

  public Q_SLOTS:
    void start_record();
    void stop_record();

#endif

  Q_SIGNALS:
    void startMinimized ();

  private Q_SLOTS:
    void swapRubberBand ();
    void showRubber ( bool );
    void toRubber ( int );
    void grabFromWindow();
    void systemTrayWatcher ( QSystemTrayIcon::ActivationReason );
    void openSettings ();
    void showEvent ( QShowEvent * );
    void hideEvent ( QHideEvent * );
    void closeEvent ( QCloseEvent * );
    void pushInfoMessage ( const QString & );
    void pushErrorMessage ( const QString &, const QString & );
    void pushToolTip ( const QString & );
    void startRecord();
    void setActionsBack();
};

#endif
