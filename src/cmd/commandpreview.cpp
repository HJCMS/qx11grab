/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann http://qx11grab.hjcms.de, (C) 2007-2013
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

#include "commandpreview.h"
#include "commandlineedit.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QRegExp>

/* QtGui */
#include <QtGui/QLabel>
#include <QtGui/QIcon>
#include <QtGui/QToolButton>
#include <QtGui/QGridLayout>

CommandPreview::CommandPreview ( QWidget * parent )
    : QWidget ( parent )
{
  setObjectName ( QLatin1String ( "commandpreview" ) );
  setContentsMargins ( 5, 5, 5, 5 );

  QGridLayout* layout = new QGridLayout ( this );
  layout->setContentsMargins ( 0, 0, 0, 0 );

  QLabel* info = new QLabel ( trUtf8 ( "Display the current FFmpeg command." ), this );
  layout->addWidget ( info, 0, 0, 1, 1, Qt::AlignLeft );

  QToolButton* refreshButton = new QToolButton ( this );
  /*: ToolTip */
  refreshButton->setToolTip ( trUtf8 ( "Restore to current Configuration" ) );
  refreshButton->setIcon ( QIcon::fromTheme ( "edit-undo" ) );
  layout->addWidget ( refreshButton, 0, 1, 1, 1, Qt::AlignRight );

  m_commandLineEdit = new CommandLineEdit ( this );
  layout->addWidget ( m_commandLineEdit, 1, 0, 1, 2 );

  setLayout ( layout );

  connect ( refreshButton, SIGNAL ( clicked () ),
            this, SIGNAL ( restoreRequest() ) );

  connect ( m_commandLineEdit, SIGNAL ( restoreRequest () ),
            this, SIGNAL ( restoreRequest() ) );

  connect ( m_commandLineEdit, SIGNAL ( dataSaved ( const QStringList & ) ),
            this, SIGNAL ( dataSaved ( const QStringList & ) ) );
}

/*
* Kommandozeile aufsplitten
*/
void CommandPreview::setCommandLine ( QStringList &list )
{
  QString buf ( list.join ( " " ) );
  QString cmd = buf.replace ( QRegExp ( "[\\s\\t]+\\-" ), QString::fromUtf8 ( "#-" ) );
  m_commandLineEdit->setData ( cmd.split ( "#" ) );
}

/*
* Die vom Benutzer Editierte Kommandozeile zurück geben!
*/
const QStringList CommandPreview::currentCommandLine()
{
  // qDebug() << Q_FUNC_INFO << m_commandLineEdit->data();
  return m_commandLineEdit->data();
}

CommandPreview::~CommandPreview()
{}
