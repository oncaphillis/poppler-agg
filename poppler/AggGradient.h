//========================================================================
//
// AggColorRange.h
//
// Copyright 2012-14 Sebastian Kloska
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

template<class GFX>
struct ShadingTraits
{
};

template<>
struct ShadingTraits<GfxAxialShading>
{
    typedef agg::gradient_x agg_gradient_t;
};

template<>
struct ShadingTraits<GfxRadialShading>
{
    typedef agg::gradient_radial agg_gradient_t;
};

/** @short Mediator between the way the AGG Lib and the Poppler system
    represents gradients. The TRAITS argument covers everything which describes
    the color model we are currently using. The GFXSHADING represents the shading
    model as used by Poppler which degaults to GfxAxialShading
*/

template< class TRAITS, class GFXSHADING, class  SHADINGTRAITS=ShadingTraits<GFXSHADING> >
struct AggGradient {
private:
    typedef TRAITS                                 traits_t;
    typedef typename traits_t::pixfmt_t            pixfmt_t;
    typedef typename traits_t::color_t             color_t;
    typedef typename SHADINGTRAITS::agg_gradient_t agg_gradient_t;
public:
    typedef GFXSHADING                            gfx_shading_t;
    typedef AggColorRange<traits_t,gfx_shading_t> color_range_t;
    typedef AggMatrix                             matrix_t;
    typedef AggPoint                              point_t;

    AggGradient( gfx_shading_t & g,double min,double max) 
        : _g(g),
          _color_range(g),
          _minmax(min,max)
    {
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

    const color_range_t & getColorRange() const {
        return _color_range;
    }

    static int calculate(int x, int y, int z)    {
        return agg_gradient_t::calculate(x,y,z);
    }

private:
    gfx_shading_t & _g;
    color_range_t   _color_range;
    point_t         _minmax;
};

template<class TRAITS>
class AggLinearGradient : public AggGradient<TRAITS,GfxAxialShading>
{
private:
    typedef AggGradient<TRAITS,GfxAxialShading> super;
public:
    typedef typename super::gfx_shading_t gfx_shading_t;
    typedef typename super::matrix_t      matrix_t;
    typedef typename super::point_t       point_t;

    AggLinearGradient(gfx_shading_t & g,double min,double max)
        : super(g,min,max)
    {

    }

    void getCoords(AggPoint & p0,AggPoint & p1) const {
        (*this)->getCoords(&p0.x,&p0.y,&p1.x,&p1.y);
    }

    double getAngle(const matrix_t & m=matrix_t()) const {
        point_t p0,p1;
        getCoords(p0,p1);
        return (p0*m).getAngle(p1*m);
    }
};

template<class TRAITS>
class AggRadialGradient : public AggGradient<TRAITS,GfxRadialShading>
{
private:
    typedef AggGradient<TRAITS,GfxRadialShading> super;
public:
    typedef typename super::gfx_shading_t gfx_shading_t;
    AggRadialGradient(gfx_shading_t & g,double min,double max)
        : super(g,min,max)
    {

    }

    void getCoords(AggPoint & p0,AggPoint & p1,AggPoint::coord_t & r0,AggPoint::coord_t & r1)
    {
        (*this)->getCoords(&p0.x,&p0.y,&p1.y,&p1.x,&r0,&r1);
    }
};

template<class T,class S>
std::ostream & operator<<(std::ostream & os , const AggGradient<T,S> & g) {
    os << "Range::(minmax:[" << g.getMinMax() << "] from:" << g[0] << ";to:" << g[1] 
       << "(" << g.getAngle() << ")) Color:[" << g.getColorRange() << "]";
    return os;
}

#endif // AGGGRADIENT_H
