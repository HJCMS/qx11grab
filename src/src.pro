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
CONFIG += qt debug_and_release
QT += gui dbus
##
OBJECTS_DIR += build
UI_DIR += build
MOC_DIR += build
RCC_DIR += build

HEADERS	=	qx11grab.h

SOURCES	=	main.cpp \
	qx11grab.cpp

RESOURCES	=	qx11grab.qrc

FORMS	=	qx11grabmain.ui

target.path	+=	$$[QT_INSTALL_BINS]

INSTALLS	+=	target
