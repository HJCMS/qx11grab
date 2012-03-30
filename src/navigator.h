/**
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

#ifndef NAVIGATOR_H
#define NAVIGATOR_H

/* QtCore */
#include <QtCore/QEvent>
#include <QtCore/QObject>
#include <QtCore/QPoint>
#include <QtCore/QSettings>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QDesktopWidget>
#include <QtGui/QHideEvent>
#include <QtGui/QLineEdit>
#include <QtGui/QMouseEvent>
#include <QtGui/QMoveEvent>
#include <QtGui/QPaintEvent>
#include <QtGui/QShowEvent>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

/* QX11Grab */
#include "mainwindow.h"

class PlayerAction;
class Settings;

class Navigator : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    struct MoveState
    {
      QDesktopWidget* widget;
      QPoint startPos;
      bool move;
      bool activ;
    };
    MoveState* m_state;
    Settings* m_settings;
    QToolButton* m_actionStartRecord;
    QToolButton* m_actionStopRecord;
    QToolButton* m_rubberbandAction;
    QLineEdit* m_infoData;
    PlayerAction* m_playerAction;

    void initMoveState ( const QPoint &p );
    void startMoveWidget ( bool b = false );
    void stopMoveWidget();

  protected:
    virtual void contextMenuEvent ( QContextMenuEvent * event );
    virtual void paintEvent ( QPaintEvent * event );
    virtual void hideEvent ( QHideEvent * event );
    virtual void showEvent ( QShowEvent * event );
    virtual void mouseMoveEvent ( QMouseEvent * event );
    virtual void mousePressEvent ( QMouseEvent * event );
    virtual void mouseReleaseEvent ( QMouseEvent * event );
    virtual void leaveEvent ( QEvent * event );

  Q_SIGNALS:
    void startRecord();
    void stopRecord();
    void rubberBand();

  public Q_SLOTS:
    void setActivity ( bool b = false );
    Q_SCRIPTABLE void setInfo ( const QString &info );
    Q_SCRIPTABLE void setPlayerEnabled ( bool b = false );

  public:
    explicit Navigator ( QWidget * parent = 0 );
    virtual ~Navigator();
};

#endif
