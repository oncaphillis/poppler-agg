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

const AggMatrix AggMatrix::MirrorX = AggMatrix(1.0, 0.0, 0.0, 1.0,  0.0,  0.0);
const AggMatrix AggMatrix::MirrorY = AggMatrix(1.0, 0.0, 0.0,-1.0,  0.0,  0.0);

const AggMatrix AggMatrix::Scaling(double x,double y) {
  agg::trans_affine t;
  return  agg::trans_affine_scaling( x , y );
}

const AggMatrix AggMatrix::Rotation(double a) {
    agg::trans_affine t;
    return agg::trans_affine_rotation( ((2*agg::pi) * a) / 360.0 );
}

const AggMatrix AggMatrix::Translation(double x,double y) {
    agg::trans_affine t;
    return agg::trans_affine_translation(x,y);
}

AggMatrix AggMatrix::rotate(double a) const {
    return *this * Rotation(a);
}

AggMatrix AggMatrix::translate(double x,double y) const {
    return *this * Translation(x,y);
}

AggMatrix AggMatrix::scale(double x,double y ) const {
    return *this * Scaling(x,y);
}

AggMatrix AggMatrix::invert() const {
     AggMatrix m(*this);
     m._trans.invert();
     return m;
}
