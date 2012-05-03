/*
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
*/

#ifndef QX11GRAB_VIDEOINFODIALOG_H
#define QX11GRAB_VIDEOINFODIALOG_H

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QWidget>

/* QX11Grab */
#include "avoptions.h"

namespace QX11Grab
{
  /**
  * \class VideoInfoDialog
  */
  class VideoInfoDialog : public QDialog
  {
      Q_OBJECT
      Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
      Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

    private:
      QLabel* m_vCodecName;
      QLabel* m_aCodecName;
      QLabel* m_bitRate;
      QLabel* m_fileName;
      QLabel* m_fileModified;
      QLabel* m_videoSize;

      void setBitrateLabel ( int );
      void setFileInfos ( const QString & );
      void setSizeInfo ( int w, int h );

      bool queryFullFileInfo ( const QByteArray & );

    public:
      /**
      * \class VideoInfoDialog
      */
      VideoInfoDialog ( QWidget * parent = 0 );

      int exec ( const QString &file );

      virtual ~VideoInfoDialog();
  };
}  /* eof namespace QX11Grab */

#endif
