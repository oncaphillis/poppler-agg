//========================================================================
//
// AggMatrix.cc
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

#include "AggMatrix.h"
#include "AggPoint.h"

const AggMatrix AggMatrix::MirrorX = AggMatrix(1.0, 0.0, 0.0, 1.0,  0.0,  0.0);
const AggMatrix AggMatrix::MirrorY = AggMatrix(1.0, 0.0, 0.0,-1.0,  0.0,  0.0);

const AggMatrix AggMatrix::Scaling(double x,double y) {
  return  agg::trans_affine_scaling( x , y );
}

const AggMatrix AggMatrix::Rotation(double a) {
    return agg::trans_affine_rotation( ((2*agg::pi) * a) / 360.0 );
}

const AggMatrix AggMatrix::Rotation(double x0, double y0,double x1,double y1) {
    return agg::trans_affine_rotation( atan2(x1-x0, y1-y0) );
}

const AggMatrix AggMatrix::Translation(double x,double y) {
    return agg::trans_affine_translation(x,y);
}

AggMatrix AggMatrix::rotate(double a) const {
    return *this * Rotation(a);
}

AggMatrix AggMatrix::translate(double x,double y) const {
    return *this * Translation(x,y);
}

AggMatrix AggMatrix::translate(const AggPoint & p ) const {
    return translate(p.x,p.y);
}

AggMatrix AggMatrix::scale(double x,double y ) const {
    return *this * Scaling(x,y);
}

AggMatrix AggMatrix::invert() const {
     AggMatrix m(*this);
     m._trans.invert();
     return m;
}
