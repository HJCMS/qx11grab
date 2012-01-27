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
#include "desktopinfo.h"
#include "screencombobox.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QRect>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QtGui/QHBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QPalette>
#include <QtGui/QSpacerItem>

/**
* @class GrabberInfo
* Widget für Anzeige der Dimensionen
*/
GrabberInfo::GrabberInfo ( QWidget * parent )
    : QWidget ( parent )
    , screenGeometry ( QRect ( 0, 0, 100, 100 ) )
{
  setObjectName ( QLatin1String ( "grabberinfo" ) );
  setContentsMargins ( 5, 5, 5, 5 );

  int minBox = 80;

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
  /*: WhatsThis */
  screenComboBox->setWhatsThis ( trUtf8 ( "predefined frame sizes" ) );
  gridLayout->addWidget ( screenComboBox, grow++, 1, 1, 2 );
  // end: Size

  // begin: Width
  QLabel* txt2 = new QLabel ( this );
  txt2->setText ( trUtf8 ( "Width:" ) );
  txt2->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt2, grow, 0, 1, 1 );

  setWidthBox = new QSpinBox ( this );
  setWidthBox->setObjectName ( QLatin1String ( "setWidthBox" ) );
  setWidthBox->setMinimum ( minBox );
  setWidthBox->setValue ( minBox );
  /*: WhatsThis */
  setWidthBox->setWhatsThis ( trUtf8 ( "made rubberband witdh smaller/greater" ) );
  gridLayout->addWidget ( setWidthBox, grow, 1, 1, 1 );

  setWidthSlider = new QSlider ( Qt::Horizontal, this );
  setWidthSlider->setObjectName ( QLatin1String ( "setWidthSlider" ) );
  setWidthSlider->setSingleStep ( 2 );
  setWidthSlider->setMinimum ( minBox );
  /*: WhatsThis */
  setWidthSlider->setWhatsThis ( trUtf8 ( "made rubberband witdh smaller/greater" ) );
  gridLayout->addWidget ( setWidthSlider, grow++, 2, 1, 1 );
  // end: Width

  // begin: Height
  QLabel* txt3 = new QLabel ( this );
  txt3->setText ( trUtf8 ( "Height:" ) );
  txt3->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt3, grow, 0, 1, 1 );

  setHeightBox = new QSpinBox ( this );
  setHeightBox->setObjectName ( QLatin1String ( "setHeightBox" ) );
  setHeightBox->setMinimum ( minBox );
  setHeightBox->setValue ( minBox );
  /*: WhatsThis */
  setHeightBox->setWhatsThis ( trUtf8 ( "made rubberband height smaller/greater" ) );
  gridLayout->addWidget ( setHeightBox, grow, 1, 1, 1 );

  setHeightSlider = new QSlider ( Qt::Horizontal, this );
  setHeightSlider->setObjectName ( QLatin1String ( "setHeightSlider" ) );
  setHeightSlider->setSingleStep ( 2 );
  setHeightSlider->setMinimum ( minBox );
  /*: WhatsThis */
  setHeightSlider->setWhatsThis ( trUtf8 ( "made rubberband height smaller/greater" ) );
  gridLayout->addWidget ( setHeightSlider, grow++, 2, 1, 1 );
  // end: Height

  // begin: X Position
  QLabel* txt5 = new QLabel ( this );
  txt5->setText ( trUtf8 ( "Position X:" ) );
  txt5->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt5, grow, 0, 1, 1 );

  setXBox = new QSpinBox ( this );
  setXBox->setObjectName ( QLatin1String ( "setXBox" ) );
  setXBox->setMinimum ( 0 );
  setXBox->setValue ( 0 );
  /*: WhatsThis */
  setXBox->setWhatsThis ( trUtf8 ( "Displaces x-axis coordinates from Rubberband" ) );
  gridLayout->addWidget ( setXBox, grow, 1, 1, 1 );

  setXSlider = new QSlider ( Qt::Horizontal, this );
  setXSlider->setObjectName ( QLatin1String ( "setXSlider" ) );
  setXSlider->setSingleStep ( 1 );
  setXSlider->setMinimum ( 0 );
  /*: WhatsThis */
  setXSlider->setWhatsThis ( trUtf8 ( "Displaces x-axis coordinates from Rubberband" ) );
  gridLayout->addWidget ( setXSlider, grow++, 2, 1, 1 );
  // end: X Position

  // begin: Y Position
  QLabel* txt6 = new QLabel ( this );
  txt6->setText ( trUtf8 ( "Position Y:" ) );
  txt6->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt6, grow, 0, 1, 1 );

  setYBox = new QSpinBox ( this );
  setYBox->setObjectName ( QLatin1String ( "setYBox" ) );
  setYBox->setMinimum ( 0 );
  setYBox->setValue ( 0 );
  /*: WhatsThis */
  setYBox->setWhatsThis ( trUtf8 ( "Displaces y-axis coordinates from Rubberband" ) );
  gridLayout->addWidget ( setYBox, grow, 1, 1, 1 );

  setYSlider = new QSlider ( Qt::Horizontal, this );
  setYSlider->setObjectName ( QLatin1String ( "setYSlider" ) );
  setYSlider->setSingleStep ( 1 );
  setYSlider->setMinimum ( 0 );
  /*: WhatsThis */
  setYSlider->setWhatsThis ( trUtf8 ( "Displaces y-axis coordinates from Rubberband" ) );
  gridLayout->addWidget ( setYSlider, grow++, 2, 1, 1 );
  // end: Y Position

  QHBoxLayout* horizontalLayout = new QHBoxLayout;
  // begin: Desktop Screen
  QLabel* txt7 = new QLabel ( this );
  txt7->setText ( trUtf8 ( "Screen:" ) );
  txt7->setAlignment ( labelAlignment );
  horizontalLayout->addWidget ( txt7, Qt::AlignRight );

  m_screenBox = new QSpinBox ( this );
  m_screenBox->setRange ( 0, 8 );
  /*: WhatsThis */
  m_screenBox->setToolTip ( trUtf8 ( "current selected screen" ) );
  horizontalLayout->addWidget ( m_screenBox, Qt::AlignLeft );
  // end: Desktop Screen

  // begin: Desktop Color Depth
  QLabel* txt8 = new QLabel ( this );
  txt8->setText ( trUtf8 ( "Color depth:" ) );
  txt8->setAlignment ( labelAlignment );
  horizontalLayout->addWidget ( txt8, Qt::AlignRight );

  setDepth = new QSpinBox ( this );
  setDepth->setDisabled ( true );
  /*: WhatsThis */
  setDepth->setToolTip ( trUtf8 ( "current color depth" ) );
  horizontalLayout->addWidget ( setDepth, Qt::AlignLeft );
  // end: Desktop Color Depth

  // begin: Frame Rate
  QLabel* txt9 = new QLabel ( this );
  txt9->setText ( trUtf8 ( "Framerate:" ) );
  txt9->setAlignment ( labelAlignment );
  horizontalLayout->addWidget ( txt9, Qt::AlignRight );

  setFrameRate = new QSpinBox ( this );
  /*: WhatsThis */
  setFrameRate->setWhatsThis ( trUtf8 ( "Set the framerate in the captured video." ) );
  setFrameRate->setRange ( 0, 150 );
  setFrameRate->setValue ( 25 );
  horizontalLayout->addWidget ( setFrameRate, Qt::AlignLeft );
  // end: Frame Rate

  gridLayout->addLayout ( horizontalLayout, grow++, 0, 1, 3 );

  gridLayout->setRowStretch ( grow++, 1 );

  setLayout ( gridLayout );

  // Default Dimensions
  m_desktopInfo =  new DesktopInfo ( this );
  setInputDefaults ( m_desktopInfo->getScreen() );

  connect ( screenComboBox, SIGNAL ( screenWidthChanged ( int ) ),
            setWidthBox, SLOT ( setValue ( int ) ) );

  connect ( screenComboBox, SIGNAL ( screenHeightChanged ( int ) ),
            setHeightBox, SLOT ( setValue ( int ) ) );

  connect ( screenComboBox, SIGNAL ( screenDepthChanged ( int ) ),
            setDepth, SLOT ( setValue ( int ) ) );

  // SIGNALS:Width {
  connect ( setWidthBox, SIGNAL ( valueChanged ( int ) ),
            setWidthSlider, SLOT ( setValue ( int ) ) );

  connect ( setWidthSlider, SIGNAL ( valueChanged ( int ) ),
            setWidthBox, SLOT ( setValue ( int ) ) );

  connect ( setWidthBox, SIGNAL ( valueChanged ( int ) ),
            this, SLOT ( setRubberbandUpdate ( int ) ) );
  // } SIGNALS:Width
  // SIGNALS:Height {
  connect ( setHeightBox, SIGNAL ( valueChanged ( int ) ),
            setHeightSlider, SLOT ( setValue ( int ) ) );

  connect ( setHeightSlider, SIGNAL ( valueChanged ( int ) ),
            setHeightBox, SLOT ( setValue ( int ) ) );

  connect ( setHeightBox, SIGNAL ( valueChanged ( int ) ),
            this, SLOT ( setRubberbandUpdate ( int ) ) );
  // } SIGNALS:Height
  // SIGNALS:X-Position {
  connect ( setXBox, SIGNAL ( valueChanged ( int ) ),
            setXSlider, SLOT ( setValue ( int ) ) );

  connect ( setXSlider, SIGNAL ( valueChanged ( int ) ),
            setXBox, SLOT ( setValue ( int ) ) );

  connect ( setXBox, SIGNAL ( valueChanged ( int ) ),
            this, SLOT ( setRubberbandUpdate ( int ) ) );
  // } SIGNALS:X-Position
  // SIGNALS:Y-Position {
  connect ( setYBox, SIGNAL ( valueChanged ( int ) ),
            setYSlider, SLOT ( setValue ( int ) ) );

  connect ( setYSlider, SIGNAL ( valueChanged ( int ) ),
            setYBox, SLOT ( setValue ( int ) ) );

  connect ( setYBox, SIGNAL ( valueChanged ( int ) ),
            this, SLOT ( setRubberbandUpdate ( int ) ) );
  // } SIGNALS:Y-Position

  // Updates
  connect ( setFrameRate, SIGNAL ( valueChanged ( int ) ),
            this, SLOT ( integerUpdate ( int ) ) );

  connect ( m_screenBox, SIGNAL ( valueChanged ( int ) ),
            this, SLOT ( integerUpdate ( int ) ) );

  connect ( m_desktopInfo, SIGNAL ( resized ( int ) ),
            this, SLOT ( setInputDefaults ( int ) ) );
}

/**
* Umleitung Signal int nach void Update Datensatz
*/
void GrabberInfo::integerUpdate ( int )
{
  emit postUpdate();
}

/**
* Wenn der Benutzer die Aufkösung wechselt die
* Maximal Werte neu setzen!
*/
void GrabberInfo::setInputDefaults ( int screen )
{
  m_screenBox->setValue ( screen );
  screenGeometry = m_desktopInfo->screenGeometry ( screen );
  // Weidth
  setWidthBox->setMaximum ( ( screenGeometry.width() + 2 ) );
  setWidthSlider->setMaximum ( ( screenGeometry.width() + 2 ) );
  // Height
  setHeightBox->setMaximum ( ( screenGeometry.height() + 2 ) );
  setHeightSlider->setMaximum ( ( screenGeometry.height() + 2 ) );
  // X Postion
  setXBox->setMaximum ( screenGeometry.width() );
  setXSlider->setMaximum ( screenGeometry.width() );
  // Y Position
  setYBox->setMaximum ( screenGeometry.height() );
  setYSlider->setMaximum ( screenGeometry.height() );
}

/**
* Wenn einer der Slider/Spinboxen verändert!
* @note Das Gummiband darf nicht aus dem Fenster
*/
void GrabberInfo::setRubberbandUpdate ( int i )
{
  emit showRubber ( true );
  int maxWidth = screenGeometry.width();
  int boxRight = qRound ( setXBox->value() + setWidthBox->value() );
  if ( boxRight >= maxWidth )
  {
    int w = qRound ( setWidthBox->value() - ( boxRight - maxWidth ) );
    setWidthBox->setValue ( ( ( ( w % 2 ) == 0 ) ? w : ( w - 1 ) ) );
  }

  int maxHeight = screenGeometry.height();
  int boxBottom = qRound ( setYBox->value() + setHeightBox->value() );
  if ( boxBottom >= maxHeight )
  {
    int h = qRound ( setHeightBox->value() - ( boxBottom - maxHeight ) );
    setHeightBox->setValue ( ( ( ( h % 2 ) == 0 ) ? h : ( h - 1 ) ) );
  }

  emit screenDataChanged ( ( ( i > 0 ) ? true : false ) );
}

/**
* Widget Einstellungen suchen und laden.
*/
void GrabberInfo::load ( QSettings *cfg )
{
  // Grabber
  setRect ( cfg->value ( QLatin1String ( "Grabber/Dimension" ) ).toRect() );
  setDepth->setValue ( cfg->value ( QLatin1String ( "Grabber/Depth" ), 24 ).toUInt() );
  setFrameRate->setValue ( cfg->value ( QLatin1String ( "Grabber/FrameRate" ), 25 ).toUInt() );
}

/**
* Aktuelle Widget Einstellungen speichern.
*/
void GrabberInfo::save ( QSettings *cfg )
{
  // Grabber
  cfg->setValue ( QLatin1String ( "Grabber/Dimension" ), getRect() );
  cfg->setValue ( QLatin1String ( "Grabber/Depth" ), setDepth->value() );
  cfg->setValue ( QLatin1String ( "Grabber/FrameRate" ), setFrameRate->value() );
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

void GrabberInfo::setRect ( const QRect &rect, int screen )
{
  m_screenBox->setValue ( screen );
  setXBox->setValue ( rect.x() );
  setYBox->setValue ( rect.y() );
  setWidthBox->setValue ( rect.width() );
  setHeightBox->setValue ( rect.height() );
}

/**
* Gibt die Aktuelle Dimension in form eines QRect zurück.
*/
const QRect GrabberInfo::getRect()
{
  QRect rect ( setXBox->value(), setYBox->value(), 1, 1 );
  rect.setWidth ( ( setWidthBox->value() % 2 == 0 ) ? setWidthBox->value() : ( setWidthBox->value() - 1 ) );
  rect.setHeight ( ( setHeightBox->value() % 2 == 0 ) ? setHeightBox->value() : ( setHeightBox->value() - 1 ) );
  return rect;
}

/**
* Gibt die Aktuelle FrameRate zurück.
*/
int GrabberInfo::frameRate()
{
  int rate = setFrameRate->value();
  return ( rate > 0 ) ? rate : 25;
}

/**
* Gibt die Aktuelle Geometrie zurück.
*/
const QString GrabberInfo::getGeometry()
{
  QRect r = getRect();
  return QString ( "%1x%2" ).arg ( QString::number ( r.width() ), QString::number ( r.height() ) );
}

/**
* Gibt die Aktuelle Geometrie zurück.
*/
const QString GrabberInfo::getX11GrabIdent()
{
  QRect r = getRect();
  return QString ( ":%1.%2+%3,%4 " ) .arg (
             QString::number ( QX11Info::appScreen() ),
             QString::number ( m_screenBox->value() ),
             QString::number ( r.x() ),
             QString::number ( r.y() )
         );
}

GrabberInfo::~GrabberInfo()
{}
