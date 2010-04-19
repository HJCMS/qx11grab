/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
* Copyright: See COPYING file that comes with this distribution
**/

#include "ffprocess.h"
#include "settings.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtCore/QRegExp>
#include <QtCore/QTime>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QIODevice>
#include <QtGui/QMessageBox>

FFProcess::FFProcess ( QObject *parent, Settings *settings )
    : QObject ( parent )
    , cfg ( settings )
    , xInfo()
{
  arguments = QStringList();
}

const QString FFProcess::application()
{
  return cfg->value ( QLatin1String ( "ff_path" ), "ffmpeg" ).toString();
}

const QString FFProcess::workdir()
{
  QString p = cfg->value ( "tempdir", "/tmp" ).toString();
  QDir d ( p );
  if ( ! d.isReadable() )
    QMessageBox::warning ( 0x00, trUtf8 ( "Warning" ), trUtf8 ( "Permission Denied: %1." ).arg ( p ) );

  return p;
}

bool FFProcess::create ( const QRect &r )
{
  if ( r.isValid() )
    return true;

  emit errmessage ( trUtf8 ( "Dimension" ), trUtf8 ( "Invalid Window geometry" ) );
  return false;
}

bool FFProcess::start ( const QStringList &cmd )
{
  if ( cmd.size() < 3 || application().isEmpty() || workdir().isEmpty() )
    return false;

  QStringList arguments ( cmd );

  if ( arguments.contains( application() ) )
    arguments.removeOne( application() );

  m_QProcess = new QProcess ( this );
  m_QProcess->setWorkingDirectory ( workdir() );
  m_QProcess->setProcessChannelMode ( QProcess::SeparateChannels );
  m_QProcess->setReadChannel ( QProcess::StandardOutput );
  m_QProcess->setStandardErrorFile ( LOG_FILE );

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

bool FFProcess::stop()
{
  if ( ! m_QProcess )
    return false;

  emit message ( trUtf8 ( "shutdown please wait ..." ) );

  char quit = 'q';
  m_QProcess->write ( &quit );

  return isRunning();
}

bool FFProcess::kill()
{
  if ( ! m_QProcess )
    return false;

  emit message ( trUtf8 ( "force shutdown" ) );

  m_QProcess->kill ();
  return true;
}

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

void FFProcess::exited ( int exitCode, QProcess::ExitStatus stat )
{
  Q_UNUSED ( exitCode )

  switch ( stat )
  {
    case QProcess::NormalExit:
      emit message ( trUtf8 ( "Recording finished." ) );
      break;

    case QProcess::CrashExit:
      emit message ( trUtf8 ( "Process crashed see logfile %1" ).arg ( LOG_FILE ) );
      break;

    default:
      return;
  }
}

void FFProcess::startCheck()
{
  if ( isRunning() )
    emit message ( trUtf8 ( "Recording started writing to: %1" ).arg ( LOG_FILE ) );
}

FFProcess::~FFProcess()
{}
