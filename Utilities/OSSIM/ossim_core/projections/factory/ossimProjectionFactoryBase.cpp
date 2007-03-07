//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//*******************************************************************
//  $Id: ossimProjectionFactoryBase.cpp,v 1.3 2004/09/24 17:19:40 dburken Exp $

#include <projections/factory/ossimProjectionFactoryBase.h>
#include <projections/factory/ossimProjectionFactoryRegistry.h>
#include <base/data_types/ossimFilename.h>

//---
// Define Trace flags for use within this file:
//---
#include <base/common/ossimTrace.h>
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
         << endl;
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
               << endl;
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
            << endl;
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
         << endl;
   }

   return NULL;
}
