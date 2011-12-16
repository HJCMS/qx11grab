#!/usr/bin/env sh
#################################################################
# Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
#
# Copyright: See COPYING file that comes with this distribution
#################################################################

## -silent
for i in i18n/qx11grab_*.ts ; do
	/usr/bin/lupdate -noobsolete -recursive ./src/ ./build/src -ts ${i}
done
