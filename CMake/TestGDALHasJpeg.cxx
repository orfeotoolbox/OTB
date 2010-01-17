#include <cstdio>
//---
// Using windows .NET compiler there is a conflict in the libjpeg with INT32
// in the file jmorecfg.h.  Defining XMD_H fixes this.
//---
#if defined(__BORLANDC__)
#include <iostream>
using std::size_t;
#include <stdlib.h>
#include <stdio.h>
#endif

extern "C"
{
#if defined(_MSC_VER) || defined(__MINGW32__) || defined(__BORLANDC__)
#  ifndef XMD_H
#    define XMD_H
#  endif
#endif

#include "jpeglib.h"
}

int main(int argc,char * argv[])
{
  struct jpeg_decompress_struct theCinfo;
  jpeg_create_decompress(&theCinfo);
  return 0;
}

