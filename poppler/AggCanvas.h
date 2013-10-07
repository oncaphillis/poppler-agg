//========================================================================
//
// AggCanvas.h
//
// Copyright 2012-13 Sebastian Kloska
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
#include "AggPath.h"
#include "AggColorTraits.h"

#include "agg_conv_bspline.h"
#include "agg_conv_segmentator.h"
#include "agg_conv_dash.h"
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_p.h"
#include "agg_scanline_boolean_algebra.h"
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

#include "agg_span_allocator.h"
#include "agg_span_interpolator_linear.h"
#include "agg_span_gradient.h"

#include <vector>
#include <stack>

class gradient_polymorphic_wrapper_base
{
public:
    virtual 
    ~gradient_polymorphic_wrapper_base() {
    }
    virtual int calculate(int x, int y, int) const = 0;
};

template<class GradientF> 
class gradient_polymorphic_wrapper : public gradient_polymorphic_wrapper_base
{
public:
    
    gradient_polymorphic_wrapper() 
        : m_adaptor(m_gradient) {
    };

    virtual int calculate(int x, int y, int d) const
    {
        // std::cerr << "@" << d << "::[" << x << " " << y << "]::" << m_adaptor.calculate(x, y, d) << std::endl;
        return m_adaptor.calculate(x, y, d);
    }

    GradientF m_gradient;

    agg::gradient_reflect_adaptor<GradientF> m_adaptor;
};

template<class COLOR>
struct color_function_profile
{
    typedef COLOR color_t;

    color_function_profile() {
    }

    color_function_profile(const color_t * c, const agg::int8u * p) :
        _colors(c), 
        _profile(p) 
    {
    }
    
    static unsigned size() { 
        return 256; 
    }

    const color_t operator [] (unsigned v) const
    { 
        return _cc;
    }
    const color_t      _cc;
    const color_t    * _colors;
    const agg::int8u * _profile;
};

template<>
const agg::cmyka color_function_profile<agg::cmyka>::operator [] (unsigned v) const;

class AggAbstractCanvas
{
private:
public:
  typedef GfxState         gfxstate_t;
  typedef GfxState         gfxpath_t;
  typedef AggMatrix        matrix_t;
  typedef AggPath          path_t;

  typedef agg::path_storage  path_storage_t;
  typedef agg::line_join_e   join_t;
  typedef agg::line_cap_e    cap_t;

  /** @short This node serves as the base class for nodes of color model dependent 
      suclasses of AggAbstractCanvas. With these nodes we build up a stack
      of graphics states whoich we might push and pop to implement the save/restore
      mode of the PDF model.
  */

  struct GfxNode
  {
    GfxNode()  {
      _clip.active = false;
      _flatness    = 1;
      _line_width  = 1.0;
      _miter_limit = 1.0;
    }

    matrix_t             _ctm;
    matrix_t             _def;
    join_t               _join;
    cap_t                _cap;
    double               _line_width;
    double               _miter_limit;
    int                  _flatness;

    struct {
      bool                 active;
      path_t               path;
      matrix_t             matrix;
    } _clip;

    std::vector<double>  _dash;
 };

 AggAbstractCanvas(double rx = 72.0, double ry = 72.0) : 
    _res_x( rx ), _res_y( ry )
  {
    if( _res_x <= 0.0 || _res_y <= 0.0)
    {
      std::cerr << "illegal resolution " << rx << ";" << ry << std::endl;
    }
    _scaling = AggMatrix::Scaling( rx / 72.0, ry / 72.0 );
  }
  
  virtual ~AggAbstractCanvas() {
  }

  virtual size_t getWidth()  const = 0;
  virtual size_t getHeight() const = 0;

  const matrix_t & getScaling() const {
    return _scaling;
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

  bool hasClip() const {
    return getNode()._clip.active;
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
  
  void setCap(gfxstate_t * state) {
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

  void setJoin(gfxstate_t * state) {
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
  
  void setClip(gfxstate_t * state) {
    getNode()._clip.path   = *(state->getPath());
    getNode()._clip.matrix = state->getCTM();
    getNode()._clip.active = true;
  }


  AggPath & getClipPath() {
    return getNode()._clip.path;
  }

  AggMatrix & getClipMatrix() {
    return getNode()._clip.matrix;
  }

  void setFlatness(gfxstate_t * state ) {
    getNode()._flatness = state->getFlatness();
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
  
  double getMinimalLineWidth() const  {
    return 72.0 / 
      (getResolutionY() < getResolutionX() ? getResolutionX() : getResolutionY() );
  }
  
  double getMiterLimit() {
    return getNode()._miter_limit;
  }

  int getFlatness() {
    return getNode()._miter_limit;
  }

  double getResolutionX() const {
    return _res_x;
  }
 
  double getResolutionY() const {
    return _res_y;
  }
  
  void setMiterLimit(double m) {
    getNode()._miter_limit=m;
  }


  void setResolutionX(double x)  {
    _res_x = x;
  }
  
  void setResolutionY(double y)  {
   _res_y = y;
 }
 
 void setResolution(double x, double y) {
   setResolutionX(x);
   setResolutionY(y);
 }
 
 void setLineWidth(gfxstate_t * state) {
   getNode()._line_width = state->getLineWidth();
 }

 virtual void setFillAlpha(gfxstate_t * state) = 0;
 virtual void setStrokeAlpha(gfxstate_t * state) = 0;
 
 virtual void setFillColor(GfxState * state)   = 0;
 virtual void setStrokeColor(GfxState * state) = 0;

 virtual void setFillColor(GfxState * state,double offset)   = 0;
 virtual void setStrokeColor(GfxState * state,double offset) = 0;

 virtual void push()         = 0;
 virtual void pop()          = 0;

 virtual GfxNode & getNode() = 0;
 virtual const GfxNode & getNode() const = 0;

 virtual void render( agg::rasterizer_scanline_aa<> & ras ) = 0;
 virtual void fill( agg::rasterizer_scanline_aa<> & ras ) = 0;
 virtual void fill( agg::rasterizer_scanline_aa<> & , agg::rasterizer_scanline_aa<> &  ) = 0;

 virtual bool writePpm(const std::string & fname) = 0;
 virtual bool writeTiff(const std::string & rFName) = 0;
 virtual void doBackgroundTest();

private:
  AggMatrix _scaling;
  AggPath   _clip_path;
  double    _res_x;
  double    _res_y;
};

/** @short Templated subclass with color model specific functions. Color model 
    specific primitives are further abstracted away into the AggColorTraits
    class.
*/

template<class COLOR,class COLORTRAITS=AggColorTraits< COLOR, GfxState > >
class BasicAggCanvas : public AggAbstractCanvas {

private:
  typedef AggAbstractCanvas                                super;
  typedef COLORTRAITS                                      traits_t;
  typedef typename traits_t::pixfmt_t                      pixfmt_t;
  typedef agg::renderer_base<pixfmt_t>                     renderer_base_t;
  typedef agg::renderer_scanline_aa_solid<renderer_base_t> renderer_sbool_t;
  typedef agg::renderer_scanline_aa_solid<renderer_base_t> renderer_solid_t;

  typedef typename traits_t::color_t    color_t;
  typedef typename traits_t::pixfmt_t   fmt_t;
  typedef typename traits_t::data_t     data_t;

public:
  typedef super::gfxstate_t gfxstate_t;

  struct GfxNode : public super::GfxNode
  {
    color_t        _fill_color;
    color_t        _stroke_color;
  };

  BasicAggCanvas(long w,long h,double _rx=72.0,double _ry=72.0) 
    : super(_rx,_ry),
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

  virtual void setFillAlpha( gfxstate_t * state ) override {
    traits_t::toAggAlpha(state->getFillColorSpace(),state->getFillOpacity(),_the_node._fill_color);
  }
  
  virtual void setStrokeAlpha( gfxstate_t * state ) override {
    traits_t::toAggAlpha(state->getStrokeColorSpace(),state->getStrokeOpacity(),_the_node._stroke_color);
  }

  virtual void setFillColor( gfxstate_t * state ) override {
    traits_t::toAggColor(state->getFillColorSpace(),state->getFillColor(),_the_node._fill_color);
  }
  
  virtual void setStrokeColor( gfxstate_t * state ) override {
    traits_t::toAggColor(state->getStrokeColorSpace(),state->getStrokeColor(),_the_node._stroke_color);
  }

  virtual void setFillColor( gfxstate_t * state,double offset ) override {
    // std::cerr << __PRETTY_FUNCTION__ << "::@" << offset << std::endl;
    traits_t::toAggColor(state->getFillColorSpace(),state->getFillColor(),_the_node._fill_color);
  }
  
  virtual void setStrokeColor( gfxstate_t * state,double offset ) override {
    traits_t::toAggColor(state->getStrokeColorSpace(),state->getStrokeColor(),_the_node._stroke_color);
  }

  virtual   
  const color_t getStrokeColor() const  {
    return _the_node._stroke_color;
  }

  virtual  
  const color_t getFillColor() const  {
   return _the_node._fill_color;
  }
  
  virtual  
  void render( agg::rasterizer_scanline_aa<> & ras ) override {
    agg::scanline_p8 sl;
    renderer_base_t  rbase( *getFmt() );
    renderer_solid_t rsolid(rbase);
    rsolid.color( getStrokeColor() );
    agg::render_scanlines(ras, sl, rsolid);
  }
  

  virtual  
  void fill( agg::rasterizer_scanline_aa<> & ras0,agg::rasterizer_scanline_aa<> & ras1) override {

    agg::scanline_p8  sl0;
    agg::scanline_p8  sl1;
    agg::scanline_p8  sl2;

    renderer_base_t   rbase( * getFmt() );
    
    typedef agg::renderer_scanline_aa_solid<renderer_base_t> sbool_renderer_type;
    sbool_renderer_type sren(rbase);

    sren.color( getFillColor() );

    agg::sbool_combine_shapes_aa(agg::sbool_and, ras0, ras1, sl0, sl1, sl2, sren);
  }
  
  virtual  
  void fill( agg::rasterizer_scanline_aa<> & ras0 ) override {

    renderer_base_t   rbase( * getFmt() );
    agg::scanline_p8  sl;

    // agg::render_scanlines_aa_solid(ras0, sl, rbase, getFillColor() );
    
    {
        gradient_polymorphic_wrapper<agg::gradient_radial>            gr_x;

        typedef agg::span_interpolator_linear<> interpolator_t;

        typedef agg::span_gradient< typename pixfmt_t::color_type,
                                   interpolator_t,
                                   gradient_polymorphic_wrapper_base,
                                   color_function_profile<color_t> > gradient_span_gen;

        typedef agg::span_allocator< typename gradient_span_gen::color_type > gradient_span_alloc_t;

        gradient_span_alloc_t    span_alloc;

        color_t color_profile[256]; // color_type is defined in pixel_formats.h

        int i;
        agg::int8u pr[256];
        for(i = 0; i < 256; i++)
        {
            color_profile[i] = color_t();
        }

        color_function_profile<color_t> colors(color_profile, pr );

        agg::trans_affine               mtx_g1;

        mtx_g1 *= agg::trans_affine_scaling(100,100);

        interpolator_t                  inter(mtx_g1);
        gradient_span_gen               span_gen(inter, gr_x, colors, 0, 150);

        agg::render_scanlines_aa(ras0, sl, rbase, span_alloc, span_gen );

        return;
    }
  }

  virtual  bool writePpm(const std::string & fname);
  virtual  bool writeTiff(const std::string & fname);

private:
  traits_t            _traits;
  pixfmt_t          * _fmt;
  GfxNode             _the_node;
  std::stack<GfxNode> _stack;
};

template<>
BasicAggCanvas< agg::cmyka, AggColorTraits< agg::cmyka, GfxState > >::~BasicAggCanvas();

typedef BasicAggCanvas<agg::cmyka> AggCmykCanvas;
typedef BasicAggCanvas<agg::rgba>  AggRgbCanvas;

#endif // AGGCANVAS_H
