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
#include "bookmark.h"
#include "settings.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QHash>
#include <QtCore/QHashIterator>
#include <QtCore/QRegExp>
#include <QtCore/QString>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

/* QtDBus */
#include <QtDBus/QDBusConnection>

BookmarkDialog::BookmarkDialog ( Settings * cfg, QWidget * parent )
    : QDialog ( parent )
    , settings ( cfg )
    , xml ( new Bookmark() )
    , dbusPath ( "/Bookmark" )
{
  setObjectName ( QLatin1String ( "BookmarkDialog" ) );
  setWindowTitle ( trUtf8 ( "Bookmark" ) );
  setMinimumWidth ( 250 );
  setSizeGripEnabled ( true );

  QGridLayout* layout = new QGridLayout ( this );
  layout->setObjectName ( "BookmarkDialog/Layout" );

  // Title {
  QLabel* lb_title = new QLabel ( trUtf8 ( "Title" ), this );
  layout->addWidget ( lb_title, 0, 0, 1, 1 );

  m_titleEdit = new QLineEdit ( this );
  m_titleEdit->setWhatsThis ( trUtf8 ( "Required Bookmark Identifier" ) );
  layout->addWidget ( m_titleEdit, 0, 1, 1, 1 );
  // } Title

  m_buttonBox = new QDialogButtonBox ( ( QDialogButtonBox::Save | QDialogButtonBox::Close )
                                       , Qt::Horizontal, this );
  m_buttonBox->setObjectName ( "BookmarkDialog/ButtonBox" );
  layout->addWidget ( m_buttonBox, 1, 0, 1, 2 );
  m_buttonBox->button ( QDialogButtonBox::Save )->setEnabled ( false );

  setLayout ( layout );

  connect ( m_titleEdit, SIGNAL ( textChanged ( const QString & ) ),
            this, SLOT ( titleChanged ( const QString & ) ) );

  connect ( m_buttonBox, SIGNAL ( accepted () ), this, SLOT ( saveAndExit() ) );
  connect ( m_buttonBox, SIGNAL ( rejected () ), this, SLOT ( reject() ) );

  QDBusConnection p_dbus = QDBusConnection::sessionBus();
  p_dbus.registerObject ( dbusPath, this, ( QDBusConnection::ExportScriptableContents ) );
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
  m_titleEdit->setText ( str );
}

const QString BookmarkDialog::getBookmark()
{
  return m_titleEdit->text();
}

void BookmarkDialog::titleChanged ( const QString &txt )
{
  if ( txt.length() >= 3 )
    m_buttonBox->button ( QDialogButtonBox::Save )->setEnabled ( true );
}

void BookmarkDialog::saveAndExit()
{
  QString id = m_titleEdit->text();
  xml->open();

  BookmarkEntry entry = xml->entry ( id );
  entry.addVCodecs ( settings->value ( "video_codec" ).toString(), settings->readGroup ( "VideoOptions" ) );
  entry.addACodecs ( settings->value ( "audio_codec" ).toString(), settings->readGroup ( "AudioOptions" ) );
#ifdef MAINTAINER_REPOSITORY
  qDebug() << Q_FUNC_INFO << xml->toString ( 1 );
#endif

  if ( xml->save() )
    accept();
  else
    reject();
}

BookmarkDialog::~BookmarkDialog()
{
  QDBusConnection::sessionBus().unregisterObject ( dbusPath, QDBusConnection::UnregisterNode );
}
