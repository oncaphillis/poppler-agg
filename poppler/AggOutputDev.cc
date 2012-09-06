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
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::updateCTM(GfxState *state, double m11, double m12,
				double m21, double m22,
				double m31, double m32) {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
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
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::fill(GfxState *state) {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void AggOutputDev::eoFill(GfxState *state) {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
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
