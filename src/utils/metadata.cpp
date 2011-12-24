/**
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
**/

#include "metadata.h"

/* QtCore */
#include <QtCore/QLocale>
#include <QtCore/QRegExp>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QGridLayout>
#include <QtGui/QSpacerItem>
#include <QtGui/QLabel>
#include <QtGui/QPalette>

MetaData::MetaData ( QWidget * parent )
    : QWidget ( parent )
{
  setObjectName ( QLatin1String ( "metadata" ) );

  int grow = 0;
  QGridLayout* gridLayout = new QGridLayout ( this );
  gridLayout->setContentsMargins ( 2, 2, 2, 2 );
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
  metadata_INAM->setObjectName ( QLatin1String ( "metadata/title" ) );
  metadata_INAM->setToolTip ( QLatin1String ( "INAM" ) );
  gridLayout->addWidget ( metadata_INAM, grow++, 1, 1, 1 );

  QLabel* txt_artist = new QLabel ( this );
  txt_artist->setText ( trUtf8 ( "Artist:" ) );
  txt_artist->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt_artist, grow, 0, 1, 1 );

  metadata_IART = new QLineEdit ( this );
  metadata_IART->setObjectName ( QLatin1String ( "metadata/author" ) );
  metadata_IART->setToolTip ( QLatin1String ( "IART" ) );
  gridLayout->addWidget ( metadata_IART, grow++, 1, 1, 1 );

  QLabel* txt_copyright = new QLabel ( this );
  txt_copyright->setText ( trUtf8 ( "Copyright:" ) );
  txt_copyright->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt_copyright, grow, 0, 1, 1 );

  metadata_ICOP = new QLineEdit ( this );
  metadata_ICOP->setObjectName ( QLatin1String ( "metadata/copyright" ) );
  metadata_ICOP->setToolTip ( QLatin1String ( "ICOP" ) );
  gridLayout->addWidget ( metadata_ICOP, grow++, 1, 1, 1 );

  QLabel* txt_createdate = new QLabel ( this );
  txt_createdate->setText ( trUtf8 ( "Creation Date:" ) );
  txt_createdate->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt_createdate, grow, 0, 1, 1 );

  metadata_ICRD = new QDateTimeEdit ( this );
  metadata_ICRD->setObjectName ( QLatin1String ( "metadata/year" ) );
  metadata_ICRD->setToolTip ( QLatin1String ( "ICRD" ) );
  metadata_ICRD->setDateTime ( QDateTime::currentDateTime() );
  metadata_ICRD->setCalendarPopup ( true );
  gridLayout->addWidget ( metadata_ICRD, grow++, 1, 1, 1, Qt::AlignLeft );

  QLabel* txt_subject = new QLabel ( this );
  txt_subject->setText ( trUtf8 ( "Subject:" ) );
  txt_subject->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt_subject, grow, 0, 1, 1 );

  metadata_ISBJ = new QLineEdit ( this );
  metadata_ISBJ->setObjectName ( QLatin1String ( "metadata/subject" ) );
  metadata_ISBJ->setToolTip ( QLatin1String ( "ISBJ" ) );
  gridLayout->addWidget ( metadata_ISBJ, grow++, 1, 1, 1 );

  QLabel* txt_description = new QLabel ( this );
  txt_description->setText ( trUtf8 ( "Description:" ) );
  txt_description->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt_description, grow, 0, 1, 1 );

  metadata_ICMT = new QLineEdit ( this );
  metadata_ICMT->setObjectName ( QLatin1String ( "metadata/description" ) );
  metadata_ICMT->setToolTip ( QLatin1String ( "ICMT" ) );
  gridLayout->addWidget ( metadata_ICMT, grow++, 1, 1, 1 );

  QLabel* txt_language = new QLabel ( this );
  txt_language->setText ( trUtf8 ( "Language:" ) );
  txt_language->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt_language, grow, 0, 1, 1 );

  /* main language in which the work is performed, preferably in ISO 639-2 format.
  * Multiple languages can be specified by separating them with commas. */
  metadata_LANG = new QLineEdit ( this );
  metadata_LANG->setObjectName ( QLatin1String ( "metadata/language" ) );
  metadata_LANG->setToolTip ( QLatin1String ( "Language in ISO 639-2 format. Multiple languages can be specified by separating them with commas." ) );
  gridLayout->addWidget ( metadata_LANG, grow++, 1, 1, 1 );

  QSpacerItem* spacer  = new QSpacerItem ( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
  gridLayout->addItem ( spacer, grow++, 0, 1, 2 );

  setLayout ( gridLayout );
}

/**
* Einstellungen Laden
* http://wiki.multimedia.cx/index.php?title=FFmpeg_Metadata
*/
const QList<QLineEdit*> MetaData::metadataObjects()
{
  QList<QLineEdit*> items;
  items << metadata_INAM << metadata_IART << metadata_ICOP;
  items << metadata_ISBJ << metadata_ICMT << metadata_LANG;
  return items;
}

/**
* Einstellungen Laden
*/
void MetaData::load ( QSettings * cfg )
{
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
  foreach ( QLineEdit* edit, metadataObjects() )
  {
    if ( edit->text().isEmpty() )
      cfg->remove ( edit->objectName() );
    else
      cfg->setValue ( edit->objectName(), edit->text() );
  }
}

/**
* Einlesen der aktuellen Metadaten
* \todo Je nach Codec den Schlüssel ändern!
*/
const QStringList MetaData::getCmd ( const QString &codec )
{
  Q_UNUSED ( codec );

  QStringList cmd;

  foreach ( QLineEdit* edit, metadataObjects() )
  {
    if ( edit->text().isEmpty() )
      continue;

    QString param ( edit->objectName() );
    cmd << "-metadata" << QString ( "%1=\"%2\"" ).arg ( param.remove ( "metadata/" ), edit->text() );
  }

  cmd << "-metadata" << QString ( "year=\"%1\"" ).arg ( metadata_ICRD->date().year() );
  // DEPRECATED creation_time Key ISO 8601
  cmd << "-metadata" << QString ( "creation_time=\"%1\"" ).arg ( metadata_ICRD->dateTime().toString ( Qt::ISODate ) );

  return cmd;
}

MetaData::~MetaData()
{}
