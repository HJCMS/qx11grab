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

#ifndef FONTBOX_H
#define FONTBOX_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QColor>
#include <QtGui/QComboBox>
#include <QtGui/QFont>
#include <QtGui/QIcon>
#include <QtGui/QLabel>
#include <QtGui/QSlider>
#include <QtGui/QLineEdit>
#include <QtGui/QWidget>

/* Plugin */
#include "fontpreview.h"

class Q_DECL_EXPORT FontBox : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    FontPreview* m_fontPreview;
    QIcon fontIcon;
    QFont currentFont;
    QString fontFilePath;
    QComboBox* m_fontComboBox;
    QLineEdit* m_lineEdit;
    QSlider* m_sliderSize;

  private Q_SLOTS:
    void updatedView();
    void fontIndexChanged ( int );
    void fontSizeChanged ( int );

  Q_SIGNALS:
    void updateFont ();

  public:
    explicit FontBox ( QWidget * parent = 0 );

    void initFontConfigDatabase();

    void setPreviewColor ( const QString &bg, const QString &fg );

    const QFont font();
    const QString path();
    const QString text();
    const QString size();

    ~FontBox();
};

#endif
