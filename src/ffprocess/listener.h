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

#ifndef LISTENER_H
#define LISTENER_H

/* QtCore */
#include <QtCore/QDateTime>
#include <QtCore/QFileInfo>
#include <QtCore/QFileSystemWatcher>
#include <QtCore/QObject>
#include <QtCore/QString>

class Listener : public QFileSystemWatcher
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )

  private:
    QFileInfo* m_fileInfo;
    QDateTime* m_timeSpec;
    void setInfoUpdate ();

  private Q_SLOTS:
    void updated ( const QString & );

  Q_SIGNALS:
    void info ( const QString & );

  public Q_SLOTS:
    void start();
    void clear();
    
  public:
    Listener ( QObject * parent = 0 );
    void setOutputFile ( const QString &outFile );
    ~Listener();
};

#endif
