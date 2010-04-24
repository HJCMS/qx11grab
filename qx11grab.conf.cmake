[General]
Version=@QX11GRAB_VERSION@
showRubberband=true
startMinimized=false
tempdir=/tmp
outputName=@CMAKE_PROJECT_NAME@-XXXXXX.avi
SoundRecording=false
audio_codec=pcm_s16le
audio_engine=alsa
audio_intensifier=280
video_codec=rawvideo
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
