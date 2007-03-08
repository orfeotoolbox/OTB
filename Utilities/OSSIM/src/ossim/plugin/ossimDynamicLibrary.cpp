//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// LICENSE: LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*********************************************************************
// $Id: ossimDynamicLibrary.cpp 9094 2006-06-13 19:12:40Z dburken $
#include <ossim/plugin/ossimDynamicLibrary.h>
#include <ossim/plugin/ossimSharedObjectBridge.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotifyContext.h>


RTTI_DEF(ossimDynamicLibrary, "ossimDynamicLibrary");

// Static trace for debugging.
static ossimTrace traceDebug(ossimString("ossimDynamicLibrary:debug"));

ossimDynamicLibrary::ossimDynamicLibrary()
   :theLibrary(0)
{
}

ossimDynamicLibrary::ossimDynamicLibrary(const ossimString& name)
{
   load(name);
}

ossimDynamicLibrary::~ossimDynamicLibrary()
{
}

bool ossimDynamicLibrary::load()
{
   return load(theLibraryName);
}


bool ossimDynamicLibrary::load(const ossimString& name)
{
   ossimFilename libraryName = name;
#if OSSIM_DYNAMIC_ENABLED
#  if defined(__WIN32__) || defined(_WIN32)
   libraryName.convertForwardToBackSlashes();
   theLibrary = LoadLibrary(libraryName.c_str());
#  elif HAVE_DLFCN_H
   libraryName.convertBackToForwardSlashes();
   theLibrary = dlopen(libraryName.c_str(), RTLD_LAZY);
#  endif
#endif

   if (isLoaded())
   {
      theLibraryName = libraryName;
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimDynamicLibrary::load DEBUG:" << std::endl;
      
      if (isLoaded())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "Loaded library:  " << name << std::endl;
      }
      else
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimDynamicLibrary::load DEBUG:"
            << "\nFailed to load library:  " << name
            << std::endl;
#if OSSIM_DYNAMIC_ENABLED
#  if HAVE_DLFCN_H
         ossimNotify(ossimNotifyLevel_DEBUG) << dlerror() << std::endl;
#  endif
#endif
      }
      
   }
   
   return isLoaded();
}

void ossimDynamicLibrary::unload()
{
   if(isLoaded())
   {
#if OSSIM_DYNAMIC_ENABLED
#  if defined(__WIN32__) || defined(_WIN32)
      FreeLibrary(theLibrary);
#  elif HAVE_DLFCN_H
      dlclose(theLibrary);
#  endif
#endif
      theLibrary = NULL;
   }
}

void *ossimDynamicLibrary::getSymbol(const ossimString& name) const
{
   if(isLoaded())
   {
#if OSSIM_DYNAMIC_ENABLED
#  if defined(__WIN32__) || defined(_WIN32)
      return (void*)GetProcAddress( (HINSTANCE)theLibrary, name.c_str());
#  elif HAVE_DLFCN_H
      return dlsym(theLibrary, name.c_str());
#  endif
#endif
   }

   return (void*)NULL;
}
