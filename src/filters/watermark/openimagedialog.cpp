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

#include "openimagedialog.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QDesktopServices>
#include <QtGui/QPixmap>

OpenImageDialog::OpenImageDialog ( QWidget * parent )
    : QFileDialog ( parent )
    , p_imageFile ( QString::null )
{
  setObjectName ( QLatin1String ( "OpenImageDialog" ) );
  /*: WindowTitle */
  setWindowTitle ( trUtf8 ( "Open Image File" ) );
  setOptions ( QFileDialog::DontResolveSymlinks );
  setFileMode ( QFileDialog::ExistingFile );
  setViewMode ( QFileDialog::Detail );
  setNameFilter ( trUtf8 ( "Alpha Channel PNG %1" ).arg ( "(*.png *.PNG)" ) );
  setDefaultSuffix ( QLatin1String ( "png" ) );
  setDirectory ( QDesktopServices::storageLocation ( QDesktopServices::PicturesLocation ) );
}

bool OpenImageDialog::isIntersected ( const QRect &rect )
{
  QSettings cfg;
  QRect area = cfg.value ( "Grabber/Dimension" ).toRect();
  QRect bRect ( rect );
  bRect.moveTo ( area.topLeft() );
  if ( area.isValid() && ( rect.width() < area.width() ) && ( rect.height() < area.height() ) )
    return true;

  return false;
}

/**
* Es gibt Einschränkungen was die Bild Größe und den Farbraum betrifft!
* \li Das Bild darf nicht Größer als der Aufnhamebereich sein!
* \li Es kann kein Bild mit einer anderen Farbtiefe als das Video eingefügt werden.
* Aus diesem Grund wird jedes Bild zuerst überprüft bevor der Dialog nach der
* Dateiauswahl geschlossen werden kann.
*/
bool OpenImageDialog::isValidImage ( const QString &file )
{
  QString title ( trUtf8 ( "Assumption Rejected" ) );
  QPixmap pixmap ( file, "PNG", Qt::ColorOnly );
  if ( ! isIntersected ( pixmap.rect() ) )
  {
    emit message ( title, trUtf8 ( "selected image is larger then tapped rectangle." ) );
    p_imageFile = QString::null;
    return false;
  }
  else if ( ! pixmap.hasAlphaChannel() )
  {
    emit message ( title, trUtf8 ( "selected image has no legal transparency." ) );
    p_imageFile = QString::null;
    return false;
  }
  else if ( ! pixmap.isNull() )
  {
    p_imageFile = file;
    return true;
  }
  else
  {
    p_imageFile = QString::null;
    return false;
  }
}

void OpenImageDialog::accept()
{
  if ( isValidImage ( selectedFiles().last() ) )
    done ( QDialog::Accepted );
}

const QString OpenImageDialog::filePath()
{
  return p_imageFile;
}

OpenImageDialog::~OpenImageDialog()
{}
