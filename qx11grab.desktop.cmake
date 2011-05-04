[Desktop Entry]
Type=Application
Name=@CMAKE_PROJECT_NAME@
GenericName=X11 Window Recorder
Comment=Application for recording x11 windows
Comment[de]=Programm zum Aufnehmen von X11 Fenstern
Exec=@CMAKE_PROJECT_NAME@
Icon=@CMAKE_PROJECT_NAME@
Categories=Qt;KDE;X-KDE-Utilities-Desktop;Utility;Applet;
StartupNotify=false
X-KDE-StartupNotify=true
X-DBUS-ServiceName=@QX11GRAB_DBUS_DOMAIN_NAME@
X-DBUS-ServiceType=Unique
X-KDE-autostart-after=plasma
X-KDE-AutoEmbed=false
