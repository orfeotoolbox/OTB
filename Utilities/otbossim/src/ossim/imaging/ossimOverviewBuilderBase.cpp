//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:  Interface class for overview builders.
//
//----------------------------------------------------------------------------
// $Id: ossimOverviewBuilderBase.cpp 21745 2012-09-16 15:21:53Z dburken $

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

static const std::string SCAN_FLOAT_DATA_KW = "overview_builder.scan_for_min_max_null_if_float";
   
ossimOverviewBuilderBase::ossimOverviewBuilderBase()
   : m_overviewStopDimension(0),
     m_histoMode(OSSIM_HISTO_MODE_UNKNOWN),
     m_bitMaskSpec(),
     m_imageHandler(0),
     m_originalBandList(0),
     m_maskWriter(0),
     m_maskFilter(0),
     m_outputFile(ossimFilename::NIL),
     m_scanForMinMax(false),
     m_scanForMinMaxNull(false),
     m_scanFloatData(false)
{
   initializePreferenceSettings();
}

ossimOverviewBuilderBase::~ossimOverviewBuilderBase()
{
   m_imageHandler = 0;
   m_maskWriter = 0;
   m_maskFilter = 0;
}

bool ossimOverviewBuilderBase::setOutputWriter(ossimImageFileWriter* /* outputWriter */)
{
   return false;
}

bool ossimOverviewBuilderBase::setInputSource(ossimImageHandler* imageSource)
{
   bool result = false;
   if ( imageSource )
   {
      if ( imageSource->getErrorStatus() == ossimErrorCodes::OSSIM_OK )
      {
         m_imageHandler = imageSource;

         // Check handler to see if it's filtering bands.
         if ( m_imageHandler->isBandSelector() )
         { 
            // Capture for finalize method.
            m_imageHandler->getOutputBandList( m_originalBandList );
            
            // Set output list to input.
            m_imageHandler->setOutputToInputBandList();
         }

         // This will set the flag to scan for min, max, nulls if needed.
         initializeScanOptions();
         
         result = true;
      }
   }
   return result;
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

void ossimOverviewBuilderBase::finalize()
{
   if ( m_imageHandler.valid() && m_imageHandler->isBandSelector() && m_originalBandList.size() )
   {
      m_imageHandler->setOutputBandList( m_originalBandList );
   }
}

void ossimOverviewBuilderBase::setBitMaskSpec(const ossimKeywordlist& bit_mask_spec)
{
   m_bitMaskSpec = bit_mask_spec;
}

void ossimOverviewBuilderBase::setScanForMinMax(bool flag)
{
   m_scanForMinMax  = flag;
}

bool ossimOverviewBuilderBase::getScanForMinMax() const
{
   return m_scanForMinMax;
}

void ossimOverviewBuilderBase::setScanForMinMaxNull(bool flag)
{
   m_scanForMinMaxNull = flag;
}

bool ossimOverviewBuilderBase::getScanForMinMaxNull() const
{
   return m_scanForMinMaxNull;
}

void ossimOverviewBuilderBase::initializePreferenceSettings()
{
   // Get the stop dimension from ossim preferences.
   const char* lookup = ossimPreferences::instance()->
      findPreference(ossimKeywordNames::OVERVIEW_STOP_DIMENSION_KW);
   if (lookup)
   {
     m_overviewStopDimension = ossimString(lookup).toUInt32();
   }

   if ( m_overviewStopDimension == 0 )
   {
      // Use the smallest default tile size.
      ossimIpt tileSize;
      ossim::defaultTileSize(tileSize);
      m_overviewStopDimension = tileSize.x < tileSize.y ? tileSize.x : tileSize.y;
   }

   // Look for overview_builder.scan_for_min_max_null_if_float:
   lookup = ossimPreferences::instance()->findPreference(SCAN_FLOAT_DATA_KW.c_str());
   if ( lookup )
   {
      m_scanFloatData = ossimString(lookup).toBool();
   }
}

void ossimOverviewBuilderBase::initializeScanOptions()
{
   if ( m_scanFloatData && m_imageHandler.valid())
   {
      ossimScalarType scalar = m_imageHandler->getOutputScalarType();
      if ( ( scalar == OSSIM_FLOAT32 ) || ( scalar == OSSIM_FLOAT64 ) )
      {
         setScanForMinMaxNull(true);
      }
   }
}
