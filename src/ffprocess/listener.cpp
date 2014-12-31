/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann (Undefined) http://qx11grab.hjcms.de, (C) 2007-2015
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

#include "listener.h"

#include <limits.h>
#include <sys/mount.h>

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QIODevice>
#include <QtCore/QStringList>
#include <QtCore/QTime>

Listener::Listener ( QObject * parent )
    : QFileSystemWatcher ( parent )
{
  setObjectName ( QLatin1String ( "Listener" ) );

  m_fileInfo = new QFileInfo ( QDir::temp(), "qx11grab_dummy.avi" );
  m_fileInfo->setCaching ( false );

  m_timeSpec = new QDateTime ( QDateTime::currentDateTime() );

  connect ( this, SIGNAL ( fileChanged ( const QString & ) ),
            this, SLOT ( updated ( const QString & ) ) );
}

void Listener::setInfoUpdate ()
{
  if ( ! m_fileInfo->exists() )
    return;

  int sec = m_timeSpec->secsTo ( m_fileInfo->lastModified() );
  if ( sec > 5 )
  {
    QString buffer, size;
    QString t = QTime ( 0, 0 ).addSecs ( sec ).toString ( "hh:mm:ss" );
    qlonglong bytes = m_fileInfo->size();
    if ( bytes > ( BLOCK_SIZE * BLOCK_SIZE * BLOCK_SIZE ) )
      size = buffer.sprintf ( "%lld GiB", ( bytes / ( BLOCK_SIZE * BLOCK_SIZE * BLOCK_SIZE ) ) );
    else if ( bytes > ( BLOCK_SIZE * BLOCK_SIZE ) )
      size = buffer.sprintf ( "%lld MiB", ( bytes / ( BLOCK_SIZE * BLOCK_SIZE ) ) );
    else if ( bytes > BLOCK_SIZE )
      size = buffer.sprintf ( "%lld KiB", ( bytes / BLOCK_SIZE ) );
    else
      size = buffer.sprintf ( "%lld Byte", bytes );

    /*: StatusBar */
    QString out = trUtf8 ( "Time: %1 Size: %2" ).arg ( t, size );
    emit info ( out );
  }
}

void Listener::start()
{
  if ( m_fileInfo->size() > 0 )
    addPath ( m_fileInfo->absoluteFilePath() );
}

void Listener::updated ( const QString & )
{
  setInfoUpdate ();
}

void Listener::clear()
{
  if ( files().size() > 0 )
    removePaths ( files() );
}

bool Listener::setOutputFile ( const QString &path )
{
  if ( m_fileInfo->absoluteFilePath().compare ( path ) == 0 )
    return true;

  QFile fp ( path );
  if ( fp.open ( QIODevice::WriteOnly ) )
  {
    /** WARNING File Size must greater then 0Byte to add into QFileSystemWatcher
    * @ref start function check this!
    */
    fp.setPermissions ( ( QFile::ReadOwner | QFile::WriteOwner | QFile::ReadGroup | QFile::ReadOther ) );
    m_fileInfo->setFile ( path );
    m_timeSpec->setTime ( m_fileInfo->created().time() );
    fp.close();
    return true;
  }
  return false;
}

Listener::~Listener()
{}
