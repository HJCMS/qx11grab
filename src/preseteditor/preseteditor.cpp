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

#include "preseteditor.h"

/* QX11Grab */
#include "presetedit.h"
#include "presetselector.h"
#include "filesavedialog.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QPair>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QTextStream>

/* QtGui */
#include <QtGui/QIcon>
#include <QtGui/QVBoxLayout>

PresetEditor::PresetEditor ( QWidget * parent )
    : QDialog ( parent )
{
  setObjectName ( QLatin1String ( "PresetEditor" ) );
  setWindowIcon ( QIcon::fromTheme ( "qx11grab" ) );
  setSizeGripEnabled ( true );

  QVBoxLayout* layout = new QVBoxLayout ( this );
  layout->setObjectName ( QLatin1String ( "PresetEditor/Layout" ) );

  m_presetSelector = new PresetSelector ( this );
  layout->addWidget ( m_presetSelector );

  m_presetEdit = new PresetEdit ( this );
  layout->addWidget ( m_presetEdit );

  m_buttonBox = new QDialogButtonBox ( Qt::Horizontal, this );
  m_buttonBox->setStandardButtons ( ( QDialogButtonBox::Save | QDialogButtonBox::Close ) );
  layout->addWidget ( m_buttonBox );

  setLayout ( layout );

  connect ( m_presetSelector, SIGNAL ( openPreset ( const QString & ) ),
            m_presetEdit, SLOT ( openPreset ( const QString & ) ) );

  connect ( m_buttonBox, SIGNAL ( accepted () ), this, SLOT ( saveAndExit() ) );
  connect ( m_buttonBox, SIGNAL ( rejected () ), this, SLOT ( reject() ) );
}

/**
* Schreibe ffpreset mit Neuen Namen nach ~/.ffmpeg/
*/
bool PresetEditor::setOtherFileSaveTarget ( const QString &name, const QString &content )
{
  bool b = false;
  FileSaveDialog* d = new FileSaveDialog ( name, content, this );
  if ( d->exec() == QDialog::Accepted )
    b = true;

  delete d;

  return b;
}

/**
* Suche Daten zum speichern, bei bedarf werden
* weitere Dialoge zum speichern aufgerufen.
*/
void PresetEditor::saveAndExit()
{
  QPair<QString,QString> pair = m_presetEdit->data();
  QFileInfo info ( pair.first );
  // Wenn beschreibbar dann speichern!
  if ( info.exists() && info.isWritable() )
  {
    QFile fp ( info.absoluteFilePath() );
    if ( fp.open ( QIODevice::WriteOnly ) )
    {
      QTextStream stream ( &fp );
      stream << pair.second;
      fp.close();
      accept();
    }
  }
  else if ( setOtherFileSaveTarget ( info.completeBaseName(), pair.second ) )
  {
    // Wenn mit neuen Namen gespeichert
    accept();
  }
  else
  {
    // Alles ist fehlgeschlagen
    reject();
  }
}

PresetEditor::~PresetEditor()
{}
