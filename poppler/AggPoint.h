//========================================================================
//
// AggPoint.h
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

#ifndef AGGPOINT_H
#define AGGPOINT_H

struct AggPoint : std::pair<double,double> {
private:
    typedef std::pair<double,double> super;
public:
    AggPoint(double xi=0.0,double yi=0.0) 
        : super(xi,yi),
          x(super::first),
          y(super::second)
    {
    }
    double & x;
    double & y;
};

std::ostream & operator<<(std::ostream & os , const AggPoint & p);

#endif // AGGPOINT_H
