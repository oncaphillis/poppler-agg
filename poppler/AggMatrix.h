//========================================================================
//
// AggMatrix.h
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

#ifndef AGGMATRIX_H
#define AGGMATRIX_H

#include <iostream>

#include "agg_trans_affine.h"


/** Out if some not so well understood reasons the agg_trans_affine objects
    generates junk data (bad paths) whenever used as a class member. I always
    had to allocate them via 'new ' operator. This class hides this fact from me,
    and allows me to name transformation matrix members PDF styple.
*/

class AggPoint;

class AggMatrix {
private:
  typedef agg::trans_affine trans_t;

  trans_t  _trans;

    /** The Poppler side of the engine likes to see transformation matrixes
     *  as pointer to an array of double.
     *  Ths proxy allows passing the AggMAtrix transparently to these function.
     */

    class ArrayProxy
    {
    public:
        ArrayProxy(const AggMatrix & m) :
            _m(m)
        {
        }

        operator double * () {
            _a[0] = _m.a;
            _a[1] = _m.b;
            _a[2] = _m.c;
            _a[3] = _m.d;
            _a[4] = _m.h;
            _a[5] = _m.v;
            return _a;
        }

    private:
        double            _a[6];
        const AggMatrix & _m;
    };

public:

 /** @short Standard Constructor generates the unit matrix
  *  aka not transformation at all
  */
 AggMatrix()
    : _trans(),
      a(_trans.sx),
      b(_trans.shy),
      c(_trans.shx),
      d(_trans.sy),
      h(_trans.tx),
      v(_trans.ty)
  {
      *this = Translation(0.0,0.0);
  }
  
  AggMatrix(const trans_t & t)
    : _trans(t),
      a(_trans.sx),
      b(_trans.shy),
      c(_trans.shx),
      d(_trans.sy),
      h(_trans.tx),
      v(_trans.ty)
    
  {
  }
  
  AggMatrix(const double * m)
    : _trans(m),
      a(_trans.sx),
      b(_trans.shy),
      c(_trans.shx),
      d(_trans.sy),
      h(_trans.tx),
      v(_trans.ty)
  {
  }
  
  AggMatrix(const double aa,const double bb,const double cc,const double dd,const double hh,const double vv)
    : _trans(aa,bb,cc,dd,hh,vv),
      a(_trans.sx),
      b(_trans.shy),
      c(_trans.shx),
      d(_trans.sy),
      h(_trans.tx),
      v(_trans.ty)
  {
  }

  ~AggMatrix() {
  }

  AggMatrix & operator=(const AggMatrix & m) {
    _trans = m._trans;
    return *this;
  }
  
  operator const trans_t & () const {
    return _trans;
  }

  /** This way we may pass the AggMatrix to any method expecting an trans_t & aka
   *  the AGG side we have to serve.
   */

  operator trans_t & () {
    return _trans;
  }

  trans_t * operator->() {
      return &_trans;
  }

  const trans_t * operator->() const {
      return &_trans;
  }

  /** @short Matrix multiplication
   */

  AggMatrix  operator * (const AggMatrix & a) const {
    return _trans * a._trans;
  }

  /** @short Matrix multiplication with assignment.
   */

  AggMatrix & operator *= (const AggMatrix & a) {
    _trans *= a._trans;
    return *this;
  }

  /** @short Return a new AggMatrix based on the current rotated by a
   */

  AggMatrix rotate(double a) const;

  /* @short Return a new matrix based on the current scales by x and y
   */

  AggMatrix scale(double x,double y) const;

  /** @short Return a new matrix based on the current translated by x,y
   */

  AggMatrix translate(double x, double y) const;

  AggMatrix translate(const AggPoint & p) const;

  /** @short Matrix inversion.
   */

  AggMatrix invert() const;

  AggMatrix skew(float a,float b) const;

  /** This way we may pass the AggMatrix to any method that expects double * as an
   *  argument aka. the poppler interfacs.
   */

  operator double  * () const {
      return ArrayProxy(*this);
  }

public:
  const double & a;
  const double & b;
  const double & c;
  const double & d;
  const double & h;
  const double & v;

  /** A const AggMatrix that performs mirroring on the Y axes (flipping X)
   */

  static const AggMatrix MirrorX;
  /** A const AggMatrix that performs mirroring on the X axes (flipping Y)
   */

  static const AggMatrix MirrorY;

  /** @short Generate an AggMatrix doing a scaling in x and y direction.
   */

  static const AggMatrix Scaling(double x,double y);

  /** @short Generate an AggMatrix doing a Rotation by angle a
   */

  static const AggMatrix Rotation( double a );

  /** @short Shortcut for Rotation(atan2(x1-x0,y1-y0)) whenever one
      wants to directly use two vectors for the definition of angle
  */

  static const AggMatrix Rotation( double x0, double y0, double x1, double y1 );

  /** @short Generate an AggMatrix doing a translation in x an y direction.
   */

  static const AggMatrix Translation(double x,double y);

  static const AggMatrix Translation(const AggPoint & p);

  /** @short Build up a matrix for horizontal & vertical
       skewing.
   */
  static const AggMatrix Skewing(float a,float b);
};

std::ostream & operator<<(std::ostream & os, const AggMatrix & m);

/** @short Mulitplication with const double *, which will be interpreted
    as d[6] representing the matrix type in AGG nomenclature.
    */

inline
AggMatrix operator*(const double * d, const AggMatrix & m) {
    return AggMatrix(d)*m;
}

#endif
