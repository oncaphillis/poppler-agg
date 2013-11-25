//========================================================================
//
// AggException.h
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

#ifndef AGGEXCEPTION_H
#define AGGEXCEPTION_H

#include <iostream>
#include <stdexcept>

class AggException : public std::runtime_error {
private:
    typedef std::runtime_error super;;
public:
    AggException(const std::string & w) : 
        super(w)
    {
    }
private:
};


#endif // AGGEXCEPTION_H
