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

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <sched.h>
#include <stdint.h>

#include "threadsspinbox.h"

/* QtCore */
#include <QtCore/QDebug>

ThreadsSpinBox::ThreadsSpinBox ( QWidget * parent )
    : QSpinBox ( parent )
{
  setObjectName ( QLatin1String ( "ThreadsSpinBox" ) );
  /*: ToolTip */
  setToolTip ( trUtf8 ( "Thread count" ) );
  /*: WhatsThis */
  setWhatsThis ( trUtf8 ( "Set Multithreading count" ) );
  setMinimum ( 0 );
  QString p_suffix ( " " );
  p_suffix.append ( trUtf8 ( "Threads" ) );
  setSuffix ( p_suffix );
}

/**
* Setze die Maximal zulässige Angabe für Prozessangabe.
* Verhindert das der Benutzer bei der thread Angabe zu viele Kerne angibt!
*/
void ThreadsSpinBox::setMaxThreads()
{
  qint32 ncp = 0;
  cpu_set_t p_CPUset;
  memset ( &p_CPUset, 0, sizeof ( p_CPUset ) );
  if ( sched_getaffinity ( 0, sizeof ( p_CPUset ), &p_CPUset ) )
  {
    setMaximum ( 1 );
    return;
  }

  // die neuere glibc bietet ein macro
#ifdef CPU_COUNT
  setMaximum ( CPU_COUNT ( &p_CPUset ) );
  return;
#endif

  for ( uint bit = 0; bit < ( 8 * sizeof ( p_CPUset ) ); bit++ )
  {
    ncp += ( ( ( reinterpret_cast<uint8_t*> ( &p_CPUset ) ) [bit / 8] >> ( bit % 8 ) ) & 1 );
  }
  setMaximum ( ncp );
}

/**
* Kommandozeile zurück geben!
*/
const QString ThreadsSpinBox::argument()
{
  if ( value() > 1 )
    return QString::fromUtf8 ( "-threads %1" ).arg ( QString::number ( value() ) );
  else
    return QString::null;
}

ThreadsSpinBox::~ThreadsSpinBox()
{}
