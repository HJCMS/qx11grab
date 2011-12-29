[General]
Version=@QX11GRAB_VERSION@
showRubberband=false
startMinimized=false
Metadata=false
SoundRecording=false
video_codec=theora
audio_codec=libvorbis

[Targets]
Binary=@CMAKE_INSTALL_PREFIX@/bin/ffmpeg
TemplateName=@CMAKE_PROJECT_NAME@-XXXXXX
Directory=/tmp

[Audio]
Engine=alsa
Device=default
Intensifier=256
SampleFormat=s16
ServiceType=ma

[VideoOptions]
size=3
1\argument=-r
1\value=25
2\argument=-pix_fmt
2\value=yuv420p
3\argument=-b:v
3\value=800k

[AudioOptions]
size=3
1\argument=-ar
1\value=44100
2\argument=-ab
2\value=128k
3\argument=-ac
3\value=1

