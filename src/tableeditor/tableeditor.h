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
#include <QtGui/QLabel>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

class CodecTable;
class CodecSelecter;
class Settings;

namespace QX11Grab
{
  class FormatMenu;
  class CodecSelecter;
}

class TableEditor : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )

  private:
    QString currentType;
    QStringList sharedVideoCodec;
    QStringList sharedAudioCodec;
    QString currentCodecExtension;
    QX11Grab::CodecSelecter* m_codecSelecter;
    QX11Grab::FormatMenu* m_formatMenu;
    CodecTable* m_tableWidget;

    void findVideoCodecs();
    void findAudioCodecs();
    const QHash<QString,QVariant> readSection ( const QString &, Settings * );
    void loadTableOptions ( const QString &, Settings * );
    void saveTableOptions ( const QString &, Settings * );

  private Q_SLOTS:
    void addTableRow();
    void delTableRow();

  protected Q_SLOTS:
    void setCodecExtension ( const QString & );

  Q_SIGNALS:
    void postUpdate();

  public Q_SLOTS:
    void load ( const QString &, Settings * );
    void save ( const QString &, Settings * );

  public:
    TableEditor ( QWidget * parent = 0 );
    const QHash<QString,QVariant> getTableItems();
    const QString selectedCodec();
    const QString selectedCodecExtension();
    const QStringList getCmd();
    void setCodecByName ( const QString &txt );
    void setCodecOptions ( const QHash<QString,QVariant> &options );
    ~TableEditor ();
};

#endif
