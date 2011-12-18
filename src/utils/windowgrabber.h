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

#ifndef WINDOWGRABBER_H
#define WINDOWGRABBER_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QRect>

/* QtGui */
#include <QtGui/QWidget>
#include <QtGui/QX11Info>

class WindowGrabber : public QObject
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Juergen Heinemann (Undefined)" )

  private:
    const QX11Info xInfo;

  protected:
    /** Do not accept Incorrect aspect ratio specification. */
    inline int normalize ( int ) const;

  public:
    explicit WindowGrabber ( QObject *parent = 0 );

    /** grab Window Dimension from X-Server */
    const QRect grabWindowRect();

    ~WindowGrabber();

};

#endif