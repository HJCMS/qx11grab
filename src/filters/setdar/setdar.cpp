/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann (Undefined) http://qx11grab.hjcms.de, (C) 2007-2013
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

#include "setdar.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFormLayout>
#include <QtGui/QLabel>
#include <QtGui/QIcon>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

SetDar::SetDar ( QWidget * parent )
    : QDialog ( parent )
    , cfg ( 0 )
{
  setObjectName ( QLatin1String ( "SetDar" ) );
  setWindowTitle ( trUtf8 ( "DAR Filter" ) );
  setWindowIcon ( QIcon::fromTheme ( "preferences-plugin" ) );
  setSizeGripEnabled ( true );
  setMinimumSize ( 100, 100 );

  cfg = new QSettings ( QSettings::NativeFormat, QSettings::UserScope, "hjcms.de", "qx11grab", this );

  QVBoxLayout* vLayout = new QVBoxLayout ( this );

  QFormLayout* layout = new QFormLayout ();
  layout->setObjectName ( QLatin1String ( "SetDar/Layout" ) );
  vLayout->addLayout ( layout );

  QLabel* info0 = new QLabel ( this );
  info0->setWordWrap ( true );
  info0->setText ( trUtf8 ( "DAR - Display Aspect Ratio" ) );
  layout->addRow ( info0 );

  m_comboBox = new QComboBox ( this );
  // http://de.wikipedia.org/wiki/Seitenverhältnis_(Film)
  m_comboBox->addItem ( trUtf8 ( "Aspect Ratio" ), QString() );
  m_comboBox->addItem ( QString::fromUtf8 ( "4/3 (= 1,33:1)" ), QString ( "1.3333" ) );
  m_comboBox->addItem ( QString::fromUtf8 ( "15/9 (= 1,66:1)" ), QString ( "1.6666" ) );
  m_comboBox->addItem ( QString::fromUtf8 ( "16/9 (= 1,78:1)" ), QString ( "1.7777" ) );
  m_comboBox->addItem ( QString::fromUtf8 ( "21/9 (= 2,35:1)" ), QString ( "2.3333" ) );
  m_comboBox->setCurrentIndex ( cfg->value ( "Filter_setdar/index", 1 ).toUInt() );
  layout->addRow ( m_comboBox );

  QLabel* info1 = new QLabel ( this );
  info1->setWordWrap ( true );
  info1->setText ( trUtf8 ( "Keep in mind that this filter does not modify the pixel dimensions of the video frame. Also the display aspect ratio set by this filter may be changed by later filters in the filterchain." ) );
  layout->addRow ( info1 );

  m_lineEdit = new QLineEdit ( this );
  m_lineEdit->setText ( "setdar=1.7777" );
  layout->addRow ( m_lineEdit );

  vLayout->addStretch();

  QDialogButtonBox* m_buttonBox = new QDialogButtonBox ( Qt::Horizontal, this );
  m_buttonBox->setObjectName ( QLatin1String ( "SetDar/ButtonBox" ) );
  m_buttonBox->setStandardButtons ( ( QDialogButtonBox::Save | QDialogButtonBox::Cancel ) );
  vLayout->addWidget ( m_buttonBox );

  setLayout ( vLayout );

  connect ( m_comboBox, SIGNAL ( currentIndexChanged ( int ) ),
            this, SLOT ( update ( int ) ) );

  connect ( m_buttonBox, SIGNAL ( accepted () ), this, SLOT ( accept() ) );
  connect ( m_buttonBox, SIGNAL ( rejected () ), this, SLOT ( reject() ) );

  update ( m_comboBox->currentIndex() );
}

void SetDar::update ( int index )
{
  QString value ( "setdar=" );
  value.append ( m_comboBox->itemData ( index, Qt::UserRole ).toString() );
  m_lineEdit->setText ( value );
}

const QString SetDar::data()
{
  cfg->setValue ( "Filter_setdar/index", m_comboBox->currentIndex() );
  return m_lineEdit->text ();
}

SetDar::~SetDar()
{}

