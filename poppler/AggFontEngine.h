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
#include "agg_conv_contour.h"
#include "agg_renderer_scanline.h"

#include "FontInfo.h"
#include "GfxFont.h"

#include <string>
#include <iostream>

class AggFontEngine {
private:
public:
    typedef agg::font_engine_freetype_int32 font_engine_t;
    typedef agg::font_cache_manager<font_engine_t> font_manager_t;
    typedef agg::glyph_rendering rendering_t;
    typedef agg::conv_curve<font_manager_t::path_adaptor_type> font_curve_t;
    typedef agg::conv_contour<agg::conv_curve<font_manager_t::path_adaptor_type> > font_contour_t;

    AggFontEngine(GfxFont &gfxfont);

    template< class Renderer >
    void render(unsigned chr, Renderer& ren) {
        const agg::glyph_cache * gc = _agg_fmang.glyph(chr);
        if(gc!=NULL) {
            std::cerr << " #(" << gc->data_size << ")# " << std::endl;
            _agg_fmang.init_embedded_adaptors(gc, 150, 600);
            agg::render_scanlines(_agg_fmang.mono_adaptor(),
                                  _agg_fmang.mono_scanline(),ren);
        }
     }

private:
    std::string    _fontfile;
    rendering_t    _rend_type;
    font_engine_t  _agg_feng;
    font_manager_t _agg_fmang;
    font_curve_t   _agg_fcurves;
    font_contour_t _agg_fcontour;
};

#endif // AGGFONTENGINE_H
