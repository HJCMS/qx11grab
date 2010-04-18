/**
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann http://qx11grab.hjcms.de, (C) 2007-2010
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

#ifndef DEFAULTS_H
#define DEFAULTS_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QSettings>

/* QtGui */
#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

class Defaults : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://xhtmldbg.hjcms.de" )

  private:
    QLineEdit* ff_path;
    QLineEdit* outputName;
    QLineEdit* outputDirectory;
    QLineEdit* ff_oss;
    QSpinBox* ossVolume;

  private Q_SLOTS:
    void setFFmpegBinary();
    void setOutpuDirectory();

  public Q_SLOTS:
    void load ( QSettings * cfg );
    void save ( QSettings * cfg );

  public:
    Defaults ( QWidget * parent = 0 );
    const QString binary();
    const QString ossdevice();
    const QString output();
    ~Defaults ();
};

#endif
