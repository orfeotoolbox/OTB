//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfFrameFileIndexRecord.h 9967 2006-11-29 02:01:23Z gpotts $
#ifndef ossimRpfFrameFileIndexRecord_HEADER
#define ossimRpfFrameFileIndexRecord_HEADER
#include <iostream>
using namespace std;
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorContext.h>
#include <ossim/base/ossimFilename.h>

class ossimRpfFrameFileIndexRecord
{
public:
   friend ostream& operator <<(ostream& out,
                               const ossimRpfFrameFileIndexRecord& data);
   
   ossimRpfFrameFileIndexRecord();
   ossimErrorCode parseStream(istream& in, ossimByteOrder byteOrder);
   void clearFields();
   void print(ostream& out)const;
   
   ossim_uint16 getBoundaryRecNumber()const{return theBoundaryRectRecordNumber;}
   ossim_uint16  getLocationRowNumber()const{return theLocationRowNumber;}
   ossim_uint16  getLocationColNumber()const{return theLocationColumnNumber;}
   ossim_uint32  getPathnameRecordOffset()const{return thePathnameRecordOffset;}
   ossimFilename getFilename()const{return ossimFilename(theFilename);}
   
private:
   ossim_uint16 theBoundaryRectRecordNumber;
   ossim_uint16 theLocationRowNumber;
   ossim_uint16 theLocationColumnNumber;
   ossim_uint32 thePathnameRecordOffset;

   /*!
    * This is a 12 byte asci field.
    */
   char           theFilename[13];

   /*!
    * this is a 6 byte asci field.
    */
   char           theGeographicLocation[7];

   char           theSecurityClassification;

   /*!
    * is a 2 byte field.
    */
   char           theFileSecurityCountryCode[3];

   /*!
    * This is a 2 byte field.
    */
   char           theFileSecurityReleaseMarking[3];
};

#endif
