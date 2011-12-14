/**
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann http://qx11grab.hjcms.de, (C) 2007-2010
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
#include <QtGui/QCloseEvent>
#include <QtGui/QHideEvent>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QPushButton>
#include <QtGui/QShowEvent>
#include <QtGui/QStatusBar>
#include <QtGui/QSystemTrayIcon>
#include <QtGui/QWidget>

#include "ui_qx11grabmain.h"

class QAction;
class Settings;
class GrabberInfo;
class Defaults;
class MetaData;
class TableEditor;
class DesktopInfo;
class RubberBand;
class FFProcess;
class CommandPreview;

class QX11Grab
      : public QMainWindow
      , protected Ui::QX11GrabMain
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Juergen Heinemann (Undefined)" )

  private:
    Settings *cfg;
    int TimeOutMessages;
    GrabberInfo* m_grabberInfo;
    Defaults* m_defaults;
    MetaData* m_metaData;
    TableEditor* m_videoEditor;
    TableEditor* m_audioEditor;
    CommandPreview* m_commandPreview;
    DesktopInfo *m_DesktopInfo;
    RubberBand *m_RubberBand;
    FFProcess *m_FFProcess;
    QMenu* systemTrayMenu;
    QAction* grabActionFromWindow;
    QAction* showRubberbandWindow;
    QAction* stopRecordingWindow;
    QAction* startRecordingWindow;
    QAction* minimizeWindowAction;
    QAction* displayWindowAction;
    QAction* quitWindowAction;
    QPushButton* logviewBtn;
    QSystemTrayIcon *systemTrayIcon;
    void createActions();
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

  Q_SIGNALS:
    void startMinimized ();

  public Q_SLOTS:
    void swapRubberBand ();
    void record();
    void stop();
    const QString currentCommandLine();
    const QString getSettingsValue ( const QString &key );

  public:
    QX11Grab ( Settings *settings = 0 );
    ~QX11Grab();

};

#endif
