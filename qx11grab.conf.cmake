[General]
Version=@QX11GRAB_VERSION@
showRubberband=true
startMinimized=false
tempdir=/tmp
outputName=@CMAKE_PROJECT_NAME@-XXXXXX.ogg
SoundRecording=false
audio_engine=alsa
audio_intensifier=256
video_codec=theora
audio_codec=libvorbis
Metadata=false

[metadata]
INAM=@CMAKE_PROJECT_NAME@
ICOP=CC

[VideoOptions]
1\argument=-r
1\value=25
2\argument=-b
2\value=9800k
size=2

[AudioOptions]
1\argument=-ar
1\value=44100
2\argument=-ac
2\value=1
size=2
