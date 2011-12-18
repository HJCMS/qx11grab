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

#include "grabberinfo.h"
#include "screencombobox.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QRect>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QPalette>
#include <QtGui/QSpacerItem>
#include <QtGui/QX11Info>

GrabberInfo::GrabberInfo ( QWidget * parent )
    : QWidget ( parent )
{
  setObjectName ( QLatin1String ( "grabberinfo" ) );
  setBackgroundRole ( QPalette::Window );

  QX11Info xinfo;
  QRect desktopRect = QApplication::desktop()->availableGeometry ( xinfo.screen() );

  int grow = 0;
  QGridLayout* gridLayout = new QGridLayout ( this );
  gridLayout->setContentsMargins ( 2, 2, 2, 2 );
  gridLayout->setObjectName ( QLatin1String ( "gridLayout" ) );

  Qt::Alignment labelAlignment = ( Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter );

  // begin: Size
  QLabel* txt1 = new QLabel ( this );
  txt1->setText ( trUtf8 ( "Dimension:" ) );
  txt1->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt1, grow, 0, 1, 1 );

  screenComboBox = new ScreenComboBox ( this );
  gridLayout->addWidget ( screenComboBox, grow++, 1, 1, 2 );
  // end: Size

  // begin: Width
  QLabel* txt2 = new QLabel ( this );
  txt2->setText ( trUtf8 ( "Width:" ) );
  txt2->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt2, grow, 0, 1, 1 );

  setWidthBox = new QSpinBox ( this );
  setWidthBox->setObjectName ( QLatin1String ( "setWidthBox" ) );
  setWidthBox->setRange ( 10, 2800 );
  setWidthBox->setValue ( 100 );
  gridLayout->addWidget ( setWidthBox, grow, 1, 1, 1 );

  setWidthSlider = new QSlider ( Qt::Horizontal, this );
  setWidthSlider->setObjectName ( QLatin1String ( "setWidthSlider" ) );
  setWidthSlider->setSingleStep ( 2 );
  setWidthSlider->setMinimum ( desktopRect.left() );
  setWidthSlider->setMaximum ( desktopRect.right() );
  gridLayout->addWidget ( setWidthSlider, grow++, 2, 1, 1 );
  // end: Width

  // begin: Height
  QLabel* txt3 = new QLabel ( this );
  txt3->setText ( trUtf8 ( "Height:" ) );
  txt3->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt3, grow, 0, 1, 1 );

  setHeightBox = new QSpinBox ( this );
  setHeightBox->setObjectName ( QLatin1String ( "setHeightBox" ) );
  setHeightBox->setRange ( 10, 2800 );
  setHeightBox->setValue ( 96 );
  gridLayout->addWidget ( setHeightBox, grow, 1, 1, 1 );

  setHeightSlider = new QSlider ( Qt::Horizontal, this );
  setHeightSlider->setObjectName ( QLatin1String ( "setHeightSlider" ) );
  setHeightSlider->setSingleStep ( 2 );
  setHeightSlider->setMinimum ( desktopRect.left() );
  setHeightSlider->setMaximum ( desktopRect.right() );
  gridLayout->addWidget ( setHeightSlider, grow++, 2, 1, 1 );
  // end: Height

  // begin: Format Mode Name
  QLabel* txt4 = new QLabel ( this );
  txt4->setText ( trUtf8 ( "Name:" ) );
  txt4->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt4, grow, 0, 1, 1 );

  setModeName = new QLineEdit ( this );
  setModeName->setObjectName ( QLatin1String ( "setModeName" ) );
  gridLayout->addWidget ( setModeName, grow++, 1, 1, 2 );
  // end: Format Mode Name

  // begin: X Position
  QLabel* txt5 = new QLabel ( this );
  txt5->setText ( trUtf8 ( "Position X:" ) );
  txt5->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt5, grow, 0, 1, 1 );

  setXBox = new QSpinBox ( this );
  setXBox->setObjectName ( QLatin1String ( "setXBox" ) );
  setXBox->setRange ( 0, 5000 );
  setXBox->setValue ( 0 );
  gridLayout->addWidget ( setXBox, grow, 1, 1, 1 );

  setXSlider = new QSlider ( Qt::Horizontal, this );
  setXSlider->setObjectName ( QLatin1String ( "setXSlider" ) );
  setXSlider->setSingleStep ( 1 );
  setXSlider->setMinimum ( desktopRect.left() );
  setXSlider->setMaximum ( desktopRect.right() );
  gridLayout->addWidget ( setXSlider, grow++, 2, 1, 1 );
  // end: X Position

  // begin: Y Position
  QLabel* txt6 = new QLabel ( this );
  txt6->setText ( trUtf8 ( "Position Y:" ) );
  txt6->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt6, grow, 0, 1, 1 );

  setYBox = new QSpinBox ( this );
  setYBox->setObjectName ( QLatin1String ( "setYBox" ) );
  setYBox->setRange ( 0, 5000 );
  setYBox->setValue ( 30 );
  gridLayout->addWidget ( setYBox, grow, 1, 1, 1 );

  setYSlider = new QSlider ( Qt::Horizontal, this );
  setYSlider->setObjectName ( QLatin1String ( "setYSlider" ) );
  setYSlider->setSingleStep ( 1 );
  setYSlider->setMinimum ( desktopRect.top() );
  setYSlider->setMaximum ( desktopRect.bottom() );
  gridLayout->addWidget ( setYSlider, grow++, 2, 1, 1 );
  // end: Y Position

  // begin: Desktop Color Depth
  QLabel* txt7 = new QLabel ( this );
  txt7->setText ( trUtf8 ( "Depth:" ) );
  txt7->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt7, grow, 0, 1, 1 );

  QSpinBox* depth = new QSpinBox ( this );
  depth->setDisabled ( true );
  gridLayout->addWidget ( depth, grow++, 1, 1, 2 );
  // end: Desktop Color Depth

  // begin: Frame Rate
  QLabel* txt8 = new QLabel ( this );
  txt8->setText ( trUtf8 ( "Framerate:" ) );
  txt8->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt8, grow, 0, 1, 1 );

  setFrameRate = new QSpinBox ( this );
  setFrameRate->setRange ( 0, 150 );
  setFrameRate->setValue ( 25 );
  gridLayout->addWidget ( setFrameRate, grow++, 1, 1, 2 );
  // end: Frame Rate

  QGridLayout* checkBoxLayout = new QGridLayout;

  showRubberband = new QCheckBox ( this );
  showRubberband->setText ( trUtf8 ( "Display Rubberband" ) );
  showRubberband->setChecked ( true );
  checkBoxLayout->addWidget ( showRubberband, 0, 0, 1, 1 );

  startMinimized = new QCheckBox ( this );
  startMinimized->setText ( trUtf8 ( "Start Minimized" ) );
  startMinimized->setChecked ( true );
  checkBoxLayout->addWidget ( startMinimized, 0, 1, 1, 1 );

  setMetadata = new QCheckBox ( this );
  setMetadata->setText ( trUtf8 ( "Insert Metadata" ) );
  setMetadata->setChecked ( true );
  checkBoxLayout->addWidget ( setMetadata, 1, 0, 1, 1 );

  soundRecording = new QCheckBox ( this );
  soundRecording->setText ( trUtf8 ( "Enable Audio Recording" ) );
  soundRecording->setChecked ( true );
  checkBoxLayout->addWidget ( soundRecording, 1, 1, 1, 1 );

  gridLayout->addItem ( checkBoxLayout, grow++, 0, 1, 3 );

  QSpacerItem* spacer  = new QSpacerItem ( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
  gridLayout->addItem ( spacer, grow++, 0, 1, 2 );

  setLayout ( gridLayout );

  connect ( screenComboBox, SIGNAL ( screenNameChanged ( const QString & ) ),
            setModeName, SLOT ( setText ( const QString & ) ) );

  connect ( screenComboBox, SIGNAL ( screenWidthChanged ( int ) ),
            setWidthBox, SLOT ( setValue ( int ) ) );

  connect ( screenComboBox, SIGNAL ( screenHeightChanged ( int ) ),
            setHeightBox, SLOT ( setValue ( int ) ) );

  connect ( screenComboBox, SIGNAL ( screenDepthChanged ( int ) ),
            depth, SLOT ( setValue ( int ) ) );

  connect ( setWidthBox, SIGNAL ( valueChanged ( int ) ),
            this, SIGNAL ( screenDataChanged ( int ) ) );

  connect ( setWidthBox, SIGNAL ( valueChanged ( int ) ),
            setWidthSlider, SLOT ( setValue ( int ) ) );

  connect ( setWidthSlider, SIGNAL ( valueChanged ( int ) ),
            this, SLOT ( sliderUpdateChanged ( int ) ) );

  connect ( setHeightBox, SIGNAL ( valueChanged ( int ) ),
            this, SIGNAL ( screenDataChanged ( int ) ) );

  connect ( setHeightBox, SIGNAL ( valueChanged ( int ) ),
            setHeightSlider, SLOT ( setValue ( int ) ) );

  connect ( setHeightSlider, SIGNAL ( valueChanged ( int ) ),
            this, SLOT ( sliderUpdateChanged ( int ) ) );

  connect ( setXBox, SIGNAL ( valueChanged ( int ) ),
            this, SIGNAL ( screenDataChanged ( int ) ) );

  connect ( setXBox, SIGNAL ( valueChanged ( int ) ),
            setXSlider, SLOT ( setValue ( int ) ) );

  connect ( setXSlider, SIGNAL ( valueChanged ( int ) ),
            this, SLOT ( sliderUpdateChanged ( int ) ) );

  connect ( setYBox, SIGNAL ( valueChanged ( int ) ),
            this, SIGNAL ( screenDataChanged ( int ) ) );

  connect ( setYBox, SIGNAL ( valueChanged ( int ) ),
            setYSlider, SLOT ( setValue ( int ) ) );

  connect ( setYSlider, SIGNAL ( valueChanged ( int ) ),
            this, SLOT ( sliderUpdateChanged ( int ) ) );

  connect ( setFrameRate, SIGNAL ( valueChanged ( int ) ),
            this, SIGNAL ( screenDataChanged ( int ) ) );

  connect ( showRubberband, SIGNAL ( toggled ( bool ) ),
            this, SIGNAL ( showRubber ( bool ) ) );

  // Updates
  connect ( setMetadata, SIGNAL ( stateChanged ( int ) ),
            this, SIGNAL ( postUpdate () ) );

  connect ( soundRecording, SIGNAL ( stateChanged ( int ) ),
            this, SIGNAL ( postUpdate () ) );
}

/**
* Wenn einer der Slider sich bewegt
*/
void GrabberInfo::sliderUpdateChanged ( int )
{
  showRubberband->setChecked ( true );
  setXBox->setValue ( setXSlider->value() );
  setYBox->setValue ( setYSlider->value() );
  if ( ( setWidthSlider->value() % 2 ) == 0 )
    setWidthBox->setValue ( setWidthSlider->value() );
  if ( ( setHeightSlider->value() % 2 ) == 0 )
    setHeightBox->setValue ( setHeightSlider->value() );
}

/**
* Widget Einstellungen suchen und laden.
*/
void GrabberInfo::load ( QSettings *cfg )
{
  showRubberband->setChecked ( cfg->value ( QLatin1String ( "showRubberband" ) ).toBool() );
  startMinimized->setChecked ( cfg->value ( QLatin1String ( "startMinimized" ) ).toBool() );
  soundRecording->setChecked ( cfg->value ( QLatin1String ( "SoundRecording" ) ).toBool() );
  setMetadata->setChecked ( cfg->value ( QLatin1String ( "Metadata" ) ).toBool() );
}

/**
* Aktuelle Widget Einstellungen speichern.
*/
void GrabberInfo::save ( QSettings *cfg )
{
  cfg->setValue ( QLatin1String ( "showRubberband" ), showRubberband->isChecked() );
  cfg->setValue ( QLatin1String ( "startMinimized" ), startMinimized->isChecked() );
  cfg->setValue ( QLatin1String ( "SoundRecording" ), soundRecording->isChecked() );
  cfg->setValue ( QLatin1String ( "Metadata" ), setMetadata->isChecked() );
}

/**
* Soll das Gummiband angezeigt werden?
*/
bool GrabberInfo::RubberbandIsVisible()
{
  return showRubberband->isChecked();
}

/**
* Ist die Audio Aufnahme aktiviert?
*/
bool GrabberInfo::soundEnabled()
{
  return soundRecording->isChecked ();
}

/**
* Sollen die Kopfdaten eingefügt werden?
*/
bool GrabberInfo::metaEnabled()
{
  return setMetadata->isChecked ();
}

/**
* SLOT für die Anzeige des Gummibandes
*/
void GrabberInfo::setRubberbandCheckBox ( bool b )
{
  showRubberband->setChecked ( b );
}

/**
* Aufnahme Breite in Pixel
*/
void GrabberInfo::setScreenWidth ( int w )
{
  setWidthBox->setValue ( w );
}

/**
* Aufnahme Höhe in Pixel
*/
void GrabberInfo::setScreenHeight ( int h )
{
  setHeightBox->setValue ( h );
}

/**
* Der Horizontale Aufnahme Startpunkt
*/
void GrabberInfo::setScreenX ( int x )
{
  setXBox->setValue ( x );
}

/**
* Der Vertikale Aufnahme Startpunkt
*/
void GrabberInfo::setScreenY ( int y )
{
  setYBox->setValue ( y );
}

/**
* Gibt die Aktuelle Dimension zurück.
* Das Rückgabe format ist "WxH" oder z.B: "svga" Zeichenkette.
*/
void GrabberInfo::setScreenMode ( const QString &mode )
{
  setModeName->setText ( mode );
}

/**
* Gibt die Aktuelle Dimension in form eines QRect zurück.
*/
const QRect GrabberInfo::getRect()
{
  return QRect ( setXBox->value(), setYBox->value(), setWidthBox->value(), setHeightBox->value() );
}

/**
* Gibt die Aktuelle FrameRate zurück.
*/
int GrabberInfo::frameRate()
{
  int rate = setFrameRate->value();
  return ( rate > 0 ) ? rate : 25;
}

GrabberInfo::~GrabberInfo()
{}