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

#ifndef BOOKMARKDIALOG_H
#define BOOKMARKDIALOG_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QSettings>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QLineEdit>
#include <QtGui/QWidget>

class Settings;
class Bookmark;

class BookmarkDialog : public QDialog
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )
    Q_PROPERTY ( QString bookmark READ getBookmark WRITE setBookmark )

  private:
    Settings* settings;
    Bookmark* xml;
    const QString dbusPath;
    QLineEdit* m_titleEdit;
    QDialogButtonBox* m_buttonBox;
    const QString implode ( const QStringList &data ) const;
    const QStringList explode ( const QString &data ) const;

  private Q_SLOTS:
    void titleChanged ( const QString & );
    void saveAndExit();

  public:
    explicit BookmarkDialog ( Settings * cfg, QWidget * parent = 0 );

    Q_SCRIPTABLE void setBookmark ( const QString &str );
    Q_SCRIPTABLE const QString getBookmark();

    virtual ~BookmarkDialog();
};

#endif
