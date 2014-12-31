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

#ifndef BOOKMARKDIALOG_H
#define BOOKMARKDIALOG_H

/* QtCore */
#include <QtCore/QHash>
#include <QtCore/QObject>
#include <QtCore/QSettings>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QLineEdit>
#include <QtGui/QWidget>

class Bookmark;

class BookmarkDialog : public QDialog
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )
    Q_PROPERTY ( QString bookmark READ getBookmark WRITE setBookmark )

  private:
    Bookmark* xml;
    const QString dbusPath;
    QString vCodecID;
    QHash<QString,QVariant> vCodec;
    QString aCodecID;
    QHash<QString,QVariant> aCodec;
    QLineEdit* m_titleEdit;
    QDialogButtonBox* m_buttonBox;

    bool saveBookmark();

  private Q_SLOTS:
    void titleTextChanged ( const QString & );
    void saveAndExit();

  public:
    /** Bookmark Editor Dialog */
    explicit BookmarkDialog ( QWidget * parent = 0 );

    /** set VCodec List with ID */
    void setVCodecOptions ( const QString &id, const QHash<QString,QVariant> &hash );

    /** set ACodec List with ID  */
    void setACodecOptions ( const QString &id, const QHash<QString,QVariant> &hash );

    /** set Bookmark Title */
    Q_SCRIPTABLE void setBookmark ( const QString &str );

    /** get current Bookmark */
    Q_SCRIPTABLE const QString getBookmark();

    virtual ~BookmarkDialog();
};

#endif
