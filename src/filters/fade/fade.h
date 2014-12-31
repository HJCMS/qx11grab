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

#ifndef QX11GRAB_FADE_H
#define QX11GRAB_FADE_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QSettings>
#include <QtCore/QString>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>
#include <QtGui/QLineEdit>
#include <QtGui/QWidget>

class Q_DECL_EXPORT fade : public QDialog
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    QSettings* cfg;
    // Types
    QRadioButton* m_in;
    QRadioButton* m_out;
    // Frames
    QSpinBox* m_start_frame;
    QSpinBox* m_nb_frames;
    // Alpha
    QCheckBox* m_alpha;
    // Output
    QLineEdit* m_lineEdit;

    void setSettings ( const QString &key, const QVariant &value );
    const QVariant settingsValue ( const QString &key, const QVariant &defaultValue = QVariant() );
    void loadDefaults();

  private Q_SLOTS:
    void updateChanged ( bool );
    void updateChanged ( int );
    void update();

  public:
    explicit fade ( QWidget * parent = 0 );

    /** open dialog */
    int start();

    Q_SCRIPTABLE const QString data();

    ~fade();
};

#endif

