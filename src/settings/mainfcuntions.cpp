/*
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
*/

#include "mainfcuntions.h"

/* QX11Grab */
#include "threadsspinbox.h"
#include "loglevelcombobox.h"
#include "iconthemeselector.h"
#include "graphicsengineselecter.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QFormLayout>
#include <QtGui/QLabel>

MainFunctions::MainFunctions ( QWidget * parent )
    : AbstractConfigWidget ( parent )
{
  setObjectName ( QLatin1String ( "MainFunctions" ) );
  /*: GroupBoxTitle */
  setTitle ( trUtf8 ( "Features" ) );
  setFlat ( true );

  QFormLayout* layout = new QFormLayout ( this );
  layout->setLabelAlignment ( Qt::AlignRight );

  showRubberband = new QCheckBox ( trUtf8 ( "Display Rubberband" ), this );
  /*: ToolTip */
  showRubberband->setToolTip ( trUtf8 ( "enable rubberband at application start" ) );
  /*: WhatsThis */
  showRubberband->setWhatsThis ( trUtf8 ( "always show the rubberband on application start" ) );
  showRubberband->setChecked ( true );
  layout->addRow ( showRubberband );

  startMinimized = new QCheckBox ( trUtf8 ( "Start Minimized" ), this );
  /*: ToolTip */
  startMinimized->setToolTip ( trUtf8 ( "start qx11grab minimized" ) );
  /*: WhatsThis */
  startMinimized->setWhatsThis ( trUtf8 ( "only show qx11grab on start in systray" ) );
  startMinimized->setChecked ( true );
  layout->addRow ( startMinimized );

  expertMode = new QCheckBox ( trUtf8 ( "Expert Mode" ), this );
  /*: ToolTip */
  expertMode->setToolTip ( trUtf8 ( "Enable/Disable expert editor mode" ) );
  /*: WhatsThis */
  expertMode->setWhatsThis ( trUtf8 ( "Enable auto insert my expert settings" ) );
  expertMode->setChecked ( false );
  layout->addRow ( expertMode );

  unsavedChanges = new QCheckBox ( trUtf8 ( "record with unsaved changes" ), this );
  /*: ToolTip */
  unsavedChanges->setToolTip ( trUtf8 ( "force start record with unsaved changes" ) );
  /*: WhatsThis */
  unsavedChanges->setWhatsThis ( trUtf8 ( "start recording if qx11grab have unsaved changes" ) );
  unsavedChanges->setChecked ( false );
  layout->addRow ( unsavedChanges );

  m_threadsSpinBox = new ThreadsSpinBox ( this );
  layout->addRow ( trUtf8 ( "Threads" ), m_threadsSpinBox );

  m_logLevelComboBox = new LogLevelComboBox ( this );
  layout->addRow ( trUtf8 ( "Report Level" ), m_logLevelComboBox );

  m_iconThemeSelector = new IconThemeSelector ( this );
  layout->addRow ( trUtf8 ( "Icon Theme" ), m_iconThemeSelector );

  m_graphicsEngine = new GraphicsEngineSelecter ( this );
  layout->addRow ( trUtf8 ( "Graphics System" ), m_graphicsEngine );

  layout->setVerticalSpacing ( 1 );
  setLayout ( layout );

  connect ( showRubberband, SIGNAL ( clicked ( bool ) ),
            this, SIGNAL ( postUpdate ( bool ) ) );

  connect ( startMinimized, SIGNAL ( clicked ( bool ) ),
            this, SIGNAL ( postUpdate ( bool ) ) );

  connect ( m_iconThemeSelector, SIGNAL ( modified ( bool ) ),
            this, SIGNAL ( postUpdate ( bool ) ) );
}

void MainFunctions::load ( Settings * cfg )
{
  showRubberband->setChecked ( cfg->showRubberOnStart() );
  unsavedChanges->setChecked ( cfg->value ( QLatin1String ( "unsavedChanges" ), false ).toBool() );
  startMinimized->setChecked ( cfg->value ( QLatin1String ( "startMinimized" ), false ).toBool() );
  expertMode->setChecked ( cfg->expertMode() );
  m_threadsSpinBox->setValue ( cfg->value ( QLatin1String ( "Threads" ), 0 ).toUInt() );
  m_logLevelComboBox->setValue ( cfg->logLevel() );
  m_iconThemeSelector->setValue ( cfg->value ( QLatin1String ( "IconTheme" ), "oxygen" ).toString() );
  m_graphicsEngine->setValue ( cfg->value ( QLatin1String ( "GraphicsSystem" ), "native" ).toString() );
}

void MainFunctions::save ( Settings * cfg )
{
  cfg->setValue ( QLatin1String ( "showRubberband" ), showRubberband->isChecked() );
  cfg->setValue ( QLatin1String ( "unsavedChanges" ), unsavedChanges->isChecked() );
  cfg->setValue ( QLatin1String ( "startMinimized" ), startMinimized->isChecked() );
  cfg->setValue ( QLatin1String ( "ExpertMode" ), expertMode->isChecked() );
  cfg->setValue ( QLatin1String ( "Threads" ), m_threadsSpinBox->value() );
  cfg->setLogLevel ( m_logLevelComboBox->value() );
  cfg->setValue ( QLatin1String ( "IconTheme" ), m_iconThemeSelector->value() );
  cfg->setValue ( QLatin1String ( "GraphicsSystem" ), m_graphicsEngine->value() );
}

MainFunctions::~MainFunctions()
{}
