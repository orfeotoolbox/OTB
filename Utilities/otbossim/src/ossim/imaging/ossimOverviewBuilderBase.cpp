//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:  Interface class for overview builders.
//
//----------------------------------------------------------------------------
// $Id: ossimOverviewBuilderBase.cpp 17932 2010-08-19 20:34:35Z dburken $

#include <ossim/imaging/ossimOverviewBuilderBase.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimPreferences.h>
#include <ossim/imaging/ossimImageHandler.h>

RTTI_DEF3(ossimOverviewBuilderBase,
          "ossimOverviewBuilderBase",
          ossimSource,
          ossimProcessInterface,
          ossimConnectableObjectListener)
   
ossimOverviewBuilderBase::ossimOverviewBuilderBase()
   : m_overviewStopDimension(0),
     m_histoMode(OSSIM_HISTO_MODE_UNKNOWN),
     m_maskBuildFlag(false)
{
   m_overviewStopDimension = getDefaultStopDimension();
}

ossimOverviewBuilderBase::~ossimOverviewBuilderBase()
{
}

bool ossimOverviewBuilderBase::setOutputWriter(ossimImageFileWriter* /* outputWriter */)
{
   return false;
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

      while(largestImageDimension > m_overviewStopDimension)
      {
         largestImageDimension /= 2;
         ++result;
      }
   }
   return result;
}

ossim_uint32 ossimOverviewBuilderBase::getOverviewStopDimension() const
{
   return m_overviewStopDimension;
}

void ossimOverviewBuilderBase::setOverviewStopDimension(ossim_uint32 dim)
{
   m_overviewStopDimension = dim;
}

ossimHistogramMode ossimOverviewBuilderBase::getHistogramMode() const
{
   return m_histoMode;
}

void ossimOverviewBuilderBase::setHistogramMode(ossimHistogramMode mode)
{
   m_histoMode = mode;
}

ossim_uint32 ossimOverviewBuilderBase::getDefaultStopDimension() const
{
   ossim_uint32 result = 0;
   
     // Get the stop dimension from ossim preferences.
   const char* lookup = ossimPreferences::instance()->
      findPreference(ossimKeywordNames::OVERVIEW_STOP_DIMENSION_KW);
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
