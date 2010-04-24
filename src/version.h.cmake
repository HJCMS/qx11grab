/**
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann http://qx11grab.hjcms.de, (C) 2007-2010
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Library General Public
* License as published by the Free Software Foundation; either
* version 2 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU Library General Public License
* along with this library; see the file COPYING.LIB.  If not, write to
* the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
* Boston, MA 02110-1301, USA.
**/

#include <iostream>
#include <cstdio>
#include <cstdlib>

#define QX11GRAB_VERSION "@QX11GRAB_VERSION@"

#define HTML_NOTICE "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n" \
"<html><head><meta name=\"qrichtext\" content=\"1\" />" \
"<style type=\"text/css\">p, li { white-space: pre-wrap; }</style></head>" \
"<body style=\" font-family:'Sans Serif'; font-size:12pt; font-weight:400; font-style:normal;\">\n" \
"<p style=\" margin:0px; text-indent:0px;\"><span style=\" color:#ff0000;\">%1</span>: %2</p>" \
"</body></html>"

#define OSS_IN_USE "Audio Device %s already in use\nDisable Capture with -f oss Option"
