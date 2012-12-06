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
// $Id: ossimRLevelFilter.cpp 21631 2012-09-06 18:10:55Z dburken $
#include <ossim/imaging/ossimRLevelFilter.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageGeometry.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/base/ossimTrace.h>

static const char* CURRENT_RLEVEL_KW    = "current_rlevel";
static const char* OVERRIDE_GEOMETRY_KW = "override_geometry";

static ossimTrace traceDebug("ossimRLevelFilter:debug");

RTTI_DEF1(ossimRLevelFilter, "ossimRLevelFilter", ossimImageSourceFilter);

//**************************************************************************************************
ossimRLevelFilter::ossimRLevelFilter()
   : ossimImageSourceFilter(),
     theCurrentRLevel(0),
     theOverrideGeometryFlag(false)
{
}

//**************************************************************************************************
ossimRLevelFilter::~ossimRLevelFilter()
{
}

//**************************************************************************************************
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

//**************************************************************************************************
bool ossimRLevelFilter::getOverrideGeometryFlag() const
{
   return theOverrideGeometryFlag;
}

//**************************************************************************************************
void ossimRLevelFilter::setOverrideGeometryFlag(bool override)
{
   theOverrideGeometryFlag = override;
}

//**************************************************************************************************
void ossimRLevelFilter::setCurrentRLevel(ossim_uint32 rlevel)
{
   theCurrentRLevel = rlevel;
   updateGeometry();
}

//**************************************************************************************************
ossim_uint32 ossimRLevelFilter::getCurrentRLevel()const
{
   return theCurrentRLevel;
}

//**************************************************************************************************
// Returns a pointer reference to the active image geometry at this filter. The input source
// geometry is modified, so we need to maintain our own geometry object as a data member.
//**************************************************************************************************
ossimRefPtr<ossimImageGeometry> ossimRLevelFilter::getImageGeometry()
{
   // Have we already defined our own geometry? Return it if so:
   if (m_ScaledGeometry.valid()) return m_ScaledGeometry;

   if (!theInputConnection) return ossimRefPtr<ossimImageGeometry>();

   ossim_uint32 rlevel = getCurrentRLevel();
   ossimRefPtr<ossimImageGeometry> inputGeom = theInputConnection->getImageGeometry();

   // If no scaling is happening, just return the input image geometry:
   if ((!inputGeom) || (rlevel == 0) || (getEnableFlag() == false))
      return inputGeom;

   // Need to create a copy of the input geom and modify it as our own, then pass that:
   m_ScaledGeometry = new ossimImageGeometry(*inputGeom);
   updateGeometry();

   // Return our version of the image geometry:
   return m_ScaledGeometry;
}

//**************************************************************************************************
//! If this object is maintaining an ossimImageGeometry, this method needs to be called after 
//! a scale change so that the geometry's projection is modified accordingly.
//**************************************************************************************************
void ossimRLevelFilter::updateGeometry()
{
   if (m_ScaledGeometry.valid())
   {
      // Modify the image geometry's projection with the scale factor before returning geom:
      ossimProjection* proj = m_ScaledGeometry->getProjection();
      ossimMapProjection* mapProj = PTR_CAST(ossimMapProjection, proj);;
      if(mapProj)
      {
         ossimDpt decimation;
         getDecimationFactor(theCurrentRLevel, decimation);
         decimation.x = 1.0/decimation.x;
         decimation.y = 1.0/decimation.y;

         // This will adjust both the scale and the tie point to account for decimation.
         mapProj->applyScale(decimation, true);  // recenter tie point flag
      }
   }
}

//**************************************************************************************************
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

//**************************************************************************************************
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

//**************************************************************************************************
ossimRefPtr<ossimImageData> ossimRLevelFilter::getTile(
   const ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
   if(!theInputConnection)
   {
      return 0;
   }

   if(!isSourceEnabled())
   {
      return theInputConnection->getTile(tileRect, resLevel);
   }

   // Ignoring resLevel as we're in charge of it here!
   return theInputConnection->getTile(tileRect, theCurrentRLevel);
}

//**************************************************************************************************
bool ossimRLevelFilter::loadState(const ossimKeywordlist& kwl,
                                  const char* prefix)
{
   const char* current  = kwl.find(prefix, CURRENT_RLEVEL_KW);
   const char* override = kwl.find(prefix, OVERRIDE_GEOMETRY_KW);

   if(current)
      theCurrentRLevel = ossimString(current).toULong();

   if(override)
      theOverrideGeometryFlag = ossimString(override).toBool();

   return ossimImageSourceFilter::loadState(kwl, prefix);

   // An existing image geometry object here will need to be updated:
   updateGeometry();
}

//**************************************************************************************************
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
