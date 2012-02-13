/**
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann (Undefined) http://qx11grab.hjcms.de, (C) 2007-2012
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

#include "fade.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QDialogButtonBox>
#include <QtGui/QLabel>
#include <QtGui/QIcon>
#include <QtGui/QWidget>
#include <QtGui/QVBoxLayout>

fade::fade ( QWidget * parent )
    : QDialog ( parent )
{
  setObjectName ( QLatin1String ( "fade" ) );
  setWindowTitle ( trUtf8 ( "Fade (In/Out) Filter" ) );
  setWindowIcon ( QIcon::fromTheme ( "preferences-plugin" ) );
  setSizeGripEnabled ( true );
  setMinimumSize ( 100, 100 );

  cfg = new QSettings ( QSettings::NativeFormat, QSettings::UserScope, "hjcms.de", "qx11grab", this );

  QVBoxLayout* layout = new QVBoxLayout ( this );
  layout->setObjectName ( QLatin1String ( "fade/Layout" ) );

  QLabel* info0 = new QLabel ( this );
  info0->setText ( trUtf8 ( "Description: %1" ).arg ( "fade=type:start_frame:nb_frames[:options]" ) );
  layout->addWidget ( info0 );

  QLabel* info1 = new QLabel ( this );
  info1->setText ( trUtf8 ( "Example: %1" ).arg ( "fade=in:0:25:alpha=1" ) );
  layout->addWidget ( info1 );

  m_lineEdit = new QLineEdit ( this );
  layout->addWidget ( m_lineEdit );

  QDialogButtonBox* m_buttonBox = new QDialogButtonBox ( Qt::Horizontal, this );
  m_buttonBox->setObjectName ( QLatin1String ( "fade/ButtonBox" ) );
  m_buttonBox->setStandardButtons ( ( QDialogButtonBox::Save | QDialogButtonBox::Cancel ) );
  layout->addWidget ( m_buttonBox );

  setLayout ( layout );

  connect ( m_buttonBox, SIGNAL ( accepted() ), this, SLOT ( accept() ) );
  connect ( m_buttonBox, SIGNAL ( rejected() ), this, SLOT ( reject() ) );
}

void fade::setSettings ( const QString &key, const QVariant &value )
{
  QString path = QString::fromUtf8 ( "Filter_fade/%1" ).arg ( key );
  cfg->setValue ( path, value );
}

const QVariant fade::settingsValue ( const QString &key, const QVariant &defaultValue )
{
  QString path = QString::fromUtf8 ( "Filter_fade/%1" ).arg ( key );
  return cfg->value ( path, defaultValue );
}

void fade::loadDefaults()
{
  update();
}

void fade::update()
{
  QString value ( "fade=in:0:25:alpha=1" );
  m_lineEdit->setText ( value );
}

/** den Dialog starten */
int fade::start()
{
  loadDefaults(); // set defaults
  return exec();
}

const QString fade::data()
{
  return m_lineEdit->text();
}

fade::~fade()
{}

