/**
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann (Undefined) http://qx11grab.hjcms.de, (C) 2007-2012
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

#ifndef WEBCAMCAPTUREFRAMES_H
#define WEBCAMCAPTUREFRAMES_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtCore/QString>
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

/* QX11Grab */
#include "webcamdeviceinfo.h"
#include "v4l2-api.h"

class WebCamCaptureFrames : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    v4l2* m_v4l2;
    QImage* m_frameImage;
    QSocketNotifier* m_socketNotifier;
    struct v4lconvert_data* m_convertData;
    QString p_device;

    QTimer* m_timer;
    QPushButton* m_button;
    QSpinBox* m_setTimout;

    unsigned char* m_streamData;
    struct v4l2_format m_inputFormat;
    struct v4l2_format m_outputFormat;

    void startCaptureFrames ( bool );

  private Q_SLOTS:
    void captureFrame ( int );
    void buttonClicked();

  Q_SIGNALS:
    void frameCaptered ( const QImage & );

  public Q_SLOTS:
    void stopCapture();

  public:
    explicit WebCamCaptureFrames ( QWidget * parent = 0 );
    void setInterface ( const WebCamDeviceInfo & );
    virtual ~WebCamCaptureFrames();
};

#endif
