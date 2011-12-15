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

#include "toolbar.h"
#include "menubar.h"

/* QtCore */
#include <QtCore/QString>

ToolBar::ToolBar ( QX11Grab * parent )
    : QToolBar ( parent )
    , mainWindow ( parent )
{
  setObjectName ( QLatin1String ( "ToolBar" ) );

  QAction* refreh = MenuBar::refreshAction ( this );
  addAction ( refreh );

  m_viewLogAction = MenuBar::viewAction ( this );
  m_viewLogAction->setEnabled ( false );
  addAction ( m_viewLogAction );

  QAction* save = MenuBar::saveAction ( this );
  addAction ( save );

  connect ( refreh, SIGNAL ( triggered() ),
            mainWindow, SLOT ( perparePreview() ) );

  connect ( m_viewLogAction, SIGNAL ( triggered() ),
            mainWindow, SLOT ( openLogFileDialog() ) );

  connect ( save, SIGNAL ( triggered() ),
            mainWindow, SLOT ( saveSettings() ) );
}

void ToolBar::setActionsEnabled ( bool b )
{
  m_viewLogAction->setEnabled ( b );
}

ToolBar::~ToolBar()
{}
