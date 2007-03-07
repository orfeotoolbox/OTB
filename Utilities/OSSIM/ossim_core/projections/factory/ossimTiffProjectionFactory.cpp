//*****************************************************************************
// FILE: ossimTiffProjectionFactory.cc
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// DESCRIPTION:
//   Contains implementation of class ossimTiffProjectionFactory
//
//*****************************************************************************
//  $Id: ossimTiffProjectionFactory.cpp,v 1.3 2004/09/24 17:19:40 dburken Exp $

#include "ossimTiffProjectionFactory.h"
#include "support_data/tiff/ossimGeoTiff.h"
#include "ossimProjectionFactoryRegistry.h"
#include "base/data_types/ossimKeywordlist.h"
#include "base/common/ossimKeywordNames.h"
#include "base/data_types/ossimFilename.h"
#include "projections/ossimProjection.h"
#include <fstream>

ossimTiffProjectionFactory* ossimTiffProjectionFactory::theInstance = 0;

//*****************************************************************************
//  STATIC METHOD: ossimTiffProjectionFactory::instance()
//  
//*****************************************************************************
ossimTiffProjectionFactory* ossimTiffProjectionFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimTiffProjectionFactory;
   }

   return (ossimTiffProjectionFactory*) theInstance;
}

ossimProjection*
ossimTiffProjectionFactory::createProjection(const ossimFilename& filename,
                                              ossim_uint32 entryIdx)const
{
   if(!filename.exists())
   {
      return NULL;
   }

   // See if there is an external geomtry.
   ossimProjection* result = createProjectionFromGeometryFile(filename,
                                                              entryIdx);
   if (result)
   {
      return result;
   }

   if(isTiff(filename))
   {
      ossimGeoTiff geotiff(filename);
      ossimKeywordlist kwl;
      
      if(geotiff.addImageGeometry(kwl))
      {
         return ossimProjectionFactoryRegistry::instance()->
            createProjection(kwl);
      }
   }
   
   return NULL;
}

//*****************************************************************************
//  METHOD: ossimSensorModelFactory::create(kwl, prefix)
//  
//*****************************************************************************
ossimProjection*
ossimTiffProjectionFactory::createProjection(const ossimKeywordlist &keywordList,
                                            const char *prefix) const
{
   return NULL;
}

//*****************************************************************************
//  METHOD: ossimSensorModelFactory::create(projection_name)
//  
//*****************************************************************************
ossimProjection* ossimTiffProjectionFactory::createProjection(const ossimString &name) const
{
   return NULL;
}

ossimObject* ossimTiffProjectionFactory::createObject(const ossimString& typeName)const
{
   return (ossimObject*)createProjection(typeName);
}

ossimObject* ossimTiffProjectionFactory::createObject(const ossimKeywordlist& kwl,
                                                     const char* prefix)const
{
   return createProjection(kwl, prefix);
}

void ossimTiffProjectionFactory::getTypeNameList(std::vector<ossimString>& typeList)const
{
   
}

list<ossimString> ossimTiffProjectionFactory::getList()const
{
   list<ossimString> result;

   
   return result;
}

bool ossimTiffProjectionFactory::isTiff(const ossimFilename& filename)const
{
   std::ifstream in;
   unsigned char header[2];

   in.open( filename.c_str(), ios::in|ios::binary);
   if( !in)
       return false;

   in.read( (char*)header, 2);

   if( (header[0] != 'M' || header[1] != 'M')
       && (header[0] != 'I' || header[1] != 'I') )
   {
      return false;
   }

   return true;
}
