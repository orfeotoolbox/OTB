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
// $Id: ossimPluginLibrary.cpp,v 1.4 2005/05/20 16:28:31 gpotts Exp $
#include "ossimPluginLibrary.h"

RTTI_DEF1(ossimPluginLibrary, "ossimPluginLibrary", ossimDynamicLibrary);

ossimPluginLibrary::ossimPluginLibrary()
   :ossimDynamicLibrary(),
    theInfo(0)
{
}

ossimPluginLibrary::ossimPluginLibrary(const ossimString& name)
   :ossimDynamicLibrary(name),
    theInfo(0)
{
   initialize();
}

ossimPluginLibrary::~ossimPluginLibrary()
{
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
      init(&theInfo);
   }
}

void ossimPluginLibrary::finalize()
{
   ossimSharedLibraryInitializePtr finalizeLib = (ossimSharedLibraryInitializePtr)getSymbol("ossimSharedLibraryFinalize");
   if(finalizeLib)
   {         
      finalizeLib(&theInfo);
   }
   
   unload();
}

ossimString ossimPluginLibrary::getDescription()const
{
   ossimString result;

   if(theInfo&&isLoaded()&&theInfo->getDescription)
   {
      result = theInfo->getDescription();
   }

   return result;
}

void ossimPluginLibrary::getClassNames(std::vector<ossimString>& classNames)const
{
   if(theInfo&&theInfo->getNumberOfClassNames&&theInfo->getClassName)
   {
      ossim_int32 idx        = 0;
      ossim_int32 numObjects = 0;

      numObjects = theInfo->getNumberOfClassNames();
      for(idx = 0; idx < numObjects; ++idx)
      {
         classNames.push_back(theInfo->getClassName(idx));
      }
   }
}
