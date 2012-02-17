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

#include "formatmenu.h"

/* QtCore */
#include <QtCore/QDebug>

FormatMenu::FormatMenu ( QWidget * parent )
    : QToolButton ( parent )
    , p_Icon ( QIcon::fromTheme ( "menu-video-edit" ) )
    , m_menu ( 0 )
{
  setObjectName ( QLatin1String ( "FormatMenu" ) );
  setPopupMode ( QToolButton::MenuButtonPopup );
  setText ( trUtf8 ( "Video Extension" ) );
  m_menu = new QMenu ( trUtf8 ( "Extensions" ), this );
  setMenu ( m_menu );

  m_signalMapper = new QSignalMapper ( this );
  connect ( m_signalMapper, SIGNAL ( mapped ( const QString & ) ),
            this, SIGNAL ( extensionChanged ( const QString & ) ) );

}

void FormatMenu::updateMenu ( CodecID id )
{
  m_menu->clear();
  p_ActionsList.clear();
  QX11Grab::AVOptions* av = new QX11Grab::AVOptions ( this );
  foreach ( QX11Grab::FFFormat f, av->supportedFormats ( id ) )
  {
    QMenu* m = m_menu->addMenu ( p_Icon, f.description );
    foreach ( QString ext, f.extensions.toStringList() )
    {
      QAction* ac = m->addAction ( p_Icon, ext );
      ac->setCheckable ( true );
      connect ( ac, SIGNAL ( triggered() ), m_signalMapper, SLOT ( map() ) );
      m_signalMapper->setMapping ( ac, ext );
      p_ActionsList.append ( ac );
    }
  }
  delete av;
}

void FormatMenu::setEntryEnabled ( const QString &name )
{
  for ( int i = 0; i < p_ActionsList.size(); ++i )
  {
    // qDebug() << Q_FUNC_INFO << name << p_ActionsList.at ( i )->text();
    if ( p_ActionsList.at ( i )->isChecked() )
      p_ActionsList.at ( i )->setChecked ( false );

    if ( p_ActionsList.at ( i )->text().compare ( name ) == 0 )
    {
      p_ActionsList.at ( i )->setChecked ( true );
    }
  }
}

FormatMenu::~FormatMenu()
{
  if ( m_menu )
    m_menu->clear();
}
