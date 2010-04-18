/**
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann http://qx11grab.hjcms.de, (C) 2007-2010
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
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QGridLayout>
#include <QtGui/QSpacerItem>
#include <QtGui/QLabel>

MetaData::MetaData ( QWidget * parent )
    : QWidget ( parent )
{
  setObjectName ( QLatin1String ( "metadata" ) );

  QGridLayout* gridLayout = new QGridLayout ( this );
  gridLayout->setContentsMargins ( 2, 2, 2, 2 );
  gridLayout->setObjectName ( QLatin1String ( "gridLayout" ) );

  Qt::Alignment labelAlignment = ( Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter );

  QLabel* txt_title = new QLabel ( this );
  txt_title->setText ( trUtf8 ( "Title:" ) );
  txt_title->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt_title, 0, 0, 1, 1 );

  metadata_INAM = new QLineEdit ( this );
  metadata_INAM->setObjectName ( QLatin1String ( "metadata/INAM" ) );
  gridLayout->addWidget ( metadata_INAM, 0, 1, 1, 1 );

  QLabel* txt_artist = new QLabel ( this );
  txt_artist->setText ( trUtf8 ( "Artist:" ) );
  txt_artist->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt_artist, 1, 0, 1, 1 );

  metadata_IART = new QLineEdit ( this );
  metadata_IART->setObjectName ( QLatin1String ( "metadata/IART" ) );
  gridLayout->addWidget ( metadata_IART, 1, 1, 1, 1 );

  QLabel* txt_copyright = new QLabel ( this );
  txt_copyright->setText ( trUtf8 ( "Copyright:" ) );
  txt_copyright->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt_copyright, 2, 0, 1, 1 );

  metadata_ICOP = new QLineEdit ( this );
  metadata_ICOP->setObjectName ( QLatin1String ( "metadata/ICOP" ) );
  gridLayout->addWidget ( metadata_ICOP, 2, 1, 1, 1 );

  QLabel* txt_createdate = new QLabel ( this );
  txt_createdate->setText ( trUtf8 ( "Creation Date:" ) );
  txt_createdate->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt_createdate, 3, 0, 1, 1 );

  metadata_ICRD = new QDateTimeEdit ( this );
  metadata_ICRD->setObjectName ( QLatin1String ( "metadata/ICRD" ) );
  gridLayout->addWidget ( metadata_ICRD, 3, 1, 1, 1 );

  QLabel* txt_subject = new QLabel ( this );
  txt_subject->setText ( trUtf8 ( "Subject:" ) );
  txt_subject->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt_subject, 4, 0, 1, 1 );

  metadata_ISBJ = new QLineEdit ( this );
  metadata_ISBJ->setObjectName ( QLatin1String ( "metadata/ISBJ" ) );
  gridLayout->addWidget ( metadata_ISBJ, 4, 1, 1, 1 );

  QLabel* txt_description = new QLabel ( this );
  txt_description->setText ( trUtf8 ( "Description:" ) );
  txt_description->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt_description, 5, 0, 1, 1 );

  metadata_ICMT = new QLineEdit ( this );
  metadata_ICMT->setObjectName ( QLatin1String ( "metadata/ICMT" ) );
  gridLayout->addWidget ( metadata_ICMT, 5, 1, 1, 1 );

  QLabel* txt_medium = new QLabel ( this );
  txt_medium->setText ( trUtf8 ( "Medium:" ) );
  txt_medium->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt_medium, 6, 0, 1, 1 );

  metadata_IMED = new QLineEdit ( this );
  metadata_IMED->setObjectName ( QLatin1String ( "metadata/IMED" ) );
  gridLayout->addWidget ( metadata_IMED, 6, 1, 1, 1 );

  QSpacerItem* spacer  = new QSpacerItem ( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
  gridLayout->addItem ( spacer, 7, 0, 1, 2 );

  setLayout ( gridLayout );
}

/**
* Einstellungen Laden
*/
void MetaData::load ( QSettings * cfg )
{
  QList<QLineEdit*> items;
  items << metadata_INAM << metadata_IART << metadata_ICOP << metadata_ISBJ << metadata_ICMT << metadata_IMED;

  foreach ( QLineEdit* edit, items )
  {
    edit->setText ( cfg->value ( edit->objectName() ).toString() );
  }
}

/**
* Einstellungen Speichern
*/
void MetaData::save ( QSettings * cfg )
{
  QList<QLineEdit*> items;
  items << metadata_INAM << metadata_IART << metadata_ICOP << metadata_ISBJ << metadata_ICMT << metadata_IMED;

  foreach ( QLineEdit* edit, items )
  {
    if ( edit->text().isEmpty() )
      cfg->remove ( edit->objectName() );
    else
      cfg->setValue ( edit->objectName(), edit->text() );
  }
}

MetaData::~MetaData()
{}
