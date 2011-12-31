.IX Title "QX11GRAB 1"
.TH QX11GRAB 1 "2011-06-30" "@QX11GRAB_VERSION_MAJOR@" "qx11grab"
.SH NAME
QX11Grab \- a X11 Desktop Recording Software
.SH SYNOPSIS
.B qx11grab
.SH DESCRIPTION
a systray application that uses FFmpeg for recording X11 desktops.
qx11grab grab window dimensions from desktop and prepare the command line for ffmpeg with additional configuration options.

qx11grab sends the generated parameter list to a new FFmpeg process.

Do not forget to test if Your FFmpeg installtion "x11grab" supported.

.SH EXAMPLE
.nf
ffmpeg -f x11grab -s svga -r 24 -b 6500 -bt 256k -sameq -i :0.0+1,33 /tmp/out.avi
.fi
.SH AUTHOR
Juergen Heinemann (Undefined) <qx11grab(AT)hjcms(DOT)de>.
.SH HOMEPAGE
http://qx11grab.hjcms.de
.PP
