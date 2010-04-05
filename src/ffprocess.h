/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
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
    Settings* cfg;
    const QX11Info xInfo;
    QString program, workdir;
    QStringList arguments;
    QProcess *m_QProcess;

    /**  Add Default X11Grab Command Line
     * @code
     *  -f x11grab -xerror -s (Width)x(Height) -i :(SCREEN).(qApp->SCREEN)+x,y
     * @endcode
     * @param r Dimension
     * @param o Options
     */
    void addVideo( const QRect &r, const QStringList &o );

    /**  Add Audio Command Line
     * @code
     *  -f oss -i /dev/dsp*
     * @endcode
     */
    void addAudio();

    /** set Output filepath
     */
    const QString addOutput ();

  public:
    FFProcess ( QObject *parent = 0, Settings *settings = 0 );
    bool create ( const QRect & );
    ~FFProcess();

  public Q_SLOTS:
    bool start();
    bool stop();
    bool kill();
    bool isRunning();

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

};

#endif
