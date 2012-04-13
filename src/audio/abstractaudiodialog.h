/*
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

#ifndef ABSTRACTAUDIODIALOG_H
#define ABSTRACTAUDIODIALOG_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QListWidget>
#include <QtGui/QWidget>

/* QX11Grab */
#include "audiodevice.h"

class AbstractAudioDialog : public QDialog
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_PROPERTY ( QString card READ getCard WRITE setCard )

  private:
    QDialogButtonBox* m_buttonBox;

  protected:
    /** Device ListWidget for User Selection */
    QListWidget* m_deviceListWidget;

    /**
    * This Virtual Method is used for Initial Audio
    * Interfaces and is needet to set in subclass construtor
    */
    virtual void initInterface() = 0;

  public:
    /**
    * \class AbstractAudioDialog
    */
    AbstractAudioDialog ( QWidget * parent = 0 );

    /** Current Card Configuration */
    const QString getCard ();

    /** Complite Card Information */
    const AudioDevice cardInfo ();

    /** Set Card Device */
    void setCard ( const QString & );

    ~AbstractAudioDialog();
};

#endif
