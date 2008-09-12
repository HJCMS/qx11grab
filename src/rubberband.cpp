/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2008
* Copyright: See COPYING file that comes with this distribution
**/

#include "rubberband.h"

RubberBand::RubberBand ( QWidget *parent )
    : QRubberBand ( QRubberBand::Rectangle, parent )
{
  /* set Minimum Size */
  resize ( 100, 100 );
  move ( 0, 30 );
}

RubberBand::~RubberBand()
{
}


