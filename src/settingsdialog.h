/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
* Copyright: See COPYING file that comes with this distribution
**/

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

/* QtCore */
#include <QtCore/QObject>

/* QtGui */
#include <QtGui/QWidget>
#include <QtGui/QDialog>

#include "ui_settingsdialogmain.h"

class Settings;

class SettingsDialog : public QDialog
      , public Ui::SettingsDialogMain
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Juergen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    Settings* cfg;

  private Q_SLOTS:
    void loadSettings();
    void saveSettings();
    void setBinaryPath();
    void setOutpuDirectory();

  public:
    SettingsDialog ( QWidget * parent = 0, Settings * settings = 0 );
    ~SettingsDialog();

};

#endif
