/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2008
* Copyright: See COPYING file that comes with this distribution
**/

#ifndef WINDOWGRABBER_H
#define WINDOWGRABBER_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QRect>

/* QtGui */
#include <QtGui/QWidget>
#include <QtGui/QX11Info>

class WindowGrabber : public QObject
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Juergen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    const QX11Info xInfo;
    int getValidValue ( int );

  public:
    WindowGrabber ( QObject *parent = 0 );

    /** grab Window Dimension from X-Server */
    const QRect grabWindowRect();

    ~WindowGrabber();

};

#endif
