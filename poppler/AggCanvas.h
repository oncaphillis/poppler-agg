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
#include "AggColorTraits.h"

#include "agg_conv_bspline.h"
#include "agg_conv_segmentator.h"
#include "agg_conv_dash.h"
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


#include <vector>
#include <stack>

class AbstractAggCanvas
{
private:
public:

  typedef AggMatrix        matrix_t;
  typedef agg::line_join_e join_t;
  typedef agg::line_cap_e  cap_t;

  struct GfxNode
  {
    matrix_t             _scale;
    matrix_t             _ctm;
    matrix_t             _def;
    join_t               _join;
    cap_t                _cap;
    double               _line_width;
    std::vector<double>  _dash;
  };

  
  virtual ~AbstractAggCanvas() {
  }

  virtual size_t getWidth()  const = 0;
  virtual size_t getHeight() const = 0;

  const matrix_t & getScaling() const {
    return getNode()._scale;
  }

  void setScaling(const matrix_t & m) {
    getNode()._scale = m;
  }

  const matrix_t & getDefMatrix() const {
    return getNode()._def;
  }

  void setDefMatrix(const matrix_t & m) {
    getNode()._def = m;
  }

  const matrix_t &  getCTM() const {
    return getNode()._ctm;
  }

  void setCTM(const matrix_t & m) {
    getNode()._ctm = m;
  }

  matrix_t getTotalCTM() const {
    return getCTM() * getScaling();
  }

  const std::vector<double> & getDash() const  {
    return getNode()._dash;
  }

  void setDash(const std::vector<double> & d )  {
    GfxNode & gfx = getNode();
    gfx._dash.clear();
    for(std::vector<double>::const_iterator i = d.begin();i!=d.end();i++)
    {
      gfx._dash.push_back(*i);
    }
  }
  
  void setCap(GfxState * state) {
    GfxNode & gfx = getNode();
    switch (state->getLineCap()) {
    case 0:
      gfx._cap = agg::butt_cap;
      break;
    case 1:
      gfx._cap = agg::round_cap;
      break;
    case 2:
      gfx._cap = agg::square_cap;
      break;
    }
  }

  void setJoin(GfxState * state) {
    GfxNode & gfx = getNode();
    switch (state->getLineJoin()) {
    case 0:
      gfx._join =  agg::miter_join;
      break;
    case 1:
      gfx._join = agg::round_join;
      break;
    case 2:
      gfx._join = agg::bevel_join;
      break;
    }
  }
  
  join_t getJoin() const  {
    return getNode()._join;
  }
  cap_t getCap() const  {
    return getNode()._cap;
  }
  double getLineWidth() const  {
    return getNode()._line_width;
  }

  void setLineWidth(GfxState * state) {
    getNode()._line_width = state->getLineWidth();
  }

  virtual void setFillColor(GfxState * state)   = 0;
  virtual void setStrokeColor(GfxState * state) = 0;

  virtual void push()         = 0;
  virtual void pop()          = 0;
  virtual GfxNode & getNode() = 0;
  virtual const GfxNode & getNode() const = 0;
};

template<class COLOR,class COLORTRAITS=AggColorTraits<COLOR> >
class BasicAggCanvas : public AbstractAggCanvas {

private:
  typedef AbstractAggCanvas          super;
  typedef COLORTRAITS                traits_t;
  typedef typename traits_t::color_t color_t;
  typedef typename traits_t::fmt_t   fmt_t;
  typedef typename traits_t::data_t  data_t;

public:

  struct GfxNode : public super::GfxNode
  {
    color_t        _fill_color;
    color_t        _stroke_color;
  };

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

  virtual void push()
  {
    _stack.push(_the_node);
  }

  virtual void pop()
  {
    if(!_stack.empty())
    {
      _the_node=_stack.top();
      _stack.pop();
    }
  }

  virtual super::GfxNode & getNode()  {
    return _the_node;
  }

  virtual const super::GfxNode & getNode()  const {
    return _the_node;
  }

  virtual void setFillColor( GfxState * state ) {
    _traits.toAggColor(state->getFillColorSpace(),state->getFillColor(),_the_node._fill_color);
    std::cerr << __PRETTY_FUNCTION__ << "(" << _the_node._fill_color << ")" << std::endl;
  }
  
  virtual void setStrokeColor( GfxState * state) {
    _traits.toAggColor(state->getStrokeColorSpace(),state->getStrokeColor(),_the_node._stroke_color);
    std::cerr << __PRETTY_FUNCTION__ << "(" << _the_node._stroke_color << ")" << std::endl;
  }
 
  const color_t getStrokeColor() const {
   return _the_node._stroke_color;
  }

  const color_t getFillColor() const {
    return _the_node._fill_color;
  }

private:
  traits_t            _traits;
  fmt_t             * _fmt;
  GfxNode             _the_node;
  std::stack<GfxNode> _stack;
};

typedef BasicAggCanvas<agg::cmyk> AggCmykCanvas;

#endif // AGGCANVAS_H