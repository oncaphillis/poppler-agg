//========================================================================
//
// AggColorTraits.h
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

#ifndef AGGCOLORTRAITS_H
#define AGGCOLORTRAITS_H

#include "GfxState.h"

#include "agg_pixfmt_cmyk.h"
#include "agg_pixfmt_rgb.h"
#include "agg_pixfmt_device_n.h"

#include <iostream>

template<class COLOR,class STATE> 
class AggColorTraits;

/** @short The central class for AGG specific color management.
    This Traits structure provides the definition of (1) colors
    we are using internally, (2) the target color space and (3)
    methods for transformation between them.

    It also provides methods to map the color space used by the 
    poppler engine to and from the AGG engine by processing the
    poppler all perpous struct GfxState.

    The Traits also represents an AGG rendering_buffer_t when
    actually allocating an object of this class. 

*/

class BasicColorTraits {
public:
  typedef unsigned char             ubyte_t;
  typedef agg::rendering_buffer     rendering_buffer_t;
  
  BasicColorTraits(size_t w,size_t h,size_t s) :
    _array( new ubyte_t[ h * s ] ),
    _rendering_buffer(_array,w,h,s)
  {
  };

  ~BasicColorTraits() {
    delete _array;
  }

  rendering_buffer_t & buffer() {
    return _rendering_buffer;
  }

  const rendering_buffer_t & buffer() const {
    return _rendering_buffer;
  }

  ubyte_t * array() {
    return _array;
  }

  const ubyte_t * array() const {
    return _array;
  }

private:
  ubyte_t             * _array; 
  rendering_buffer_t  _rendering_buffer;
};

/** @short The standard instantiation of AggColorTraits. Maps the input colorspace
    to cmyk32.
*/

template<> 
class AggColorTraits<agg::cmyka,GfxState> : public BasicColorTraits {
private:
  typedef BasicColorTraits          super;
  typedef GfxDeviceRGBColorSpace    colorspace_t;
  typedef super::rendering_buffer_t rendering_buffer_t;
public:
  typedef agg::cmyka          color_t;
  typedef agg::pixfmt_cmyk32  pixfmt_t;
  typedef ubyte_t             data_t;

  AggColorTraits(long w,long h) :
    super( w, h, w * 4 )
  {
    ::memset(array(),0x00,w * h * 4);
    _fmt = new pixfmt_t( this->buffer() );
    _cs  = new colorspace_t();
  }
  
  ~AggColorTraits() {
    delete _cs;
    delete _fmt;
  }

  pixfmt_t * fmt() {
    return _fmt;
  }

  static
  color_t toAggColor(GfxColorSpace * cs,GfxColor * ci,color_t &co) {

    GfxCMYK cmyk;

    cs->getCMYK(ci,&cmyk);

    co  = color_t( (double)cmyk.c / 65535.0,
                   (double)cmyk.m / 65535.0,
                   (double)cmyk.y / 65535.0,
                   (double)cmyk.k / 65535.0,co.a);
    return co;
  }

  static
  color_t toAggAlpha(GfxColorSpace * cs,double alpha,color_t & co) {
    co.alpha(alpha);
    return co;
  }

private:
  colorspace_t        * _cs; 
  pixfmt_t            * _fmt;
};

std::ostream & operator<<(std::ostream & os,const agg::cmyka & c);

/** @short Standard mapping from inernal color space rgba will be
    mapped to rgb24.
*/

template<> 
class AggColorTraits<agg::rgba,GfxState> : public BasicColorTraits {
private:
  typedef BasicColorTraits          super;
  typedef super::rendering_buffer_t rendering_buffer_t;
  typedef unsigned char             ubyte_t;
  typedef GfxDeviceRGBColorSpace    colorspace_t;
public:
  typedef agg::rgba           color_t;
  typedef agg::pixfmt_rgb24   pixfmt_t;
  typedef ubyte_t             data_t;

  AggColorTraits(long w,long h) :
    super(w,h,w*3)
  {
    ::memset(array(),0xff,w*h*3);
    _fmt = new pixfmt_t( this->buffer() );
    _cs  = new colorspace_t();
  }
  
  ~AggColorTraits() {
    delete _cs;
    delete _fmt;
  }

  pixfmt_t * fmt() {
    return _fmt;
  }
  static 
  color_t toAggColor(GfxColorSpace * cs,GfxColor * ci,color_t &co) {

    GfxRGB rgb;

    cs->getRGB(ci,&rgb);

    co  = color_t( (double) rgb.r / 65535.0,
                   (double) rgb.g / 65535.0,
                   (double) rgb.b / 65535.0,co.a);
    return co;
  }

  static
  color_t toAggAlpha(GfxColorSpace * cs,double alpha,color_t & co) {
    co.alpha(alpha);
    return co;
  }

private:
  colorspace_t        * _cs; 
  pixfmt_t            * _fmt;
};

template<int N>
class AggColorTraits<agg::device_na<N>,GfxState> : public BasicColorTraits {
private:
  typedef BasicColorTraits          super;
  //typedef GfxDeviceRGBColorSpace    colorspace_t;
  typedef super::rendering_buffer_t rendering_buffer_t;
public:
  typedef agg::device_na<N>       color_t;
  //typedef agg::pixfmt_device_n_32  pixfmt_t;
  typedef agg::pixfmt_alpha_blend_device_n<
       agg::blender_device_n<agg::device_na8<N>,
       agg::order_device_n<N> >,
       agg::rendering_buffer >   pixfmt_t;

  typedef ubyte_t                 data_t;

  AggColorTraits(long w,long h) :
    super( w, h, w * N )
  {
    ::memset(array(),0x00,w * h * N);
    _fmt = new pixfmt_t( this->buffer() );
  }

  ~AggColorTraits() {
    delete _fmt;
  }

  pixfmt_t * fmt() {
    return _fmt;
  }

  static
  color_t toAggColor(GfxColorSpace * cs,GfxColor * ci,color_t &co) {

    GfxColor dn;

    cs->getDeviceN(ci,&dn);

    for(int i=0;i<N;i++)
    {
        co[i] = (double)(dn.c[i]  / 65535.0);
    }
    return co;
  }

  static
  color_t toAggAlpha(GfxColorSpace * cs,double alpha,color_t & co) {
    co.alpha(alpha);
    return co;
  }

private:
  pixfmt_t            * _fmt;
};

std::ostream & operator<<(std::ostream & os,const agg::rgba & c);

#endif // AGGCOLORTRAITS_H
