//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimSharedPluginRegistry.cpp 9194 2006-06-26 17:32:31Z gpotts $
#include <algorithm>
#include <ossim/plugin/ossimSharedPluginRegistry.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/plugin/ossimSharedObjectBridge.h>

ossimSharedPluginRegistry* ossimSharedPluginRegistry::theInstance = NULL;
//ossimPluginBridgeStructure ossimSharedPluginRegistry::thePluginBridgeFactoryPointers;

static ossimTrace traceDebug("ossimSharedPluginRegistry:debug");

ossimSharedPluginRegistry::~ossimSharedPluginRegistry()
{
   theLibraryList.clear();

   theInstance = NULL;
}

ossimSharedPluginRegistry* ossimSharedPluginRegistry::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimSharedPluginRegistry;
   }

   return theInstance;
}

bool ossimSharedPluginRegistry::registerPlugin(const ossimFilename& filename)
{
   bool result = false;
   if(!getPlugin(filename))
   {
      ossimPluginLibrary *lib =new ossimPluginLibrary;
      if(lib->load(filename))
      {
         if(lib->getSymbol("ossimSharedLibraryInitialize"))
         {
            lib->initialize();
            theLibraryList.push_back(lib);
            result = true;
         }
         else
         {
            if (traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG)
                  << "ossimSharedPluginRegistry DEBUG:"
                  << "ossimSharedLibraryInitialize symbol not found\n"
                  << std::endl;
            }
            
         }
      }
      else
      {
         delete lib;
         lib = 0;
      }
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN) << "WARNING: Plugin with the name " << filename << std::endl
                                         << "Already registered with OSSIM" << std::endl;
   }
   
   return result;
}

bool ossimSharedPluginRegistry::unregisterPlugin(int idx)
{
   ossimPluginLibrary* lib = getPlugin(idx);
   bool result = false;
   if(lib)
   {
      std::vector<ossimRefPtr<ossimPluginLibrary> >::iterator iter = std::find(theLibraryList.begin(),
                                                                   theLibraryList.end(),
                                                                   lib);

      if(iter != theLibraryList.end())
      {
         (*iter)->finalize();
         theLibraryList.erase(iter);
         result  = true;
      }
   }
   
   return result;
}

const ossimPluginLibrary* ossimSharedPluginRegistry::getPlugin(const ossimFilename& filename)const
{
   ossim_uint32 idx = 0;

   for(idx = 0; idx < theLibraryList.size();++idx)
   {
      if(filename == theLibraryList[idx]->getName())
      {
         return theLibraryList[idx].get();
      }
   }
   
   return 0;
}

ossimPluginLibrary* ossimSharedPluginRegistry::getPlugin(const ossimFilename& filename)
{
   ossim_uint32 idx = 0;

   for(idx = 0; idx < theLibraryList.size();++idx)
   {
      if(filename == theLibraryList[idx]->getName())
      {
         return theLibraryList[idx].get();
      }
   }
   
   return 0;
}

ossim_uint32 ossimSharedPluginRegistry::getIndex(const ossimPluginLibrary* lib)const
{
   ossim_uint32 idx = 0;

   for(idx = 0; idx < theLibraryList.size(); ++idx)
   {
      if(theLibraryList[idx] == lib)
      {
         return idx;
      }
   }

   return idx;
}

ossimPluginLibrary* ossimSharedPluginRegistry::getPlugin(ossim_uint32 idx)
{
   ossimPluginLibrary* result = 0;
   
   if((idx>=0)&&(idx < theLibraryList.size()))
   {
      result = theLibraryList[idx].get();
   }

   return result;
}

const ossimPluginLibrary* ossimSharedPluginRegistry::getPlugin(ossim_uint32 idx)const
{
   const ossimPluginLibrary* result = 0;
   
   if((idx>=0)&&(idx < theLibraryList.size()))
   {
      result = theLibraryList[idx].get();
   }

   return result;
}

ossim_uint32 ossimSharedPluginRegistry::getNumberOfPlugins()const
{
   return theLibraryList.size();
}

