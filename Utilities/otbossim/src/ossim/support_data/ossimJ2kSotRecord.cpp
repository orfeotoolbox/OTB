//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// // Description: Container class for J2K "Start Of Tile" (SOT) record.
//
// See document BPJ2K01.00 Table 7-3 Image and tile size (15444-1 Annex A.4.2)
// 
//----------------------------------------------------------------------------
// $Id: ossimJ2kSotRecord.h,v 1.5 2005/10/13 21:24:47 dburken Exp $

#include <iostream>
#include <iomanip>

#include <ossim/support_data/ossimJ2kSotRecord.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimEndian.h>

ossimJ2kSotRecord::ossimJ2kSotRecord()
   :
   theMarker(0xff90),
   theLsot(0),
   theIsot(0),
   thePsot(0),
   theTpsot(0),
   theTnsot(0)
{
}

ossimJ2kSotRecord::~ossimJ2kSotRecord()
{
}

void ossimJ2kSotRecord::parseStream(std::istream& in)
{
   // Note: marker not read...
   
   in.read((char*)&theLsot,  2);
   in.read((char*)&theIsot,  2);
   in.read((char*)&thePsot,  4);
   in.read((char*)&theTpsot, 1);
   in.read((char*)&theTnsot, 1);

   if (ossim::byteOrder() == OSSIM_LITTLE_ENDIAN)
   {
      // Stored big endian, must swap.
      ossimEndian s;
      s.swap(theLsot);
      s.swap(theIsot);
      s.swap(thePsot);
   }
}

std::ostream& ossimJ2kSotRecord::print(std::ostream& out,
                                       const std::string& prefix) const
{
   // Capture the original flags.
   std::ios_base::fmtflags f = out.flags();

   std::string pfx = prefix;
   pfx += "sot.";

   out.setf(std::ios_base::hex, std::ios_base::basefield);
   out << pfx << "marker: 0x" << theMarker << "\n";
   out.setf(std::ios_base::fmtflags(0), std::ios_base::basefield);

   out << pfx << "Lsot:   "  << theLsot      << "\n"
       << pfx << "Isot:   "  << theIsot      << "\n"
       << pfx << "Psot:   "  << thePsot      << "\n"
       << pfx << "Tpsot:  " << int(theTpsot) << "\n"
       << pfx << "Tnsot:  " << int(theTnsot)
       << std::endl;

   // Reset flags.
   out.setf(f);

   return out;
}

std::ostream& operator<<(std::ostream& out, const ossimJ2kSotRecord& obj)
{
   return obj.print(out);
}
