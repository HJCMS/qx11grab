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
**/

#ifndef QX11GRAB_CODECSELECTER_H
#define QX11GRAB_CODECSELECTER_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QComboBox>
#include <QtGui/QWidget>

/* QX11Grab */
#include "avoptions.h"

namespace QX11Grab
{
  class CodecSelecter : public QComboBox
  {
      Q_OBJECT
      Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
      Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )
      Q_PROPERTY ( QString codec READ getCodec WRITE setCodec USER true )

    private Q_SLOTS:
      void itemSelected ( int );

    Q_SIGNALS:
      /** codec entry has changed */
      void codecChanged ( const QString &name, CodecID );

    public Q_SLOTS:
      /** marking codec as selected */
      void setCodec ( const QString &name );

    public:
      /** Codec Selecter ComboBox */
      CodecSelecter ( QWidget * parent = 0 );

      /** insert codec list */
      void setCodecItems ( const QList<QX11Grab::FFCodec> & );

      /** insert a custom codec and dont override existing entries */
      void setCustomItem ( const QString &key, const QVariant &value );

      /** current selected codec */
      const QString getCodec ();

      ~CodecSelecter();
  };
}  /* eof namespace QX11Grab */

#endif
