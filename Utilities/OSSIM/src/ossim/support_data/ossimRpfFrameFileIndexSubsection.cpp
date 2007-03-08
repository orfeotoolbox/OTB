//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfFrameFileIndexSubsection.cpp 9963 2006-11-28 21:11:01Z gpotts $
#include <ossim/support_data/ossimRpfFrameFileIndexSubsection.h>
#include <string.h> // for memset
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimErrorCodes.h>

ostream& operator <<(ostream& out,
                     const ossimRpfFrameFileIndexSubsection& data)
{
   data.print(out);
   
   return out;
}

ossimRpfFrameFileIndexSubsection::ossimRpfFrameFileIndexSubsection()
{
   clearFields();
}

ossimErrorCode ossimRpfFrameFileIndexSubsection::parseStream(istream &in,
                                                             ossimByteOrder byteOrder)
{
   ossimErrorCode result = ossimErrorCodes::OSSIM_OK;
   
   if(in && (theIndexTable.size()>0))
   {
      unsigned long index;
      for(index = 0;
          (index < theIndexTable.size())&&(result ==
                                           ossimErrorCodes::OSSIM_OK);
          index++)
      {
         result = theIndexTable[index].parseStream(in, byteOrder);
      }
      for(index = 0;
          (index < thePathnameTable.size())&&(result ==
                                              ossimErrorCodes::OSSIM_OK);
          index++)
      {
         result = thePathnameTable[index].parseStream(in, byteOrder);
      }
   }
   else
   {
      result = ossimErrorCodes::OSSIM_ERROR;
   }

   return result;
}

void ossimRpfFrameFileIndexSubsection::clearFields()
{
   theIndexTable.clear();
   thePathnameTable.clear();
}

void ossimRpfFrameFileIndexSubsection::print(ostream& out)const
{
   copy(theIndexTable.begin(),
        theIndexTable.end(),
        ostream_iterator<ossimRpfFrameFileIndexRecord>(out, "\n"));
   copy(thePathnameTable.begin(),
        thePathnameTable.end(),
        ostream_iterator<ossimRpfPathnameRecord>(out, "\n"));
}


void ossimRpfFrameFileIndexSubsection::setNumberOfFileIndexRecords(unsigned long numberOfIndexRecords)
{
   theIndexTable.resize(numberOfIndexRecords);
}

void ossimRpfFrameFileIndexSubsection::setNumberOfPathnames(unsigned long numberOfPathnames)
{
   thePathnameTable.resize(numberOfPathnames);
}
