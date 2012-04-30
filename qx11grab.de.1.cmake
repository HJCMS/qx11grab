.IX Title "QX11GRAB 1"
.TH QX11GRAB 1 "2011-06-30" "@QX11GRAB_VERSION_MAJOR@" "qx11grab"
.SH Name
QX11Grab \- Ein hoch flexibles Digitales Bildschirmaufzeichner Programm für die X11 Desktop Oberfläche.
.SH Programm Name
.B qx11grab
.SH Beschreibung
Ist ein Systemleisten Programm das mit FFmpeg den X11 Desktop auf nimmt.

QX11Grab greift die Fenster Dimension ab und erstellt aus diesen Informationen und Zusätzlichen Konfigurations- Optionen eine Kommandozeile für \fIffmpeg\fR\|(1).

Die so generierten Parameter werden an einen neuen FFmpeg Prozess übergeben.

.IP "\fBAusstattung\fR"
.nf
\[bu] Unterstützung zum abgreifen der Dimensionen auf Einzel/Multi und Xinerama Bildschirmen,
\[bu] Formular zum einfügen der Kopfdaten,
\[bu] Audio/Video Kodierer Eingabe Tabellen mit Kontext Menüs und vordefinierten Prädikaten,
\[bu] Experten Eingabetabelle für Benutzerdefinierte Argumente,
\[bu] Editierbare Kommandozeilen Vorschau zum besseren Verständnis der FFmpeg Optionen,
\[bu] Konfigurierbare Audioaufnahme Schnittstelle für Alsa/Pulse und OSS,
\[bu] Lesezeichen Unterstützung für ein besseres wechseln in den Kodierer Konfigurationen,
\[bu] Prädikat bestimmungs- Editor für ffpresets,
\[bu] Kommandozeilen export für Shell Scripte.
.fi
.PP
QX11Grab beinhaltet außerdem eine Plugin Schnittstelle für Filter Dialoge.
.IP "\fBFilterliste\fR"
.nf
\[bu] Text Zeichnen Filter Dialog,
\[bu] Un/Schärfen Filter Dialog,
\[bu] Display Aspect Ratio (DAR) Dialog,
\[bu] Wasserzeichen Filter Dialog,
\[bu] Ein/Ausblenden Filter Dialog,
\[bu] Webcam einbetten Filter.
.fi
.PP

.SH Hinweis
.I Bitte stellen Sie sicher das Ihre FFmpeg Installation die Funktion "x11grab" Unterstützt.
.nf
ffmpeg -f x11grab -i :0.0+1,33 -s svga -r 24 -b:v 6500k -bt 256k -sameq -y /tmp/out.avi
.fi
.SH Homepage
http://qx11grab.hjcms.de
.SH Autor
.AU
Jürgen Heinemann (Undefined) <nospam(AT)hjcms(DOT)de>.
.SH Siehe auch
\&\fIffmpeg\fR\|(1) und die FFmpeg HTML Dokumentation http://www.ffmpeg.org/documentation.html
.PP
