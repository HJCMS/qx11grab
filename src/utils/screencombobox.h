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

#ifndef SCREENCOMBOBOX_H
#define SCREENCOMBOBOX_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QRect>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QComboBox>
#include <QtGui/QWidget>

class DesktopInfo;

class ScreenComboBox : public QComboBox
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Juergen Heinemann (Undefined)" )

  private:
    DesktopInfo* m_DesktopInfo;
    void setItems();
    inline void moveToIndex ( int );

  private Q_SLOTS:
    void setDataChanged ( int );

  Q_SIGNALS:
    /**
     * This signal is sent the Name for changed Modes and
     * emitted whenever a valid Rectangle has changed.
     */
    void screenNameChanged ( const QString & );

    /**
     * This signal is sent the \b Rectangle \b Width for changed
     * Modes and emitted whenever a valid Rectangle has changed.
     */
    void screenWidthChanged ( int );

    /**
     * This signal is sent the \b Rectangle \b Height for changed
     * Modes and emitted whenever a valid Rectangle has changed.
     */
    void screenHeightChanged ( int );

    /**
     * This signal is sent the Selected Desktop Screen and emitted
     * whenever a valid Rectangle has changed.
     */
    void screenDepthChanged ( int );

  public Q_SLOTS:
    /**
     * Find and select ItemData with given Rectangle
     * \note This Method blocking all QComboBox Signals!
     * \param rect Rectangle
     */
    void setDataChanged ( const QRect &rect );

  public:
    explicit ScreenComboBox ( QWidget *parent = 0 );

    ~ScreenComboBox();
};

#endif
