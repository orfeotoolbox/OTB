//*******************************************************************
///
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
// 
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimPluginLibrary.cpp 20694 2012-03-19 12:22:05Z dburken $
#include <ossim/plugin/ossimPluginLibrary.h>
#include <iostream>

ossimPluginLibrary::ossimPluginLibrary()
   :ossimDynamicLibrary(),
    m_info(0)
{
}

ossimPluginLibrary::ossimPluginLibrary(const ossimString& name, const ossimString& options)
   :ossimDynamicLibrary(name),
    m_options(options),
    m_info(0)
{
   initialize();
}

ossimPluginLibrary::~ossimPluginLibrary()
{
   finalize();
}

void ossimPluginLibrary::initialize()
{
   if(!isLoaded())
   {
      load();
   }
   
   ossimSharedLibraryInitializePtr init = (ossimSharedLibraryInitializePtr)getSymbol("ossimSharedLibraryInitialize");
   if(init)
   {
      init(&m_info, m_options.c_str());
   }
}

void ossimPluginLibrary::finalize()
{
   ossimSharedLibraryFinalizePtr finalizeLib = (ossimSharedLibraryFinalizePtr)getSymbol("ossimSharedLibraryFinalize");
   if(finalizeLib)
   {         
      finalizeLib();
   }
   
   unload();
}

ossimString ossimPluginLibrary::getDescription()const
{
   ossimString result;

   if(m_info&&isLoaded()&&m_info->getDescription)
   {
      result = m_info->getDescription();
   }

   return result;
}

void ossimPluginLibrary::getClassNames(std::vector<ossimString>& classNames)const
{
   if(m_info&&m_info->getNumberOfClassNames&&m_info->getClassName)
   {
      ossim_int32 idx        = 0;
      ossim_int32 numObjects = 0;

      numObjects = m_info->getNumberOfClassNames();
      for(idx = 0; idx < numObjects; ++idx)
      {
         classNames.push_back(m_info->getClassName(idx));
      }
   }
}

void ossimPluginLibrary::setOptions(const ossimString& options)
{
   m_options = options;
   
}
