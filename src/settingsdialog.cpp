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
#include <QtCore/QMap>
#include <QtCore/QMapIterator>
#include <QtCore/QRegExp>
#include <QtCore/QString>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QFileDialog>
#include <QtGui/QIcon>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidget>

SettingsDialog::SettingsDialog ( QWidget * parent, Settings * settings )
    : QDialog ( parent )
    , cfg ( settings )
{
  setupUi ( this );

  loadSettings();
  setVideoOptionsTable();
  setAudioOptionsTable();

  QPushButton* okbtn = buttonBox->addButton ( trUtf8 ( "&Save" ), QDialogButtonBox::ActionRole );
  okbtn->setIcon ( QIcon ( QString::fromUtf8 ( ":/images/apply.png" ) ) );

  connect ( addVideoRowBtn, SIGNAL ( clicked() ), this, SLOT ( addVideoRow() ) );
  connect ( removeVideoRowBtn, SIGNAL ( clicked() ), this, SLOT ( removeVideoRow() ) );
  connect ( addAudioRowBtn, SIGNAL ( clicked() ), this, SLOT ( addAudioRow() ) );
  connect ( removeAudioRowBtn, SIGNAL ( clicked() ), this, SLOT ( removeAudioRow() ) );
  connect ( setffmpegBtn, SIGNAL ( clicked() ), this, SLOT ( setBinaryPath() ) );
  connect ( setOutputBtn, SIGNAL ( clicked() ), this, SLOT ( setOutpuDirectory() ) );
  connect ( okbtn, SIGNAL ( clicked() ), this, SLOT ( saveSettings() ) );
}

/**
* Einen Tabellen Eintrag erstellen.
*/
QTableWidgetItem* SettingsDialog::createItem ( const QString &data )
{
  QTableWidgetItem* item = new QTableWidgetItem ( QTableWidgetItem::UserType );
  item->setText ( data );
  item->setData ( Qt::ToolTipRole, data );
  item->setData ( Qt::UserRole, data );
  return item;
}

/**
* Lese aus der Konfiguration die Video Optionen
* und speichere diese in die Tabelle @ref encodingOptions
*/
void SettingsDialog::setVideoOptionsTable()
{
  int row = 0;
  QMap<QString,QVariant> map = cfg->readGroup ( "VideoOptions" );
  if ( map.size() < 1 )
    return;

  encodingOptions->clearContents();
  encodingOptions->setRowCount ( map.size() );

  QMapIterator<QString,QVariant> it ( map );
  while ( it.hasNext() )
  {
    it.next();
    encodingOptions->setItem ( row, 0, createItem ( it.key() ) );
    if ( it.value().toString().isEmpty() )
      encodingOptions->setItem ( row, 1, createItem ( QString::null ) );
    else
      encodingOptions->setItem ( row, 1, createItem ( it.value().toString() ) );

    row++;
  }
}

/**
* Lese aus der Konfiguration die Audio Optionen
* und speichere diese in die Tabelle @ref audioOptions
*/
void SettingsDialog::setAudioOptionsTable()
{
  int row = 0;
  QMap<QString,QVariant> map = cfg->readGroup ( "AudioOptions" );
  if ( map.size() < 1 )
    return;

  audioOptions->clearContents();
  audioOptions->setRowCount ( map.size() );

  QMapIterator<QString,QVariant> it ( map );
  while ( it.hasNext() )
  {
    it.next();
    audioOptions->setItem ( row, 0, createItem ( it.key() ) );
    if ( it.value().toString().isEmpty() )
      audioOptions->setItem ( row, 1, createItem ( QString::null ) );
    else
      audioOptions->setItem ( row, 1, createItem ( it.value().toString() ) );

    row++;
  }
}

/**
* Speichere die Video Optionen aus Tabelle @ref encodingOptions
*/
void SettingsDialog::saveVideoOptions()
{
  int rows = encodingOptions->rowCount();
  cfg->remove ( QLatin1String ( "VideoOptions" ) );
  if ( rows >= 1 )
  {
    cfg->beginWriteArray ( QLatin1String ( "VideoOptions" ) );
    for ( int r = 0; r < rows; r++ )
    {
      // Argument
      QTableWidgetItem* keyItem = encodingOptions->item ( r, 0 );
      /* NOTICE Wenn der Benutzer eine Leere Zeile einfügt.
      * Diese aber nicht Bearbeitet, dann verhindern das
      * an dieser Stelle das Programm wegen fehlenden
      * QTableWidgetItem Pointer nicht abstürzt! */
      if ( ! keyItem || keyItem->text().isEmpty() )
        continue;

      cfg->setArrayIndex ( r );
      cfg->setValue ( "argument", keyItem->text() );

      // Wertzuweisung
      QTableWidgetItem* valItem = encodingOptions->item ( r, 1 );
      if ( valItem && ! valItem->text().isEmpty() )
        cfg->setValue ( "value", valItem->text() );
      else
        cfg->setValue ( "value", "" );
    }
    cfg->endArray();
  }
}

/**
* Speichere die Audio Optionen aus Tabelle @ref audioOptions
*/
void SettingsDialog::saveAudioOptions()
{
  int rows = audioOptions->rowCount();
  cfg->remove ( QLatin1String ( "AudioOptions" ) );
  if ( rows >= 1 )
  {
    cfg->beginWriteArray ( QLatin1String ( "AudioOptions" ) );
    for ( int r = 0; r < rows; r++ )
    {
      // Argument
      QTableWidgetItem* keyItem = audioOptions->item ( r, 0 );
      /* NOTICE Wenn der Benutzer eine Leere Zeile einfügt.
      * Diese aber nicht Bearbeitet, dann verhindern das
      * an dieser Stelle das Programm wegen fehlenden
      * QTableWidgetItem Pointer nicht abstürzt! */
      if ( ! keyItem || keyItem->text().isEmpty() )
        continue;

      cfg->setArrayIndex ( r );
      cfg->setValue ( "argument", keyItem->text() );

      // Wertzuweisung
      QTableWidgetItem* valItem = audioOptions->item ( r, 1 );
      if ( valItem && ! valItem->text().isEmpty() )
        cfg->setValue ( "value", valItem->text() );
      else
        cfg->setValue ( "value", "" );
    }
    cfg->endArray();
  }
}

/**
* Eine neue Zeile ind Tabelle @ref encodingOptions einfügen.
*/
void SettingsDialog::addVideoRow()
{
  int cur = encodingOptions->rowCount();
  encodingOptions->setRowCount ( ++cur );
}

/**
* Ausgewählte Zeile aus @ref encodingOptions entfernen.
*/
void SettingsDialog::removeVideoRow()
{
  QList<QTableWidgetItem *> items = encodingOptions->selectedItems ();
  if ( items.size() < 1 )
    return;

  int r = items.first()->row();
  if ( r >= 0 )
    encodingOptions->removeRow ( r );
}

/**
* Eine neue Zeile ind Tabelle @ref audioOptions einfügen.
*/
void SettingsDialog::addAudioRow()
{
  int cur = audioOptions->rowCount();
  audioOptions->setRowCount ( ++cur );
}

/**
* Ausgewählte Zeile aus @ref audioOptions entfernen.
*/
void SettingsDialog::removeAudioRow()
{
  QList<QTableWidgetItem *> items = audioOptions->selectedItems ();
  if ( items.size() < 1 )
    return;

  int r = items.first()->row();
  if ( r >= 0 )
    audioOptions->removeRow ( r );
}

/**
* Lade beim Start des Dialoges alle Einstellungen.
*/
void SettingsDialog::loadSettings()
{
  startMinimized->setChecked ( cfg->value ( QLatin1String ( "startMinimized" ), false ).toBool() );
  enableAudioRecording->setChecked ( cfg->value ( QLatin1String ( "enableAudioRecording" ), false ).toBool() );
  foreach ( QLineEdit* edit, findChildren<QLineEdit*> () )
  {
    if ( ! edit->objectName().contains ( "qt_" ) )
    {
      QString value = cfg->value ( edit->objectName(), edit->text() ).toString();
      edit->setText ( value );
    }
  }
}

/**
* Speichere alle Einstellungen.
*/
void SettingsDialog::saveSettings()
{
  cfg->setValue ( QLatin1String ( "startMinimized" ), startMinimized->isChecked() );
  cfg->setValue ( QLatin1String ( "enableAudioRecording" ), enableAudioRecording->isChecked() );

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
  saveVideoOptions();
  saveAudioOptions();
}

/**
* Setze den Pfad zum FFmpeg Programm
*/
void SettingsDialog::setBinaryPath()
{
  QString path ( "/usr/bin" );
  QStringList filt;
  filt << trUtf8 ( "FFmpeg %1" ).arg ( "ffmpeg* *ffmpeg" );
  filt << trUtf8 ( "Other %1" ).arg ( "*" );

  path = QFileDialog::getOpenFileName ( this, trUtf8 ( "FFmpeg Binary" ), path, filt.join ( ";;" ) );

  QFileInfo db ( path );
  if ( db.exists() )
    ff_path->setText ( db.absoluteFilePath() );
}

/**
* Setze den Pfad zum Ausgabe Verzeichnis
*/
void SettingsDialog::setOutpuDirectory()
{
  QString path ( "/tmp" );
  path = QFileDialog::getExistingDirectory ( this, trUtf8 ( "Output Directory" ), path );

  QFileInfo db ( path );
  if ( db.isWritable() )
    tempdir->setText ( path );
}

SettingsDialog::~SettingsDialog()
{}
