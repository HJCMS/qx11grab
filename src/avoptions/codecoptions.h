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

#ifndef QX11GRAB_CODECOPTIONS_H
#define QX11GRAB_CODECOPTIONS_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QSignalMapper>
#include <QtCore/QString>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QIcon>
#include <QtGui/QMenu>
#include <QtGui/QWidget>

namespace QX11Grab
{
  /** Predefined Codec Options Menu */
  class CodecOptions : public QMenu
  {
      Q_OBJECT
      Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
      Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

    private:
      QIcon p_icon;
      QString p_codec;
      QSignalMapper* m_signalMapper;
      void queryCodec();

    private Q_SLOTS:
      void optionTriggered ( const QString & );

    Q_SIGNALS:
      /** option entry was selected */
      void optionChanged ( const QString &, const QVariant & );

    public:
      explicit CodecOptions ( QWidget * parent = 0 );
      ~CodecOptions();
  };
}  /* eof namespace QX11Grab */

#endif
