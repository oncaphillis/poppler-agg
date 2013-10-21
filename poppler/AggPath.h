//========================================================================
//
// AggPath.h
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

#ifndef AGGPATH_H
#define AGGPATH_H

#include "GfxState.h"
#include "agg_path_storage.h"
#include <AggMatrix.h>
#include <iostream>

template < class C >
class AggOnion {
public:
  typedef C               value_t;
  typedef value_t       & ref_t;
  typedef const value_t & const_ref_t;
  typedef C             * ptr_t;

  AggOnion() : _v()  {
  }

  AggOnion(C & v) : _v(v)  {
  }
        
  operator const_ref_t  () const {
    return _v;
  }

  operator ref_t  ()  {
    return _v;
  }

  const_ref_t toAgg () const {
    return _v;
  }

  ref_t  toAgg ()  {
    return _v;
  }

  ptr_t operator->() {
      return &_v;
  }

  AggOnion & operator=(const value_t & v) {
      _v=v;
  }

private:
  value_t _v;
};


template< class C >
std::ostream & operator<<(std::ostream & os,const AggOnion<C> & o) {
  AggOnion<C> m=o;
  int cmd;
  double x,y;

  while( ! agg::is_stop(cmd = m.toAgg().vertex(&x, &y) ) )
  {
    os << cmd << ":[" << x << "," << y << "] ";
  }  

  return os;
}

/** @short AggPath servers as an adapter between the AGG path definition 
    and the GfxPath definion. Yoit construct it from an GfxPath or an
    agg::path_storage and use it wherever an agg::path_storage is needed.
*/

class AggPath : public AggOnion< agg::path_storage > {
private:
  typedef AggOnion< agg::path_storage > super;
public:
  typedef super::value_t  path_storage_t;
  typedef GfxPath         gfxpath_t;
  typedef super::value_t  agg_t;
  typedef AggMatrix       matrix_t;

  AggPath() :  super()  {
  }
  
  AggPath(const super & s) : super(s) {

  }

  AggPath( path_storage_t & p ) :  super(p)  {
  }

  AggPath(gfxpath_t & p) {
    init(p);
  }

  AggPath(gfxpath_t * p) {
    init(*p);

  }

private:

  /** @short A helper for constructor and addignment from
      gfxpath_t
  */

  void init(gfxpath_t & p);
};

#endif // AGGPATH_H
