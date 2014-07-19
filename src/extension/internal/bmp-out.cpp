/*
 * A simple utility for exporting svg images as bmp images.
 * This then makes 'save as...' BMP.
 *
 * Authors:
 *   Rikhard Prydii <idsgpr@gmail.com>
 *
 * Copyright (C) 2014 Authors
 *
 * Released under GNU GPL, read the file 'COPYING' for more information
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <cerrno>
#include <cstdio>
#include "bmp-out.h"
#include "cairo-png-out.h"
#include <print.h>
#include "extension/system.h"
#include "extension/print.h"
#include "extension/db.h"
#include "extension/output.h"
#include "display/drawing.h"

#include "display/curve.h"
#include "display/canvas-bpath.h"
#include "sp-item.h"
#include "style.h"
#include "sp-root.h"
#include "sp-shape.h"

#include "io/sys.h"
#include "document.h"

#include <Magick++.h>
#include <iostream>

namespace Inkscape {
namespace Extension {
namespace Internal {

bool BmpOutput::check(Inkscape::Extension::Extension * /*module*/)
{
    return true;
}

/**
    \brief  This function calls the output module with the filename
	\param  mod   unused
	\param  doc   Document to be saved
    \param  uri   Filename to save to (probably will end in .bmp)
*/
void BmpOutput::save(Inkscape::Extension::Output * mod, SPDocument *doc, gchar const *filename)
{
#ifdef WIN32
	char tmpfile[L_tmpnam];
	tmpnam(tmpfile);
#else
	char *tmpfile = new char[12];
 	strcpy(tmpfile, "pngXXXXXX");
 	mkstemp(tmpfile);	
#endif
 	char const *tmpwork = tmpfile;
 #ifdef WIN32
 	++tmpwork;
 #endif
 	CairoRendererOutput png_out;
	png_out.save(mod, doc, tmpwork);
	try {
		Magick::Image img;
		img.magick("png");
		img.read(tmpwork);
		img.magick("bmp");
		img.write(filename);
	} catch (Magick::Exception &e) {
		remove(tmpwork);
#ifndef WIN32
		delete[] tmpfile;
#endif
		throw Inkscape::Extension::Output::save_failed();		
	}
	remove(tmpwork);
#ifndef WIN32
		delete[] tmpfile;
#endif	
}

/**
	\brief   A function allocate a copy of this function.

	This is the definition of bmp out.  This function just
	calls the extension system with the memory allocated XML that
	describes the data.
*/
void
BmpOutput::init (void)
{
	Inkscape::Extension::build_from_mem(
		"<inkscape-extension xmlns=\"" INKSCAPE_EXTENSION_URI "\">\n"
			"<name>BMP Output</name>\n"
			"<id>rikhard.inkscape.output.bmp.cairo</id>\n"
			"<output>\n"
				"<extension>.bmp</extension>\n"
                "<mimetype>image/bmp</mimetype>\n"
				"<filetypename>Bitmap image (*.bmp)</filetypename>\n"
				"<filetypetooltip>BMP File</filetypetooltip>\n"
			"</output>\n"
		"</inkscape-extension>", new BmpOutput());

	return;
}

} } }  /* namespace Inkscape, Extension, Implementation */

