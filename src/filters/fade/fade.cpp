/*
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
*/

#include "fade.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QIcon>
#include <QtGui/QWidget>

fade::fade ( QWidget * parent )
    : QDialog ( parent )
{
  setObjectName ( QLatin1String ( "fade" ) );
  setWindowTitle ( trUtf8 ( "Fade (In/Out) Filter" ) );
  setWindowIcon ( QIcon::fromTheme ( "preferences-plugin" ) );
  setSizeGripEnabled ( true );
  setMinimumSize ( 100, 100 );

  cfg = new QSettings ( QSettings::NativeFormat, QSettings::UserScope, "hjcms.de", "qx11grab", this );

  int grow = 0;
  QGridLayout* layout = new QGridLayout ( this );
  layout->setObjectName ( QLatin1String ( "fade/Layout" ) );

  QLabel* info0 = new QLabel ( this );
  info0->setText ( QString::fromUtf8 ( "<b>%1</b>: fade=type:start_frame:nb_frames[:options]" )
                   .arg ( trUtf8 ( "Description" ) ) );
  layout->addWidget ( info0, grow++, 0, 1, 2 );

  QLabel* info1 = new QLabel ( this );
  info1->setText ( QString::fromUtf8 ( "<b>%1</b>: %2 (fade=in:0:25:alpha=1)" )
                   .arg ( trUtf8 ( "Example" ), trUtf8 ( "Fade in the first 25 Frames and only use alpha channel." ) ) );
  layout->addWidget ( info1, grow++, 0, 1, 2 );

  // types Group
  QGroupBox* m_groupTypes = new QGroupBox ( trUtf8 ( "Type" ), this );
  QHBoxLayout* h1Layout = new QHBoxLayout ( m_groupTypes );
  m_groupTypes->setLayout ( h1Layout );

  m_in = new QRadioButton ( trUtf8 ( "in" ), m_groupTypes );
  m_in->setChecked ( true );
  h1Layout->addWidget ( m_in );

  m_out = new QRadioButton ( trUtf8 ( "out" ), m_groupTypes );
  h1Layout->addWidget ( m_out );

  layout->addWidget ( m_groupTypes, grow, 0, 1, 1 );

  // types Group
  QGroupBox* m_groupFrames = new QGroupBox ( trUtf8 ( "Frames" ), this );
  QHBoxLayout* h2Layout = new QHBoxLayout ( m_groupFrames );
  m_groupFrames->setLayout ( h2Layout );

  m_start_frame = new  QSpinBox ( m_groupFrames );
  m_start_frame->setValue ( 0 );
  m_start_frame->setSingleStep ( 2 );
  m_start_frame->setPrefix ( QString::fromUtf8 ( "start_frame=" ) );
  h2Layout->addWidget ( m_start_frame );

  m_nb_frames = new QSpinBox ( m_groupFrames );
  m_nb_frames->setValue ( 10 );
  m_nb_frames->setMinimum ( 2 );
  m_nb_frames->setSingleStep ( 2 );
  m_nb_frames->setPrefix ( QString::fromUtf8 ( "nb_frames=" ) );
  h2Layout->addWidget ( m_nb_frames );

  layout->addWidget ( m_groupFrames, grow++, 1, 1, 1 );

  m_alpha = new QCheckBox ( trUtf8 ( "Only alpha channel" ), this );
  layout->addWidget ( m_alpha, grow++, 0, 1, 2 );

  m_lineEdit = new QLineEdit ( this );
  layout->addWidget ( m_lineEdit, grow++, 0, 1, 2 );

  QDialogButtonBox* m_buttonBox = new QDialogButtonBox ( Qt::Horizontal, this );
  m_buttonBox->setObjectName ( QLatin1String ( "fade/ButtonBox" ) );
  m_buttonBox->setStandardButtons ( ( QDialogButtonBox::Save | QDialogButtonBox::Cancel ) );
  layout->addWidget ( m_buttonBox, grow++, 0, 1, 2 );

  setLayout ( layout );

  connect ( m_in, SIGNAL ( toggled ( bool ) ),
            this, SLOT ( updateChanged ( bool ) ) );

  connect ( m_out, SIGNAL ( toggled ( bool ) ),
            this, SLOT ( updateChanged ( bool ) ) );

  connect ( m_alpha, SIGNAL ( toggled ( bool ) ),
            this, SLOT ( updateChanged ( bool ) ) );

  connect ( m_start_frame, SIGNAL ( valueChanged ( int ) ),
            this, SLOT ( updateChanged ( int ) ) );

  connect ( m_nb_frames, SIGNAL ( valueChanged ( int ) ),
            this, SLOT ( updateChanged ( int ) ) );

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
  m_in->setChecked ( settingsValue ( "type", true ).toBool() );
  m_start_frame->setValue ( settingsValue ( "start_frame", 0 ).toUInt() );
  m_nb_frames->setValue ( settingsValue ( "nb_frames", 10 ).toUInt() );
  m_alpha->setChecked ( settingsValue ( "alpha", true ).toBool() );
  update();
}

void fade::updateChanged ( bool )
{
  update();
}

void fade::updateChanged ( int )
{
  update();
}

void fade::update()
{
  QString buffer;
  QString value = buffer.sprintf ( "fade=%s:%d:%d:alpha=%d",
                                   ( m_in->isChecked() ? "in" : "out" ),
                                   m_start_frame->value(),
                                   m_nb_frames->value(),
                                   ( m_alpha->isChecked() ? 1 : 0 ) );

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
  setSettings ( "type", m_in->isChecked() );
  setSettings ( "start_frame", m_start_frame->value() );
  setSettings ( "nb_frames", m_nb_frames->value() );
  setSettings ( "alpha", m_alpha->isChecked() );
  return m_lineEdit->text();
}

fade::~fade()
{}

