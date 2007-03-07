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
// $Id: ossimRpfTocEntry.h,v 1.1 2003/04/02 20:45:00 gpotts Exp $
#ifndef ossimRpfTocEntry_HEADER
#define ossimRpfTocEntry_HEADER
#include <vector>
#include <iostream>
#include <iterator>
using namespace std;

#include "ossimRpfBoundaryRectRecord.h"
#include "base/common/ossimConstants.h" // ossiByteOrder and RPF constants
#include "base/context/ossimErrorContext.h" // for ossimErrorCode
#include "ossimRpfFrameEntry.h"

class ossimRpfTocEntry
{
public:
   friend ostream& operator <<(ostream& out,
                               const ossimRpfTocEntry& data);
   ossimRpfTocEntry();
   ossimErrorCode parseStream(istream &in, ossimByteOrder byteOrder);
   void print(ostream& out)const;
   void setEntry(const ossimRpfFrameEntry& entry,
                 long row,
                 long col);

   bool getEntry(long row,
                 long col,
                 ossimRpfFrameEntry& result)const;

   ossimString getProductType()const{return theBoundaryInformation.getProductType();}
   /*!
    * returns how many subimges or frames exist in the horizontal
    * direction.
    */
   unsigned long getNumberOfFramesHorizontal()const;

   /*!
    * returns how many subimges or frames exist in the vertical
    * direction.
    */
   unsigned long getNumberOfFramesVertical()const;

   
   const ossimRpfBoundaryRectRecord& getBoundaryInformation()const
      {
         return theBoundaryInformation;
      }
   /*!
    * If there is an entry and all the files don't exist we will return
    * true.
    */
   bool isEmpty()const;
   
private:
   void allocateFrameEntryArray();
   
   ossimRpfBoundaryRectRecord           theBoundaryInformation;
   vector< vector<ossimRpfFrameEntry> > theFrameEntryArray;
};

#endif
