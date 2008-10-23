//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimRLevelFilter.cpp 9526 2006-09-06 16:18:51Z dburken $
#include <ossim/imaging/ossimRLevelFilter.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/base/ossimTrace.h>

static const char* CURRENT_RLEVEL_KW    = "current_rlevel";
static const char* OVERRIDE_GEOMETRY_KW = "override_geometry";

static ossimTrace traceDebug("ossimRLevelFilter:debug");

RTTI_DEF1(ossimRLevelFilter, "ossimRLevelFilter", ossimImageSourceFilter);

ossimRLevelFilter::ossimRLevelFilter()
   : ossimImageSourceFilter(),
     theCurrentRLevel(0),
     theOverrideGeometryFlag(false)
{
}

ossimRLevelFilter::~ossimRLevelFilter()
{
}

void ossimRLevelFilter::getDecimationFactor(ossim_uint32 resLevel,
                                            ossimDpt& result) const
{
   if(!theInputConnection)
   {
      result.makeNan();
   }

   theInputConnection->getDecimationFactor(resLevel,
                                           result);
}

bool ossimRLevelFilter::getOverrideGeometryFlag() const
{
   return theOverrideGeometryFlag;
}

void ossimRLevelFilter::setOverrideGeometryFlag(bool override)
{
   theOverrideGeometryFlag = override;
}

void ossimRLevelFilter::setCurrentRLevel(ossim_uint32 rlevel)
{
   theCurrentRLevel = rlevel;
}

ossim_uint32 ossimRLevelFilter::getCurrentRLevel()const
{
   return theCurrentRLevel;
}

bool ossimRLevelFilter::getImageGeometry(ossimKeywordlist& kwl,
                                         const char* prefix)
{
   if (!theInputConnection)
   {
      return false;
   }

   ossim_uint32 rlevel = getCurrentRLevel();
   bool status = theInputConnection->getImageGeometry(kwl, prefix);

   if (!theOverrideGeometryFlag || (status == false) ||
       (rlevel == 0) || (getEnableFlag() == false) )
   {
      return status;
   }

   ossimProjection* proj = ossimProjectionFactoryRegistry::instance()->
      createProjection(kwl);
   ossimMapProjection* mapProj = PTR_CAST(ossimMapProjection, proj);;
   if(mapProj)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimRLevelFilter::getImageGeometry DEBUG:"
            << "\nOriginal projection:\n";
         mapProj->print(ossimNotify(ossimNotifyLevel_DEBUG));
      }

      ossimDpt decimation;
      getDecimationFactor(rlevel, decimation);
      decimation.x = 1.0/decimation.x;
      decimation.y = 1.0/decimation.y;

      //---
      // This will adjust both the scale and the tie point to account for
      // decimation.
      //---
      mapProj->applyScale(decimation,
                          true);  // recenter tie point flag

      //---
      // There are still old keywords out there so clear the original
      // geometry file out prior to saving.
      //---
      kwl.clear();

      // Save off the changes back to the keyword list.
      mapProj->saveState(kwl, prefix);

      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimRLevelFilter::getImageGeometry DEBUG:"
            << "\nUpdated projection:\n";
         mapProj->print(ossimNotify(ossimNotifyLevel_DEBUG));
         
      }

      return true;

   } // End of "if(mapProj))"

   return status;
}

void ossimRLevelFilter::getSummedDecimation(ossimDpt& result) const
{
   // Start with fresh values.
   result.x = 0.0;
   result.y = 0.0;

   ossim_uint32 rlevel = getCurrentRLevel();

   if (rlevel == 0)
   {
      return; // no decimation.
   }

   // Sum the decimations from rlevel one to the current rlevel.
   for (ossim_uint32 i = 1; i <= rlevel; ++i)
   {
      ossimDpt dpt;
      getDecimationFactor(i, dpt);
      if (dpt.hasNans())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimRLevelFilter::getSummedDecimation WARNING:"
            << "\nNans returned from getDecimationFactor!"
            << std::endl;
         result.makeNan();
         return;
      }
      result.x += dpt.x;
      result.y += dpt.y;
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimRLevelFilter::getSummedDecimation result: " << result
         << std::endl;
   }
}

ossimIrect ossimRLevelFilter::getBoundingRect(ossim_uint32 resLevel)const
{
   ossimIrect rect;
   rect.makeNan();
   if(!theInputConnection) return rect;

   if (isSourceEnabled())
   {
      return theInputConnection->getBoundingRect(theCurrentRLevel);
   }
   return theInputConnection->getBoundingRect(resLevel);
}

ossimRefPtr<ossimImageData> ossimRLevelFilter::getTile(
   const ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
   if(!theInputConnection)
   {
      return NULL;
   }

   if(!isSourceEnabled())
   {
      return theInputConnection->getTile(tileRect, resLevel);
   }

   // Ignoring resLevel as we're in charge of it here!
   return theInputConnection->getTile(tileRect, theCurrentRLevel);
}

bool ossimRLevelFilter::loadState(const ossimKeywordlist& kwl,
                                  const char* prefix)
{
   const char* current  = kwl.find(prefix, CURRENT_RLEVEL_KW);
   const char* override = kwl.find(prefix, OVERRIDE_GEOMETRY_KW);

   if(current)
   {
      theCurrentRLevel = ossimString(current).toULong();
   }

   if(override)
   {
      theOverrideGeometryFlag = ossimString(override).toBool();
   }

   return ossimImageSourceFilter::loadState(kwl, prefix);
}

bool ossimRLevelFilter::saveState(ossimKeywordlist& kwl,
                                  const char* prefix)const
{
   kwl.add(prefix,
           CURRENT_RLEVEL_KW,
           theCurrentRLevel,
           true);

   kwl.add(prefix,
           OVERRIDE_GEOMETRY_KW,
           (ossim_uint32)theOverrideGeometryFlag,
           true);

   return ossimImageSourceFilter::saveState(kwl, prefix);
}
