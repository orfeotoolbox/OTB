//*****************************************************************************
// FILE: ossimTiffProjectionFactory.cc
//
// License:  See top level LICENSE.txt file.
//
// DESCRIPTION:
//   Contains implementation of class ossimTiffProjectionFactory
//
//*****************************************************************************
//  $Id: ossimTiffProjectionFactory.cpp 12082 2007-11-26 21:46:44Z dburken $

#include <ossim/projection/ossimTiffProjectionFactory.h>
#include <ossim/support_data/ossimGeoTiff.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/projection/ossimProjection.h>
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
      ossimGeoTiff geotiff(filename, entryIdx);
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
