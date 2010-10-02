#!/bin/sh
#################################################################
# Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
#
# Copyright: See COPYING file that comes with this distribution
#################################################################

debug_build_target=$HOME/hjcms/qx11grab/build

enable_kde4_support=0

MY_CFLAGS="-O3 -mtune=athlon-xp -march=i686 -pedantic"

function runcmake() {
  if test $enable_kde4_support -eq 1 ; then
    cmake \
      -DCMAKE_CXX_FLAGS:STRING="$MY_CFLAGS" \
      -DCMAKE_C_FLAGS:STRING="$MY_CFLAGS" \
      -DCMAKE_INSTALL_PREFIX:PATH=/usr \
      -DCMAKE_BUILD_TYPE:STRING=Debug \
      -DQTIDY_ENABLE_FPIE:BOOL=ON \
      -DCMAKE_SKIP_RPATH:BOOL=ON \
      -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
      -DAUTOMOC4_EXECUTABLE:FILEPATH=$(which automoc4) \
      -DAutomoc4_DIR:PATH=$(dirname /usr/*/cmake/automoc4/Automoc4Config.cmake) \
      ../
  else
    cmake \
      -DCMAKE_CXX_FLAGS:STRING="$MY_CFLAGS" \
      -DCMAKE_C_FLAGS:STRING="$MY_CFLAGS" \
      -DCMAKE_INSTALL_PREFIX:PATH=/usr \
      -DCMAKE_BUILD_TYPE:STRING=Debug \
      -DQTIDY_ENABLE_FPIE:BOOL=ON \
      -DCMAKE_SKIP_RPATH:BOOL=ON \
      -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
      -DDISABLE_KDE_SUPPORT:BOOL=ON \
      ../
  fi
}

echo "cmake"

if test "$PWD" = "${debug_build_target}" ; then
  if test -f cmake_install.cmake ; then
    rm -rf ${debug_build_target}/*
  fi
  runcmake
else
  mkdir -p build
  pushd build
    runcmake
  popd
fi
