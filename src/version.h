/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2008
* Copyright: See COPYING file that comes with this distribution
**/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#define QX11GRAB_VERSION "0.1.5"

#define LOG_FILE "/tmp/qx11grab.log"

#define HTML_NOTICE "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n" \
"<html><head><meta name=\"qrichtext\" content=\"1\" />" \
"<style type=\"text/css\">p, li { white-space: pre-wrap; }</style></head>" \
"<body style=\" font-family:'Sans Serif'; font-size:12pt; font-weight:400; font-style:normal;\">\n" \
"<p style=\" margin:0px; text-indent:0px;\"><span style=\" color:#ff0000;\">%1</span>: %2</p>" \
"</body></html>"

#define OSS_IN_USE "Audio Device %s already in use\nDisable Capture with -f oss Option"
