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
// $Id: ossimRpfBoundaryRectTable.h,v 1.1 2003/04/02 20:45:00 gpotts Exp $
#ifndef ossimRpfBoundaryRectTable_HEADER
#define ossimRpfBoundaryRectTable_HEADER
#include <iostream>
#include <vector>
#include <iterator>
using namespace std;

#include "base/common/ossimConstants.h" // ossiByteOrder and RPF constants
#include "base/context/ossimErrorContext.h" // for ossimErrorCode
#include "ossimRpfBoundaryRectRecord.h"

class ossimRpfBoundaryRectTable
{
public:
   friend ostream& operator <<(ostream& out,
                               const ossimRpfBoundaryRectTable& data);
   
   ossimRpfBoundaryRectTable(unsigned long numberOfEntries=0);
   virtual ~ossimRpfBoundaryRectTable(){}
   ossimErrorCode parseStream(istream& in, ossimByteOrder byteOrder);
   void setNumberOfEntries(unsigned long numberOfEntries);
   virtual void print(ostream& out)const;
   
private:
   vector<ossimRpfBoundaryRectRecord> theTable;
};
#endif
