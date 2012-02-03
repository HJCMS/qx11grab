[General]
Version=@QX11GRAB_VERSION@
Autostart=true
showRubberband=false
startMinimized=false
Metadata=false
SoundRecording=false
video_codec=theora
audio_codec=libvorbis
IconTheme=oxygen

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
1\argument=-r
1\value=25
size=7
2\argument=-pix_fmt
2\value=yuv420p
3\argument=-tune
3\value=animation
4\argument=-bufsize
4\value=5097k
5\argument=-vpre
5\value=QX11Grab
6\argument=-vf
6\value="unsharp=3:3:0.2:3:3:0.0"
7\argument=-preset
7\value=medium

[AudioOptions]
1\argument=-ar
1\value=44100
2\argument=-ab
2\value=128k
3\argument=-ac
3\value=1
size=3
