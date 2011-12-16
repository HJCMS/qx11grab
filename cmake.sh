#!/bin/sh
#################################################################
# Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2012
#
# Copyright: See COPYING file that comes with this distribution
#################################################################

if test -f $HOME/hjcms/projectconfigurations.sh ; then
source $HOME/hjcms/projectconfigurations.sh
fi

ffsuffix="$1"

cmake -Wdev \
  -DCMAKE_CXX_FLAGS:STRING="${CFLAGS:-"-O2"}" \
  -DCMAKE_INSTALL_PREFIX:PATH=/usr \
  -DCMAKE_BUILD_TYPE:STRING=Debug \
  -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
  -DFFMPEG_SUFFIX:STRING=$ffsuffix \
  $CMAKE_EXTRAS ../
