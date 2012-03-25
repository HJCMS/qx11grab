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

#include "navigator.h"

/* QX11Grab */
#include "menubar.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QRect>
#include <QtCore/QString>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QBrush>
#include <QtGui/QColor>
#include <QtGui/QLinearGradient>
#include <QtGui/QPalette>
#include <QtGui/QPainter>
#include <QtGui/QStyle>
#include <QtGui/QStyleOptionToolBar>
#include <QtGui/QVBoxLayout>

Navigator::Navigator ( QWidget * parent )
    : QToolBar ( parent )
    , m_settings ( new QSettings ( QSettings::NativeFormat, QSettings::UserScope, "hjcms.de", "qx11grab", parent ) )
{
  setObjectName ( QLatin1String ( "Navigation" ) );
  setWindowFlags ( ( Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint ) );
  setWindowModality ( Qt::NonModal );
  setAttribute ( Qt::WA_SetPalette, false );
  setAttribute ( Qt::WA_SetStyle, false );
  setAttribute ( Qt::WA_NoSystemBackground, true );
  setAttribute ( Qt::WA_OpaquePaintEvent, true );
#ifdef ENABLE_EXPERIMENTAL
  // This will not work with move windows :-/
  setAttribute ( Qt::WA_PaintOnScreen, true );
  setWindowOpacity ( 1.0 );
#endif

  addSeparator();

  m_actionStartRecord = MenuBar::startRecordAction ( this );
  addAction ( m_actionStartRecord );

  m_actionStopRecord = MenuBar::stopRecordAction ( this );
  addAction ( m_actionStopRecord );

  m_rubberbandAction = MenuBar::rubberbandAction ( this );
  addAction ( m_rubberbandAction );

  addSeparator();

  m_infoLabel = new QLineEdit ( this );
  m_infoLabel->setMinimumWidth ( 250 );
  m_infoLabel->setReadOnly ( true );
  addWidget ( m_infoLabel );

  addSeparator();

  connect ( m_actionStartRecord, SIGNAL ( triggered() ), this, SIGNAL ( startRecord() ) );
  connect ( m_actionStopRecord, SIGNAL ( triggered() ), this, SIGNAL ( stopRecord() ) );
  connect ( m_rubberbandAction, SIGNAL ( triggered() ), this, SIGNAL ( rubberBand() ) );
}

/**
* Hintergrund verändern
*/
void Navigator::paintEvent ( QPaintEvent * event )
{
  QStyleOptionToolBar option;
  initStyleOption ( &option );

  QPainter painter ( this );
  painter.setRenderHint ( QPainter::Antialiasing );
  painter.setCompositionMode ( QPainter::CompositionMode_Screen );
  painter.setBackgroundMode ( Qt::TransparentMode );
  painter.setBackground ( Qt::NoBrush );
  painter.setPen ( Qt::NoPen );

  QPalette pal = qApp->palette();
  QRect rect = event->rect();

  QLinearGradient gradient ( 0.0, 0.0, 0.0, rect.height() );
  gradient.setColorAt ( 0.0, pal.dark().color() );
  gradient.setColorAt ( 0.5, pal.light().color() );
  gradient.setColorAt ( 1.0, pal.dark().color() );
  painter.setBrush ( gradient );

  QPainterPath rectPath;
#ifdef ENABLE_EXPERIMENTAL
  rectPath.addRoundedRect ( 0.0, 0.0, rect.width(), rect.height(), 5.0, 5.0, Qt::AbsoluteSize );
#else
  rectPath.addRect ( 0.0, 0.0, rect.width(), rect.height() );
#endif
  rectPath.setFillRule ( Qt::WindingFill );
  painter.drawPath ( rectPath );
}

void Navigator::hideEvent ( QHideEvent * event )
{
  m_settings->setValue ( "Navigator/Rect", frameGeometry() );
  QToolBar::hideEvent ( event );
}

void Navigator::showEvent ( QShowEvent * event )
{
  QRect r = m_settings->value ( "Navigator/Rect", QRect ( 10, 10, 100, 24 ) ).toRect();
  r.setWidth ( rect().width() );
  setGeometry ( r );
  QToolBar::showEvent ( event );
}

void Navigator::setInfo ( const QString &info )
{
  m_infoLabel->setText ( info );
}

void Navigator::setActivity ( bool b )
{
  m_actionStopRecord->setEnabled ( b );
  m_actionStartRecord->setEnabled ( ( ( b ) ? false : true ) );
}

Navigator::~Navigator()
{
  if ( m_settings )
    delete m_settings;
}
