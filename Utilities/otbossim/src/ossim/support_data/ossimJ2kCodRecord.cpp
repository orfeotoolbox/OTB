//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Container class for J2K Coding style default (COD) record.
//
// See document BPJ2K01.00 Table 7-7 Image and tile size (15444-1 Annex A5.1)
// 
//----------------------------------------------------------------------------
// $Id: ossimJ2kCodRecord.h,v 1.5 2005/10/13 21:24:47 dburken Exp $

#include <iostream>
#include <iomanip>

#include <ossim/support_data/ossimJ2kCodRecord.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimEndian.h>


ossimJ2kCodRecord::ossimJ2kCodRecord()
   :
   theMarker(0xff52),
   theLcod(0),
   theScod(0),
   theSGcod(0),
   theSPcod(0)
{
}

ossimJ2kCodRecord::~ossimJ2kCodRecord()
{
}

void ossimJ2kCodRecord::parseStream(std::istream& in)
{
   // Get the stream posistion.
   std::streamoff pos = in.tellg();

   // Note: Marker is not read.
   in.read((char*)&theLcod,      2);
   in.read((char*)&theScod,      1);
   in.read((char*)&theSGcod,     4);
   in.read((char*)&theSPcod,     4);

   if (ossim::byteOrder() == OSSIM_LITTLE_ENDIAN)
   {
      // Stored big endian, must swap.
      ossimEndian s;
      s.swap(theLcod);
      s.swap(theSGcod);
   }

   //---
   // Seek to next record.  This is needed because there are sometimes extra
   // bytes.
   //---
   in.seekg(pos + theLcod, std::ios_base::beg);
}

std::ostream& ossimJ2kCodRecord::print(std::ostream& out,
                                       const std::string& prefix) const
{
   // Capture the original flags.
   std::ios_base::fmtflags f = out.flags();

   std::string pfx = prefix;
   pfx += "cod.";

   out.setf(std::ios_base::hex, std::ios_base::basefield);
   out << pfx << "marker: 0x" << theMarker << "\n";
   out.setf(std::ios_base::fmtflags(0), std::ios_base::basefield);

   out << pfx << "Lcod:   " << theLcod       << "\n"
       << pfx << "Scod:   " << int(theScod)  << "\n"
       << pfx << "SGcod:  " << theSGcod      << "\n"
       << pfx << "SPcod:  " << int(theSPcod)
       << std::endl;

   // Reset flags.
   out.setf(f);

   return out;
}

std::ostream& operator<<(std::ostream& out, const ossimJ2kCodRecord& obj)
{
   return obj.print(out);
}
