.IX Title "QX11GRAB 1"
.TH QX11GRAB 1 "2011-06-30" "@QX11GRAB_VERSION_MAJOR@" "qx11grab"
.SH NAME
QX11Grab \- Ist eine X11 Desktop Aufnahme Software
.SH SYNOPSIS
.B qx11grab
.SH DESCRIPTION
Ist ein Systemleisten Programm das mit FFmpeg den X11 Desktop auf nimmt.

QX11Grab greift die Fenster Dimension ab und erstellt aus diesen
Informationen und Zusaetzlichen Konfigurations- Optionen eine Kommandozeile fuer FFmpeg.

Die so generierten Parameter werden an einen neuen FFmpeg Prozess uebergeben.

Bitte stellen Sie sicher das Ihre FFmpeg Installation die Funktion "x11grab" Unterstuetzt.

.SH EXAMPLE
.nf
ffmpeg -f x11grab -s svga -r 24 -b 6500 -bt 256k -sameq -i :0.0+1,33 /tmp/out.avi
.fi
.SH AUTHOR
Juergen Heinemann (Undefined) <qx11grab(AT)hjcms(DOT)de>.
.SH HOMEPAGE
http://qx11grab.hjcms.de
.PP
