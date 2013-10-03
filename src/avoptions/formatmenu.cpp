/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann (Undefined) http://qx11grab.hjcms.de, (C) 2007-2013
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

#include "formatmenu.h"
#include "optionsfinder.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QSettings>

namespace QX11Grab
{
  /**
  * Diese Menü Button Klasse dient zum auffinden von
  * Erweiterungen des ausgewählten Kodierers und wird
  * ausschließlich von \ref TableEditor für Video
  * Kodierer verwendet.
  * Primäre Aufgabe ist es dem Benutzer ein Menü an zu bieten
  * womit er die Erweiterung des Ausgabe Videos ändern kann.
  */
  FormatMenu::FormatMenu ( QWidget * parent )
      : QToolButton ( parent )
      , p_Icon ( QIcon::fromTheme ( "menu-video-edit" ) )
      , m_menu ( 0 )
  {
    setObjectName ( QLatin1String ( "FormatMenu" ) );
    setPopupMode ( QToolButton::MenuButtonPopup );
    setText ( QString::fromUtf8 ( "*.?" ) );

    // Standard Menü Initialisieren
    m_menu = new QMenu ( trUtf8 ( "Extensions" ), this );
    setMenu ( m_menu );

    // Eintrag wurde ausgewählt
    connect ( this, SIGNAL ( triggered ( QAction * ) ),
              this, SLOT ( itemTriggered ( QAction * ) ) );

    // Bei einem Button Klick das Menü anzeigen
    connect ( this, SIGNAL ( clicked () ),
              this, SLOT ( showMenu () ) );
  }

  /**
  * Suche den Kodierer zuerst in den Einstellungen des Benutzers!
  */
  const QString FormatMenu::findSettingsExtension ( const QString &name )
  {
    QString ext;
    QSettings cfg ( QSettings::NativeFormat, QSettings::UserScope, "hjcms.de", "qx11grab" );
    if ( cfg.contains ( "CodecExtensions/size" ) )
    {
      int size = cfg.beginReadArray ( "CodecExtensions" );
      if ( size > 0 )
      {
        for ( int i = 0; i < size; ++i )
        {
          cfg.setArrayIndex ( i );
          if ( cfg.value ( "format" ).toString().compare ( name ) == 0 )
          {
            ext = cfg.value ( "defaultExt" ).toString();
            break;
          }
        }
      }
      cfg.endArray();
    }
    return ext;
  }

  /**
  * Suche mit Kodierer \ref name nach dem Passenden
  * XML Eintrag in den Options Vorgabe XML Dateien.
  * Wird eine Standard Erweiterungen gefunden gebe
  * diese zurück andern falls ist die Zeichenkette leer.
  */
  const QString FormatMenu::findDefaultExtension ( const QString &name )
  {
    QString ext = findSettingsExtension ( name );
    if ( ! ext.isEmpty() )
      return ext;

    QX11Grab::OptionsFinder finder ( name );
    QList<QX11Grab::VideoExtension> list = finder.extensionList();
    if ( list.isEmpty() )
      return ext;

    for ( int i = 0; i < list.size(); ++i )
    {
      QX11Grab::VideoExtension e = list.at ( i );
      if ( e.isDefault )
        return e.name;
    }

    return ext;
  }

  /**
  * Lese das Menü neu ein und setze den Button Text zurück.
  * Lese mit \ref AVCodecID alle Unterstützen Formate für diesen
  * Codec (\ref name) Neu ein. Und erstelle ein Menü mit den
  * einzelnen Formaten an. Im Moment fehlt noch seitens FFmpeg
  * die Unterstützung für die Standard Auswahl. Aus diesem Grund
  * wird hier zusätzlich auf \ref findDefaultExtension zugegriffen.
  */
  void FormatMenu::updateMenu ( const QString &name, AVCodecID id )
  {
    m_menu->clear();
    p_ActionsList.clear();

    // Button Text zurück setzen
    setText ( QString::fromUtf8 ( "*.?" ) );
    if ( name.isEmpty() )
      return;

    m_menu->setToolTip ( name );
    QX11Grab::AVOptions* av = new QX11Grab::AVOptions ( this );
    // Alle verfügbaren Format Erweiterungen einfügen
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

    /* Gibt es eine Standard Erweiterung ?
    * Wenn Ja - Dann diesen Eintrag markieren!
    * Versuche zusätzlich noch ein paar andere
    * Erweiterungen um es dem Benutzer einfacher zu machen!
    */
    QString etr = findDefaultExtension ( name );
    if ( ! etr.isEmpty() )
    {
      setEntryEnabled ( etr );
    }
    else if ( ( p_ActionsList.size() > 0 ) && ( p_ActionsList.size() < 4 ) )
    {
      /* Nur ein bis drei Einträge vorhanden
      * dann Automatisch den ersten auswählen */
      setEntryEnabled ( p_ActionsList.first()->text() );
    }
    else if ( name.contains ( "yuv4mpeg", Qt::CaseSensitive ) )
    {
      /* Ist es ein MPEG 2 Container ? */
      setEntryEnabled ( QLatin1String ( "y4m" ) );
    }
    else if ( name.contains ( "mjpeg", Qt::CaseSensitive ) )
    {
      /* Ist es ein mjpeg Container ? */
      setEntryEnabled ( QLatin1String ( "mjpeg" ) );
    }
    else if ( name.contains ( "mpeg1", Qt::CaseSensitive ) )
    {
      /* Ist es ein MPEG 1 Container ? */
      setEntryEnabled ( QLatin1String ( "mpg" ) );
    }
    else if ( name.contains ( "mpeg2", Qt::CaseSensitive ) )
    {
      /* Ist es ein MPEG 2 Container ? */
      setEntryEnabled ( QLatin1String ( "m2v" ) );
    }
    else
    {
      /* in letzter instanz versuche avi */
      setEntryEnabled ( QLatin1String ( "avi" ) );
    }

    // Änderungen Nachricht an die ober Klasse
    emit postUpdate();
  }

  /**
  * Wird immer dann aufgerufen wenn der Benutzer
  * einen Eintrag ausgewählt hat. Gleichzeitig wird
  * der Button Text für die Auswahl angepasst.
  */
  void FormatMenu::itemTriggered ( QAction * ac )
  {
    // Button Text setzen
    setText ( QString::fromUtf8 ( "*.%1" ).arg ( ac->text() ) );
    emit extensionChanged ( ac->text() );
  }

  /**
  * QToolButton bietet keine Option gesetzte Einträge zu demarkieren.
  * Hier werden zuerst alle ausgewählte Markierungen entfernt und der
  * Eintrag neu Ausgwählt der mit name übergeben wurde.
  * Auch hier wird der Button Text geändert!
  */
  void FormatMenu::setEntryEnabled ( const QString &ext )
  {
    // alle Markierungen entfernen
    for ( int i = 0; i < p_ActionsList.size(); ++i )
    {
      p_ActionsList.at ( i )->setChecked ( false );
    }

    if ( ext.isEmpty() )
      return;

    for ( int i = 0; i < p_ActionsList.size(); ++i )
    {
      // Eintrags Titel ist gleich "ext" dann Markieren und Button Text setzen
      if ( p_ActionsList.at ( i )->text().compare ( ext ) == 0 )
      {
        p_ActionsList.at ( i )->setChecked ( true );
        setText ( QString::fromUtf8 ( "*.%1" ).arg ( p_ActionsList.at ( i )->text() ) );
        break;
      }
    }
  }

  /**
  * Sucht nach der Aktuell augewählten Erweiterung
  * Wird keine gefunden ist die Zeichenkette leer
  */
  const QString FormatMenu::currentExtension()
  {
    QString ext;
    for ( int i = 0; i < p_ActionsList.size(); ++i )
    {
      if ( p_ActionsList.at ( i )->isChecked () )
      {
        ext = p_ActionsList.at ( i )->text();
        break;
      }
    }
    return ext;
  }

  /**
  * Menu leeren
  */
  FormatMenu::~FormatMenu()
  {
    if ( m_menu )
      m_menu->clear();
  }
}  /* eof namespace QX11Grab */
