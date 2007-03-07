//******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
// Description: This file contains the implementation of the
//              endian byte swap routines.
//
//***********************************
// $Id: ossimEndian.cpp,v 1.6 2005/02/09 15:16:58 dburken Exp $

#include <base/misc/ossimEndian.h>

ossimEndian::ossimEndian()
{
   ossim_uint16  test;
   ossim_uint8  *testPtr=NULL;
   test = 0x0001;
   
   testPtr       = reinterpret_cast<unsigned char*>(&test);
   theSystemEndianType = testPtr[0] ? OSSIM_LITTLE_ENDIAN : OSSIM_BIG_ENDIAN;
}

ossimByteOrder ossimEndian::getSystemEndianType() const
{
   return theSystemEndianType;
}

void ossimEndian::swap(ossim_int16 &data) const
{
   swapTwoBytes(reinterpret_cast<void*>(&data));   
} 

void ossimEndian::swap(ossim_uint16 &data) const
{
   swapTwoBytes(reinterpret_cast<void*>(&data));
}

void ossimEndian::swap(ossim_int32 &data) const
{
   swapFourBytes(reinterpret_cast<void*>(&data));
}

void ossimEndian::swap(ossim_uint32 &data) const
{
   swapFourBytes(reinterpret_cast<void*>(&data));
}

void ossimEndian::swap(ossim_float32 &data) const
{
   swapFourBytes(reinterpret_cast<void*>(&data));
}

void ossimEndian::swap(ossim_float64 &data) const
{
   swapEightBytes(reinterpret_cast<void*>(&data));
}

void ossimEndian::swapTwoBytes(void *data) const
{
   unsigned char *c = reinterpret_cast<unsigned char*>(data);

   swap(&c[0], &c[1]);
}

void ossimEndian::swapFourBytes(void* data) const
{
   unsigned char *c = reinterpret_cast<unsigned char*>(data);

   swap(&c[0], &c[3]);
   swap(&c[1], &c[2]);
}

void ossimEndian::swapEightBytes(void* data) const
{
   unsigned char *c = reinterpret_cast<unsigned char*>(data);

   swap(&c[0], &c[7]);
   swap(&c[1], &c[6]);
   swap(&c[2], &c[5]);
   swap(&c[3], &c[4]);
}

void ossimEndian::swap(ossim_uint8 *c1,                       
                       ossim_uint8 *c2) const
{
   ossim_uint8 temp_c = *c1;
   *c1 = *c2;
   *c2 = temp_c;
}

void ossimEndian::swap(ossimScalarType scalar,
                       void* data, ossim_uint32 size) const
{
   switch (scalar)
   {
   case OSSIM_USHORT16:
   case OSSIM_SSHORT16:
   case  OSSIM_USHORT11:
      swapTwoBytes(data, size);
      return;

   case OSSIM_FLOAT:
   case OSSIM_NORMALIZED_FLOAT:
      swapFourBytes(data, size);
      return;

   case OSSIM_DOUBLE:
   case OSSIM_NORMALIZED_DOUBLE:
      swapEightBytes(data, size);
      break;

   default:
      return;
   }
}

void ossimEndian::swap(ossim_int16* data, ossim_uint32 size) const
{
   swapTwoBytes(data, size);
}

void ossimEndian::swap(ossim_uint16* data, ossim_uint32 size) const
{
   swapTwoBytes(data, size);
}

void ossimEndian::swap(ossim_int32* data, ossim_uint32 size) const
{
   swapFourBytes(data, size);
}

void ossimEndian::swap(ossim_uint32* data, ossim_uint32 size) const
{
   swapFourBytes(data, size);
}

#ifdef HAS_LONG_LONG

void ossimEndian::swap(ossim_int64* data, ossim_uint32 size) const
{
   swapEightBytes(data, size);
}

void ossimEndian::swap(ossim_uint64* data, ossim_uint32 size) const
{
   swapEightBytes(data, size);
}

#endif /* End of #ifdef HAS_LONG_LONG */

void ossimEndian::swap(ossim_float32* data, ossim_uint32 size) const
{
   swapFourBytes(data, size);
}

void ossimEndian::swap(ossim_float64* data, ossim_uint32 size) const
{
   swapEightBytes(data, size);
}

void ossimEndian::swapTwoBytes(void* data, ossim_uint32 size) const
{
   ossim_uint16* buf = reinterpret_cast<ossim_uint16*>(data);
   for (ossim_uint32 i=0; i<size; ++i)
   {
      buf[i] = (buf[i] << 8) | (buf[i] >> 8);
   }
}

void ossimEndian::swapFourBytes(void* data, ossim_uint32 size) const
{
   ossim_uint32* buf = reinterpret_cast<ossim_uint32*>(data);
   for (ossim_uint32 i=0; i<size; ++i)
   {
      buf[i]
         = (  ((buf[i] & 0xff000000) >> 24)
            | ((buf[i] & 0x00ff0000) >> 8)
            | ((buf[i] & 0x0000ff00) << 8)
            | ((buf[i] & 0x000000ff) << 24));
   }
}

void ossimEndian::swapEightBytes(void* data, ossim_uint32 size) const
{
#ifdef HAS_LONG_LONG
   uint64* buf = reinterpret_cast<ossim_uint64*>(data);
   for (uint32 i=0; i<size; ++i)
   {
      buf[i]
         = (  ((buf[i] & 0xff00000000000000ull) >> 56)
            | ((buf[i] & 0x00ff000000000000ull) >> 40)
            | ((buf[i] & 0x0000ff0000000000ull) >> 24)
            | ((buf[i] & 0x000000ff00000000ull) >> 8)
            | ((buf[i] & 0x00000000ff000000ull) << 8)
            | ((buf[i] & 0x0000000000ff0000ull) << 24)
            | ((buf[i] & 0x000000000000ff00ull) << 40)
            | ((buf[i] & 0x00000000000000ffull) << 56));
   }
#else
   ossim_float64* buf = reinterpret_cast<ossim_float64*>(data);
   for (ossim_uint32 i=0; i<size; ++i)
   {
      swapEightBytes(buf+i);
   }
   
#endif
   
}
