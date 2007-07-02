//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: This class give the capability to access tiles from an
//              vpf file.
//
//********************************************************************
// $Id: ossimVpfCoverage.h 9967 2006-11-29 02:01:23Z gpotts $
#ifndef ossimVpfCoverage_HEADER
#define ossimVpfCoverage_HEADER
#include <map>
#include <vector>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimErrorContext.h>
#include <ossim/vec/ossimVpfTable.h>
#include <ossim/vec/ossimVpfFeatureClass.h>
#include <ossim/vec/ossimVpfExtent.h>

class ossimVpfLibrary;
class ossimVpfFeatureClassSchema;
class OSSIMDLLEXPORT ossimVpfCoverage
{
public:
   ossimVpfCoverage ();
   virtual ~ossimVpfCoverage();
   
   bool openCoverage(ossimVpfLibrary* library,
                     const ossimString& name,
                     const ossimFilename& fileName);
   const ossimFilename& getPath()const;
   const ossimString& getName()const;
   
   ossim_uint32 getNumberOfFeatureClasses()const;
   void print(ostream& out)const;

   bool getExtent(ossimVpfExtent& extent)const;
   ossimVpfLibrary* getLibrary();
   bool openFeatureClassSchema(ossimVpfFeatureClassSchema& schema);
   
private:
   ossimVpfLibrary* theLibrary;
   ossimString      theCoverageName;

   /*!
    * will hold the full path to this coverage.
    */
   ossimFilename theCoverageNameFullPath;
   
};

#endif
