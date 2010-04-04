/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
* Copyright: See COPYING file that comes with this distribution
**/

#ifndef SETTINGS_H
#define SETTINGS_H

/* QtCore */
#include <QtCore/QSettings>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QList>
#include <QtCore/QSize>
#include <QtCore/QVariant>
/* QtGui */
#include <QtGui/QWidget>
#include <QtGui/QMainWindow>

class Settings : public QSettings
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Juergen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  public:
    Settings ( QObject *parent = 0 );
    int getInt ( const QString &path );
    bool getBool ( const QString &path );
    const QString getStr ( const QString &path );
    const QSize getSize ( const QString &path, const QSize &min = QSize ( 100,250 ) );
    const QVariant getMapOption ( const QString &path, const QString &key );
    const QStringList getCommand();
    ~Settings();

};

#endif
