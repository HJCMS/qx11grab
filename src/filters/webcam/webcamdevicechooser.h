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

#ifndef WEBCAMDEVICECHOOSER_H
#define WEBCAMDEVICECHOOSER_H

/* QtCore */
#include <QtCore/QFileInfo>
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QSize>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QComboBox>
#include <QtGui/QIcon>
#include <QtGui/QWidget>

/* QX11Grab */
#include "webcamdeviceinfo.h"

class WebCamDeviceChooser : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    QComboBox* m_setWebcamDevice; /**< set device */
    QComboBox* m_setCaptureSize; /**< select predefined framesize */
    const QIcon cameraIcon;
    QList<WebCamDeviceInfo> devInfo;
    inline const QString toString ( unsigned char* ptr ) const;
    bool insertWebCamDevice ( const QFileInfo &dev );
    void searchDevices();

  private Q_SLOTS:
    void itemChanged ( int index );
    void itemSizeChanged ( int index );

  Q_SIGNALS:
    void cameraSelected ( const WebCamDeviceInfo & );
    void captureSizeSelected ( const QSize & );

  public Q_SLOTS:
    void init();

  public:
    explicit WebCamDeviceChooser ( QWidget * parent = 0 );
    ~WebCamDeviceChooser();
};

#endif
