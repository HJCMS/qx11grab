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

#ifndef GRABBERINFO_H
#define GRABBERINFO_H

/* QtCore */
#include <QtCore/QRect>
#include <QtCore/QObject>
#include <QtCore/QSettings>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QCheckBox>
#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

class ScreenComboBox;

class GrabberInfo : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )

  private:
    ScreenComboBox* screenComboBox;
    QLineEdit* setModeName;
    QSpinBox* setWidthBox;
    QSpinBox* setHeightBox;
    QSpinBox* setXBox;
    QSpinBox* setYBox;
    QSpinBox* setFrameRate;
    QCheckBox* showRubberband;
    QCheckBox* startMinimized;
    QCheckBox* soundRecording;
    QCheckBox* setMetadata;

  Q_SIGNALS:
    void showRubber ( bool );
    void screenDataChanged ( int );
    void postUpdate();

  public Q_SLOTS:
    void load ( QSettings * );
    void save ( QSettings * );

  public:
    GrabberInfo ( QWidget * parent = 0 );
    bool RubberbandIsVisible();
    bool soundEnabled();
    bool metaEnabled();
    void setRubberbandCheckBox ( bool b );
    void setScreenWidth ( int w );
    void setScreenHeight ( int h );
    void setScreenX ( int x );
    void setScreenY ( int y );
    void setScreenMode ( const QString &mode );
    const QRect getRect();
    int frameRate();
    ~GrabberInfo();
};

#endif
