/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2008
* Copyright: See COPYING file that comes with this distribution
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
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    Settings *m_Settings;
    const QX11Info xInfo;
    QString program, workdir;
    QStringList arguments;
    QProcess *m_QProcess;

  public:
    FFProcess ( QObject *parent = 0, Settings *settings = 0 );
    bool create ( const QRect & );
    bool start();
    bool stop();
    bool isRunning();
    ~FFProcess();

  Q_SIGNALS:
    void message ( const QString & );
    void trigger ( const QString & );
    void errmessage ( const QString & );

  private Q_SLOTS:
    void readOutput ();
    void startCheck ();
    void errors ( QProcess::ProcessError );
    void exited ( int, QProcess::ExitStatus );

};

#endif
