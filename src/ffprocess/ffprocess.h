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

#ifndef FFPROCESS_H
#define FFPROCESS_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QRect>
#include <QtGui/QX11Info>
#include <QtCore/QProcess>

class Settings;

class FFProcess : public QObject
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Juergen Heinemann (Undefined)" )

  private:
    Settings* cfg;
    const QX11Info xInfo;
    QStringList arguments;
    QProcess *m_QProcess;
    const QString application();
    const QString workdir();

  Q_SIGNALS:
    void running ();
    void down ();
    void message ( const QString & );
    void trigger ( const QString & );
    void errmessage ( const QString &, const QString & );

  private Q_SLOTS:
    void startCheck ();
    void errors ( QProcess::ProcessError );
    void exited ( int, QProcess::ExitStatus );

  public Q_SLOTS:
    bool start ( const QStringList &cmd );
    bool stop();
    bool kill();
    bool isRunning();

  public:
    explicit FFProcess ( QObject *parent = 0, Settings *settings = 0 );
    bool create ( const QRect & );
    ~FFProcess();
};

#endif
