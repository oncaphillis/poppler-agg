#ifndef AGGCOLORTRAITS_H
#define AGGCOLORTRAITS_H

#include "GfxState.h"

#include "agg_pixfmt_cmyk.h"
#include "agg_pixfmt_rgb.h"

template<class COLOR> 
class AggColorTraits;

template<> 
class AggColorTraits<agg::cmyk> {
private:
  typedef agg::rendering_buffer     rendering_buffer_t;
  typedef unsigned char             ubyte_t;
  typedef GfxDeviceRGBColorSpace   colorspace_t;
public:
  typedef agg::cmyk           color_t;
  typedef agg::pixfmt_cmyk32  fmt_t;
  typedef ubyte_t             data_t;

  AggColorTraits(long w,long h) {
    size_t s          = w * h * 4;
    _array            = new ubyte_t[ s ]; 
    ::memset(_array,0,s);
    _rendering_buffer = new rendering_buffer_t(_array, w , h , w * 4);
    _fmt              = new fmt_t(*_rendering_buffer);
    _cs               = new colorspace_t();
  }
  
  ~AggColorTraits() {
    delete _cs;
    delete _rendering_buffer;
    delete _array;
    delete _fmt;
  }

  fmt_t * fmt() {
    return _fmt;
  }

  data_t * data() {
    return _array;
  }

  void toAggColor(GfxColorSpace * cs,GfxColor * ci,color_t &co) {

    GfxCMYK cmyk;

    cs->getCMYK(ci,&cmyk);

    co  = color_t( (double)cmyk.c / 65535.0,
                   (double)cmyk.m / 65535.0,
                   (double)cmyk.y / 65535.0,
                   (double)cmyk.k / 65535.0);
  }

private:
  colorspace_t        * _cs; 
  rendering_buffer_t  * _rendering_buffer;
  ubyte_t             * _array;
  fmt_t               * _fmt;
};

std::ostream & operator<<(std::ostream & os,const agg::cmyk & c);


#endif // AGGCOLORTRAITS_H
