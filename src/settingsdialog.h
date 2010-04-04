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
#include <QtGui/QCloseEvent>

#include "ui_settingsdialogform.h"

class QPushButton;
class Settings;
class QListWidgetItem;

class SettingsDialog : public QDialog
      , public Ui::SettingsDialogForm
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Juergen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    Settings *m_Settings;
    QPushButton *btn_apply, *btn_cancel;

  public:
    SettingsDialog ( QWidget *parent = 0, Settings *cfg = 0 );
    ~SettingsDialog();

  private slots:
    void savepages();
    void changeSettings ( QListWidgetItem *, QListWidgetItem * );

  protected:
    void closeEvent( QCloseEvent * );

};

#endif
