/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
* Copyright: See COPYING file that comes with this distribution
**/

#include "screencombobox.h"
#include "desktopinfo.h"

#include <QtCore/QVariant>

ScreenComboBox::ScreenComboBox ( QWidget *parent )
    : QComboBox ( parent )
{
  setObjectName( "screenComboBox" );
  setItems();

  connect ( this, SIGNAL ( currentIndexChanged ( int ) ),
            this, SLOT ( setDataChanged ( int ) ) );
}

void ScreenComboBox::setItems()
{
  m_DesktopInfo = new DesktopInfo ( this );
  int index = 0;
  clear();

  foreach ( FrameMode mode, m_DesktopInfo->modes( this ) )
  {
    insertItem ( index, mode.summary );
    setItemData ( index, mode.name, Qt::UserRole );
    index++;
  }
}

void ScreenComboBox::setDataChanged ( int index )
{
  QString data = itemData ( index, Qt::UserRole ).toString();
  if ( data.isEmpty() )
    return;

  FrameMode mode = m_DesktopInfo->getFrameMode ( data, this );
  if ( mode.width < 100 )
    return;

  if ( mode.height < 90 )
    return;

  emit screenNameChanged ( data );
  emit screenWidthChanged ( mode.width );
  emit screenHeightChanged ( mode.height );
  emit screenDepthChanged ( mode.depth );
}

ScreenComboBox::~ScreenComboBox()
{
}
