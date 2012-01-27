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

#ifndef DESKTOPINFO_H
#define DESKTOPINFO_H

/* QtCore */
#include <QtCore/QList>
#include <QtCore/QMetaType>
#include <QtCore/QObject>
#include <QtCore/QRect>
#include <QtCore/QString>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QDesktopWidget>
#include <QtGui/QWidget>
#include <QtGui/QX11Info>

/**
* \class DesktopInfo
* Create Desktop Information from Current Screen
*/
class DesktopInfo : public QObject, public QX11Info
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Juergen Heinemann (Undefined)" )

  private:
    QDesktopWidget* m_desktopWidget;

  Q_SIGNALS:
    /**
     * This signal is emitted when the size of screen changes.
     */
    void resized ( int screen );

  public:
    /**
     * It is recommend to get parent object for screen identification
     */
    explicit DesktopInfo ( QObject * parent );

    /**
     * FameMode
     * @ingroup Declarations
     */
    typedef struct
    {
      QString name; /**< identifier name or title */
      int width; /**< mode with */
      int height; /**< mode height */
      int depth; /**< current screen depth */
      QString summary; /**< Mode Description e.g.: name (width x height) */
    } FrameMode;

    /**
     * Generate a Frame Modes with given arguments
     * @param n  Identifier Name
     * @param w  Mode Width
     * @param h  Mode Height
     */
    const FrameMode generateFrameMode ( const QString &n, int w, int h );

    /**
     * Returns the number of the screen currently in use.
     */
    int getScreen();

    /**
     * width request at time
     */
    int getMaxWidth();

    /**
     * height request at time
     */
    int getMaxHeight();

    /**
     * depth request at time
     */
    int getDepth();

    /**
     * widget that represents this screen
     */
    QWidget* screenWidget();

    /**
     * returns a list from all fixed frame size modes by ffmpeg.
     * for more Infromation @see man:ffmpeg
     * @param parent grab Dimension from
     */
    const QList<FrameMode> modes ( QWidget * parent = 0 );

    /**
     * get current Screen Dimension from Desktop
     * @param screen Screen ID
     */
    const QRect screenGeometry ( int screen = 0 );

    /**
     * find frame mode by selected itemdata
     * @param name  ident FrameMode name
     * @param parent grab Dimension from
     */
    const FrameMode getFrameMode ( const QString &n, QWidget * parent = 0 );

    /**
     * get current Screen Dimension from parent Widget
     * @param parent  grab Dimension from
     */
    const FrameMode grabScreenGeometry ( QWidget * parent = 0 );

    virtual ~DesktopInfo();

};

Q_DECLARE_METATYPE ( DesktopInfo::FrameMode )

#endif
