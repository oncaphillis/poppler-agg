//========================================================================
//
// AggPath.cc
//
// Copyright 2012 Sebastian Kloska
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

#include "AggPath.h"

void AggPath::init(gfxpath_t  & p ) {
  _path.free_all();
  for (int i = 0; i < p.getNumSubpaths(); ++i) {

    GfxSubpath *sub = p.getSubpath(i);
    
    if (sub->getNumPoints() > 0) {
      _path.move_to( sub->getX(0), sub->getY(0) );
      
      int j = 1;

      while (j < sub->getNumPoints()) {

        if (sub->getCurve(j)) {
          _path.curve4(sub->getX(j),   sub->getY(j),
                       sub->getX(j+1), sub->getY(j+1),
                       sub->getX(j+2), sub->getY(j+2) );
          j += 3;
        } else {
          _path.line_to( sub->getX(j), sub->getY(j) );
          ++j;
        }
      }
      
      if (sub->isClosed()) {
        _path.close_polygon();
      }
    }
  }   
}
