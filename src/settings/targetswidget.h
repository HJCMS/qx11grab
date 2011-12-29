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

#ifndef TARGETSWIDGET_H
#define TARGETSWIDGET_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QSettings>
#include <QtCore/QString>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QGroupBox>
#include <QtGui/QCheckBox>
#include <QtGui/QLineEdit>
#include <QtGui/QWidget>

/* QX11Grab */
#include "settings.h"

class TargetsWidget : public QGroupBox
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    QLineEdit* m_binaryFile;
    QLineEdit* m_outputName;
    QLineEdit* m_outputDirectory;

  private Q_SLOTS:
    void setFFmpegBinary();
    void valiadateInput ( const QString &name );
    void setOutpuDirectory();
    void restoreFileName();

  Q_SIGNALS:
    void postUpdate ( bool );

  public Q_SLOTS:
    void load ( Settings * cfg );
    void save ( Settings * cfg );

  public:
    TargetsWidget ( QWidget * parent = 0 );
    ~TargetsWidget();
};

#endif
