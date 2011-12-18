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

#ifndef QX11GRAB_H
#define QX11GRAB_H

#ifndef QX11GRAB_VERSION
# include "version.h"
#endif

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QRect>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QCloseEvent>
#include <QtGui/QHideEvent>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QPushButton>
#include <QtGui/QShowEvent>
#include <QtGui/QStatusBar>
#include <QtGui/QSystemTrayIcon>
#include <QtGui/QWidget>

class Settings;
class MenuBar;
class ToolBar;
class GrabberInfo;
class Defaults;
class MetaData;
class TableEditor;
class DesktopInfo;
class RubberBand;
class FFProcess;
class CommandPreview;
class SystemTray;

class QX11Grab : public QMainWindow
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Juergen Heinemann (Undefined)" )

  private:
    Settings* cfg;
    FFProcess* m_FFProcess;
    MenuBar* m_menuBar;
    ToolBar* m_toolBar;
    int TimeOutMessages;
    GrabberInfo* m_grabberInfo;
    Defaults* m_defaults;
    MetaData* m_metaData;
    TableEditor* m_videoEditor;
    TableEditor* m_audioEditor;
    CommandPreview* m_commandPreview;
    DesktopInfo* m_DesktopInfo;
    RubberBand* m_RubberBand;
    // SysTray
    SystemTray* m_systemTray;
    // triggred Actions
    void createEnviroment();
    void createSystemTrayIcon();
    void loadStats();
    void saveStats();

  private Q_SLOTS:
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
    void updateCommandLine ( const QStringList & );
    void startRecord();
    void setActionsBack();
    void loadSettings();
    void saveSettings();
    void openLogFileDialog();
    void perparePreview();
    void exportCommand();
    void openBookmarkEditor();

  Q_SIGNALS:
    void stopRecording();
    void killProcess();
    void startMinimized ();

  public Q_SLOTS:
    void swapRubberBand ();
    void record();
    void stop();
    void statusBarMessage ( const QString &msg, int timeout = 5000 );
    const QString getSettingsValue ( const QString &key );
    const QString currentCommandLine();
    const QString audioCodec();
    const QString videoCodec();

  public:
    QX11Grab ( Settings *settings = 0 );
    ~QX11Grab();

};

#endif
