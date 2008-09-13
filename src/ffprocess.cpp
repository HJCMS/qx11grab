/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2008
* Copyright: See COPYING file that comes with this distribution
**/

#include "ffprocess.h"
#include "settings.h"

#include <QtCore/QDebug>
#include <QtCore/QRegExp>
#include <QtCore/QTime>

#define LOG_FILE "/tmp/qx11grab.log"
/*
ffmpeg -f x11grab \
  -s sxga -r 24 -b 6500 -bt 256k \
  -sameq -i :0.0+1,33 /tmp/out.avi
#define DEFAULT_CMD "-f x11grab -s %1x%2 -i %3+%4,%5 %6 %7/%8"

*/

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
    arguments << "-f" << "x11grab";
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
    QString outFile = QString ( "%1/%2" ).arg ( cOpt.at ( 2 ), cOpt.at ( 3 ) );
    QString timeStamp = QTime::currentTime().toString ( "hhmmss" );
    outFile.replace ( QRegExp ( "\\bXXXXXX\\b" ), timeStamp );
    arguments << outFile;

    program = cOpt.at ( 0 ).trimmed();
    workdir = cOpt.at ( 2 ).trimmed();
    m_Settings->setValue ( "ffmpeg", program );
    // m_Settings->setValue ( "arguments", arguments.join ( " " ) );
    return true;
  }
  emit message ( trUtf8 ( "Invalid Window geometry" ) );
  return false;
}

bool FFProcess::start()
{
  if ( program.isEmpty() || arguments.size() < 2 )
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

  qDebug() << Q_FUNC_INFO << program << arguments;
  m_QProcess->start ( program, arguments );
  return true;
}

bool FFProcess::stop()
{
  if ( ! m_QProcess )
    return false;

  emit message ( trUtf8 ( "shutdown process please wait ..." ) );

  const char quit[1] = {'q'};
  m_QProcess->write ( quit );

  return true;
}

bool FFProcess::isRunning()
{
  if ( ! m_QProcess )
    return false;

  switch ( m_QProcess->state() )
  {
    case QProcess::NotRunning:
      return false;

    case QProcess::Starting:
      return true;

    case QProcess::Running:
      return true;

    default:
      return false;
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
      emit message ( trUtf8 ( "Process crashed see logfile /tmp/qx11grab.errors" ) );
      break;

    default:
      return;
  }
}

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
  }
  qDebug() << data;
}

void FFProcess::startCheck()
{
  if ( isRunning() )
    emit message ( trUtf8 ( "Recording started writing to: %1" ).arg ( LOG_FILE ) );
}

FFProcess::~FFProcess()
{
}


