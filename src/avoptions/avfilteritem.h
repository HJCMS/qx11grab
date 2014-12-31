/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann (Undefined) http://qx11grab.hjcms.de, (C) 2007-2015
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

#ifndef QX11GRAB_AVFILTERITEM_H
#define QX11GRAB_AVFILTERITEM_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QThread>

namespace QX11Grab
{
  /**
  * AVFilter
  * \sa http://ffmpeg.org/libavfilter.html
  */
  class AVFilterItem
  {
    private:
      QString p_labels;
      QString p_filter;

    public:
      explicit AVFilterItem ( const QString &data );

      /** Current Filter Parameter */
      const QString filterName();

      /** Have this Filter a pipe [in] Label? */
      bool hasInLabel();

      /** Have this Filter a Labels e.g.: [logo] ? */
      bool hasPipeLabel();

      /** polished Filter CommandLine */
      const QString filter();

      /** Have this Filter a Labels e.g.: [logo] ? */
      const QString labels();

      /** Have this Filter [in] and [out] Labels? */
      static bool isPipeFilter ( const QString &filter );

      virtual ~AVFilterItem();
  };

} /* eof namespace QX11Grab */

#endif
