
qx11grab_de = \
	@srcdir@/qt_de.ts \
	@srcdir@/qx11grab_de.ts

trusersdir = @datarootdir@/qx11grab
trusers_DATA = qx11grab_untranslated.ts

transdir = @Q_LOCALE_DIR@
trans_DATA = qx11grab_de.qm

all: qx11grab_de

qx11grab_de:
	@LRELEASE@ -compress -nounfinished -silent ${qx11grab_de} -qm qx11grab_de.qm
