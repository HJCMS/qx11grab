####################################################################
##
## Author: Juergen Heinemann http://tourenbuch.hjcms.de, (C) 2007-2008
##
## Copyright: See COPYING file that comes with this distribution
##
####################################################################

PREFIX = $$[QT_INSTALL_PREFIX]

debug {
	QMAKE_CXXFLAGS_DEBUG += -O0 -mtune=generic -march=i686 -fstack-protector -D_FORTIFY_SOURCE=2
	QMAKE_CXXFLAGS_DEBUG += -Wall -W -Wextra -pedantic -Wstack-protector
	QMAKE_CXXFLAGS_DEBUG += -Wno-long-long -Wno-overlength-strings
}

release {
	QMAKE_CXXFLAGS_DEBUG += -O2 -mtune=i686 -march=i386 -fstack-protector
	QMAKE_CXXFLAGS_DEBUG += -Wall -W -Wstack-protector -D_FORTIFY_SOURCE=2
}
