#!/bin/sh

PRJ_HOME=/srv/projects/qx11grab

includes="/usr/include/libavcodec \
/usr/include/libavdevice \
/usr/include/libavfilter \
/usr/include/libavformat \
/usr/include/libavutil \
/usr/include/libpostproc \
/usr/include/libswresample \
/usr/include/qt4 \
/usr/include/pulse"

test -d ${PRJ_HOME}/src || exit 1

/usr/bin/ctags -R --c++-types=+px --exclude=CMakeLists.txt -o ${PRJ_HOME}/.ctags src/ ${includes}

exit $?
