
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
#include "AggMatrix.h"

#include <iostream>
#include <fontconfig/fontconfig.h>
#include <stdexcept>

AggFontEngine::AggFontEngine(GfxFont &gfxfont,const AggMatrix & m)
        : _rend_type(agg::glyph_ren_outline),
          _agg_feng(),
          _agg_fmang(_agg_feng),
          _agg_fcurves(_agg_fmang.path_adaptor()),
          _agg_fcontour(_agg_fcurves)
{

    FcInit();

    // configure the search pattern,
    // assume "name" is a std::string with the desired font name in it

    FcPattern* pat = FcNameParse( (const FcChar8*) "Century Gothic" /*(gfxfont.getName()->getCString())*/ );
    FcConfigSubstitute(NULL, pat, FcMatchPattern);
    FcDefaultSubstitute(pat);

    FcResult r;
    // find the font
    FcPattern* font = FcFontMatch(NULL, pat, &r);


    if (font)
    {
       FcChar8* file = NULL;
       if (FcPatternGetString(font, FC_FILE, 0, &file) == FcResultMatch)
       {
          // save the file to another std::string
          _fontfile = std::string((char*)file);
       }
       FcPatternDestroy(font);

    }

    std::cerr  << gfxfont.getName()->getCString() << " => " << _fontfile << std::endl;

    FcPatternDestroy(pat);

    // _agg_fcontour.width(20.0);
    _agg_fcurves.approximation_scale(2.0);
    _agg_fcontour.auto_detect_orientation(true);

    //_fontfile = "/home/kloska/timesi.ttf";

    if(!_agg_feng.load_font(_fontfile.c_str() ,0,_rend_type)) {
        throw std::runtime_error("Failed to load '"+_fontfile+"'");
    }

    _agg_feng.hinting(true);

    _agg_feng.height(27.0);
    _agg_feng.width(27.0);

    _agg_feng.flip_y(false);

    //std::cerr << "@@@@ <" << m << " @@@@" << std::endl;

    _agg_feng.transform(m*AggMatrix(gfxfont.getFontMatrix()));

}

