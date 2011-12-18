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
#include "bookmarkselecter.h"

/* QtCore */
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QSizePolicy>
#include <QtGui/QHBoxLayout>

ToolBar::ToolBar ( QX11Grab * parent )
    : QToolBar ( parent )
    , mainWindow ( parent )
{
  setObjectName ( QLatin1String ( "ToolBar" ) );
  setAllowedAreas ( ( Qt::TopToolBarArea | Qt::BottomToolBarArea ) );

  QAction* refreh = MenuBar::refreshAction ( this );
  addAction ( refreh );

  m_viewLogAction = MenuBar::viewAction ( this );
  m_viewLogAction->setEnabled ( false );
  addAction ( m_viewLogAction );

  QAction* exp = MenuBar::exportAction ( this );
  addAction ( exp );

  QAction* save = MenuBar::saveAction ( this );
  addAction ( save );

  // Spacer {
  QWidget* m_spacer = new QWidget ( this );
  QHBoxLayout* m_layout = new QHBoxLayout ( m_spacer );
  m_layout->addStretch ( 1 );
  m_spacer->setLayout ( m_layout );
  addWidget ( m_spacer );
  // } Spacer

  m_bookmarkSelecter = new BookmarkSelecter ( this );
  addWidget ( m_bookmarkSelecter );

  connect ( refreh, SIGNAL ( triggered() ),
            mainWindow, SLOT ( perparePreview() ) );

  connect ( m_viewLogAction, SIGNAL ( triggered() ),
            mainWindow, SLOT ( openLogFileDialog() ) );

  connect ( exp, SIGNAL ( triggered() ),
            mainWindow, SLOT ( exportCommand() ) );

  connect ( save, SIGNAL ( triggered() ),
            mainWindow, SLOT ( saveSettings() ) );

  connect ( m_bookmarkSelecter, SIGNAL ( openBookmark ( const QString & ) ),
            mainWindow, SLOT ( openBookmark ( const QString & ) ) );
}

void ToolBar::setActionsEnabled ( bool b )
{
  m_viewLogAction->setEnabled ( b );
}

ToolBar::~ToolBar()
{}
