//========================================================================
//
// AggCanvas.h
//
// Copyright 2012 Sebastian Kloska
//
//========================================================================

//========================================================================
//
// Modified under the Poppler project - http://poppler.freedesktop.org
//
// All changes made under the Poppler project to this file are licensed
// under GPL version 2 or later
//
// To see a description of the changes please see the Changelog file that
// came with your tarball or type make ChangeLog if you are building from git
//
//========================================================================

#ifndef AGGCANVAS_H
#define AGGCANVAS_H

#include "AggMatrix.h"

#include "GfxState.h"

#include "agg_conv_bspline.h"
#include "agg_conv_segmentator.h"
#include "agg_conv_dash.h"
#include "agg_pixfmt_cmyk.h"
#include "agg_pixfmt_rgb.h"
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_pixfmt_rgb.h"
#include "agg_gamma_lut.h"
#include "agg_ellipse.h"
#include "agg_rounded_rect.h"
#include "agg_conv_stroke.h"
#include "agg_font_freetype.h"
#include "agg_glyph_raster_bin.h"
#include "agg_renderer_raster_text.h"
#include "agg_embedded_raster_fonts.h"
#include "agg_conv_curve.h"
#include "agg_conv_contour.h"
#include "agg_conv_bspline.h"
#include "agg_conv_segmentator.h"
#include "agg_trans_single_path.h"
#include "agg_conv_transform.h"
#include "agg_path_storage.h"


class AbstractAggCanvas
{
private:
public:
  typedef AggMatrix matrix_t;
  
  virtual ~AbstractAggCanvas() {
  }

  virtual size_t getWidth()  const = 0;
  virtual size_t getHeight() const = 0;

  const matrix_t & getScaling() const {
    return _scale;
  }

  void setScaling(const matrix_t & m) {
    _scale = m;
  }

  const matrix_t & getDefMatrix() const {
    return _def;
  }

  void setDefMatrix(const matrix_t & m) {
    _def = m;
  }

  const matrix_t &  getCTM() const {
      return _ctm;
  }

  void setCTM(const matrix_t & m) {
    _ctm = m;
  }

protected:
  matrix_t _scale;
  matrix_t _ctm;
  matrix_t _def;
};

template<class COLOR> 
class AggColorTraits;

template<> 
class AggColorTraits<agg::cmyk> {
private:
  typedef agg::rendering_buffer   rendering_buffer_t;
  typedef unsigned char           ubyte_t;
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
  }
  
  ~AggColorTraits() {
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

private:
  rendering_buffer_t * _rendering_buffer;
  ubyte_t            * _array;
  fmt_t              * _fmt;
};


template<class COLOR,class COLORTRAITS=AggColorTraits<COLOR> >
class BasicAggCanvas : public AbstractAggCanvas {

private:
  typedef COLORTRAITS traits_t;
  typedef typename traits_t::color_t color_t;
  typedef typename traits_t::fmt_t   fmt_t;
  typedef typename traits_t::data_t  data_t;
public:
  BasicAggCanvas(long w,long h) :
    _traits(w,h),
    _fmt(_traits.fmt())
  {
  }
  virtual ~BasicAggCanvas() {
  }
  virtual size_t getWidth() const {
    return _fmt->width();
  }
  virtual size_t getHeight() const {
    return _fmt->height();
  }

  fmt_t * getFmt() {
    return _traits.fmt();
  }

  data_t * getData() {
    return _traits.data();
  }

private:
  traits_t    _traits;
  fmt_t     * _fmt;
};

typedef BasicAggCanvas<agg::cmyk> AggCmykCanvas;

#endif // AGGCANVAS_H
