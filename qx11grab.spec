## Specfile for OpenSuSE Build Service

%define packagename  qx11grab-0.1.7

%define _qmake       qmake
%define _lrelease    lrelease
%define _qt_bindir   %(%{_qmake} -query QT_INSTALL_BINS)
%define _qt_transdir %(%{_qmake} -query QT_INSTALL_TRANSLATIONS)

Name:           qx11grab
Summary:        qt4 applet for recording x11 windows with ffmpeg
Summary(de):    Qt4 Programm zum Aufnehmen von X11 Fenstern mit ffmpeg
Version:        0.1.7
Release:        0
License:        GPL 2
Autoreqprov:    on
Source0:        %{_sourcedir}/%packagename.tar.bz2
Group:          System/GUI/QT
Url:            http://qx11grab.hjcms.de
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
Requires:       ffmpeg
BuildRequires:  libqt4-devel
Vendor:         Juergen Heinemann (Undefined) http://www.hjcms.de

%description
qt4 applet for recording x11 windows with ffmpeg
this prgramm requires ffmpeg from http://ffmpeg.mplayerhq.hu

before run this application please test with your xterm or console
that ffmpeg installation support this feature:
  ffmpeg -f x11grab -s svga -r 24 -b 6500 -bt 256k -sameq -i :0.0+1,33 /tmp/out.avi

Help: --------------
  http://ffmpeg.mplayerhq.hu/ffmpeg-doc.html#SEC4
  https://lists.mplayerhq.hu/mailman/listinfo

Author: -------------
  Juergen Heinemann (Undefined)
  http://www.hjcms.de

%description -l de
Ein Qt Basierendes Programm zum Aufnehmen von X11 Fenstern.
Das Programm verwendet ffmpeg von http://ffmpeg.mplayerhq.hu

Bevor sie das Programm verwenden bitte testen sie zuerst auf dem terminal oder console
ob ihre ffmpeg Installation diese funktion unterstützt:
  ffmpeg -f x11grab -s svga -r 24 -b 6500 -bt 256k -sameq -i :0.0+1,33 /tmp/out.avi

Hilfe: --------------
  http://ffmpeg.mplayerhq.hu/ffmpeg-doc.html#SEC4
  https://lists.mplayerhq.hu/mailman/listinfo

Autor: -------------
  Juergen Heinemann (Undefined)
  http://www.hjcms.de

%prep
%setup -q
%build

./configure -no-dbus -release -verbose

%__make

%install
%__mkdir_p %{buildroot}

%__make INSTALL_ROOT=%{buildroot} install

%clean
rm -rf %{buildroot}

%files
%defattr(-, root, root)
%{_qt_bindir}/%{name}
%{_qt_transdir}/%{name}*.qm
%{_datadir}/applications/%{name}.desktop

%changelog
#eof
