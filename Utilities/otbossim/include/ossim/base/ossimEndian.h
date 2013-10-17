//******************************************************************
//
// License:  See top level LICENSE.txt file.
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

// for OSSIM_LITTLE_ENDIAN AND BIG_ENDIAN
#include <ossim/base/ossimConstants.h>

class OSSIMDLLEXPORT ossimEndian
{
public:
   ossimEndian();

   /*!
    *  Swap bytes methods that work on a single pixel.
    */
   inline void swap(ossim_sint8    &/*data*/){}
   inline void swap(ossim_uint8    &/*data*/){}
   inline void swap(ossim_int16   &data) const;
   inline void swap(ossim_uint16  &data) const;
   inline void swap(ossim_int32   &data) const;
   inline void swap(ossim_uint32  &data) const;
   inline void swap(ossim_uint64  &data) const;
   inline void swap(ossim_sint64  &data) const;
   inline void swap(ossim_float32 &data) const;
   inline void swap(ossim_float64 &data) const;

   /*!
    *  Swap bytes methods that work on arrays of pixels.
    *
    *  @note size is number of pixels, not number of bytes.
    */
   inline void swap(ossimScalarType scalar, void* data, ossim_uint32 size) const;
   // only here to allow template based swaps to compile correctly
   //
   inline void swap(ossim_sint8* data, ossim_uint32 size)const;
   inline void swap(ossim_uint8* data, ossim_uint32 size)const;
   
   inline void swap(ossim_int16* data, ossim_uint32 size) const;
   inline void swap(ossim_uint16* data, ossim_uint32 size) const;

   inline void swap(ossim_int32* data, ossim_uint32 size) const;
   inline void swap(ossim_uint32* data, ossim_uint32 size) const;

   inline void swap(ossim_int64* data, ossim_uint32 size) const;
   inline void swap(ossim_uint64* data, ossim_uint32 size) const;

   inline void swap(ossim_float32* data, ossim_uint32 size) const;
   inline void swap(ossim_float64* data, ossim_uint32 size) const;

   inline void swapTwoBytes(void* data, ossim_uint32 size) const;
   inline void swapFourBytes(void* data, ossim_uint32 size) const;
   inline void swapEightBytes(void* data, ossim_uint32 size) const;
   inline ossimByteOrder getSystemEndianType() const;

private:

   // Holds the Endian of the architecture that you are running on.
   ossimByteOrder  theSystemEndianType;  
   
   void swapTwoBytes(void *data) const;
   void swapFourBytes(void *data) const;
   void swapEightBytes(void *data) const;

   void swapPrivate(ossim_uint8 *c1,
                    ossim_uint8 *c2) const;
};

inline ossimByteOrder ossimEndian::getSystemEndianType() const
{
   return theSystemEndianType;
}

inline void ossimEndian::swap(ossim_sint8* /* data */,
                              ossim_uint32 /* size */ )const
{
   //intentionally left blank
}

inline void ossimEndian::swap(ossim_uint8* /* data */,
                              ossim_uint32 /* size */ )const
{
   //intentionally left blank
}

inline void ossimEndian::swap(ossim_int16 &data) const
{
   swapTwoBytes(reinterpret_cast<void*>(&data));   
} 

inline void ossimEndian::swap(ossim_uint16 &data) const
{
   swapTwoBytes(reinterpret_cast<void*>(&data));
}

inline void ossimEndian::swap(ossim_int32 &data) const
{
   swapFourBytes(reinterpret_cast<void*>(&data));
}

inline void ossimEndian::swap(ossim_uint32 &data) const
{
   swapFourBytes(reinterpret_cast<void*>(&data));
}

inline void ossimEndian::swap(ossim_uint64 &data) const
{
   swapEightBytes(reinterpret_cast<void*>(&data));
}

inline void ossimEndian::swap(ossim_sint64 &data) const
{
   swapEightBytes(reinterpret_cast<void*>(&data));
}

inline void ossimEndian::swap(ossim_float32 &data) const
{
   swapFourBytes(reinterpret_cast<void*>(&data));
}

inline void ossimEndian::swap(ossim_float64 &data) const
{
   swapEightBytes(reinterpret_cast<void*>(&data));
}

inline void ossimEndian::swapTwoBytes(void *data) const
{
   unsigned char *c = reinterpret_cast<unsigned char*>(data);

   swapPrivate(&c[0], &c[1]);
}

inline void ossimEndian::swapFourBytes(void* data) const
{
   unsigned char *c = reinterpret_cast<unsigned char*>(data);

   swapPrivate(&c[0], &c[3]);
   swapPrivate(&c[1], &c[2]);
}

inline void ossimEndian::swapEightBytes(void* data) const
{
   unsigned char *c = reinterpret_cast<unsigned char*>(data);

   swapPrivate(&c[0], &c[7]);
   swapPrivate(&c[1], &c[6]);
   swapPrivate(&c[2], &c[5]);
   swapPrivate(&c[3], &c[4]);
}

inline void ossimEndian::swapPrivate(ossim_uint8 *c1,                       
                                     ossim_uint8 *c2) const
{
   ossim_uint8 temp_c = *c1;
   *c1 = *c2;
   *c2 = temp_c;
}

inline void ossimEndian::swap(ossimScalarType scalar,
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

inline void ossimEndian::swap(ossim_int16* data, ossim_uint32 size) const
{
   swapTwoBytes(data, size);
}

inline void ossimEndian::swap(ossim_uint16* data, ossim_uint32 size) const
{
   swapTwoBytes(data, size);
}

inline void ossimEndian::swap(ossim_int32* data, ossim_uint32 size) const
{
   swapFourBytes(data, size);
}

inline void ossimEndian::swap(ossim_uint32* data, ossim_uint32 size) const
{
   swapFourBytes(data, size);
}

inline void ossimEndian::swap(ossim_int64* data, ossim_uint32 size) const
{
   swapEightBytes(data, size);
}

inline void ossimEndian::swap(ossim_uint64* data, ossim_uint32 size) const
{
   swapEightBytes(data, size);
}

inline void ossimEndian::swap(ossim_float32* data, ossim_uint32 size) const
{
   swapFourBytes(data, size);
}

inline void ossimEndian::swap(ossim_float64* data, ossim_uint32 size) const
{
   swapEightBytes(data, size);
}

inline void ossimEndian::swapTwoBytes(void* data, ossim_uint32 size) const
{
   ossim_uint16* buf = reinterpret_cast<ossim_uint16*>(data);
   for (ossim_uint32 i=0; i<size; ++i)
   {
      buf[i] = ((buf[i] & 0x00ff) << 8) | ((buf[i] & 0xff00) >> 8);
   }
}

inline void ossimEndian::swapFourBytes(void* data, ossim_uint32 size) const
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

inline void ossimEndian::swapEightBytes(void* data, ossim_uint32 size) const
{
   ossim_uint64* buf = reinterpret_cast<ossim_uint64*>(data);
   for (ossim_uint32 i=0; i<size; ++i)
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
}

#endif /* End of #ifndef ossimEndian_HEADER */
