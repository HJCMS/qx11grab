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

#include "aboutdialog.h"

/* QX11Grab */
#include "version.h"
#include "settings.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QUrl>

/* QtGui */
#include <QtGui/QDialogButtonBox>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>

AboutDialog::AboutDialog ( QWidget * parent )
    : QDialog ( parent )
{
  setObjectName ( QLatin1String ( "AboutDialog" ) );
  setWindowIcon ( Settings::themeIcon ( "qx11grab" ) );
  setMinimumSize ( 400, 250 );

  QVBoxLayout* layout = new QVBoxLayout ( this );
  layout->setObjectName ( QLatin1String ( "AboutDialog/Layout" ) );

  QTabWidget* m_tabWidget = new QTabWidget ( this );
  layout->addWidget ( m_tabWidget );

  m_about = new QLabel ( m_tabWidget );
  m_about->setObjectName ( QLatin1String ( "AboutDialog/Tab/Label1" ) );
  m_about->setWordWrap ( true );
  m_about->setTextFormat ( Qt::RichText );
  m_about->setTextInteractionFlags ( Qt::TextBrowserInteraction );
  m_about->setOpenExternalLinks ( true );
  m_tabWidget->addTab ( m_about, Settings::themeIcon ( "qx11grab" ), QLatin1String ( "QX11Grab" ) );

  m_copying = new QTextBrowser ( m_tabWidget );
  m_copying->setObjectName ( QLatin1String ( "AboutDialog/Tab/Label2" ) );
  m_copying->setOpenExternalLinks ( true );
  m_tabWidget->addTab ( m_copying, Settings::themeIcon ( "qx11grab" ), trUtf8 ( "License" ) );

  QDialogButtonBox* m_box = new QDialogButtonBox ( QDialogButtonBox::Ok, Qt::Horizontal, this );
  m_box->setObjectName ( QLatin1String ( "AboutDialog/ButtonBox" ) );
  m_box->setCenterButtons ( true );
  connect ( m_box, SIGNAL ( accepted () ), this, SLOT ( accept() ) );
  layout->addWidget ( m_box );

  insertAbout();
  insertCopying();
  setLayout ( layout );
}

void AboutDialog::insertAbout()
{
  QFile fp ( QString::fromUtf8 ( ":/html/about.html" ) );
  if ( fp.open ( QIODevice::ReadOnly ) )
  {
    QTextStream stream ( &fp );
    m_about->setText ( stream.readAll() );
    fp.close();
  }
}

void AboutDialog::insertCopying()
{
  QFile fp ( QString::fromUtf8 ( ":/html/copying.html" ) );
  if ( fp.open ( QIODevice::ReadOnly ) )
  {
    QTextStream stream ( &fp );
    m_copying->setHtml ( stream.readAll() );
    fp.close();
  }
}

AboutDialog::~AboutDialog()
{}
