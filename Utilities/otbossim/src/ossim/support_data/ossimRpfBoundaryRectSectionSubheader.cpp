//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfBoundaryRectSectionSubheader.cpp 9963 2006-11-28 21:11:01Z gpotts $
#include <ossim/support_data/ossimRpfBoundaryRectSectionSubheader.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimErrorCodes.h>

ostream& operator <<(ostream& out,
                     const ossimRpfBoundaryRectSectionSubheader &data)
{
   data.print(out);

   return out;
}

ossimRpfBoundaryRectSectionSubheader::ossimRpfBoundaryRectSectionSubheader()
{
   clearFields();
}

ossimErrorCode ossimRpfBoundaryRectSectionSubheader::parseStream(istream& in,
                                                                 ossimByteOrder byteOrder)
{
   if(in)
   {
      ossimEndian anEndian;

      clearFields();
      
      in.read((char*)&theRectangleTableOffset, 4);
      in.read((char*)&theNumberOfEntries, 2);
      in.read((char*)&theLengthOfEachEntry, 2);

      if(anEndian.getSystemEndianType() != byteOrder)
      {
         anEndian.swap(theRectangleTableOffset);
         anEndian.swap(theNumberOfEntries);
         anEndian.swap(theLengthOfEachEntry);
      }
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }
   
   return ossimErrorCodes::OSSIM_OK;
}

void ossimRpfBoundaryRectSectionSubheader::print(ostream& out)const
{
   out << "theRectangleTableOffset:      " << theRectangleTableOffset << endl
       << "theNumberOfEntries:           " << theNumberOfEntries << endl
       << "theLengthOfEachEntry:         " << theLengthOfEachEntry;
}

void ossimRpfBoundaryRectSectionSubheader::clearFields()
{
   theRectangleTableOffset = 0;
   theNumberOfEntries      = 0;
   theLengthOfEachEntry    = 0;
}
