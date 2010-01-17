//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts  (gpotts@imagelinks.com)
//
//*******************************************************************
// $Id: ossimAigDataFileHeader.cpp 9963 2006-11-28 21:11:01Z gpotts $
#include <ossim/support_data/ossimAigDataFileHeader.h>
#include <ossim/base/ossimEndian.h>

bool ossimAigDataFileHeader::writeStream(std::ostream& out)const
{
   ossimEndian endian;
   ossim_uint32 tempInt;
   
   if(endian.getSystemEndianType() == OSSIM_LITTLE_ENDIAN)
   {
      out.write((char*)theMagicNumber, 8);
      out.write((char*)theZeroFill1, 16);
      tempInt = theFileSize;
      endian.swap(tempInt);
      out.write((char*)(&tempInt), 4);
      out.write((char*)theZeroFill2, 72);
   }
   else
   {
      out.write((char*)theMagicNumber, 8);
      out.write((char*)theZeroFill1, 16);
      out.write((char*)(&theFileSize), 8);
      out.write((char*)theZeroFill2, 72);
   }
   
   return out.good();
}
