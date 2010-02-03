//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfBoundaryRectTable.cpp 9963 2006-11-28 21:11:01Z gpotts $
#include <ossim/support_data/ossimRpfBoundaryRectTable.h>
#include <iomanip>
using namespace std;

#include <string.h> // for memset
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimErrorCodes.h>

ostream& operator <<(ostream& out,
                     const ossimRpfBoundaryRectTable& data)
{
   data.print(out);

   return out;
}

ossimRpfBoundaryRectTable::ossimRpfBoundaryRectTable(unsigned long numberOfEntries)
{
   theTable.resize(numberOfEntries);  
}

ossimErrorCode ossimRpfBoundaryRectTable::parseStream(istream& in,
                                                      ossimByteOrder byteOrder)
{
   if(in)
   {
      vector<ossimRpfBoundaryRectRecord>::iterator entry = theTable.begin();
      
      while(entry != theTable.end())
      {
         (*entry).parseStream(in, byteOrder);
         ++entry;
      }
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }

   return ossimErrorCodes::OSSIM_OK;
}

void ossimRpfBoundaryRectTable::setNumberOfEntries(unsigned long numberOfEntries)
{
   theTable.resize(numberOfEntries);
}

void ossimRpfBoundaryRectTable::print(ostream& out)const
{
   copy(theTable.begin(),
        theTable.end(),
        ostream_iterator<ossimRpfBoundaryRectRecord>(out, "\n"));
}
