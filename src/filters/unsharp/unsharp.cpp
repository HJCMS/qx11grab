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

#include "unsharp.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFormLayout>
#include <QtGui/QLabel>
#include <QtGui/QIcon>
#include <QtGui/QWidget>

Unsharp::Unsharp ( QWidget * parent )
    : QDialog ( parent )
    , cfg ( 0 )
{
  setObjectName ( QLatin1String ( "Unsharp" ) );
  setWindowTitle ( trUtf8 ( "Unsharp Filter" ) );
  setWindowIcon ( QIcon::fromTheme ( "preferences-plugin" ) );
  setSizeGripEnabled ( true );
  setMinimumSize ( 100, 100 );

  cfg = new QSettings ( QSettings::NativeFormat, QSettings::UserScope, "hjcms.de", "qx11grab", this );

  QFormLayout* layout = new QFormLayout ( this );
  layout->setObjectName ( QLatin1String ( "Unsharp/Layout" ) );

  QLabel* info = new QLabel ( this );
  info->setWordWrap ( true );
  info->setText ( trUtf8 ( "Sharpen or blur the input" ) );
  layout->addRow ( info );

  luma_msize_x = new QSlider ( Qt::Horizontal, this );
  luma_msize_x->setRange ( 3, 13 );
  luma_msize_x->setTickInterval ( 1 );
  luma_msize_x->setTickPosition ( QSlider::TicksAbove );
  layout->addRow ( trUtf8 ( "horizontal luma matrix" ), luma_msize_x );

  luma_msize_y = new QSlider ( Qt::Horizontal, this );
  luma_msize_y->setRange ( 3, 13 );
  luma_msize_y->setTickInterval ( 1 );
  luma_msize_y->setTickPosition ( QSlider::TicksAbove );
  layout->addRow ( trUtf8 ( "vertical luma matrix" ), luma_msize_y );

  luma_amount = new QDoubleSpinBox ( this );
  luma_amount->setRange ( -2.0, 5.0 );
  luma_amount->setValue ( 0.4 );
  luma_amount->setSingleStep ( 0.1 );
  layout->addRow ( trUtf8 ( "luma effect strength" ), luma_amount );

  chroma_msize_x = new QSlider ( Qt::Horizontal, this );
  chroma_msize_x->setRange ( 3, 13 );
  chroma_msize_x->setTickInterval ( 1 );
  chroma_msize_x->setTickPosition ( QSlider::TicksAbove );
  layout->addRow ( trUtf8 ( "horizontal chroma matrix" ), chroma_msize_x );

  chroma_msize_y = new QSlider ( Qt::Horizontal, this );
  chroma_msize_y->setRange ( 3, 13 );
  chroma_msize_y->setTickInterval ( 1 );
  chroma_msize_y->setTickPosition ( QSlider::TicksAbove );
  layout->addRow ( trUtf8 ( "vertical chroma matrix" ), chroma_msize_y );

  chroma_amount = new QDoubleSpinBox ( this );
  chroma_amount->setRange ( -2.0, 5.0 );
  chroma_amount->setValue ( 0.0 );
  chroma_amount->setSingleStep ( 0.1 );
  layout->addRow ( trUtf8 ( "chroma effect strength" ), chroma_amount );

  m_preview = new QLineEdit ( this );
  layout->addRow ( m_preview );

  QDialogButtonBox* m_buttonBox = new QDialogButtonBox ( Qt::Horizontal, this );
  m_buttonBox->setObjectName ( QLatin1String ( "Unsharp/ButtonBox" ) );
  m_buttonBox->setStandardButtons ( ( QDialogButtonBox::Save | QDialogButtonBox::Cancel ) );
  layout->addRow ( m_buttonBox );

  setLayout ( layout );

  connect ( luma_msize_x, SIGNAL ( valueChanged ( int ) ), this, SLOT ( update ( int ) ) );
  connect ( luma_msize_y, SIGNAL ( valueChanged ( int ) ), this, SLOT ( update ( int ) ) );
  connect ( luma_amount, SIGNAL ( valueChanged ( double ) ), this, SLOT ( update ( double ) ) );
  connect ( chroma_msize_x, SIGNAL ( valueChanged ( int ) ), this, SLOT ( update ( int ) ) );
  connect ( chroma_msize_y, SIGNAL ( valueChanged ( int ) ), this, SLOT ( update ( int ) ) );
  connect ( chroma_amount, SIGNAL ( valueChanged ( double ) ), this, SLOT ( update ( double ) ) );
  connect ( m_buttonBox, SIGNAL ( accepted() ), this, SLOT ( accept() ) );
  connect ( m_buttonBox, SIGNAL ( rejected() ), this, SLOT ( reject() ) );

  loadDefaults(); // set defaults
}

void Unsharp::setSettings ( const QString &key, const QVariant &value )
{
  QString path = QString::fromUtf8 ( "Filter_Unsharp/%1" ).arg ( key );
  cfg->setValue ( path, value );
}

const QVariant Unsharp::settingsValue ( const QString &key, const QVariant &defaultValue )
{
  QString path = QString::fromUtf8 ( "Filter_Unsharp/%1" ).arg ( key );
  return cfg->value ( path, defaultValue );
}

void Unsharp::loadDefaults()
{
  luma_msize_x->setValue ( settingsValue ( "luma_msize_x", 3 ).toUInt() );
  luma_msize_y->setValue ( settingsValue ( "luma_msize_y", 3 ).toUInt() );
  luma_amount->setValue ( settingsValue ( "luma_amount", 0.2 ).toDouble() );
  chroma_msize_x->setValue ( settingsValue ( "chroma_msize_x", 3 ).toUInt() );
  chroma_msize_y->setValue ( settingsValue ( "chroma_msize_y", 3 ).toUInt() );
  chroma_amount->setValue ( settingsValue ( "chroma_amount", 0.0 ).toDouble() );
  update();
}

void Unsharp::update()
{
  QString buffer;
  QString value = buffer.sprintf ( "unsharp=%d:%d:%0.1f:%d:%d:%0.1f",
                                   luma_msize_x->value(),
                                   luma_msize_y->value(),
                                   luma_amount->value(),
                                   chroma_msize_x->value(),
                                   chroma_msize_y->value(),
                                   chroma_amount->value()
                                 );

  m_preview->setText ( value );
}

void Unsharp::update ( int )
{
  update();
}

void Unsharp::update ( double )
{
  update();
}

const QString Unsharp::data()
{
  setSettings ( "luma_msize_x", luma_msize_x->value() );
  setSettings ( "luma_msize_y", luma_msize_y->value() );
  setSettings ( "luma_amount", luma_amount->value() );
  setSettings ( "chroma_msize_x", chroma_msize_x->value() );
  setSettings ( "chroma_msize_y", chroma_msize_y->value() );
  setSettings ( "chroma_amount", chroma_amount->value() );
  return m_preview->text();
}

Unsharp::~Unsharp()
{}
