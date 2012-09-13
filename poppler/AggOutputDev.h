//========================================================================
//
// AggOutputDev.h
//
// Copyright 2004 Sebastian Kloska
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

#ifndef AGGOUTPUTDEV_H
#define AGGOUTPUTDEV_H

#include "goo/gtypes.h"
#include "OutputDev.h"
#include "TextOutputDev.h"
#include "GfxState.h"

#include "agg_conv_bspline.h"
#include "agg_conv_segmentator.h"
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
#include "ctrl/agg_polygon_ctrl.h"

class PDFDoc;
class GfxState;
class GfxPath;
class Gfx8BitFont;
struct GfxRGB;
class CairoFontEngine;
class CairoFont;

//------------------------------------------------------------------------
// CairoOutputDev
//------------------------------------------------------------------------

class AggOutputDev: public OutputDev {

private:
  typedef agg::pixfmt_cmyk32            PIXFMT;
  typedef PIXFMT                        pixfmt_t;
  typedef agg::path_storage             path_storage_t;
  typedef agg::rendering_buffer         rendering_buffer_t;
  typedef agg::renderer_base<PIXFMT>    renderer_base_t;

public:

  typedef unsigned char     ubyte_t;
  typedef agg::trans_affine matrix_t;

  // Constructor.
  AggOutputDev();

  // Destructor.
  virtual ~AggOutputDev();

  // Set the ultimate target of drawing operations i.e. an array of appropriately dimensioned  bytes etc.
  virtual GBool setAgg(long width,long height,long resx,long resy);

  //----- get info about output device

  // Does this device use upside-down coordinates?
  // (Upside-down means (0,0) is the top left corner of the page.)
  virtual GBool upsideDown() { return gTrue; }

  // Does this device use drawChar() or drawString()?
  virtual GBool useDrawChar() { return gTrue; }

  // Does this device use tilingPatternFill()?  If this returns false,
  // tiling pattern fills will be reduced to a series of other drawing
  // operations.
  virtual GBool useTilingPatternFill() { return gTrue; }

  // Does this device use FillColorStop()?
  virtual GBool useFillColorStop() { return gTrue; }

  // Does this device use beginType3Char/endType3Char?  Otherwise,
  // text in Type 3 fonts will be drawn with drawChar/drawString.
  virtual GBool interpretType3Chars() { return gFalse; }

  //----- initialization and control

  // Start a page.
  virtual void startPage(int pageNum, GfxState *state);

  // End a page.
  virtual void endPage();

  //----- save/restore graphics state
  virtual void saveState(GfxState *state);
  virtual void restoreState(GfxState *state);

  //----- update graphics state
  virtual void updateAll(GfxState *state);
  virtual void setDefaultCTM(double *ctm);
  virtual void updateCTM(GfxState *state, double m11, double m12,
			 double m21, double m22, double m31, double m32);
  virtual void updateLineDash(GfxState *state);
  virtual void updateFlatness(GfxState *state);
  virtual void updateLineJoin(GfxState *state);
  virtual void updateLineCap(GfxState *state);
  virtual void updateMiterLimit(GfxState *state);
  virtual void updateLineWidth(GfxState *state);
  virtual void updateFillColor(GfxState *state);
  virtual void updateStrokeColor(GfxState *state);
  virtual void updateFillOpacity(GfxState *state);
  virtual void updateStrokeOpacity(GfxState *state);
  virtual void updateFillColorStop(GfxState *state, double offset);
  virtual void updateBlendMode(GfxState *state);

  //----- update text state
  virtual void updateFont(GfxState *state);

  //----- path painting
  virtual void stroke(GfxState *state);
  virtual void fill(GfxState *state);
  virtual void eoFill(GfxState *state);
  virtual void clipToStrokePath(GfxState *state);
  virtual GBool tilingPatternFill(GfxState *state, Gfx *gfx, Catalog *cat, Object *str,
				  double *pmat, int paintType, int tilingType, Dict *resDict,
				  double *mat, double *bbox,
				  int x0, int y0, int x1, int y1,
				  double xStep, double yStep);
  virtual GBool axialShadedFill(GfxState *state, GfxAxialShading *shading, double tMin, double tMax);
  virtual GBool axialShadedSupportExtend(GfxState *state, GfxAxialShading *shading);
  virtual GBool radialShadedFill(GfxState *state, GfxRadialShading *shading, double sMin, double sMax);
  virtual GBool radialShadedSupportExtend(GfxState *state, GfxRadialShading *shading);

  //----- path clipping
  virtual void clip(GfxState *state);
  virtual void eoClip(GfxState *state);

  //----- text drawing
  void beginString(GfxState *state, GooString *s);
  void endString(GfxState *state);
  void drawChar(GfxState *state, double x, double y,
		double dx, double dy,
		double originX, double originY,
		CharCode code, int nBytes, Unicode *u, int uLen);
  void beginActualText(GfxState *state, GooString *text);
  void endActualText(GfxState *state);

  virtual GBool beginType3Char(GfxState *state, double x, double y,
			       double dx, double dy,
			       CharCode code, Unicode *u, int uLen);
  virtual void endType3Char(GfxState *state);
  virtual void beginTextObject(GfxState *state);
  virtual void endTextObject(GfxState *state);

  //----- image drawing
  virtual void drawImageMask(GfxState *state, Object *ref, Stream *str,
			     int width, int height, GBool invert, GBool interpolate,
			     GBool inlineImg);
  virtual void setSoftMaskFromImageMask(GfxState *state,
					Object *ref, Stream *str,
					int width, int height, GBool invert,
					GBool inlineImg, double *baseMatrix);
  virtual void unsetSoftMaskFromImageMask(GfxState *state, double *baseMatrix);
  void drawImageMaskPrescaled(GfxState *state, Object *ref, Stream *str,
			      int width, int height, GBool invert, GBool interpolate,
			      GBool inlineImg);
  void drawImageMaskRegular(GfxState *state, Object *ref, Stream *str,
			    int width, int height, GBool invert, GBool interpolate,
			    GBool inlineImg);

  virtual void drawImage(GfxState *state, Object *ref, Stream *str,
			 int width, int height, GfxImageColorMap *colorMap,
			 GBool interpolate, int *maskColors, GBool inlineImg);
  virtual void drawSoftMaskedImage(GfxState *state, Object *ref, Stream *str,
				   int width, int height,
				   GfxImageColorMap *colorMap,
				   GBool interpolate,
				   Stream *maskStr,
				   int maskWidth, int maskHeight,
				   GfxImageColorMap *maskColorMap,
				   GBool maskInterpolate);

  virtual void drawMaskedImage(GfxState *state, Object *ref, Stream *str,
			       int width, int height,
			       GfxImageColorMap *colorMap,
			       GBool interpolate,
			       Stream *maskStr,
			       int maskWidth, int maskHeight,
			       GBool maskInvert, GBool maskInterpolate);

  //----- transparency groups and soft masks
  virtual void beginTransparencyGroup(GfxState * /*state*/, double * /*bbox*/,
                                      GfxColorSpace * /*blendingColorSpace*/,
                                      GBool /*isolated*/, GBool /*knockout*/,
                                      GBool /*forSoftMask*/);
  virtual void endTransparencyGroup(GfxState * /*state*/);
  void popTransparencyGroup();
  virtual void paintTransparencyGroup(GfxState * /*state*/, double * /*bbox*/);
  virtual void setSoftMask(GfxState * /*state*/, double * /*bbox*/, GBool /*alpha*/,
                           Function * /*transferFunc*/, GfxColor * /*backdropColor*/);
  virtual void clearSoftMask(GfxState * /*state*/);

  //----- Type 3 font operators
  virtual void type3D0(GfxState *state, double wx, double wy);
  virtual void type3D1(GfxState *state, double wx, double wy,
      double llx, double lly, double urx, double ury);

  //----- special access
  
  // Called to indicate that a new PDF document has been loaded.
  void startDoc(PDFDoc *docA);
 
  GBool isReverseVideo() { return gFalse; }
  
  void setTextPage (TextPage *text);

  bool writePpm(const std::string & fname);

protected:
  void getScaledSize(int orig_width, int orig_height,
		     int *scaledWidth, int *scaledHeight);

  GBool getStreamData (Stream *str, char **buffer, int *length);
  void fillToStrokePathClip(GfxState *state);
  void alignStrokeCoords(GfxSubpath *subpath, int i, double *x, double *y);

  GfxRGB fill_color, stroke_color;
  double fill_opacity;
  double stroke_opacity;
  GBool stroke_adjust;
  GBool adjusted_stroke_width;
  GBool align_stroke_coords;

  void _clearPath(path_storage_t * agg_path);
  void _moveTo( path_storage_t * agg_path,double x,double y);
  void _lineTo( path_storage_t * agg_path,double x, double y);
  void _curveTo( path_storage_t * agg_path,double x0, double y0,double x1, double y1,double x2, double y2);    
  void _closePath(path_storage_t * agg_path);

  void _alignStrokeCoords(GfxSubpath *subpath, int i, double *x, double *y);
  void _doPath( GfxState *state, GfxPath *path, path_storage_t * ps);


  struct StrokePathClip {
    GfxPath *path;
    double line_width;
    double *dashes;
    int dash_count;
    double dash_offset;
    double miter;
  } *strokePathClip;

  PDFDoc *doc;			// the current document

private:
  ubyte_t               * _array;
  rendering_buffer_t    * _render_buffer;
  pixfmt_t              * _pixfmt;
  path_storage_t        * _path_storage;
  matrix_t              * _matrix;
  double                  _scale_x;
  double                  _scale_y;
};

//------------------------------------------------------------------------
// AggImageOutputDev
//------------------------------------------------------------------------

//XXX: this should ideally not inherit from AggOutputDev but use it instead perhaps
class AggImageOutputDev: public AggOutputDev {
public:

  // Constructor.
  AggImageOutputDev();

  // Destructor.
  virtual ~AggImageOutputDev();

  //----- get info about output device

  // Does this device use upside-down coordinates?
  // (Upside-down means (0,0) is the top left corner of the page.)
  virtual GBool upsideDown() { return gTrue; }

  // Does this device use drawChar() or drawString()?
  virtual GBool useDrawChar() { return gFalse; }

  // Does this device use tilingPatternFill()?  If this returns false,
  // tiling pattern fills will be reduced to a series of other drawing
  // operations.
  virtual GBool useTilingPatternFill() { return gTrue; }


  // Does this device use FillColorStop()?
  virtual GBool useFillColorStop() { return gFalse; }

  // Does this device use beginType3Char/endType3Char?  Otherwise,
  // text in Type 3 fonts will be drawn with drawChar/drawString.
  virtual GBool interpretType3Chars() { return gFalse; }

  // Does this device need non-text content?
  virtual GBool needNonText() { return gTrue; }

  //----- save/restore graphics state
  virtual void saveState(GfxState *state) { }
  virtual void restoreState(GfxState *state) { }

  //----- update graphics state
  virtual void updateAll(GfxState *state) { }
  virtual void setDefaultCTM(double *ctm) { }
  virtual void updateCTM(GfxState *state, double m11, double m12,
				 double m21, double m22, double m31, double m32) { }
  virtual void updateLineDash(GfxState *state) { }
  virtual void updateFlatness(GfxState *state) { }
  virtual void updateLineJoin(GfxState *state) { }
  virtual void updateLineCap(GfxState *state) { }
  virtual void updateMiterLimit(GfxState *state) { }
  virtual void updateLineWidth(GfxState *state) { }
  virtual void updateFillColor(GfxState *state) { }
  virtual void updateStrokeColor(GfxState *state) { }
  virtual void updateFillOpacity(GfxState *state) { }
  virtual void updateStrokeOpacity(GfxState *state) { }
  virtual void updateBlendMode(GfxState *state) { }

  //----- update text state
  virtual void updateFont(GfxState *state) { }

  //----- path painting
  virtual void stroke(GfxState *state) { }
  virtual void fill(GfxState *state) { }
  virtual void eoFill(GfxState *state) { }
  virtual void clipToStrokePath(GfxState *state) { }
  virtual GBool tilingPatternFill(GfxState *state, Gfx *gfx, Catalog *cat, Object *str,
				  double *pmat, int paintType, int tilingType, Dict *resDict,
				  double *mat, double *bbox,
				  int x0, int y0, int x1, int y1,
				  double xStep, double yStep) { return gTrue; }
  virtual GBool axialShadedFill(GfxState *state,
				GfxAxialShading *shading,
				double tMin, double tMax) { return gTrue; }
  virtual GBool radialShadedFill(GfxState *state,
				 GfxRadialShading *shading,
				 double sMin, double sMax) { return gTrue; }

  //----- path clipping
  virtual void clip(GfxState *state) { }
  virtual void eoClip(GfxState *state) { }

  //----- image drawing
  virtual void drawImageMask(GfxState *state, Object *ref, Stream *str,
			     int width, int height, GBool invert,
			     GBool interpolate, GBool inlineImg);
  virtual void drawImage(GfxState *state, Object *ref, Stream *str,
			 int width, int height, GfxImageColorMap *colorMap,
			 GBool interpolate, int *maskColors, GBool inlineImg);
  virtual void drawSoftMaskedImage(GfxState *state, Object *ref, Stream *str,
				   int width, int height,
				   GfxImageColorMap *colorMap,
				   GBool interpolate,
				   Stream *maskStr,
				   int maskWidth, int maskHeight,
				   GfxImageColorMap *maskColorMap,
				   GBool maskInterpolate);
  virtual void drawMaskedImage(GfxState *state, Object *ref, Stream *str,
			       int width, int height,
			       GfxImageColorMap *colorMap,
			       GBool interpolate,
			       Stream *maskStr,
			       int maskWidth, int maskHeight,
			       GBool maskInvert, GBool maskInterpolate);

  //----- transparency groups and soft masks
  virtual void beginTransparencyGroup(GfxState * /*state*/, double * /*bbox*/,
				      GfxColorSpace * /*blendingColorSpace*/,
				      GBool /*isolated*/, GBool /*knockout*/,
				      GBool /*forSoftMask*/) {}
  virtual void endTransparencyGroup(GfxState * /*state*/) {}
  virtual void paintTransparencyGroup(GfxState * /*state*/, double * /*bbox*/) {}
  virtual void setSoftMask(GfxState * /*state*/, double * /*bbox*/, GBool /*alpha*/,
			   Function * /*transferFunc*/, GfxColor * /*backdropColor*/) {}
  virtual void clearSoftMask(GfxState * /*state*/) {}
  
private:
};

#endif
