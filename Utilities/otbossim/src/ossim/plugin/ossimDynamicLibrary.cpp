//*******************************************************************
//
// LICENSE: LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*********************************************************************
// $Id: ossimDynamicLibrary.cpp 20694 2012-03-19 12:22:05Z dburken $
#include <ossim/plugin/ossimDynamicLibrary.h>
#include <ossim/plugin/ossimSharedObjectBridge.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotifyContext.h>

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
   unload();
}

bool ossimDynamicLibrary::load()
{
   return load(theLibraryName);
}

bool ossimDynamicLibrary::load(const ossimString& name)
{
   ossimFilename libraryName = name.trim();
   if(libraryName.empty()||!libraryName.isFile()) return false;
   
#  if defined(__WIN32__) || defined(_WIN32)
   theLibrary = LoadLibrary(libraryName.c_str());
#  else 
   theLibrary = dlopen(libraryName.c_str(), RTLD_LAZY);
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
#  if !defined(__WIN32__) && !defined(_WIN32)
         ossimNotify(ossimNotifyLevel_DEBUG) << dlerror() << std::endl;
#endif
      }
      
   }
   
   return isLoaded();
}

void ossimDynamicLibrary::unload()
{
   if(isLoaded())
   {
#if defined(__WIN32__) || defined(_WIN32)
      FreeLibrary(theLibrary);
// #else HAVE_DLFCN_H
#else
      dlclose(theLibrary);
#endif
      theLibrary = 0;
   }
}

void *ossimDynamicLibrary::getSymbol(const ossimString& name) const
{
   if(isLoaded())
   {
#if defined(__WIN32__) || defined(_WIN32)
      return (void*)GetProcAddress( (HINSTANCE)theLibrary, name.c_str());
#else
      return dlsym(theLibrary, name.c_str());
#endif
   }

   return (void*)0;
}
