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
// $Id: ossimRpfCoverageSection.cpp 16997 2010-04-12 18:53:48Z dburken $

#include <istream>
#include <iostream>

#include <ossim/support_data/ossimRpfCoverageSection.h>
#include <ossim/base/ossimCommon.h> /* ossim::byteOrder */
#include <ossim/base/ossimEndian.h>

std::ostream& operator <<(std::ostream &out, const ossimRpfCoverageSection &data)
{
   return data.print(out);
}

ossimRpfCoverageSection::ossimRpfCoverageSection()
   :
   theUpperLeftLat(0.0),
   theUpperLeftLon(0.0),

   theLowerLeftLat(0.0),
   theLowerLeftLon(0.0),

   theUpperRightLat(0.0),
   theUpperRightLon(0.0),

   theLowerRightLat(0.0),
   theLowerRightLon(0.0),

   theVerticalResolution(0.0),
   theHorizontalResolution(0.0),

   theVerticalInterval(0.0),
   theHorizontalInterval(0.0)
{
}

ossimRpfCoverageSection::ossimRpfCoverageSection(const ossimRpfCoverageSection& obj)
   :
   theUpperLeftLat(obj.theUpperLeftLat),
   theUpperLeftLon(obj.theUpperLeftLon),

   theLowerLeftLat(obj.theLowerLeftLat),
   theLowerLeftLon(obj.theLowerLeftLon),

   theUpperRightLat(obj.theUpperRightLat),
   theUpperRightLon(obj.theUpperRightLon),

   theLowerRightLat(obj.theLowerRightLat),
   theLowerRightLon(obj.theLowerRightLon),

   theVerticalResolution(obj.theVerticalResolution),
   theHorizontalResolution(obj.theHorizontalResolution),

   theVerticalInterval(obj.theVerticalInterval),
   theHorizontalInterval(obj.theHorizontalInterval)
{
}

const ossimRpfCoverageSection& ossimRpfCoverageSection::operator=(
   const ossimRpfCoverageSection& rhs)
{
   if ( this != & rhs )
   {
      theUpperLeftLat = rhs.theUpperLeftLat;
      theUpperLeftLon = rhs.theUpperLeftLon;
      
      theLowerLeftLat = rhs.theLowerLeftLat;
      theLowerLeftLon = rhs.theLowerLeftLon;
      
      theUpperRightLat = rhs.theUpperRightLat;
      theUpperRightLon = rhs.theUpperRightLon;
      
      theLowerRightLat = rhs.theLowerRightLat;
      theLowerRightLon = rhs.theLowerRightLon;
      
      theVerticalResolution = rhs.theVerticalResolution;
      theHorizontalResolution = rhs.theHorizontalResolution;
      
      theVerticalInterval = rhs.theVerticalInterval;
      theHorizontalInterval = rhs.theHorizontalInterval;
   }
   return *this;
}

ossimErrorCode ossimRpfCoverageSection::parseStream(std::istream &in,
                                                    ossimByteOrder byteOrder)
{
   if(in)
   {
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

      if( ossim::byteOrder()  != byteOrder )
      {
         ossimEndian anEndian;
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

void ossimRpfCoverageSection::writeStream(std::ostream& out)
{
   ossimEndian anEndian;

   if( anEndian.getSystemEndianType() != OSSIM_BIG_ENDIAN )
   {
      // Always write out big endian.
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
   
   out.write((char*)&theUpperLeftLat,  8);
   out.write((char*)&theUpperLeftLon,  8);
   out.write((char*)&theLowerLeftLat,  8);
   out.write((char*)&theLowerLeftLon,  8);
   out.write((char*)&theUpperRightLat, 8);
   out.write((char*)&theUpperRightLon, 8);
   out.write((char*)&theLowerRightLat, 8);
   out.write((char*)&theLowerRightLon, 8);
   out.write((char*)&theVerticalResolution,   8);
   out.write((char*)&theHorizontalResolution, 8);
   out.write((char*)&theVerticalInterval,     8);
   out.write((char*)&theHorizontalInterval,   8);
   
   if( anEndian.getSystemEndianType() != OSSIM_BIG_ENDIAN )
   {
      // Swap back to native byte order.
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

void ossimRpfCoverageSection::setUlLat(ossim_float64 value)
{
   theUpperLeftLat = value;
}

void ossimRpfCoverageSection::setUlLon(ossim_float64 value)
{
   theUpperLeftLon = value;
}

void ossimRpfCoverageSection::setLlLat(ossim_float64 value)
{
   theLowerLeftLat = value;
}

void ossimRpfCoverageSection::setLlLon(ossim_float64 value)
{
   theLowerLeftLon = value;
}

void ossimRpfCoverageSection::setLrLat(ossim_float64 value)
{
   theLowerRightLat = value;
}

void ossimRpfCoverageSection::setLrLon(ossim_float64 value)
{
   theLowerRightLon = value;
}

void ossimRpfCoverageSection::setUrLat(ossim_float64 value)
{
   theUpperRightLat = value;
}

void ossimRpfCoverageSection::setUrLon(ossim_float64 value)
{
   theUpperRightLon = value;
}

void ossimRpfCoverageSection::setVerticalResolution(ossim_float64 value)
{
   theVerticalResolution = value;
}

void ossimRpfCoverageSection::setHorizontalResolution(ossim_float64 value)
{
   theHorizontalResolution = value;
}

void ossimRpfCoverageSection::setVerticalInterval(ossim_float64 value)
{
   theVerticalInterval = value;
}

void ossimRpfCoverageSection::setHorizontalInterval(ossim_float64 value)
{
   theHorizontalInterval = value;
}
