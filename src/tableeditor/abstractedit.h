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
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU Library General Public License
* along with this library; see the file COPYING.LIB. If not, write to
* the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
* Boston, MA 02110-1301, USA.
**/

#ifndef ABSTRACTEDIT_H
#define ABSTRACTEDIT_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QContextMenuEvent>
#include <QtGui/QLineEdit>
#include <QtGui/QRegExpValidator>
#include <QtGui/QWidget>

/* QX11Grab */
#include "avoptions.h"

class AbstractEdit : public QLineEdit
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_PROPERTY ( QVariant value READ value WRITE setValue USER true )

  private:
    QStringList proposeList;

  private Q_SLOTS:
    void openProposedDialog();

  protected:
    QRegExpValidator* m_regExpValidator;
    void contextMenuEvent ( QContextMenuEvent * );
    const QString implode ( const QStringList &data ) const;
    const QStringList explode ( const QString &data ) const;

  public Q_SLOTS:
    void setValue ( const QVariant &value );

  public:
    explicit AbstractEdit ( QWidget * parent = 0 );
    void setCompleters ( const QList<QX11Grab::FFOption> &list );
    const QVariant value();
    const QByteArray valuePropertyName () const;
    ~AbstractEdit();
};

#endif
