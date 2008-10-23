//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfBoundaryRectTable.h 9967 2006-11-29 02:01:23Z gpotts $
#ifndef ossimRpfBoundaryRectTable_HEADER
#define ossimRpfBoundaryRectTable_HEADER
#include <iostream>
#include <vector>
#include <iterator>
using namespace std;

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorContext.h>
#include <ossim/support_data/ossimRpfBoundaryRectRecord.h>

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
