//*******************************************************************
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimProjectionFactoryRegistry.cpp 16308 2010-01-09 02:45:54Z eshirschorn $
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimProjectionFactoryBase.h>
#include <ossim/projection/ossimSrsProjectionFactory.h>
#include <ossim/projection/ossimMapProjectionFactory.h>
#include <ossim/projection/ossimTiffProjectionFactory.h>
#include <ossim/projection/ossimNitfProjectionFactory.h>
#include <ossim/projection/ossimPcsCodeProjectionFactory.h>
#include <ossim/projection/ossimGcsCodeProjectionFactory.h>
#include <ossim/projection/ossimStatePlaneProjectionFactory.h>
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
   ossimProjection* result = createNativeObjectFromRegistry(kwl, prefix);

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
                  ossim_uint32 idx = 0;
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
   registerFactory(ossimNitfProjectionFactory::instance());
   registerFactory(ossimSensorModelFactory::instance());//OTB: switch with Tiff to work on QB/Ikonos/SPOT5
   registerFactory(ossimTiffProjectionFactory::instance());
   registerFactory(ossimMapProjectionFactory::instance());
   registerFactory(ossimSrsProjectionFactory::instance());
   registerFactory(ossimPcsCodeProjectionFactory::instance());   
   registerFactory(ossimGcsCodeProjectionFactory::instance());   
   registerFactory(ossimStatePlaneProjectionFactory::instance());
   registerFactory(ossimMiscProjectionFactory::instance());
}

extern "C"
{
  void* ossimProjectionFactoryRegistryGetInstance()
  {
    return ossimProjectionFactoryRegistry::instance();
  }

}
