//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Container class for J2K Image and tile size (SIZ) record.
//
// See document BPJ2K01.00 Table 7-6 Image and tile size (15444-1 Annex A5.1)
// 
//----------------------------------------------------------------------------
// $Id: ossimJ2kSizRecord.h,v 1.5 2005/10/13 21:24:47 dburken Exp $

#include <iostream>
#include <iomanip>

#include <ossim/support_data/ossimJ2kSizRecord.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimEndian.h>


ossimJ2kSizRecord::ossimJ2kSizRecord()
   :
   theMarker(0xff51),
   theLsiz(0),
   theRsiz(0),
   theXsiz(0),
   theYsiz(0),
   theXOsiz(0),
   theYOsiz(0),
   theXTsiz(0),
   theYTsiz(0),
   theXTOsiz(0),
   theYTOsiz(0),
   theCsiz(0),
   theSsiz(0),
   theXRsiz(0),
   theYRsiz(0)
{
}

ossimJ2kSizRecord::~ossimJ2kSizRecord()
{
}

void ossimJ2kSizRecord::parseStream(std::istream& in)
{
   // Get the stream posistion.
   std::streamoff pos = in.tellg();

   // Note: Marker is not read.
   in.read((char*)&theLsiz,      2);
   in.read((char*)&theRsiz,      2);
   in.read((char*)&theXsiz,      4);
   in.read((char*)&theYsiz,      4);
   in.read((char*)&theXOsiz,     4);
   in.read((char*)&theYOsiz,     4);
   in.read((char*)&theXTsiz,     4);
   in.read((char*)&theYTsiz,     4);
   in.read((char*)&theXTOsiz,    4);
   in.read((char*)&theYTOsiz,    4);
   in.read((char*)&theCsiz,      2);
   in.read((char*)&theSsiz,      1);
   in.read((char*)&theXRsiz,     1);
   in.read((char*)&theYRsiz,     1);

   if (ossim::byteOrder() == OSSIM_LITTLE_ENDIAN)
   {
      // Stored big endian, must swap.
      ossimEndian s;
      s.swap(theLsiz);
      s.swap(theRsiz);
      s.swap(theXsiz);
      s.swap(theYsiz);
      s.swap(theXOsiz);
      s.swap(theYOsiz);
      s.swap(theXTsiz);
      s.swap(theYTsiz);
      s.swap(theXTOsiz);
      s.swap(theYTOsiz);
      s.swap(theCsiz);
   }

   //---
   // Seek to next record.  This is needed because there are sometimes extra
   // bytes.
   //---
   in.seekg(pos + theLsiz, std::ios_base::beg);
}

ossimScalarType ossimJ2kSizRecord::getScalarType() const
{
   ossimScalarType result = OSSIM_SCALAR_UNKNOWN;

   // Bits per pixel first seven bits plus one.
   ossim_uint8 bpp = ( theSsiz & 0x3f ) + 1;

   // Signed bit is msb.
   bool isSigned = ( theSsiz & 0x80 ) ? true : false;

   // std::cout << "bpp: " << int(bpp) << " signed: " << isSigned << std::endl;
      
   if(bpp <= 8)
   {
      if(isSigned == 0)
      {
         result = OSSIM_UINT8;
      }
      else if(isSigned == 1)
      {
         result = OSSIM_SINT8;
      }
   }
   else if(bpp <= 16)
   {
      if(isSigned == 0)
      {
         result = OSSIM_UINT16;
      }
      else if(isSigned == 1)
      {
         result = OSSIM_SINT16;
      }
   }
   return result;
}

std::ostream& ossimJ2kSizRecord::print(std::ostream& out,
                                       const std::string& prefix) const
{
   // Capture the original flags.
   std::ios_base::fmtflags f = out.flags();

   std::string pfx = prefix;
   pfx += "siz.";

   out.setf(std::ios_base::hex, std::ios_base::basefield);
   out << pfx << "marker: 0x" << theMarker << "\n";
   out.setf(std::ios_base::fmtflags(0), std::ios_base::basefield);

   out << pfx << "Lsiz:   " << theLsiz       << "\n"
       << pfx << "Rsiz:   " << theRsiz       << "\n"
       << pfx << "Xsiz:   " << theXsiz       << "\n"
       << pfx << "Yziz:   " << theYsiz       << "\n"
       << pfx << "XOsiz:  " << theXOsiz      << "\n"
       << pfx << "YOsiz:  " << theYOsiz      << "\n"
       << pfx << "XTsiz:  " << theXTsiz      << "\n"
       << pfx << "YTsiz:  " << theYTsiz      << "\n"
       << pfx << "XTOsiz: " << theXTOsiz     << "\n"
       << pfx << "YTOsiz: " << theYTOsiz     << "\n"
       << pfx << "Csiz:   " << theCsiz       << "\n"
       << pfx << "Ssiz:   " << int(theSsiz)  << "\n"
       << pfx << "XRsiz:  " << int(theXRsiz) << "\n"
       << pfx << "YRsiz:  " << int(theYRsiz)
       << std::endl;

   // Reset flags.
   out.setf(f);

   return out;
}

std::ostream& operator<<(std::ostream& out, const ossimJ2kSizRecord& obj)
{
   return obj.print(out);
}
