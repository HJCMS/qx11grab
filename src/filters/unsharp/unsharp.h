/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann (Undefined) http://qx11grab.hjcms.de, (C) 2007-2013
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

#ifndef QX11GRAB_UNSHARP_H
#define QX11GRAB_UNSHARP_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QSettings>
#include <QtCore/QString>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QDialog>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QLineEdit>
#include <QtGui/QSlider>
#include <QtGui/QWidget>

class Q_DECL_EXPORT Unsharp : public QDialog
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    QSettings* cfg;
    QSlider* luma_msize_x;
    QSlider* luma_msize_y;
    QDoubleSpinBox* luma_amount;
    QSlider* chroma_msize_x;
    QSlider* chroma_msize_y;
    QDoubleSpinBox* chroma_amount;
    QLineEdit* m_preview;

  protected:
    void setSettings ( const QString &key, const QVariant &value );
    const QVariant settingsValue ( const QString &key, const QVariant &defaultValue = QVariant() );
    void loadDefaults();

  private Q_SLOTS:
    void update ();
    void update ( int );
    void update ( double );

  public:
    explicit Unsharp ( QWidget * parent = 0 );
    Q_SCRIPTABLE const QString data();
    ~Unsharp();
};

#endif
