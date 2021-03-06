//========================================================================
//
// AggCanvas.h
//
// Copyright 2012-14 Sebastian Kloska (oncaphillis@snafu.de)
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

#include "AggPath.h"
#include "AggColorTraits.h"
#include "AggGradient.h"
#include "AggPoint.h"
#include "AggFontEngine.h"

#include "agg_conv_bspline.h"
#include "agg_conv_segmentator.h"
#include "agg_conv_contour.h"
#include "agg_conv_dash.h"
#include "agg_conv_gpc.h"
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_p.h"
#include "agg_scanline_boolean_algebra.h"
#include "agg_renderer_scanline.h"
#include "agg_pixfmt_rgb.h"
#include "agg_gamma_lut.h"
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
#include "agg_span_solid.h"

#include <vector>
#include <stack>
#include <map>
#include <memory>
#include <stdlib.h>

/** @short Base class for BasicAggCanvas<COLOR>. This aspect does not need to know anything about
    the concrete color type we are working with.
*/

class AggAbstractCanvas
{
public:
  typedef GfxState           gfxstate_t;
  typedef GfxState           gfxpath_t;
  typedef GfxFont            gfxfont_t;
  typedef AggMatrix          matrix_t;
  typedef AggPath            path_t;

  typedef agg::line_join_e   join_t;
  typedef agg::line_cap_e    cap_t;
  typedef agg::font_engine_freetype_int32 font_engine_t;

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

  AggPoint getSize() {
      return AggPoint(getWidth(),getHeight());
  }

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

 void setFont(gfxfont_t * font,const AggMatrix & m ) {
   _font_ptr = std::unique_ptr<AggFontEngine>(new AggFontEngine(this,*font,m));
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
 virtual void renderChar( agg::rasterizer_scanline_aa<> & ras,unsigned chr,GfxState *state,int x, int y) = 0;

 virtual void fill( agg::rasterizer_scanline_aa<> & r) = 0;
 virtual void fill( agg::rasterizer_scanline_aa<> & r, GfxAxialShading * , 
                    const matrix_t & m,double min, double max)  = 0;

 virtual void fill(agg::rasterizer_scanline_aa<> & r,
                   GfxRadialShading * sh,const matrix_t & m,double min,double max) = 0;

 virtual bool writePpm(const std::string & fname) = 0;
 virtual bool writeTiff(const std::string & rFName) = 0;

 AggFontEngine & getFont() {
     if(_font_ptr.get()==NULL) {
         throw std::runtime_error("font is undefined");
     }
     return *_font_ptr;
 }

private:
  AggMatrix _scaling;
  AggPath   _clip_path;
  std::unique_ptr<AggFontEngine> _font_ptr;
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

  typedef agg::renderer_scanline_aa_solid<renderer_base_t>  renderer_solid_t;
  typedef agg::renderer_scanline_bin_solid<renderer_base_t> renderer_bin_t;
  typedef super::font_engine_t font_engine_t;

  typedef typename traits_t::color_t    color_t;
  typedef typename traits_t::pixfmt_t   fmt_t;
  typedef typename traits_t::data_t     data_t;

public:
  typedef super::gfxstate_t      gfxstate_t;

  struct GfxNode : public super::GfxNode
  {
    GfxNode() :
        _fill_color(),
        _stroke_color()
    {
    }
    color_t _fill_color;
    color_t _stroke_color;
  };

  BasicAggCanvas(long w,long h,double _rx=72.0,double _ry=72.0) 
    : super(_rx,_ry),
      _traits(w,h),
      _fmt(_traits.fmt())
  {
  }

  virtual ~BasicAggCanvas() {
  }

  fmt_t * getFmt() {
    return _traits.fmt();
  }

  virtual size_t getWidth() const  override  {
    return _fmt->width();
  }

  virtual size_t getHeight() const  override  {
    return _fmt->height();
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
    traits_t::toAggAlpha(
                state->getFillColorSpace(),
                state->getFillOpacity(),_the_node._fill_color);
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

  virtual void setFillColor( gfxstate_t * state,double o ) override {
    traits_t::toAggColor(state->getFillColorSpace(),
                         state->getFillColor(),_the_node._fill_color );
  }

  virtual void setStrokeColor( gfxstate_t * state,double o ) override {
    traits_t::toAggColor(state->getFillColorSpace(),
                         state->getFillColor(),_the_node._stroke_color );
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
  void renderChar( agg::rasterizer_scanline_aa<> & ras,
                   unsigned chr,GfxState *state,int x,int y ) override {
      agg::scanline_p8 sl;

      renderer_base_t  rbase( *getFmt() );
      renderer_bin_t   rbin(rbase);
      renderer_solid_t rsolid( rbase );

      rbin.color( getFillColor() );
      rsolid.color( getFillColor() );

      getFont().render(chr,ras,sl,rbin,state,x,y);
  }
  
  /** Solid color fill.
   */

  virtual
  void fill( agg::rasterizer_scanline_aa<> & r ) override {
      
    renderer_base_t   rbase( * getFmt() );
    agg::scanline_p8  sl;

    typedef agg::span_solid< typename pixfmt_t::color_type >         span_color_t;
    typedef agg::span_allocator< typename span_color_t::color_type  >  span_alloc_t;

    span_alloc_t    span_alloc;

    span_color_t    span_gen;

    color_t c = getFillColor();
    c.alpha(0.5);
    span_gen.color(getFillColor());

    agg::render_scanlines_aa(r, sl, rbase, span_alloc, span_gen );

    return;
  }

  /** Do a radial fill. The GfxRadialShading object specifies a color range and two
   * points (start & stop) with two radius.
   */

  virtual void fill(agg::rasterizer_scanline_aa<> & ras,
                    GfxRadialShading * sh,const matrix_t & m,double min,double max) override {
      typedef AggRadialGradient<traits_t> gradient_t;
      typedef agg::span_interpolator_linear<> interpolator_t;
      typedef agg::span_gradient<typename pixfmt_t::color_type,
                                 interpolator_t,
                                 gradient_t,
                                 typename gradient_t::color_range_t > span_gen_t;
      typedef agg::span_allocator< typename span_gen_t::color_type > gradient_span_alloc_t;

      renderer_base_t rbase( * getFmt() );

      agg::scanline_p8  sl;

      gradient_t gr(*sh,sh->getDomain0(),sh->getDomain1());
      
      // Start and end point transformed into scaled space

      AggPoint p0;
      AggPoint p1;
      AggPoint::coord_t r0;
      AggPoint::coord_t r1;

      gr.getCoords(p0,p1,r0,r1);

      //matrix_t cm = getNode()._clip.active ? getNode()._clip.matrix : matrix_t();
      matrix_t mg = (matrix_t::Translation(p0) *  m * this->getScaling() ).invert();

      std::cerr << " p0(" << p0 << ") p1(" << p1 << ") r0(" << r0 << ") r1(" << r1 << ")" << std::endl;

      interpolator_t inter( mg );
      
      span_gen_t span_gen(inter, gr, gr.getColorRange(), 0, r1 );

      gradient_span_alloc_t    span_alloc;

      agg::render_scanlines_aa(ras, sl, rbase, span_alloc, span_gen );

  }
  /** Linear gradient fill. Gradient is specified as GfxAxialShading object. Defining
   *  a color range, a transformation function , start and end points and a range in which
   *  to apply coloring.
   */

  virtual void fill(agg::rasterizer_scanline_aa<> & r,
                      GfxAxialShading * sh,const matrix_t & m,double min,double max ) override { 
 
 
        typedef AggLinearGradient< traits_t > gradient_t;
        typedef agg::span_interpolator_linear<> interpolator_t;                         
        typedef agg::span_gradient< typename pixfmt_t::color_type, 
                                    interpolator_t, 
                                    gradient_t, 
                                    typename gradient_t::color_range_t > span_gen_t; 
        typedef agg::span_allocator< typename span_gen_t::color_type  >  gradient_span_alloc_t; 

        renderer_base_t   rbase( * getFmt() );

        agg::scanline_p8  sl; 

        gradient_t gr(*sh,sh->getDomain0(),sh->getDomain1()); 
 
        // Start and end point transformed into scaled space
        AggPoint p0;
        AggPoint p1; 
 
        gr.getCoords(p0,p1);

        // matrix_t cm = getNode()._clip.active ? getNode()._clip.matrix : matrix_t();

        matrix_t mg = (matrix_t::Rotation( gr.getAngle() ).translate(p0) * m * this->getScaling() ).invert();

        interpolator_t inter( mg ); 

        // Translate the distance into range space
        // double d=(p1 * cm.invert()).getDistance(p0*cm.invert()); 

        double d=(p1).getDistance(p0); 

        span_gen_t span_gen(inter, gr, gr.getColorRange(), 0, d);
     
        gradient_span_alloc_t    span_alloc; 
        
        agg::render_scanlines_aa(r, sl, rbase, span_alloc, span_gen ); 
    }  

  virtual bool writePpm(const std::string & fname);
  virtual bool writeTiff(const std::string & fname);

private:
  traits_t            _traits;
  pixfmt_t          * _fmt;
  GfxNode             _the_node;
  std::stack<GfxNode> _stack;
};

template<>
BasicAggCanvas< agg::cmyka, AggColorTraits< agg::cmyka, GfxState > >::~BasicAggCanvas();

typedef BasicAggCanvas<agg::cmyka>      AggCmykCanvas;

// typedef BasicAggCanvas<agg::device_na<5>> AggCmykCanvas;

typedef BasicAggCanvas<agg::rgba>  AggRgbCanvas;

#endif // AGGCANVAS_H
