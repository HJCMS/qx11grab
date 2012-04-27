## Specfile for OpenSuSE Build Service

%define packagename  qx11grab-@QX11GRAB_VERSION_MAJOR@.@QX11GRAB_VERSION_MINOR@.@QX11GRAB_VERSION_RELEASE@
####
%define ff_suffix    %{nil}
%define enable_kde4  0


Name:           qx11grab
Summary:        a high flexible screencast Application for X11 Desktop
Version:        @QX11GRAB_VERSION_MAJOR@.@QX11GRAB_VERSION_MINOR@.@QX11GRAB_VERSION_RELEASE@
Release:        0
License:        GPLv2+
AutoReqProv:    on
Source0:        %{_sourcedir}/%packagename.tar.bz2
Group:          Productivity/Multimedia/Other
Url:            http://qx11grab.hjcms.de
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
Requires:       ffmpeg
BuildRequires:  cmake >= 2.8.0 ffmpeg%{ff_suffix}-devel
%if %{defined suse_version}
BuildRequires:  libqt4-devel >= 4.6.0 update-desktop-files
%else
BuildRequires:  qt4-devel >= 4.6.0
%endif 
%if %enable_kde4
BuildRequires:  automoc4
%endif
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

%package ffpresets
Summary:        QX11Grab ffpreset Files for FFmpeg
Group:          Productivity/Editors/Other
BuildArch:      noarch
Supplements:    packageand(ffpresets:ffmpeg)

%description ffpresets
ffpreset Files for FFmpeg

Author:
-------------
  Juergen Heinemann (Undefined)
  http://www.hjcms.de

%prep
%setup -q

%__mkdir_p build

%build

cd build
cmake -Wno-dev \
  -DCMAKE_BUILD_TYPE:STRING=MinSizeRel \
  -DCMAKE_CXX_FLAGS_RELWITHDEBINFO:STRING="$RPM_OPT_FLAGS" \
  -DCMAKE_INSTALL_PREFIX:PATH=/usr \
  -DENABLE_DRAWTEXT_FILTER:BOOL=ON \
  -DFFMPEG_SUFFIX:STRING="%{ff_suffix}" \
%if %{_lib} == lib64
  -DLIB_SUFFIX:STRING=64 \
%endif
%if %enable_kde4
  -DENABLE_OPENGL:BOOL=ON \
  -DENABLE_KDE_SUPPORT:BOOL=ON \
  -DAUTOMOC4_EXECUTABLE:FILEPATH=%(which automoc4) \
%endif
  -DINSTALL_FFPRESETS:BOOL=ON \
  ../

%__make

%install

pushd build
  %makeinstall
popd

%if %{defined suse_version}
%suse_update_desktop_file qx11grab -r Utility DesktopUtility
%endif 

%post
#

%postun
#

%files
%defattr(-, root, root)
%dir %{_sysconfdir}/xdg/hjcms.de
%config %{_sysconfdir}/xdg/hjcms.de/qx11grab.conf
%{_bindir}/qx11grab
%{_prefix}/%{_lib}/qx11grab
%{_datadir}/qt*/translations/qx11grab*.qm
%dir %{_datadir}/qx11grab
%doc %{_datadir}/qx11grab/AUTHORS
%doc %{_datadir}/qx11grab/COPYING
%doc %{_datadir}/qx11grab/ChangeLog
%doc %{_datadir}/qx11grab/NEWS
%doc %{_datadir}/qx11grab/README
%dir %{_datadir}/qx11grab/options
%{_datadir}/qx11grab/options/*.xml
%{_datadir}/qx11grab/options/options.dtd
%{_datadir}/qx11grab/qx11grab_untranslated.ts
%{_datadir}/applications/qx11grab.desktop
%{_datadir}/dbus-1/interfaces/*.qx11grab.xml
%{_datadir}/dbus-1/services/*.qx11grab.service
## Images
%{_datadir}/pixmaps/qx11grab.png
%{_datadir}/icons/hicolor/*/apps/qx11grab.png
%{_datadir}/icons/hicolor/*/actions/qx11grab_position_*.png
%{_datadir}/icons/hicolor/scalable/apps/qx11grab.svgz
## Manpages
%doc %{_mandir}/de/man1/qx11grab.1*
%doc %{_mandir}/man1/qx11grab.1*

%post ffpresets
##

%postun ffpresets
##

%files ffpresets
%defattr(-,root,root,-)
%dir %{_datadir}/ffmpeg
%{_datadir}/ffmpeg/*.ffpreset

%clean
rm -rf %{buildroot}

%changelog
#eof
