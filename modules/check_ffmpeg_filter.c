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

#include <stdio.h>
#include <stdlib.h>
#include <libavutil/avutil.h>
#include <libavfilter/avfilter.h>
#include <libavfilter/avfiltergraph.h>

int main(int argc, char **argv)
{
  if( ( argc < 1 ) || strlen(argv[1]) < 3 )
  {
    fprintf ( stderr, "Missing Filter arguments\n" );
    return EXIT_FAILURE;
  }

  const char* filterName = argv[1];
  fprintf ( stdout, "Search Filter - %s\n", filterName );

  /*
   * nm -D /usr/lib/libavfilter.so | grep -e avfilter_register_all -e avfilter_get_by_name
   * grep avfilter_register_all /usr/include/libavfilter/\*.h
   */
  avfilter_register_all();
  AVFilter* filter = avfilter_get_by_name(filterName);
  return ( filter == NULL ) ?  EXIT_FAILURE : EXIT_SUCCESS;
}
