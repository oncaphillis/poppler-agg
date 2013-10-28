//========================================================================
//
// AggColorRange.h
//
// Copyright 2012-13 Sebastian Kloska
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

#ifndef AGGGRADIENT_H
#define AGGGRADIENT_H

#include "agg_span_gradient.h"

#include "GfxState.h"
#include "AggColorRange.h"
#include "AggMatrix.h"

template< class TRAITS, class GFXSHADING=GfxAxialShading >
struct AggGradient
{
private:
    typedef TRAITS                                traits_t;
    typedef typename traits_t::pixfmt_t           pixfmt_t;
    typedef typename traits_t::color_t            color_t;
    typedef GFXSHADING                            gfx_shading_t;
    typedef agg::gradient_x                       agg_gradient_t; 

public:
    typedef AggColorRange<traits_t,gfx_shading_t> color_range_t;
    typedef AggMatrix                             matrix_t;

    AggGradient( gfx_shading_t & g,double min,double max) 
        : _g(g),
          _color_range(g),
          _min(min),
          _max(max) { 
        double x0,y0,x1,y1;
        
        _g.getCoords(&x0,&y0,&x1,&y1);
        std::cerr << "axial:(" << x0 << ";" << y0 << ")(" << x1 << ";" << y1 << ")" << std::endl;
    }  
    
public:
    color_range_t & getColorRange() {
        return _color_range;
    }

    static int calculate(int x, int y, int z)
    {
        int o=agg_gradient_t::calculate(x,y,z);
        return o;
    }
private:
    gfx_shading_t & _g;
    color_range_t   _color_range;
    double          _min;
    double          _max;
};

#endif // AGGGRADIENT_H
