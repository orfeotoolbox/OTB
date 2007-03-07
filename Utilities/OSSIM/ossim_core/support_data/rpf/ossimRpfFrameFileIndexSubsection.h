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
// $Id: ossimRpfFrameFileIndexSubsection.h,v 1.1 2003/04/02 20:45:00 gpotts Exp $
#ifndef ossimRpfFrameFileIndexSubsection_HEADER
#define ossimRpfFrameFileIndexSubsection_HEADER
#include <iostream>
#include <vector>
#include <iterator>
using namespace std;
#include "base/data_types/ossimString.h"
#include "base/common/ossimConstants.h"
#include "ossimRpfFrameFileIndexRecord.h"
#include "ossimRpfPathnameRecord.h"

class ossimRpfFrameFileIndexSubsection
{
public:
   friend ostream& operator <<(ostream& out,
                               const ossimRpfFrameFileIndexSubsection& data);
   ossimRpfFrameFileIndexSubsection();
   ossimErrorCode parseStream(istream &in, ossimByteOrder byteOrder);
   void clearFields();
   void print(ostream& out)const;
   void setNumberOfFileIndexRecords(unsigned long numberOfIndexRecords);
   void setNumberOfPathnames(unsigned long numberOfPathnames);
   const vector<ossimRpfFrameFileIndexRecord>& getIndexTable()const{return theIndexTable;}
   
private:
   vector<ossimRpfFrameFileIndexRecord> theIndexTable;

   vector<ossimRpfPathnameRecord> thePathnameTable;
};

#endif
