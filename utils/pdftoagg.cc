//========================================================================
//
// pdftoagg.cc
//
// Copyright 2012 Sebastian Kloska (oncaphillis@snafu.de)
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

#include "config.h"
#include <poppler-config.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include "parseargs.h"
#include "goo/gtypes.h"
#include "goo/gtypes_p.h"
#include "goo/GooString.h"
#include "goo/ImgWriter.h"
#include "goo/JpegWriter.h"
#include "goo/PNGWriter.h"
#include "GlobalParams.h"
#include "Object.h"
#include "PDFDoc.h"
#include "PDFDocFactory.h"
#include "AggCanvas.h"
#include "AggOutputDev.h"

bool rgb          = false;
bool cmyk         = false;
bool printHelp    = false;
bool printVersion = false;

static const ArgDesc argDesc[] = {
  {"-rgb",   argFlag,     &rgb,           0,
   "generate a RGB file (default)"},
  {"-cmyk",argFlag,    &cmyk,       0,
   "generate a CMYK file"},
  {"-help",  argFlag,     &printHelp,     0,
   "print usage information"},
  {"-h",      argFlag,     &printHelp,     0,
   "print usage information"},
  {"-?",      argFlag,     &printHelp,     0,
   "print usage information"},
  {"-v",      argFlag,     &printVersion,  0,
   "print copyright and version info"},
  {"-version",argFlag,     &printVersion,  0,
   "print copyright and version info"},
  {NULL}
};

int main(int argc, char *argv[]) {

  PDFDoc    *doc = NULL;
  bool ok = parseArgs(argDesc,&argc,argv);

  if (rgb && cmyk) {
    ok = gFalse;
  }

  if (!globalParams) {
    globalParams = new GlobalParams();
  }

  if (!ok || argc > 3 || printVersion || printHelp) {
    std::cerr << "pdftoagg version " << PACKAGE_VERSION << std::endl
              << popplerCopyright    << std::endl
              << xpdfCopyright       << std::endl;

    if (!printVersion) {
      printUsage("pdftoagg", "[PDF-file [PPM-file-prefix]]", argDesc);
    }

    if (printVersion || printHelp)
      return 0;
    return 1;
  }


  if(argc==2)
  {
    bool useCropBox = true;
    int  page       = 0;
    int  pg_w       = 0;
    int  pg_h       = 0;
    
    std::string si(argv[1]);
    std::string so(::basename(si.c_str()));

    {
      size_t idx;
      if((idx=so.rfind('.'))!=std::string::npos) {
        so=so.substr(0,idx);
      }
    }
    so+=".tif";
    GooString fn(si.c_str());

    // parse args
    doc = PDFDocFactory().createPDFDoc(GooString(si.c_str()), NULL, NULL);

    if(!doc->isOk())
    {
      std::cerr << "failed to open '" << si << "'" << std::endl;
      return 1;
    }
      
    AggOutputDev aggOut;
      
    int pages = doc->getNumPages();
    
    for(int pg=1;pg<=pages;pg++) {
      if (useCropBox) {
        pg_w = doc->getPageCropWidth(pg);
        pg_h = doc->getPageCropHeight(pg);
      } else {
        pg_w = doc->getPageMediaWidth(pg);
        pg_h = doc->getPageMediaHeight(pg);
      }
    }
    
    AggAbstractCanvas * cv=NULL;
    
    if(cmyk)
    {
      std::cerr << "CMYK CANVAS" << std::endl;
      cv = new AggCmykCanvas( pg_w  * 2 , pg_h * 2 , 72.0 * 2 , 72.0 * 2 );
    }
    else
    {
      std::cerr << "RGB CANVAS" << std::endl;
      cv = new AggRgbCanvas( pg_w  * 2 , pg_h * 2 , 72.0 * 2 , 72.0 * 2 );
    }
    
    aggOut.setCanvas( cv );
    aggOut.startDoc(doc);
    
    doc->displayPageSlice(&aggOut,
                          1,
                            72.0,72.0,
                          0,           /* rotate */
                          gFalse,      /* useMediaBox */
                          gFalse,      /* Crop */
                          gFalse,
                          -1, -1, -1, -1);
    
    std::cerr << "writing '"<< so << "'" <<std::endl;

    aggOut.getCanvas().writeTiff(so.c_str());

    delete doc;
    delete cv;
  }
}
