//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:  Interface class for overview builders.
//
//----------------------------------------------------------------------------
// $Id: ossimOverviewBuilderBase.cpp 15366 2009-09-04 13:30:04Z dburken $

#include <ossim/imaging/ossimOverviewBuilderBase.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimPreferences.h>
#include <ossim/imaging/ossimImageHandler.h>

const char* ossimOverviewBuilderBase::OVERVIEW_STOP_DIMENSION_KW =
   "overview_stop_dimension";

// Property keywords.
RTTI_DEF3(ossimOverviewBuilderBase,
          "ossimOverviewBuilderBase",
          ossimSource,
          ossimProcessInterface,
          ossimConnectableObjectListener)
   
ossimOverviewBuilderBase::ossimOverviewBuilderBase()
   : theOverviewStopDimension(0)
{
   theOverviewStopDimension = getDefaultStopDimension();
}

ossimOverviewBuilderBase::~ossimOverviewBuilderBase()
{
}

bool ossimOverviewBuilderBase::hasOverviewType(const ossimString& type) const
{
   std::vector<ossimString> typeList;
   getTypeNameList(typeList);
   std::vector<ossimString>::const_iterator i = typeList.begin();
   while ( i != typeList.end() )
   {
      if ( (*i) == type )
      {
         return true;
      }
      ++i;
   }
   return false;
}

ossim_uint32 ossimOverviewBuilderBase::getRequiredResLevels(
   const ossimImageHandler* ih) const
{
   ossim_uint32 result = 0;
   if (ih)
   {
      // Note we always have one rset
      result = 1;

      ossim_uint32 largestImageDimension =
         ih->getNumberOfSamples(0) >
         ih->getNumberOfLines(0) ?
         ih->getNumberOfSamples(0) :
         ih->getNumberOfLines(0);

      while(largestImageDimension > theOverviewStopDimension)
      {
         largestImageDimension /= 2;
         ++result;
      }
   }
   return result;
}

ossim_uint32 ossimOverviewBuilderBase::getOverviewStopDimension() const
{
   return theOverviewStopDimension;
}

void ossimOverviewBuilderBase::setOverviewStopDimension(ossim_uint32 dim)
{
   theOverviewStopDimension = dim;
}

ossim_uint32 ossimOverviewBuilderBase::getDefaultStopDimension() const
{
   ossim_uint32 result = 0;
   
     // Get the stop dimension from ossim preferences.
   const char* lookup = ossimPreferences::instance()->
      findPreference(OVERVIEW_STOP_DIMENSION_KW);
   if (lookup)
   {
      result = ossimString(lookup).toUInt32();
   }

   if (result == 0)
   {
      // Use the smallest default tile size.
      ossimIpt tileSize;
      ossim::defaultTileSize(tileSize);
      result  = tileSize.x < tileSize.y ? tileSize.x : tileSize.y;
   }
   return result;
}
