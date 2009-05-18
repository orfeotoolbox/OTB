//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
// 
// Author: Garrett Potts
// 
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfCoverageSection.cpp 14241 2009-04-07 19:59:23Z dburken $

#include <istream>
#include <iostream>
#include <ossim/support_data/ossimRpfCoverageSection.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimErrorCodes.h>

std::ostream& operator <<(std::ostream &out,
                          const ossimRpfCoverageSection &data)
{
   data.print(out);
   
   return out;
}

ossimRpfCoverageSection::ossimRpfCoverageSection()
{
   clearFields();
}

ossimErrorCode ossimRpfCoverageSection::parseStream(std::istream &in,
                                                    ossimByteOrder byteOrder)
{
   if(in)
   {
      ossimEndian anEndian;


      in.read((char*)&theUpperLeftLat,  8);
      in.read((char*)&theUpperLeftLon,  8);
      in.read((char*)&theLowerLeftLat,  8);
      in.read((char*)&theLowerLeftLon,  8);
      in.read((char*)&theUpperRightLat, 8);
      in.read((char*)&theUpperRightLon, 8);
      in.read((char*)&theLowerRightLat, 8);
      in.read((char*)&theLowerRightLon, 8);
      in.read((char*)&theVerticalResolution,   8);
      in.read((char*)&theHorizontalResolution, 8);
      in.read((char*)&theVerticalInterval,     8);
      in.read((char*)&theHorizontalInterval,   8);

      if(anEndian.getSystemEndianType() != byteOrder)
      {
         anEndian.swap(theUpperLeftLat);
         anEndian.swap(theUpperLeftLon);
         anEndian.swap(theLowerLeftLat);
         anEndian.swap(theLowerLeftLon);
         anEndian.swap(theUpperRightLat);
         anEndian.swap(theUpperRightLon);
         anEndian.swap(theLowerRightLat);
         anEndian.swap(theLowerRightLon);
         anEndian.swap(theVerticalResolution);
         anEndian.swap(theHorizontalResolution);
         anEndian.swap(theVerticalInterval);
         anEndian.swap(theHorizontalInterval);
      }
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }

   return ossimErrorCodes::OSSIM_OK;
}

std::ostream& ossimRpfCoverageSection::print(
   std::ostream& out, const std::string& prefix) const
{
   out << prefix << "ul_lat: "
       << theUpperLeftLat << "\n"
       << prefix << "ul_lon: "
       << theUpperLeftLon << "\n"
       << prefix << "ll_lat: "
       << theLowerLeftLat << "\n"
       << prefix << "ll_lon: "
       << theLowerLeftLon << "\n"
       << prefix << "ur_lat: "
       << theUpperRightLat << "\n"
       << prefix << "ur_lon: "
       << theUpperRightLon << "\n"
       << prefix << "lr_lat: "
       << theLowerRightLat << "\n"
       << prefix << "lr_lon: "
       << theLowerRightLon << "\n"
       << prefix << "vertical_resolution: "
       << theVerticalResolution << "\n"
       << prefix << "horizontal_resolution: "
       << theHorizontalResolution << "\n"
       << prefix << "vertical_interval: "
       << theVerticalInterval << "\n"
       << prefix << "horizontal_interval: "
       << theHorizontalInterval << std::endl;
   return out;
}

void ossimRpfCoverageSection::clearFields()
{
   theUpperLeftLat  = 0.0;
   theUpperLeftLon  = 0.0;
   theLowerLeftLat  = 0.0;
   theLowerLeftLon  = 0.0;
   theUpperRightLat = 0.0;
   theUpperRightLon = 0.0;
   theLowerRightLat = 0.0;
   theLowerRightLon = 0.0;
   theVerticalResolution   = 0.0;
   theHorizontalResolution = 0.0;
   theVerticalInterval     = 0.0;
   theHorizontalInterval   = 0.0;
}
