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

#include "pulseaudiodialog.h"

/* QtCore */
#include <QtCore/QCoreApplication>
#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QGlobalStatic>
#include <QtCore/QList>
#include <QtCore/QVariant>

/* QtDBus */
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>

/* QtGui */
#include <QtGui/QListWidgetItem>

extern "C"
{
#include <pulse/context.h>
#include <pulse/mainloop.h>
#include <pulse/introspect.h>
#include <pulse/error.h>
}

static pa_mainloop_api* mainloop_api = NULL;
static pa_context* context = NULL;

/**
* send errors to mainWindow
*/
static void sendPulseErrorToMainWindow ( const char* error )
{
  QString message ( "Pulse:" );
  message.append ( QString::fromUtf8 ( error ) );

  QDBusInterface iface ( "de.hjcms.qx11grab", "/", "de.hjcms.qx11grab" );
  iface.call ( "message", message );
}

/**
* struct for simple list of Card Input Sources
* \note struct for pa_source_info
*/
typedef struct PaDevInfo
{
  quint32 index;
  QString name;
  QString description;
} padevinfo_t;

/** Insert all input cards into pulseInfo */
static QList<PaDevInfo> pulseInfo;

/** Pulse "pa_context_get_source_info_list" callback function */
static void getPulseSourceInfo ( pa_context *, const pa_source_info *i, int success, void * )
{
  if ( success > 0 )
  {
    if ( pa_context_errno ( context ) == PA_ERR_NOENTITY )
      return;

    qWarning ( "Source callback failure" );
    mainloop_api->quit ( mainloop_api, 0 );
    return;
  }

  if ( success < 0 )
    return;

  // qDebug() <<  "Pulse::getPulseSourceInfo >> " << i->index << i->name << i->description;
  PaDevInfo devinfo;
  devinfo.index = i->index;
  devinfo.name = QString ( i->name );
  devinfo.description = QString ( i->description );
  pulseInfo.append ( devinfo );
}

/** Pulse "pa_context_set_state_callback" callback function */
static void pulseContextState ( pa_context *c, void *userdata )
{
  padevinfo_t* dummy = static_cast<padevinfo_t*> ( userdata );

  Q_ASSERT ( c );

  switch ( pa_context_get_state ( c ) )
  {
    case PA_CONTEXT_CONNECTING:
    case PA_CONTEXT_AUTHORIZING:
    case PA_CONTEXT_SETTING_NAME:
      break;

    case PA_CONTEXT_READY:
    {
      fprintf ( stderr, "Pulse: Connection established.\n" );
      pa_operation *o;
      if ( ! ( o = pa_context_get_source_info_list ( c, getPulseSourceInfo, dummy ) ) )
      {
        qWarning ( "Pulse context get card info list failed!" );
        mainloop_api->quit ( mainloop_api, 0 );
        return;
      }
      pa_operation_unref ( o );
      break;
    }

    case PA_CONTEXT_TERMINATED:
      mainloop_api->quit ( mainloop_api, 0 );
      break;

    case PA_CONTEXT_FAILED:
    default:
    {
      fprintf ( stderr, "Context error: %s\n", pa_strerror ( pa_context_errno ( c ) ) );
      sendPulseErrorToMainWindow ( pa_strerror ( pa_context_errno ( c ) ) );
      /** FIXME pulse:abort(); Ist keine gute Idee weil:
      * Wenn kein Server erreichbar ist bringt abort mit sigsev QX11Grab zum absturts!
      */
      mainloop_api->quit ( mainloop_api, 0 );
      break;
    }
  }
}

/** \class PulseAudioDialog */
PulseAudioDialog::PulseAudioDialog ( QWidget * parent )
    : AbstractAudioDialog ( parent )
{
  setObjectName ( QLatin1String ( "PulseAudioDialog" ) );
  /*: WindowTitle */
  setWindowTitle ( trUtf8 ( "Pulse Device Selection" ) );
  m_deviceListWidget->clear();
  pulseInfo.clear();
  initInterface();
}

/**
* insert all items into \ref m_deviceListWidget
*/
void PulseAudioDialog::insertItems()
{
  int size = pulseInfo.size();
  if ( size > 0 )
  {
    for ( int i = 0; i < size; ++i )
    {
      QString index = QString::number ( pulseInfo.at ( i ).index );
      QString name = pulseInfo.at ( i ).name;
      QString desc = pulseInfo.at ( i ).description;
      QListWidgetItem* item = new QListWidgetItem ( m_deviceListWidget );
      item->setText ( QString ( "%1 \"%2\" Index:%3" ).arg ( desc, name, index ) );
      item->setData ( Qt::UserRole, name );
      item->setData ( Qt::ToolTipRole, index );
      item->setData ( Qt::StatusTipRole, desc );
      m_deviceListWidget->addItem ( item );
    }
  }
}

/**
* main method to get interfaces from pulseaudio server
*/
void PulseAudioDialog::initInterface()
{
  pa_mainloop* mloop = NULL;
  char* server = NULL;
  QByteArray client_name = qApp->applicationName().toUtf8();
  int ret = 0;

  // qDebug() <<  Q_FUNC_INFO << __LINE__ << "pa_mainloop_new";
  if ( ! ( mloop = pa_mainloop_new() ) )
  {
    fprintf ( stderr, "Set up a new pulse main loop failed with.\n" );
    return;
  }

  // qDebug() <<  Q_FUNC_INFO << __LINE__ << "pa_mainloop_get_api";
  mainloop_api = pa_mainloop_get_api ( mloop );

  // qDebug() <<  Q_FUNC_INFO << __LINE__ << "pa_context_new";
  if ( ! ( context = pa_context_new ( mainloop_api, client_name.constData() ) ) )
  {
    fprintf ( stderr, "Create a new pulse connection context failed.\n" );
    return;
  }

  // qDebug() <<  Q_FUNC_INFO << __LINE__ << "pa_context_set_state_callback";
  pa_context_set_state_callback ( context, pulseContextState, NULL );

  // qDebug() <<  Q_FUNC_INFO << __LINE__ << "pa_context_connect";
  if ( pa_context_connect ( context, server, PA_CONTEXT_NOFLAGS, NULL ) < 0 )
  {
    fprintf ( stderr, "Pulse connection failed.\n" );
    pa_mainloop_free ( mloop );
    return;
  }

  // qDebug() <<  Q_FUNC_INFO << __LINE__ << "pa_mainloop_run";
  if ( pa_mainloop_run ( mloop, &ret ) < 0 )
  {
    fprintf ( stderr, "Running pulse mainloop failed.\n" );
    pa_mainloop_free ( mloop );
    return;
  }

  // qDebug() <<  Q_FUNC_INFO << __LINE__ << "pa_mainloop_free";
  pa_mainloop_free ( mloop );

  insertItems();
}

PulseAudioDialog::~PulseAudioDialog()
{}
