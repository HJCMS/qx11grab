[General]
Version=@QX11GRAB_VERSION@
showRubberband=false
startMinimized=false
Metadata=true
SoundRecording=false
tempdir=/tmp
outputName=@CMAKE_PROJECT_NAME@-XXXXXX.ogg
audio_intensifier=256
audio_engine=alsa
audio_device=default
audio_sample_fmt=s16
video_codec=theora
audio_codec=libvorbis

[metadata]
INAM=@CMAKE_PROJECT_NAME@
ICOP=CC
ISBJ=@CMAKE_PROJECT_NAME@
ICMT=Demo

[VideoOptions]
size=3
1\argument=-r
1\value=25
2\argument=-pix_fmt
2\value=yuv420p
3\argument=-b:v
3\value=14000k

[AudioOptions]
size=3
1\argument=-ar
1\value=44100
2\argument=-ab
2\value=230k
3\argument=-ac
3\value=1

