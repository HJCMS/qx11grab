####################################################################
##
## Author: Juergen Heinemann http://qtidy.hjcms.de, (C) 2007-2008
##
## Copyright: See COPYING file that comes with this distribution
##
####################################################################

DESTDIR = .
TEMPLATE = app
TARGET = qx11grab
CONFIG += qt debug_and_release link_pkgconfig
PKGCONFIG += x11 xmuu
QT += gui
OBJECTS_DIR += build
UI_DIR += build
MOC_DIR += build
RCC_DIR += build

include(../config.pri)

TRANSLATIONS = ../tr/qx11grab_untranslated.ts \
	../tr/qx11grab_de.ts

HEADERS =	qx11grab.h \
	settings.h \
	version.h \
	screencombobox.h \
	desktopinfo.h \
	windowgrabber.h \
	rubberband.h \
	settingsdialog.h \
	settingspageone.h \
	settingspagetwo.h \
	ffprocess.h

SOURCES =	main.cpp \
	qx11grab.cpp \
	settings.cpp \
	screencombobox.cpp \
	desktopinfo.cpp \
	windowgrabber.cpp \
	rubberband.cpp \
	settingsdialog.cpp \
	settingspageone.cpp \
	settingspagetwo.cpp \
	ffprocess.cpp

contains(QT,dbus) {
	HEADERS += qx11grabadaptor.h
	SOURCES += qx11grabadaptor.cpp
}

RESOURCES =	qx11grab.qrc

FORMS =	qx11grabmain.ui \
	settingsdialogform.ui

xdgdata.path = $$[QT_INSTALL_PREFIX]/share/applications
xdgdata.files = qx11grab.desktop

pixmap.path = $$[QT_INSTALL_PREFIX]/share/pixmaps
pixmap.files = images/qx11grab.png

target.path +=	$$[QT_INSTALL_BINS]

INSTALLS +=	target xdgdata pixmap
