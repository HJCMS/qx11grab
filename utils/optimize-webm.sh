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

in="${1:-"dummy"}"

## https://support.google.com/youtube/answer/1722171?ref_topic=2888648
test -e ${in}.webm || {
  echo "FATAL: ${in}.webm not found"
  exit 1
}

echo "Optimized for YouTube Stereo 1080p"
/usr/bin/ffmpeg -i ${in}.webm -threads 4 \
  -c:v libvpx -r:v 30 -s 1920x1080 -pix_fmt yuv420p -speed 2 -vpre 1080p \
  -c:a libvorbis -ar 44100 -b:a 512k -q:a 6 \
  -metadata copyright='CC BY-NC-SA 3.0' \
  -y /tmp/${in}_data.webm

# EOF
