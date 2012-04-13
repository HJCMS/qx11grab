/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann http://qx11grab.hjcms.de, (C) 2007-2012
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

#ifndef QX11GRAB_VERSION
# include "version.h"
#endif

#include "logviewer.h"
#include "textview.h"

/* QtCore */
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QTextStream>

/* QtGui */
#include <QtGui/QVBoxLayout>

/* QtDBus */
#include <QtDBus/QDBusConnection>

LogViewer::LogViewer ( const QFileInfo &log, QWidget * parent )
    : QDialog ( parent )
    , logFile ( log )
{
  setObjectName ( QLatin1String ( "logviewer" ) );
  /*: WindowTitle */
  setWindowTitle ( trUtf8 ( "qx11grab logfile" ) );
  setWindowModality ( Qt::NonModal );
  setContentsMargins ( 5, 5, 5, 15 );
  setMinimumSize ( 350, 200 );
  setSizeGripEnabled ( true );

  QVBoxLayout* vLayout = new QVBoxLayout ( this );
  vLayout->setMargin ( 0 );

  m_textView = new TextView ( this );
  vLayout->addWidget ( m_textView );

  setLayout ( vLayout );

  connect ( m_textView, SIGNAL ( refresh() ),
            this, SLOT ( readLog() ) );

  QDBusConnection p_dbus = QDBusConnection::sessionBus();
  p_dbus.registerObject ( QLatin1String( "/LogViewer" ), this, ( QDBusConnection::ExportScriptableSlots ) );
  readLog();
}

void LogViewer::readLog()
{
  QFile fp ( logFile.absoluteFilePath(), m_textView );
  if ( fp.open ( QIODevice::ReadOnly ) )
  {
    QTextStream stream ( &fp );
    QString buffer = stream.readAll();
    m_textView->insertText ( buffer );
    fp.close();
  }
}

LogViewer::~LogViewer()
{}
