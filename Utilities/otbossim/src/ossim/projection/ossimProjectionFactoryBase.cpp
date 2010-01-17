//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//*******************************************************************
//  $Id: ossimProjectionFactoryBase.cpp 15766 2009-10-20 12:37:09Z gpotts $

#include <ossim/projection/ossimProjectionFactoryBase.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/imaging/ossimImageHandler.h>
//---
// Define Trace flags for use within this file:
//---
#include <ossim/base/ossimTrace.h>
static ossimTrace traceDebug = ossimTrace("ossimProjectionFactoryBase:debug");

RTTI_DEF1(ossimProjectionFactoryBase, "ossimProjectionFactoryBase", ossimObjectFactory);

ossimProjection* ossimProjectionFactoryBase::createProjectionFromGeometryFile(const ossimFilename& imageFile, ossim_uint32 entryIdx)const
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimProjectionFactoryBase::createProjectionFromGeometryFile "
         << "DEBUG:"
         << "\nimageFile:  " << imageFile
         << "\nentryIdx:   " << entryIdx
         << std::endl;
   }

   // Try to find a geometry file for imageFile.
   
   ossimFilename geomFile = imageFile;
   if (entryIdx == 0)
   {
      geomFile.setExtension("geom");
      if(geomFile.exists())
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "Found geometry file:  " << geomFile
               << std::endl;
         }

         ossimKeywordlist kwl;
         if(kwl.addFile(geomFile))
         {
            return ossimProjectionFactoryRegistry::instance()->
               createProjection(kwl);
         }
      }
   }

   // Look for an indexed geometry file.
   geomFile = imageFile.fileNoExtension();
   geomFile += "_e";
   geomFile += ossimString::toString(entryIdx);
   geomFile.setExtension("geom");
   if(geomFile.exists())
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "Found geometry file:  " << geomFile
            << std::endl;
      }
      
      ossimKeywordlist kwl;
      if(kwl.addFile(geomFile))
      {
         return ossimProjectionFactoryRegistry::instance()->
            createProjection(kwl);
      }
   }   

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG:  No geometry file found.  Returning NULL..."
         << std::endl;
   }

   return NULL;
}

ossimProjection* ossimProjectionFactoryBase::createProjection(ossimImageHandler* handler)const
{
   if(!handler) return 0;
   return createProjection(handler->getFilename(), handler->getCurrentEntry());
}

