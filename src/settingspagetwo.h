/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2008
* Copyright: See COPYING file that comes with this distribution
**/

#ifndef SETTINGSPAGETWO_H
#define SETTINGSPAGETWO_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QMap>

/* QtGui */
#include <QtGui/QWidget>
#include <QtGui/QLineEdit>
#include <QtGui/QTableWidget>
#include <QtGui/QTableWidgetItem>
#include <QtGui/QContextMenuEvent>

class SettingsPageTwo : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Juergen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    QLineEdit *ff_cmd;
    QTableWidget *ff_tableWidget;
    void setTableDefaults();
    void resizeTableContents ();
    void pushItemRow( int, const QString &, const QVariant & );

  public:
    SettingsPageTwo ( QWidget *parent = 0 );
    const QMap<QString,QVariant> getOptions();
    ~SettingsPageTwo();

  public Q_SLOTS:
    void setOptions ( const QMap<QString,QVariant> & );

  private Q_SLOTS:
    void prepareProcessLine ();
    void contextMenuEvent ( QContextMenuEvent * );
    void addItemRow();
    void removeItemRow();
};

#endif
