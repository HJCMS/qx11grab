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

#include "textposition.h"

/* QX11Grab */
#include "interface.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QMetaEnum>
#include <QtCore/QMetaType>
#include <QtCore/QSize>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QHBoxLayout>
#include <QtGui/QIcon>
#include <QtGui/QPixmap>

static const QIcon query_extra_icon ( const QString &n )
{
  QString p = QString ( "%1/share/pixmaps/qx11grab/position_%2.png" ).arg ( QX11GRAB_INSTALL_PREFIX, n );
  QIcon scIcon = QIcon::fromTheme ( "video-display" );
  QSize dSize = scIcon.pixmap ( 0, QIcon::Normal, QIcon::On ).size();
  QPixmap pixmap ( p, "PNG", Qt::AutoColor );
  if ( pixmap.isNull() )
    return scIcon;

  QIcon icon ( pixmap.scaled ( dSize, Qt::KeepAspectRatio, Qt::FastTransformation ) );
  if ( icon.isNull() )
    return scIcon;

  return icon;
}

TextPosition::TextPosition ( QWidget * parent )
    : QWidget ( parent )
    , _x ( "5" )
    , _y ( "5" )
{
  setObjectName ( QLatin1String ( "TextPosition" ) );
  setContentsMargins ( 0, 0, 0, 0 );

  QHBoxLayout* layout = new QHBoxLayout ( this );

  m_comboBox = new QComboBox ( this );
  /*: ToolTip */
  m_comboBox->setToolTip ( trUtf8 ( "Text Position" ) );

  int i = 0;
  m_comboBox->insertItem ( i++, query_extra_icon ( "unknown" ),  trUtf8 ( "Position" ), UNKNOWN );
  m_comboBox->insertItem ( i++, query_extra_icon ( "top_left" ),  trUtf8 ( "Top Left" ), TOP_LEFT );
  m_comboBox->insertItem ( i++, query_extra_icon ( "top_center" ),  trUtf8 ( "Top Centered" ), TOP_CENTER );
  m_comboBox->insertItem ( i++, query_extra_icon ( "top_right" ),  trUtf8 ( "Top Right" ), TOP_RIGHT );
  m_comboBox->insertItem ( i++, query_extra_icon ( "middle_left" ),  trUtf8 ( "Middle Left" ), MIDDLE_LEFT );
  m_comboBox->insertItem ( i++, query_extra_icon ( "middle_center" ),  trUtf8 ( "Middle Centered" ), MIDDLE_CENTER );
  m_comboBox->insertItem ( i++, query_extra_icon ( "middle_right" ),  trUtf8 ( "Middle Right" ), MIDDLE_RIGHT );
  m_comboBox->insertItem ( i++, query_extra_icon ( "bottom_left" ),  trUtf8 ( "Bottom Left" ), BOTTOM_LEFT );
  m_comboBox->insertItem ( i++, query_extra_icon ( "bottom_center" ),  trUtf8 ( "Bottom Centered" ), BOTTOM_CENTER );
  m_comboBox->insertItem ( i++, query_extra_icon ( "bottom_rigth" ),  trUtf8 ( "Bottom Right" ), BOTTOM_RIGTH );
  layout->addWidget ( m_comboBox );

  m_xIndent = new QSpinBox ( this );
  /*: ToolTip */
  m_xIndent->setToolTip ( trUtf8 ( "Horizontal Text Indent" ) );
  m_xIndent->setValue ( 5 );
  layout->addWidget ( m_xIndent );

  m_yIndent = new QSpinBox ( this );
  /*: ToolTip */
  m_yIndent->setToolTip ( trUtf8 ( "Vertical Text Indent" ) );
  m_yIndent->setValue ( 5 );
  layout->addWidget ( m_yIndent );

  setLayout ( layout );

  connect ( m_comboBox, SIGNAL ( currentIndexChanged ( int ) ),
            this, SLOT ( indexChanged ( int ) ) );

  connect ( m_xIndent, SIGNAL ( valueChanged ( int ) ),
            this, SLOT ( indentChanged ( int ) ) );

  connect ( m_yIndent, SIGNAL ( valueChanged ( int ) ),
            this, SLOT ( indentChanged ( int ) ) );
}

/**
* Sende Positions Signale
*/
void TextPosition::indexChanged ( int index )
{
  switch ( m_comboBox->itemData ( index, Qt::UserRole ).toUInt() )
  {
    case TOP_LEFT:
    {
      /**
      * x = X_INDENT
      * y = Y_INDENT
      */
      _x = QString::number ( m_xIndent->value() );
      _y = QString::number ( m_yIndent->value() );
    }
    break;

    case TOP_CENTER:
    {
      /**
      * x = ( ( VIDEO_WITH - TEXT_WIDTH ) / 2 )
      * y = Y_INDENT
      */
      _x = "(w-text_w)/2";
      _y = QString::number ( m_yIndent->value() );
    }
    break;

    case TOP_RIGHT:
    {
      /**
      * x = ( VIDEO_WITH - TEXT_WIDTH - X_INDENT )
      * y = Y_INDENT
      */
      _x = QString::fromUtf8 ( "(w-text_w-%1)" ).arg ( QString::number ( m_xIndent->value() ) );
      _y = QString::number ( m_yIndent->value() );
    }
    break;

    case MIDDLE_LEFT:
    {
      /**
      * x = Pixels Left
      * y = ( ( VIDEO_HEIGHT - TEXT_HEIGHT - LINE_HEIGTH ) / 2 )
      */
      _x = QString::number ( m_xIndent->value() );
      _y = "(h-text_h-line_h)/2";
    }
    break;

    case MIDDLE_CENTER:
    {
      /**
      * x = ( ( VIDEO_WITH - TEXT_WIDTH ) / 2 )
      * y = ( ( VIDEO_HEIGHT - TEXT_HEIGHT - LINE_HEIGTH ) / 2 )
      */
      _x = "(w-text_w)/2";
      _y = "(h-text_h-line_h)/2";
    }
    break;

    case MIDDLE_RIGHT:
    {
      /**
      * x = ( VIDEO_WITH - TEXT_WIDTH - X_INDENT )
      * y = ( ( VIDEO_HEIGHT - TEXT_HEIGHT - LINE_HEIGTH ) / 2 )
      */
      _x = QString::fromUtf8 ( "(w-text_w-%1)" ).arg ( QString::number ( m_xIndent->value() ) );
      _y = "(h-text_h-line_h)/2";
    }
    break;

    case BOTTOM_LEFT:
    {
      /**
      * x = Pixels Left
      * y = ( VIDEO_HEIGHT - TEXT_HEIGHT - Y_INDENT )
      */
      _x = QString::number ( m_xIndent->value() );
      _y = QString::fromUtf8 ( "(h-text_h-%1)" ).arg ( QString::number ( m_yIndent->value() ) );
    }
    break;

    case BOTTOM_CENTER:
    {
      /**
      * x = ( ( VIDEO_WITH - TEXT_WIDTH ) / 2 )
      * y = ( VIDEO_HEIGHT - TEXT_HEIGHT - Y_INDENT )
      */
      _x = "(w-text_w)/2";
      _y = QString::fromUtf8 ( "(h-text_h-%1)" ).arg ( QString::number ( m_yIndent->value() ) );
    }
    break;

    case BOTTOM_RIGTH:
    {
      /**
      * x = ( VIDEO_WITH - TEXT_WIDTH - X_INDENT )
      * y = ( VIDEO_HEIGHT - TEXT_HEIGHT - Y_INDENT )
      */
      _x = QString::fromUtf8 ( "(w-text_w-%1)" ).arg ( QString::number ( m_xIndent->value() ) );
      _y = QString::fromUtf8 ( "(h-text_h-%1)" ).arg ( QString::number ( m_yIndent->value() ) );
    }
    break;

    default:
    {
      /**
      * x = X_INDENT
      * y = Y_INDENT
      */
      _x = QString::number ( m_xIndent->value() );
      _y = QString::number ( m_yIndent->value() );
    }
    break;
  };
  emit postUpdate();
}

void TextPosition::indentChanged ( int )
{
  indexChanged ( m_comboBox->currentIndex() );
}

void TextPosition::setIndex ( int index )
{
  m_comboBox->setCurrentIndex ( index );
}

const QString TextPosition::x()
{
  return _x;
}

const QString TextPosition::y()
{
  return _y;
}

int TextPosition::index()
{
  return m_comboBox->currentIndex();
}

TextPosition::~TextPosition()
{}
