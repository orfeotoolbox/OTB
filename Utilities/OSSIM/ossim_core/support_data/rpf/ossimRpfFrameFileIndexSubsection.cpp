//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfFrameFileIndexSubsection.cpp,v 1.1 2003/04/02 20:45:00 gpotts Exp $
#include "ossimRpfFrameFileIndexSubsection.h"
#include <string.h> // for memset
#include "base/misc/ossimEndian.h"
#include "base/common/ossimErrorCodes.h"

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
