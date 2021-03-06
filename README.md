# poppler-agg README

## INTRO

poppler-agg  is   meant   to provide   an   AGG (http://antigrain.com)
rendering  back-end to  the poppler  (http://poppler.freedesktop.org/)
library.

The  git  repository contains  two  branches. (a) "master"  which is a
clone of  the official poppler repo  and (b) "agg_support" which I use
to add everything for (you guessed it) AGG support.

## MOTIVATION   

AGG  the Anti-Grain Geometry   Library  represents a somewhat  obscure
software project.   Although  one  might   say that it   is  currently
unmaintained it still   provides  a hi-quality  graphics  engine  well
comparable  with projects like  cairo.   AGG makes   heavy use of  C++
templates which  constitutes a major advantage  as well as a potential
drawback.

On the one hand, all  aspects of the underling  color space and  color
storage  model can be  manipulated  via template arguments, making  it
possibly to   provide a large set   of output formats  for specialized
hardware  like  printers  with very specific   needs  for custom color
spaces  and bit-depth per  channel.  Furthermore there are no hardware
dependent parts in the AGG  library.  It simply operates on abstracted
two dimensional arrays of pixels.

On the other hand  the heavy use  of templated  classes might make  it
complicated to create speed  optimized hardware specific code. One may
not expect performance wonders when using an AGG back-end.

The poppler-agg relies on my variant of the agg code to be found under

https://github.com/oncaphillis/agg

## STATUS   

* The AGG backend  currently supports three color spaces   "rgba"  and
  "cmyka" "device_na<N>". The is still ecperimenta and meant to mirror
  the DeviceN colorspace of PDF.   

* All Vector  graphics  shound work. 
  All amjor line drawings  and fill operations should workd.  Although
  more exotic fill types are still missing.
 
## NEXT TO COME

The next thing on my mind is Text drawing.

## INSTALL   

First you will have to install a modified  version of AGG which can be
found under (https://github.com/oncaphillis/agg.git).     This variant
supports the agg::cmyka color space  and the agg::pixfmt_cmyk  storage
model.

After this you will  have to switch into  the "agg_support" branch  of
the poppler-agg checkout and configure the package with the --with-agg
argument. The headers and libs of the modified agg installation should
be made available via the  CXXFLAGS and LDFLAGS environment  variables
like:

  LDFLAGS="-L/path/to/agg/libs/" CXXFLAGS="-I/path/to/agg/headers" ./configure --with-agg
  
Install the poppler library as usual.  In addition to the the standard
poppler utils you will find the "pdftoagg" program.

  Invoking

  pdftoagg [-cmyk|-rgb] [file.pdf]

Will create "file.tif" from "file.pdf"  representing the first page of the
pdf document  (at least the  vector graphics part). The internal color
model will be cmyk or rgb depending on the  give flag. Under standard
configuration the  resulting "file.tif" will  always be RGB. CMYK colors
will  be transformed befor  saving. If compilation   is done with the 
SPLASH_CMYK=1 macro defined the program will output a CMYK tif file.

Sebastian Kloska (oncaphillis@snafu.de)

# Original poppler README

This is poppler, a PDF rendering library.

Poppler is a fork of the xpdf PDF viewer developed by Derek Noonburg
of Glyph and Cog, LLC.  The purpose of forking xpdf is twofold.
First, we want to provide PDF rendering functionality as a shared
library, to centralize the maintenance effort.  Today a number of
applications incorporate the xpdf code base, and whenever a security
issue is discovered, all these applications exchange patches and put
out new releases.  In turn, all distributions must package and release
new version of these xpdf based viewers.  It's safe to say that
there's a lot of duplicated effort with the current situation.  Even if
poppler in the short term introduces yet another xpdf derived code
base to the world, we hope that over time these applications will
adopt poppler.  After all, we only need one application to use poppler
to break even.

Second, we would like to move libpoppler forward in a number of areas
that don't fit within the goals of xpdf.  By design, xpdf depends on
very few libraries and runs a wide range of X based platforms.  This
is a strong feature and reasonable design goal.  However, with poppler
we would like to replace parts of xpdf that are now available as
standard components of modern Unix desktop environments.  One such
example is fontconfig, which solves the problem of matching and
locating fonts on the system, in a standardized and well understood
way.  Another example is cairo, which provides high quality 2D
rendering.  See the file TODO for a list of planned changes.

Please note that xpdf, and thus poppler, is licensed under the GPL,
not the LGPL.  Consequently, any application using poppler must also
be licensed under the GPL.  If you want to incorporate Xpdf based PDF
rendering in a closed source product, please contact Glyph & Cog
(www.glyphandcog.com) for commercial licensing options.

	Kristian Høgsberg, Feb. 27, 2005


See the README-XPDF for the original xpdf-3.03 README.

