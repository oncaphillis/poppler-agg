//========================================================================
//
// pdftocairo.cc
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

#include "config.h"
#include <poppler-config.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include "parseargs.h"
#include "goo/gmem.h"
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
#include "AggOutputDev.h"
#if USE_CMS
#ifdef USE_LCMS1
#include <lcms.h>
#else
#include <lcms2.h>
#endif
#endif
#include <cairo.h>
#if CAIRO_HAS_PS_SURFACE
#include <cairo-ps.h>
#endif
#if CAIRO_HAS_PDF_SURFACE
#include <cairo-pdf.h>
#endif
#if CAIRO_HAS_SVG_SURFACE
#include <cairo-svg.h>
#endif


int main(int argc, char *argv[]) {

  PDFDoc    *doc = NULL;
  GooString *fileName = NULL;
  GooString *outputName = NULL;
  GooString *outputFileName = NULL;
  GooString *imageFileName = NULL;
  GooString *ownerPW, *userPW;
  int pg, pg_num_len;
  double pg_w, pg_h, tmp, output_w, output_h;
  int num_outputs;

  if (!globalParams) {
    globalParams = new GlobalParams();
  }

  if(argc==2)
  {
    
    bool useCropBox = true;
    int  page       = 0;
    int  pg_w       = 0;
    int  pg_h       = 0;

    fileName = new GooString(argv[1]);

    // parse args

    doc = PDFDocFactory().createPDFDoc(*fileName, NULL, NULL);

    std::cerr << "(" << doc->isOk() << ")" << std::endl;

    AggOutputDev * aggOut = new AggOutputDev();
    int pages = doc->getNumPages();
    for(pg=1;pg<=pages;pg++) {
      if (useCropBox) {
        pg_w = doc->getPageCropWidth(pg);
        pg_h = doc->getPageCropHeight(pg);
      } else {
        pg_w = doc->getPageMediaWidth(pg);
        pg_h = doc->getPageMediaHeight(pg);
      }
      std::cerr << "#" << page << " " << pg_w << "x" << pg_h << std::endl;
    }

    aggOut->setAgg( pg_w ,pg_h, 720.0, 720.0 );

    aggOut->startDoc(doc);
      
    doc->displayPageSlice(aggOut,
                          1,
                          72.0,72.0,
                          0,           /* rotate */
                          gFalse,      /* useMediaBox */
                          gFalse,      /* Crop */
                          gFalse,
                          -1, -1, -1, -1);
      
    aggOut->writePpm("BLA.ppm");

    delete aggOut;
    delete doc;
  }
  else
  {
    std::cerr << "Usage: pdftoagg <filename>" << std::endl;
  }
}
