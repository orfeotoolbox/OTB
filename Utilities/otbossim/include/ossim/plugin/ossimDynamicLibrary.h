//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// LICENSE: LGPL
//
// See LICENSE.txt file in the top level directory for more details.
// 
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimDynamicLibrary.h 12319 2008-01-16 19:55:24Z gpotts $
#ifndef ossimDynamicLibrary_HEADER
#define ossimDynamicLibrary_HEADER

#include <ossim/base/ossimString.h>
#include <ossim/base/ossimRtti.h>
#include <ossim/base/ossimReferenced.h>

#include <ossim/ossimConfig.h>

#if defined(_WIN32) || defined(__WIN32__)
#include <windows.h>
#elif HAVE_DLFCN_H
#include <dlfcn.h>
#else
#define OSSIM_DYNAMIC_DISABLED
// #error "dynamic loading interface is unsupported on this platform"
#endif

class ossimDynamicLibrary : public ossimReferenced
{
public:
   ossimDynamicLibrary();
   ossimDynamicLibrary(const ossimString& name);
   virtual ~ossimDynamicLibrary();
   // return TRUE if the library was loaded successfully
   bool isLoaded() const { return theLibrary != 0; }

   // load the library with the given name (full or not), return TRUE on
   // success
   bool load(const ossimString& name);

   bool load();
   // unload the library, also done automatically in dtor
   void unload();

   // load a symbol from the library, return NULL if an error occured or
   // symbol wasn't found
   void *getSymbol(const ossimString& name) const;

   const ossimString& getName()const
      {
         return theLibraryName;
      }

protected:
    // the handle to DLL or NULL
#if defined(_WIN32)
	HINSTANCE theLibrary;
#else
	void* theLibrary;
#endif
   ossimString theLibraryName;

TYPE_DATA
};


#endif
