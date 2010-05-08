#!/usr/bin/env sh
#################################################################
# Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
#
# Copyright: See COPYING file that comes with this distribution
#################################################################

## -silent
for i in ar da de es fr iw ja_JP pl pt ru sk sl sv uk zh_CN zh_TW untranslated ; do
	lupdate -noobsolete -extensions h,cpp,cxx \
		src/ build/src -ts i18n/qx11grab_${i}.ts
done
