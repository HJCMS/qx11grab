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

#include "grabberinfo.h"
#include "desktopinfo.h"
#include "screencombobox.h"
#include "cspinbox.h"
#include "screenbox.h"
#include "desktoptapping.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QRect>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QtGui/QHBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QPalette>

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

  m_screenComboBox = new ScreenComboBox ( this );
  /*: WhatsThis */
  m_screenComboBox->setWhatsThis ( trUtf8 ( "predefined frame sizes" ) );
  gridLayout->addWidget ( m_screenComboBox, grow++, 1, 1, 2 );
  // end: Size

  // begin: Width
  QLabel* txt2 = new QLabel ( this );
  txt2->setText ( trUtf8 ( "Width:" ) );
  txt2->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt2, grow, 0, 1, 1 );

  setWidthBox = new CSpinBox ( this );
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

  setHeightBox = new CSpinBox ( this );
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
  horizontalLayout->setContentsMargins ( 5, 5, 5, 5 );
  horizontalLayout->addStretch ( 1 );

  // begin: Desktop Screen
  m_grouBox0 = new QGroupBox ( trUtf8 ( "Screen" ), this );

  QHBoxLayout* m_screenLayout = new QHBoxLayout ( m_grouBox0 );
  m_grouBox0->setLayout ( m_screenLayout );

  m_screenBox = new ScreenBox ( m_grouBox0 );
  /*: WhatsThis */
  m_screenBox->setToolTip ( trUtf8 ( "current selected screen" ) );
  m_screenLayout->addWidget ( m_screenBox, Qt::AlignLeft );

  horizontalLayout->addWidget ( m_grouBox0, Qt::AlignLeft );
  // end: Desktop Screen

  // begin: Frame Rate
  m_grouBox1 = new QGroupBox ( trUtf8 ( "Framerate" ), this );
  m_grouBox1->setCheckable ( true );
  m_grouBox1->setChecked ( false );
  QHBoxLayout* m_framerateLayout = new QHBoxLayout ( m_grouBox1 );
  m_grouBox1->setLayout ( m_framerateLayout );

  setFrameRate = new QSpinBox ( m_grouBox1 );
  /*: WhatsThis */
  setFrameRate->setWhatsThis ( trUtf8 ( "Set the framerate in the captured video." ) );
  setFrameRate->setRange ( 0, 150 );
  setFrameRate->setValue ( 25 );
  m_framerateLayout->addWidget ( setFrameRate, Qt::AlignLeft );

  horizontalLayout->addWidget ( m_grouBox1, Qt::AlignLeft );
  // end: Frame Rate

  gridLayout->addLayout ( horizontalLayout, grow++, 0, 1, 3 );

  gridLayout->setRowStretch ( grow++, 1 );

  setLayout ( gridLayout );

  // Default Dimensions
  m_desktopInfo =  new DesktopInfo ( this );
  m_desktopTapping = new DesktopTapping ( this );
  setInputDefaults ( m_desktopInfo->getScreen() );

  connect ( m_screenComboBox, SIGNAL ( screenWidthChanged ( int ) ),
            setWidthBox, SLOT ( setValue ( int ) ) );

  connect ( m_screenComboBox, SIGNAL ( screenHeightChanged ( int ) ),
            setHeightBox, SLOT ( setValue ( int ) ) );

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
  connect ( m_grouBox1, SIGNAL ( toggled ( bool ) ),
            this, SLOT ( toggleUpdate ( bool ) ) );

  connect ( setFrameRate, SIGNAL ( valueChanged ( int ) ),
            this, SLOT ( integerUpdate ( int ) ) );

  connect ( m_screenBox, SIGNAL ( valueChanged ( int ) ),
            this, SLOT ( integerUpdate ( int ) ) );

  connect ( m_desktopInfo, SIGNAL ( resized ( int ) ),
            this, SLOT ( setInputDefaults ( int ) ) );
}

/**
* Umleitung Signal bool nach void Update Datensatz
*/
void GrabberInfo::toggleUpdate ( bool )
{
  emit postUpdate();
}

/**
* Umleitung Signal int nach void Update Datensatz
*/
void GrabberInfo::integerUpdate ( int )
{
  emit postUpdate();
}

/**
* Wenn der Benutzer die Auflösung wechselt die
* Maximalen Werte neu setzen!
*/
void GrabberInfo::setInputDefaults ( int screen )
{
  QRect maxRect = m_desktopTapping->fullDesktopsRect();
  m_screenBox->setValue ( screen );
  // Weidth
  setWidthBox->setMaximum ( ( maxRect.width() + 2 ) );
  setWidthSlider->setMaximum ( ( maxRect.width() + 2 ) );
  // Height
  setHeightBox->setMaximum ( ( maxRect.height() + 2 ) );
  setHeightSlider->setMaximum ( ( maxRect.height() + 2 ) );
  // X Postion
  setXBox->setMaximum ( maxRect.width() );
  setXSlider->setMaximum ( maxRect.width() );
  // Y Position
  setYBox->setMaximum ( maxRect.height() );
  setYSlider->setMaximum ( maxRect.height() );
}

/**
* Wenn einer der Slider/Spinboxen verändert!
* @note Das Gummiband darf nicht aus dem Fenster
*/
void GrabberInfo::setRubberbandUpdate ( int i )
{
  QRect maxRect = m_desktopTapping->fullDesktopsRect();
  emit showRubber ( true );

  int maxWidth = maxRect.width();
  int boxRight = qRound ( setXBox->value() + setWidthBox->value() );
  if ( boxRight >= maxWidth )
  {
    int w = qRound ( setWidthBox->value() - ( boxRight - maxWidth ) );
    setWidthBox->setValue ( ( ( w & 1 ) ? ( 1 ^ w ) : w ) );
  }

  int maxHeight = maxRect.height();
  int boxBottom = qRound ( setYBox->value() + setHeightBox->value() );
  if ( boxBottom >= maxHeight )
  {
    int h = qRound ( setHeightBox->value() - ( boxBottom - maxHeight ) );
    setHeightBox->setValue ( ( ( h & 1 ) ? ( 1 ^ h ) : h ) );
  }

  // Alle Signale von x/y 0 bis max w/h durchlassen!
  if ( i >= 0 )
    emit screenDataChanged ( true );
}

/**
* Widget Einstellungen suchen und laden.
*/
void GrabberInfo::load ( QSettings *cfg )
{
  // Grabber
  setRect ( cfg->value ( QLatin1String ( "Grabber/Dimension" ) ).toRect() );
  m_grouBox1->setChecked ( cfg->value ( QLatin1String ( "Grabber/FrameRateEnabled" ), false ).toBool() );
  setFrameRate->setValue ( cfg->value ( QLatin1String ( "Grabber/FrameRate" ), 25 ).toUInt() );
}

/**
* Aktuelle Widget Einstellungen speichern.
*/
void GrabberInfo::save ( QSettings *cfg )
{
  // Grabber
  cfg->setValue ( QLatin1String ( "Grabber/Dimension" ), getRect() );
  cfg->setValue ( QLatin1String ( "Grabber/FrameRateEnabled" ), m_grouBox1->isChecked() );
  cfg->setValue ( QLatin1String ( "Grabber/FrameRate" ), setFrameRate->value() );
}

/**
* Sucht mit Geometrie und Desktop Nummer nach den Einstellungen
* Diese Methode wird unter anderem von Windowgrabber aufgerufen
*/
void GrabberInfo::setRect ( const QRect &rect, int screen )
{
  m_screenBox->setValue ( screen );
  setXBox->setValue ( rect.x() );
  setYBox->setValue ( rect.y() );
  setWidthBox->setValue ( rect.width() );
  setHeightBox->setValue ( rect.height() );
  m_screenComboBox->setDataChanged ( rect );
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
* Gibt die Aktuelle Dimension in form eines QRect zurück.
*/
const QRect GrabberInfo::getRect()
{
  QRect rect ( setXBox->value(), setYBox->value(), 1, 1 );
  // Normalisieren
  rect.setWidth ( ( setWidthBox->value() & 1 ) ? ( 1 ^ setWidthBox->value() ) : setWidthBox->value() );
  rect.setHeight ( ( setHeightBox->value() & 1 ) ? ( 1 ^ setHeightBox->value() ) : setHeightBox->value() );
  // Wenn eine Abfrage gestartet wird, mit der ScreenComboBox abgleichen
  m_screenComboBox->setDataChanged ( rect );

  return rect;
}

/**
* Gibt die Aktuelle FrameRate zurück.
*/
int GrabberInfo::frameRate()
{
  if ( !m_grouBox1->isChecked() )
    return 0;

  int rate = setFrameRate->value();
  return ( rate > 0 ) ? rate : 25;
}

/** Gibt die Aktuelle Bildschirm Erkennung zurück */
const QString GrabberInfo::getScreens()
{
  return m_screenBox->text();
}

/** Gibt die Aktuelle Geometrie zurück. */
const QString GrabberInfo::getGeometry()
{
  QRect r = getRect();
  return QString ( "%1x%2" ).arg ( QString::number ( r.width() ), QString::number ( r.height() ) );
}

/** Komplette Bildschirm und Abmessungen ausgeben */
const QString GrabberInfo::getX11GrabIdent()
{
  QRect r = getRect();
  return QString::fromUtf8 ( "%1+%2,%3" ).arg (
             m_screenBox->text(),
             QString::number ( r.x() ),
             QString::number ( r.y() )
         );
}

GrabberInfo::~GrabberInfo()
{}
