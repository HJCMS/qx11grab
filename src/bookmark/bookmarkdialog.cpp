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
*/

#include "bookmarkdialog.h"
#include "bookmark.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QHashIterator>
#include <QtCore/QString>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QCompleter>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

/* QtDBus */
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>

BookmarkDialog::BookmarkDialog ( QWidget * parent )
    : QDialog ( parent )
    , xml ( new Bookmark() )
    , dbusPath ( "/Bookmark" )
{
  setObjectName ( QLatin1String ( "BookmarkDialog" ) );
  /*: WindowTitle */
  setWindowTitle ( trUtf8 ( "Bookmark" ) );
  setMinimumWidth ( 250 );
  setSizeGripEnabled ( true );
  xml->open();

  QGridLayout* layout = new QGridLayout ( this );
  layout->setObjectName ( "BookmarkDialog/Layout" );

  // Title {
  QLabel* lb_title = new QLabel ( trUtf8 ( "Title" ), this );
  layout->addWidget ( lb_title, 0, 0, 1, 1 );

  m_titleEdit = new QLineEdit ( this );
  /*: WhatsThis */
  m_titleEdit->setWhatsThis ( trUtf8 ( "Required Bookmark Identifier" ) );
  layout->addWidget ( m_titleEdit, 0, 1, 1, 1 );

  QCompleter* m_compliter = new QCompleter ( xml->entries(), m_titleEdit );
  m_titleEdit->setCompleter ( m_compliter );
  // } Title

  m_buttonBox = new QDialogButtonBox ( ( QDialogButtonBox::Save | QDialogButtonBox::Close )
                                       , Qt::Horizontal, this );
  m_buttonBox->setObjectName ( "BookmarkDialog/ButtonBox" );
  layout->addWidget ( m_buttonBox, 1, 0, 1, 2 );
  m_buttonBox->button ( QDialogButtonBox::Save )->setEnabled ( false );

  setLayout ( layout );

  connect ( m_titleEdit, SIGNAL ( textChanged ( const QString & ) ),
            this, SLOT ( titleTextChanged ( const QString & ) ) );

  connect ( m_buttonBox, SIGNAL ( accepted () ), this, SLOT ( saveAndExit() ) );
  connect ( m_buttonBox, SIGNAL ( rejected () ), this, SLOT ( reject() ) );

  QDBusConnection p_dbus = QDBusConnection::sessionBus();
  p_dbus.registerObject ( dbusPath, this, ( QDBusConnection::ExportScriptableContents ) );
}

bool BookmarkDialog::saveBookmark()
{
  QString id = m_titleEdit->text();
  if ( ! xml->open() )
    qFatal ( "Permission Denied" );

  BookmarkEntry entry = xml->entry ( id );
  if ( entry.isValid() )
  {
    entry.setCodecOptions ( BookmarkEntry::VCODEC, vCodecID, vCodec );
    entry.setCodecOptions ( BookmarkEntry::ACODEC, aCodecID, aCodec );
  }
  // qDebug() << Q_FUNC_INFO << xml->toString();

  if ( xml->save() )
  {
    QDBusInterface iface ( "de.hjcms.qx11grab", "/BookmarkSelect", "de.hjcms.qx11grab.BookmarkSelecter" );
    iface.call ( "reload" );
    return true;
  }
  return false;
}

void BookmarkDialog::titleTextChanged ( const QString &txt )
{
  if ( txt.length() >= 3 )
    m_buttonBox->button ( QDialogButtonBox::Save )->setEnabled ( true );
}

void BookmarkDialog::saveAndExit()
{
  if ( saveBookmark() )
    accept();
  else
    reject();
}

void BookmarkDialog::setVCodecOptions ( const QString &id, const QHash<QString,QVariant> &hash )
{
  vCodecID = id;
  vCodec.clear();
  vCodec = hash;
}

void BookmarkDialog::setACodecOptions ( const QString &id, const QHash<QString,QVariant> &hash )
{
  aCodecID = id;
  aCodec.clear();
  aCodec = hash;
}

void BookmarkDialog::setBookmark ( const QString &str )
{
  m_titleEdit->setText ( str );
}

const QString BookmarkDialog::getBookmark()
{
  return m_titleEdit->text();
}

BookmarkDialog::~BookmarkDialog()
{
  QDBusConnection::sessionBus().unregisterObject ( dbusPath, QDBusConnection::UnregisterNode );
}
