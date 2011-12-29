/**
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann http://qx11grab.hjcms.de, (C) 2007-2012
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
**/

#ifndef SETTINGS_H
#define SETTINGS_H

/* QtCore */
#include <QtCore/QHash>
#include <QtCore/QSettings>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

class Settings : public QSettings
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Juergen Heinemann (Undefined)" )

  public:
    Settings ( QObject * parent = 0 );

    /**
     * Display Rubberband on Application start
     */
    bool showRubberOnStart();

    /**
     * Muster of the Univers ;)
     */
    bool expertMode();

    /**
     * Write Group Options
     */
    void saveGroup ( const QString &group, const QHash<QString,QVariant> &data );

    /**
     * Group Options
     */
    const QHash<QString,QVariant> readGroup ( const QString &group = QLatin1String ( "VideoOptions" ) );

    /**
     * FFmpeg Binary path
     **/
    const QString binaryPath();

    /**
     * Output Directory
     **/
    const QString outputDirectory();

    /**
     * Template Name
     **/
    const QString outputTemplateName();

    /**
     * temporary output file configuration
     **/
    void setOutputPath ( const QString &fullpath );

    /**
     * temporary output file configuration
     **/
    const QString absoluteOutputPath();

    /**
     * FFmpeg -loglevel
     **/
    void setLogLevel ( const QString &level );

    /**
     * FFmpeg -loglevel
     **/
    const QString logLevel();

    /**
     * Save Audio Engine
     **/
    void setAudioEngine ( const QString &engine );

    /**
     * Audio Engine
     **/
    const QString audioEngine();

    /**
     * Save Audio Device
     **/
    void setAudioDevice ( const QString &path );

    /**
     * Audio Device
     **/
    const QString audioDevice();

    /**
     * save Audio Volume
     **/
    void setAudioVolume ( qint16 i );

    /**
     * Audio Volume
     **/
    qint16 audioVolume();

    /**
     * Save Sample Format
     **/
    void setSampleFormat ( const QString &format );

    /**
     * Sample Format
     **/
    const QString sampleFormat();

    /**
     * Save Audio Media Type
     **/
    void setAudioType ( const QString &type );

    /**
     * Audio Media Type
     **/
    const QString audioType();

    /**
     * save Audio Device Description Commandline
     **/
    void setAudioDeviceCommand ( const QStringList &cmd );

    /**
     * Audio Device Description Commandline
     **/
    const QStringList getAudioDeviceCommand ();

    /**
     * save Commandline
     **/
    void setCommandLine ( const QStringList &cmd );

    /**
     * get Current Commandline
     **/
    const QStringList getCommandline();

    /**
     * read Extra Commandline Options
     **/
    const QStringList getExpertCommand();

    ~Settings();

};

#endif
