/**
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
**/

#include <cstdlib>

#include "ffprocess.h"
#include "settings.h"

#ifndef QX11GRAB_VERSION
#include "version.h"
#endif

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QFile>
#include <QtCore/QRegExp>
#include <QtCore/QTime>
#include <QtCore/QIODevice>
#include <QtCore/QProcessEnvironment>

/* QtGui */
#include <QtGui/QMessageBox>

FFProcess::FFProcess ( QObject *parent, Settings *settings )
    : QObject ( parent )
    , cfg ( settings )
    , xInfo()
    , m_QProcess ( 0 )
{
  arguments = QStringList();
}

/**
* Gibt den Pfad zur ffmpeg zurück.
*/
const QString FFProcess::application()
{
  QFileInfo bin ( cfg->binaryPath() );
  if ( bin.isExecutable() )
    return bin.absoluteFilePath();
  else
    return QString();
}

/**
* Das aktuelle Arbeits Verzeichnis
*/
const QString FFProcess::workdir()
{
  QString p = cfg->outputDirectory();
  QDir d ( p );
  if ( ! d.isReadable() )
    emit errmessage ( trUtf8 ( "Working Directory" ), trUtf8 ( "Permission Denied: %1." ).arg ( p ) );

  return p;
}

/**
* Erstellt das Schellscript zum Ausführen von FFmpeg
* NOTE Wir müssen ein Schellscript verwenden weil QProcess keinen Delimiter modifikator anbietet!
*/
const QString FFProcess::writeScript ( const QStringList &cmd )
{
  QByteArray username = qgetenv ( "USER" );
  QString script = QString::fromUtf8 ( "%1/qx11grab_%2.sh" ).arg ( workdir(), QString( username ) );
  QFile fp ( script );
  if ( fp.open ( QIODevice::WriteOnly ) )
  {
    QTextStream stream ( &fp );
    stream << QLatin1String ( "#!/usr/bin/env sh\n" );
    stream << "## QX11Grab FFmpeg QCRreencast Script\n\n";
    stream << cmd.join ( " " ).trimmed();
    stream << " \"$@\"\n\n";
    stream << "exit $?\n\n";
    stream << "# EOF\n";
    fp.setPermissions ( ( QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner ) );
    fp.close();
  }
  return script;
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
  if ( cmd.size() < 3 || application().isEmpty() || workdir().isEmpty() )
    return false;

  QFileInfo script ( writeScript ( cmd ) );
  if ( ! script.isExecutable() )
  {
    emit errmessage ( trUtf8 ( "Executable Script" ),
                      trUtf8 ( "Permission Denied: %1." ).arg ( script.absoluteFilePath() ) );
    return false;
  }

  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert ( "FFMPEG_FORCE_NOCOLOR", "1" );
  env.insert ( "AV_LOG_FORCE_NOCOLOR", "1" );

  m_QProcess = new QProcess ( this );
  m_QProcess->setProcessEnvironment ( env );
  m_QProcess->setWorkingDirectory ( workdir() );
  m_QProcess->setProcessChannelMode ( QProcess::SeparateChannels );
  m_QProcess->setReadChannel ( QProcess::StandardOutput );
  m_QProcess->setStandardErrorFile ( Settings::logfile() );

  connect ( m_QProcess, SIGNAL ( stateChanged ( QProcess::ProcessState ) ),
            this, SLOT ( status ( QProcess::ProcessState ) ) );

  connect ( m_QProcess, SIGNAL ( error ( QProcess::ProcessError ) ),
            this, SLOT ( errors ( QProcess::ProcessError ) ) );

  connect ( m_QProcess, SIGNAL ( finished ( int, QProcess::ExitStatus ) ),
            this, SLOT ( exited ( int, QProcess::ExitStatus ) ) );

  connect ( m_QProcess, SIGNAL ( started() ),
            this, SLOT ( startCheck() ) );

  m_QProcess->start ( script.absoluteFilePath() );
  return true;
}

/**
* Stoppe die Aufnahme
*/
void FFProcess::stop()
{
  if ( ! m_QProcess )
    return;

  emit message ( trUtf8 ( "shutdown please wait ..." ) );

  // FIXME Wenn der Sound Channel nicht stimmt entsteht ein loop!
  char q = 'q';
  if ( ( m_QProcess->write ( &q ) != -1 ) && ( m_QProcess->waitForBytesWritten () ) )
    m_QProcess->closeWriteChannel();
  else
    m_QProcess->kill();
}

/**
* Aufnahme abbrechen
*/
void FFProcess::kill()
{
  if ( ! m_QProcess )
    return;

  emit message ( trUtf8 ( "force shutdown" ) );

  m_QProcess->kill ();
}

/**
* Status Überprüfung
*/
bool FFProcess::isRunning()
{
  if ( ! m_QProcess )
    return false;

  // If no process is currently running, 0 is returned.
  if ( m_QProcess->pid() == 0 )
    return false;

  return true;
}

void FFProcess::status ( QProcess::ProcessState status )
{
  switch ( status )
  {
    case QProcess::NotRunning:
      emit down ();
      break;

    case QProcess::Starting:
      break;

    case QProcess::Running:
      emit running ();
      break;

    default:
      break;
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
      emit message ( trUtf8 ( "Process crashed see logfile %1" ).arg ( Settings::logfile() ) );
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
    emit message ( trUtf8 ( "Recording started writing to: %1" ).arg ( Settings::logfile() ) );
}

FFProcess::~FFProcess()
{}
