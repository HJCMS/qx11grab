
bin_PROGRAMS = qx11grab
AM_CXXFLAGS ?= -g -O2 -pipe
qx11grab_LDFLAGS = @QT_LIBS@ $(FFMPEG_LIBS)
qx11grab_CXXFLAGS = $(AM_CXXFLAGS) @QT_CFLAGS@
qx11grab_CXXFLAGS += -I@top_srcdir@ -I@top_builddir@
qx11grab_CXXFLAGS += -Wall -W -Wextra -pedantic -Wstack-protector -Wno-long-long -Wno-overlength-strings

UIC = @UIC@
RCC = @RCC@
MOC = @MOC@
MOC_INCDIR = @srcdir@
ADAPTOR = @QDBUSXML2CPP@

EXTRA_DIST = \
	qx11grab.qrc \
	qx11grabwin.ui \
	settingsdialogform.ui

noinst_HEADERS =	\
	qx11grab.h \
	settings.h \
	version.h \
	screencombobox.h \
	desktopinfo.h \
	windowgrabber.h \
	rubberband.h \
	settingsdialog.h \
	settingspageone.h \
	settingspagetwo.h \
	ffprocess.h \
	qx11grabadaptor.h

BUILT_SOURCES = \
	qrc_qx11grab.cpp \
	moc_desktopinfo.cpp \
	moc_ffprocess.cpp \
	moc_qx11grabadaptor.cpp \
	moc_qx11grab.cpp \
	moc_rubberband.cpp \
	moc_screencombobox.cpp \
	moc_settingsdialog.cpp \
	moc_settings.cpp \
	moc_settingspageone.cpp \
	moc_settingspagetwo.cpp \
	moc_windowgrabber.cpp \
	ui_qx11grabmain.h \
	ui_settingsdialogform.h

nodist_qx11grab_SOURCES = $(BUILT_SOURCES)

qx11grab_SOURCES = \
	main.cpp \
	qx11grab.cpp \
	settings.cpp \
	screencombobox.cpp \
	desktopinfo.cpp \
	windowgrabber.cpp \
	rubberband.cpp \
	settingsdialog.cpp \
	settingspageone.cpp \
	settingspagetwo.cpp \
	ffprocess.cpp \
	qx11grabadaptor.cpp

moc_%.cpp: %.h
	$(MOC) -I${MOC_INCDIR} -o $@ $<

ui_%.h: %.ui
	$(UIC) -o $@ $<

qrc_%.cpp: %.qrc
	$(RCC) -name $* -o $@ $<

lupdate:
	@LUPDATE@ -noobsolete -extensions ui,cpp,h ${srdir} ${buildir} -ts @top_srcdir@/tr/qx11grab_de.ts
	@LUPDATE@ -noobsolete -extensions ui,cpp,h ${srdir} ${buildir} -ts @top_srcdir@/tr/qx11grab_untranslated.ts

CLEANFILES = $(BUILT_SOURCES)
