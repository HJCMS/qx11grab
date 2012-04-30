.IX Title "QX11GRAB 1"
.TH QX11GRAB 1 "2011-06-30" "@QX11GRAB_VERSION_MAJOR@" "qx11grab"
.SH NAME
QX11Grab \- a X11 Desktop Recording Software
.SH SYNOPSIS
.B qx11grab
.SH DESCRIPTION
a systray application that uses FFmpeg for recording X11 desktops.
qx11grab grab window dimensions from desktop and prepare the command line for FFmpeg with additional configuration options.

qx11grab sends the generated parameter list to a new \fIffmpeg\fR\|(1) process.

.IP "\fBFeatures\fR"
.nf
\[bu] Support for tapping Dimensions on Single/Multi and Xinerama Desktops,
\[bu] Meta data input Formula,
\[bu] Audio/Video Codec Table Editors with Context Menus and predefined Arguments,
\[bu] Custom command Table editor for experts,
\[bu] Command line Preview for better understanding FFmpeg option sets,
\[bu] Configurable Audio capture Interface for Alsa/Pulse and OSS Devices,
\[bu] Bookmark support for better switch Configuration to other codec sets,
\[bu] Presets Editor for ffpresets,
\[bu] Command line Shell script export.
.fi
.PP
QX11Grab also implied a plugin interface for FFmpeg codec filters.
.IP "\fBFilters\fR"
.nf
\[bu] Drawing text filter dialog,
\[bu] Un/sharpen filter dialog,
\[bu] Display Aspect Ratio (DAR) dialog,
\[bu] Watermark filter dialog,
\[bu] Fade (In/Out) filter dialog.
.fi
.PP

.SH NOTE
Do not forget to test if Your FFmpeg installtion "x11grab" supported.
.nf
ffmpeg -f x11grab -i :0.0+1,33 -s svga -r 24 -b:v 6500k -bt 256k -sameq -y /tmp/out.avi
.fi
.SH HOMEPAGE
http://qx11grab.hjcms.de
.SH AUTHOR
Juergen Heinemann (Undefined) <nospam(AT)hjcms(DOT)de>.
.SH SEE ALSO
\&\fIffmpeg\fR\|(1) and the FFmpeg HTML documentation http://www.ffmpeg.org/documentation.html
.PP
