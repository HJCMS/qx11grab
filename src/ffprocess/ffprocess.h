/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann http://qx11grab.hjcms.de, (C) 2007-2013
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

#ifndef FFPROCESS_H
#define FFPROCESS_H

/* QtCore */
#include <QtCore/QChar>
#include <QtCore/QMutex>
#include <QtCore/QObject>
#include <QtCore/QProcess>
#include <QtCore/QRect>
#include <QtCore/QString>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QX11Info>

class Settings;
class Listener;

class FFProcess : public QObject
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Juergen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    Settings* cfg;
    const QX11Info xInfo;
    QProcess* m_QProcess;
    Listener* m_listener;
    QStringList arguments;
    QMutex mutex;
    const QString application();
    const QString workdir();
    const QString writeScript ( const QStringList &cmd, const QString &vfile );

  private Q_SLOTS:
    void startCheck ();
    void status ( QProcess::ProcessState );
    void errors ( QProcess::ProcessError );
    void exited ( int, QProcess::ExitStatus );

  Q_SIGNALS:
    /**
    * This signal is emitted if QProcess::Running is changed
    */
    void running ();

    /**
    * This signal is emitted if QProcess::NotRunning is changed
    */
    void down();

    /**
    * This signal is emitted with internal notification messages
    */
    void message ( const QString & );

    /**
    * This signal is emitted when QProcess::status changed messages
    */
    void statusMessage ( const QString & );

    /**
    * This signal is emitted when errors detected
    */
    void errmessage ( const QString &, const QString & );

  public Q_SLOTS:
    /**
    * normal shutdown
    */
    void stop();

    /**
    * force shutdown
    */
    void kill();

  public:
    explicit FFProcess ( QObject * parent = 0, Settings * settings = 0 );

    /**
    * Send a custom command to current process
    */
    bool send ( const QChar &data );

    /**
    * start with given command line to video output file
    * \param cmd Command Line
    * \param out Output File
    */
    bool start ( const QStringList &cmd, const QString &out );

    /**
    * check if is running
    */
    bool isRunning();

    virtual ~FFProcess();
};

#endif
