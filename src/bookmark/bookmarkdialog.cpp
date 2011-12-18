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

#include "bookmarkdialog.h"
#include "bookmarkinfo.h"
#include "settings.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QRegExp>

/* QtGui */
#include <QtGui/QVBoxLayout>

/* QtDBus */
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>

BookmarkDialog::BookmarkDialog ( Settings * cfg, QWidget * parent )
    : QDialog ( parent )
    , settings ( cfg )
    , dbusPath ( "/Bookmark" )
{
  setObjectName ( QLatin1String ( "BookmarkDialog" ) );
  setWindowTitle ( trUtf8 ( "Bookmark" ) );
  setSizeGripEnabled ( true );

  QVBoxLayout* layout = new QVBoxLayout ( this );
  layout->setObjectName ( "BookmarkDialog/Layout" );

  // QStackedLayout {
  m_stackedLayout = new QStackedLayout ( this );
  m_stackedLayout->setObjectName ( "BookmarkDialog/StackedLayout" );
  layout->addLayout ( m_stackedLayout );

  m_infoWidget = new BookmarkInfo ( this );
  m_stackedLayout->addWidget ( m_infoWidget );

  // } QStackedLayout

  m_buttonBox = new QDialogButtonBox ( ( QDialogButtonBox::Apply | QDialogButtonBox::Close )
                                       , Qt::Horizontal, this );

  m_buttonBox->setObjectName ( "BookmarkDialog/ButtonBox" );
  layout->addWidget ( m_buttonBox );
  setLayout ( layout );

  connect ( m_buttonBox, SIGNAL ( accepted () ), this, SLOT ( accept() ) );
  connect ( m_buttonBox, SIGNAL ( rejected () ), this, SLOT ( reject() ) );

  QDBusConnection p_dbus = QDBusConnection::sessionBus();
  p_dbus.registerObject ( dbusPath, this, ( QDBusConnection::ExportScriptableContents ) );

  // minimize it
  resize ( 250, 350 );
}

const QString BookmarkDialog::implode ( const QStringList &data ) const
{
  QString buffer ( data.join ( " " ) );
  QRegExp pattern ( "[\\s\\t]+\\-" );
  buffer.replace ( pattern, "\n-" );
  return buffer;
}

const QStringList BookmarkDialog::explode ( const QString &data ) const
{
  QString buffer ( data );
  return buffer.split ( "\n" );
}

void BookmarkDialog::setBookmark ( const QString &str )
{
  m_infoWidget->m_titleEdit->setText ( str );
}

const QString BookmarkDialog::getBookmark()
{
  return m_infoWidget->m_titleEdit->text();
}

void BookmarkDialog::setMetadata ()
{
  
//   m_infoWidget->m_editMeta->setPlainText ( implode ( str ) );
}

void BookmarkDialog::setVCodec ()
{
//   m_infoWidget->m_editVCodec->setPlainText ( implode ( str ) );
}

void BookmarkDialog::setACodec ()
{
//   m_infoWidget->m_editACodec->setPlainText ( implode ( str ) );
}

BookmarkDialog::~BookmarkDialog()
{
  QDBusConnection::sessionBus().unregisterObject ( dbusPath, QDBusConnection::UnregisterNode );
}
