//========================================================================
//
// AggOutputDev.h
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

#ifndef AGGOUTPUTDEV_H
#define AGGOUTPUTDEV_H

#include "goo/gtypes.h"
#include "OutputDev.h"
#include "TextOutputDev.h"
#include "GfxState.h"
#include "AggCanvas.h"


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
  typedef OutputDev                  super;
  typedef agg::path_storage          path_storage_t;
  typedef AggAbstractCanvas          canvas_t;

public:

  typedef unsigned char      ubyte_t;
  typedef canvas_t::matrix_t matrix_t;
  typedef canvas_t::path_t   path_t;

  // Constructor.
  AggOutputDev(canvas_t * c = NULL);

  // Destructor.
  virtual ~AggOutputDev();

  // Set the ultimate target of drawing operations i.e. an array of appropriately dimensioned  bytes etc.
  virtual GBool setCanvas( canvas_t * c);

  //----- get info about output device

  // Does this device use upside-down coordinates?
  // (Upside-down means (0,0) is the top left corner of the page.)
  virtual GBool upsideDown() { return gTrue; }

  // Does this device use drawChar() or drawString()?
  virtual GBool useDrawChar() { return gTrue; }

  // Does this device use tilingPatternFill()?  If this returns false,
  // tiling pattern fills will be reduced to a series of other drawing
  // operations.
  virtual GBool useTilingPatternFill() override { return gFalse; }

  virtual GBool useShadedFills(int type) override { return gTrue; }

  // Does this device use FillColorStop()?
  virtual GBool useFillColorStop() override { return gTrue; }

  // Does this device use beginType3Char/endType3Char?  Otherwise,
  // text in Type 3 fonts will be drawn with drawChar/drawString.
  virtual GBool interpretType3Chars() { return gFalse; }

  //----- initialization and control

  // Start a page.
  virtual void startPage(int pageNum, GfxState *state, XRef *xref) override;

  // End a page.
  virtual void endPage() override;

  //----- save/restore graphics state
  virtual void saveState(GfxState *state) override;
  virtual void restoreState(GfxState *state) override;

  //----- update graphics state
  virtual void updateAll(GfxState *state) override;
  virtual void setDefaultCTM(double *ctm) override;
  virtual void updateCTM(GfxState *state, double m11, double m12,
             double m21, double m22, double m31, double m32) override;
  virtual void updateLineDash(GfxState *state) override;
  virtual void updateFlatness(GfxState *state) override;
  virtual void updateLineJoin(GfxState *state) override;
  virtual void updateLineCap(GfxState *state) override;
  virtual void updateMiterLimit(GfxState *state) override;
  virtual void updateLineWidth(GfxState *state) override;
  virtual void updateFillColor(GfxState *state) override;
  virtual void updateStrokeColor(GfxState *state) override;
  virtual void updateFillOpacity(GfxState *state) override;
  virtual void updateStrokeOpacity(GfxState *state) override;
  virtual void updateFillColorStop(GfxState *state, double offset) override;
  virtual void updateBlendMode(GfxState *state) override;

  //----- update text state
  virtual void updateFont(GfxState *state) override;

  //----- path painting
  virtual void stroke(GfxState *state) override;
  virtual void stroke(GfxState *state, AggPath & p);

  virtual void fill(GfxState *state) override;
  virtual void eoFill(GfxState *state) override;
  virtual void clipToStrokePath(GfxState *state) override;
  virtual GBool tilingPatternFill(GfxState *state, Gfx *gfx, Catalog *cat, Object *str,
				  double *pmat, int paintType, int tilingType, Dict *resDict,
				  double *mat, double *bbox,
				  int x0, int y0, int x1, int y1,
                  double xStep, double yStep) override;
  virtual GBool axialShadedFill(GfxState *state, GfxAxialShading *shading, double tMin, double tMax) override;
  virtual GBool axialShadedSupportExtend(GfxState *state, GfxAxialShading *shading) override;
  virtual GBool radialShadedFill(GfxState *state, GfxRadialShading *shading, double sMin, double sMax) override;
  virtual GBool radialShadedSupportExtend(GfxState *state, GfxRadialShading *shading) override;
  virtual GBool functionShadedFill(GfxState *,GfxFunctionShading *) override;

  //----- path clipping
  virtual void clip(GfxState *state) override;
  virtual void eoClip(GfxState *state) override;

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

  canvas_t & getCanvas() {
    return * _canvas;
  }

protected:
  void getScaledSize(int orig_width, int orig_height,
		     int *scaledWidth, int *scaledHeight);

  GBool getStreamData (Stream *str, char **buffer, int *length);
  void fillToStrokePathClip(GfxState *state);
  void alignStrokeCoords(GfxSubpath *subpath, int i, double *x, double *y);

  GfxRGB  fill_color, stroke_color;
  double  fill_opacity;
  double  stroke_opacity;
  GBool   stroke_adjust;
  GBool   adjusted_stroke_width;
  GBool   align_stroke_coords;

  void    _fill(GfxState * state,bool eo);

private:
  canvas_t              * _canvas;
};


#endif
