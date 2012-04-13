/*
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
*/

#ifndef THREADSSPINBOX_H
#define THREADSSPINBOX_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

/**
* a class for setting the maximum Thread count
*/
class ThreadsSpinBox : public QSpinBox
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    void setMaxThreads();

  public:
    explicit ThreadsSpinBox ( QWidget * parent = 0 );

    /**
    * Commandline Argument e.g.: "-threads 2"
    * @note If value is lesser then 2, it will return an empty string!
    */
    const QString argument();

    ~ThreadsSpinBox();
};

#endif
