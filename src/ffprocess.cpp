/**
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann http://qx11grab.hjcms.de, (C) 2007-2010
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

#include "ffprocess.h"
#include "settings.h"

#ifndef QX11GRAB_VERSION
#include "version.h"
#endif

#include <QtCore/QDebug>
#include <QtCore/QRegExp>
#include <QtCore/QTime>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QIODevice>
#include <QtGui/QMessageBox>

static inline bool qx11grabSuspendArts()
{
  QStringList args;
  args << "-q" << "terminate";
  return QProcess::startDetached ( "artsshell", args );
}

static inline bool qx11grabSuspendPulse()
{
  QStringList args ( "qx11grab" );
  // QProcess::startDetached ( "pactl", QStringList() << "suspend-source" << "quelle" << "1" );
  return QProcess::startDetached ( "pasuspender", args );
}

FFProcess::FFProcess ( QObject *parent, Settings *settings )
    : QObject ( parent )
    , cfg ( settings )
    , xInfo()
{
  arguments = QStringList();
}

/**
* Gibt den Pfad zur ffmpeg zurück.
*/
const QString FFProcess::application()
{
  return  cfg->value ( QLatin1String ( "ff_path" ), QLatin1String ( "ffmpeg" ) ).toString();
}

/**
* Das aktuelle Arbeits Verzeichnis
*/
const QString FFProcess::workdir()
{
  QString p = cfg->value ( QLatin1String ( "tempdir" ), QLatin1String ( "/tmp" ) ).toString();
  QDir d ( p );
  if ( ! d.isReadable() )
    QMessageBox::warning ( 0x00, trUtf8 ( "Warning" ), trUtf8 ( "Permission Denied: %1." ).arg ( p ) );

  return p;
}

/**
* Überprüfe ob die Dimensionen in Ordnung sind
*/
bool FFProcess::create ( const QRect &r )
{
  if ( r.isValid() )
    return true;

  emit errmessage ( trUtf8 ( "Dimension" ), trUtf8 ( "Invalid Window geometry" ) );
  return false;
}

/**
* Starte die Aufnahme
*/
bool FFProcess::start ( const QStringList &cmd )
{
  // shutdown Sound Daemons
  if ( cfg->value ( QLatin1String ( "enable_pulse_pasuspender" ), true ).toBool() )
  {
    qx11grabSuspendArts();
    qx11grabSuspendPulse();
  }

  if ( cmd.size() < 3 || application().isEmpty() || workdir().isEmpty() )
    return false;

  QStringList arguments ( cmd );

  if ( arguments.contains ( application() ) )
    arguments.removeOne ( application() );

  m_QProcess = new QProcess ( this );
  m_QProcess->setWorkingDirectory ( workdir() );
  m_QProcess->setProcessChannelMode ( QProcess::SeparateChannels );
  m_QProcess->setReadChannel ( QProcess::StandardOutput );
  m_QProcess->setStandardErrorFile ( qx11grabLogfile() );

  connect ( m_QProcess, SIGNAL ( error ( QProcess::ProcessError ) ),
            this, SLOT ( errors ( QProcess::ProcessError ) ) );

  connect ( m_QProcess, SIGNAL ( finished ( int, QProcess::ExitStatus ) ),
            this, SLOT ( exited ( int, QProcess::ExitStatus ) ) );

  connect ( m_QProcess, SIGNAL ( started() ),
            this, SLOT ( startCheck() ) );

#ifdef QX11GRAB_DEBUG
  qDebug() << Q_FUNC_INFO << application() << arguments;
#endif
  m_QProcess->start ( application(), arguments );
  return true;
}

/**
* Stoppe die Aufnahme
*/
bool FFProcess::stop()
{
  if ( ! m_QProcess )
    return false;

  emit message ( trUtf8 ( "shutdown please wait ..." ) );

  // FIXME Wenn der Sound Channel nicht stimmt entsteht ein loop!
  char q = 'q';
  if ( ( m_QProcess->write ( &q ) != -1 ) && ( m_QProcess->waitForBytesWritten () ) )
    m_QProcess->closeWriteChannel();
  else
    m_QProcess->kill();

  if ( cfg->value ( QLatin1String ( "enable_pulse_pasuspender" ), false ).toBool() )
    QProcess::startDetached ( "pactl", QStringList() << "suspend-source" << "quelle" << "0" );

  return isRunning();
}

/**
* Aufnahme abbrechen
*/
bool FFProcess::kill()
{
  if ( ! m_QProcess )
    return false;

  emit message ( trUtf8 ( "force shutdown" ) );

  m_QProcess->kill ();
  return true;
}

/**
* Status Überprüfung
*/
bool FFProcess::isRunning()
{
  if ( ! m_QProcess )
    return false;

  switch ( m_QProcess->state() )
  {
    case QProcess::NotRunning:
    {
      emit down ();
      return false;
    }

    case QProcess::Starting:
    {
      emit running ();
      return true;
    }

    case QProcess::Running:
    {
      emit running ();
      return true;
    }

    default:
    {
      emit down ();
      return false;
    }
  }
}

/**
* Fehler Meldungen verarbeiten
*/
void FFProcess::errors ( QProcess::ProcessError err )
{
  QString errtxt = m_QProcess->errorString();
  switch ( err )
  {
    case QProcess::FailedToStart:
      emit errmessage ( trUtf8 ( "Recording" ), trUtf8 ( "FailedToStart (%1) ..." ).arg ( errtxt ) );
      break;

    case QProcess::Crashed:
      emit errmessage ( trUtf8 ( "Recording" ), trUtf8 ( "Crashed (%1) ..." ).arg ( errtxt ) );
      break;

    case QProcess::Timedout:
      emit errmessage ( trUtf8 ( "Recording" ), trUtf8 ( "Timedout (%1) ..." ).arg ( errtxt ) );
      break;

    case QProcess::WriteError:
      emit errmessage ( trUtf8 ( "Recording" ), trUtf8 ( "WriteError (%1) ..." ).arg ( errtxt ) );
      break;

    case QProcess::ReadError:
      emit errmessage ( trUtf8 ( "Recording" ), trUtf8 ( "ReadError (%1) ..." ).arg ( errtxt ) );
      break;

    case QProcess::UnknownError:
      emit errmessage ( trUtf8 ( "Recording" ), trUtf8 ( "UnknownError (%1) ..." ).arg ( errtxt ) );
      break;
  }
}

/**
* Beenden verarbeiten
*/
void FFProcess::exited ( int exitCode, QProcess::ExitStatus stat )
{
  Q_UNUSED ( exitCode )

  switch ( stat )
  {
    case QProcess::NormalExit:
      emit message ( trUtf8 ( "Recording finished." ) );
      break;

    case QProcess::CrashExit:
      emit message ( trUtf8 ( "Process crashed see logfile %1" ).arg ( qx11grabLogfile() ) );
      break;

    default:
      return;
  }
}

/**
* Vor dem Starten auf laufenden Prozess prüfen!
*/
void FFProcess::startCheck()
{
  if ( isRunning() )
    emit message ( trUtf8 ( "Recording started writing to: %1" ).arg ( qx11grabLogfile() ) );
}

FFProcess::~FFProcess()
{}
