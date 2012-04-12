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

#ifndef QX11GRAB_WEBCAM_H
#define QX11GRAB_WEBCAM_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QSizeF>
#include <QtCore/QSettings>
#include <QtCore/QString>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QLineEdit>
#include <QtGui/QResizeEvent>
#include <QtGui/QSlider>
#include <QtGui/QWidget>

/* QX11Grab */
#include "v4l2-api.h"
#include "webcamdeviceinfo.h"

class WebCamPreview;
class WebCamDeviceChooser;
class WebCamCaptureFrames;

class Webcam : public QDialog
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    WebCamDeviceChooser* m_webcamDeviceChooser;
    WebCamCaptureFrames* m_webCamCaptureFrames;
    QSettings* cfg;
    QString p_Overlay; /**< overlay orientation */
    QString p_v4lDevice; /**< video4linux device */
    qreal p_MarginX; /**< Margin (x) */
    qreal p_MarginY; /**< Margin (y) */
    qreal p_ScaleFactor; /**< Factor for FrameScale */
    WebCamPreview* m_webcamPreview;
    QSlider* m_scaleFrame; /**< Scale Camera Frames */
    QSlider* m_xIndent; /**< Margin (x) */
    QSlider* m_yIndent; /**< Margin (y) */
    QComboBox* m_setOverlayComboBox; /**< Overlay Selection */
    QLineEdit* m_outputEdit; /**< Filter Preview */

    void setSettings ( const QString &key, const QVariant &value );
    const QVariant settingsValue ( const QString &key, const QVariant &defaultValue = QVariant() );
    void loadDefaults();

    /** webcam overlay position */
    enum CORNER_POSITION
    {
      TOP_LEFT_CORNER, /**< Top left corner */
      TOP_RIGHT_CORNER, /**< Top right corner */
      BOTTOM_LEFT_CORNER, /**< Bottom left corner */
      BOTTOM_RIGHT_CORNER /**< Bottom right corner */
    };

  private Q_SLOTS:
    /** camera changed */
    void cameraDeviceChanged ( const WebCamDeviceInfo & );

    /** combobox position changed */
    void positionIndexChanged ( int index );

    /** update/repaint graphicsscene */
    void updateScene();

    /** scale cam frame */
    void setScale ( int s );

    /** left|right Margin */
    void setMarginX ( int x );

    /** top|bottom Margin */
    void setMarginY ( int y );

    /** update/comamnd */
    void updateCommand();

  protected:
    virtual void resizeEvent ( QResizeEvent * event );

  public:
    explicit Webcam ( QWidget * parent = 0 );

    /** open dialog and init all subobjects */
    int start();

    /** get generated filter data */
    Q_SCRIPTABLE const QString data();

    ~Webcam();
};

#endif
