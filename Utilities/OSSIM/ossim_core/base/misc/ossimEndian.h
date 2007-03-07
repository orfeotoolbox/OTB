//******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
// Description: This file contains the interface to the
//              endian byte swap
//
//***********************************
// $ID$
#ifndef ossimEndian_HEADER
#define ossimEndian_HEADER
#include <iostream>
using namespace std;

// for OSSIM_LITTLE_ENDIAN AND BIG_ENDIAN
#include <base/common/ossimConstants.h>

class OSSIMDLLEXPORT ossimEndian
{
public:
   ossimEndian();

   /*!
    *  Swap bytes methods that work on a single pixel.
    */
   void swap(ossim_int16   &data) const;
   void swap(ossim_uint16  &data) const;
   void swap(ossim_int32   &data) const;
   void swap(ossim_uint32  &data) const;
   void swap(ossim_float32 &data) const;
   void swap(ossim_float64 &data) const;

   /*!
    *  Swap bytes methods that work on arrays of pixels.
    *
    *  @note size is number of pixels, not number of bytes.
    */
   void swap(ossimScalarType scalar, void* data, ossim_uint32 size) const;
   
   void swap(ossim_int16* data, ossim_uint32 size) const;
   void swap(ossim_uint16* data, ossim_uint32 size) const;

   void swap(ossim_int32* data, ossim_uint32 size) const;
   void swap(ossim_uint32* data, ossim_uint32 size) const;

#ifdef HAS_LONG_LONG
   void swap(ossim_int64* data, ossim_uint32 size) const;
   void swap(ossim_uint64* data, ossim_uint32 size) const;
#endif

   void swap(ossim_float32* data, ossim_uint32 size) const;
   void swap(ossim_float64* data, ossim_uint32 size) const;

   void swapTwoBytes(void* data, ossim_uint32 size) const;
   void swapFourBytes(void* data, ossim_uint32 size) const;
   void swapEightBytes(void* data, ossim_uint32 size) const;
   ossimByteOrder getSystemEndianType() const;

private:

   // Holds the Endian of the architecture that you are running on.
   ossimByteOrder  theSystemEndianType;  
   
   void swapTwoBytes(void *data) const;
   void swapFourBytes(void *data) const;
   void swapEightBytes(void *data) const;

   void swap(ossim_uint8 *c1,
             ossim_uint8 *c2) const;
};

#endif /* End of #ifndef ossimEndian_HEADER */
