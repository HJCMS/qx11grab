/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2008
* Copyright: See COPYING file that comes with this distribution
**/

#include "settingspageone.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDir>

/* QtGui */
#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>

SettingsItem::SettingsItem ( QWidget *parent )
    : QGroupBox ( parent )
{
  setFlat ( true );
  setContentsMargins ( 2, 5, 2, 2 );
  setMinimumHeight ( 50 );

  QVBoxLayout *layout = new QVBoxLayout ( this );
  layout->setContentsMargins ( 0, 4, 0, 0 );

  label = new QLabel ( this );
  layout->addWidget ( label );

  edit = new QLineEdit ( this );
  edit->setMinimumHeight ( 18 );
  layout->addWidget ( edit );

  setLayout ( layout );
}

void SettingsItem::setText ( const QString &txt )
{
  label->setText ( txt );
}

void SettingsItem::setValue ( const QVariant &val )
{
  QString str = val.toString();
  edit->setText ( str );
}

const QVariant SettingsItem::value()
{
  return QVariant ( edit->text() );
}

/** SettingsPageOne */
SettingsPageOne::SettingsPageOne ( QWidget *parent )
    : QWidget ( parent )
{
  setObjectName ( "SettingsPageOne" );
  setContentsMargins ( 2, 5, 2, 2 );
  setMinimumWidth ( 350 );
  setMinimumHeight ( 250 );

  topLayout = new QVBoxLayout ( this );
  topLayout->setSpacing ( 2 );
  topLayout->addWidget ( new QLabel ( trUtf8 ( "Application Settings" ), this ) );

  startMinimized = new QCheckBox ( trUtf8 ( "Start qx11grab Minimized" ), this );
  startMinimized->setObjectName ( "startMinimized" );
  topLayout->addWidget ( startMinimized );

  setDefaults();

  topLayout->addStretch ( 1 );
  setLayout ( topLayout );
}

const QMap<QString,QVariant> SettingsPageOne::getDefaults ()
{
  QMap<QString,QVariant> map;
  map.insert ( "ff_path", QVariant ( "/usr/bin/ffmpeg" ) );
  map.insert ( "tempdir", QVariant ( QDir::tempPath() ) );
  map.insert ( "outputName", QVariant ( "qx11grab-XXXXXX.mpg" ) );
  return map;
}

const QString SettingsPageOne::getDescription ( const QString &key )
{
  QMap<QString,QString> map;
  map.insert ( "ff_path", trUtf8 ( "Full path to ffmpeg Binary" ) );
  map.insert ( "tempdir", trUtf8 ( "Default tmp Directory" ) );
  map.insert ( "outputName", trUtf8 ( "Default output file name" ) );

  if ( map.contains ( key ) )
    return map[key];
  else
    return QString::null;
}

void SettingsPageOne::setDefaults()
{
  QMapIterator<QString,QVariant> it ( getDefaults() );
  while ( it.hasNext() )
  {
    it.next();
    SettingsItem *item = new SettingsItem ( this );
    item->setObjectName ( it.key() );
    item->setText ( getDescription ( it.key() ) );
    item->setValue ( it.value() );
    topLayout->addWidget ( item );
  }
}

const QMap<QString,QVariant> SettingsPageOne::getOptions()
{
  QMap<QString,QVariant> map;
  map.insert ( "startMinimized", startMinimized->isChecked() );

  foreach ( SettingsItem *item, findChildren<SettingsItem*>() )
  {
    if ( ! item->objectName().isEmpty() )
    {
      map.insert ( item->objectName(), item->value() );
    }
  }
  return map;
}

void SettingsPageOne::setOptions ( const QMap<QString,QVariant> &map )
{
  if ( map.contains ( "startMinimized" ) )
    startMinimized->setChecked ( map["startMinimized"].toBool() );

  QMapIterator<QString,QVariant> it ( map );
  while ( it.hasNext() )
  {
    it.next();

    if ( it.key().isEmpty() )
      continue;

    SettingsItem *item = findChild<SettingsItem*>( it.key() );
    if ( item && it.value().type() == QVariant::String )
      item->setValue ( it.value() );
  }
}

SettingsPageOne::~SettingsPageOne()
{
}


