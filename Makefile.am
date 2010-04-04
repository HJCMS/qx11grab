
AUTOMAKE_OPTIONS = 1.10 gnu foreign color-tests dist-bzip2 dist-xz

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = auxdir \
	m4 \
	src \
	tr \
	extras

destdir = @docdir@
dest_DATA = AUTHORS ChangeLog INSTALL NEWS README

xdgdir = @datarootdir@/applications
xdg_DATA = qx11grab.desktop

pixmapdir = @datarootdir@/pixmaps
pixmap = src/images/qx11grab.png

EXTRA_DIST = lupdate
