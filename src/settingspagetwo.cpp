/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2008
* Copyright: See COPYING file that comes with this distribution
**/

#include "settingspagetwo.h"
#include "version.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QHeaderView>

SettingsPageTwo::SettingsPageTwo ( QWidget *parent )
    : QWidget ( parent )
{
  setObjectName ( "SettingsPageTwo" );
  setContentsMargins ( 2, 5, 2, 2 );
  setMinimumWidth ( 350 );

  QVBoxLayout *topLayout = new QVBoxLayout ( this );
  topLayout->addWidget ( new QLabel ( trUtf8 ( "FFMpeg Recording Settings" ), this ) );

  ff_tableWidget = new QTableWidget ( this );
  ff_tableWidget->setColumnCount ( 2 );
  QStringList hText ( trUtf8 ( "Option" ) );
  hText << trUtf8 ( "Value" );
  ff_tableWidget->setHorizontalHeaderLabels ( hText );
  topLayout->addWidget ( ff_tableWidget );

  ff_cmd = new QLineEdit ( this );
  ff_cmd->setReadOnly ( true );
  topLayout->addWidget ( ff_cmd );


  QString txt = QString ( HTML_NOTICE ).arg ( trUtf8 ( "Notice" ) ,trUtf8 ( "Do not add the -i,-f,-s and output file options. This Parameters will automatically inserted by qx11grab." ) );
  QLabel *labelNotice = new QLabel ( txt, this );
  labelNotice->setWordWrap ( true );
  topLayout->addWidget ( labelNotice );

  setLayout ( topLayout );

  connect ( ff_tableWidget, SIGNAL ( itemChanged ( QTableWidgetItem * ) ),
            this, SLOT ( prepareProcessLine() ) );
}

void SettingsPageTwo::setTableDefaults()
{
  ff_tableWidget->setRowCount ( 4 );
  QStringList params, values;
  params << "-b" << "-bt" << "-r" << "-sameq";
  values << "6500" << "256k" << "25" << "";
  int row = 0;
  foreach ( QString p, params )
  {
    ff_tableWidget->setItem ( row, 0, new  QTableWidgetItem ( p ) );
    if ( row != 4 )
      ff_tableWidget->setItem ( row, 1, new  QTableWidgetItem ( values.at ( row ) ) );

    row++;
  }
  resizeTableContents();
  prepareProcessLine();
}

const QMap<QString,QVariant> SettingsPageTwo::getOptions()
{
  QMap<QString,QVariant> map;
  int r;
  int rows = ff_tableWidget->rowCount();
  for ( r = 0; r < rows; r++ )
  {
    QString param ( ff_tableWidget->item ( r, 0 )->text() );
    QString value = QString::null;
    if ( ff_tableWidget->item ( r, 1 ) )
      value = ff_tableWidget->item ( r, 1 )->text();

    map.insert ( param, value );
  }
  return map;
}

void SettingsPageTwo::pushItemRow ( int row, const QString &p, const QVariant &v )
{
  ff_tableWidget->setItem ( row, 0, new  QTableWidgetItem ( p ) );
  if ( v.isValid() && ! v.toString().isEmpty() )
    ff_tableWidget->setItem ( row, 1, new  QTableWidgetItem ( v.toString() ) );
}

void SettingsPageTwo::setOptions ( const QMap<QString,QVariant> &map )
{
  if ( map.size() < 1 )
  {
    setTableDefaults();
    return;
  }

  ff_tableWidget->setColumnCount ( 2 );
  ff_tableWidget->setRowCount ( map.size() );

  QMapIterator<QString,QVariant> it ( map );
  int row = 0;
  while ( it.hasNext() )
  {
    it.next();
    pushItemRow ( row, it.key(), it.value() );
    row++;
  }
  resizeTableContents();
  prepareProcessLine();
}

void SettingsPageTwo::prepareProcessLine()
{
  int r;
  int rows = ff_tableWidget->rowCount();
  QStringList list;
  for ( r = 0; r < rows; r++ )
  {
    /* NOTE very strange if reloading table all rows a empty ! */
    if ( ! ff_tableWidget->item ( r, 0 ) )
      continue;

    QString val ( ff_tableWidget->item ( r, 0 )->text() );
    if ( ff_tableWidget->item ( r, 1 ) )
    {
      val.append ( " " );
      val.append ( ff_tableWidget->item ( r, 1 )->text() );
    }
    list << val;
    val.clear();
  }
  if ( list.size() > 2 )
    ff_cmd->setText ( list.join ( " " ) );
}

void SettingsPageTwo::resizeTableContents ()
{
  QHeaderView *qhv = ff_tableWidget->horizontalHeader();
  qhv->setResizeMode ( 0, QHeaderView::ResizeToContents );
  qhv->setResizeMode ( 1, QHeaderView::Stretch );
}

void SettingsPageTwo::contextMenuEvent ( QContextMenuEvent *ev )
{
  QMenu *m_QMenu = new QMenu ( "Actions", this );
  // Eintrag hinzufügen
  QAction *m_AddItem = m_QMenu->addAction ( trUtf8 ( "New" ) );
  // Eintrag entfernen
  QAction *m_RemoveItem = m_QMenu->addAction ( trUtf8 ( "Remove" ) );

  // Signale
  connect ( m_AddItem, SIGNAL ( triggered() ), this, SLOT ( addItemRow() ) );
  connect ( m_RemoveItem, SIGNAL ( triggered() ), this, SLOT ( removeItemRow() ) );
  m_QMenu->exec ( ev->globalPos() );
  delete m_QMenu;
}

void SettingsPageTwo::addItemRow()
{
  int rows = ff_tableWidget->rowCount();
  ff_tableWidget->setRowCount ( ++rows );
}

void SettingsPageTwo::removeItemRow()
{
  if ( ff_tableWidget->currentIndex ().isValid() )
  {
    int row = ff_tableWidget->currentIndex ().row();
    if ( ff_tableWidget->item ( row, 0 ) )
      ff_tableWidget->removeRow ( row );
  }
}

SettingsPageTwo::~SettingsPageTwo()
{
}


