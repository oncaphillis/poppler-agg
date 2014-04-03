//========================================================================
//
// pdftoagg.cc
//
// Copyright 2012-13 Sebastian Kloska (oncaphillis@snafu.de)
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
#include <unistd.h>
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

static double resolution   = 0.0;
static double resolution_x = 0.0;
static double resolution_y = 0.0;
static int    page         = 1;
static bool rgb          = false;
static bool cmyk         = false;
static bool test         = false;
static bool verbose      = false;
static bool printHelp    = false;
static bool printVersion = false;

static const ArgDesc argDesc[] = {
      {"-rgb",    argFlag,      &rgb,          0, "generate a RGB file (default)"},
      {"-cmyk",    argFlag,     &cmyk,         0, "generate a CMYK file"},
      {"-help",    argFlag,     &printHelp,    0, "print usage information"},
      {"-h",       argFlag,     &printHelp,    0, "print usage information"},
      {"-?",       argFlag,     &printHelp,    0, "print usage information"},
      {"-v",       argFlag,     &printVersion, 0, "print copyright and version info"},
      {"-version", argFlag,     &printVersion, 0, "print copyright and version info"},
      {"-r",       argFP,       &resolution,   0, "Resolution, in DPI (default is 72)"},
      {"-rx",      argFP,       &resolution_x, 0, "X-resolution, in DPI (default is 72"},
      {"-ry",      argFP,       &resolution_y, 0, "Y-Resolution, in DPI (default is 150)"},
      {"-test",    argFlag,     &test,         0, "run self-test"},
      {"-page",    argInt,      &page,         0, "select page to work on"},
      {"-verbose",argFlag,     &verbose,       0, "print löts of additional info"},
      {NULL}
};

int main(int argc, char *argv[]) {
  PDFDoc    *doc = NULL;
  bool ok = parseArgs(argDesc,&argc,argv);

  if (rgb && cmyk) {
    ok = gFalse;
  }

  if ( (resolution!=0.0 && resolution_x!=0.0) || (resolution!=0.0 && resolution_x!=0.0) ) {
    ok = gFalse;
  }

  if(resolution!=0.0)
  {
    resolution_x = resolution_y = resolution;
  }
  
  if(resolution_x < 0.0 || resolution_y < 0.0)
  {
    ok = gFalse;
  }

  if(resolution_x == 0.0)
      resolution_x = 72.0;

  if(resolution_y == 0.0)
    resolution_y = 72.0;

  if (!globalParams) {
    globalParams = new GlobalParams();
  }

  if (!ok || argc > 3 || (test!=0 && argc!=2) || printVersion || printHelp) {
    std::cerr << "pdftoagg version " << PACKAGE_VERSION << std::endl
              << popplerCopyright    << std::endl
              << xpdfCopyright       << std::endl;

    if (!printVersion) {
      printUsage("pdftoagg", "[PDF-file [TIF-file-prefix]]", argDesc);
    }

    if (printVersion || printHelp)
      return 0;
    
    return 1;
  }

  if(argc==2 || argc==3)
  {
    bool useCropBox = true;
    int  pg_w       = 0;
    int  pg_h       = 0;
    
    std::string si(argv[1]);
    std::string so(argc==2 ? ::basename(si.c_str()) : argv[2]);

    if(test!=0) {
        char n[]="/tmp/popperagg_XXXXXX";
        const char * p = ::mktemp(n);
        so = std::string(p);
    } else if(argc==2) {
      size_t idx;
      if( (idx=so.rfind('.'))!=std::string::npos ) {
        so=so.substr(0,idx);
      }
      so+=".tif";
    }

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
    
    if(true || cmyk)
    {
      cv = new AggCmykCanvas( (pg_w * resolution_x) / 72.0, (pg_h * resolution_y) / 72.0, 
                              resolution_x , resolution_y); 
    }
#if 0
    else
    {
      cv = new AggRgbCanvas( (pg_w * resolution_x) / 72.0, (pg_h * resolution_y) / 72.0, 
                             resolution_x , resolution_y); 
    }
#endif
    aggOut.setCanvas( cv );
    aggOut.startDoc(doc);
    
    doc->displayPageSlice(&aggOut,
                          page,
                          72.0,72.0,
                          0,           /* rotate */
                          gFalse,      /* useMediaBox */
                          gFalse,      /* Crop */
                          gFalse,
                          -1, -1, -1, -1);
    
    if(test) {

        if(verbose)
            std::cerr << "showing '" << so << "'" << std::endl;
        
        aggOut.getCanvas().writeTiff(so.c_str());
        
        ::system((std::string("okular ")+so+" >/dev/null 2>&1").c_str());

        if(verbose)
            std::cerr << "Done ... deleting '" << so << "'" << std::endl;

        if(::unlink(so.c_str())==-1)
        {
            std::cerr << " failed to delete '" << so << "'" << std::endl;
        }
    } else {
        std::cerr << "writing '"<< so << "'" <<std::endl;
        aggOut.getCanvas().writeTiff(so.c_str());
    }

    delete doc;
    delete cv;
  }
}
