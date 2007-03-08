//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Class declaration for ossimIkonosMetaData.
// 
// This class parses a Space Imaging Ikonos meta data file.
//
//********************************************************************
// $Id: ossimIkonosMetaData.h 9967 2006-11-29 02:01:23Z gpotts $

#ifndef ossimIkonosMetaData_HEADER
#define ossimIkonosMetaData_HEADER

#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimErrorStatusInterface.h>

class OSSIMDLLEXPORT ossimIkonosMetaData : public ossimObject,
                                           public ossimErrorStatusInterface
{
public:
   ossimIkonosMetaData();
   ossimIkonosMetaData(const char* metaDataFile);
   virtual ~ossimIkonosMetaData();

 private:
   
TYPE_DATA   
};

#endif /* #ifndef ossimIkonosMetaData_HEADER */
