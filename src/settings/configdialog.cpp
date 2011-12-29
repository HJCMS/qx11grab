/**
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann (Undefined) http://qx11grab.hjcms.de, (C) 2007-2012
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

#include "configdialog.h"
#include "targetswidget.h"
#include "mainfcuntions.h"
#include "audiodevicewidget.h"
#include "extraoptions.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QGridLayout>
#include <QtGui/QLayout>
#include <QtGui/QIcon>
#include <QtGui/QSizePolicy>

ConfigDialog::ConfigDialog ( Settings * settings, QWidget * parent )
    : QDialog ( parent )
    , cfg ( settings )
{
  setObjectName ( QLatin1String ( "ConfigDialog" ) );
  /*: WindowTitle */
  setWindowTitle ( trUtf8 ( "Configure[*]" ) );
  setWindowIcon ( QIcon::fromTheme ( "configure" ) );
  setSizeGripEnabled ( true );

  QGridLayout* layout = new QGridLayout ( this );
  layout->setObjectName ( QLatin1String ( "ConfigDialog/Layout" ) );

  m_stackedWidget = new QStackedWidget ( this );
  m_stackedWidget->setObjectName ( QLatin1String ( "ConfigDialog/StackedWidget" ) );
  m_stackedWidget->setSizePolicy ( QSizePolicy::Expanding, QSizePolicy::Expanding );
  layout->addWidget ( m_stackedWidget, 0, 0, 1, 1 );

  m_listWidget = new QListWidget ( this );
  m_listWidget->setObjectName ( QLatin1String ( "ConfigDialog/ListWidget" ) );
  layout->addWidget ( m_listWidget, 0, 1, 1, 1, Qt::AlignRight );

  // Begin: insertItems {
  int index = 0;
  m_mainFunctions = new MainFunctions ( m_stackedWidget );
  m_stackedWidget->insertWidget ( index, m_mainFunctions );
  m_listWidget->insertItem ( index++, trUtf8 ( "Application" ) );

  m_targets = new TargetsWidget ( m_stackedWidget );
  m_stackedWidget->insertWidget ( index, m_targets );
  m_listWidget->insertItem ( index++, trUtf8 ( "Targets" ) );

  m_audioDeviceWidget = new AudioDeviceWidget ( m_stackedWidget );
  m_stackedWidget->insertWidget ( index, m_audioDeviceWidget );
  m_listWidget->insertItem ( index++, trUtf8 ( "Audio" ) );

  m_extraOptions = new ExtraOptions ( m_stackedWidget );
  m_stackedWidget->insertWidget ( index, m_extraOptions );
  m_listWidget->insertItem ( index++, trUtf8 ( "Experts" ) );
  // } End: insertItems

  m_buttonBox = new QDialogButtonBox ( Qt::Horizontal, this );
  m_buttonBox->setObjectName ( QLatin1String ( "ConfigDialog/ButtonBox" ) );
  m_buttonBox->setStandardButtons ( ( QDialogButtonBox::Save | QDialogButtonBox::Cancel ) );
  layout->addWidget ( m_buttonBox, 1, 0, 1, 2 );

  setLayout ( layout );

  connect ( m_listWidget, SIGNAL ( currentRowChanged ( int ) ),
            m_stackedWidget, SLOT ( setCurrentIndex ( int ) ) );

  connect ( m_mainFunctions, SIGNAL ( postUpdate ( bool ) ),
            this, SLOT ( checkDistinctions ( bool ) ) );

  connect ( m_targets, SIGNAL ( postUpdate ( bool ) ),
            this, SLOT ( checkDistinctions ( bool ) ) );

  connect ( m_audioDeviceWidget, SIGNAL ( postUpdate ( bool ) ),
            this, SLOT ( checkDistinctions ( bool ) ) );

  connect ( m_buttonBox, SIGNAL ( accepted () ), this, SLOT ( saveAndExit() ) );
  connect ( m_buttonBox, SIGNAL ( rejected () ), this, SLOT ( reject() ) );

  loadSettings();
}

void ConfigDialog::checkDistinctions ( bool b )
{
  // TODO check Distinctions
  setWindowModified ( b );
}

void ConfigDialog::loadSettings()
{
  // Application {
  m_mainFunctions->load ( cfg );
  // } Application

  // Targets {
  m_targets->load ( cfg );
  // } Targets

  // AudioDeviceWidget {
  m_audioDeviceWidget->setVolume ( cfg->audioVolume() );
  m_audioDeviceWidget->setAudioDevice ( cfg->audioDevice() );
  m_audioDeviceWidget->setAudioEngine ( cfg->audioEngine() );
  m_audioDeviceWidget->setSampleFormat ( cfg->sampleFormat() );
  m_audioDeviceWidget->setAudioServiceType ( cfg->audioType() );
  // } AudioDeviceWidget

  // Experts {
  m_extraOptions->load ( cfg );
  // } Experts

  setWindowModified ( false );
}

void ConfigDialog::saveAndExit()
{
  // Application {
  m_mainFunctions->save ( cfg );
  // } Application

  // Targets {
  m_targets->save ( cfg );
  // } Targets

  // AudioDeviceWidget {
  cfg->setAudioEngine ( m_audioDeviceWidget->getAudioEngine() );
  cfg->setAudioVolume ( m_audioDeviceWidget->getVolume() );
  cfg->setAudioDevice ( m_audioDeviceWidget->getAudioDevice() );
  cfg->setSampleFormat ( m_audioDeviceWidget->getSampleFormat() );
  cfg->setAudioType ( m_audioDeviceWidget->getAudioServiceType() );
  cfg->setAudioDeviceCommand ( m_audioDeviceWidget->data() );
  // } AudioDeviceWidget

  // Experts {
  m_extraOptions->save ( cfg );
  // } Experts

  accept();
}

ConfigDialog::~ConfigDialog()
{}
