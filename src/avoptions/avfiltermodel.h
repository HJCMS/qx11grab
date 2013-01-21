/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann (Undefined) http://qx11grab.hjcms.de, (C) 2007-2013
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

#ifndef QX11GRAB_AVFILTERMODEL_H
#define QX11GRAB_AVFILTERMODEL_H

/* QtCore */
#include <QtCore/QList>
#include <QtCore/QRegExp>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/** \brief Filter Delimiter */
#ifndef QX11GRAB_FILTER_DELIMITER
# define QX11GRAB_FILTER_DELIMITER QLatin1String(",")
#endif

namespace QX11Grab
{

  class AVFilterItem;

  /**
  * AVFilter List
  * \sa http://ffmpeg.org/libavfilter.html
  */
  class AVFilterModel
  {
    private:
      /** current filters */
      QStringList currentFilters;

      /** piped plugin available */
      bool isPiped;

      /** FilterList */
      QList<AVFilterItem*> filters;

    public:
      explicit AVFilterModel ( const QString &filter );

      /** insert a new filter */
      void insertFilter ( const QVariant &data );

      /** generated filter list */
      const QString values();

      virtual ~AVFilterModel();
  };

}  /* eof namespace QX11Grab */

#endif
