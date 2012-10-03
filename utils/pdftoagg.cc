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

int main(int argc, char *argv[]) {

  PDFDoc    *doc = NULL;
  GooString *fileName = NULL;

  if (!globalParams) {
    globalParams = new GlobalParams();
  }

  if(argc==2)
  {
    bool useCropBox = true;
    int  page       = 0;
    int  pg_w       = 0;
    int  pg_h       = 0;
    double resx = 180.0;
    double resy = 180.0;
    fileName = new GooString(argv[1]);

    // parse args

    doc = PDFDocFactory().createPDFDoc(*fileName, NULL, NULL);

    std::cerr << "(" << doc->isOk() << ")" << std::endl;

    AggOutputDev * aggOut = new AggOutputDev();
    int pages = doc->getNumPages();
    for(int pg=1;pg<=pages;pg++) {
      if (useCropBox) {
        pg_w = doc->getPageCropWidth(pg);
        pg_h = doc->getPageCropHeight(pg);
      } else {
        pg_w = doc->getPageMediaWidth(pg);
        pg_h = doc->getPageMediaHeight(pg);
      }
      std::cerr << "#" << page << " " << pg_w << "x" << pg_h << std::endl;
    }

    aggOut->setAgg( pg_w ,pg_h, resx, resy );

    aggOut->startDoc(doc);
      
    doc->displayPageSlice(aggOut,
                          1,
                          36.0,36.0,
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
