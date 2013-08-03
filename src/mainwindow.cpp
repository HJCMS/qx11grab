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

/* QX11Grab */
#include "version.h"

#include "mainwindow.h"
#include "settings.h"
#include "menubar.h"
#include "toolbar.h"
#include "systemtray.h"

/* QX11Grab subtargets */
#include "rubberband.h"
#include "desktoptapping.h"
#include "desktopinfo.h"
#include "grabberinfo.h"
#include "videotableeditor.h"
#include "audiotableeditor.h"
#include "metadata.h"
#include "ffprocess.h"
#include "commandpreview.h"
#include "logviewer.h"
#include "exportdialog.h"
#include "bookmarkdialog.h"
#include "bookmark.h"
#include "configdialog.h"
#include "preseteditor.h"

/* QtCore */
#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QIODevice>
#include <QtCore/QString>
#include <QtCore/QTextStream>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QAction>
#include <QtGui/QDesktopWidget>
#include <QtGui/QFileDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QIcon>
#include <QtGui/QInputDialog>
#include <QtGui/QKeySequence>
#include <QtGui/QMessageBox>
#include <QtGui/QPalette>
#include <QtGui/QPixmap>
#include <QtGui/QRubberBand>
#include <QtGui/QVBoxLayout>
#include <QtGui/QX11Info>

/* QtDBus */
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusMessage>

MainWindow::MainWindow ( Settings * settings )
    : QMainWindow()
    , cfg ( settings )
    , m_FFProcess ( 0 )
{
  setObjectName ( QLatin1String ( "qx11grab" ) );
  setWindowTitle ( QString::fromUtf8 ( "QX11Grab (%1)[*]" ).arg ( QX11GRAB_VERSION_STRING ) );
  setMinimumWidth ( 450 );
  setMinimumHeight ( 400 );
  setWindowFlags ( ( windowFlags() | Qt::WindowContextHelpButtonHint ) );
  setContentsMargins ( 0, 5, 0, 5 );
#ifdef HAVE_OPENGL
  setAnimated ( true );
#endif

  QIcon boxIcon = cfg->themeIcon ( "qx11grab" );
  setWindowIcon ( boxIcon );

  m_menuBar = new MenuBar ( this );
  setMenuBar ( m_menuBar );

  m_toolBar = new ToolBar ( this );
  addToolBar ( Qt::TopToolBarArea, m_toolBar );

  statusBar()->show();

  // Process Management
  m_FFProcess = new FFProcess ( this, cfg );

  QWidget* layerWidget = new QWidget ( this );
  layerWidget->setContentsMargins ( 0, 0, 0, 0 );

  QVBoxLayout* verticalLayout = new QVBoxLayout ( layerWidget );
  verticalLayout->setContentsMargins ( 0, 0, 0, 0 );

  m_toolBox = new QToolBox ( layerWidget, Qt::Widget );
  m_toolBox->setObjectName ( QLatin1String ( "toolbox" ) );
  m_toolBox->setBackgroundRole ( QPalette::Window );
  m_toolBox->setContentsMargins ( 0, 5, 0, 10 );
  verticalLayout->addWidget ( m_toolBox );

  // Dimension {
  m_grabberInfo = new GrabberInfo ( m_toolBox );
  m_toolBox->addItem ( m_grabberInfo, cfg->themeIcon ( "user-desktop", "qx11grab" ), trUtf8 ( "Dimension" ) );
  // } Dimension

  // MetaData {
  m_metaData = new MetaData ( m_toolBox );
  m_metaData->setToolTip ( QString::fromUtf8 ( "-metadata" ) );
  m_toolBox->addItem ( m_metaData, cfg->themeIcon ( "user-properties", "user-identity" ), trUtf8 ( "Metadata" ) );
  // } MetaData

  // vCodec {
  m_videoEditor = new VideoTableEditor ( m_toolBox );
  m_toolBox->addItem ( m_videoEditor, cfg->themeIcon ( "tool-animator", "qx11grab" ), trUtf8 ( "Video" ) );
  // } vCodec

  // aCodec {
  m_audioGroupBox = new QGroupBox ( m_toolBox );
  m_audioGroupBox->setObjectName ( QString::fromUtf8 ( "acodec" ) );
  m_audioGroupBox->setFlat ( true );
  m_audioGroupBox->setCheckable ( true );
  m_audioGroupBox->setTitle ( trUtf8 ( "Audio Recording" ) );
  /*: WhatsThis */
  m_audioGroupBox->setWhatsThis ( trUtf8 ( "enable/disable audio recording in the captured video" ) );
  m_toolBox->addItem ( m_audioGroupBox, cfg->themeIcon ( "audio-input-microphone", "qx11grab" ), trUtf8 ( "Audio" ) );

  QVBoxLayout* audioBoxlayout = new QVBoxLayout ( m_audioGroupBox );
  m_audioEditor = new AudioTableEditor ( m_audioGroupBox );
  audioBoxlayout->addWidget ( m_audioEditor );
  m_audioGroupBox->setLayout ( audioBoxlayout );
  // } aCodec

  // Preview {
  m_commandPreview = new CommandPreview ( m_toolBox );
  /*: ToolTip */
  m_commandPreview->setToolTip ( trUtf8 ( "command line preview" ) );
  m_toolBox->addItem ( m_commandPreview, cfg->themeIcon ( "ffmpeg" ), trUtf8 ( "FFmpeg" ) );
  // } Preview

  layerWidget->setLayout ( verticalLayout );
  setCentralWidget ( layerWidget );

  /* init Actions */
  loadStats();
  createEnviroment();
  createSystemTrayIcon();

  /* Signals */
  connect ( m_FFProcess, SIGNAL ( message ( const QString & ) ),
            this, SLOT ( pushInfoMessage ( const QString & ) ) );

  connect ( m_FFProcess, SIGNAL ( errmessage ( const QString &, const QString & ) ),
            this, SLOT ( pushErrorMessage ( const QString &, const QString & ) ) );

  connect ( m_FFProcess, SIGNAL ( statusMessage ( const QString & ) ),
            this, SLOT ( statusBarMessage ( const QString & ) ) );

  connect ( m_FFProcess, SIGNAL ( statusMessage ( const QString & ) ),
            m_systemTray, SLOT ( setCustomToolTip ( const QString & ) ) );

  connect ( m_grabberInfo, SIGNAL ( screenDataChanged ( bool ) ),
            this, SLOT ( toRubber ( bool ) ) );

  connect ( m_grabberInfo, SIGNAL ( showRubber ( bool ) ),
            this, SLOT ( showRubber ( bool ) ) );

  connect ( this, SIGNAL ( stopRecording() ),
            m_FFProcess, SLOT ( stop() ) );

  connect ( this, SIGNAL ( killProcess() ),
            m_FFProcess, SLOT ( kill() ) );

  connect ( m_FFProcess, SIGNAL ( down() ),
            this, SLOT ( setActionsBack() ) );

  connect ( m_commandPreview, SIGNAL ( dataSaved ( const QStringList & ) ),
            this, SLOT ( updateCommandLine ( const QStringList & ) ) );

  // Widget Updates
  connect ( m_grabberInfo, SIGNAL ( postUpdate() ),
            this, SLOT ( preparePreview() ) );

  connect ( m_metaData, SIGNAL ( postUpdate() ),
            this, SLOT ( preparePreview() ) );

  connect ( m_videoEditor, SIGNAL ( postUpdate() ),
            this, SLOT ( preparePreview() ) );

  connect ( m_audioGroupBox, SIGNAL ( toggled ( bool ) ),
            this, SLOT ( preparePreviewRequest ( bool ) ) );

  connect ( m_audioEditor, SIGNAL ( postUpdate() ),
            this, SLOT ( preparePreview() ) );

  connect ( m_commandPreview, SIGNAL ( restoreRequest() ),
            this, SLOT ( preparePreview() ) );

  setWindowModified ( false );
}

/**
* Started die Aufnahme
*/
void MainWindow::record()
{
  if ( ! m_FFProcess || m_FFProcess->isRunning() )
    return;

  startRecord();
}

/**
* Aufnahme beenden
*/
void MainWindow::stop()
{
  if ( ! m_FFProcess )
    return;

  if ( m_FFProcess->isRunning() )
    m_FFProcess->stop();
}

/**
* Initialisiert die Desktop Umgebung
*/
void MainWindow::createEnviroment()
{
  m_DesktopInfo = new DesktopInfo ( this );
  m_DesktopInfo->grabScreenGeometry ( centralWidget() );

  m_RubberBand = new RubberBand ( m_DesktopInfo->screenWidget() );
  connect ( m_RubberBand, SIGNAL ( error ( const QString &, const QString & ) ),
            this, SLOT ( pushErrorMessage ( const QString &, const QString & ) ) );

  showRubber ( cfg->showRubberOnStart() );

  toRubber ( true );

  m_desktopTapping = new DesktopTapping ( this );
  connect ( m_desktopTapping, SIGNAL ( rectChanged ( const QRect &, int ) ),
            m_grabberInfo, SLOT ( setRect ( const QRect &, int ) ) );
}

/**
* Das Symbol in der System Statusleiste setzen.
*/
void MainWindow::createSystemTrayIcon()
{
  m_systemTray = new SystemTray ( this );
  connect ( m_systemTray, SIGNAL ( activated ( QSystemTrayIcon::ActivationReason ) ),
            this, SLOT ( systemTrayWatcher ( QSystemTrayIcon::ActivationReason ) ) );

  m_systemTray->show();

  if ( ! ( cfg->value ( "startMinimized", false ).toBool() ) )
    show();
}

/**
* Ein/Ausblenden funktion für die Gummibandanzeige.
*/
void MainWindow::showRubber ( bool b )
{
  if ( b )
    m_RubberBand->show();
  else
    m_RubberBand->hide();
}

/**
* Automtische Ein/Ausblenden funktion für die Gummibandanzeige.
*/
void MainWindow::swapRubberBand()
{
  showRubber ( ( ( m_RubberBand->isVisible() ) ? false : true ) );
}

/**
* Lese die Fenster Geometrie neu ein.
* @note Wird immer nur bei Konstruktor und show() aufgerufen!
*/
void MainWindow::loadStats()
{
  if ( cfg->contains ( "window/position" ) )
    move ( cfg->value ( "window/position", pos() ).toPoint() );

  if ( cfg->contains ( "window/size" ) )
    resize ( cfg->value ( "window/size", size() ).toSize() );

  if ( cfg->contains ( "window/state" ) )
    restoreState ( cfg->value ( "window/state", QByteArray() ).toByteArray() );

  loadSettings();

  QFileInfo log ( cfg->logfile() );
  m_toolBar->setActionsEnabled ( log.exists() );
}

/**
* Fenster Verhältnisse Speichern
* @note Wird immer nur bei hide() und \ref shutdown() aufgerufen!
*/
void MainWindow::saveStats()
{
  cfg->setValue ( "window/state", saveState ( 0 ) );
  cfg->setValue ( "window/position", pos() );
  cfg->setValue ( "window/size", size() );
}

/**
* Ausgabepfad erstellen und zwischenspeichern!
* @note Der Dateiname ändert sich bei jedem aufruf!
*/
const QString MainWindow::generateOutputFile()
{
  // Ausgabe Verzeichnis
  QString dest = cfg->outputDirectory();
  dest.append ( "/" );

  // Dateiname
  QString f = cfg->outputTemplateName();
  if ( f.contains ( "XXX" ) )
  {
    QDateTime dt = QDateTime::currentDateTime();
    QString timeStamp = QString::number ( dt.date().dayOfYear() );
    timeStamp.append ( dt.toString ( "hhmm" ) );
    f.replace ( QRegExp ( "\\b(X{3,})\\b" ), timeStamp );
  }
  dest.append ( f );

  QString outFile;
  QString codec = videoCodec();
  // Datei Erweiterung ermitteln
  QString extension = m_videoEditor->selectedCodecExtension();
  if ( ! extension.isEmpty() )
    outFile = QString ( "%1.%2" ).arg ( dest, extension );
  else if ( codec.contains ( "theora", Qt::CaseInsensitive ) )
    outFile = QString ( "%1.ogg" ).arg ( dest );
  else if ( codec.contains ( "libvpx", Qt::CaseInsensitive ) )
    outFile = QString ( "%1.webm" ).arg ( dest );
  else if ( codec.contains ( "libx264", Qt::CaseInsensitive ) )
    outFile = QString ( "%1.mkv" ).arg ( dest );
  else if ( codec.contains ( "mpeg4", Qt::CaseInsensitive ) )
    outFile = QString ( "%1.avi" ).arg ( dest );
  else if ( codec.contains ( "h26", Qt::CaseInsensitive ) )
    outFile = QString ( "%1.avi" ).arg ( dest );
  else if ( codec.contains ( "mpeg", Qt::CaseInsensitive ) )
    outFile = QString ( "%1.mpg" ).arg ( dest );
  else
    outFile = QString ( "%1.avi" ).arg ( dest );

  cfg->setOutputPath ( outFile );
  return outFile;
}

/**
* Sende verschieben Info an Klasse @class RubberBand
*/
void MainWindow::toRubber ( bool )
{
  if ( ! m_RubberBand )
    return;

  QRect r = m_grabberInfo->getRect();
  m_RubberBand->resize ( r.width(), r.height() );
  m_RubberBand->move ( r.x(), r.y() );
  preparePreview();
}

/**
* Fenster Dimensionen abgreifen
*/
void MainWindow::grabFromWindow()
{
  if ( ! m_RubberBand || ! m_desktopTapping )
    return;

  m_desktopTapping->createRequest();
}

/**
* System Symbolleisten hide(), show() Aktionen verarbeiten
*/
void MainWindow::systemTrayWatcher ( QSystemTrayIcon::ActivationReason type )
{
  switch ( type )
  {
    case QSystemTrayIcon::Trigger:
    {
      if ( isVisible() )
        hide();
      else
        show();
    }
    break;

    default:
      return;
  }
}

/**
* Informationen an die Statusleiste senden.
*/
void MainWindow::pushInfoMessage ( const QString &txt )
{
  if ( m_systemTray )
    m_systemTray->sendMessage ( trUtf8 ( "Info" ), txt, QSystemTrayIcon::Information );

  if ( ! m_FFProcess->isRunning() )
    m_systemTray->setIcon ( cfg->themeIcon ( "qx11grab" ) );
}

/**
* Fehler Meldungen an die Statusleiste senden.
*/
void MainWindow::pushErrorMessage ( const QString &title, const QString &txt )
{
  if ( m_systemTray )
  {
    m_systemTray->sendMessage ( title, txt, QSystemTrayIcon::Critical );

    if ( ! m_FFProcess->isRunning() )
      m_systemTray->setIcon ( cfg->themeIcon ( "qx11grab" ) );
  }
}

/**
* Tips an die Statusleiste senden.
*/
void MainWindow::pushToolTip ( const QString &txt )
{
  if ( m_systemTray )
    m_systemTray->setToolTip ( txt );
}

/**
* Die Daten wurden von @class CommandLineEdit modifiziert
* und müssen neu geschrieben werden.
* Das hat nur einen Einfluss auf die gespeicherte Kommandozeile
* und wird bei einem @b preparePreview wieder Überschrieben!
**/
void MainWindow::updateCommandLine ( const QStringList &cmd )
{
  if ( cmd.contains ( cfg->binaryPath() ) )
  {
    cfg->setCommandLine ( cmd );
    setWindowModified ( false );
  }
}

/**
* Starte die Aufnahme und Sperre gleichzeitig
* einige Aktionen um doppel Klicks zu vermeiden.
*/
void MainWindow::startRecord()
{
  if ( ! m_RubberBand->isScalability() )
    return;

  /* Wenn der Benutzer Änderungen nicht gespeichert hat, wird im das hier mitgeteilt.
  * Ist in der Konfiguration die Option "unsavedChanges" nicht aktiviert ist.
  * Wird an dieser Stelle abgebrochen.
  * Andernfalls startet die Aufnahme nur mit einem Warnhinweis */
  if ( isWindowModified() )
  {
    bool force = cfg->value ( QLatin1String ( "unsavedChanges" ), false ).toBool();
    m_systemTray->sendMessage ( ( ( force ) ? trUtf8 ( "Warning" ) : trUtf8 ( "Stopping" ) ),
                                trUtf8 ( "unsaved changes" ),
                                QSystemTrayIcon::Information );

    if ( ! force )
      return;
  }

  if ( m_grabberInfo->getRect().isValid() )
  {
    // Nehme die Editierte Zeile des Benutzers
    QStringList cmd = m_commandPreview->currentCommandLine();
    // Output File
    QString outFile = generateOutputFile();

    if ( cmd.size() < 1 )
    {
      statusBarMessage ( trUtf8 ( "Missing Input" ) );
      return;
    }

    showRubber ( false );
    if ( m_FFProcess->start ( cmd, outFile ) )
    {
      m_systemTray->setActionsEnabled ( true );
      m_menuBar->setActionsEnabled ( true );
      m_systemTray->setIcon ( cfg->themeIcon ( "media-record" ) );
      m_toolBar->setPlayerEnabled ( false );
    }
  }
  else
    m_systemTray->sendMessage ( trUtf8 ( "Error" ), trUtf8 ( "qx11grap not started" ), QSystemTrayIcon::Critical );

  m_toolBar->setActionsEnabled ( true );
}

/**
* Beim beenden einer Aufnahme alles in die Neutrale Stellung bringen.
*/
void MainWindow::setActionsBack()
{
  m_systemTray->setActionsEnabled ( false );
  m_menuBar->setActionsEnabled ( false );
  m_systemTray->setIcon ( cfg->themeIcon ( "qx11grab" ) );
  m_toolBar->setPlayerEnabled ( true );
}

/**
* Lade beim Start des Dialoges alle Einstellungen.
*/
void MainWindow::loadSettings()
{
  m_grabberInfo->load ( cfg );
  m_metaData->load ( cfg );
  m_videoEditor->load ( cfg );
  m_audioEditor->load ( cfg );
  m_audioGroupBox->setChecked ( cfg->value ( "SoundRecording", false ).toBool() );
  preparePreview();
  setWindowModified ( false );

  if ( cfg->value ( "startMinimized", false ).toBool() )
    hide();
  else
    show();
}

/**
* Speichere alle Einstellungen.
*/
void MainWindow::saveSettings()
{
  cfg->setValue ( "SoundRecording", m_audioGroupBox->isChecked() );
  m_grabberInfo->save ( cfg );
  m_metaData->save ( cfg );
  m_videoEditor->save ( cfg );
  m_audioEditor->save ( cfg );
  setWindowModified ( false );
}

/**
* Log Dialog öffnen
*/
void MainWindow::openLogFileDialog()
{
  QFileInfo log ( cfg->logfile() );
  if ( log.isReadable() )
  {
    LogViewer dialog ( log, centralWidget() );
    dialog.exec();
  }
}

/**
* Kommando Zeile für Textausgabe Aufbereiten.
*/
void MainWindow::preparePreview ()
{
  QStringList commandLine;
  commandLine << cfg->binaryPath();
  commandLine << "-xerror";
  commandLine << "-loglevel" << cfg->logLevel();
  commandLine << "-f" << "x11grab";
  commandLine << "-framerate" << QString::number ( m_grabberInfo->frameRate() );

  /* WARNING ordered SIZE before POINT is implicit required
  * or else FFmpeg didnt correct scale the stream */
  commandLine << "-video_size" <<  m_grabberInfo->getGeometry();
  commandLine << "-i" << m_grabberInfo->getX11GrabIdent();

  // Decoder
  commandLine << "-dcodec" << "copy";

  // Audio System
  if ( m_audioGroupBox->isChecked() )
    commandLine << cfg->getAudioDeviceCommand();

  quint32 pthreads = cfg->value ( QLatin1String ( "Threads" ), 0 ).toUInt();
  if ( pthreads > 0 )
    commandLine << "-threads" << QString::number ( pthreads );

  // Experts
  if ( cfg->expertMode() )
  {
    QStringList expert = cfg->getExpertCommand();
    if ( expert.size() > 0 )
      commandLine << expert;
  }

  // Video Options
  commandLine << m_videoEditor->getCmd();

  // Meta Daten
  if ( m_metaData->isChecked() )
    commandLine << m_metaData->getCmd ( videoCodec() );

  // Audio Aufnahme
  if ( m_audioGroupBox->isChecked() )
    commandLine << m_audioEditor->getCmd();

  m_commandPreview->setCommandLine ( commandLine );

  cfg->setValue ( QLatin1String ( "SoundRecording" ), m_audioGroupBox->isChecked() );
  cfg->setCommandLine ( commandLine );

  setWindowModified ( true );
}

/** Signal Überladung */
void MainWindow::preparePreviewRequest ( bool b )
{
  Q_UNUSED ( b );
  preparePreview();
}

/**
* Aktuelle Kommando Zeile in Shell Script exportieren!
*/
void MainWindow::exportCommand()
{
  // Nehme die Editierte Zeile des Benutzers
  QStringList cmd = m_commandPreview->currentCommandLine();
  cmd << "-y" << outputFile();

  ExportDialog* d = new ExportDialog ( cmd.join ( " " ), this );
  if ( d->exec() == QFileDialog::Accepted )
    statusBarMessage ( trUtf8 ( "commandline exported" ) );

  delete d;
}

/**
* Leszeichen Editor erstellen
*/
void MainWindow::openCreateBookmark()
{
  BookmarkDialog* d = new BookmarkDialog ( this );
  d->setVCodecOptions ( m_videoEditor->selectedCodec(), m_videoEditor->getTableItems() );
  d->setACodecOptions ( m_audioEditor->selectedCodec(), m_audioEditor->getTableItems() );
  d->exec();
  delete d;
}

/**
* Leszeichen Editor entfernen
*/
void MainWindow::openRemoveBookmark()
{
  Bookmark doc;
  if ( doc.open() )
  {
    bool ok;
    QStringList selection ( trUtf8 ( "" ) );
    selection << doc.entries();
    QString br = QInputDialog::getItem ( this, trUtf8 ( "Remove Bookmark" ), trUtf8 ( "Bookmark" ), selection, 0, false, &ok );
    if ( ! br.isEmpty() &&  doc.removeEntryById ( br ) )
    {
      QDBusInterface iface ( "de.hjcms.qx11grab", "/BookmarkSelect", "de.hjcms.qx11grab.BookmarkSelecter" );
      iface.call ( "reload" );
    }
  }
}

/**
* Leszeichen Öffnen siehe Toolbar
*/
void MainWindow::openBookmark ( const QString &id )
{
  Bookmark doc;
  if ( doc.open() )
  {
    BookmarkEntry entry = doc.entry ( id );
    if ( ! entry.isValid() )
    {
      statusBarMessage ( trUtf8 ( "Open Bookmark %1 failed" ).arg ( id ), 5000 );
      return;
    }

    // Video
    m_videoEditor->setCodecByName ( entry.getCodecName ( BookmarkEntry::VCODEC ) );
    m_videoEditor->setCodecOptions ( entry.getCodecOptions ( BookmarkEntry::VCODEC ) );
    // Audio
    m_audioEditor->setCodecByName ( entry.getCodecName ( BookmarkEntry::ACODEC ) );
    m_audioEditor->setCodecOptions ( entry.getCodecOptions ( BookmarkEntry::ACODEC ) );

    statusBarMessage ( trUtf8 ( "Open Bookmark %1" ).arg ( id ), 5000 );
  }
}

/**
* Configurations Dialog Öffnen
*/
void MainWindow::openConfiguration()
{
  ConfigDialog* d = new ConfigDialog ( cfg, this );
  if ( d->exec() == QDialog::Accepted )
    preparePreview();

  delete d;
}

void MainWindow::openPresetEditor()
{
  PresetEditor* d = new PresetEditor ( this );
  if ( d->exec() == QDialog::Accepted )
    preparePreview();

  delete d;
}

/**
* Sauber beenden!
* @note Diese Methode wird von den Menüs aufgerufen!
*/
void MainWindow::shutdown()
{
  if ( m_FFProcess )
  {
    stop();
    delete m_FFProcess;
  }

  qDebug ( "qx11grab quit" );
  qApp->quit();
}

/**
* Beim Minimieren die Fenster Geometrie speichern
*/
void MainWindow::hideEvent ( QHideEvent * ev )
{
  saveStats();
  QMainWindow::hideEvent ( ev );
}

/**
* Das beenden über den WindowManager CloseButton
* verhindern! Statt dessen die Einstellungen mit
* \ref hideEvent Speichern und Hauptfenster in das
* System Symbol minimieren!
*/
void MainWindow::closeEvent ( QCloseEvent * ev )
{
  if ( ev->type() == QEvent::Close )
  {
    ev->ignore();
    hide();
    return;
  }
  QMainWindow::closeEvent ( ev );
}

/**
* Schreibe Nachricht in das Meldungs Label
*/
void MainWindow::statusBarMessage ( const QString &msg, int timeout )
{
  statusBar()->showMessage ( msg, timeout );
}

/**
* Liest die aktuelle Kommandozeile aus der Konfiguration
*/
const QString MainWindow::currentCommandLine()
{
  QStringList cmd = cfg->getCommandline();
  return cmd.join ( " " );
}

/**
* Gibt den Aktuellen Aufnahme Bereich zurück
*/
const QString MainWindow::recordingArea()
{
  QString buffer;
  QRect r =  m_grabberInfo->getRect();
  return buffer.sprintf ( "@Rect(%d %d %d %d)", r.x(), r.y(), r.width(), r.height() );
}

/**
* NOTE Wird von Table Editor benötigt!
* Sucht nach der Aktuell ausgewaählten Editor Tabelle und gibt
* den Codec zurück. Ist keine der Codec Tabellen (avcodec/vcodec)
* ausgewählt wird eine leere Zeichenkette zurück geben!
* \see Adaptor::editorcodec()
*/
const QString MainWindow::selectedCodecEditor()
{
  QString codec;
  QString table = m_toolBox->currentWidget()->objectName();
  if ( table.compare ( "vcodec" ) == 0 )
    codec = m_videoEditor->selectedCodec();
  else if ( table.compare ( "acodec" ) == 0 )
    codec = m_audioEditor->selectedCodec();
  else
    codec = QString::null;

  return codec;
}

/**
* Aktuell verwendeter Audio Codec
* Wird für DBus Adaptor und ItemDelegation benötigt!
*/
const QString MainWindow::audioCodec()
{
  return m_audioEditor->selectedCodec();
}

/**
* Aktuell verwendeter Video Codec
* Wird für DBus Adaptor und ItemDelegation benötigt!
*/
const QString MainWindow::videoCodec()
{
  return m_videoEditor->selectedCodec();
}

/**
* Aufnahme ausgabe Dateipfad
* Wird für DBus Adaptor und CommandLine benötigt!
*/
const QString MainWindow::outputFile()
{
  return cfg->absoluteOutputPath();
}

/**
* Teile der System Statusleiste mit,
* das sie DBus Notification verwenden soll!
*/
void MainWindow::registerMessanger ( QDBusConnection* bus )
{
  if ( ! bus || ! m_systemTray )
    return;

  m_systemTray->setMessanger ( bus );
}

MainWindow::~MainWindow()
{}
