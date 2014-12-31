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
  bool isInputDevice;
} padevinfo_t;

/** Insert all input cards into pulseInfo */
static QList<PaDevInfo> pulseInfo;

/** Pulse "pa_context_get_source_info_list" callback function */
static void getPulseSourceInfo ( pa_context *co, const pa_source_info *i, int success, void * state )
{
  if ( success > 0 )
  {
    if ( pa_context_errno ( co ) == PA_ERR_NOENTITY )
      return;

    mainloop_api->quit ( mainloop_api, 0 );
    return;
  }

  if ( success < 0 )
    return;

  bool inp = false;
  if ( i->proplist )
  {
    const char* key = pa_proplist_iterate ( i->proplist, &state );
    while ( key != NULL )
    {
      if ( QString::fromUtf8 ( key ).contains ( "device.class", Qt::CaseInsensitive ) )
      {
        inp = !QString::fromUtf8 ( pa_proplist_gets ( i->proplist, key ) ).contains ( "monitor", Qt::CaseInsensitive );
        break;
      }
      key = pa_proplist_iterate ( i->proplist, &state );
    }
  }

  // qDebug() <<  "Pulse::getPulseSourceInfo >> " << i->index << i->name << i->description << inp;
  PaDevInfo devinfo;
  devinfo.index = i->index;
  devinfo.name = QString ( i->name );
  devinfo.description = QString ( i->description );
  devinfo.isInputDevice = inp;
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
      qDebug ( "QX11Grab: connection established." );
      pa_operation *o;
      if ( ! ( o = pa_context_get_source_info_list ( c, getPulseSourceInfo, dummy ) ) )
      {
        const char* errorMessage = pa_strerror ( pa_context_errno ( c ) );
        fprintf ( stderr, "QX11Grab: pulse context get card info list failed! - %s\n", errorMessage );
        sendPulseErrorToMainWindow ( errorMessage );
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
      const char* errorMessage = pa_strerror ( pa_context_errno ( c ) );
      fprintf ( stderr, "QX11Grab: context error - %s\n", errorMessage );
      sendPulseErrorToMainWindow ( errorMessage );
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
* Füge Inhalte in \b DeviceListWidget
*/
void PulseAudioDialog::insertItems()
{
  int size = pulseInfo.size();
  QIcon inputIcon = QIcon::fromTheme ( "audio-input-microphone" );
  QIcon monitorIcon = QIcon::fromTheme ( "audio-card" );
  if ( size > 0 )
  {
    for ( int i = 0; i < size; ++i )
    {
      QString index = QString::number ( pulseInfo.at ( i ).index );
      QString name = pulseInfo.at ( i ).name;
      QString desc = pulseInfo.at ( i ).description;
      QListWidgetItem* item = new QListWidgetItem ( m_deviceListWidget );
      item->setText ( desc );
      item->setData ( Qt::UserRole, name );
      item->setData ( Qt::ToolTipRole, QString ( "%1 = %2" ).arg ( index, name ) );
      item->setData ( Qt::StatusTipRole, desc );
      if ( pulseInfo.at ( i ).isInputDevice )
        item->setData ( Qt::DecorationRole, inputIcon );
      else
        item->setData ( Qt::DecorationRole, monitorIcon );

      m_deviceListWidget->addItem ( item );
    }
  }
}

/**
* main method to get interfaces from pulseaudio server
* http://freedesktop.org/software/pulseaudio/doxygen/async.html
*/
void PulseAudioDialog::initInterface()
{
  pa_mainloop* mloop = NULL;
  char* server = NULL;
  QByteArray client_name ( "qx11grab" );
  int ret = 0;

  // qDebug() <<  Q_FUNC_INFO << __LINE__ << "pa_mainloop_new";
  mloop = pa_mainloop_new();
  if ( ! mloop )
  {
    fprintf ( stderr, "QX11Grab: Set up a new pulse main loop failed with.\n" );
    return;
  }

  // qDebug() <<  Q_FUNC_INFO << __LINE__ << "pa_mainloop_get_api";
  mainloop_api = pa_mainloop_get_api ( mloop );

  // qDebug() <<  Q_FUNC_INFO << __LINE__ << "pa_context_new";
  context = pa_context_new ( mainloop_api, client_name.constData() );
  if ( ! context )
  {
    fprintf ( stderr, "QX11Grab: Create a new pulse connection context failed.\n" );
    pa_mainloop_free ( mloop );
    return;
  }

  // qDebug() <<  Q_FUNC_INFO << __LINE__ << "pa_context_set_state_callback";
  pa_context_set_state_callback ( context, pulseContextState, NULL );

  // qDebug() <<  Q_FUNC_INFO << __LINE__ << "pa_context_connect";
  if ( pa_context_connect ( context, server, PA_CONTEXT_NOFLAGS, NULL ) < 0 )
  {
    fprintf ( stderr, "QX11Grab: Pulse connection failed.\n" );
    pa_mainloop_free ( mloop );
    return;
  }

  // qDebug() <<  Q_FUNC_INFO << __LINE__ << "pa_mainloop_run";
  if ( pa_mainloop_run ( mloop, &ret ) < 0 )
  {
    fprintf ( stderr, "QX11Grab: Running pulse mainloop failed.\n" );
    pa_mainloop_free ( mloop );
    return;
  }

  // qDebug() <<  Q_FUNC_INFO << __LINE__ << "pa_mainloop_free";
  qDebug ( "QX11Grab: pulse source info done." );
  pa_context_unref ( context );
  pa_mainloop_free ( mloop );

  insertItems();
}

PulseAudioDialog::~PulseAudioDialog()
{}
