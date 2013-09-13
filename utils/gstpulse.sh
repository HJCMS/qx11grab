#!/usr/bin/env bash
########################################################################################
# This file is part of the qx11grab project
#
# Copyright (C) Juergen Heinemann (Undefined) http://qx11grab.hjcms.de, (C) 2007-2013
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

## http://wiki.gentoo.org/wiki/PulseAudio

gst_ver=${1:-"0.10"}

if ! which gconftool-2 &> /dev/null; then
  echo -e "\E[1;37;44m\033[1m FATAL: missing gconftool-2\033[0m"
  exit 1
fi

function setPulseAudio()
{
  echo -e "\E[1;37;44m\033[1m Update Configuration \033[0m"
  gconftool-2 -R /system/gstreamer/${gst_ver}/default

  gconftool-2 -t string --set /system/gstreamer/${gst_ver}/default/audiosink pulsesink
  gconftool-2 -t string --set /system/gstreamer/${gst_ver}/default/audiosrc pulsesrc
  gconftool-2 -t string --set /system/gstreamer/${gst_ver}/default/musicaudiosink pulsesink
  gconftool-2 -t string --set /system/gstreamer/${gst_ver}/default/videosink glimagesink
  gconftool-2 -t string --set /system/gstreamer/${gst_ver}/default/chataudiosink fakesink

  echo -e "\E[1;37;44m\033[1m Show Configuration \033[0m"
  gconftool-2 -R /system/gstreamer/${gst_ver}/default
}

setPulseAudio

echo "gst-launch-${gst_ver} playbin uri=file:///usr/share/sounds/test/test.wav"
