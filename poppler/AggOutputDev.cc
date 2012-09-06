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
#include "AggOutputDev.h"

#ifdef USE_GCC_PRAGMAS
#pragma implementation
#endif

//------------------------------------------------------------------------
// AggOutputDev
//------------------------------------------------------------------------

AggOutputDev::AggOutputDev() {
}

AggOutputDev::~AggOutputDev() {
}


void AggOutputDev::setTextPage(TextPage *text)
{
}


void AggOutputDev::startPage(int pageNum, GfxState *state) {
}

void AggOutputDev::endPage() {
}

void AggOutputDev::saveState(GfxState *state) {
}

void AggOutputDev::restoreState(GfxState *state) {
}

void AggOutputDev::updateAll(GfxState *state) {
}

void AggOutputDev::setDefaultCTM(double *ctm) {
}

void AggOutputDev::updateCTM(GfxState *state, double m11, double m12,
				double m21, double m22,
				double m31, double m32) {
}

void AggOutputDev::updateLineDash(GfxState *state) {
}

void AggOutputDev::updateFlatness(GfxState *state) {
}

void AggOutputDev::updateLineJoin(GfxState *state) {
}

void AggOutputDev::updateLineCap(GfxState *state) {
}

void AggOutputDev::updateMiterLimit(GfxState *state) {
}

void AggOutputDev::updateLineWidth(GfxState *state) {
}

void AggOutputDev::updateFillColor(GfxState *state) {
}

void AggOutputDev::updateStrokeColor(GfxState *state) {
}

void AggOutputDev::updateFillOpacity(GfxState *state) {
}

void AggOutputDev::updateStrokeOpacity(GfxState *state) {
}

void AggOutputDev::updateFillColorStop(GfxState *state, double offset) {
}

void AggOutputDev::updateBlendMode(GfxState *state) {
}

void AggOutputDev::updateFont(GfxState *state) {
}

void AggOutputDev::alignStrokeCoords(GfxSubpath *subpath, int i, double *x, double *y)
{
}

void AggOutputDev::stroke(GfxState *state) {
}

void AggOutputDev::fill(GfxState *state) {
}

void AggOutputDev::eoFill(GfxState *state) {
}

GBool AggOutputDev::tilingPatternFill(GfxState *state, Gfx *gfx1, Catalog *cat, Object *str,
					double *pmat, int paintType, int /*tilingType*/, Dict *resDict,
					double *mat, double *bbox,
					int x0, int y0, int x1, int y1,
					double xStep, double yStep)
{
    return gTrue;
}

GBool AggOutputDev::axialShadedFill(GfxState *state, GfxAxialShading *shading, double tMin, double tMax) {
    return gTrue;
}

GBool AggOutputDev::axialShadedSupportExtend(GfxState *state, GfxAxialShading *shading)
{
  return (shading->getExtend0() == shading->getExtend1());
}

GBool AggOutputDev::radialShadedFill(GfxState *state, GfxRadialShading *shading, double sMin, double sMax) {
    return gTrue;
}

GBool AggOutputDev::radialShadedSupportExtend(GfxState *state, GfxRadialShading *shading)
{
    return gTrue;
}



void AggOutputDev::clip(GfxState *state) {
}

void AggOutputDev::eoClip(GfxState *state) {
}

void AggOutputDev::clipToStrokePath(GfxState *state) {
}

void AggOutputDev::fillToStrokePathClip(GfxState *state) {
}

void AggOutputDev::beginString(GfxState *state, GooString *s)
{
}

void AggOutputDev::drawChar(GfxState *state, double x, double y,
			      double dx, double dy,
			      double originX, double originY,
			      CharCode code, int nBytes, Unicode *u, int uLen)
{
}

void AggOutputDev::endString(GfxState *state)
{
}


GBool AggOutputDev::beginType3Char(GfxState *state, double x, double y,
				      double dx, double dy,
				      CharCode code, Unicode *u, int uLen) {

    return gTrue;
}

void AggOutputDev::endType3Char(GfxState *state) {
}

void AggOutputDev::type3D0(GfxState *state, double wx, double wy) {
}

void AggOutputDev::type3D1(GfxState *state, double wx, double wy,
			     double llx, double lly, double urx, double ury) {
}

void AggOutputDev::beginTextObject(GfxState *state) {
}

void AggOutputDev::endTextObject(GfxState *state) {
}

void AggOutputDev::beginActualText(GfxState *state, GooString *text)
{
}

void AggOutputDev::endActualText(GfxState *state)
{
}

void AggOutputDev::beginTransparencyGroup(GfxState * /*state*/, double * /*bbox*/,
                                      GfxColorSpace * blendingColorSpace,
                                      GBool /*isolated*/, GBool knockout,
				      GBool forSoftMask) {
}

void AggOutputDev::endTransparencyGroup(GfxState * /*state*/) {
}

void AggOutputDev::paintTransparencyGroup(GfxState * /*state*/, double * /*bbox*/) {
}

/* XXX: do we need to deal with shape here? */
void AggOutputDev::setSoftMask(GfxState * state, double * bbox, GBool alpha,
                                 Function * transferFunc, GfxColor * backdropColor) {
}

void AggOutputDev::popTransparencyGroup() {
}


void AggOutputDev::clearSoftMask(GfxState * /*state*/) {
}


void AggOutputDev::getScaledSize(int  orig_width,
				   int  orig_height,
				   int *scaledWidth,
				   int *scaledHeight) {
}


void AggOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str,
				   int width, int height, GBool invert,
				   GBool interpolate, GBool inlineImg) {

}

void AggOutputDev::setSoftMaskFromImageMask(GfxState *state, Object *ref, Stream *str,
				   int width, int height, GBool invert,
				   GBool inlineImg, double *baseMatrix) {

}

void AggOutputDev::unsetSoftMaskFromImageMask(GfxState *state, double *baseMatrix) {
}

void AggOutputDev::drawImageMaskRegular(GfxState *state, Object *ref, Stream *str,
					  int width, int height, GBool invert,
					  GBool interpolate, GBool inlineImg) {
}


void AggOutputDev::drawImageMaskPrescaled(GfxState *state, Object *ref, Stream *str,
					    int width, int height, GBool invert,
					    GBool interpolate, GBool inlineImg) {
}

void AggOutputDev::drawMaskedImage(GfxState *state, Object *ref,
				     Stream *str, int width, int height,
				     GfxImageColorMap *colorMap,
				     GBool interpolate,
				     Stream *maskStr, int maskWidth,
				     int maskHeight, GBool maskInvert,
				     GBool maskInterpolate)
{
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
}

GBool AggOutputDev::getStreamData (Stream *str, char **buffer, int *length)
{
    return gTrue;
}

void AggOutputDev::drawImage(GfxState *state, Object *ref, Stream *str,
			       int width, int height,
			       GfxImageColorMap *colorMap,
			       GBool interpolate,
			       int *maskColors, GBool inlineImg)
{
}


//------------------------------------------------------------------------
// ImageOutputDev
//------------------------------------------------------------------------

AggImageOutputDev::AggImageOutputDev()
{
}

AggImageOutputDev::~AggImageOutputDev()
{
}


void AggImageOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str,
					int width, int height, GBool invert,
					GBool interpolate, GBool inlineImg)
{
}

void AggImageOutputDev::drawImage(GfxState *state, Object *ref, Stream *str,
				    int width, int height, GfxImageColorMap *colorMap,
				    GBool interpolate, int *maskColors, GBool inlineImg)
{
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
}

void AggImageOutputDev::drawMaskedImage(GfxState *state, Object *ref, Stream *str,
					  int width, int height,
					  GfxImageColorMap *colorMap,
					  GBool interpolate,
					  Stream *maskStr,
					  int maskWidth, int maskHeight,
					  GBool maskInvert, GBool maskInterpolate)
{
}
