/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2008
* Copyright: See COPYING file that comes with this distribution
**/

#include "settings.h"

#include <QtCore/QDebug>
#include <QtCore/QFileInfo>

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

Settings::~Settings()
{
}


