/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2008
* Copyright: See COPYING file that comes with this distribution
**/

#ifndef DESKTOPINFO_H
#define DESKTOPINFO_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QList>

/* QtGui */
#include <QtGui/QWidget>
#include <QtGui/QX11Info>
#include <QtGui/QDesktopWidget>

typedef struct
{
  QString name; /* framse size name */
  int width; /* frame with */
  int height; /* frame height */
  int depth; /* screen depth */
  QString summary; /* summary "name (width x height)" */
} FrameMode;

typedef const QList<FrameMode> ModeList;

class DesktopInfo : public QObject
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Juergen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    const QX11Info xInfo;
    QDesktopWidget *desktopWidget;
    int Screens;
    int maxHeight, maxWidth, Depth;
    const FrameMode fetchFrameMode ( const QString &n, int w, int h );

  public:
    DesktopInfo ( QObject *parent = 0 );

    /** width request at time */
    int getMaxWidth();

    /** height request at time */
    int getMaxHeight();

    /** depth request at time */
    int getDepth();

    /** returns a list from all fixed frame size modes by ffmpeg.
     * for more Information @see man:ffmpeg
     * @param parent grab Dimension from
     */
    ModeList modes ( QWidget *parent = 0 );

    /** find frame mode by selected itemdata
     * @param name  ident FrameMode name
     * @param parent grab Dimension from
     */
    const FrameMode getFrameMode ( const QString &n, QWidget *parent = 0 );

    /** get current Screen Dimension form parent Widget
     * @param parent  grab Dimension from
     */
    const FrameMode grabScreenGeometry ( QWidget *parent );

    ~DesktopInfo();

};

#endif
