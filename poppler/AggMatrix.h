#ifndef AGGMATRIX_H
#define AGGMATRIX_H

#include <iostream>

#include "agg_trans_affine.h"
          
/** Out if some not so well understood reasons the agg_trans_affine objects
    henerates junk data (bad paths) whenever used as a class member. I always
    had to allocate them via 'new ' operator. This class hides this fact from me,
    an allows me to name transformation matrix members PDF styple.
    
*/


class AggMatrix {
private:
  typedef agg::trans_affine trans_t;
public:

  AggMatrix()
    : _trans(),
      a(_trans.sx),
      b(_trans.shy),
      c(_trans.shx),
      d(_trans.sy),
      h(_trans.tx),
      v(_trans.ty)
  {
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

  operator trans_t & () {
    return _trans;
  }

  AggMatrix  operator * (const AggMatrix & a) const {
    return _trans * a._trans;
  }

  AggMatrix & operator *= (const AggMatrix & a) {
    _trans *= a._trans;
    return *this;
  }
             
private:
  trans_t  _trans;
  
public:
  const double & a;
  const double & b;
  const double & c;
  const double & d;
  const double & h;
  const double & v;

  static const AggMatrix MirrorX;
  static const AggMatrix MirrorY;
};

std::ostream & operator<<(std::ostream & os, const AggMatrix & m);

#endif

