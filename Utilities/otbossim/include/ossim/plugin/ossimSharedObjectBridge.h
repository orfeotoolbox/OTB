//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks)
//
//*************************************************************************
// $Id: ossimSharedObjectBridge.h 12048 2007-11-15 15:45:35Z gpotts $
#ifndef ossimSharedObjectBridge_HEADER
#define ossimSharedObjectBridge_HEADER
#include <vector>
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimConstants.h>

extern "C"
{
   typedef const char* (*ossimSharedLibraryGetDescriptionPtr)();
   typedef int (*ossimSharedLibraryGetNumberOfClassNamesPtr)();
   typedef const char* (*ossimSharedLibraryGetClassNamesPtr)(int idx);
   typedef const char* (*ossimSharedLibraryGetIdPtr())();
   typedef const char* (*ossimSharedLibraryGetOssimVersionStringPtr())();
   typedef void  (*ossimSharedLibraryRegisterPluginPtr())();
   
   struct OSSIMDLLEXPORT ossimSharedObjectInfo
   {
      ossimSharedLibraryGetDescriptionPtr        getDescription;
      ossimSharedLibraryGetNumberOfClassNamesPtr getNumberOfClassNames;
      ossimSharedLibraryGetClassNamesPtr         getClassName;
/*       ossimSharedLibraryGetId                    getId; */
/*       ossimSharedLibraryGetOssimVersionString    getOssimVersion; */
/*       ossimSharedLibraryRegisterPluginPtr        registerPlugin; */
   };
   
   
   typedef void (*ossimSharedLibraryInitializePtr)(ossimSharedObjectInfo** info);
   typedef void (*ossimSharedLibraryFinalizePtr)();

   /*
    * these need to define these in your plugin.  They are only here for clairity
    * 
    * extern void ossimSharedLibraryInitialize(ossimSharedObjectInfo** info);
    * extern void ossimSharedLibraryFinalize();
    */
}

#endif
