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
**/

/* QX11Grab */
#include "version.h"
#include "playeraction.h"
#include "settings.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QProcess>
#include <QtCore/QSettings>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QDesktopServices>
#include <QtGui/QStyleOptionToolButton>
#include <QtGui/QVBoxLayout>

/*
* @class PlayerAction
* Menü für das abspielen von Videoausgabe Dateien
*/
PlayerAction::PlayerAction ( QWidget * parent )
    : QToolButton ( parent )
{
  setObjectName ( QLatin1String ( "PlayerAction" ) );
  setPopupMode ( QToolButton::MenuButtonPopup );
  setToolButtonStyle ( Qt::ToolButtonIconOnly );
  setIcon ( Settings::themeIcon ( "start-here" ) );

  m_menu =  new QMenu ( this );
  setMenu ( m_menu );

  m_signalMapper = new QSignalMapper ( this );
  connect ( m_signalMapper, SIGNAL ( mapped ( const QString & ) ),
            this, SLOT ( playOuputFile ( const QString & ) ) );

  connect ( this, SIGNAL ( clicked () ), this, SLOT ( showMenu () ) );

  searchPlayers();
}

/*
* Vordefinierte Video Abspieler
* Wird im Kontainter kein Eintrag gefunden wird der
* Binärname zurück gegeben.
*/
const QString PlayerAction::predefinedApps ( const QString &txt ) const
{
  QHash<QString,QString> hash;
  //: MenuEntry for "mplayer" http://www.mplayerhq.hu
  hash["mplayer"] = trUtf8 ( "MPlayer" );
  //: MenuEntry for "gmplayer" http://www.mplayerhq.hu
  hash["gmplayer"] = trUtf8 ( "MPlayer GUI" );
  //: MenuEntry for "kmplayer" http://kmplayer.kde.org
  hash["kmplayer"] = trUtf8 ( "KMPlayer" );
  //: MenuEntry for "smplayer" http://smplayer.sourceforge.net
  hash["smplayer"] = trUtf8 ( "SMPlayer" );
  //: MenuEntry for "xine" http://www.xine-project.org
  hash["xine"] = trUtf8 ( "Xine" );
  //: MenuEntry for "vlc" http://www.videolan.org/vlc/
  hash["vlc"] = trUtf8 ( "VLC" );
  //: MenuEntry for "ffplay" http://www.ffmpeg.org
  hash["ffplay"] = trUtf8 ( "FFPlay" );
  //: MenuEntry for "dragon" http://kde.org/applications/multimedia/dragonplayer/
  hash["dragon"] = trUtf8 ( "Dragon Player" );
  //: MenuEntry for "totem" http://projects.gnome.org/totem/
  hash["totem"] = trUtf8 ( "Totem Player" );
  return ( hash[txt].isEmpty() ) ? txt : hash[txt];
}

/*
* Sucht in "~/bin","/usr/bin" und "/usr/local/bin" nach
* Vordefinierten Video Abspielern und setz bei gefunden
* einen Menü Eintrag.
*/
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
      foreach ( QString f, dir.entryList ( nameFilters, QDir::Files, QDir::Size ) )
      {
        QFileInfo info ( dir.path(), f );
        if ( info.exists() )
        {
          QString name = info.baseName();
          QAction* ac = m_menu->addAction ( Settings::themeIcon ( name, "qx11grab" ), predefinedApps ( name ) );
          connect ( ac, SIGNAL ( triggered() ), m_signalMapper, SLOT ( map() ) );
          m_signalMapper->setMapping ( ac, info.absoluteFilePath() );
        }
      }
    }
  }
}

/*
* Wir muessen wegen Naviagator bei einem Move neu Zeichnen!
*/
void PlayerAction::paintEvent ( QPaintEvent * event )
{
  QStyleOptionToolButton option;
  initStyleOption ( &option );
  QToolButton::paintEvent ( event );
}

/*
* Fragt in den Einstellungen nach der Ausgabe Datei.
* Wenn gefunden wird die abgespielt!
*/
void PlayerAction::playOuputFile ( const QString &player )
{
  Settings settings;
  QFileInfo file ( settings.absoluteOutputPath() );
  if ( file.exists() )
    QProcess::startDetached ( player, QStringList ( file.absoluteFilePath() ) );
}

PlayerAction::~PlayerAction()
{}
