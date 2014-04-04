//========================================================================
//
// AggCanvas.cc
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

#include "AggCanvas.h"
#include "goo/TiffWriter.h"

#include <iostream>
#include <fstream>
#include <map>

#include "splash/SplashTypes.h"

static const agg::cmyka  _xx = agg::cmyka(1.0, 0.0, 0.0, 0.0, 0.7);

static std::map<int,int> _m;

template<>
BasicAggCanvas<agg::cmyka,AggColorTraits< agg::cmyka, GfxState > >::~BasicAggCanvas() 
{ 
    for(std::map<int,int>::const_iterator i=_m.begin();i!=_m.end();i++) {
        std::cerr << "@(" << (*i).first << ")=>(" << (*i).second << ")" << std::endl;
    }
};

agg::cmyka operator*(agg::cmyka c,float f) {
    return agg::cmyka(f,c.m,c.y ,c.k ,c.a);
}

/** @short Specialization of the color_f idx functor
 */

static agg::rgba to_rgba(const agg::cmyka & c)
{
    agg::rgba r;
    agg::cmyka cl = c.to_cmy();
    r.r = ( 1.0 - cl.c );
    r.g = ( 1.0 - cl.m );
    r.b = ( 1.0 - cl.y );
    r.a = c.a;
    
    return r;
}

template<class T>
static bool write(const std::string & fn,TiffWriter & writer,agg::row_accessor<T> & r ) {

  FILE *fp = fopen(fn.c_str(),"w");
  if(fp==NULL)
    return false;

  if (!writer.init(fp, r.width(), r.height(), 72, 72)) {
    ::fclose(fp);
    return false;
  }

  unsigned char **ptr = new unsigned char*[r.height()];
  
  for (size_t y = 0; y < r.height(); ++y) {
    ptr[y] = r.row_ptr(y);
  }
  
  if (!writer.writePointers(ptr, r.height())) {
    delete[] ptr;
    ::fclose(fp);
    return false;
  }
  
  delete[] ptr;

  if (!writer.close()) {
    ::fclose(fp);
    return false;
  }
  ::fclose(fp);
  return true;
}

#if !SPLASH_CMYK

/** @short If we don't have any CMYK capable TiffWriter at hand we transform the CMYK
    AggCanvas to RGB befor saving.
*/

template<>
bool write<agg::cmyka>(const std::string & fn,TiffWriter & writer,agg::row_accessor<agg::cmyka> & r ) {

  std::cerr << "RGB WRITE" << std::endl;
  FILE *fp = fopen(fn.c_str(),"w");
  
  if(fp==NULL)
    return false;

  if (!writer.init(fp, r.width(), r.height(), 72, 72)) {
    ::fclose(fp);
    return false;
  }

  unsigned char *ptr = new unsigned char[r.width()*3];
  
  for (size_t i = 0; i < r.height(); ++i)  {
   for (size_t j = 0; j < r.width(); ++j) {
     agg::rgba8 rgb = to_rgba(agg::to_cmyka(r.row_ptr(j)[j]));
     ptr[j*3+0] = rgb.r;
     ptr[j*3+1] = rgb.g;
     ptr[j*3+2] = rgb.b;
   }
   unsigned char *pp=ptr;
   if (!writer.writeRow( & pp )) {
     delete[] ptr;
     ::fclose(fp);
     return false;
   }
  }

  delete[] ptr;

  if (!writer.close()) {
    ::fclose(fp);
    return false;
  }
  ::fclose(fp);
  return true;
}
#endif

template<>
bool BasicAggCanvas< agg::device_na<5> >::writePpm(const std::string & fname)
{
  agg::device_na8<5> c;

  std::ofstream of(fname.c_str());
  if(of)
  {
    of << "P6" << std::endl
       << "# Created by ARip" << std::endl
       << getWidth() << " " <<  getHeight() << " " << 255 << std::endl;

    for(size_t i = 0; i < getHeight(); ++i)
    {
      for(size_t j = 0; j < getWidth(); ++j)
      {
        c = getFmt()->pixel(j,i);
        of << (char) c[0] << (char) c[1] << (char) c[2];
      }
    }
    return true;
  }
  return false;
}

template<>
bool BasicAggCanvas<agg::device_na<5> >::writeTiff(const std::string & fname)
{
#if SPLASH_CMYK
    TiffWriter w(TiffWriter::DEVICE_N5);
#else
    TiffWriter w(TiffWriter::RGB);
#endif
    write(fname,w,_traits.buffer());
    return true;
}

template<>
bool BasicAggCanvas<agg::cmyka>::writePpm(const std::string & fname)
{
  agg::cmyka8 c;
  
  std::ofstream of(fname.c_str());
  if(of)
  {
    of << "P6" << std::endl 
       << "# Created by ARip" << std::endl
       << getWidth() << " " <<  getHeight() << " " << 255 << std::endl;
    
    for(size_t i = 0; i < getHeight(); ++i)
    {
      for(size_t j = 0; j < getWidth(); ++j)
      {
        c = getFmt()->pixel(j,i);
        agg::rgba8 r = to_rgba(agg::to_cmyka(c));
        of << (char) r.r << (char) r.g << (char) r.b;
      }
    }
    return true;
  }
  return false;
}

template<>
bool BasicAggCanvas<agg::cmyka>::writeTiff(const std::string & fname)
{
#if SPLASH_CMYK
    TiffWriter w(TiffWriter::CMYK);
#else
    TiffWriter w(TiffWriter::RGB);
#endif
    write(fname,w,_traits.buffer());
    return true;
}

template<>
bool BasicAggCanvas<agg::rgba>::writePpm(const std::string & fname)
{
  agg::rgba8 c;
  
  std::ofstream of(fname.c_str());
  if(of)
  {
    of << "P6" << std::endl 
       << "# Created by ARip" << std::endl
       << getWidth() << " " <<  getHeight() << " " << 255 << std::endl;
    
    for(size_t i = 0; i < getHeight(); ++i)
    {
      for(size_t j = 0; j < getWidth(); ++j)
      {
        c = getFmt()->pixel(j,i);
        of << (char) c.r << (char) c.g << (char) c.b;
      }
    }
    return true;
  }
  return false;
}

template<>
bool BasicAggCanvas< agg::rgba >::writeTiff(const std::string & fname)
{
    TiffWriter w(TiffWriter::RGB);
    w.setCompressionString("");
    write(fname,w,_traits.buffer());
    return true;
}
