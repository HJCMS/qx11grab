/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
* Copyright: See COPYING file that comes with this distribution
**/

#include "settingsdialog.h"
#include "settings.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QRegExp>
#include <QtCore/QString>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QFileDialog>
#include <QtGui/QIcon>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

SettingsDialog::SettingsDialog ( QWidget * parent, Settings * settings )
    : QDialog ( parent )
    , cfg ( settings )
{
  setupUi ( this );

  loadSettings();

  QPushButton* okbtn = buttonBox->addButton ( trUtf8 ( "&Ok" ), QDialogButtonBox::ActionRole );
  okbtn->setIcon ( QIcon ( QString::fromUtf8 ( ":/images/apply.png" ) ) );

  connect ( setffmpegBtn, SIGNAL ( clicked() ), this, SLOT ( setBinaryPath() ) );
  connect ( setOutputBtn, SIGNAL ( clicked() ), this, SLOT ( setOutpuDirectory() ) );
  connect ( okbtn, SIGNAL ( clicked() ), this, SLOT ( saveSettings() ) );
}

void SettingsDialog::loadSettings()
{
  startMinimized->setChecked ( cfg->value ( QLatin1String ( "startMinimized" ), false ).toBool() );
  foreach ( QLineEdit* edit, findChildren<QLineEdit*> () )
  {
    if ( ! edit->objectName().contains ( "qt_" ) )
    {
      QString value = cfg->value ( edit->objectName(), edit->text() ).toString();
      edit->setText ( value );
    }
  }
}

void SettingsDialog::saveSettings()
{
  cfg->setValue ( QLatin1String ( "startMinimized" ), startMinimized->isChecked() );

  foreach ( QLineEdit* edit, findChildren<QLineEdit*> () )
  {
    if ( !edit->objectName().contains ( "qt_" ) )
    {
      if ( edit->text().isEmpty() )
        cfg->remove ( edit->objectName() );
      else
        cfg->setValue ( edit->objectName(), edit->text() );
    }
  }
  // Video Optionen
  int rows = encodingOptions->rowCount();
  cfg->remove ( QLatin1String ( "VideoOptions" ) );
  if ( rows >= 1 )
  {
    cfg->beginWriteArray ( QLatin1String ( "VideoOptions" ) );
    for ( int r = 0; r < rows; r++ )
    {
      QString key = encodingOptions->item ( r, 0 )->data ( Qt::EditRole ).toString();
      QString val = encodingOptions->item ( r, 1 )->data ( Qt::EditRole ).toString();
      cfg->setArrayIndex ( r );
      cfg->setValue ( "argument", key );
      cfg->setValue ( "value", val );
    }
    cfg->endArray();
  }
}

void SettingsDialog::setBinaryPath()
{
  QString path ( "/usr/bin" );
  QStringList filt;
  filt << trUtf8 ( "FFmpeg %1" ).arg ( "ffmpeg* *ffmpeg" );
  filt << trUtf8 ( "Other %1" ).arg ( "*" );

  path = QFileDialog::getOpenFileName ( this, trUtf8 ( "Settings FFmpeg Binary" ), path, filt.join ( ";;" ) );

  QFileInfo db ( path );
  if ( db.exists() )
    ff_path->setText ( db.absoluteFilePath() );
}

void SettingsDialog::setOutpuDirectory()
{
  QString path ( "/tmp" );
  path = QFileDialog::getExistingDirectory ( this, trUtf8 ( "Settings Output Directory" ), path );

  QFileInfo db ( path );
  if ( db.isWritable() )
    tempdir->setText ( path );
}

SettingsDialog::~SettingsDialog()
{}
