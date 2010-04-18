/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
* Copyright: See COPYING file that comes with this distribution
**/

#include "settingsdialog.h"
#include "settings.h"
#include "defaults.h"
#include "tableeditor.h"
#include "metadata.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QDialogButtonBox>
#include <QtGui/QIcon>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidget>
#include <QtGui/QToolBox>

SettingsDialog::SettingsDialog ( QWidget * parent, Settings * settings )
    : QDialog ( parent )
    , cfg ( settings )
{
  setObjectName ( QLatin1String ( "settingsdialog" ) );
  setWindowTitle ( trUtf8 ( "Settings" ) );
  setMinimumWidth ( 500 );
  setMinimumHeight ( 450 );

  QIcon boxIcon;
  boxIcon.addFile ( QString::fromUtf8 ( ":/images/qx11grab.png" ), QSize(), QIcon::Normal, QIcon::Off );

  QVBoxLayout* verticalLayout = new QVBoxLayout ( this );

  QToolBox* toolBox = new QToolBox ( this, Qt::Widget );
  verticalLayout->addWidget ( toolBox );

  m_defaults = new Defaults ( toolBox );
  toolBox->addItem ( m_defaults, boxIcon, QString::fromUtf8 ( "Default" ) );

  m_metaData = new MetaData ( toolBox );
  m_metaData->setToolTip ( QString::fromUtf8 ( "-metadata" ) );
  toolBox->addItem ( m_metaData, boxIcon, QString::fromUtf8 ( "Metadata" ) );

  m_videoEditor = new TableEditor ( toolBox );
  m_videoEditor->setToolTip ( QString::fromUtf8 ( "-vcodec" ) );
  toolBox->addItem ( m_videoEditor, boxIcon, QString::fromUtf8 ( "Video" ) );

  m_audioEditor = new TableEditor ( toolBox );
  m_audioEditor->setToolTip ( QString::fromUtf8 ( "-acodec" ) );
  toolBox->addItem ( m_audioEditor, boxIcon, QString::fromUtf8 ( "Audio" ) );

  commandLineEdit = new QTextEdit ( this );
  commandLineEdit->setObjectName ( "CommandLine" );
  commandLineEdit->setToolTip ( trUtf8 ( "Display the current FFmpeg command." ) );
  verticalLayout->addWidget ( commandLineEdit );

  QDialogButtonBox* buttonBox = new QDialogButtonBox ( Qt::Horizontal, this );
  QPushButton* btnSave = buttonBox->addButton ( QDialogButtonBox::Save );
  QPushButton* btnClose = buttonBox->addButton ( QDialogButtonBox::Close );
  verticalLayout->addWidget ( buttonBox );

  setLayout ( verticalLayout );

  loadSettings();

  // ButtonBox
  connect ( btnSave, SIGNAL ( clicked() ), this, SLOT ( perparePreview() ) );
  connect ( btnSave, SIGNAL ( clicked() ), this, SLOT ( saveSettings() ) );
  connect ( btnClose, SIGNAL ( clicked() ), this, SLOT ( reject() ) );
}

/**
* Lade beim Start des Dialoges alle Einstellungen.
*/
void SettingsDialog::loadSettings()
{
  m_defaults->load ( cfg );
  m_metaData->load ( cfg );
  m_videoEditor->load ( QString::fromUtf8 ( "VideoOptions" ), cfg );
  m_audioEditor->load ( QString::fromUtf8 ( "AudioOptions" ), cfg );
  perparePreview();
}

/**
* Speichere alle Einstellungen.
*/
void SettingsDialog::saveSettings()
{
  m_defaults->save ( cfg );
  m_metaData->save ( cfg );
  m_videoEditor->save ( QString::fromUtf8 ( "VideoOptions" ), cfg );
  m_audioEditor->save ( QString::fromUtf8 ( "AudioOptions" ), cfg );
}

void SettingsDialog::perparePreview()
{
  commandLineEdit->clear();

  QString cmd ( m_defaults->binary () );
  cmd.append ( QString::fromUtf8 ( " -f x11grab -xerror @X11GRAB_PLACE_HOLDER@ " ) );
  cmd.append ( m_videoEditor->getCmd () );
//   cmd.append ( QString::fromUtf8 ( " " ) );
//   cmd.append ( m_metaData->getCmd () );
  cmd.append ( QString::fromUtf8 ( " " ) );
  cmd.append ( m_audioEditor->getCmd ( m_defaults->ossdevice() ) );
  cmd.append ( QString::fromUtf8 ( " " ) );
  cmd.append ( m_defaults->output() );
  commandLineEdit->setPlainText ( cmd );
}

SettingsDialog::~SettingsDialog()
{}
