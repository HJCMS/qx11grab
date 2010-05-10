#!/bin/bash

base=$HOME/hjcms/qx11grab

test -x ${base}/build/app/qx11grab || exit 1

unset XDG_DATA_HOME
unset XDG_DATA_DIRS

export QDBUS_DEBUG=1
export QT_DEBUG_PLUGINS=0
export PHONON_GST_DEBUG=0
export QT_LAYOUT_DEBUG=1
export QT_CRASH_OUTPUT=${base}/build/app/crash.log

/usr/bin/ddd -x --directory=$PWD ${base}/build/app/qx11grab
