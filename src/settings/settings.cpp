/*
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
*/

#include "settings.h"

#ifndef QX11GRAB_VERSION
#include "version.h"
#endif

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QDir>
#include <QtCore/QGlobalStatic>
#include <QtCore/QHashIterator>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QPixmap>

Settings::Settings ( QObject *parent )
    : QSettings ( QSettings::NativeFormat, QSettings::UserScope, "hjcms.de", "qx11grab", parent )
{
  /** Veraltete Einstellungen bei einem neuen Versions Start in die Tonne drücken. */
  if ( ! value ( "Version" ).toString().contains ( QX11GRAB_VERSION ) )
  {
    // DEPRECATED Config Options
    remove ( "windowPos" );
    remove ( "windowSize" );
    remove ( "windowState" );
    remove ( "enable_pulse_pasuspender" );
    setValue ( "Version", QX11GRAB_VERSION );
  }
}

bool Settings::showRubberOnStart()
{
  return value ( "showRubberband", true ).toBool();
}

bool Settings::expertMode()
{
  return value ( "ExpertMode", false ).toBool();
}

void Settings::saveGroup ( const QString &group, const QHash<QString,QVariant> &data )
{
  if ( group.isEmpty() )
    return;

  int row = 0;
  remove ( group );
  beginWriteArray ( group );
  QHashIterator<QString,QVariant> it ( data );
  while ( it.hasNext() )
  {
    it.next();
    setArrayIndex ( row );
    setValue ( "argument", it.key() );
    setValue ( "value", it.value() );
    row++;
  }
  endArray();
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

const QString Settings::binaryPath()
{
  return value ( "Targets/Binary", "ffmpeg" ).toString();
}

const QString Settings::outputDirectory()
{
  return value ( "Targets/Directory", "/tmp" ).toString();
}

const QString Settings::outputTemplateName()
{
  return value ( "Targets/TemplateName", "qx11grab-XXXXXX" ).toString();
}

void Settings::setOutputPath ( const QString &fullpath )
{
  setValue ( "Targets/OutputFile", fullpath );
}

const QString Settings::absoluteOutputPath()
{
  return value ( "Targets/OutputFile", "/tmp/qx11grab-dummy.avi" ).toString();
}

void Settings::setLogLevel ( const QString &level )
{
  setValue ( "LogLevel", level );
}

const QString Settings::logLevel()
{
  return value ( "LogLevel", "info" ).toString();
}

void Settings::setAudioEngine ( const QString &engine )
{
  setValue ( "Audio/Engine", engine );
}

const QString Settings::audioEngine()
{
  return value ( "Audio/Engine", "alsa" ).toString();
}

void Settings::setAudioDevice ( const QString &path )
{
  setValue ( "Audio/Device", path );
}

const QString Settings::audioDevice()
{
  return value ( "Audio/Device", "default" ).toString();
}


void Settings::setAudioVolume ( qint16 i )
{
  setValue ( "Audio/Intensifier", i );
}

qint16 Settings::audioVolume()
{
  return value ( "Audio/Intensifier", 256 ).toUInt();
}

void Settings::setSampleFormat ( const QString &format )
{
  setValue ( "Audio/SampleFormat", format );
}

const QString Settings::sampleFormat()
{
  return value ( "Audio/SampleFormat", "s16" ).toString();
}

void Settings::setAudioType ( const QString &type )
{
  setValue ( "Audio/ServiceType", type );
}

const QString Settings::audioType()
{
  return value ( "Audio/ServiceType", "ma" ).toString();
}

void Settings::setAudioDeviceCommand ( const QStringList &cmd )
{
  setValue ( "Command/AudioDevice", cmd );
}

const QStringList Settings::getAudioDeviceCommand()
{
  QStringList min;
  min << "-f" << "alsa" << "-i" << "default";
  return value ( "Command/AudioDevice", min ).toStringList();
}

void Settings::setCommandLine ( const QStringList &cmd )
{
  setValue ( "Command/Current", cmd );
}

const QStringList Settings::getCommandline()
{
  QStringList min;
  min << binaryPath() << "-xerror";
  return value ( "Command/Current", min ).toStringList();
}

const QStringList Settings::getExpertCommand()
{
  QStringList out;

  int size = beginReadArray ( "ExpertOptions" );
  if ( size < 1 )
  {
    endArray(); // Nicht vergessen ;)
    return out;
  }

  for ( int i = 0; i < size; i++ )
  {
    setArrayIndex ( i );
    out << value ( "argument" ).toString() << value ( "value" ).toString();
  }
  endArray();

  return out;
}

/**
* Sucht nach Array Einträgen!
* \param group  Gruppe
* \param key    Schlüsselwort
* \param val    Den Wert für Schlüssel z.b.: ( Settings::value(key) == value )
* \param sub    Gesuchter Ausgabe Schlüssel der mit value Referenziert wurde!
*/
const QVariant Settings::getArrayItem ( const QString &group, const QString &ref,
                                        const QString &val, const QString &sub )
{
  QVariant ret;
  int size = beginReadArray ( group );
  if ( size > 0 )
  {
    for ( int i = 0; i < size; ++i )
    {
      setArrayIndex ( i );
      if ( value ( ref ).toString().compare ( val ) == 0 )
      {
        ret = value ( sub );
        break;
      }
    }
  }
  endArray(); // !! Muss immmer geschlossen werden !!
  return ret;
}

/**
* Sucht nach einem Symbol, wenn nicht gefunden andere Auswahl setzen!
* \param icon    Titel ohne Erweiterung
* \param broken  Ausweich Titel
* NOTE Ist abhängig von application.cpp Q_INIT_RESOURCE
*/
const QIcon Settings::themeIcon ( const QString &icon, const QString &broken )
{
  QMap<QString,QString> map;
  map["maximize"] = "maximize";
  map["minimize"] = "minimize";
  map["qx11grab"] = "qx11grab";
  map["qx11grab_large"] = "qx11grab-128";
  map["ffmpeg"] = "ffmpeg";
  map["view-grid"] = "grid";
  map["application-exit"] = "exit";

  QString mIcon =  QString::fromUtf8 ( "://images/%1.png" )
                   .arg ( ( ( map[icon].isEmpty() ) ? broken : map[icon] ) );

  QIcon fallbackIcon;
  QPixmap pixmap = QPixmap ( mIcon );
  fallbackIcon.addPixmap ( pixmap, QIcon::Normal, QIcon::Off );

  return QIcon::fromTheme ( icon, fallbackIcon );
}

/**
* Sucht nach einer Bilddatei im Pixmaps Verzeichnis
* \param name   Titel ohne Erweiterung
* \param size   Größenangabe wenn geändert werden soll
*/
const QPixmap Settings::pixmapIcon ( const QString &name, const QSize &size )
{
  QString pixmapPath ( "/usr/share/pixmaps/qx11grab" ); // Standard
  QDir p_dir;
  QStringList paths ( "/usr/share/pixmaps/qx11grab" );
  paths << "/usr/local/share/pixmaps/qx11grab";
  paths << qApp->applicationDirPath() + "../share/pixmaps/qx11grab" ;
  p_dir.setSearchPaths ( "pixmaps", paths );
  foreach ( QString path, p_dir.searchPaths ( "pixmaps" ) )
  {
    p_dir.setPath ( path );
    if ( p_dir.exists() )
    {
      pixmapPath = p_dir.absolutePath();
      break;
    }
  }

  QString p = QString ( "%1/%2.png" ).arg ( pixmapPath, name );
  QPixmap pixmap ( p, "PNG", Qt::AutoColor );
  if ( pixmap.isNull() )
    return QPixmap();

  return ( pixmap.size() == size ) ? pixmap : pixmap.scaled ( size, Qt::KeepAspectRatio );
}

/**
* @short recording logfile
*/
const QString Settings::logfile()
{
  QString log;
  log.sprintf ( "qx11grab-%s_%s.log", getenv ( "USER" ), getenv ( "DISPLAY" ) );
  QDir dir ( QDir::tempPath() );
  return dir.filePath ( log );
}

/**
* @short Contact E-Mail
*/
const char* Settings::aboutMail()
{
  QByteArray m ( "nospam" );
  m.append ( "@" );
  m.append ( "hjcms.de" );
  return m.constData();
}

Settings::~Settings()
{}
