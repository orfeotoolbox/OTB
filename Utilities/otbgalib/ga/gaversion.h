/* ----------------------------------------------------------------------------
  version.h
  mbwall 10oct98

   This is the header file to keep track of the versions and revisions of the
GA library.  You can use the ident command to extract the version and other
build information from the galib object file.
---------------------------------------------------------------------------- */
#ifndef _ga_version_h_
#define _ga_version_h_

#include <ga/gaconfig.h>

#define GALIB_LIBRARY_IDENTIFIER \
  "$Date: 2004-12-29 11:24:43 -0500 (Wed, 29 Dec 2004) $"\
  "$Revision: 43 $"\
  "$Configuration: " GALIB_OS "-" GALIB_CPU "-" GALIB_COMPILER " $"

const char* GAConfig();

#endif
