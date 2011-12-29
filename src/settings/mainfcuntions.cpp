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

#include "mainfcuntions.h"

/* QX11Grab */
#include "loglevelcombobox.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QFormLayout>
#include <QtGui/QLabel>

MainFunctions::MainFunctions ( QWidget * parent )
    : QGroupBox ( parent )
{
  setObjectName ( QLatin1String ( "MainFunctions" ) );
  setTitle ( trUtf8 ( "Features" ) );

  QFormLayout* layout = new QFormLayout ( this );
  layout->setLabelAlignment ( Qt::AlignRight );

  showRubberband = new QCheckBox ( this );
  /*: ToolTip */
  showRubberband->setToolTip ( trUtf8 ( "enable rubberband at application start" ) );
  /*: WhatsThis */
  showRubberband->setWhatsThis ( trUtf8 ( "always show the rubberband on application start" ) );
  showRubberband->setText ( trUtf8 ( "Display Rubberband" ) );
  showRubberband->setChecked ( true );
  layout->addRow ( showRubberband );

  startMinimized = new QCheckBox ( this );
  /*: WhatsThis */
  startMinimized->setWhatsThis ( trUtf8 ( "start QX11Grab minimized" ) );
  startMinimized->setText ( trUtf8 ( "Start Minimized" ) );
  startMinimized->setChecked ( true );
  layout->addRow ( startMinimized );

  m_logLevelComboBox = new LogLevelComboBox ( this );
  layout->addRow ( trUtf8 ( "Report Level" ), m_logLevelComboBox );

  layout->setVerticalSpacing ( 1 );
  setLayout ( layout );

  connect ( showRubberband, SIGNAL ( clicked ( bool ) ),
            this, SIGNAL ( postUpdate ( bool ) ) );

  connect ( startMinimized, SIGNAL ( clicked ( bool ) ),
            this, SIGNAL ( postUpdate ( bool ) ) );

}

void MainFunctions::load ( Settings * cfg )
{
  showRubberband->setChecked ( cfg->showRubberOnStart() );
  startMinimized->setChecked ( cfg->value ( QLatin1String ( "startMinimized" ) ).toBool() );
  m_logLevelComboBox->setValue ( cfg->logLevel() );
}

void MainFunctions::save ( Settings * cfg )
{
  cfg->setValue ( QLatin1String ( "showRubberband" ), showRubberband->isChecked() );
  cfg->setValue ( QLatin1String ( "startMinimized" ), startMinimized->isChecked() );
  cfg->setLogLevel ( m_logLevelComboBox->value() );
}

MainFunctions::~MainFunctions()
{}
