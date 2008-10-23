//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfFrameFileIndexRecord.cpp 9963 2006-11-28 21:11:01Z gpotts $
#include <ossim/support_data/ossimRpfFrameFileIndexRecord.h>
#include <ossim/base/ossimEndian.h>
#include <string.h> // for memset
#include <ossim/base/ossimErrorCodes.h>

ostream& operator <<(ostream& out,
                     const ossimRpfFrameFileIndexRecord& data)
{
   data.print(out);
   
   return out;
}

ossimRpfFrameFileIndexRecord::ossimRpfFrameFileIndexRecord()
{
   clearFields();
}

ossimErrorCode ossimRpfFrameFileIndexRecord::parseStream(istream& in, ossimByteOrder byteOrder)
{
   if(in)
   {
      ossimEndian anEndian;

      clearFields();
            
      in.read((char*)&theBoundaryRectRecordNumber, 2);
      in.read((char*)&theLocationRowNumber, 2);
      in.read((char*)&theLocationColumnNumber, 2);
      in.read((char*)&thePathnameRecordOffset, 4);
      in.read((char*)theFilename, 12);
      in.read((char*)theGeographicLocation, 6);
      in.read((char*)&theSecurityClassification, 1);
      in.read((char*)theFileSecurityCountryCode, 2);
      in.read((char*)theFileSecurityReleaseMarking, 2);

      if(anEndian.getSystemEndianType() != byteOrder)
      {
         anEndian.swap(theBoundaryRectRecordNumber);
         anEndian.swap(theLocationRowNumber);
         anEndian.swap(theLocationColumnNumber);
         anEndian.swap(thePathnameRecordOffset);
      }
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }
   
   return ossimErrorCodes::OSSIM_OK;
}

void ossimRpfFrameFileIndexRecord::print(ostream& out)const
{
   out << "theBoundaryRectRecordNumber:      " << theBoundaryRectRecordNumber << endl
       << "theLocationRowNumber:             " << theLocationRowNumber << endl
       << "theLocationColumnNumber:          " << theLocationColumnNumber << endl
       << "thePathnameRecordOffset:          " << thePathnameRecordOffset << endl
       << "theFilename:                      " << theFilename << endl
       << "theGeographicLocation:            " << theGeographicLocation << endl
       << "theSecurityClassification:        " << theSecurityClassification << endl
       << "theFileSecurityCountryCode:       " << theFileSecurityCountryCode << endl
       << "theFileSecurityReleaseMarking:    " << theFileSecurityReleaseMarking;
}

void ossimRpfFrameFileIndexRecord::clearFields()
{
   theBoundaryRectRecordNumber = 0;
   theLocationRowNumber = 0;
   theLocationColumnNumber= 0;
   thePathnameRecordOffset = 0;
   memset(theFilename, ' ', 12);
   memset(theGeographicLocation, ' ', 6);
   theSecurityClassification = ' ';;
   memset(theFileSecurityCountryCode, ' ', 2);
   memset(theFileSecurityReleaseMarking, ' ', 2);

   theFilename[12] = '\0';
   theGeographicLocation[6] = '\0';
   theFileSecurityCountryCode[2] = '\0';
   theFileSecurityReleaseMarking[2] = '\0';
}

