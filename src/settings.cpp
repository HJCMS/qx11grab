/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
* Copyright: See COPYING file that comes with this distribution
**/

#include "settings.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QIODevice>

/* QtGui */
#include <QtGui/QMessageBox>

Settings::Settings ( QObject *parent )
    : QSettings ( QSettings::NativeFormat, QSettings::UserScope, "hjcms.de", "qx11grab", parent )
{
}

const QString Settings::ffbin()
{
  QString ff = value ( "ff_path", "/usr/bin/ffmpeg" ).toString();
  QFileInfo info ( ff );
  if ( info.isExecutable() )
    return ff;
  else
    qWarning ( "Missing FFmpeg Binary!!!" );

  return QString();
}

const QString Settings::audiodev()
{
  QString ffoss = value ( "ff_oss", "/dev/dsp0" ).toString();
  QFileInfo info ( ffoss );
  if ( info.isRoot() )
    return QString();

  if ( info.isReadable() )
  {
    /* NOTICE ffmpeg will crash if Audio Device is alreay in use! */
    QFile fp ( info.absoluteFilePath() );
    if ( fp.open ( QIODevice::ReadOnly ) )
    {
      fp.close();
      return ffoss;
    }
    else
    {
      // "Audio Device %s already in use\nDisable Capture with -f oss Option"
      qWarning ( OSS_IN_USE, qPrintable ( ffoss ) );
    }
  }
  return QString();
}

int Settings::getInt ( const QString &path )
{
  if ( ! contains ( path ) )
    return 0;

  return value ( path ).toInt();
}

bool Settings::getBool ( const QString &path )
{
  if ( ! contains ( path ) )
    return false;

  return value ( path ).toBool();
}

const QString Settings::getStr ( const QString &path )
{
  if ( ! contains ( path ) )
    return QString();

  return value ( path ).toString();
}

const QSize Settings::getSize ( const QString &path, const QSize &min )
{
  if ( ! contains ( path ) )
    return QSize();

  return value ( path, min ).toSize();
}

const QVariant Settings::getMapOption ( const QString &path, const QString &key )
{
  if ( ! contains ( path ) )
    return QVariant();

  QMap<QString,QVariant> map = value ( path ).toMap();
  if ( map.contains ( key ) )
    return map[key];

  return QVariant();
}

const QStringList Settings::commandMetaData()
{
  QStringList meta;
  foreach ( QString key, childKeys() )
  {
    if ( key.contains ( "metadata_" ) )
    {
      meta << QLatin1String ( "-metadata" );
      meta << QString ( "%1=\"%2\"" ).arg ( key.remove ( "metadata_" ), value ( key ).toString() );
    }
  }
  return meta;
}

const QHash<QString,QVariant> Settings::readGroup ( const QString &group )
{
  QHash <QString,QVariant> map;
  int size = beginReadArray ( group );
  if ( size < 1 )
  {
    endArray(); // Nicht vergessen ;)
    return map;
  }

  for ( int i = 0; i < size; i++ )
  {
    setArrayIndex ( i );
    map[ value ( "argument" ).toString() ] =  value ( "value", "" );
  }
  endArray();

  return map;
}

const QStringList Settings::getVideoOptions()
{
  QStringList params;
  QHashIterator<QString,QVariant> it ( readGroup ( "VideoOptions" ) );
  while ( it.hasNext() )
  {
    it.next();
    params << it.key();
    if ( ! it.value().toString().isEmpty() )
      params << it.value().toString();
  }
  params << commandMetaData();
  return params;
}

const QStringList Settings::getAudioOptions()
{
  QStringList params;
  QHashIterator<QString,QVariant> it ( readGroup ( "AudioOptions" ) );
  while ( it.hasNext() )
  {
    it.next();
    params << it.key();
    if ( ! it.value().toString().isEmpty() )
      params << it.value().toString();
  }
  return params;
}

Settings::~Settings()
{
}
