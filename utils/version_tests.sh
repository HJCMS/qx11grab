#!/usr/bin/env bash
########################################################################################
# This file is part of the qx11grab project
#
# Copyright (C) Juergen Heinemann (Undefined) http://qx11grab.hjcms.de, (C) 2007-2015
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
#
# Test Script for Building all my Installed FFmpeg/libav Versions with QX11Grab
#
########################################################################################

target_project="qx11grab"
ff_suffix=
smp_flags="-j$(getconf _NPROCESSORS_ONLN)"

notify_message() {
  echo -e '\033[1m' "$1" '\033[0m';
}

if test -f $HJCMS_PROJECTS_DIR/projectconfigurations.sh ; then
source $HJCMS_PROJECTS_DIR/projectconfigurations.sh
fi

mkdir -p /tmp/${target_project}
rm -rf /tmp/${target_project}/*.*

> $HJCMS_PROJECTS_DIR/${target_project}/BUILD_TESTS

find /usr/include/ffmpeg -mindepth 1 -maxdepth 1 -type d -exec basename "{}" \; > /tmp/qx11grab.destdirs
find /usr/include/libav -mindepth 1 -maxdepth 1 -type d -exec basename "{}" \; >> /tmp/qx11grab.destdirs

while read v ; do
  summary=""
  v1="`echo $v | cut -d\. -f2`"
  if [ -e  /usr/lib/libavformat-${v}.so ] || [ -e /usr/lib64/libavformat-${v}.so ] ; then
    ff_suffix="-${v}"
    summary="Building with FFmpeg v${v}"
    notify_message "Running ffmpeg Test vor version ${ff_suffix}"
  elif [ -e  /usr/lib/libavformat_${v1}x.so ] || [ -e /usr/lib64/libavformat-${v1}x.so ] ; then
    ff_suffix="_${v1}x"
    summary="Building with FFmpeg v${v}"
    notify_message "Running ffmpeg Test vor version ${ff_suffix}"
  elif [ -e  /usr/lib/libavformat-av-${v}.so ] || [ -e /usr/lib64/libavformat-av-${v}.so ] ; then
    ff_suffix="-av-${v}"
    summary="Building with libav v${v}"
    notify_message "Running libav Test vor version ${ff_suffix}"
  else
    continue
  fi
  mkdir -p /tmp/${target_project}/${v}
  pushd /tmp/${target_project}/${v}

    /usr/bin/cmake -W-dev \
      -DCMAKE_CXX_FLAGS:STRING="$CXXFLAGS" \
      -DCMAKE_BUILD_TYPE:STRING=Debug \
      -DFFMPEG_SUFFIX:STRING="${ff_suffix}" \
      -DENABLE_EXPERIMENTAL:BOOL=ON \
      -DINSTALL_FFPRESETS:BOOL=OFF \
      -DCMAKE_SKIP_RPATH:BOOL=ON \
      -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
      $CMAKE_EXTRAS $@ $HJCMS_PROJECTS_DIR/${target_project}/

    LC_ALL=C make ${smp_flags} -C /tmp/${target_project}/${v}

  popd

  if test -x /tmp/${target_project}/${v}/app/${target_project} ; then
    notify_message "Building with ${v} and ${ff_suffix} done!"
    echo "${summary} OK" >> $HJCMS_PROJECTS_DIR/${target_project}/BUILD_TESTS
  else
    notify_message "Building with ${v} and ${ff_suffix} failed!"
    echo "${summary} FAILED" >> $HJCMS_PROJECTS_DIR/${target_project}/BUILD_TESTS
    tput bel
    exit 1
  fi

done < /tmp/qx11grab.destdirs

rm -f /tmp/qx11grab.destdirs

cat $HJCMS_PROJECTS_DIR/${target_project}/BUILD_TESTS

##EOF
