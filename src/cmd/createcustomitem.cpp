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

#include "createcustomitem.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFormLayout>
#include <QtGui/QIcon>
#include <QtGui/QLabel>
#include <QtGui/QSizePolicy>

CreateCustomItem::CreateCustomItem ( QWidget * parent )
    : QDialog ( parent )
{
  setObjectName ( QLatin1String ( "CreateCustomItem" ) );

  QFormLayout* layout = new QFormLayout ( this );
  layout->setObjectName ( QLatin1String ( "CreateCustomItem/Layout" ) );

  QLabel* m_info = new QLabel ( trUtf8 ( "Create a custom entry" ) , this );
  layout->addRow ( m_info );

  lineEdit = new QLineEdit ( this );
  lineEdit->setObjectName ( QLatin1String ( "CreateCustomItem/LineEdit" ) );
  layout->addRow ( lineEdit );

  QDialogButtonBox* m_buttonBox = new QDialogButtonBox ( Qt::Horizontal, this );
  m_buttonBox->setObjectName ( QLatin1String ( "CreateCustomItem/ButtonBox" ) );
  m_buttonBox->setStandardButtons ( ( QDialogButtonBox::Ok | QDialogButtonBox::Cancel ) );
  layout->addRow ( m_buttonBox );

  setLayout ( layout );

  connect ( m_buttonBox, SIGNAL ( accepted () ), this, SLOT ( accept() ) );
  connect ( m_buttonBox, SIGNAL ( rejected () ), this, SLOT ( reject() ) );
}

CreateCustomItem::~CreateCustomItem()
{}
