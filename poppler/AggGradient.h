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
#include "AggPoint.h"
#include "AggException.h"

/** @short Mediator between the way the AGG Lib and the Poppler system
    represents gradients. The TRAITS argument covers everything which describes
    the color model we are currently using. The GFXSHADING represents the shading
    model as used by Poppler which degaults to GfxAxialShading
*/

template< class TRAITS, class GFXSHADING=GfxAxialShading >
struct AggGradient {
private:
    typedef TRAITS                                traits_t;
    typedef typename traits_t::pixfmt_t           pixfmt_t;
    typedef typename traits_t::color_t            color_t;
    typedef GFXSHADING                            gfx_shading_t;
    typedef agg::gradient_x                       agg_gradient_t; 

public:
    typedef AggColorRange<traits_t,gfx_shading_t> color_range_t;
    typedef AggMatrix                             matrix_t;
    typedef AggPoint                              point_t;

    AggGradient( gfx_shading_t & g,double min,double max) 
        : _g(g),
          _color_range(g),
          _minmax(min,max)
    {
        _g.getCoords( &_p[0].x, &_p[0].y,
                      &_p[1].x, &_p[1].y );
    }  
    
    const gfx_shading_t * operator->() const {
        return &_g;
    }

    gfx_shading_t * operator->() {
        return &_g;
    }
    
    const color_range_t & GetColorRange() const {
        return _color_range;
    }

    const point_t & operator[](int idx) const throw(AggException) {
        if(idx<0 || idx>1) {
            throw AggException("index out of range");
        }
        return _p[idx];
    }

    const point_t & getMinMax() const {
        return _minmax;
    }

    const color_range_t & getColorRange() const {
        return _color_range;
    }

    double getAngle() const {
        double x0,y0,x1,y1;
        _g.getCoords(&x0,&y0,&x1,&y1);
        return ::atan2(y1-y0,x1-x0) / agg::pi * 180.0;
    }

    void getCoords(AggPoint & p0,AggPoint & p1) {
        _g.getCoords(&p0.x,&p0.y,&p1.x,&p1.y);
    }

    static int calculate(int x, int y, int z)    {
        return agg_gradient_t::calculate(x,y,z);
    }

private:
    gfx_shading_t & _g;
    color_range_t   _color_range;
    point_t         _minmax;
    point_t         _p[2];
};

template<class T,class S>
std::ostream & operator<<(std::ostream & os , const AggGradient<T,S> & g) {
    os << "Range::(minmax:[" << g.getMinMax() << "] from:" << g[0] << ";to:" << g[1] 
       << "(" << g.getAngle() << ")) Color:[" << g.getColorRange() << "]";
    return os;
}

#endif // AGGGRADIENT_H
