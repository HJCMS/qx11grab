/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann http://qx11grab.hjcms.de, (C) 2007-2015
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

#ifndef GRABBERINFO_H
#define GRABBERINFO_H

/* QtCore */
#include <QtCore/QRect>
#include <QtCore/QObject>
#include <QtCore/QSettings>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QCheckBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGroupBox>
#include <QtGui/QSlider>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

class ScreenComboBox;
class CSpinBox;
class ScreenBox;
class DesktopInfo;
class DesktopTapping;

class GrabberInfo : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_PROPERTY ( QRect rect READ getRect WRITE setRect USER true )

  private:
    QRect screenGeometry;
    DesktopInfo* m_desktopInfo;
    DesktopTapping* m_desktopTapping;
    ScreenComboBox* m_screenComboBox;
    CSpinBox* setWidthBox;
    QSlider* setWidthSlider;
    CSpinBox* setHeightBox;
    QSlider* setHeightSlider;
    QSpinBox* setXBox;
    QSlider* setXSlider;
    QSpinBox* setYBox;
    QSlider* setYSlider;
    QGroupBox* m_grouBox0;
    ScreenBox* m_screenBox;
    QGroupBox* m_grouBox1;
    QSpinBox* setFrameRate;

  private Q_SLOTS:
    void toggleUpdate ( bool );
    void integerUpdate ( int );
    void setInputDefaults ( int screen );
    void setRubberbandUpdate ( int );

  Q_SIGNALS:
    void showRubber ( bool );
    void screenDataChanged ( bool );
    void postUpdate();

  public Q_SLOTS:
    void load ( QSettings * );
    void save ( QSettings * );
    void setRect ( const QRect &rect, int screen = 0 );

  public:
    /**
    * Widget to display the current dimension settings
    */
    GrabberInfo ( QWidget * parent = 0 );
    void setScreenWidth ( int w );
    void setScreenHeight ( int h );
    void setScreenX ( int x );
    void setScreenY ( int y );

    /** Rectangle Rect */
    const QRect getRect();

    /** Framerate */
    int frameRate();

    /** Screens Definition \e Example \b :0.0 */
    const QString getScreens();

    /** Screen Geometry */
    const QString getGeometry();

    /** FFmpeg Definition for x11grab Input */
    const QString getX11GrabIdent();

    ~GrabberInfo();
};

#endif
