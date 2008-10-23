//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfImageDisplayParameterSubheader.h 9967 2006-11-29 02:01:23Z gpotts $
#ifndef ossimRpfImageDisplayParameterSubheader_HEADER
#define ossimRpfImageDisplayParameterSubheader_HEADER
#include <iostream>
using namespace std;

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorContext.h>

class ossimRpfImageDisplayParameterSubheader
{
public:
   friend ostream& operator<<(ostream& out,
                              const ossimRpfImageDisplayParameterSubheader& data);
   ossimRpfImageDisplayParameterSubheader();
   ossimErrorCode parseStream(istream& in, ossimByteOrder byteOrder);
   
   void print(ostream& out)const;

   ossim_uint32 getStartOffset()const
      {
         return theStartOffset;
      }
   ossim_uint32 getEndOffset()const
      {
         return theEndOffset;
      }
   ossim_uint32 getNumberOfImageRows()const
      {
         return theNumberOfImageRows;
      }
   ossim_uint32 getNumberOfImageCodesPerRow()const
      {
         return theNumberOfImageCodesPerRow;
      }
   ossim_uint32 getImageCodeLength()const
      {
         return theImageCodeBitLength;
      }
   
private:
   void clearFields();

   ossim_uint32 theStartOffset;
   ossim_uint32 theEndOffset;
   
   ossim_uint32 theNumberOfImageRows;
   ossim_uint32 theNumberOfImageCodesPerRow;
   ossim_uint8  theImageCodeBitLength;
};

#endif
