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
#include "agg_conv_transform.h"
#include "agg_renderer_scanline.h"


#include "FontInfo.h"
#include "GfxFont.h"

#include "AggMatrix.h"

#include <string>
#include <iostream>

class AggAbstractCanvas;
class GfxState;

class AggFontEngine {
private:
public:
    typedef agg::font_engine_freetype_int32 font_engine_t;
    typedef agg::font_cache_manager<font_engine_t> font_manager_t;
    typedef agg::glyph_rendering rendering_t;

    typedef agg::conv_transform< font_engine_t::path_adaptor_type > font_path_trans_t;
    typedef agg::conv_transform< font_path_trans_t >                font_curve_trans_t;
    typedef agg::conv_curve< font_path_trans_t >                    font_curve_t;

    // typedef agg::conv_contour< font_curve_t >    font_contour_t;

    AggFontEngine(AggAbstractCanvas * canvas,GfxFont & gfxfont,const AggMatrix &m);
#if 0
    template<class VS> void dump_path(VS& path)
    {
        std::cerr << "------- ";

        path.rewind(0);
        unsigned cmd;
        double x, y;
        int n = 0;
        while(!agg::is_stop(cmd = path.vertex(&x, &y)))
        {
            n++;
            std::cerr << "(" << cmd << "," << x << "," << y << ") ";
        }
        std::cerr << std::endl << '#' << n << std::endl;
    }
#endif

    template< class Rasterizer,class Scanline ,class Renderer >
    void render(unsigned chr, Rasterizer & ras,
                Scanline & sl,Renderer& ren,GfxState *state,int x,int y) {

        const agg::glyph_cache * gc = _agg_fmang.glyph(chr);

        if(gc!=NULL) {
            setMatrix(_canvas,state);
            _matrix = _matrix * AggMatrix::Translation(x,y);
            _agg_fmang.init_embedded_adaptors(gc, 0, 0);
            ras.reset();
            ras.add_path(_agg_fcurves);
            agg::render_scanlines(ras,sl,ren);
        }
     }

private:
    void setMatrix(const AggAbstractCanvas * c,GfxState * state);

    AggAbstractCanvas  * _canvas;
    AggMatrix            _matrix;
    std::string          _fontfile;
    rendering_t          _rend_type;
    font_engine_t        _agg_feng;
    font_manager_t       _agg_fmang;

    font_path_trans_t    _agg_fpath_trans;
    font_curve_t         _agg_fcurves;
    //font_contour_t       _agg_fcontour;
};

#endif // AGGFONTENGINE_H
