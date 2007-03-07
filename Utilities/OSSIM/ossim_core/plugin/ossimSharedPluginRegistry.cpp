//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks)
// Description:
//
//*************************************************************************
// $Id: ossimSharedPluginRegistry.cpp,v 1.25 2005/05/20 20:28:27 dburken Exp $
#include <algorithm>
#include <plugin/ossimSharedPluginRegistry.h>
#include <base/common/ossimConstants.h>
#include <base/common/ossimTrace.h>
#include <base/common/ossimKeywordNames.h>
#include <base/data_types/ossimKeywordlist.h>
#include <plugin/ossimSharedObjectBridge.h>

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
               ossimNotify(ossimNotifyLevel_DEBUG) << "ossimSharedPluginRegistry DEBUG:"
                                                   << "ossimSharedLibraryInitialize symbol not found\n"
                                                   << endl;
            }
            
         }
      }
      else
      {
         delete lib;
         lib = 0;
      }
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
