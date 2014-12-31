/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann http://qx11grab.hjcms.de, (C) 2007-2015
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Library General Public
* License as published by the Free Software Foundation; either
* version 2 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU Library General Public License
* along with this library; see the file COPYING.LIB.  If not, write to
* the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
* Boston, MA 02110-1301, USA.
*/

#ifndef SETTINGS_H
#define SETTINGS_H

/* QtCore */
#include <QtCore/QHash>
#include <QtCore/QSettings>
#include <QtCore/QSize>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QIcon>
#include <QtGui/QPixmap>

class Settings : public QSettings
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Juergen Heinemann (Undefined)" )

  public:
    Settings ( QObject * parent = 0 );

    /** Display Rubberband on Application start */
    bool showRubberOnStart();

    /** Muster of the Univers ;) */
    bool expertMode();

    /**
    * Write Group Options
    * \param group  Group Section
    * \param data   Dataset
    */
    void saveGroup ( const QString &group, const QHash<QString,QVariant> &data );

    /**
    * Read Group Options
    * \param group  Group Section
    */
    const QHash<QString,QVariant> readGroup ( const QString &group );

    /** FFmpeg Binary path */
    const QString binaryPath();

    /** Output Directory */
    const QString outputDirectory();

    /** Template Name */
    const QString outputTemplateName();

    /** temporary output file configuration */
    void setOutputPath ( const QString &fullpath );

    /** temporary output file configuration */
    const QString absoluteOutputPath();

    /** FFmpeg -loglevel */
    void setLogLevel ( const QString &level );

    /**
     * FFmpeg -loglevel
     */
    const QString logLevel();

    /** Save Audio Engine */
    void setAudioEngine ( const QString &engine );

    /** Audio Engine */
    const QString audioEngine();

    /** Save Audio Device */
    void setAudioDevice ( const QString &path );

    /** Audio Device */
    const QString audioDevice();

    /** save Audio Volume */
    void setAudioVolume ( qint16 i );

    /** Audio Volume */
    qint16 audioVolume();

    /** Save Sample Format */
    void setSampleFormat ( const QString &format );

    /** Sample Format */
    const QString sampleFormat();

    /** Save Audio Media Type */
    void setAudioType ( const QString &type );

    /** Audio Media Type */
    const QString audioType();

    /** save Audio Device Description Commandline */
    void setAudioDeviceCommand ( const QStringList &cmd );

    /** Audio Device Description Commandline */
    const QStringList getAudioDeviceCommand ();

    /** save Commandline */
    void setCommandLine ( const QStringList &cmd );

    /** get Current Commandline */
    const QStringList getCommandline();

    /** read Extra Commandline Options */
    const QStringList getExpertCommand();

    /**
     * read Array Item
     * \param group  configuration section
     * \param ref    settings value keyword
     * \param value  e.g. ( Settings::value(key) == val )
     * \param sub    search value from other subkeyword
     */
    const QVariant getArrayItem ( const QString &group, const QString &ref,
                                  const QString &value, const QString &sub );

    /**
    * Returns the QIcon corresponding to name in the current icon theme.
    * If no such icon is found in the oxygen theme fallback from qrc is returned instead.
     * \param icon    icon name
     * \param broken  fallback from qrc
    */
    static const QIcon themeIcon ( const QString &icon,
                                   const QString &broken = QLatin1String ( "broken" ) );

    /**
    * find PNG Image in ${prefix}/share/pixmaps/qx11grab/${name}.png
    * If not found a null Pixmap is returned
    * \param name Filename without Extension
    * \param size Resize Pixmap to Size
    * \note This function was introduced in QX11Grab v0.4.4
    */
    static const QPixmap pixmapIcon ( const QString &name, const QSize &size = QSize ( 48, 48 ) );

    /** recording logfile */
    static const QString logfile();

    /** Contact E-Mail */
    static const char* aboutMail();

    ~Settings();

};

#endif
