/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann (Undefined) http://qx11grab.hjcms.de, (C) 2007-2013
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

#ifndef WEBCAMCAPTUREFRAMES_H
#define WEBCAMCAPTUREFRAMES_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtCore/QString>
#include <QtCore/QSize>
#include <QtCore/QSocketNotifier>
#include <QtCore/QTimer>

/* QtGui */
#include <QtGui/QImage>
#include <QtGui/QPixmap>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

/* libv4l2 */
#include <libv4l2.h>
#include <libv4lconvert.h>

/* QX11Grab */
#include "webcamdeviceinfo.h"
#include "v4l2-api.h"

class WebCamCaptureFrames : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    /** v4l2-api */
    v4l2* m_v4l2;

    /** to capture frame size */
    WebCamDeviceInfo* m_devInfo;

    /** captured frame image */
    QImage* m_frameImage;

    /** monitoring opened device handle */
    QSocketNotifier* m_socketNotifier;

    /** except that the try format will succeed */
    struct v4lconvert_data* m_convertData;

    /** timer for timeout capture frames */
    QTimer* m_timer;

    /** capture button */
    QPushButton* m_button;

    /** capture max time in seconds */
    QSpinBox* m_setTimout;

    /** v4l2 datastream buffer */
    unsigned char* m_streamData;

    /** v4l2 input format  */
    struct v4l2_format m_inputFormat;

    /** v4l2 output format */
    struct v4l2_format m_outputFormat;

    /** create buffers and start capture frames */
    void startCaptureFrames ( bool );

  private Q_SLOTS:
    /**  */
    void captureFrame ( int );

    /** capture button clicked */
    void buttonClicked();

  Q_SIGNALS:
    /** captured frame */
    void frameCaptered ( const QImage & );

  public Q_SLOTS:
    /** disable capture frames */
    void stopCapture();

  public:
    explicit WebCamCaptureFrames ( QWidget * parent = 0 );

    /** init capture device */
    void setInterface ( const WebCamDeviceInfo &devInfo );

    const QSize captureSize();

    virtual ~WebCamCaptureFrames();
};

#endif
