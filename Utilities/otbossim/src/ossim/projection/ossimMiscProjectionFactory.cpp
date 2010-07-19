//*****************************************************************************
// FILE: ossimMiscProjectionFactory.cc
//
// License:  See top level LICENSE.txt file.
//
// DESCRIPTION:
//   Contains implementation of class ossimMiscProjectionFactory
//
// SOFTWARE HISTORY:
//>
//   15Jun2001  Garrett Potts
//              Initial coding.
//<
//*****************************************************************************
//  $Id: ossimMiscProjectionFactory.cpp 17623 2010-06-25 15:03:42Z gpotts $

#include <ossim/projection/ossimMiscProjectionFactory.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordNames.h>

//***
// Note to programmer: To add a new projection, search this file for
// "ADD_PROJECTION" to locate places requiring editing. Functional example
// below...
//
// ADD_PROJECTION: Include all map projection headers here:
//***

#include <ossim/projection/ossimSonomaSensor.h>
#include <ossim/projection/ossimAffineProjection.h>
#include <ossim/projection/ossimBilinearProjection.h>
#include <ossim/projection/ossimQuadProjection.h>
#include <ossim/projection/ossimWarpProjection.h>
#include <ossim/projection/ossimRpcProjection.h>
#include <ossim/projection/ossimPolynomProjection.h>

//***
// ADD_PROJECTION: List names of all map projections produced by this factory:
//***

ossimMiscProjectionFactory* ossimMiscProjectionFactory::theInstance = 0;

//*****************************************************************************
//  STATIC METHOD: ossimMiscProjectionFactory::instance()
//  
//*****************************************************************************
ossimMiscProjectionFactory* ossimMiscProjectionFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimMiscProjectionFactory;
   }

   return (ossimMiscProjectionFactory*) theInstance;
}

ossimProjection*
ossimMiscProjectionFactory::createProjection(const ossimFilename& filename,
                                             ossim_uint32 entryIdx) const
{
   // Check for external geometry file.
   return createProjectionFromGeometryFile(filename, entryIdx);
}

//*****************************************************************************
//  METHOD: ossimMiscProjectionFactory::createProjection(kwl, prefix)
//  
//*****************************************************************************
ossimProjection*
ossimMiscProjectionFactory::createProjection(const ossimKeywordlist &kwl,
                                             const char *prefix) const
{
   ossimProjection *result=NULL;
   
   const char *lookup = kwl.find(prefix, ossimKeywordNames::TYPE_KW);

   if(lookup)
   {
      result = createProjection(ossimString(lookup).trim());
      if(result)
      {
         result->loadState(kwl, prefix);
      }
   }
   else
   {
      // look for the geom_file keyword.
      //
      lookup = kwl.find(prefix, "geom_file");
      if(lookup)
      {
         ossimKeywordlist kwl2;

         kwl2.addFile(lookup);

         result = createProjection(kwl2, "projection.");
         // maybe they did not prepend projection. in the pro
         // file.
         if(!result)
         {
            result = createProjection(kwl2);
         }

      }      
   }
   return result;
}

//*****************************************************************************
//  METHOD: ossimMiscProjectionFactory::createProjection(projection_name)
//  
//*****************************************************************************
ossimProjection*
ossimMiscProjectionFactory::createProjection(const ossimString &name) const
{
   //***
   // ADD_PROJECTION: Test for each map projection here:
   //***
   if(name == STATIC_TYPE_NAME(ossimSonomaSensor))
   {
      return new ossimSonomaSensor;
   }
   if(name == STATIC_TYPE_NAME(ossimAffineProjection))
   {
      return new ossimAffineProjection;
   }
   if(name == STATIC_TYPE_NAME(ossimBilinearProjection))
   {
      return new ossimBilinearProjection;
   }
   if(name == STATIC_TYPE_NAME(ossimQuadProjection))
   {
      return new ossimQuadProjection;
   }
   if(name == STATIC_TYPE_NAME(ossimRpcProjection))
   {
      return new ossimRpcProjection;
   }
   if(name == STATIC_TYPE_NAME(ossimWarpProjection))
   {
      return new ossimWarpProjection;
   }
   if(name == STATIC_TYPE_NAME(ossimPolynomProjection))
   {
      return new ossimPolynomProjection;
   }

   return NULL;
}

//*****************************************************************************
//  METHOD 
//*****************************************************************************
ossimObject*
ossimMiscProjectionFactory::createObject(const ossimString& typeName)const
{
   return createProjection(typeName);
}

//*****************************************************************************
//  METHOD 
//*****************************************************************************
ossimObject*
ossimMiscProjectionFactory::createObject(const ossimKeywordlist& kwl,
                                                     const char* prefix)const
{
   return createProjection(kwl, prefix);
}

//*****************************************************************************
//  METHOD 
//*****************************************************************************
void ossimMiscProjectionFactory::getTypeNameList(std::vector<ossimString>& typeList)const
{
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimSonomaSensor)));
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimAffineProjection)));
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimBilinearProjection)));
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimRpcProjection)));
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimQuadProjection)));   
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimWarpProjection)));
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimPolynomProjection)));
}
