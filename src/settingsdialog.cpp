/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
* Copyright: See COPYING file that comes with this distribution
**/

#include "settingsdialog.h"
#include "settings.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QPushButton>

SettingsDialog::SettingsDialog ( QWidget *parent, Settings *cfg )
    : QDialog ( parent )
    , m_Settings ( cfg )
{
  setupUi ( this );
}

void SettingsDialog::saveSettings()
{}

void SettingsDialog::closeEvent ( QCloseEvent * )
{
  if ( ! m_Settings->contains ( "qx11grab/options" ) )
    saveSettings();
  else if ( ! m_Settings->contains ( "ffmpeg" ) )
    saveSettings();
}

SettingsDialog::~SettingsDialog()
{}
