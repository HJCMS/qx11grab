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
#include "optionsfinder.h"

/* QtCore */
#include <QtCore/QDebug>

namespace QX11Grab
{
  FormatMenu::FormatMenu ( QWidget * parent )
      : QToolButton ( parent )
      , p_Icon ( QIcon::fromTheme ( "menu-video-edit" ) )
      , m_menu ( 0 )
  {
    setObjectName ( QLatin1String ( "FormatMenu" ) );
    setPopupMode ( QToolButton::MenuButtonPopup );
    setText ( QString::fromUtf8 ( "*.?" ) );
    m_menu = new QMenu ( trUtf8 ( "Extensions" ), this );
    setMenu ( m_menu );

    connect ( this, SIGNAL ( triggered ( QAction * ) ),
              this, SLOT ( itemTriggered ( QAction * ) ) );

    connect ( this, SIGNAL ( clicked () ),
              this, SLOT ( showMenu () ) );
  }

  /** Sucht bei den XML Vorgabe Optionen */
  const QString FormatMenu::findDefaultExtension ( const QString &name )
  {
    QString ext;
    OptionsFinder finder ( name );
    QList<VideoExtension> list = finder.extensionList();
    if ( list.isEmpty() )
      return ext;

    for ( int i = 0; i < list.size(); ++i )
    {
      VideoExtension e = list.at ( i );
      if ( e.isDefault )
        return e.name;
    }

    return ext;
  }

  /** Menü neu einlesen */
  void FormatMenu::updateMenu ( const QString &name, CodecID id )
  {
    m_menu->clear();
    p_ActionsList.clear();

    setText ( QString::fromUtf8 ( "*.?" ) );
    if ( name.isEmpty() )
      return;

    m_menu->setToolTip ( name );
    QX11Grab::AVOptions* av = new QX11Grab::AVOptions ( this );
    foreach ( QX11Grab::FFFormat f, av->supportedFormats ( id ) )
    {
      QMenu* m = m_menu->addMenu ( p_Icon, f.format );
      m->setToolTip ( f.description );
      foreach ( QString ext, f.extensions.toStringList() )
      {
        QAction* ac = m->addAction ( p_Icon, ext );
        ac->setCheckable ( true );
        p_ActionsList.append ( ac );
      }
    }
    delete av;

    QString etr = findDefaultExtension ( name );
    if ( ! etr.isEmpty() )
      setEntryEnabled ( etr );

    emit postUpdate();
  }

  void FormatMenu::itemTriggered ( QAction * ac )
  {
    setText ( QString::fromUtf8 ( "*.%1" ).arg ( ac->text() ) );
    emit extensionChanged ( ac->text() );
  }

  /** Markiert den Ausgwählten Eintrag und ändert den Button Text */
  void FormatMenu::setEntryEnabled ( const QString &name )
  {
    for ( int i = 0; i < p_ActionsList.size(); ++i )
    {
      p_ActionsList.at ( i )->setChecked ( false );
    }

    for ( int i = 0; i < p_ActionsList.size(); ++i )
    {
      if ( p_ActionsList.at ( i )->text().compare ( name ) == 0 )
      {
        p_ActionsList.at ( i )->setChecked ( true );
        setText ( QString::fromUtf8 ( "*.%1" ).arg ( p_ActionsList.at ( i )->text() ) );
        break;
      }
    }
  }

  FormatMenu::~FormatMenu()
  {
    if ( m_menu )
      m_menu->clear();
  }
}  /* eof namespace QX11Grab */
