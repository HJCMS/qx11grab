/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann (Undefined) http://qx11grab.hjcms.de, (C) 2007-2015
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

#ifndef CFG_CONFIGRUBBERBAND_H
#define CFG_CONFIGRUBBERBAND_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QColor>
#include <QtGui/QColorDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QWidget>

/* QX11Grab */
#include "abstractconfigwidget.h"

class ConfigRubberband : public AbstractConfigWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    QColor p_color;
    QColorDialog* m_colorWidget;

  private Q_SLOTS:
    void updateColor ( const QColor & );

  public Q_SLOTS:
    void load ( Settings * cfg );
    void save ( Settings * cfg );

  public:
    ConfigRubberband ( QWidget * parent = 0 );
    ~ConfigRubberband();
};

#endif
