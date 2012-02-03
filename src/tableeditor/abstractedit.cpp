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

#include "abstractedit.h"
#include "settings.h"

/* QX11Grab interface */
#include "interface.h"
#include "qx11grabplugins.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QRegExp>

/* QtGui */
#include <QtGui/QCompleter>
#include <QtGui/QIcon>
#include <QtGui/QInputDialog>
#include <QtGui/QMenu>

AbstractEdit::AbstractEdit ( QWidget * parent )
    : QLineEdit ( parent )
    , m_regExpValidator ( new QRegExpValidator ( this ) )
{
  QRegExp pattern ( "[^ \n\t&\\]+" );
  m_regExpValidator->setRegExp ( pattern );
  proposeList.clear();
}

void AbstractEdit::openProposedDialog()
{
  bool ok;
  int index = ( text().isEmpty() ) ? 0 : proposeList.indexOf ( text() );
  /*: WindowTitle */
  QString item = QInputDialog::getItem ( this, trUtf8 ( "Proposing" ),
                                         trUtf8 ( "submitted items" ),
                                         proposeList, index, false, &ok );
  if ( ok && !item.isEmpty() )
    setText ( item );
}

/**
* Kontext Menü abfangen und einen Eingabe Dialog anbieten
*/
void AbstractEdit::contextMenuEvent ( QContextMenuEvent * e )
{
  QMenu* m = createStandardContextMenu();
  if ( proposeList.size() > 2 )
  {
    /*: MenuEntry */
    QAction* ac = m->addAction ( Settings::themeIcon ( "menu-editors" ), trUtf8 ( "Proposing" ) );
    connect ( ac, SIGNAL ( triggered() ),
              this, SLOT ( openProposedDialog() ) );
  }
  m->exec ( e->globalPos() );
  delete m;
}

/**
* Liste in Zeichnenkette wandeln
*/
const QString AbstractEdit::implode ( const QStringList &data ) const
{
  QString buffer ( data.join ( QX11Grab::delimiter ) );
  return buffer.trimmed();
}

/**
* Zeichnenkette in eine Liste wandeln.
*/
const QStringList AbstractEdit::explode ( const QString &data ) const
{
  QStringList list;
  QString buffer ( data );
  foreach ( QString s, buffer.split ( QX11Grab::delimiter ) )
  {
    list.append ( s.trimmed() );
  }
  return list;
}

/**
* Vordefinierte Standard Methode für QItemDelegate
*/
void AbstractEdit::setValue ( const QVariant &value )
{
  setText ( value.toString() );
}

/**
* Wir könnnen erst jetzt festellen ob FFmpeg eine Parameterliste
* zu Verfügung stellt. Ist dies der fall, dann \ref proposeList
* mit den Daten füttern und den Completer setzen!
*/
void AbstractEdit::setCompleters ( const QList<QX11Grab::FFOption> &list )
{
  if ( list.size() > 0 )
  {
    proposeList.clear();
    foreach ( QX11Grab::FFOption opt, list )
    {
      proposeList.append ( opt.name );
      /* Kommt immer nur dann vor wenn eine einzelne
      * Option mehrere Werte Vorschläge besitzt! */
      if ( ! opt.value.toString().isEmpty() )
      {
        QString data = opt.value.toString();
        if ( data.contains ( QX11Grab::delimiter ) )
          proposeList.append ( explode ( data ) );
      }
    }

    if ( proposeList.size() < 1 )
      return;

    proposeList.sort();

    QCompleter* m_compliter = new QCompleter ( proposeList, this );
    setCompleter ( m_compliter );
  }
}

/**
* Vordefinierte Standard Methode für QItemDelegate
*/
const QVariant AbstractEdit::value()
{
  return QVariant ( text() );
}

/**
* Vordefinierte Methode für geplante
* QStandardItemEditorCreator Einbindung.
*/
const QByteArray AbstractEdit::valuePropertyName () const
{
  return QByteArray ( "value" );
}

AbstractEdit::~AbstractEdit()
{}
