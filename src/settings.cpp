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

const QStringList Settings::getCommand()
{
  QStringList cmd;
  QFileInfo ffbin ( value ( "ff_path", "/usr/bin/ffmpeg" ).toString() );
  if ( ffbin.isExecutable() )
  {
    cmd << ffbin.absoluteFilePath();
    QMapIterator<QString,QVariant> it ( value ( "ffmpeg/options" ).toMap() );
    QStringList params;
    while ( it.hasNext() )
    {
      it.next();
      params << it.key();
      if ( ! it.value().toString().isEmpty() )
        params << it.value().toString();
    }
    cmd << params.join ( " " ).trimmed();
    cmd << getMapOption ( "qx11grab/options", "tempdir" ).toString();
    cmd << getMapOption ( "qx11grab/options", "outputName" ).toString();
  }
  else
    QMessageBox::critical ( 0, trUtf8 ( "Error" ), trUtf8 ( "Can not open ffmpeg Binary!\nPlease check your Settings." ) );

  return cmd;
}

Settings::~Settings()
{
}


