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

void FFProcess::addVideoDevice ( const QRect &r, const QStringList &o )
{
  arguments << "-f" << "x11grab" << "-xerror";
  arguments << "-s" << QString ( "%1x%2" ).arg (
      QString::number ( r.width() ),
      QString::number ( r.height() )
  );
  arguments << o;
  arguments << "-i" << QString ( ":%1.%2+%3,%4" ) .arg (
      QString::number ( xInfo.screen() ),
      QString::number ( xInfo.appScreen() ),
      QString::number ( r.x() ),
      QString::number ( r.y() )
  );
}

void FFProcess::addAudioDevice()
{
  QString ffoss = cfg->getStr ( "ff_oss" );
  if ( ffoss.isEmpty() )
    return;

  QFileInfo info ( ffoss );
  if ( info.isRoot() )
    return;

  if ( info.isReadable() )
  {
    /* NOTICE ffmpeg will crash if Audio Device is alreay in use! */
    QFile fp ( info.absoluteFilePath() );
    if ( fp.open ( QIODevice::ReadOnly ) )
    {
      fp.close();
      arguments << "-f" << "oss" << "-i" << ffoss << "-vol" << "512" << "-acodec" << "libfaac";
      arguments << "-ar" << "44100" << "-ab" << "256k" << "-ac" << "1";
      arguments << "-alang" << "ger" << "-sn";
    }
    else
      qWarning ( OSS_IN_USE, qPrintable ( ffoss ) );
  }
}

const QString FFProcess::addOutput ()
{
  QString p = cfg->value ( "tempdir", "/tmp" ).toString();
  QDir d ( p );
  if ( d.isReadable() )
  {
    /* Working Directory */
    workdir = p;

    QString f = cfg->value ( "outputName", "qx11grab-XXXXXX.avi" ).toString();
    QString outFile = QString ( "%1/%2" ).arg ( p, f );
    QString timeStamp = QTime::currentTime().toString ( "hhmmss" );
    outFile.replace ( QRegExp ( "\\b(X{3,})\\b" ), timeStamp );
    QFileInfo info ( outFile );
    if ( info.exists() )
      QMessageBox::warning ( 0L, trUtf8 ( "Warning" ),
                             trUtf8 ( "%1 already exists." ).arg ( outFile ) );

    return outFile;
  }
  return QString();
}

bool FFProcess::create ( const QRect &r )
{
  QStringList videoOptions = cfg->getVideoOptions();
  if ( r.isValid() && ! videoOptions.isEmpty() )
  {
    /* ffmpeg Binary */
    program = cfg->ffbin();

    arguments.clear();
    /* Video Device with given Options */
    addVideoDevice ( r, videoOptions );
    /* Audio */
    addAudioDevice();
    /* Output */
    QString outFile = addOutput ();
    if ( outFile.isEmpty() )
      return false;

    arguments << outFile;
    return true;
  }
  else
  {

#ifdef QX11GRAB_DEBUG
  qDebug() << Q_FUNC_INFO << videoOptions;
#endif

    emit errmessage ( trUtf8 ( "Dimension" ), trUtf8 ( "Invalid Window geometry" ) );
  }
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

  connect ( m_QProcess, SIGNAL ( started() ),
            this, SLOT ( startCheck() ) );

#ifdef QX11GRAB_DEBUG
  qDebug() << Q_FUNC_INFO << program << arguments;
#endif
  // return false;

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

void FFProcess::startCheck()
{
  if ( isRunning() )
    emit message ( trUtf8 ( "Recording started writing to: %1" ).arg ( LOG_FILE ) );
}

FFProcess::~FFProcess()
{
}


