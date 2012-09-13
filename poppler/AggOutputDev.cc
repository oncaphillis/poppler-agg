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

#ifdef USE_GCC_PRAGMAS
#pragma implementation
#endif

//------------------------------------------------------------------------
// AggOutputDev
//------------------------------------------------------------------------

AggOutputDev::AggOutputDev() 
  : _array(NULL),
    _render_buffer(NULL),
    _path_storage(NULL),
    _scale_x(0.0),
    _scale_y(0.0)
{
}

AggOutputDev::~AggOutputDev() {
  delete[] _array;
  delete   _render_buffer;
  delete   _pixfmt;
}

GBool AggOutputDev::setAgg(long w,long h,long rx,long ry) {
  long pw = (double) (w / 72.0) * rx;
  long ph = (double) (h / 72.0) * ry;

 
  if(_array!=NULL)
  {
    delete _array;
  }

  if(_render_buffer!=NULL)
  {
    delete _render_buffer;
  }

  if(_pixfmt!=NULL)
  {
    delete _pixfmt;
  }

  size_t s = pw * ph * 4;
  _array = new ubyte_t[ s ]; 

  ::memset(_array,0,s);

  _render_buffer = new rendering_buffer_t(_array, pw , ph , pw * 4);
  _path_storage  = new path_storage_t();
  _pixfmt        = new pixfmt_t(*_render_buffer);

  _scale_x = (double) 0.1 ; // rx / 72.0 ;
  _scale_y = (double) 0.1 ; // ry / 72.0 ;

  std::cerr << "w=" << w << "; h=" << h 
            << "; rx=" << rx << "; ry=" << ry 
            << " ==> (" << pw << ";" << ph << ") sx:" << _scale_x << " sy:" << _scale_y 
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
  std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::endPage() {
  std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::saveState(GfxState *state) {
  std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::restoreState(GfxState *state) {
  std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::updateAll(GfxState *state) {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::setDefaultCTM(double *ctm) {
  std::cerr << " >> " << __PRETTY_FUNCTION__ << std::endl;

  std::cerr << "(" 
            << ctm[0] << "; " << ctm[1] << "; " << ctm[2] << "; " 
            << ctm[3] << "; " << ctm[4] << "; " << ctm[5] 
            << ")" 
            << std::endl;
  
  std::cerr << " << " << __PRETTY_FUNCTION__ << std::endl;

}

void AggOutputDev::updateCTM(GfxState *state, double m11, double m12,
				double m21, double m22,
				double m31, double m32) {
  std::cerr << " >> " << __PRETTY_FUNCTION__ << std::endl;

  std::cerr << "(" 
            << m11    << "; " << m12    << "; " << m21    << "; " 
            << m22    << "; " << m31    << "; " << m32    
            << ")" 
            << std::endl;
  
  std::cerr << " << " <<__PRETTY_FUNCTION__ << std::endl;

}

void AggOutputDev::updateLineDash(GfxState *state) {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::updateFlatness(GfxState *state) {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::updateLineJoin(GfxState *state) {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::updateLineCap(GfxState *state) {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::updateMiterLimit(GfxState *state) {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::updateLineWidth(GfxState *state) {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::updateFillColor(GfxState *state) {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::updateStrokeColor(GfxState *state) {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
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
    std::cerr << " >> " << __PRETTY_FUNCTION__ << std::endl;
    _doPath (state, state->getPath(),_path_storage);
    std::cerr << " << " << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::fill(GfxState *state) {
  std::cerr << " >> " << __PRETTY_FUNCTION__ << std::endl;
  _doPath(state,state->getPath(), _path_storage);
  
  
  {
    agg::trans_affine mtx;
    agg::conv_transform<agg::path_storage> trans(*_path_storage,mtx);
    agg::conv_curve<agg::conv_transform<agg::path_storage> > curve(trans);
    agg::conv_contour
      <agg::conv_curve <agg::conv_transform  <agg::path_storage> > > contour(curve);
    
    agg::rasterizer_scanline_aa<> ras;
    agg::scanline_p8 sl;
    renderer_base_t rbase(*_pixfmt);
    
    ras.add_path(contour);
    agg::render_scanlines_aa_solid(ras, sl, rbase, agg::cmyk(0.0,0.0,0.0,1.0));
  }
  std::cerr << " << " << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::eoFill(GfxState *state) {
  std::cerr << " >> " << __PRETTY_FUNCTION__ << std::endl;
  _doPath(state,state->getPath(), _path_storage);
  std::cerr << " << " << __PRETTY_FUNCTION__ << std::endl;
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

void AggOutputDev::_clearPath( path_storage_t * agg_path) {
  std::cerr << "+C:** " ;
  agg_path->remove_all();
}

void AggOutputDev::_moveTo( path_storage_t * agg_path, double x,double y) {
  std::cerr << "+M:("  << x << ";" << y << ")" ;
  agg_path->move_to(x*_scale_x,y*_scale_y);
}

void AggOutputDev::_lineTo( path_storage_t * agg_path,double x, double y) {
  std::cerr << "+L:("  << x << ";" << y << ")" ;
  agg_path->line_to( x * _scale_x , y * _scale_y);
}

void AggOutputDev::_curveTo( path_storage_t * agg_path,double x0, double y0,double x1, double y1,double x2, double y2) {
    std::cerr << "+C:(["  << x0 << ";" << y0 << "][" << x1 << ";" << y1 << "][" << x2 << ";" << y2 <<"])" ;
    agg_path->curve4(x0 * _scale_x, y0 * _scale_y , x1 * _scale_x, y1 * _scale_y, x2 * _scale_x , y2*_scale_y);
}

void AggOutputDev::_closePath(path_storage_t * agg_path) {
    std::cerr << "+X"  << std::endl;
    agg_path->close_polygon();
}


void AggOutputDev::_alignStrokeCoords(GfxSubpath *subpath, int i, double *x, double *y){
}

void AggOutputDev::_doPath( GfxState *state, GfxPath *path, path_storage_t * agg_path ) {
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
                   subpath->getX(j), subpath->getY(j),
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

bool AggOutputDev::writePpm(const std::string & fname)
{
  agg::cmyka8 c;
  
  std::ofstream of(fname.c_str());
  if(of)
  {
    of << "P6" << std::endl 
       << "# Created by ARip" << std::endl
       << _pixfmt->width() << " " <<  _pixfmt->height() << " " << 255 << std::endl;
    
    for(size_t i = 0; i < _pixfmt->height(); ++i)
    {
      for(size_t j = 0; j < _pixfmt->width(); ++j)
      {
        c = (*_pixfmt).pixel(j,i);
        agg::rgba8 r = agg::to_cmyk(c).to_rgb();
        of << (char) r.r << (char) r.g << (char) r.b;
      }
    }
    return true;
  }
  return false;
}
