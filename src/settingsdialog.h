/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
* Copyright: See COPYING file that comes with this distribution
**/

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

/* QtCore */
#include <QtCore/QObject>

/* QtGui */
#include <QtGui/QDialog>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

class Settings;
class Defaults;
class MetaData;
class TableEditor;

class SettingsDialog : public QDialog
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Juergen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    Settings* cfg;
    Defaults* m_defaults;
    MetaData* m_metaData;
    TableEditor* m_videoEditor;
    TableEditor* m_audioEditor;
    QTextEdit* commandLineEdit;

  private Q_SLOTS:
    void loadSettings();
    void saveSettings();

  public:
    SettingsDialog ( QWidget * parent = 0, Settings * settings = 0 );
    ~SettingsDialog();

};

#endif
