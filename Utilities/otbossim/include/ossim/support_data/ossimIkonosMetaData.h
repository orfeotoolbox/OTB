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

#include <vector>
#include <iostream>

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimErrorStatusInterface.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimDpt3d.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimEcefPoint.h>

class ossimKeywordlist;

class OSSIMDLLEXPORT ossimIkonosMetaData : public ossimObject,
                                           public ossimErrorStatusInterface
{
public:
   ossimIkonosMetaData();
   ossimIkonosMetaData(const ossimFilename& imageFile);
   ossimIkonosMetaData(const ossimFilename& metadataFile, const ossimFilename& hdrFile, const ossimFilename& rpcFile);
   virtual ~ossimIkonosMetaData();

   void clearFields();

   //---
   // Convenient method to print important image info:
   //---
   void  printInfo (ostream& os) const;

   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix = 0) const;
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix = 0);


 private:
   bool parseMetaData(const ossimFilename& metadata);
   bool parseHdrData(const ossimFilename& data_file);
   bool parseRpcData(const ossimFilename& data_file);

   ossim_float64               theSunAzimuth;
   ossim_float64               theSunElevation;

   ossimString                 theBandName;

TYPE_DATA
};

#endif /* #ifndef ossimIkonosMetaData_HEADER */
