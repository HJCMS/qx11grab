/**
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann http://qx11grab.hjcms.de, (C) 2007-2012
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

#ifndef TABLEEDITOR_H
#define TABLEEDITOR_H

/* QtCore */
#include <QtCore/QHash>
#include <QtCore/QObject>
#include <QtCore/QSettings>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QComboBox>
#include <QtGui/QTableWidget>
#include <QtGui/QTableWidgetItem>
#include <QtGui/QWidget>

class TableEditor : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )

  private:
    QString currentType;
    QStringList sharedVideoCodec;
    QStringList sharedAudioCodec;
    QComboBox* m_codecComboBox;
    QTableWidget* m_tableWidget;

    void findVideoCodecs();
    void findAudioCodecs();
    const QHash<QString,QVariant> readSection ( const QString &, QSettings * );
    const QHash<QString,QString> tableItems();
    QTableWidgetItem* createItem ( const QString & );
    void loadTableOptions ( const QString &, QSettings * );
    void saveTableOptions ( const QString &, QSettings * );

  private Q_SLOTS:
    void addTableRow();
    void delTableRow();

  Q_SIGNALS:
    void postUpdate();

  public Q_SLOTS:
    void load ( const QString &, QSettings * );
    void save ( const QString &, QSettings * );

  public:
    TableEditor ( QWidget * parent = 0 );
    const QStringList getCmd();
    ~TableEditor ();
};

#endif
