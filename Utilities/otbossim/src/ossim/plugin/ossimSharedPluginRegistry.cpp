//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimSharedPluginRegistry.cpp 20608 2012-02-27 12:03:40Z gpotts $
#include <algorithm>
#include <iterator>
#include <ossim/plugin/ossimSharedPluginRegistry.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/plugin/ossimSharedObjectBridge.h>

//ossimSharedPluginRegistry* ossimSharedPluginRegistry::theInstance = NULL;
//ossimPluginBridgeStructure ossimSharedPluginRegistry::thePluginBridgeFactoryPointers;

static ossimTrace traceDebug("ossimSharedPluginRegistry:debug");

ossimSharedPluginRegistry::ossimSharedPluginRegistry()
{
}

ossimSharedPluginRegistry::~ossimSharedPluginRegistry()
{
   theLibraryList.clear();
}

ossimSharedPluginRegistry* ossimSharedPluginRegistry::instance()
{
   static ossimSharedPluginRegistry sharedInstance;

   return &sharedInstance;//theInstance;
}

bool ossimSharedPluginRegistry::registerPlugin(const ossimFilename& filename, const ossimString& options)//, bool insertFrontFlag)
{
   bool result = false;
   if(!getPlugin(filename))
   {
      ossimPluginLibrary *lib =new ossimPluginLibrary;
      if(lib->load(filename))
      {
         lib->setOptions(options);
         if(lib->getSymbol("ossimSharedLibraryInitialize"))
         {
            lib->initialize();
//            if(!insertFrontFlag)
//            {
               theLibraryList.push_back(lib);
//            }
//            else
//            {
//               theLibraryList.insert(theLibraryList.begin(), lib);
 //           }
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
   
   return unregisterPlugin(lib);
}

bool ossimSharedPluginRegistry::unregisterPlugin(ossimPluginLibrary* library)
{
   std::vector<ossimRefPtr<ossimPluginLibrary> >::iterator iter = theLibraryList.begin();
   while(iter!=theLibraryList.end())
   {
      if((*iter).get() == library)
      {
         theLibraryList.erase(iter);
         return true;
      }
      ++iter;
   }
   return false;
}

const ossimPluginLibrary* ossimSharedPluginRegistry::getPlugin(const ossimFilename& filename)const
{
   ossim_uint32 idx = 0;
   ossimFilename fileOnly = filename.file();
   for(idx = 0; idx < theLibraryList.size();++idx)
   {
      if(fileOnly == ossimFilename(theLibraryList[idx]->getName()).file())
      {
         return theLibraryList[idx].get();
      }
   }
   
   return 0;
}

ossimPluginLibrary* ossimSharedPluginRegistry::getPlugin(const ossimFilename& filename)
{
   ossim_uint32 idx = 0;
   ossimFilename fileOnly = filename.file();

   for(idx = 0; idx < theLibraryList.size();++idx)
   {
      if(fileOnly == ossimFilename(theLibraryList[idx]->getName()).file())
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
   
   if(idx < theLibraryList.size())
   {
      result = theLibraryList[idx].get();
   }

   return result;
}

const ossimPluginLibrary* ossimSharedPluginRegistry::getPlugin(ossim_uint32 idx)const
{
   const ossimPluginLibrary* result = 0;
   
   if(idx < theLibraryList.size())
   {
      result = theLibraryList[idx].get();
   }

   return result;
}

ossim_uint32 ossimSharedPluginRegistry::getNumberOfPlugins()const
{
   return (ossim_uint32)theLibraryList.size();
}


bool ossimSharedPluginRegistry::isLoaded(const ossimFilename& filename) const
   
{
   ossimFilename fileOnly = filename.file();
   bool result = false;
   ossim_uint32 count = getNumberOfPlugins();
   for (ossim_uint32 i = 0; i < count; ++i)
   {
      const ossimPluginLibrary* pi = getPlugin(i);
      if (pi)
      {
         if (fileOnly == ossimFilename(pi->getName()).file())
         {
            result = true;
            break;
         }
      }
   }
   return result;
}

void ossimSharedPluginRegistry::printAllPluginInformation(std::ostream& out)
{
   ossim_uint32 count = getNumberOfPlugins();
   ossim_uint32 idx = 0;
   
   for(idx = 0; idx < count; ++idx)
   {
      std::vector<ossimString> classNames;
      const ossimPluginLibrary* pi = getPlugin(idx);
      if(pi)
      {
         pi->getClassNames(classNames);
         out << "Plugin: " << pi->getName() << std::endl;
         out << "DESCRIPTION: \n";
         out << pi->getDescription() << "\n";
         out << "CLASSES SUPPORTED\n     ";
         std::copy(classNames.begin(),
                   classNames.end(),
                   std::ostream_iterator<ossimString>(out, "\n     "));
         out << "\n";
      }
   }
}
