//*******************************************************************
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimProjectionFactoryRegistry.cpp 20413 2012-01-03 19:03:59Z dburken $
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimProjectionFactoryBase.h>
#include <ossim/projection/ossimEpsgProjectionFactory.h>
#include <ossim/projection/ossimWktProjectionFactory.h>
#include <ossim/projection/ossimMapProjectionFactory.h>
#include <ossim/projection/ossimTiffProjectionFactory.h>
#include <ossim/projection/ossimNitfProjectionFactory.h>
#include <ossim/projection/ossimSensorModelFactory.h>
#include <ossim/projection/ossimMiscProjectionFactory.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/base/ossimObjectFactoryRegistry.h>
#include <algorithm>
#include <vector>

ossimProjectionFactoryRegistry* ossimProjectionFactoryRegistry::m_instance = 0;


ossimProjectionFactoryRegistry::ossimProjectionFactoryRegistry()
{
   initializeDefaults();
   ossimObjectFactoryRegistry::instance()->registerFactory(this);
}

ossimProjectionFactoryRegistry::ossimProjectionFactoryRegistry(const ossimProjectionFactoryRegistry& rhs)
:
ossimObjectFactory(rhs)
{}

void ossimProjectionFactoryRegistry::operator=(const ossimProjectionFactoryRegistry&)
{}

ossimProjectionFactoryRegistry::~ossimProjectionFactoryRegistry()
{
}

ossimProjectionFactoryRegistry* ossimProjectionFactoryRegistry::instance()
{
   if(!m_instance)
   {
      m_instance = new ossimProjectionFactoryRegistry();
   }
   
   return m_instance;
}

ossimProjection*
ossimProjectionFactoryRegistry::createProjection(const ossimFilename& name,
                                                 ossim_uint32 entryIdx)const
{
   ossimProjection* result = 0;
   ossim_uint32 idx = 0;
   for(idx = 0; ((idx < m_factoryList.size())&&(!result)); ++idx)
   {
      result = m_factoryList[idx]->createProjection(name, entryIdx);
   }
   
   return result;
}

ossimProjection* ossimProjectionFactoryRegistry::createProjection(ossimImageHandler* handler)const
{
   ossimProjection* result = 0;
   ossim_uint32 idx = 0;
   for(idx = 0; ((idx < m_factoryList.size())&&(!result)); ++idx)
   {
      result = m_factoryList[idx]->createProjection(handler);
   }
   
   return result;
}

ossimProjection* ossimProjectionFactoryRegistry::createProjection(
   const ossimKeywordlist& kwl, const char* prefix)const
{
   ossimProjection* result = 0;//createNativeObjectFromRegistry(kwl, prefix); 
   ossim_uint32 idx = 0; 
   for(idx = 0; ((idx < m_factoryList.size())&&!result);++idx) 
   { 
      result = m_factoryList[idx]->createProjection(kwl, prefix); 
   } 
   
   if ( (result == 0) && (prefix == 0) )
   {
      //---
      // Check for an embedded prefix like "image0."
      //
      // This is a workaround for people who use the "image_info" application
      // to create a geometry file.
      //
      // If the image has an external geometry file and it has a prefix like
      // "image0." and someone does:
      //
      // theInputConnection->getImageGeometry(kwl);
      // ossimRefPtr<ossimProjection> proj =
      // ossimProjectionFactoryRegistry::instance()->createProjection(kwl);
      //
      // It will fail because the factory doesn't know it has a prefix.
      //
      // ESH 01/2009: I've changed the following so that not just the first
      // line is checked for "image" and ".".  If the image_info .geom file
      // was created with the -m (metadata) option, the first lines of the
      // file will not have the expected structure.
      //---
      bool bFoundImageLine = false;
      ossimKeywordlist::KeywordMap::const_iterator i = kwl.getMap().begin();
      while ( (i != kwl.getMap().end()) && (bFoundImageLine == false) )
      {
         ossimString s1 = (*i).first;
         if ( s1.size() )
         {
            std::vector<ossimString> v;
            s1.split(v, ossimString("."));
            if ( v.size() )
            {
               if ( v[0].contains("image") )
               {
                  bFoundImageLine = true;
                  ossimString s2 = v[0];
                  s2 += ".";
                  idx = 0;
                  for(;((idx < m_factoryList.size())&&!result); ++idx)
                  {
                     result =  m_factoryList[idx]->createProjection(kwl, s2.c_str());
                  }
               }
            }
         }

         // Go to the next line of the .geom file
         ++i;
      }
   }

   return result;
}

void ossimProjectionFactoryRegistry::initializeDefaults()
{
   registerFactory(ossimSensorModelFactory::instance());
   registerFactory(ossimNitfProjectionFactory::instance());
   registerFactory(ossimTiffProjectionFactory::instance());
   registerFactory(ossimWktProjectionFactory::instance());
   registerFactory(ossimMapProjectionFactory::instance());
   registerFactory(ossimMiscProjectionFactory::instance());

   // KEEP THIS LAST PLEASE!
   // This factory constructs map projections from EPSG codes. An infinite loop will occur if this
   // is placed before the explicit (non-coded) factories, since this factory will invoke the above
   // factories via this registry after populating a KWL which includes a PCS code. If this factory
   // sees that request before the others, it will be caught in a loop.
   registerFactory(ossimEpsgProjectionFactory::instance()); 
}

extern "C"
{
  void* ossimProjectionFactoryRegistryGetInstance()
  {
    return ossimProjectionFactoryRegistry::instance();
  }

}
