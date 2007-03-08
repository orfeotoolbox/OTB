//*******************************************************************
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
// Description: This class extends the stl's string class.
//
//********************************************************************
// $Id: ossimRpfImageDisplayParameterSubheader.cpp 9963 2006-11-28 21:11:01Z gpotts $
#include <ossim/support_data/ossimRpfImageDisplayParameterSubheader.h>

#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimErrorCodes.h>

ostream& operator<<(ostream& out,
                    const ossimRpfImageDisplayParameterSubheader& data)
{
   data.print(out);

   return out;
}

ossimRpfImageDisplayParameterSubheader::ossimRpfImageDisplayParameterSubheader()
{
   clearFields();
}

ossimErrorCode ossimRpfImageDisplayParameterSubheader::parseStream(istream& in,
                                                         ossimByteOrder byteOrder)
{
   clearFields();
   if(in)
   {
      theStartOffset = in.tellg();
      in.read((char*)&theNumberOfImageRows, 4);
      in.read((char*)&theNumberOfImageCodesPerRow, 4);
      in.read((char*)&theImageCodeBitLength, 1);
      theEndOffset = in.tellg();

      ossimEndian anEndian;
      if(anEndian.getSystemEndianType() != byteOrder)
      {
         anEndian.swap(theNumberOfImageRows);
         anEndian.swap(theNumberOfImageCodesPerRow);
      }
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }
   
   return ossimErrorCodes::OSSIM_OK;
}
   
void ossimRpfImageDisplayParameterSubheader::print(ostream& out)const
{
   out << "theNumberOfImageRows:          " << theNumberOfImageRows << endl
       << "theNumberOfImageCodesPerRow:   " << theNumberOfImageCodesPerRow << endl
       << "theImageCodeBitLength:         " << (unsigned long)theImageCodeBitLength;
}

void ossimRpfImageDisplayParameterSubheader::clearFields()
{
   theNumberOfImageRows        = 0;
   theNumberOfImageCodesPerRow = 0;
   theImageCodeBitLength       = 0;
}
