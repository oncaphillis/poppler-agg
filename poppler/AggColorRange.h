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

#ifndef AGGCOLORRANGE_H
#define AGGCOLORRANGE_H

#include "GfxState.h"
#include "AggColorTraits.h"

#include <map>
#include <iostream>

/** @short Mediator between the AGG and the Poppler representation of color 
    gradients. 

    Central is the operator[] which allows us to access a linear index of 
    AggColorRange::color_t values.
*/

template< class TRAITS,class GFXSHADING >
struct AggColorRange
{
    typedef TRAITS                      traits_t;
    typedef typename traits_t::pixfmt_t pixfmt_t;
    typedef typename traits_t::color_t  color_t;
    typedef GFXSHADING                  gfx_shading_t;
    typedef std::map<double,std::pair<long,color_t> >          i_map_t;

    AggColorRange( gfx_shading_t & s, int size=256) 
        : _s(s),
          _size(size) {
    }

    ~AggColorRange() {
    }

    unsigned size() const {
        return _size;
    }

    const color_t operator [] (unsigned i) const
    { 
        GfxColor gc;
        double d = _s.getDomain0() + ::fabs( ( _s.getDomain1() - _s.getDomain0()) * i ) / _size;
        _s.getColor(d,&gc);
        color_t c = traits_t::toAggColor(_s.getColorSpace(),&gc,c);
        c.a = 1.0;
        return c;
    }

private:
    AggColorRange(const AggColorRange &);
    AggColorRange();

    gfx_shading_t & _s;
    int             _size;
};

template<class T,class S>
std::ostream & operator<<(std::ostream & os,const AggColorRange<T,S> & cr)  {
    return os << "cr:@" << cr.size() << " (" << cr[0] << "..." << cr[cr.size()/2] << "..." << cr[cr.size()] << ")";
}

#endif // AGGCOLORRANGE_H
