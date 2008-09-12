/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2008
* Copyright: See COPYING file that comes with this distribution
**/

#ifndef SETTINGSPAGEONE_H
#define SETTINGSPAGEONE_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QMap>

/* QtGui */
#include <QtGui/QWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QCheckBox>
#include <QtGui/QGroupBox>

class SettingsItem : public QGroupBox
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Juergen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    QLabel *label;
    QLineEdit *edit;

  public:
    SettingsItem ( QWidget *parent = 0 );
    void setText ( const QString & );
    void setValue ( const QVariant & );
    const QVariant value();
    ~SettingsItem() {}
};

class SettingsPageOne : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Juergen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    QVBoxLayout *topLayout;
    QCheckBox *startMinimized;
    const QMap<QString,QVariant> getDefaults ();
    const QString getDescription ( const QString &key );

  public:
    SettingsPageOne ( QWidget *parent = 0 );
    const QMap<QString,QVariant> getOptions();
    ~SettingsPageOne();

  public Q_SLOTS:
    void setOptions ( const QMap<QString,QVariant> & );
    void setDefaults ();

};

#endif
