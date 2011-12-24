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

#include "playeraction.h"

/* QX11Grab */
// #include ""

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QProcess>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QDesktopServices>
#include <QtGui/QVBoxLayout>

/* QtDBus */
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusReply>

PlayerAction::PlayerAction ( QWidget * parent )
    : QToolButton ( parent )
    , icon ( QIcon::fromTheme ( "start-here" ) )
{
  setObjectName ( QLatin1String ( "PlayerAction" ) );
  setPopupMode ( QToolButton::MenuButtonPopup );
  setIcon ( icon );

  m_menu =  new QMenu ( this );
  setMenu ( m_menu );

  m_signalMapper = new QSignalMapper ( this );
  connect ( m_signalMapper, SIGNAL ( mapped ( const QString & ) ),
            this, SLOT ( playOuputFile ( const QString & ) ) );

  searchPlayers();
}

const QString PlayerAction::ucFirst ( const QString &txt ) const
{
  QString buffer ( txt );
  QString up = buffer.toUpper().left ( 1 );
  QString no = buffer.right ( ( txt.length() - 1 ) );
  return QString ( "%1%2" ).arg ( up, no );
}

void PlayerAction::searchPlayers()
{
  QStringList nameFilters;
  nameFilters << "*mplayer" << "xine" << "vlc" << "ffplay" << "dragon" << "kplayer";
  QByteArray env = qgetenv ( "PATH" );
  if ( env.contains ( "bin" ) )
  {
    QStringList paths;
    paths << "~/bin" << "/usr/bin" << "/usr/local/bin";
    QDir dir ( "/usr" );
    for ( int d = 0; d < paths.size(); ++d )
    {
      dir.setPath ( paths.at ( d ) );
      foreach ( QString f, dir.entryList ( nameFilters, QDir::Files, QDir::Name ) )
      {
        QFileInfo info ( dir.path(), f );
        if ( info.exists() )
        {
          QString name = info.baseName();
          QAction* ac = m_menu->addAction ( QIcon::fromTheme ( name, icon ), ucFirst ( name ) );
          connect ( ac, SIGNAL ( triggered() ), m_signalMapper, SLOT ( map() ) );
          m_signalMapper->setMapping ( ac, info.absoluteFilePath() );
        }
      }
    }
  }
}

void PlayerAction::playOuputFile ( const QString &player )
{
  QDBusInterface iface ( "de.hjcms.qx11grab", "/", "de.hjcms.qx11grab" );
  QDBusReply<QString> reply = iface.call ( "getOutputFile" );
  if ( reply.isValid() )
  {
    QFileInfo file ( reply.value() );
    if ( file.exists() )
      QProcess::startDetached ( player, QStringList ( file.absoluteFilePath() ) );
  }
}

PlayerAction::~PlayerAction()
{}
