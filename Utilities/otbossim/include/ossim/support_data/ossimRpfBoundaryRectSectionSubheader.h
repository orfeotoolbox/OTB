//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfBoundaryRectSectionSubheader.h 9967 2006-11-29 02:01:23Z gpotts $
#ifndef ossimRpfBoundaryRectSectionSubheader_HEADER
#define ossimRpfBoundaryRectSectionSubheader_HEADER
#include <iostream>
using namespace std;

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorContext.h>

class ossimRpfBoundaryRectSectionSubheader
{
public:
   friend ostream& operator <<(ostream& out,
                               const ossimRpfBoundaryRectSectionSubheader &data);
   ossimRpfBoundaryRectSectionSubheader();
   virtual ~ossimRpfBoundaryRectSectionSubheader(){}

   ossimErrorCode parseStream(istream& in, ossimByteOrder);
   virtual void print(ostream& out)const;
   ossim_uint16 getNumberOfEntries()const{return theNumberOfEntries;}
   ossim_uint32 getTableOffset()const{return theRectangleTableOffset;}
   ossim_uint16 getLengthOfEachEntry()const{return theLengthOfEachEntry;}

   void clearFields();
private:   
   ossim_uint32 theRectangleTableOffset;
   ossim_uint16 theNumberOfEntries;
   ossim_uint16 theLengthOfEachEntry;
};

#endif
