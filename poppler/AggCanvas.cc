#include "AggCanvas.h"
#include "goo/TiffWriter.h"

#include <iostream>
#include <fstream>

#include "splash/SplashTypes.h"

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
  std::cerr << "1" << std::endl;
  
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
  TiffWriter w;
  w.setCompressionString("");
  w.setSplashMode(splashModeCMYK8);
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
bool BasicAggCanvas<agg::rgba>::writeTiff(const std::string & fname)
{
    TiffWriter w;
    w.setCompressionString("");
    w.setSplashMode(splashModeRGB8);
    write(fname,w,_traits.buffer());
    return true;
}

