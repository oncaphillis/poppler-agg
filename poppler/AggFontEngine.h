//========================================================================
//
// AggFontEngine.h
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

#ifndef AGGFONTENGINE_H
#define AGGFONTENGINE_H

#include "agg_font_freetype.h"
#include "FontInfo.h"
#include "GfxFont.h"

class AggFontEngine {
private:
public:
    typedef agg::font_engine_freetype_int32 font_engine_t;
    AggFontEngine(GfxFont &gfxfont);
private:
};

#endif // AGGFONTENGINE_H
