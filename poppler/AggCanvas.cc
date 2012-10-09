git commit -a#include "AggCanvas.h"

#include <iostream>
#include <fstream>

template<>
bool BasicAggCanvas<agg::cmyk>::writePpm(const std::string & fname)
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
        agg::rgba8 r = agg::to_cmyk(c).to_rgb();
        of << (char) r.r << (char) r.g << (char) r.b;
      }
    }
    return true;
  }
  return false;
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

