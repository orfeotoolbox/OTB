//*****************************************************************************
// FILE: ossimMiscProjectionFactory.cc
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
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
//  $Id: ossimMiscProjectionFactory.cpp,v 1.8 2004/10/04 18:57:11 gpotts Exp $

#include <projections/factory/ossimMiscProjectionFactory.h>
#include <base/data_types/ossimKeywordlist.h>
#include <base/data_types/ossimFilename.h>
#include <base/common/ossimKeywordNames.h>

//***
// Note to programmer: To add a new projection, search this file for
// "ADD_PROJECTION" to locate places requiring editing. Functional example
// below...
//
// ADD_PROJECTION: Include all map projection headers here:
//***

#include <projections/ossimAffineProjection.h>
#include <projections/ossimBilinearProjection.h>
#include <projections/ossimQuadProjection.h>
#include <projections/ossimWarpProjection.h>
#include <projections/ossimRpcProjection.h>

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
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimAffineProjection)));
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimBilinearProjection)));
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimRpcProjection)));
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimQuadProjection)));   
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimWarpProjection)));
}

//*****************************************************************************
//  METHOD 
//*****************************************************************************
list<ossimString> ossimMiscProjectionFactory::getList()const
{
   list<ossimString> result;

   result.push_back(ossimString(STATIC_TYPE_NAME(ossimAffineProjection)));
   result.push_back(ossimString(STATIC_TYPE_NAME(ossimBilinearProjection)));
   result.push_back(ossimString(STATIC_TYPE_NAME(ossimRpcProjection)));
   result.push_back(ossimString(STATIC_TYPE_NAME(ossimQuadProjection)));   
   result.push_back(ossimString(STATIC_TYPE_NAME(ossimWarpProjection)));

   return result;
}
