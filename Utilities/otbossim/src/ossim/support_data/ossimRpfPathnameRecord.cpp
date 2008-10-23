//*******************************************************************
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
// Description: This class extends the stl's string class.
//
//********************************************************************
// $Id: ossimRpfPathnameRecord.cpp 9963 2006-11-28 21:11:01Z gpotts $
#include <ossim/support_data/ossimRpfPathnameRecord.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimErrorCodes.h>

ostream& operator <<(ostream& out,
                     const ossimRpfPathnameRecord& data)
{
   data.print(out);
   
   return out;
}


ossimRpfPathnameRecord::ossimRpfPathnameRecord()
{   
   clearFields();
}

void ossimRpfPathnameRecord::clearFields()
{
   theLength   = 0;
   thePathname = "";
}

ossimErrorCode ossimRpfPathnameRecord::parseStream(istream& in, ossimByteOrder byteOrder)
{
   if(in)
   {
      ossimEndian anEndian;
      
      clearFields();
      in.read((char*)&theLength, 2);

      if(anEndian.getSystemEndianType() != byteOrder)
      {
         anEndian.swap(theLength);
      }

      char *temp = new char[theLength+1];
      in.read((char*)temp, theLength);
      temp[theLength] = '\0';
      thePathname = temp;
      
      delete [] temp;
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }

   return ossimErrorCodes::OSSIM_OK;
}

void ossimRpfPathnameRecord::print(ostream& out)const
{
   out << "theLength:        " << theLength   << endl
       << "thePathname:      " << thePathname;
}
