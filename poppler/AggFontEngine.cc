
//========================================================================
//
// AggFontEngine.cc
//
// Copyright 2013-14 Sebastian Kloska
//
//========================================================================

//========================================================================
//
// Modified under the Poppler project - http://poppler.freedesktop.org
//
// All changes made under the Poppler project to this fille are licensed
// under GPL version 2 or later
//
// To see a description of the changes please see the Changelog file that
// came with your tarball or type make ChangeLog if you are building from git
//
//========================================================================

#include "AggFontEngine.h"

#include <iostream>
#include <fontconfig/fontconfig.h>

AggFontEngine::AggFontEngine(GfxFont &gfxfont) {
    std::cerr << " .. " << gfxfont.getName() << "/" << gfxfont.getFamily() << std::endl;
}
