//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: This class give the capability to access tiles from an
//              vpf file.
//
//********************************************************************
// $Id: ossimVpfLibrary.h 9967 2006-11-29 02:01:23Z gpotts $
#ifndef ossimVpfLibrary_HEADER
#define ossimVpfLibrary_HEADER
#include <vector>
#include <map>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimErrorContext.h>

class ossimVpfCoverage;
class ossimVpfDatabase;
class ossimVpfExtent;

class OSSIMDLLEXPORT ossimVpfLibrary
{
public:
   ossimVpfLibrary();
   virtual ~ossimVpfLibrary();
   bool openLibrary(ossimVpfDatabase* database,
                    const ossimString& name,
                    const ossimFilename& libraryPath);
   void              setDatabase(ossimVpfDatabase* database);
   ossimVpfDatabase* getDatabase()const;
   bool getCoverage(long coverageNumber, ossimVpfCoverage& coverage);
   bool getCoverage(const ossimString& name, ossimVpfCoverage& coverage);

   bool                getExtent(ossimVpfExtent& result)const;
   bool                hasGazette()const;

   ossimFilename       getPath()const;
   ossimString         getName()const;
   bool getCoverageNames(std::vector<ossimString>& coverageNames)const;
   int getNumberOfCoverages()const;

   bool hasCoverage(const ossimString& coverageName)const;
   ossimString getTileName(ossim_int32 id)const;
   void getTileNames(std::vector<ossimString>& tileNames)const;

private:
   ossimVpfDatabase*        theDatabase;
   ossimString              theLibraryName;
   ossimFilename            theLibraryNameFullPath;
   ossim_uint32             theNumberOfCoverages;
   std::vector<ossimString> theCoverageNames;
   mutable std::map<ossim_int32, ossimString> theTileNameMap;
   void setCoverageNames();
   void setTileNames()const;
};

#endif
