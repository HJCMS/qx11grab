/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann (Undefined) http://qx11grab.hjcms.de, (C) 2007-2015
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

#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QListWidget>
#include <QtGui/QStackedWidget>
#include <QtGui/QWidget>

/* QX11Grab */
#include "settings.h"

class MainFunctions;
class TargetsWidget;
class AudioDeviceWidget;
class ExtraOptions;
class ConfigExtensions;
class ConfigRubberband;
class Twitch;

class ConfigDialog : public QDialog
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    Settings* cfg;
    QStackedWidget* m_stackedWidget;
    MainFunctions* m_mainFunctions;
    TargetsWidget* m_targets;
    AudioDeviceWidget* m_audioDeviceWidget;
    ExtraOptions* m_extraOptions;
    ConfigExtensions* m_configExtensions;
    ConfigRubberband* m_configRubberband;
    Twitch* m_configTwitch;
    QListWidget* m_listWidget;
    QDialogButtonBox* m_buttonBox;

    void insertMenuItem ( int index, const QString &title, const QString &icon );

  private Q_SLOTS:
    void checkDistinctions ( bool );
    void loadSettings();
    void saveAndExit();

  public:
    ConfigDialog ( Settings * settings, QWidget * parent = 0 );
    ~ConfigDialog();
};

#endif
