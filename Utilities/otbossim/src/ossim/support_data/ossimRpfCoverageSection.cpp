//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfCoverageSection.cpp 9963 2006-11-28 21:11:01Z gpotts $
#include <ossim/support_data/ossimRpfCoverageSection.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimErrorCodes.h>

ostream& operator <<(ostream &out,
                     const ossimRpfCoverageSection &data)
{
   data.print(out);
   
   return out;
}

ossimRpfCoverageSection::ossimRpfCoverageSection()
{
   clearFields();
}

ossimErrorCode ossimRpfCoverageSection::parseStream(istream &in, ossimByteOrder byteOrder)
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

void ossimRpfCoverageSection::print(ostream &out)const
{
   out << "theUpperLeftLat:         " << theUpperLeftLat << endl
       << "theUpperLeftLon:         " << theUpperLeftLon << endl
       << "theLowerLeftLat:         " << theLowerLeftLat << endl
       << "theLowerLeftLon:         " << theLowerLeftLon << endl
       << "theUpperRightLat:        " << theUpperRightLat << endl
       << "theUpperRightLon:        " << theUpperRightLon << endl
       << "theLowerRightLat:        " << theLowerRightLat << endl
       << "theLowerRightLon:        " << theLowerRightLon << endl
       << "theVerticalResolution:   " << theVerticalResolution << endl
       << "theHorizontalResolution: " << theHorizontalResolution << endl
       << "theVerticalInterval:     " << theVerticalInterval << endl
       << "theHorizontalInterval:   " << theHorizontalInterval;
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
