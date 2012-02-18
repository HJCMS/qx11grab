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

#include "codecoptions.h"
#include "optionsfinder.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QAction>

/* QtDBus */
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusReply>

namespace QX11Grab
{
  CodecOptions::CodecOptions ( QWidget * parent )
      : QMenu ( parent )
      , p_icon ( QIcon::fromTheme ( QLatin1String ( "video-x-generic" ) ) )
      , p_codec ( QString::null )
  {
    setObjectName ( QLatin1String ( "CodecOptions" ) );
    setIcon ( p_icon );
    setTitle ( trUtf8 ( "Option" ) );
    setEnabled ( false );

    m_signalMapper = new QSignalMapper ( this );
    connect ( m_signalMapper, SIGNAL ( mapped ( const QString & ) ),
              this, SLOT ( optionTriggered ( const QString & ) ) );

    queryCodec();
  }

  void CodecOptions::queryCodec()
  {
    QDBusInterface iface ( "de.hjcms.qx11grab", "/", "de.hjcms.qx11grab" );
    QDBusReply<QString> reply = iface.call ( "videocodec" );
    if ( ! reply.isValid() )
    {
      // qDebug() << Q_FUNC_INFO << reply.error().name() << reply.error().message();
      return;
    }

    p_codec = reply.value();

    setEnabled ( true );

    OptionsFinder finder ( p_codec );
    foreach ( QString opt, finder.options() )
    {
      QString option = QString::fromUtf8 ( "-%1" ).arg ( opt );
      QAction* ac = addAction ( p_icon, opt );
      ac->setStatusTip ( option );
      connect ( ac, SIGNAL ( triggered () ), m_signalMapper, SLOT ( map () ) );
      m_signalMapper->setMapping ( ac, option );
    }
  }

  void CodecOptions::optionTriggered ( const QString &opt )
  {
    // TODO Standard Option erfragen
    emit optionChanged ( opt, QVariant() );
  }

  CodecOptions::~CodecOptions()
  {}
}  /* eof namespace QX11Grab */
