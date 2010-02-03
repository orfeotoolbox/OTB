//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfPathnameRecord.h 9967 2006-11-29 02:01:23Z gpotts $
#ifndef ossimRpfPathnameRecord_HEADER
#define ossimRpfPathnameRecord_HEADER
#include <iostream>
using namespace std;

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorContext.h>
#include <ossim/base/ossimString.h>

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
