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
// $Id: ossimRpfPathnameRecord.h,v 1.1 2003/04/02 20:45:00 gpotts Exp $
#ifndef ossimRpfPathnameRecord_HEADER
#define ossimRpfPathnameRecord_HEADER
#include <iostream>
using namespace std;

#include "base/common/ossimConstants.h" // ossiByteOrder and RPF constants
#include "base/context/ossimErrorContext.h" // for ossimErrorCode
#include "base/data_types/ossimString.h"

class ossimRpfPathnameRecord
{
public:
   friend ostream& operator <<(ostream& out,
                               const ossimRpfPathnameRecord& data);
   ossimRpfPathnameRecord();
   void clearFields();
   ossimErrorCode parseStream(istream& in, ossimByteOrder byteOrder);
   void print(ostream& out)const;
   ossimString getPathname()const{return thePathname;}
   
private:   
   unsigned short theLength;
   ossimString thePathname;
};

#endif
