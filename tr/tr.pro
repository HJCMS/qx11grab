####################################################################
##
## Author: Juergen Heinemann http://qtidy.hjcms.de, (C) 2007-2008
##
## Copyright: See COPYING file that comes with this distribution
##
####################################################################

DESTDIR = .
TEMPLATE = subdirs

trans.files = qx11grab_de.qm

trans.path +=	$$[QT_INSTALL_TRANSLATIONS]

INSTALLS +=	trans
