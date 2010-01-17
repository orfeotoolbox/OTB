//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfFrameFileIndexSubsection.h 9967 2006-11-29 02:01:23Z gpotts $
#ifndef ossimRpfFrameFileIndexSubsection_HEADER
#define ossimRpfFrameFileIndexSubsection_HEADER
#include <iostream>
#include <vector>
#include <iterator>
using namespace std;
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/support_data/ossimRpfFrameFileIndexRecord.h>
#include <ossim/support_data/ossimRpfPathnameRecord.h>

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
