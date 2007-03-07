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
//
//*************************************************************************
// $Id: ossimSharedObjectBridge.h,v 1.17 2005/05/20 20:28:27 dburken Exp $
#ifndef ossimSharedObjectBridge_HEADER
#define ossimSharedObjectBridge_HEADER
#include <vector>
#include "base/common/ossimObject.h"
#include "base/common/ossimConstants.h"

extern "C"
{
   typedef const char* (*ossimSharedLibraryGetDescriptionPtr)();
   typedef int (*ossimSharedLibraryGetNumberOfClassNamesPtr)();
   typedef const char* (*ossimSharedLibraryGetClassNamesPtr)(int idx);
   
   struct OSSIMDLLEXPORT ossimSharedObjectInfo
   {
      ossimSharedLibraryGetDescriptionPtr getDescription;
      ossimSharedLibraryGetNumberOfClassNamesPtr getNumberOfClassNames;
      ossimSharedLibraryGetClassNamesPtr getClassName;
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
