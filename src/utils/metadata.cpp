/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann http://qx11grab.hjcms.de, (C) 2007-2013
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

#include "metadata.h"

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QLocale>
#include <QtCore/QRegExp>
#include <QtCore/QString>
#include <QtCore/QUrl>

/* QtGui */
#include <QtGui/QCompleter>
#include <QtGui/QGridLayout>
#include <QtGui/QSpacerItem>
#include <QtGui/QLabel>
#include <QtGui/QPalette>

MetaData::MetaData ( QWidget * parent )
    : QGroupBox ( parent )
    , mType ( QString ( "mpeg" ) )
{
  setObjectName ( QLatin1String ( "metadata" ) );
  /*: GroupBoxTitle */
  setTitle ( trUtf8 ( "Insert Metadata" ) );
  /*: WhatsThis */
  setWhatsThis ( trUtf8 ( "enable/disable auto insert metadata in the captured video" ) );
  setFlat ( true );
  setCheckable ( true );
  setChecked ( false );

  int grow = 0;
  QGridLayout* gridLayout = new QGridLayout ( this );
  gridLayout->setContentsMargins ( 5, 5, 5, 5 );
  gridLayout->setObjectName ( QLatin1String ( "gridLayout" ) );

  Qt::Alignment labelAlignment = ( Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter );

  QLabel* txt_info = new QLabel ( this );
  txt_info->setText ( trUtf8 ( "Note many formats only support a handful of metadata keys." ) );
  gridLayout->addWidget ( txt_info, grow++, 0, 1, 2 );

  QLabel* txt_title = new QLabel ( this );
  txt_title->setText ( trUtf8 ( "Title:" ) );
  txt_title->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt_title, grow, 0, 1, 1 );

  metadata_INAM = new QLineEdit ( this );
  metadata_INAM->setObjectName ( QLatin1String ( "Metadata/title" ) );
  metadata_INAM->setToolTip ( QLatin1String ( "INAM" ) );
  /*: WhatsThis */
  metadata_INAM->setWhatsThis ( trUtf8 ( "Name or Movie Name" ) );
  gridLayout->addWidget ( metadata_INAM, grow++, 1, 1, 1 );

  QLabel* txt_artist = new QLabel ( this );
  txt_artist->setText ( trUtf8 ( "Artist:" ) );
  txt_artist->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt_artist, grow, 0, 1, 1 );

  metadata_IART = new QLineEdit ( this );
  metadata_IART->setObjectName ( QLatin1String ( "Metadata/author" ) );
  metadata_IART->setToolTip ( QLatin1String ( "IART" ) );
  /*: WhatsThis */
  metadata_IART->setWhatsThis ( trUtf8 ( "Author, Director or Artist" ) );
  gridLayout->addWidget ( metadata_IART, grow++, 1, 1, 1 );

  QLabel* txt_copyright = new QLabel ( this );
  txt_copyright->setText ( trUtf8 ( "Copyright:" ) );
  txt_copyright->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt_copyright, grow, 0, 1, 1 );

  metadata_ICOP = new QLineEdit ( this );
  metadata_ICOP->setObjectName ( QLatin1String ( "Metadata/copyright" ) );
  metadata_ICOP->setToolTip ( QLatin1String ( "ICOP" ) );
  /*: WhatsThis */
  metadata_ICOP->setWhatsThis ( trUtf8 ( "Copyright" ) );
  gridLayout->addWidget ( metadata_ICOP, grow++, 1, 1, 1 );

  QStringList ccLicenses;
  ccLicenses << "CC BY 3.0" << "CC BY-ND 3.0";
  ccLicenses << "CC BY-NC 3.0" << "CC BY-NC-ND 3.0";
  ccLicenses << "CC BY-NC-SA 3.0" << "CC BY-SA 3.0";

  QCompleter* m_cc_completer = new QCompleter ( ccLicenses, metadata_ICOP );
  metadata_ICOP->setCompleter ( m_cc_completer );

  QLabel* txt_createdate = new QLabel ( this );
  txt_createdate->setText ( trUtf8 ( "Creation Date:" ) );
  txt_createdate->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt_createdate, grow, 0, 1, 1 );

  metadata_ICRD = new QDateTimeEdit ( this );
  metadata_ICRD->setObjectName ( QLatin1String ( "Metadata/year" ) );
  metadata_ICRD->setToolTip ( QLatin1String ( "ICRD" ) );
  /*: WhatsThis */
  metadata_ICRD->setWhatsThis ( trUtf8 ( "Creation Date" ) );
  metadata_ICRD->setDateTime ( QDateTime::currentDateTime() );
  metadata_ICRD->setCalendarPopup ( true );
  gridLayout->addWidget ( metadata_ICRD, grow++, 1, 1, 1, Qt::AlignLeft );

  QLabel* txt_subject = new QLabel ( this );
  txt_subject->setText ( trUtf8 ( "Subject:" ) );
  txt_subject->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt_subject, grow, 0, 1, 1 );

  metadata_ISBJ = new QLineEdit ( this );
  metadata_ISBJ->setObjectName ( QLatin1String ( "Metadata/subject" ) );
  metadata_ISBJ->setToolTip ( QLatin1String ( "ISBJ" ) );
  /*: WhatsThis */
  metadata_ISBJ->setWhatsThis ( trUtf8 ( "Subject" ) );
  gridLayout->addWidget ( metadata_ISBJ, grow++, 1, 1, 1 );

  QLabel* txt_description = new QLabel ( this );
  txt_description->setText ( trUtf8 ( "Comments:" ) );
  txt_description->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt_description, grow, 0, 1, 1 );

  metadata_ICMT = new QLineEdit ( this );
  metadata_ICMT->setObjectName ( QLatin1String ( "Metadata/description" ) );
  metadata_ICMT->setToolTip ( QLatin1String ( "ICMT" ) );
  /*: WhatsThis */
  metadata_ICMT->setWhatsThis ( trUtf8 ( "Comments" ) );
  gridLayout->addWidget ( metadata_ICMT, grow++, 1, 1, 1 );

  QLabel* txt_language = new QLabel ( this );
  /*: Alternative: language */
  txt_language->setText ( trUtf8 ( "Voice Recognition:" ) );
  txt_language->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt_language, grow, 0, 1, 1 );

  /* main language in which the work is performed, preferably in ISO 639-2 format.
  * Multiple languages can be specified by separating them with commas. */
  metadata_LANG = new QLineEdit ( this );
  metadata_LANG->setObjectName ( QLatin1String ( "Metadata/language" ) );
  /*: ToolTip */
  metadata_LANG->setToolTip ( trUtf8 ( "recognition of speech must set with three letter ISO 639-2 format" ) );
  /*: WhatsThis */
  metadata_LANG->setWhatsThis ( trUtf8 ( "Language in ISO 639-2 format in three letters.\nExample: Germany=ger\nMultiple languages can be specified by separating them with commas." ) );
  gridLayout->addWidget ( metadata_LANG, grow++, 1, 1, 1 );

  QSpacerItem* spacer  = new QSpacerItem ( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
  gridLayout->addItem ( spacer, grow++, 0, 1, 2 );

  setLayout ( gridLayout );

  connect ( metadata_INAM, SIGNAL ( editingFinished () ), this, SIGNAL ( postUpdate () ) );
  connect ( metadata_IART, SIGNAL ( editingFinished () ), this, SIGNAL ( postUpdate () ) );
  connect ( metadata_ICOP, SIGNAL ( editingFinished () ), this, SIGNAL ( postUpdate () ) );
  connect ( metadata_ISBJ, SIGNAL ( editingFinished () ), this, SIGNAL ( postUpdate () ) );
  connect ( metadata_ICMT, SIGNAL ( editingFinished () ), this, SIGNAL ( postUpdate () ) );
  connect ( metadata_LANG, SIGNAL ( editingFinished () ), this, SIGNAL ( postUpdate () ) );

  // Updates
  connect ( this, SIGNAL ( toggled ( bool ) ),
            this, SLOT ( statusUpdate ( bool ) ) );
}

/**
* Liste aller Texteingabe Felder
*/
const QList<QLineEdit*> MetaData::metadataObjects()
{
  QList<QLineEdit*> items;
  items << metadata_INAM << metadata_IART << metadata_ICOP;
  items << metadata_ISBJ << metadata_ICMT << metadata_LANG;
  return items;
}

/**
* \class MetaData
* Metadaten Beschreibung
* \link http://www.ffmpeg.org/doxygen/trunk/group__metadata__api.html
* \link http://wiki.multimedia.cx/index.php?title=FFmpeg_Metadata
*/
const QString MetaData::translateKeyword ( const QString &key )
{
  QHash<QString,QString> hash;
  hash["title"] = "title";
  hash["author"] = "author";
  hash["copyright"] = "copyright";
  hash["year"] = "year";
  hash["subject"] = "subject";
  hash["description"] = "comment";
  hash["language"] = "language";
  // Updates
  if ( mType.contains ( "avi", Qt::CaseInsensitive ) )
  {
    hash["title"] = "INAM";
    hash["author"] = "IART";
    hash["copyright"] = "ICOP";
    hash["year"] = "ICRD";
    hash["subject"] = "ISBJ";
    hash["description"] = "ICMT"; // e.g. comment
    hash["language"] = "ILNG";
  }

  return hash[key];
}

/**
* Umleitung auf signal @ref postUpdate
*/
void MetaData::statusUpdate ( bool )
{
  emit postUpdate();
}

/**
* Einstellungen Laden
*/
void MetaData::load ( QSettings * cfg )
{
  setChecked ( cfg->value ( QLatin1String ( "Metadata" ) ).toBool() );
  foreach ( QLineEdit* edit, metadataObjects() )
  {
    edit->setText ( cfg->value ( edit->objectName(), edit->text() ).toString() );
  }
}

/**
* Einstellungen Speichern
*/
void MetaData::save ( QSettings * cfg )
{
  cfg->setValue ( QLatin1String ( "Metadata" ), isChecked() );
  foreach ( QLineEdit* edit, metadataObjects() )
  {
    if ( edit->text().isEmpty() )
      cfg->remove ( edit->objectName() );
    else
      cfg->setValue ( edit->objectName(), edit->text() );
  }
}

/**
* Setze MediaType für die Schlüsselwörter
*/
void MetaData::setMediaType ( const QString &t )
{
  mType = t;
}

/**
* Verwendeter Media type für die Schlüsselwörter
*/
const QString MetaData::mediaType()
{
  return mType;
}

/**
* Einlesen der aktuellen Metadaten
* \todo Je nach Codec den Schlüssel ändern!
*/
const QStringList MetaData::getCmd ( const QString &codec )
{
  QStringList cmd;

  if ( codec.contains ( "avi", Qt::CaseInsensitive ) )
    setMediaType ( "avi" );
  else
    setMediaType ( "mpeg" );

  foreach ( QLineEdit* edit, metadataObjects() )
  {
    if ( edit->text().isEmpty() )
      continue;

    QString param ( edit->objectName() );
    QString key = param.remove ( "Metadata/" );
    QString data = QString ( edit->text() ).replace ( QRegExp ( "[\\t\\n\\r]+" ), " " );
    cmd << "-metadata" << QString ( "%1=\"%2\"" ).arg ( translateKeyword ( key ), data );
  }

  cmd << "-metadata" << QString ( "year=%1" ).arg ( metadata_ICRD->date().year() );
  // DEPRECATED timestamp use "creation_time Key ISO 8601"
  QString date = metadata_ICRD->dateTime().toString ( Qt::ISODate );
  cmd << "-metadata" << QString ( "date=%1" ).arg ( date );
  cmd << "-metadata" << QString ( "creation_time=%1" ).arg ( date );

  return cmd;
}

MetaData::~MetaData()
{}
