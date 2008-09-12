/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2008
* Copyright: See COPYING file that comes with this distribution
**/

#ifndef RUBBERBAND_H
#define RUBBERBAND_H

/* QtCore */
#include <QtCore/QObject>

/* QtGui */
#include <QtGui/QWidget>
#include <QtGui/QRubberBand>

class RubberBand : public QRubberBand
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Juergen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  public:
    RubberBand ( QWidget *parent = 0 );
    ~RubberBand();

};

#endif
