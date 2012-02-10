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

#include "filtermenu.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QVBoxLayout>

/* QtDBus */
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>

FilterMenu::FilterMenu ( QWidget * parent )
    : QMenu ( trUtf8 ( "Create Filter" ), parent )
    , m_plugins ( 0 )
    , p_icon ( QIcon::fromTheme ( QLatin1String ( "preferences-plugin" ) ) )
{
  setObjectName ( QLatin1String ( "FilterMenu" ) );
  setIcon ( p_icon );

  /*
  * Wir können hier keine Plugin Dialoge aufrufen weil das Menü bei einem
  * triggered wieder vom ContextMenu zerstört wird. Aus diesem Grund senden
  * wir das signal openFilter an die CodecTabelle und fügen eine Neue Zeile ein!
  */
  m_signalMapper = new QSignalMapper ( this );
  connect ( m_signalMapper, SIGNAL ( mapped ( const QString & ) ),
            this, SIGNAL ( openFilter ( const QString & ) ) );

  m_plugins = new QX11Grab::Plugins ( this );
  if ( m_plugins )
    registerFilters();
}

/**
* Filterlisten Einträge erstellen
*/
bool FilterMenu::registerFilters()
{
  bool ok = false;
  foreach ( QX11Grab::Interface* plugin, m_plugins->getAll ( this ) )
  {
    if ( plugin )
    {
      QAction* ac = addAction ( p_icon, plugin->title() );
      ac->setStatusTip ( plugin->description() );
      connect ( ac, SIGNAL ( triggered () ), m_signalMapper, SLOT ( map () ) );
      m_signalMapper->setMapping ( ac, plugin->pluginName() );
      ok = true;
    }
  }
  return ok;
}

FilterMenu::~FilterMenu()
{}
