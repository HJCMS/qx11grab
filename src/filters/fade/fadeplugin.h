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

#ifndef QX11GRAB_FADE_PLUGIN_H
#define QX11GRAB_FADE_PLUGIN_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>

/* QX11Grab */
#include "interface.h"

class fade;

/**
* @short avfilter fade plugin dialog
* \sa http://ffmpeg.org/libavfilter.html#fade
*/
class Q_DECL_EXPORT fadePlugin : public QX11Grab::Interface
{
    Q_OBJECT
    Q_INTERFACES ( QX11Grab::Interface )

  private:
    fade* m_fade;

  public:
    bool create ( QWidget * parent );

    bool exec();

    const QString pluginName();

    const QString title();

    const QString description();

    const QString data();
};

#endif

