#ifndef AGGPATH_H
#define AGGPATH_H

#include "GfxState.h"
#include "agg_path_storage.h"

#include <iostream>

/** @short AggPath servers as an adapter between the AGG path definition 
    and the GfxPath definion. Yoit construct it from an GfxPath or an
    agg::path_storage and use it wherever an agg::path_storage is needed.
*/

class AggPath {
private:
public:
  typedef agg::path_storage path_storage_t;
  typedef GfxPath           gfxpath_t;
  
  AggPath() :  _path()  {
  }
  
  AggPath( path_storage_t & p ) :  _path(p)  {
  }

  AggPath(gfxpath_t & p) {
    init(p);
  }

  AggPath(gfxpath_t * p) {
    init(*p);
  }
    
  operator const path_storage_t & () const {
    return _path;
  }

  operator path_storage_t & ()  {
    return _path;
  }

private:

  /** @short A helper for constructor and addignment from
      gfxpath_t
  */

  void init(gfxpath_t & p);

  path_storage_t _path;
};

#endif // AGGPATH_H
