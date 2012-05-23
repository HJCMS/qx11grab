#!/bin/bash
########################################################################################
# This file is part of the qx11grab project
#
# Copyright (C) Juergen Heinemann (Undefined) http://qx11grab.hjcms.de, (C) 2007-2012
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public License
# along with this library; see the file COPYING.LIB.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301, USA.
########################################################################################

target_project="qx11grab"
ff_suffix=_10x

if test -f $HJCMS_PROJECTS_DIR/projectconfigurations.sh ; then
source $HJCMS_PROJECTS_DIR/projectconfigurations.sh
fi

mkdir -p $HJCMS_PROJECTS_DIR/${target_project}/build

/usr/bin/cmake -W-dev \
  -DCMAKE_CXX_FLAGS:STRING="$CXXFLAGS" \
  -DCMAKE_BUILD_TYPE:STRING=Debug \
  -DFFMPEG_SUFFIX:STRING="${ff_suffix}" \
  -DENABLE_EXPERIMENTAL:BOOL=ON \
  -DINSTALL_FFPRESETS:BOOL=OFF \
  -DCMAKE_SKIP_RPATH:BOOL=ON \
  -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
  $CMAKE_EXTRAS $@ ../
