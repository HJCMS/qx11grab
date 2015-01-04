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

## BUG ffmpeg-2.4 DTS -filter:a pan="5.1|c0=c1|c1=c0|c2=c2|c3=c3|c4=c4|c5=c5" \

## https://api.twitch.tv/kraken/ingests
TWITCH_DOMAIN=live-fra
STREAM_KEY=

ffmpeg -xerror -loglevel info \
  -f x11grab -framerate 20 -video_size 1920x1018 -i :0.0+0,31 -dcodec copy \
  -f pulse -i Equalizer.monitor \
  -f pulse -i alsa_input.usb-C-Media_Electronics_Inc._USB_PnP_Sound_Device____-00-Headset.analog-mono \
  -threads 4 -report -cpuflags 3dnowext \
  -c:v libx264 -pix_fmt yuv420p -keyint_min 20 -tune film -preset medium \
  -g:v 40 -b:v 2000k -minrate:v 1750k -maxrate:v 2250k -bufsize 2000k \
  -c:a libfaac -ac:a 2 -b:a 192k -ar:a 48000 \
  -filter:v movie=/usr/share/icons/hicolor/64x64/apps/qx11grab.png[logo],[in][logo]overlay=main_w-overlay_w-12:main_h-overlay_h-8,setpts=PTS-STARTPTS[out] \
  -filter_complex "[1:a]volume=-20dB[a1],[2:a]volume=+1dB[a2],[a1][a2]amix=inputs=2" \
  -metadata author="Undefined Behavior" \
  -metadata copyright="CC BY-NC-SA 3.0" \
  rtmp://$TWITCH_DOMAIN.twitch.tv/app/$STREAM_KEY
