/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2008
* Copyright: See COPYING file that comes with this distribution
**/

#ifndef SETTINGSPAGETWO_H
#define SETTINGSPAGETWO_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QTableWidget>
#include <QtGui/QTableWidgetItem>
#include <QtGui/QContextMenuEvent>

class Settings;

class ItemEditCmd : public QHBoxLayout
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Juergen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    QLabel *label;
    QLineEdit *edit;

  public:
    ItemEditCmd ( const QString &name, QWidget *parent = 0 );
    void setValue ( const QVariant & );
    const QVariant value();
    ~ItemEditCmd() {}
};

class SettingsPageTwo : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Juergen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    QLabel *ff_cmd;
    QTableWidget *ff_tableWidget;
    const QString stripString( const QString & );
    void resizeTableContents ();
    void pushItemRow( int, const QString &, const QVariant & );

  public:
    SettingsPageTwo ( QWidget *parent = 0 );
    void setTableDefaults();
    void saveOptions ( Settings * );
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
