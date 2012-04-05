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

#ifndef WEBCAMDEVICE_H
#define WEBCAMDEVICE_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QSocketNotifier>
#include <QtCore/QThread>

/* QtGui */
#include <QtGui/QImage>

/* QX11Grab */
#include "v4l2-api.h"

class WebCamDevice : public QThread, public v4l2
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    const QString &v4l2dev;
    QImage* m_captureImage;
    QSocketNotifier* m_listener;

    enum CapMethod
    {
      methodRead,
      methodMmap,
      methodUser
    };

    struct buffer
    {
      void   *start;
      size_t  length;
    };
    struct buffer* m_buffers;

    unsigned m_nbuffers;
    unsigned m_frame;
    unsigned m_lastFrame;
    unsigned m_fps;
    unsigned char *m_frameData;

    struct v4l2_format m_capSrcFormat;
    struct v4l2_format m_capDestFormat;
    struct v4lconvert_data* m_convertData;

    bool startCapture ();
    void stopCapture();

  private Q_SLOTS:
    void captureFrame();
    void deviceOpened();
    void deviceClosed();

  protected:
    virtual void run();

  Q_SIGNALS:
    void frameEntered ( const QImage & );

  public:
    explicit WebCamDevice ( const QString &device, QObject * parent = 0 );

    virtual ~WebCamDevice();
};

#endif
