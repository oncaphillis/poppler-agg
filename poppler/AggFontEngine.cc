
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
#include "AggCanvas.h"
#include "AggMatrix.h"
#include "AggPoint.h"


#include <iostream>
#include <fontconfig/fontconfig.h>
#include <stdexcept>

class AggBBox {
private:
public:
    AggBBox(double * d) {
        _x1 = d[0];
        _y1 = d[1];
        _x2 = d[2];
        _y2 = d[3];
    }

    double height() {
        return _y2;
    }

    double width() {
        return _x2;
    }

private:
    double _x1,_x2,_y1,_y2;
};

AggFontEngine::AggFontEngine(AggAbstractCanvas *canvas,GfxFont &gfxfont,const AggMatrix & m)
        : _canvas(canvas),
          _matrix(AggMatrix()),
          _rend_type(agg::glyph_ren_outline),
          _agg_feng(),
          _agg_fmang(_agg_feng),

          _agg_fpath_trans(_agg_fmang.path_adaptor(),_matrix),
          _agg_fcurves(_agg_fpath_trans)
          //_agg_fcontour(_agg_fcurves)
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
    _agg_fcurves.approximation_scale(20.0);
    //_agg_fcontour.auto_detect_orientation(true);

    if(!_agg_feng.load_font(_fontfile.c_str() ,0,_rend_type)) {
        throw std::runtime_error("Failed to load '"+_fontfile+"'");
    }

    _agg_feng.hinting(true);

    AggBBox bb(gfxfont.getFontBBox()); //  * _canvas->getScaling();

    // We make the width/height artificially large here
    // and compensate later with AggMatrix::Sacling(0.1)

    AggPoint p(AggPoint(bb.width(),bb.height() ) );

    _agg_feng.height(p.x);
    _agg_feng.width(p.y);

    _agg_feng.flip_y(false);
    _agg_feng.transform( AggMatrix(gfxfont.getFontMatrix()));

    // _matrix = ;
}

void AggFontEngine::setMatrix(const AggAbstractCanvas * c,GfxState *state) {
      _matrix = c->getScaling() * AggMatrix(state->getTextMat()) * state->getCTM();
}
