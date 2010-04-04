/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
* Copyright: See COPYING file that comes with this distribution
**/

#include "settings.h"

#include <QtCore/QDebug>
#include <QtCore/QMap>
#include <QtCore/QFileInfo>
/* QtGui */
#include <QtGui/QMessageBox>

Settings::Settings ( QObject *parent )
    : QSettings ( QSettings::IniFormat, QSettings::UserScope, "hjcms.de", "qx11grab", parent )
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

const QStringList Settings::getVideoOptions()
{
  QStringList params;
  QMapIterator<QString,QVariant> it ( videoOptions() );
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

const QMap<QString,QVariant> Settings::videoOptions()
{
  QMap <QString,QVariant> map;
  int size = beginReadArray ( QLatin1String ( "VideoOptions" ) );
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

Settings::~Settings()
{
}


