//========================================================================
//
// AggOutputDev.cc
//
// Copyright 2003 Sebstian Kloaka
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
#include <fstream>
#include "AggOutputDev.h"
#include "AggMatrix.h"

#ifdef USE_GCC_PRAGMAS
#pragma implementation
#endif

std::ostream & operator<<(std::ostream & os,const agg::cmyk & c) {
 return os << "c:" << c.c << ";m:" << c.m << ";y:" << c.y << ";k:" << c.k; 
}

std::ostream & operator<<(std::ostream & os,const agg::rgba & c) {
 return os << "r:" << c.r << ";g:" << c.g << ";b:" << c.b << ";a:" << c.a; 
}

std::ostream & operator<<(std::ostream & os,const AggMatrix & m)
{
  os << "(" 
     << "a:" << m.a << ";" << "b:" << m.b << ";" <<  "c:" << m.c << ";" << "d:" << m.d 
     << ";h:" << m.h << ";" << "v:" << m.v
     << ")";
  return os;
}


//------------------------------------------------------------------------
// AggOutputDev
//------------------------------------------------------------------------

AggOutputDev::AggOutputDev() 
  : _canvas(NULL)
{
}

AggOutputDev::~AggOutputDev() {
  delete   _canvas;
}

GBool AggOutputDev::setAgg(long w,long h,long rx,long ry) {

  // dim in pixel

  long pw = (double) (w / 72.0) * rx ;
  long ph = (double) (h / 72.0) * ry ;

  delete _canvas;

  _canvas        = new AggRgbCanvas(pw,ph);

  _canvas->setScaling(matrix_t(agg::trans_affine_scaling( rx / 72.0, ry / 72.0)));
  _canvas->setResolution(rx,ry);
  _canvas->setDefMatrix(AggMatrix());
                        
  std::cerr << "w=" << w << "; h=" << h 
            << "; rx=" << rx << "; ry=" << ry 
            << " ==> (" << pw << ";" << ph << ") "
            << std::endl;
  
  return gTrue;
}

void AggOutputDev::setTextPage(TextPage *text)
{
}

void AggOutputDev::startDoc(PDFDoc *docA) {
  std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::startPage(int pageNum, GfxState *state) {
  std::cerr << " >> " << __PRETTY_FUNCTION__ << std::endl;
  if(state!=NULL)
  {
      _canvas->setDefMatrix(AggMatrix(state->getCTM()));
  }
  std::cerr << " << " <<__PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::endPage() {
  std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::saveState(GfxState *state) {
  std::cerr << " >> " << __PRETTY_FUNCTION__ << std::endl;
  _canvas->push();
  std::cerr << " << " << __PRETTY_FUNCTION__ << std::endl;
  
}

void AggOutputDev::restoreState(GfxState *state) {
  std::cerr << " >> " << __PRETTY_FUNCTION__ << std::endl;
  _canvas->pop();
  std::cerr << " << " << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::updateAll(GfxState *state) {
    std::cerr << " >> " << __PRETTY_FUNCTION__ << std::endl;
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
    std::cerr << " << " << __PRETTY_FUNCTION__ << std::endl;
    
}

void AggOutputDev::setDefaultCTM(double *ctm) {

    /*  std::cerr << "(" 
        << ctm[0] << "; " << ctm[1] << "; " << ctm[2] << "; " 
        << ctm[3] << "; " << ctm[4] << "; " << ctm[5] 
        << ")" 
        << std::endl;
        
        _canvas->setDefMatrix( _canvas->getDefMatrix() * matrix_t(ctm[0],ctm[1],ctm[2],ctm[3],ctm[4],ctm[5])  );
    */

  std::cerr << " >> " << __PRETTY_FUNCTION__ << std::endl;
  std::cerr << AggMatrix(ctm) << std::endl;
  super::setDefaultCTM( (AggMatrix(ctm) * _canvas->getScaling()).ToArray() );
  std::cerr << " << " << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::updateCTM(GfxState *state, double m11, double m12,
				double m21, double m22,
				double m31, double m32) {

  std::cerr << " >> " << __PRETTY_FUNCTION__ << std::endl;
  
  if(state!=NULL)
  {
      std::cerr << " ###// " << AggMatrix(state->getCTM()) << " ///###" << std::endl;
      _canvas->setDefMatrix( AggMatrix(state->getCTM() ) );
  }

  //  _canvas->setCTM(   matrix_t( m11,m12,m21,m22,m31,m32) *_canvas->getDefMatrix());
  // std::cerr << "2:(" 
  // << m11    << "; " << m12    << "; " << m21    << "; " 
  // << m22    << "; " << m31    << "; " << m32    
  // << ")" 
  // << std::endl;

  std::cerr << " << " <<__PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::updateLineDash(GfxState *state) {
    std::cerr << " >> " << __PRETTY_FUNCTION__ << std::endl;
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

    std::cerr << ")" << std::endl;
    std::cerr << " >> " << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::updateFlatness(GfxState *state) {
  std::cerr << " >> " << __PRETTY_FUNCTION__ << std::endl;
  std::cerr << " << " << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::updateLineJoin(GfxState *state) {

  std::cerr << " >> " << __PRETTY_FUNCTION__ << std::endl;
  _canvas->setJoin(state);
  std::cerr << " << " << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::updateLineCap(GfxState *state) {
  std::cerr << " >> " << __PRETTY_FUNCTION__ << std::endl;
  _canvas->setCap(state);
  std::cerr << " << " << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::updateMiterLimit(GfxState *state) {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::updateLineWidth(GfxState *state) {
  std::cerr << ">> " << __PRETTY_FUNCTION__ << std::endl;
  _canvas->setLineWidth(state);
  std::cerr << "<< " << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::updateFillColor(GfxState *state) {
  std::cerr << " >> " << __PRETTY_FUNCTION__ << std::endl;
  _canvas->setFillColor( state );
  std::cerr << " << " << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::updateStrokeColor(GfxState *state) {
  std::cerr << " >> "<< __PRETTY_FUNCTION__ << std::endl;
  _canvas->setStrokeColor( state );
  std::cerr << " << "<< __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::updateFillOpacity(GfxState *state) {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::updateStrokeOpacity(GfxState *state) {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::updateFillColorStop(GfxState *state, double offset) {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::updateBlendMode(GfxState *state) {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::updateFont(GfxState *state) {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::alignStrokeCoords(GfxSubpath *subpath, int i, double *x, double *y)
{
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::stroke(GfxState *state) {

  path_storage_t p;

  _doPath (state, state->getPath(), p );

  agg::conv_curve<agg::path_storage > curve(p);
  
  agg::rasterizer_scanline_aa<> ras;
  
  {
    {
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
          std::cerr << "illegal dash size #" << da.size() << std::endl;
        }

        agg::conv_stroke< agg::conv_dash< agg::conv_curve <agg::path_storage > >  > stroke2(d);
        stroke2.line_cap( _canvas->getCap());
        stroke2.line_join( _canvas->getJoin() );
        stroke2.width(   _canvas->getLineWidth() == 0 ? 
                         _canvas->getMinimalLineWidth() : _canvas->getLineWidth());

        agg::conv_transform<agg::conv_stroke< agg::conv_dash< agg::conv_curve <agg::path_storage > >  > >
            trans( stroke2, AggMatrix(state->getCTM()) * _canvas->getScaling()  );
        ras.add_path(trans);

        _canvas->render( ras );

#if 0 // -- SK
        renderer_base_t  rbase( * _canvas->getFmt() );
        renderer_solid_t rsolid(rbase);
        rsolid.color( _canvas->getStrokeColor() );
        agg::render_scanlines(ras, sl, rsolid);
#endif
              
        return;
      }
    }

    agg::conv_stroke<agg::conv_curve <agg::path_storage > > line(curve);
    
    line.line_cap( _canvas->getCap() );
    line.line_join( _canvas->getJoin() );
    line.width(  _canvas->getLineWidth() == 0 ? 
                 _canvas->getMinimalLineWidth() : _canvas->getLineWidth());

    agg::conv_transform< agg::conv_stroke<agg::conv_curve <agg::path_storage > >  >
        trans(line,AggMatrix(state->getCTM()) * _canvas->getScaling()  );

    ras.add_path(trans);
    _canvas->render( ras );

#if 0 // SK
    renderer_base_t  rbase( * _canvas->getFmt() );
    renderer_solid_t rsolid(rbase);
    rsolid.color( _canvas->getStrokeColor() );
    agg::render_scanlines(ras, sl, rsolid);
#endif

  };

  std::cerr << " << " << __PRETTY_FUNCTION__ << std::endl;
}



void AggOutputDev::eoFill(GfxState *state) {
  std::cerr << " >> " << __PRETTY_FUNCTION__ << std::endl;
  _fill(state,true);
}

void AggOutputDev::fill(GfxState *state) {
  std::cerr << " >> " << __PRETTY_FUNCTION__ << std::endl;
  _fill(state,false);
}

void AggOutputDev::_fill(GfxState *state,bool eo) {

  std::cerr << " >> " << __PRETTY_FUNCTION__ << std::endl;
  std::cerr << " CTM#0 == {" <<  _canvas->getTotalCTM()  << "}" << std::endl;
  std::cerr << " CTM#1 == {" <<  AggMatrix(state->getCTM())  << "}" << std::endl;

  path_storage_t p;
  _doPath(state,state->getPath(), p);
  
  {
      agg::conv_transform<agg::path_storage> trans( p,  
           AggMatrix(state->getCTM())  * _canvas->getScaling()   );//  _canvas->getTotalCTM() );
    agg::conv_curve<agg::conv_transform<agg::path_storage> > curve(trans);

    agg::conv_contour
      <agg::conv_curve <agg::conv_transform  <agg::path_storage> > > contour(curve);
    
    agg::rasterizer_scanline_aa<> ras;

    ras.add_path(contour);
    ras.filling_rule(eo ? agg::fill_even_odd : agg::fill_non_zero );
    _canvas->fill( ras );
#if 0 // -- SK
    agg::scanline_p8 sl;
    renderer_base_t  rbase( * _canvas->getFmt() );
    agg::render_scanlines_aa_solid(ras, sl, rbase,     _canvas->getFillColor() );
#endif

  };
}


GBool AggOutputDev::tilingPatternFill(GfxState *state, Gfx *gfx1, Catalog *cat, Object *str,
					double *pmat, int paintType, int /*tilingType*/, Dict *resDict,
					double *mat, double *bbox,
					int x0, int y0, int x1, int y1,
					double xStep, double yStep)
{
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
    return gTrue;
}

GBool AggOutputDev::axialShadedFill(GfxState *state, GfxAxialShading *shading, double tMin, double tMax) {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
    return gTrue;
}

GBool AggOutputDev::axialShadedSupportExtend(GfxState *state, GfxAxialShading *shading)
{
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
  return (shading->getExtend0() == shading->getExtend1());
}

GBool AggOutputDev::radialShadedFill(GfxState *state, GfxRadialShading *shading, double sMin, double sMax) {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
    return gTrue;
}

GBool AggOutputDev::radialShadedSupportExtend(GfxState *state, GfxRadialShading *shading)
{
  std::cerr << __PRETTY_FUNCTION__ << std::endl;
  return gTrue;
}



void AggOutputDev::clip(GfxState *state) {
  std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::eoClip(GfxState *state) {
  std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::clipToStrokePath(GfxState *state) {
  std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::fillToStrokePathClip(GfxState *state) {
  std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::beginString(GfxState *state, GooString *s)
{
  std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::drawChar(GfxState *state, double x, double y,
			      double dx, double dy,
			      double originX, double originY,
			      CharCode code, int nBytes, Unicode *u, int uLen)
{
  std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::endString(GfxState *state)
{
  std::cerr << __PRETTY_FUNCTION__ << std::endl;
}


GBool AggOutputDev::beginType3Char(GfxState *state, double x, double y,
				      double dx, double dy,
				      CharCode code, Unicode *u, int uLen) {
  std::cerr << __PRETTY_FUNCTION__ << std::endl;
  return gTrue;
}

void AggOutputDev::endType3Char(GfxState *state) {
}

void AggOutputDev::type3D0(GfxState *state, double wx, double wy) {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::type3D1(GfxState *state, double wx, double wy,
			     double llx, double lly, double urx, double ury) {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::beginTextObject(GfxState *state) {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::endTextObject(GfxState *state) {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::beginActualText(GfxState *state, GooString *text)
{
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::endActualText(GfxState *state)
{
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::beginTransparencyGroup(GfxState * /*state*/, double * /*bbox*/,
                                      GfxColorSpace * blendingColorSpace,
                                      GBool /*isolated*/, GBool knockout,
				      GBool forSoftMask) {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::endTransparencyGroup(GfxState * /*state*/) {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::paintTransparencyGroup(GfxState * /*state*/, double * /*bbox*/) {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

/* XXX: do we need to deal with shape here? */
void AggOutputDev::setSoftMask(GfxState * state, double * bbox, GBool alpha,
                                 Function * transferFunc, GfxColor * backdropColor) {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::popTransparencyGroup() {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}


void AggOutputDev::clearSoftMask(GfxState * /*state*/) {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}


void AggOutputDev::getScaledSize(int  orig_width,
				   int  orig_height,
				   int *scaledWidth,
				   int *scaledHeight) {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}


void AggOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str,
				   int width, int height, GBool invert,
				   GBool interpolate, GBool inlineImg) {

    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::setSoftMaskFromImageMask(GfxState *state, Object *ref, Stream *str,
				   int width, int height, GBool invert,
				   GBool inlineImg, double *baseMatrix) {

    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::unsetSoftMaskFromImageMask(GfxState *state, double *baseMatrix) {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::drawImageMaskRegular(GfxState *state, Object *ref, Stream *str,
					  int width, int height, GBool invert,
					  GBool interpolate, GBool inlineImg) {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}


void AggOutputDev::drawImageMaskPrescaled(GfxState *state, Object *ref, Stream *str,
					    int width, int height, GBool invert,
					    GBool interpolate, GBool inlineImg) {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::drawMaskedImage(GfxState *state, Object *ref,
				     Stream *str, int width, int height,
				     GfxImageColorMap *colorMap,
				     GBool interpolate,
				     Stream *maskStr, int maskWidth,
				     int maskHeight, GBool maskInvert,
				     GBool maskInterpolate)
{
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}


//XXX: is this affect by AIS(alpha is shape)?
void AggOutputDev::drawSoftMaskedImage(GfxState *state, Object *ref, Stream *str,
					 int width, int height,
					 GfxImageColorMap *colorMap,
					 GBool interpolate,
					 Stream *maskStr,
					 int maskWidth, int maskHeight,
					 GfxImageColorMap *maskColorMap,
					 GBool maskInterpolate)
{
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

GBool AggOutputDev::getStreamData (Stream *str, char **buffer, int *length)
{
    std::cerr << __PRETTY_FUNCTION__ << std::endl;

    return gTrue;
}

void AggOutputDev::drawImage(GfxState *state, Object *ref, Stream *str,
			       int width, int height,
			       GfxImageColorMap *colorMap,
			       GBool interpolate,
			       int *maskColors, GBool inlineImg)
{
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

#if 0
//------------------------------------------------------------------------
// ImageOutputDev
//------------------------------------------------------------------------

AggImageOutputDev::AggImageOutputDev()
{
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

AggImageOutputDev::~AggImageOutputDev()
{
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}


void AggImageOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str,
					int width, int height, GBool invert,
					GBool interpolate, GBool inlineImg)
{
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggImageOutputDev::drawImage(GfxState *state, Object *ref, Stream *str,
				    int width, int height, GfxImageColorMap *colorMap,
				    GBool interpolate, int *maskColors, GBool inlineImg)
{
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggImageOutputDev::drawSoftMaskedImage(GfxState *state, Object *ref, Stream *str,
					      int width, int height,
					      GfxImageColorMap *colorMap,
					      GBool interpolate,
					      Stream *maskStr,
					      int maskWidth, int maskHeight,
					      GfxImageColorMap *maskColorMap,
					      GBool maskInterpolate)
{
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggImageOutputDev::drawMaskedImage(GfxState *state, Object *ref, Stream *str,
					  int width, int height,
					  GfxImageColorMap *colorMap,
					  GBool interpolate,
					  Stream *maskStr,
					  int maskWidth, int maskHeight,
					  GBool maskInvert, GBool maskInterpolate)
{
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}
#endif

void AggOutputDev::_clearPath( path_storage_t & agg_path ) {
  std::cerr << "+C:** " ;
  agg_path.remove_all();
}

void AggOutputDev::_moveTo( path_storage_t & agg_path, double x,double y) {
  std::cerr << "+M:("  << x << ";" << y << ")" ;
  agg_path.move_to(x,y);
}

void AggOutputDev::_lineTo( path_storage_t & agg_path,double x, double y) {
  std::cerr << "+L:("  << x << ";" << y << ")" ;
  agg_path.line_to( x,y);
}

void AggOutputDev::_curveTo( path_storage_t & agg_path,double x0, double y0,double x1, double y1,double x2, double y2) {
  std::cerr << "+C:(["  << x0 << ";" << y0 << "][" << x1 << ";" << y1 << "][" << x2 << ";" << y2 <<"])" ;
  agg_path.curve4(x0 , y0, x1 , y1, x2 , y2);
}

void AggOutputDev::_closePath(path_storage_t & agg_path) {
  std::cerr << "+X"  << std::endl;
  agg_path.close_polygon();
}


void AggOutputDev::_doPath( GfxState *state, GfxPath *path, path_storage_t & agg_path ) {
  GfxSubpath *subpath;
  int i, j;
  double x, y;
  _clearPath(agg_path);
  for (i = 0; i < path->getNumSubpaths(); ++i) {
    subpath = path->getSubpath(i);
    if (subpath->getNumPoints() > 0) {
      if (align_stroke_coords) {
        alignStrokeCoords(subpath, 0, &x, &y);
      } else {
        x = subpath->getX(0);
        y = subpath->getY(0);
      }

      _moveTo (agg_path, x, y);

      j = 1;
      while (j < subpath->getNumPoints()) {
	if (subpath->getCurve(j)) {
	  if (align_stroke_coords) {
            alignStrokeCoords(subpath, j + 2, &x, &y);
          } else {
            x = subpath->getX(j+2);
            y = subpath->getY(j+2);
          }
	  _curveTo(agg_path,
                   subpath->getX(j),   subpath->getY(j),
                   subpath->getX(j+1), subpath->getY(j+1),
                   x, y);

	  j += 3;
	} else {
	  if (align_stroke_coords) {
            alignStrokeCoords(subpath, j, &x, &y);
          } else {
            x = subpath->getX(j);
            y = subpath->getY(j);
          }
          _lineTo (agg_path,x, y);
	  ++j;
	}
      }
      if (subpath->isClosed()) {
	_closePath(agg_path);
      }
    }
  }
}

