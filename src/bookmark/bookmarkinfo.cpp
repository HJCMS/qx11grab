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

#include "bookmarkinfo.h"

/* QX11Grab */
// #include ""

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QSizePolicy>

BookmarkInfo::BookmarkInfo ( QWidget * parent )
    : QWidget ( parent )
{
  setObjectName ( QLatin1String ( "BookmarkInfo" ) );

  Qt::Alignment labelAlign = ( Qt::AlignRight | Qt::AlignTop );
  int minEditHeight = 30;

  QSizePolicy p_sizePolicy ( QSizePolicy::Preferred, QSizePolicy::MinimumExpanding );

  int grow = 0;
  QGridLayout* layout = new QGridLayout ( this );

  // Title {
  QLabel* lb_title = new QLabel ( trUtf8 ( "Title" ), this );
  layout->addWidget ( lb_title, grow, 0, 1, 1, labelAlign );

  m_titleEdit = new QLineEdit ( this );
  m_titleEdit->setWhatsThis ( trUtf8 ( "Required Bookmark Identifier" ) );
  layout->addWidget ( m_titleEdit, grow++, 1, 1, 1 );
  // } Title

  // Metadata {
  QLabel* lb_meta = new QLabel ( trUtf8 ( "Metadata" ), this );
  layout->addWidget ( lb_meta, grow, 0, 1, 1, labelAlign );

  m_editMeta = new QTextEdit ( this );
  m_editMeta->setObjectName ( QLatin1String ( "BookmarkInfo/Metadata" ) );
  m_editMeta->setWhatsThis ( trUtf8 ( "%1 Options" ).arg ( "-metadata" ) );
  m_editMeta->setMinimumHeight ( minEditHeight );
  m_editMeta->setSizePolicy ( p_sizePolicy );
  layout->addWidget ( m_editMeta, grow++, 1, 1, 1 );
  // } Metadata

  // vcodec {
  QLabel* lb_av = new QLabel ( trUtf8 ( "Video Codec" ), this );
  layout->addWidget ( lb_av, grow, 0, 1, 1, labelAlign );

  m_editVCodec = new QTextEdit ( this );
  m_editVCodec->setObjectName ( QLatin1String ( "BookmarkInfo/VCodec" ) );
  m_editVCodec->setWhatsThis ( trUtf8 ( "%1 Options" ).arg ( "-vcodec" ) );
  m_editVCodec->setMinimumHeight ( minEditHeight );
  m_editVCodec->setSizePolicy ( p_sizePolicy );
  layout->addWidget ( m_editVCodec, grow++, 1, 1, 1 );
  // } vcodec

  // acodec {
  QLabel* lb_ac = new QLabel ( trUtf8 ( "Audio Codec" ), this );
  layout->addWidget ( lb_ac, grow, 0, 1, 1, labelAlign );

  m_editACodec = new QTextEdit ( this );
  m_editACodec->setObjectName ( QLatin1String ( "BookmarkInfo/ACodec" ) );
  m_editACodec->setWhatsThis ( trUtf8 ( "%1 Options" ).arg ( "-acodec" ) );
  m_editACodec->setMinimumHeight ( minEditHeight );
  m_editACodec->setSizePolicy ( p_sizePolicy );
  layout->addWidget ( m_editACodec, grow++, 1, 1, 1 );
  // } acodec

  setLayout ( layout );
}

BookmarkInfo::~BookmarkInfo()
{}
