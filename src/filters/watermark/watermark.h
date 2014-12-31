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

#ifndef QX11GRAB_WATERMARK_H
#define QX11GRAB_WATERMARK_H

/* QtCore */
#include <QtCore/QObject>
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

class ImagePreview;

class Q_DECL_EXPORT Watermark : public QDialog
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    QSettings* cfg;
    QString p_Overlay; // overlay position
    QString p_Image; // imageFilePath
    qreal p_MarginX; // Margin (x)
    qreal p_MarginY; // Margin (y)
    ImagePreview* m_imagePreview;
    QSlider* m_xIndent; // Margin (x)
    QSlider* m_yIndent; // Margin (y)
    QComboBox* m_setOverlayComboBox;
    QSlider* m_transparency;
    QLineEdit* m_outputEdit; // Filter Preview

    void setSettings ( const QString &key, const QVariant &value );
    const QVariant settingsValue ( const QString &key, const QVariant &defaultValue = QVariant() );
    void loadDefaults();

    bool rewritePixmap ();

    /** watermark overlays */
    enum CORNER_POSITION
    {
      TOP_LEFT_CORNER, /**< Top left corner */
      TOP_RIGHT_CORNER, /**< Top right corner */
      BOTTOM_LEFT_CORNER, /**< Bottom left corner */
      BOTTOM_RIGHT_CORNER /**< Bottom right corner */
    };

  private Q_SLOTS:
    /** send Notification Message to QX11Grab Messanger */
    void sendMessage ( const QString &title, const QString &text );

    /** combobox position changed */
    void positionIndexChanged ( int index );

    /** filedialog to open images */
    void openPixmapDialog();

    /** left|right Margin */
    void setMarginX ( int x );

    /** top|bottom Margin */
    void setMarginY ( int y );

    /** image transparency changed */
    void setTransparency ( int i );

    /** update/repaint */
    void update ();

  protected:
    virtual void resizeEvent ( QResizeEvent * event );

  public:
    explicit Watermark ( QWidget * parent = 0 );

    /** open dialog */
    int start();

    Q_SCRIPTABLE const QString data();

    ~Watermark();
};

#endif

