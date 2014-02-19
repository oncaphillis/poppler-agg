//========================================================================
//
// AggOutputDev.cc
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

#include <config.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "AggOutputDev.h"
#include "AggGradient.h"

#ifdef USE_GCC_PRAGMAS
#pragma implementation
#endif

// static std::ostream & debug(std::cerr);
static std::ofstream  debug( "/dev/null" );

// RAII class which stores and restores an ostreams format state

class OsFmtStore {
private:
    
public:
    OsFmtStore(std::ostream & os) :
        _os(os),
        _st(NULL) {
        _st.copyfmt(_os);
    }

    ~OsFmtStore() {
        _os.copyfmt(_st);
    }

private:
    // The state we are supposed to (re|)store
    std::ostream & _os;
    std::ios       _st;
};


std::ostream & operator<<(std::ostream & os,const agg::cmyka & c) {
  OsFmtStore st(os);

  os << "c:" << std::fixed << std::setprecision(3) 
     << c.c << ";m:" << c.m << ";y:" << c.y << ";k:" << c.k << ";a:" << c.a; 
  
  return os;
}

std::ostream & operator<<(std::ostream & os,const agg::rgba & c) {
  OsFmtStore st(os);
  os << "r:" << std::fixed  << std::setprecision(3) 
     << c.r << ";g:" << c.g << ";b:" << c.b << ";a:" << c.a; 

  return os;
}

std::ostream & operator<<(std::ostream & os , const AggPoint & po) {
    OsFmtStore st(os);
    os << std::setw(9) << std::fixed << std::setprecision(3) << po.x << ";" << std::setw(9) << po.y;
    return os;
}

std::ostream & operator<<(std::ostream & os,const AggMatrix & m)
{
  OsFmtStore st(os);

  os  << std::fixed << std::setprecision(3) 
      << "(" 
      << "a:" << std::setw(9) << m.a << ";b:" << std::setw(9) << m.b << ";c:" << std::setw(9) << m.c << ";" 
      << "d:" << std::setw(9) << m.d << ";h:" << std::setw(9) << m.h << ";v:" << std::setw(9) << m.v
      << ")";

  return os;
}

//------------------------------------------------------------------------
// AggOutputDev
//------------------------------------------------------------------------

AggOutputDev::AggOutputDev( canvas_t * c) 
  : _canvas(c)
{
}

AggOutputDev::~AggOutputDev() {
}

GBool AggOutputDev::setCanvas( canvas_t * c) {
  _canvas = c;
  return gTrue;
}

void AggOutputDev::setTextPage(TextPage *text) {
  debug << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::startDoc(PDFDoc *docA) {
  debug << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::startPage(int pageNum, GfxState *state, XRef *xref) {
  debug << " >> " << __PRETTY_FUNCTION__ << std::endl;
  debug << " << " <<__PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::endPage() {
  debug << " >> " << __PRETTY_FUNCTION__ << std::endl;
  debug << " << " << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::saveState(GfxState *state) {
  debug << " >> " << __PRETTY_FUNCTION__ << std::endl;
  _canvas->push();
  debug << " << " << __PRETTY_FUNCTION__ << std::endl;
  
}

void AggOutputDev::restoreState(GfxState *state) {
  debug << " >> " << __PRETTY_FUNCTION__ << std::endl;
  _canvas->pop();
  debug << " << " << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::updateAll(GfxState *state) {
  debug << " **>> " << __PRETTY_FUNCTION__ << std::endl;
  updateLineDash(state);
  updateLineJoin(state);
  updateLineCap(state);
  updateLineWidth(state);
  updateFlatness(state);
  updateMiterLimit(state);
  updateFillColor(state);
  updateStrokeColor(state);
  updateFillOpacity(state);
  updateStrokeOpacity(state);
  updateBlendMode(state);
  debug << " **<< " << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::setDefaultCTM(double *ctm) {
  AggMatrix m(ctm);
  super::setDefaultCTM( m * _canvas->getScaling() );
  _canvas->setDefMatrix( m );
}

void AggOutputDev::updateCTM(GfxState *state, double m11, double m12,
				double m21, double m22,
				double m31, double m32) {
    AggMatrix m( m11,  m12, m21,  m22, m31,  m32);
    // std::cerr << " !! new CTM @" << AggMatrix(state->getCTM() ) << " =>" << m << std::endl;
    _canvas->setDefMatrix( m );
}

void AggOutputDev::updateLineDash(GfxState *state) {
    debug << " >> " << __PRETTY_FUNCTION__ << std::endl;
    double *dashPattern;
    int dashLength;
    double dashStart;
    std::vector<double> d;

    state->getLineDash(&dashPattern, &dashLength, &dashStart);

    if(dashLength>0)
    {
      d.push_back(dashStart);
      for(int i=0;i<dashLength;i++) {
        d.push_back(dashPattern[i]); 
      }
    }
    _canvas->setDash(d);
    debug << " >> " << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::updateFlatness(GfxState *state) {
  debug << " >> " << __PRETTY_FUNCTION__ << std::endl;
  _canvas->setFlatness(state);
  debug << " << " << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::updateLineJoin(GfxState *state) {

  debug << " >> " << __PRETTY_FUNCTION__ << std::endl;
  _canvas->setJoin(state);
  debug << " << " << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::updateLineCap(GfxState *state) {
  debug << " >> " << __PRETTY_FUNCTION__ << std::endl;
  _canvas->setCap(state);
  debug << " << " << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::updateMiterLimit(GfxState *state) {
  debug << " >> " << __PRETTY_FUNCTION__ << std::endl;
  _canvas->setMiterLimit(state->getMiterLimit());
  debug << " << " << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::updateLineWidth(GfxState *state) {
  debug << ">> " << __PRETTY_FUNCTION__ << std::endl;
  _canvas->setLineWidth(state);
  debug << "<< " << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::updateFillColor(GfxState *state) {
  debug << " >> " << __PRETTY_FUNCTION__ << std::endl;
  _canvas->setFillColor( state );
  debug << " << " << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::updateStrokeColor(GfxState *state) {
  debug << " >> "<< __PRETTY_FUNCTION__ << std::endl;
  _canvas->setStrokeColor( state );
  debug << " << "<< __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::updateFillOpacity(GfxState *state) {
  debug << " >> " << __PRETTY_FUNCTION__ << std::endl;
  _canvas->setFillAlpha( state );
  debug << " << " << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::updateStrokeOpacity(GfxState *state) {
  debug << " >> " << __PRETTY_FUNCTION__ << std::endl;
  _canvas->setStrokeAlpha( state );
  debug << " << " << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::updateFillColorStop(GfxState *state, double offset) {
  //debug << " >> " << __PRETTY_FUNCTION__ << std::endl;
  _canvas->setFillColor( state,offset );
  //debug << " << " << __PRETTY_FUNCTION__ << state->getFillColor() << std::endl;
}

void AggOutputDev::updateBlendMode(GfxState *state) {
  debug << " >> " << __PRETTY_FUNCTION__ << std::endl;
  debug << " << " << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::updateFont(GfxState *state) {
  debug << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::alignStrokeCoords(GfxSubpath *subpath, int i, double *x, double *y) {
  debug << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::stroke(GfxState *state) {
  AggPath p( state->getPath() );
  stroke(state, p);
}

void AggOutputDev::stroke(GfxState *state,  AggPath & p) {

  agg::conv_curve<agg::path_storage > curve(p);
  agg::rasterizer_scanline_aa<> ras;

  debug << " >> " << __PRETTY_FUNCTION__ << " @" << p->total_vertices() << std::endl;

  const std::vector<double> & da = _canvas->getDash();

  if(da.size()>0)
  {
      agg::conv_dash< agg::conv_curve <agg::path_storage> > d(curve);
      d.dash_start(da[0]);
      
      if(da.size()>=3 && (da.size() & 0x01) != 0)
      {
        for(size_t i=1;i<da.size();i+=2)
        {
          d.add_dash(da[i], da[i+1]);
        }
      }
      else
      {
        debug << "illegal dash size #" << da.size() << std::endl;
      }
      
      agg::conv_stroke< agg::conv_dash< agg::conv_curve <agg::path_storage > >  > stroke2(d);
      stroke2.line_cap( _canvas->getCap());
      stroke2.line_join( _canvas->getJoin() );
      stroke2.miter_limit( _canvas->getMiterLimit() );
      stroke2.width(   _canvas->getLineWidth() == 0 ? 
                       _canvas->getMinimalLineWidth() : _canvas->getLineWidth());
      
      matrix_t m2 = matrix_t(state->getCTM()) * _canvas->getScaling();
  
      agg::conv_transform<agg::conv_stroke<agg::conv_dash<agg::conv_curve<agg::path_storage > > > >  trans( stroke2,  m2 );
                 
      ras.add_path(trans);
      _canvas->render( ras );

    }
    else
    {
        agg::conv_stroke<agg::conv_curve <agg::path_storage > > line(curve);
    
        line.line_cap( _canvas->getCap() );
        line.line_join( _canvas->getJoin() );
        line.miter_limit( _canvas->getMiterLimit() );
        line.width(  _canvas->getLineWidth() == 0 ? 
                     _canvas->getMinimalLineWidth() : _canvas->getLineWidth());
        

        matrix_t m2 = matrix_t(state->getCTM()) * _canvas->getScaling();

        agg::conv_transform< agg::conv_stroke<agg::conv_curve <agg::path_storage > >  >
            trans(line,m2 );
  
        ras.add_path(trans);
        _canvas->render( ras );
    }
  
  debug << " << " << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::eoFill(GfxState *state) {
  debug << " << " << __PRETTY_FUNCTION__ << std::endl;
  _fill(state,true);
}

void AggOutputDev::fill(GfxState *state) {
  debug << " << " << __PRETTY_FUNCTION__ << std::endl;
  _fill(state,false);
}

void AggOutputDev::_fill(GfxState *state,bool eo) {

  agg::rasterizer_scanline_aa<> r;
  debug << " << " << __PRETTY_FUNCTION__ << std::endl;

  {
    AggPath   p0 = path_t(state->getPath());
    AggMatrix m  = AggMatrix(state->getCTM() * _canvas->getScaling());

    typedef agg::conv_transform< AggPath::agg_t> trans_t;
    typedef agg::conv_curve<trans_t >            curve_t;
    typedef agg::conv_contour< curve_t>          contour_t;
    typedef agg::conv_gpc<contour_t,trans_t>     gpc_t;

    trans_t    trans0( p0 , m );
    curve_t    curve(trans0);
    contour_t  contour(curve);
    if(_canvas->getNode()._clip.active) {
        debug << "WITH CLIP" << std::endl;
        trans_t    trans1( _canvas->getNode()._clip.path,
                           _canvas->getNode()._clip.matrix);
        gpc_t gpc(contour,trans1);
        gpc.operation(agg::gpc_and);
        r.reset();
        r.filling_rule(eo ? agg::fill_even_odd : agg::fill_non_zero );
        r.add_path(contour);
        _canvas->fill( r );
    } else {
        debug << "WITHOUT CLIP" << std::endl;
        r.add_path(contour);
        r.filling_rule(eo ? agg::fill_even_odd : agg::fill_non_zero );
        _canvas->fill( r );
    }
  }
}


GBool AggOutputDev::tilingPatternFill(GfxState *state, Gfx *gfx1, Catalog *cat, Object *str,
					double *pmat, int paintType, int /*tilingType*/, Dict *resDict,
					double *mat, double *bbox,
					int x0, int y0, int x1, int y1,
					double xStep, double yStep)
{
    debug << __PRETTY_FUNCTION__ << std::endl;
    return gTrue;
}

GBool AggOutputDev::axialShadedFill(GfxState *state, GfxAxialShading *shading, 
                                    double tMin, double tMax) {
    
  typedef agg::conv_transform< AggPath::agg_t> trans_t;
  typedef agg::conv_curve<trans_t >            curve_t;
  typedef agg::conv_contour< curve_t>          contour_t;

  agg::rasterizer_scanline_aa<> r;

  path_t   p0;
  matrix_t mp,mg;

  mg = matrix_t(state->getCTM());
  mp = _canvas->getNode()._clip.matrix * _canvas->getScaling();

  if(_canvas->getNode()._clip.active) {
      p0 = _canvas->getNode()._clip.path;
      mp = _canvas->getNode()._clip.matrix * _canvas->getScaling();
  }
  else
  {
      p0  = path_t(state->getPath());
      mp  = matrix_t(state->getCTM() * _canvas->getScaling());
  }

  trans_t    trans0( p0 , mp );
  curve_t    curve(trans0);
  contour_t  contour(curve);
  
  r.reset();

  double x0,y0,x1,y1;
  shading->getCoords(&x0,&y0,&x1,&y1);

  tMin = x0;
  tMax = x1;

  r.add_path(contour);
  _canvas->fill( r, shading, mg,tMin, tMax );

  return gTrue;
}

GBool AggOutputDev::axialShadedSupportExtend(GfxState *state, GfxAxialShading *shading) {
    debug << __PRETTY_FUNCTION__ << " " << (shading->getExtend0() == shading->getExtend1())
          << std::endl;
    return (shading->getExtend0() == shading->getExtend1());
}

GBool AggOutputDev::functionShadedFill(GfxState *,GfxFunctionShading *) {
    debug << __PRETTY_FUNCTION__ << std::endl;
    return gTrue;
}
#if 0
GBool AggOutputDev::functionShadedSupportExtend(GfxState *state, GfxFunctionShading *sh) {
    debug << __PRETTY_FUNCTION__ << std::endl;

    return gTrue;
}
#endif
GBool AggOutputDev::radialShadedFill(GfxState *state, GfxRadialShading *shading, 
                                     double sMin, double sMax) {
    debug << __PRETTY_FUNCTION__ << std::endl;
    return gTrue;
}

GBool AggOutputDev::radialShadedSupportExtend(GfxState *state, GfxRadialShading *shading) {
  debug << __PRETTY_FUNCTION__ << std::endl;
  return gTrue;
}

void AggOutputDev::clip(GfxState *state) {
  debug << " >> " <<  __PRETTY_FUNCTION__ << std::endl;
  _canvas->setClip(state);
  debug << " << " <<  __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::eoClip(GfxState *state) {
  debug << " >> " <<  __PRETTY_FUNCTION__ << std::endl;
  _canvas->setClip(state);
  debug << " << " <<  __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::clipToStrokePath(GfxState *state) {
  debug << __PRETTY_FUNCTION__ << std::endl;
  _canvas->setClip(state);
  debug << " << " <<  __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::fillToStrokePathClip(GfxState *state) {
  debug << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::beginString(GfxState *state, GooString *s)
{
  debug << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::drawChar(GfxState *state, double x, double y,
			      double dx, double dy,
			      double originX, double originY,
			      CharCode code, int nBytes, Unicode *u, int uLen)
{
  debug << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::endString(GfxState *state) {
  debug << __PRETTY_FUNCTION__ << std::endl;
}

GBool AggOutputDev::beginType3Char(GfxState *state, double x, double y,
				      double dx, double dy,
				      CharCode code, Unicode *u, int uLen) {
  debug << __PRETTY_FUNCTION__ << std::endl;
  return gTrue;
}

void AggOutputDev::endType3Char(GfxState *state) {
}

void AggOutputDev::type3D0(GfxState *state, double wx, double wy) {
    debug << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::type3D1(GfxState *state, double wx, double wy,
			     double llx, double lly, double urx, double ury) {
    debug << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::beginTextObject(GfxState *state) {
    debug << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::endTextObject(GfxState *state) {
    debug << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::beginActualText(GfxState *state, GooString *text) {
    debug << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::endActualText(GfxState *state) {
  debug << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::beginTransparencyGroup(GfxState * /*state*/, double * /*bbox*/,
                                      GfxColorSpace * blendingColorSpace,
                                      GBool /*isolated*/, GBool knockout,
				      GBool forSoftMask) {
  debug << ">> " << __PRETTY_FUNCTION__ << std::endl;
  debug << "<< " << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::endTransparencyGroup(GfxState * /*state*/) {
  debug << ">> " << __PRETTY_FUNCTION__ << std::endl;
  debug << "<< " << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::paintTransparencyGroup(GfxState * /*state*/, double * /*bbox*/) {
  debug << ">> " << __PRETTY_FUNCTION__ << std::endl;
  debug << "<< " << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::setSoftMask(GfxState * state, double * bbox, GBool alpha,
                                 Function * transferFunc, GfxColor * backdropColor) {
  debug << ">> " << __PRETTY_FUNCTION__ << std::endl;
  debug << "<< " << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::popTransparencyGroup() {
  debug << ">> " << __PRETTY_FUNCTION__ << std::endl;
  debug << "<< " << __PRETTY_FUNCTION__ << std::endl;
}


void AggOutputDev::clearSoftMask(GfxState * /*state*/) {
  debug << __PRETTY_FUNCTION__ << std::endl;
}


void AggOutputDev::getScaledSize(int  orig_width,
				   int  orig_height,
				   int *scaledWidth,
				   int *scaledHeight) {
  debug << __PRETTY_FUNCTION__ << std::endl;
}


void AggOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str,
				   int width, int height, GBool invert,
				   GBool interpolate, GBool inlineImg) {
  debug << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::setSoftMaskFromImageMask(GfxState *state, Object *ref, Stream *str,
				   int width, int height, GBool invert,
				   GBool inlineImg, double *baseMatrix) {

 debug << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::unsetSoftMaskFromImageMask(GfxState *state, double *baseMatrix) {
    debug << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::drawImageMaskRegular(GfxState *state, Object *ref, Stream *str,
					  int width, int height, GBool invert,
					  GBool interpolate, GBool inlineImg) {
    debug << __PRETTY_FUNCTION__ << std::endl;
}


void AggOutputDev::drawImageMaskPrescaled(GfxState *state, Object *ref, Stream *str,
					    int width, int height, GBool invert,
					    GBool interpolate, GBool inlineImg) {
  debug << __PRETTY_FUNCTION__ << " >> " << std::endl;
  debug << __PRETTY_FUNCTION__ << " << " << std::endl;
}

void AggOutputDev::drawMaskedImage(GfxState *state, Object *ref,
				     Stream *str, int width, int height,
				     GfxImageColorMap *colorMap,
				     GBool interpolate,
				     Stream *maskStr, int maskWidth,
				     int maskHeight, GBool maskInvert,
				     GBool maskInterpolate)
{
  debug << __PRETTY_FUNCTION__ << " >> " << std::endl;
  debug << __PRETTY_FUNCTION__ << " << " << std::endl;
}

void AggOutputDev::drawSoftMaskedImage(GfxState *state, Object *ref, Stream *str,
					 int width, int height,
					 GfxImageColorMap *colorMap,
					 GBool interpolate,
					 Stream *maskStr,
					 int maskWidth, int maskHeight,
					 GfxImageColorMap *maskColorMap,
					 GBool maskInterpolate) {
  debug << __PRETTY_FUNCTION__ << " >> " << std::endl;
  debug << __PRETTY_FUNCTION__ << " << " << std::endl;
}

GBool AggOutputDev::getStreamData (Stream *str, char **buffer, int *length) {
  debug << __PRETTY_FUNCTION__ << " >> " << std::endl;
  debug << __PRETTY_FUNCTION__ << " << " << std::endl;
  return gTrue;
}

void AggOutputDev::drawImage(GfxState *state, Object *ref, Stream *str,
			       int width, int height,
			       GfxImageColorMap *colorMap,
			       GBool interpolate,
			       int *maskColors, GBool inlineImg) {
  debug << __PRETTY_FUNCTION__ << " >> " << std::endl;
  debug << __PRETTY_FUNCTION__ << " << " << std::endl;
}

#if 0

//------------------------------------------------------------------------
// ImageOutputDev
//------------------------------------------------------------------------

AggImageOutputDev::AggImageOutputDev()
{
    debug << __PRETTY_FUNCTION__ << std::endl;
}

AggImageOutputDev::~AggImageOutputDev()
{
    debug << __PRETTY_FUNCTION__ << std::endl;
}


void AggImageOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str,
					int width, int height, GBool invert,
					GBool interpolate, GBool inlineImg) {
    debug << __PRETTY_FUNCTION__ << std::endl;
}

void AggImageOutputDev::drawImage(GfxState *state, Object *ref, Stream *str,
				    int width, int height, GfxImageColorMap *colorMap,
				    GBool interpolate, int *maskColors, GBool inlineImg) {
    debug << __PRETTY_FUNCTION__ << std::endl;
}

void AggImageOutputDev::drawSoftMaskedImage(GfxState *state, Object *ref, Stream *str,
					      int width, int height,
					      GfxImageColorMap *colorMap,
					      GBool interpolate,
					      Stream *maskStr,
					      int maskWidth, int maskHeight,
					      GfxImageColorMap *maskColorMap,
					      GBool maskInterpolate) {
    debug << __PRETTY_FUNCTION__ << std::endl;
}

void AggImageOutputDev::drawMaskedImage(GfxState *state, Object *ref, Stream *str,
					  int width, int height,
					  GfxImageColorMap *colorMap,
					  GBool interpolate,
					  Stream *maskStr,
					  int maskWidth, int maskHeight,
					  GBool maskInvert, GBool maskInterpolate) {
    debug << __PRETTY_FUNCTION__ << std::endl;
}
#endif
