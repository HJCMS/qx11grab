/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
* Copyright: See COPYING file that comes with this distribution
**/

#ifndef RUBBERBAND_H
#define RUBBERBAND_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QWidget>
#include <QtGui/QRubberBand>

class DesktopInfo;

class RubberBand : public QRubberBand
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Juergen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    DesktopInfo *m_DesktopInfo;

  Q_SIGNALS:
    void warning ( const QString & );
    void error ( const QString &, const QString & );

  public:
    RubberBand ( QWidget *parent = 0 );
    bool isScalability();
    ~RubberBand();

};

#endif
