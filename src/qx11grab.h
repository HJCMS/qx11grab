/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
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
#include <QtGui/QSplitter>
#include <QtGui/QHideEvent>
#include <QtGui/QCloseEvent>

#include "version.h"
#include "ui_qx11grabmain.h"

class QAction;
class QMenu;
class Settings;
class GrabberInfo;
class Defaults;
class MetaData;
class TableEditor;
class DesktopInfo;
class RubberBand;
class FFProcess;
class CommandPreview;

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
    QSplitter* m_splitter;
    GrabberInfo* m_grabberInfo;
    Defaults* m_defaults;
    MetaData* m_metaData;
    TableEditor* m_videoEditor;
    TableEditor* m_audioEditor;
    CommandPreview* m_commandPreview;
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

  private Q_SLOTS:
    void swapRubberBand ();
    void showRubber ( bool );
    void toRubber ( int );
    void grabFromWindow();
    void systemTrayWatcher ( QSystemTrayIcon::ActivationReason );
    void showEvent ( QShowEvent * );
    void hideEvent ( QHideEvent * );
    void closeEvent ( QCloseEvent * );
    void pushInfoMessage ( const QString & );
    void pushErrorMessage ( const QString &, const QString & );
    void pushToolTip ( const QString & );
    void startRecord();
    void setActionsBack();
    void loadSettings();
    void saveSettings();
    void perparePreview();

  Q_SIGNALS:
    void startMinimized ();

#ifdef HAVE_DBUS

  public Q_SLOTS:
    void start_record();
    void stop_record();

#endif

  public:
    QX11Grab ( Settings *settings = 0 );
    ~QX11Grab();

};

#endif
