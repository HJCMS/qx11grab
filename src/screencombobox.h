/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
* Copyright: See COPYING file that comes with this distribution
**/

#ifndef SCREENCOMBOBOX_H
#define SCREENCOMBOBOX_H

#include <QComboBox>

class DesktopInfo;

class ScreenComboBox : public QComboBox
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Juergen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    DesktopInfo *m_DesktopInfo;
    void setItems();

  public:
    ScreenComboBox ( QWidget *parent = 0 );
    ~ScreenComboBox();

  Q_SIGNALS:
    void screenNameChanged ( const QString & );
    void screenWidthChanged ( int );
    void screenHeightChanged ( int );
    void screenDepthChanged ( int );

  private Q_SLOTS:
    void setDataChanged ( int );

};

#endif
