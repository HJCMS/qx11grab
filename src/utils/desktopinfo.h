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
#include <QtCore/QObject>
#include <QtCore/QRect>

/* QtGui */
#include <QtGui/QDesktopWidget>
#include <QtGui/QWidget>
#include <QtGui/QX11Info>

typedef struct
{
  QString name; /* framse size name */
  int width; /* frame with */
  int height; /* frame height */
  int depth; /* screen depth */
  QString summary; /* summary "name (width x height)" */
} FrameMode;

typedef const QList<FrameMode> ModeList;

class DesktopInfo : public QObject
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Juergen Heinemann (Undefined)" )

  private:
    const QX11Info xInfo;
    QDesktopWidget *desktopWidget;
    int Screens;
    int maxHeight, maxWidth, Depth;
    const FrameMode fetchFrameMode ( const QString &n, int w, int h );

  Q_SIGNALS:
    void resized ( int screen );

  public:
    DesktopInfo ( QObject *parent = 0 );

    /** current screen  */
    int screen();

    /** width request at time */
    int getMaxWidth();

    /** height request at time */
    int getMaxHeight();

    /** depth request at time */
    int getDepth();

    /** returns a list from all fixed frame size modes by ffmpeg.
     * for more Information @see man:ffmpeg
     * @param parent grab Dimension from
     */
    ModeList modes ( QWidget *parent = 0 );

    /** find frame mode by selected itemdata
     * @param name  ident FrameMode name
     * @param parent grab Dimension from
     */
    const FrameMode getFrameMode ( const QString &n, QWidget *parent = 0 );

    /** get current Screen Dimension form parent Widget
     * @param parent  grab Dimension from
     */
    const FrameMode grabScreenGeometry ( QWidget *parent );

    /** get current Screen Dimension form Desktop
     * @param screen Screen ID
     */
    const QRect screenGeometry ( int screen = 0 );

    ~DesktopInfo();

};

#endif
