/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2008
* Copyright: See COPYING file that comes with this distribution
**/

#include "ffprocess.h"
#include "settings.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtCore/QRegExp>
#include <QtCore/QTime>

FFProcess::FFProcess ( QObject *parent, Settings *settings )
    : QObject ( parent )
    , m_Settings ( settings )
    , xInfo()
{
}

bool FFProcess::create ( const QRect &r )
{
  if ( r.isValid() )
  {
    QStringList cOpt = m_Settings->getCommand();
    arguments.clear();
    arguments << "-f" << "x11grab" << "-xerror";
    arguments << "-s" << QString ( "%1x%2" ).arg (
        QString::number ( r.width() ),
        QString::number ( r.height() )
    );
    arguments << cOpt.at ( 1 ).split ( QRegExp ( "[ ]+" ) );
    arguments << "-i" << QString ( ":%1.%2+%3,%4" ) .arg (
        QString::number ( xInfo.screen() ),
        QString::number ( xInfo.appScreen() ),
        QString::number ( r.x() ),
        QString::number ( r.y() )
    );
    /* Extras Options Title Comment etc. */
    QStringList extras ( "ff_title" );
    extras << "ff_author" << "ff_copyright" << "ff_comment" << "ff_genre";
    foreach ( QString n, extras )
    {
      if ( ! m_Settings->getStr ( n ).isEmpty() )
        arguments << n.replace( "ff_", "-" ) << QString( "\"%1\"" ).arg( m_Settings->getStr ( n ) );
    }

    QString outFile = QString ( "%1/%2" ).arg ( cOpt.at ( 2 ), cOpt.at ( 3 ) );
    QString timeStamp = QTime::currentTime().toString ( "hhmmss" );
    outFile.replace ( QRegExp ( "\\bXXXXXX\\b" ), timeStamp );
    arguments << outFile;

    program = cOpt.at ( 0 ).trimmed();
    workdir = cOpt.at ( 2 ).trimmed();
    return true;
  }
  emit message ( trUtf8 ( "Invalid Window geometry" ) );
  return false;
}

bool FFProcess::start()
{
  if ( program.isEmpty() || arguments.size() < 2 || workdir.isEmpty() )
    return false;

  m_QProcess = new QProcess ( this );
  m_QProcess->setWorkingDirectory ( workdir );
  m_QProcess->setProcessChannelMode ( QProcess::SeparateChannels );
  m_QProcess->setReadChannel ( QProcess::StandardOutput );
  m_QProcess->setStandardErrorFile ( LOG_FILE );

  connect ( m_QProcess, SIGNAL ( error ( QProcess::ProcessError ) ),
            this, SLOT ( errors ( QProcess::ProcessError ) ) );

  connect ( m_QProcess, SIGNAL ( finished ( int, QProcess::ExitStatus ) ),
            this, SLOT ( exited ( int, QProcess::ExitStatus ) ) );

  connect ( m_QProcess, SIGNAL ( readyReadStandardOutput() ),
            this, SLOT ( readOutput() ) );

  connect ( m_QProcess, SIGNAL ( started() ),
            this, SLOT ( startCheck() ) );

  // qDebug() << Q_FUNC_INFO << program << arguments;
  m_QProcess->start ( program, arguments );
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

/**
 * TODO
 */
void FFProcess::readOutput()
{
  QString data = m_QProcess->readAllStandardOutput();
  data.trimmed();
  if ( data.contains ( "frame=" ) )
  {
    data.replace ( QRegExp ( "[ ]+" ), " " );
    data.replace ( QRegExp ( "= " ), "=" );
    data.trimmed();
    QStringList list = data.split ( " " );
    qDebug() << list;
  }
  // qDebug() << data;
}

void FFProcess::startCheck()
{
  if ( isRunning() )
    emit message ( trUtf8 ( "Recording started writing to: %1" ).arg ( LOG_FILE ) );
}

FFProcess::~FFProcess()
{
}


