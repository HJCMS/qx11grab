/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2008
* Copyright: See COPYING file that comes with this distribution
**/

#ifndef QX11GRABADAPTOR_H
#define QX11GRABADAPTOR_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtDBus/QDBusAbstractAdaptor>
#include <QtDBus/QDBusConnection>

class QX11GrabAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )
    Q_CLASSINFO ( "D-Bus Interface", "de.hjcms" )
    Q_CLASSINFO ( "D-Bus Introspection", ""
"  <interface name=\"de.hjcms\" >\n"
"    <method name=\"stop\" />\n"
"    <method name=\"start\" />\n"
"    <signal name=\"message\">\n"
"      <arg direction=\"out\" type=\"s\" name=\"mess\" />\n"
"    </signal>\n"
"  </interface>\n"
"" )

  protected:
    QDBusConnection *rcDBus;

  public:
    QX11GrabAdaptor ( QObject *parent = 0 );
    virtual ~QX11GrabAdaptor();

  public Q_SLOTS:
    void stop ();
    void start ();

  Q_SIGNALS:
    void message ( const QString &mess );

};

#endif
