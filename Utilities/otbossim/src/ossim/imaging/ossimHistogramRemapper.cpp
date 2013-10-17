//*******************************************************************
// 
// See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:
//
// Histogram remapper class declaration.  Derived from ossimTableRemapper.
//
// Remaps a tile based on mode and histogram clip points.
//
//*************************************************************************
// $Id: ossimHistogramRemapper.cpp 22187 2013-03-07 20:29:00Z dburken $

#include <ossim/imaging/ossimHistogramRemapper.h>
#include <ossim/base/ossimMultiResLevelHistogram.h>
#include <ossim/base/ossimMultiBandHistogram.h>
#include <ossim/base/ossimHistogram.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimHistogramSource.h>
#include <ossim/imaging/ossimImageData.h>
#include <cstdlib>
#include <iomanip>

RTTI_DEF1(ossimHistogramRemapper, "ossimHistogramRemapper", ossimTableRemapper)

static ossimTrace traceDebug("ossimHistogramRemapper:debug");

static const char* STRETCH_MODE[] = { "linear_one_piece",
                                      "linear_1std_from_mean",
                                      "linear_2std_from_mean",
                                      "linear_3std_from_mean",
                                      "linear_auto_min_max",
                                      "stretch_unknown" };

static const char NORMALIZED_LOW_CLIP_POINT_KW[] = "normalized_low_clip_point";
static const char NORMALIZED_HIGH_CLIP_POINT_KW[]="normalized_high_clip_point";
static const char MID_POINT_KW[] = "mid_point";
static const char MIN_OUTPUT_VALUE_KW[] = "min_output_value";
static const char MAX_OUTPUT_VALUE_KW[] = "max_output_value";
static const char STRETCH_MODE_KW[] = "stretch_mode";
static const char HISTOGRAM_FILENAME_KW[] = "histogram_filename";

#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimHistogramRemapper.cpp 22187 2013-03-07 20:29:00Z dburken $";
#endif

ossimHistogramRemapper::ossimHistogramRemapper()
   :
   ossimTableRemapper(),  // base class
   theStretchMode(ossimHistogramRemapper::LINEAR_ONE_PIECE),
   theDirtyFlag(false),
   theHistogram(0),
   theNormalizedLowClipPoint(),
   theNormalizedHighClipPoint(),
   theMidPoint(),
   theMinOutputValue(),
   theMaxOutputValue(),
   theBandList(),
   theBypassFlag(true),
   theResetBandIndicesFlag(false)
{
   setNumberOfInputs(2);
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimHistogramRemapper::ossimHistogramRemapper entered...\n";
#ifdef OSSIM_ID_ENABLED
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "OSSIM_ID:  " << OSSIM_ID << "\n";
#endif      
   }
}

ossimHistogramRemapper::~ossimHistogramRemapper()
{
}

void ossimHistogramRemapper::initialize()
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimHistogramRemapper::initialize entered..." << endl;
   }
	

   theResetBandIndicesFlag=(dynamic_cast<const ossimHistogramSource*>(getInput(1))!=0);
   //---
   // Call the base class initialize.
   // Note:  This will reset "theInputConnection" if it changed...
   //---
   ossimTableRemapper::initialize();
	
   // Check the band list to see if it's changes.
   vector<ossim_uint32> bandList;
   getOutputBandList(bandList);
   ossim_uint32 idx = 0;
   // bool emptyOutput = theBandList.empty();

   if(theResetBandIndicesFlag)
   {
      for(idx = 0; idx < bandList.size();++idx)
      {
         bandList[idx] = idx;
      }
   }
   if (theBandList.size() != 0 && bandList.size())
   {
      if (theBandList.size() != bandList.size())
      {
         // Somethings changed.  Set everthing back.
         initializeClips();
         setNullCount();
         theTable.clear();
         theDirtyFlag = true;
      }
      else
      {
         // Sizes same check the order.
         for(ossim_uint32 band = 0; band < theBandList.size(); ++band)
         {
            if (theBandList[band] != bandList[band])
            {
               //initializeClips();
               //setNullCount();
               theDirtyFlag = true;
               break;
            }            
         }
      }
   }
   else
   {
      initializeMinMaxOutput();
   }

   // Update the band list with the latest.
   theBandList = bandList;
	
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimHistogramRemapper::initialize exited..." << endl;
   }
   verifyEnabled();

}

void ossimHistogramRemapper::reset()
{
   // We could delete theTable to free up memory???
   setStretchMode(LINEAR_ONE_PIECE, false);
   initializeClips();
   verifyEnabled();
}

void
ossimHistogramRemapper::setHistogram(ossimRefPtr<ossimMultiResLevelHistogram> histogram)
{
   theHistogram = histogram;
   setNullCount();
	
   // Note: initializeClips before setNullCount since it relies on clips.
   initializeClips();
   theTable.clear();
   theDirtyFlag = true;
}

bool ossimHistogramRemapper::openHistogram(const ossimFilename& histogram_file)
{
   ossimRefPtr<ossimMultiResLevelHistogram> h = new ossimMultiResLevelHistogram();
   if (h->importHistogram(histogram_file))
   {
      setHistogram(h);
      return true;
   }
	
   // Import failed if here!
   return false;
}

ossimRefPtr<ossimImageData> ossimHistogramRemapper::getTile(
   const ossimIrect& tile_rect,
   ossim_uint32 resLevel)
{
   
#if 0 /* Please leave for serious debug. (drb) */
   cout << "\ntheEnableFlag: " << theEnableFlag
        << "\ntheBypassFlag: " << theBypassFlag
        << "\ntheDirtyFlag:  " << theDirtyFlag
        << endl;
#endif
   ossimRefPtr<ossimImageData> result = 0;

   makeClean();
   
   if (!theBypassFlag||!theEnableFlag) // Not bypassed...
   {
      // Base handles the rest...
      result = ossimTableRemapper::getTile(tile_rect, resLevel);
   }
   else if (theInputConnection)
   {
      // Fetch tile from pointer from the input source.
      result = theInputConnection->getTile(tile_rect, resLevel);
   }

   return result;
}

void ossimHistogramRemapper::setLowNormalizedClipPoint(const ossim_float64& clip)
{
   const ossim_uint32 BANDS = getNumberOfInputBands();
   for (ossim_uint32 band = 0; band < BANDS; ++band)
   {
      setLowNormalizedClipPoint(clip, band);
   }
}

void ossimHistogramRemapper::makeClean()
{
   if (theEnableFlag && theDirtyFlag) // Enabled and dirty.
   {
      // Always rebuild table if dirty.
      buildTable();
      
      // Check for internal bypass.
      verifyEnabled();

      // Always clear the dirty flag.
      theDirtyFlag = false;
   }
}

void ossimHistogramRemapper::setLowNormalizedClipPoint(const ossim_float64& clip,
                                                  ossim_uint32 zero_based_band)
{
   const ossim_uint32 BANDS = getNumberOfInputBands();
	
   if (zero_based_band >= BANDS)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHistogramRemapper::setLowNormalizedClipPoint ERROR:"
         << "\nband " << zero_based_band << " is out of range!"
         << "\nhighest band:  " << (BANDS-1) << endl;
   }
	
   if (theNormalizedLowClipPoint.size() != BANDS)
   {
      initializeClips();
   }
	
   if ( clip != theNormalizedLowClipPoint[zero_based_band] &&
        clip < theNormalizedHighClipPoint[zero_based_band] )
   {
      theDirtyFlag = true;
      theNormalizedLowClipPoint[zero_based_band] = clip;
   }
}

void
ossimHistogramRemapper::setHighNormalizedClipPoint(const ossim_float64& clip)
{
   const ossim_uint32 BANDS = getNumberOfInputBands();
   for (ossim_uint32 band = 0; band < BANDS; ++band)
   {
      setHighNormalizedClipPoint(clip, band);
   }
}

void ossimHistogramRemapper::setHighNormalizedClipPoint(
   const ossim_float64& clip, ossim_uint32 zero_based_band)
{
   const ossim_uint32 BANDS = getNumberOfInputBands();
	
   if (zero_based_band >= BANDS)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHistogramRemapper::setHighNormalizedClipPoint ERROR:"
         << "\nband " << zero_based_band << " is out of range!"
         << "\nhighest band:  " << (BANDS-1) << endl;
   }
	
   if (theNormalizedHighClipPoint.size() != BANDS)
   {
      initializeClips();
   }
	
   if ( clip != theNormalizedHighClipPoint[zero_based_band] &&
        clip > theNormalizedLowClipPoint[zero_based_band] )
   {
      theDirtyFlag = true;
      theNormalizedHighClipPoint[zero_based_band] = clip;
   }
}

void ossimHistogramRemapper::setLowClipPoint(const ossim_float64& clip)
{
   const ossim_uint32 BANDS = getNumberOfInputBands();
   for (ossim_uint32 band = 0; band < BANDS; ++band)
   {
      setLowClipPoint(clip, band);
   }
}

void ossimHistogramRemapper::setLowClipPoint(const ossim_float64& clip,
                                             ossim_uint32 zero_based_band)
{
   // allow the call to getHistogram to happen this way we can calculate a histogram if 
   // a histosource is connected to this object
   //
   //   if (!theHistogram) return;
	
   ossimRefPtr<ossimHistogram> h = getHistogram(zero_based_band);
   if (h.valid())
   {
      ossim_float64 d = h->getLowFractionFromValue(clip);
      if (ossim::isnan(d) == false)
      {
         setLowNormalizedClipPoint(d, zero_based_band);
      }
   }
}

void ossimHistogramRemapper::setHighClipPoint(const ossim_float64& clip)
{
   const ossim_uint32 BANDS = getNumberOfInputBands();
   for (ossim_uint32 band = 0; band < BANDS; ++band)
   {
      setHighClipPoint(clip, band);
   }
}

void ossimHistogramRemapper::setHighClipPoint(const ossim_float64& clip,
                                              ossim_uint32 zero_based_band)
{
   // allow the call to getHistogram to happen this way we can calculate a histogram if 
   // a histosource is connected to this object
   //
   //   if (!theHistogram) return;
	
   ossimRefPtr<ossimHistogram> h = getHistogram(zero_based_band);
   if (h.valid())
   {
      ossim_float64 d = h->getHighFractionFromValue(clip);
      if (ossim::isnan(d) == false)
      {
         d = 1.0-d;
         setHighNormalizedClipPoint(d, zero_based_band);
      }
   }
}

void ossimHistogramRemapper::setMidPoint(const ossim_float64& value)
{
   const ossim_uint32 BANDS = getNumberOfInputBands();
   for (ossim_uint32 band = 0; band < BANDS; ++band)
   {
      setMidPoint(value, band);
   }
}

void ossimHistogramRemapper::setMidPoint(const ossim_float64& value,
                                         ossim_uint32 zero_based_band)
{
   const ossim_uint32 BANDS = getNumberOfInputBands();
	
   if (zero_based_band >= BANDS)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHistogramRemapper::setMidPoint ERROR:"
         << "\nband " << zero_based_band << " is out of range!"
         << "\nhighest band:  " << (BANDS-1) << endl;
   }
	
   if (theMidPoint.size() != BANDS)
   {
      initializeClips();
   }
	
   if (theMidPoint[zero_based_band] != value)
   {
      theDirtyFlag = true;
      theMidPoint[zero_based_band] = value;
   }
}

void ossimHistogramRemapper::setMinOutputValue(const ossim_float64& value)
{
   const ossim_uint32 BANDS = getNumberOfInputBands();
   for (ossim_uint32 band = 0; band < BANDS; ++band)
   {
      setMinOutputValue(value, band);
   }
}

void ossimHistogramRemapper::setMinOutputValue(const ossim_float64& value,
                                               ossim_uint32 zero_based_band)
{
   if (theInputConnection)
   {
      const ossim_uint32 BANDS = getNumberOfInputBands();
      
      if (zero_based_band >= BANDS)
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimHistogramRemapper::setMinOutputValue ERROR:"
            << "\nband " << zero_based_band << " is out of range!"
            << "\nhighest band:  " << (BANDS-1) << endl;
      }
      
      if (theMinOutputValue.size() != BANDS)
      {
         initializeClips();
      }
      
      if ( value != theMinOutputValue[zero_based_band] &&
           value >= theInputConnection->getMinPixelValue(zero_based_band) &&
           value < theMaxOutputValue[zero_based_band] )
      {
         theDirtyFlag = true;
         theMinOutputValue[zero_based_band] = value;
      }
   }
}

void ossimHistogramRemapper::setMaxOutputValue(const ossim_float64& value)
{
   const ossim_uint32 BANDS = getNumberOfInputBands();
   for (ossim_uint32 band = 0; band < BANDS; ++band)
   {
      setMaxOutputValue(value, band);
   }
}

void ossimHistogramRemapper::setMaxOutputValue(const ossim_float64& value,
                                               ossim_uint32 zero_based_band)
{
   if (theInputConnection)
   {
      const ossim_uint32 BANDS = getNumberOfInputBands();
      
      if (zero_based_band >= BANDS)
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimHistogramRemapper::setMidPoint ERROR:"
            << "\nband " << zero_based_band << " is out of range!"
            << "\nhighest band:  " << (BANDS-1) << endl;
      }
      
      if (theMaxOutputValue.size() != BANDS)
      {
         initializeClips();
      }
      
      if ( value != theMaxOutputValue[zero_based_band] &&
           value <= theInputConnection->getMaxPixelValue(zero_based_band) &&
           value >  theMinOutputValue[zero_based_band] )
      {
         theDirtyFlag = true;
         theMaxOutputValue[zero_based_band] = value;
      }
   }
}

ossim_float64 ossimHistogramRemapper::getLowNormalizedClipPoint(ossim_uint32 zero_based_band) const
{
   if (theNormalizedLowClipPoint.size() == 0 ||
       zero_based_band >= getNumberOfInputBands())
   {
      return ossim::nan();
   }
   
   if (zero_based_band >= theNormalizedLowClipPoint.size())
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHistogramRemapper::getLowNormalizedClipPoint ERROR:"
         << "\nband " << zero_based_band << " is out of range!"
         << "\nhighest band:  " << (theNormalizedLowClipPoint.size()-1)
         << endl;
      return ossim::nan();
   }
	
   return theNormalizedLowClipPoint[zero_based_band];
}

ossim_float64 ossimHistogramRemapper::getLowNormalizedClipPoint() const
{
   if (theNormalizedLowClipPoint.size() == 0)
   {
      return ossim::nan();
   }
   
   ossim_float64 d = 0.0;
   const ossim_uint32 BANDS = (ossim_uint32)theNormalizedLowClipPoint.size();
   for (ossim_uint32 band = 0; band < BANDS; ++band)
   {
      d += getLowNormalizedClipPoint(band);
   }
   
   return (d / BANDS);
}

ossim_float64 ossimHistogramRemapper::getHighNormalizedClipPoint(ossim_uint32 zero_based_band) const
{
   if (theNormalizedHighClipPoint.size() == 0 ||
       zero_based_band >= getNumberOfInputBands())
   {
      return ossim::nan();
   }
   
   if (zero_based_band >= theNormalizedHighClipPoint.size())
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHistogramRemapper::getHighNormalizedClipPoint ERROR:"
         << "\nband " << zero_based_band << " is out of range!"
         << "\nhighest band:  " << (theNormalizedHighClipPoint.size()-1)
         << endl;
      return ossim::nan();
   }
	
   return theNormalizedHighClipPoint[zero_based_band];
}

ossim_float64 ossimHistogramRemapper::getHighNormalizedClipPoint() const
{
   if (theNormalizedHighClipPoint.size() == 0)
   {
      return ossim::nan();
   }
   
   ossim_float64 d = 0.0;
   const ossim_uint32 BANDS = (ossim_uint32)theNormalizedHighClipPoint.size();
   for (ossim_uint32 band = 0; band < BANDS; ++band)
   {
      d += getHighNormalizedClipPoint(band);
   }
   
   return (d / BANDS);
}

ossim_float64 ossimHistogramRemapper::getLowClipPoint(ossim_uint32 zero_based_band) const
{
   if(zero_based_band >= getNumberOfInputBands() ||
      !theHistogram || zero_based_band >= theNormalizedLowClipPoint.size())
   {
      return ossim::nan();
   }
	
   if (theNormalizedLowClipPoint[zero_based_band] == 0.0)
   {
      return getMinPixelValue(zero_based_band);
   }   
	
   ossimRefPtr<ossimHistogram> h = getHistogram(zero_based_band);
   if (h.valid())
   {
      ossim_float64 d =
         h->LowClipVal(theNormalizedLowClipPoint[zero_based_band]);
		
      return floor(d);
   }
	
   return ossim::nan();
}

ossim_float64 ossimHistogramRemapper::getLowClipPoint() const
{
   if (theNormalizedLowClipPoint.size() == 0 || !theHistogram)
   {
      return ossim::nan();
   }
   
   ossim_float64 d = 0.0;
   const ossim_uint32 BANDS = getNumberOfInputBands();
   for (ossim_uint32 band = 0; band < BANDS; ++band)
   {
      d += getLowClipPoint(band);
   }
   
   return (d / BANDS);
}

ossim_float64 ossimHistogramRemapper::getHighClipPoint(ossim_uint32 zero_based_band) const
{
   if(zero_based_band >= getNumberOfInputBands() ||
      !theHistogram || zero_based_band >= theNormalizedHighClipPoint.size())
   {
      return ossim::nan();
   }
	
   if (theNormalizedHighClipPoint[zero_based_band] == 1.0)
   {
      return getMaxPixelValue(zero_based_band);
   }
	
   ossimRefPtr<ossimHistogram> h = getHistogram(zero_based_band);
   if (h.valid())
   {
      ossim_float64 d =
         h->HighClipVal(1.0-theNormalizedHighClipPoint[zero_based_band]);
      return ceil(d);
   }
   
   return ossim::nan();
}

ossim_float64 ossimHistogramRemapper::getHighClipPoint() const
{
   if (theNormalizedHighClipPoint.size() == 0 || !theHistogram)
   {
      return ossim::nan();
   }
   
   ossim_float64 d = 0.0;
   const ossim_uint32 BANDS = getNumberOfInputBands();
   for (ossim_uint32 band = 0; band < BANDS; ++band)
   {
      d += getHighClipPoint(band);
   }
   
   return (d / BANDS);
}

ossim_float64 ossimHistogramRemapper::getMidPoint(ossim_uint32 zero_based_band) const
{
   if (theMidPoint.size() == 0 ||
       zero_based_band >= getNumberOfInputBands())
   {
      return ossim::nan();
   }
   
   if (zero_based_band >= theMidPoint.size())
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHistogramRemapper::getMidPoint ERROR:"
         << "\nband " << zero_based_band << " is out of range!"
         << "\nhighest band:  " << (theMidPoint.size()-1)
         << endl;
      return ossim::nan();
   }
	
   return theMidPoint[zero_based_band];
}

ossim_float64 ossimHistogramRemapper::getMidPoint() const
{
   if (theMidPoint.size() == 0)
   {
      return ossim::nan();
   }
   
   ossim_float64 d = 0.0;
   const ossim_uint32 BANDS = (ossim_uint32)theMidPoint.size();
   for (ossim_uint32 band = 0; band < BANDS; ++band)
   {
      d += getMidPoint(band);
   }
   
   return (d / BANDS);
}

ossim_float64 ossimHistogramRemapper::getMinOutputValue(ossim_uint32 zero_based_band) const
{
   if (theMinOutputValue.size() == 0 ||
       zero_based_band >= getNumberOfInputBands())
   {
      return ossim::nan();
   }
   
   if (zero_based_band >= theMinOutputValue.size())
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHistogramRemapper::getMinOutputValue ERROR:"
         << "\nband " << zero_based_band << " is out of range!"
         << "\nhighest band:  " << (theMinOutputValue.size()-1)
         << endl;
      return ossim::nan();
   }
	
   return theMinOutputValue[zero_based_band];
}

ossim_float64 ossimHistogramRemapper::getMinOutputValue() const
{
   if (theMinOutputValue.size() == 0)
   {
      return ossim::nan();
   }
   
   ossim_float64 d = 0.0;
   const ossim_uint32 BANDS = (ossim_uint32)theMinOutputValue.size();
   for (ossim_uint32 band = 0; band < BANDS; ++band)
   {
      d += getMinOutputValue(band);
   }
   
   return (d / BANDS);
}

ossim_float64 ossimHistogramRemapper::getMaxOutputValue(ossim_uint32 zero_based_band) const
{
   if (theMaxOutputValue.size() == 0 ||
       zero_based_band >= getNumberOfInputBands())
   {
      return ossim::nan();
   }
   
   if (zero_based_band >= theMaxOutputValue.size())
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimHistogramRemapper::getMaxOutputValue ERROR:"
         << "\nband " << zero_based_band << " is out of range!"
         << "\nhighest band:  " << (theMaxOutputValue.size()-1)
         << endl;
      return ossim::nan();
   }
	
   return theMaxOutputValue[zero_based_band];
}

ossim_float64 ossimHistogramRemapper::getMaxOutputValue() const
{
   if (theMaxOutputValue.size() == 0)
   {
      return ossim::nan();
   }
   
   ossim_float64 d = 0.0;
   const ossim_uint32 BANDS = (ossim_uint32)theMaxOutputValue.size();
   for (ossim_uint32 band = 0; band < BANDS; ++band)
   {
      d += getMaxOutputValue(band);
   }
   
   return (d / BANDS); 
}

bool ossimHistogramRemapper::loadState(const ossimKeywordlist& kwl,
                                       const char* prefix)
{
   static const char MODULE[] = "ossimHistogramRemapper::loadState";
   if (traceDebug())
   {
      CLOG << " Entered..."
           << "\nprefix:  " << prefix << endl;
   }
   // Load the base class states...
   bool status = ossimTableRemapper::loadState(kwl, prefix);
   if (status)
   {
      const char* lookup = 0;
      ossimString keyword;
      
      lookup = kwl.find(prefix, HISTOGRAM_FILENAME_KW);
      if (lookup)
      {
         if ( !openHistogram(ossimFilename(lookup)) )
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "ossimHistogramRemapper::loadState ERROR!"
               << "\nCould not open file:  " << lookup
               << "\nReturning..." << endl;
            return false;
         }
      }
     
      //---
      // Get the band specific keywords.
      // NOTES:
      // -  This loadState assumes the all keywords will have the same number
      //    of bands.
      // -  The set methods cannot be used here as there may not be a connection
      //    yet that they need.
      //---
      ossim_uint32 bands = 0;
      lookup = kwl.find(prefix, ossimKeywordNames::NUMBER_BANDS_KW);
      if (lookup)
      {
         bands = ossimString::toUInt32(lookup);
      }
      else  // For backwards compatibility.
      {
         // Use the low clip to find number of bands...
         keyword = NORMALIZED_LOW_CLIP_POINT_KW;
         bands = kwl.numberOf(prefix, keyword);
      }
      //if(!bands&&theHistogram.valid())
     // {
     //    bands = theHistogram->getNumberOfBands();
     // }

      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimHistogramRemapper::loadState DEBUG:"
            << "\nBands:  " << bands
            << endl;
      }
      if (bands)
      {
         // Start with fresh clips.
         initializeClips(bands);
        for (ossim_uint32 band = 0; band < bands; ++band)
         {
            // Get the low clip.
            keyword = NORMALIZED_LOW_CLIP_POINT_KW;
            keyword += ".";
            keyword += ossimKeywordNames::BAND_KW;
            keyword += ossimString::toString(band+1);
            lookup = kwl.find(prefix, keyword);
            if(lookup)
            {
               theNormalizedLowClipPoint[band] = atof(lookup);
            }
            
            // Get the high clip.
            keyword = NORMALIZED_HIGH_CLIP_POINT_KW;
            keyword += ".";
            keyword += ossimKeywordNames::BAND_KW;
            keyword += ossimString::toString(band+1);
            lookup = kwl.find(prefix, keyword);
            if(lookup)
            {
               theNormalizedHighClipPoint[band] = atof(lookup);
            }
            
            // Get the mid point.
            keyword = MID_POINT_KW;
            keyword += ".";
            keyword += ossimKeywordNames::BAND_KW;
            keyword += ossimString::toString(band+1);
            lookup = kwl.find(prefix, keyword);
            if(lookup)
            {
               theMidPoint[band] = atof(lookup);
            }
            
            // Get the min output value.
            keyword = MIN_OUTPUT_VALUE_KW;
            keyword += ".";
            keyword += ossimKeywordNames::BAND_KW;
            keyword += ossimString::toString(band+1);
            lookup = kwl.find(prefix, keyword);
            if(lookup)
            {
               theMinOutputValue[band] = atof(lookup);
            }
            
            // Get the max output value.
            keyword = MAX_OUTPUT_VALUE_KW;
            keyword += ".";
            keyword += ossimKeywordNames::BAND_KW;
            keyword += ossimString::toString(band+1);
            lookup = kwl.find(prefix, keyword);
            if(lookup)
            {
               theMaxOutputValue[band] = atof(lookup);
            }      
         }
     }
     
      lookup = kwl.find(prefix, STRETCH_MODE_KW);
      if (lookup)
      {
         ossimString s = lookup;
         s.downcase();
         if (s == STRETCH_MODE[ossimHistogramRemapper::LINEAR_ONE_PIECE])
         {
            theStretchMode = ossimHistogramRemapper::LINEAR_ONE_PIECE;
         }
         else if (s == STRETCH_MODE[ossimHistogramRemapper::LINEAR_1STD_FROM_MEAN])
         {
            theStretchMode = ossimHistogramRemapper::LINEAR_1STD_FROM_MEAN;
         }
         else if (s == STRETCH_MODE[ossimHistogramRemapper::LINEAR_2STD_FROM_MEAN])
         {
            theStretchMode = ossimHistogramRemapper::LINEAR_2STD_FROM_MEAN;
         }
         else if (s == STRETCH_MODE[ossimHistogramRemapper::LINEAR_3STD_FROM_MEAN])
         {
            theStretchMode = ossimHistogramRemapper::LINEAR_3STD_FROM_MEAN;
         }
         else if (s == STRETCH_MODE[ossimHistogramRemapper::LINEAR_AUTO_MIN_MAX])
         {
            theStretchMode = ossimHistogramRemapper::LINEAR_AUTO_MIN_MAX;
         }
         else
         {
            theStretchMode = ossimHistogramRemapper::STRETCH_UNKNOWN;
         }
      }

      // Always set the dirty flag.
      theDirtyFlag = true;
   }
   verifyEnabled();

   if (traceDebug())
   {
      CLOG << "ossimHistogramRemapper::loadState DEBUG:"
           << *this
           << "\nExited..." << endl;
   }
   return status;
}

bool ossimHistogramRemapper::saveState(ossimKeywordlist& kwl,
                                       const char* prefix) const
{
   if (theHistogram.valid())
   {
      kwl.add(prefix,
              HISTOGRAM_FILENAME_KW,
              theHistogram->getHistogramFile().c_str(),
              true);
   }
   kwl.add(prefix,
           STRETCH_MODE_KW,
           getStretchModeString().c_str(),
           true);
	
   const ossim_uint32 BANDS = (ossim_uint32)theNormalizedLowClipPoint.size();
   
   kwl.add(prefix,
           ossimKeywordNames::NUMBER_BANDS_KW,
           BANDS,
           true);
	
   for (ossim_uint32 band = 0; band < BANDS; ++band)
   {
      ossimString keyword;
		
      // Save the low clip.
      keyword = NORMALIZED_LOW_CLIP_POINT_KW;
      keyword += ".";
      keyword += ossimKeywordNames::BAND_KW;
      keyword += ossimString::toString(band+1);
      kwl.add(prefix,
              keyword,
              theNormalizedLowClipPoint[band],
              true);
		
      // Save the high clip.
      keyword = NORMALIZED_HIGH_CLIP_POINT_KW;
      keyword += ".";
      keyword += ossimKeywordNames::BAND_KW;
      keyword += ossimString::toString(band+1);
      kwl.add(prefix,
              keyword,
              theNormalizedHighClipPoint[band],
              true);
		
      // Save the mid point.
      keyword = MID_POINT_KW;
      keyword += ".";
      keyword += ossimKeywordNames::BAND_KW;
      keyword += ossimString::toString(band+1);
      kwl.add(prefix,
              keyword,
              theMidPoint[band],
              true);
		
      // Save the min output value.

      keyword = MIN_OUTPUT_VALUE_KW;
      keyword += ".";
      keyword += ossimKeywordNames::BAND_KW;
      keyword += ossimString::toString(band+1);
      kwl.add(prefix,
              keyword,
              theMinOutputValue[band],
              true);
		
      // Save the max output value.
      keyword = MAX_OUTPUT_VALUE_KW;
      keyword += ".";
      keyword += ossimKeywordNames::BAND_KW;
      keyword += ossimString::toString(band+1);

      kwl.add(prefix,
              keyword,
              theMaxOutputValue[band],
              true);
   }

   return ossimTableRemapper::saveState(kwl, prefix);
}

void ossimHistogramRemapper::setStretchMode(StretchMode mode,
                                            bool rebuildTable)
{
   if (theStretchMode != mode)
   {
      theStretchMode = mode;

      if (rebuildTable)
      {
         buildTable();

         // Check for internal bypass.
         verifyEnabled();

         // Clear the dirty flag.
         theDirtyFlag = false;
      }
      else
      {
         theDirtyFlag = true;
      }
   }
}

void ossimHistogramRemapper::setStretchModeAsString(const ossimString& mode,
                                                    bool rebuildTable)
{
   if( mode == "linear_one_piece")
   {
      setStretchMode(LINEAR_ONE_PIECE, rebuildTable);
   }
   else if(mode == "linear_1std_from_mean")
   {
      setStretchMode(LINEAR_1STD_FROM_MEAN, rebuildTable);
   }
   else if(mode == "linear_2std_from_mean")
   {
      setStretchMode(LINEAR_2STD_FROM_MEAN, rebuildTable);
   }
   else if(mode == "linear_3std_from_mean")
   {
      setStretchMode(LINEAR_3STD_FROM_MEAN, rebuildTable);
   }
   else if(mode == "linear_auto_min_max")
   {
      setStretchMode(LINEAR_AUTO_MIN_MAX, rebuildTable);
   }
   else
   {
      setStretchMode(STRETCH_UNKNOWN, false);
   }   
}

void ossimHistogramRemapper::buildTable()
{
   setupTable();
   switch(theStretchMode)
   {
      case ossimHistogramRemapper::LINEAR_ONE_PIECE:
      {
         buildLinearTable();
         break;
      }
      case ossimHistogramRemapper::LINEAR_AUTO_MIN_MAX:
      {
         buildAutoLinearMinMaxTable();
         break;
      }
      case ossimHistogramRemapper::LINEAR_1STD_FROM_MEAN:
      case ossimHistogramRemapper::LINEAR_2STD_FROM_MEAN:
      case ossimHistogramRemapper::LINEAR_3STD_FROM_MEAN:
         buildLinearTableStdFromMean();
         break;
         
      default:
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "ossimHistogramRemapper::buildTable ERROR!"
               << "\nUnknown stretch type!" << endl;
         }
         break;
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimHistogramRemapper::buildTable DEBUG:\n" << endl;
      print(ossimNotify(ossimNotifyLevel_DEBUG));
   }
}

void ossimHistogramRemapper::buildLinearTable()
{
   switch (getOutputScalarType())
   {
      case OSSIM_UINT8:
      {
         buildLinearTable(ossim_uint8(0));
         break;
      }
			
      case OSSIM_USHORT11:
      case OSSIM_UINT16:
      {
         buildLinearTable(ossim_uint16(0));
         break;
      }
			
      case OSSIM_SINT16:
      {
         buildLinearTable(ossim_sint16(0));
         break;
      }

      case OSSIM_UINT32:
      {
         buildLinearTable(ossim_uint32(0));
         break;
      }
			
      case OSSIM_SINT32:
      {
         buildLinearTable(ossim_sint32(0));
         break;
      }
			
      case OSSIM_NORMALIZED_FLOAT:      
      case OSSIM_FLOAT:
      {
         buildLinearTable(ossim_float32(0));
         break;
      }
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_DOUBLE:
      {
         buildLinearTable(ossim_float64(0));
         break;
      }
			
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         if(traceDebug())
         {
            // Shouldn't hit this.
            ossimNotify(ossimNotifyLevel_WARN)
               << "ossimHistogramRemapper::buildLinearTable OSSIM_SCALAR_UNKNOWN!" << endl;
         }
         break;
      }
			
   } // End of "switch (theTableType)"
}

void ossimHistogramRemapper::buildAutoLinearMinMaxTable()
{
   switch (getOutputScalarType())
   {
      case OSSIM_UINT8:
      {
         buildAutoLinearMinMaxTableTemplate(ossim_uint8(0));
         break;
      }
			
      case OSSIM_USHORT11:
      case OSSIM_UINT16:
      {
         buildAutoLinearMinMaxTableTemplate(ossim_uint16(0));
         break;
      }
			
      case OSSIM_SINT16:
      {
         buildAutoLinearMinMaxTableTemplate(ossim_sint16(0));
         break;
      }
			
      case OSSIM_NORMALIZED_FLOAT:      
      case OSSIM_FLOAT:
      {
         buildAutoLinearMinMaxTableTemplate(ossim_float32(0));
         break;
      }
			
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_DOUBLE:
      {
         buildAutoLinearMinMaxTableTemplate(ossim_float64(0));
         break;
      }
			
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         if(traceDebug())
         {
         // Shouldn't hit this.
            ossimNotify(ossimNotifyLevel_WARN)
               << "ossimTableRemapper::buildTable OSSIM_SCALAR_UNKNOWN!"
               << endl;
         }
         break;
      }
			
   } // End of "switch (theTableType)"
}

template <class T> void ossimHistogramRemapper::buildLinearTable(T /* dummy */)
{
   // This builds a native table.
   theTableType = ossimTableRemapper::NATIVE;
   
   const ossim_uint32 BANDS = getNumberOfInputBands();

   // Sanity check.
   if (theNormalizedLowClipPoint.size() != BANDS || !getHistogram(0).valid())
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimHistogramRemapper::buildTable ERROR!"
            << " Line:  " << __LINE__ << endl;
      }
      return;
   }
   
   T* table = reinterpret_cast<T*>(&theTable.front());
   ossim_uint32 index = 0;
   
   // Finally, build the table...
   for (ossim_uint32 band = 0; band < BANDS; ++band)
   {
      ossimRefPtr<ossimHistogram> h  = getHistogram(band);
      if (h.get())
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "ossimHistogramRemapper::buildLinearTable DEBUG:"
               << "\nband:  " << band
               << "\nmean:  " << h->GetMean()
               << "\nstddev:  " << h->GetStandardDev()
               << endl;
         }
      }
      else
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
            << "ossimHistogramRemapper::buildTable ERROR!"
            << " Line:  " << __LINE__ << endl;
         }
         return; 
      }
		//std::cout << "MIN OUTPUT ==== " << theMinOutputValue[band] << std::endl;
      const T NULL_PIX = static_cast<T>(getNullPixelValue(band));
      const T MIN_PIX  = static_cast<T>(theMinOutputValue[band]);
      const T MAX_PIX  = static_cast<T>(theMaxOutputValue[band]);

      ossim_float64 min_clip_value =
         h->LowClipVal(theNormalizedLowClipPoint[band]);
      ossim_float64 max_clip_value =
         h->HighClipVal(1.0-theNormalizedHighClipPoint[band]);
      min_clip_value = floor(min_clip_value);
      max_clip_value = ceil(max_clip_value);
      ossim_float64 gain = (MAX_PIX-MIN_PIX+1)/(max_clip_value-min_clip_value);
      
      table[index] = NULL_PIX;
      ++index;

      for (ossim_uint32 pix = 1; pix < theTableBinCount; ++pix)
      {
         ossim_float64 p = pix;
         if (p <= min_clip_value)
         {
            p = MIN_PIX;
         }
         else if (p >= max_clip_value)
         {
            p = MAX_PIX;
         }
         else
         {
            p = ((p - min_clip_value) * gain) + MIN_PIX - 1.0;
         }

         if(p == NULL_PIX)
         {
            p = MIN_PIX;
         }
         table[index] = static_cast<T>(p+0.5);
			
         ++index;
      }
   } // End of band loop.

   // Clear the dirty flag so the table's not rebuilt on the next getTile.
   theDirtyFlag = false;
}

template <class T> void ossimHistogramRemapper::buildAutoLinearMinMaxTableTemplate(T /* dummy */)
{
   const ossim_uint32 BANDS = getNumberOfInputBands();

   // Sanity check.
   if (theNormalizedLowClipPoint.size() != BANDS || !getHistogram(0).valid())
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimHistogramRemapper::buildAutoLinearMinMaxTableTemplate ERROR!"
            << " Line:  " << __LINE__ << endl;
      }
      return;
   }
   for (ossim_uint32 band = 0; band < BANDS; ++band)
   {
      ossimRefPtr<ossimHistogram> h      = getHistogram(band);
      T nullPix = static_cast<T>(getNullPixelValue(band));
      if(h.valid())
      {
         ossim_uint32 n     = h->GetRes();
         ossim_float64 low  = h->GetMinVal();
         ossim_float64 high = h->GetMaxVal();

 // std::cout << "LOW ========= " << low << std::endl;
  //std::cout << "HIGH ========= " << high << std::endl;

         double percentChange = .006;
         if(n > 0)
         {
            double newCount       = 0.0;
            double nextPercentage = 0.0;
            double percentage = 0.0;
            int idx = 0;
            float * counts = h->GetCounts();
            double count   = h->ComputeArea();
				
            for(idx = 0; idx < (ossim_int32)(n-1); ++idx)
            {
               if(nullPix != idx)
               {
                  newCount += counts[idx];
               }
               percentage = newCount / count;
               nextPercentage =
                  (newCount + counts[idx+1]) / count;
//               if (std::fabs(percentage - 0.006) <
//                   std::fabs(nextPercentage - 0.006))
                  if (std::fabs(percentage - percentChange) <
                      std::fabs(nextPercentage - percentChange))
               {
                  low = idx+1;
                  break;
               }
               
            }
            newCount = 0.0;
            for (idx = n-1; idx > 0; idx--)
            {
               newCount += counts[idx];
               percentage = newCount / count;
               nextPercentage =
                  (newCount + counts[idx-1]) / count;
               if (std::fabs(percentage - percentChange) <
                   std::fabs(nextPercentage - percentChange))
               {
                  high=idx-1;
                  break;
               }
            }
            if(low > high)
            {
               low = 0;
               high = n - 1;
            }
            //theMinOutputValue[band] = 1;
            //theMaxOutputValue[band] = 65535;

            //std::cout << "LOW ===== " << low << std::endl;
            //std::cout << "HIGH ===== " << high << std::endl;
            setLowClipPoint(low, band);
            setHighClipPoint(high, band);
         }
      }
   }
   
   buildLinearTable();
}

void ossimHistogramRemapper::buildLinearTableStdFromMean()
{
   const ossim_uint32 BANDS = getNumberOfInputBands();
	
   // Sanity check.
   if (theNormalizedLowClipPoint.size() != BANDS || !getHistogram(0).valid())
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimHistogramRemapper::buildTable ERROR!"
            << " Line:  " << __LINE__ << endl;
      }
      return;
   }
   
   ossim_float64 multiplier = 1.0;
   if (theStretchMode == LINEAR_2STD_FROM_MEAN)
   {
      multiplier =  2.0;
   }
   else if (theStretchMode == LINEAR_3STD_FROM_MEAN)
   {
      multiplier =  3.0;
   }
	
   // Finally, build the table...
   for (ossim_uint32 band = 0; band < BANDS; ++band)
   {
      ossimRefPtr<ossimHistogram> h      = getHistogram(band);
      ossim_float64 mean = 0.0;
      ossim_float64 stdDev = 0.0;
      if(h.valid())
      {
         mean     = h->GetMean();
         stdDev   = h->GetStandardDev();
      }
      ossim_float64 lowClip  = mean - (stdDev * multiplier);
      ossim_float64 highClip = mean + (stdDev * multiplier);
		
      // Clamp to min/max.
      if (lowClip < theMinOutputValue[band])
      {
         lowClip = theMinOutputValue[band];
      }
      if (highClip > theMaxOutputValue[band])
      {
         highClip = theMaxOutputValue[band];
      }
      
      setLowClipPoint(lowClip, band);
      setMidPoint(mean, band);
      setHighClipPoint(highClip, band);
      
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimHistogramRemapper::buildLinearStdFromMean DEBUG:"
            << "\nband:       " << band
            << "\nmean:       " << mean
            << "\nstddev:     " << stdDev
            << "\nlow clip:   " << lowClip
            << "\nhigh clip:  " << highClip
            << endl;
      }
   }
   
   buildLinearTable();
}

void ossimHistogramRemapper::initializeClips()
{
   initializeClips(getNumberOfInputBands());
}

void ossimHistogramRemapper::initializeClips(ossim_uint32 bands)
{
   //---
   // NOTE: This method deoes not set theDirtyFlag by design.
   //---
   if (bands)
   {
      theNormalizedLowClipPoint.resize(bands);
      theNormalizedHighClipPoint.resize(bands);
      theMidPoint.resize(bands);
      theMinOutputValue.resize(bands);
      theMaxOutputValue.resize(bands);
      
      for (ossim_uint32 band = 0; band < bands; ++band)
      {
         theNormalizedLowClipPoint[band]  = 0.0;
         theNormalizedHighClipPoint[band] = 1.0;
         theMidPoint[band] = 0.0;
         
         // Must have an output scalar type for getMin/Max call.
         if (theOutputScalarType != OSSIM_SCALAR_UNKNOWN)
         {
            theMinOutputValue[band] = ossim::defaultMin(theOutputScalarType);//getMinPixelValue(band);
            theMaxOutputValue[band] = ossim::defaultMax(theOutputScalarType);//getMaxPixelValue(band);
         }
         else
         {
            theMinOutputValue[band] = 0.0;
            theMaxOutputValue[band] = 0.0;
         }
      }
   }
}

void ossimHistogramRemapper::initializeMinMaxOutput()
{
   ossim_uint32 nBands = getNumberOfInputBands();
   theMinOutputValue.resize(nBands);
   theMaxOutputValue.resize(nBands);
   for (ossim_uint32 band = 0; band < nBands; ++band)
   {
         // Must have an output scalar type for getMin/Max call.
         if (theOutputScalarType != OSSIM_SCALAR_UNKNOWN)
         {
            theMinOutputValue[band] = ossim::defaultMin(theOutputScalarType);//getMinPixelValue(band);
            theMaxOutputValue[band] = ossim::defaultMax(theOutputScalarType);//getMaxPixelValue(band);
         }
         else
         {
            theMinOutputValue[band] = 0.0;
            theMaxOutputValue[band] = 0.0;
         }
  }

}

void ossimHistogramRemapper::setNullCount()
{
   // This will set the null bucket for all res zero bands to zero!
   if (getHistogram(0).valid())
   {
      const ossim_uint32 BANDS = theHistogram->getNumberOfBands();
      for (ossim_uint32 band = 0; band < BANDS; ++band)
      {
         ossimRefPtr<ossimHistogram> h = theHistogram->getHistogram(band);
         if (h.valid())
         {
            h->SetCount(0, 0);
         }
      }
   }
}

ossimHistogramRemapper::StretchMode ossimHistogramRemapper::getStretchMode() const
{
   return theStretchMode;
}

ossimString ossimHistogramRemapper::getStretchModeString() const
{
   return ossimString(STRETCH_MODE[theStretchMode]);
}

ossim_uint32 ossimHistogramRemapper::getNumberOfStretchModes() const
{
   return ossimHistogramRemapper::STRETCH_UNKNOWN; // last index
}

ossimString
ossimHistogramRemapper::getStretchModeString(ossim_uint32 index) const
{
   if (index < ossimHistogramRemapper::STRETCH_UNKNOWN)
   {
      return ossimString(STRETCH_MODE[index]);
   }
	
   return ossimString(STRETCH_MODE[ossimHistogramRemapper::STRETCH_UNKNOWN]);
   
}

ostream& ossimHistogramRemapper::print(ostream& os) const
{
   os << setiosflags(ios::fixed) << setprecision(15)
      << "\nossimHistogramRemapper::print:"
      << "\ntheStretchMode:         " << getStretchModeString().c_str()
      << "\ntheDirtyFlag:           " << (theDirtyFlag?"true":"false")
      << "\ntheBypassFlag:          " << (theBypassFlag?"true":"false")
      
      << "\ntheHistogram:           " << (theHistogram.valid()?"initialized":"null")
      << "\ntheTableSizeInBytes:    " << theTable.size()
      << endl;
	
   const ossim_uint32 BANDS = (ossim_uint32)theNormalizedLowClipPoint.size();
   for (ossim_uint32 band = 0; band < BANDS; ++band)
   {
      os << "\ntheNormalizedLowClipPoint[" << band << "]:   "
         << theNormalizedLowClipPoint[band]
         << "\ntheNormalizedHighClipPoint[" << band << "]:  "
         << theNormalizedHighClipPoint[band]
         << "\ntheMidPoint[" << band << "]:                 "
         << theMidPoint[band]
         << "\ntheMinOutputValue[" << band << "]:           "
         << theMinOutputValue[band]
         << "\ntheMaxOutputValue[" << band << "]:           "
         << theMaxOutputValue[band]
         << endl;
   }
	
   for (ossim_uint32 i = 0; i < theBandList.size(); ++i)
   {
      os << "theBandList[" << i << "]:  " << theBandList[i] << endl;
   }

   // Note: To set table call ossimTableRemapper::print or turn trace on
   // for "ossimTableRemapper:debug"
   
   return os;
}

void ossimHistogramRemapper::setupTable()
{
   const ossim_uint32 BANDS = getNumberOfInputBands();
   
   if (theNormalizedLowClipPoint.size() == 0)
   {
      initializeClips(BANDS);
   }
   
   ossim_uint32 values_per_band = 0;
   ossim_uint32 bytes_per_pixel = 0;
   
   switch (theOutputScalarType)
   {
      case OSSIM_UINT8:
         values_per_band = 256;  // 2 ^ 8
         bytes_per_pixel = 1;
         theTableType = ossimTableRemapper::NATIVE;
         break;
         
      case OSSIM_USHORT11:
         values_per_band = 2048; // 2 ^ 11
         bytes_per_pixel = 2;
         theTableType = ossimTableRemapper::NATIVE;
         break;
         
      case OSSIM_UINT16:
      case OSSIM_SINT16:
         values_per_band = 65536; // 2 ^ 16
         bytes_per_pixel = 2;
         theTableType = ossimTableRemapper::NATIVE;
         break;
         
      case OSSIM_UINT32:
      case OSSIM_SINT32:
         values_per_band = 65536; // 2 ^ 16
         bytes_per_pixel = 4;
         theTableType = ossimTableRemapper::NATIVE;
         break;
			
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT:
         bytes_per_pixel = 4;
         break;
			
      case OSSIM_NORMALIZED_DOUBLE:         
      case OSSIM_DOUBLE:
         bytes_per_pixel = 8;
         theTableType = ossimTableRemapper::NORMALIZED;
         break;
         
      default:
         break;
   }
	
   if ( theOutputScalarType == OSSIM_FLOAT ||
        theOutputScalarType == OSSIM_DOUBLE ||
        theOutputScalarType == OSSIM_NORMALIZED_FLOAT ||
        theOutputScalarType == OSSIM_NORMALIZED_DOUBLE )
   {
      for (ossim_uint32 band = 0; band < BANDS; ++band)
      {
         ossimRefPtr<ossimHistogram> h  = getHistogram(band);
			
         if (h.valid())
         {
            if (h->GetRes() > static_cast<ossim_int32>(values_per_band))
            {
               values_per_band = h->GetRes();
            }
         }
      }
   }      
   
   theTableBinCount  = values_per_band;
   theTableBandCount = BANDS;
   // Check the size of the table prior to deletion and making a new one.
   ossim_uint32 size_in_bytes = values_per_band * BANDS * bytes_per_pixel;
   if(theTable.empty() || (theTable.size() != size_in_bytes))
   {
      theTable.resize(size_in_bytes);
   }

   ossimImageSource* input = dynamic_cast<ossimImageSource*>(getInput());
   double minPix = ossim::defaultMin(getOutputScalarType());
   double maxPix = ossim::defaultMax(getOutputScalarType());
   
   if(input)
   {
      //---
      // Last check for NaNs in key data members and set to some default if so.
      // This could occur if someone stripped a keyword list down to a minimal
      // set of keywords.
      //---
      for (ossim_uint32 band = 0; band < BANDS; ++band)
      {
         minPix = input->getMinPixelValue(band);
         maxPix = input->getMaxPixelValue(band);
         if ( ossim::isnan(theMinOutputValue[band]) )
         {
            theMinOutputValue[band] = minPix;
         }
         if ( ossim::isnan(theMaxOutputValue[band]) )
         {
            theMaxOutputValue[band] = maxPix;
         }
      }   
   }
   else
   {
      //---
      // Last check for NaNs in key data members and set to some default if so.
      // This could occur if someone stripped a keyword list down to a minimal
      // set of keywords.
      //---
      for (ossim_uint32 band = 0; band < BANDS; ++band)
      {
         if ( ossim::isnan(theMinOutputValue[band]) )
         {
            theMinOutputValue[band] = minPix;
         }
         if ( ossim::isnan(theMaxOutputValue[band]) )
         {
            theMaxOutputValue[band] = maxPix;
         }
      }   
   }
}

void ossimHistogramRemapper::verifyEnabled()
{
   //---
   // Since this filter can be constructed with no input connection do not
   // output and error, simply return.
   //---	
      setBypassFlag(true);
   //if (theInputConnection)
   {

#if 1     
      // Start off bypassed.

      if (theStretchMode != STRETCH_UNKNOWN)
      {
         if(theHistogram.valid())
         {
            setBypassFlag(false);
         }
/*         
         const ossim_uint32 BANDS =
            (ossim_uint32)theNormalizedLowClipPoint.size();
         for (ossim_uint32 band = 0; band < BANDS; ++band)
         {
            //const double MIN = ossimTableRemapper::getMinPixelValue(band);
            //const double MAX = ossimTableRemapper::getMaxPixelValue(band);
            if ( theNormalizedLowClipPoint[band]  != 0.0   ||
                 theNormalizedHighClipPoint[band] != 1.0 )
                // theMinOutputValue[band] != MIN ||
                 //theMaxOutputValue[band] != MAX )
            {
               // Need to turn filter on.
               setBypassFlag(false);
               break;
            }
         }
         */
      }
#endif
   }
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimHistogramRemapper::verifyEnabled theBypassFlag: "
         << (theBypassFlag ? "true" : "false") << endl;
   }
}

ossimRefPtr<ossimMultiResLevelHistogram> ossimHistogramRemapper::getHistogram()
{
   return ossimRefPtr<ossimMultiResLevelHistogram>(theHistogram.get());
}

ossimRefPtr<const ossimMultiResLevelHistogram> ossimHistogramRemapper::getHistogram()const
{
   return ossimRefPtr<const ossimMultiResLevelHistogram>(theHistogram.get());
}

ossimRefPtr<ossimHistogram>
ossimHistogramRemapper::getHistogram(ossim_uint32 zero_based_band,
                                     ossim_uint32 res_level) const
{
   ossim_uint32 histogram_band = getHistogramBand(zero_based_band);
   
   if(!theHistogram.valid())
   {
      const ossimHistogramSource* source = dynamic_cast<const ossimHistogramSource*>(getInput(1));
      if(source)
      {
         theHistogram = const_cast<ossimHistogramSource*>(source)->getHistogram();
      }      
   }
   
   if (theHistogram.valid())
   {
      if (histogram_band < theHistogram->getNumberOfBands())
      {
         return theHistogram->getHistogram(histogram_band, res_level);
      }
   }
	
   return 0;
}

ossimFilename ossimHistogramRemapper::getHistogramFile() const
{
   if (theHistogram.valid())
   {
      return theHistogram->getHistogramFile();
   }
	
   return ossimFilename::NIL;
}

ossim_uint32
ossimHistogramRemapper::getHistogramBand(ossim_uint32 input_band) const
{
   if(theResetBandIndicesFlag) return input_band;
   vector<ossim_uint32> bandList;
   getOutputBandList(bandList);
   
   const ossim_uint32 BANDS = (ossim_uint32)bandList.size();
	
   if (BANDS == 0)
   {
      return 0;
   }
	
   return bandList[input_band];
}

ossimString ossimHistogramRemapper::getLongName()const
{
   return ossimString("Histogram Remapper, filter with controls for histogram stretching.");
}

ossimString ossimHistogramRemapper::getShortName()const
{
   return ossimString("Histogram Remapper");
}

ostream& operator<<(ostream& os, const ossimHistogramRemapper& hr)
{
   return hr.print(os);
}


// Private to disallow use...
ossimHistogramRemapper::ossimHistogramRemapper(const ossimHistogramRemapper&)
{
}

// Private to disallow use...
ossimHistogramRemapper& ossimHistogramRemapper::operator=(const ossimHistogramRemapper&)
{
   return *this;
}

void ossimHistogramRemapper::setBypassFlag(bool flag)
{
   if (theBypassFlag != flag)
   {
      //---
      // If switching from bypassed to enabled set
      // the dirty flag.  We don't care about going the other way.
      //---
      if ( theBypassFlag && !flag )
      {
         theDirtyFlag = true;
      }
      theBypassFlag = flag;
   }
}

double ossimHistogramRemapper::getMinPixelValue(ossim_uint32 band)const
{
   double result = ossimTableRemapper::getMinPixelValue(band);
   if(theEnableFlag&&!theBypassFlag &&(band < theMinOutputValue.size()))
   {
      result = theMinOutputValue[band];
   }
  // if(theOutputScalarType != OSSIM_SCALAR_UNKNOWN)
  // {
  //    return ossim::defaultMin(theOutputScalarType);
  // }

   return result;
}

double ossimHistogramRemapper::getMaxPixelValue(ossim_uint32 band)const
{
   double result = ossimTableRemapper::getMaxPixelValue(band);

   if(theEnableFlag&&!theBypassFlag &&(band < theMaxOutputValue.size()))
   {
      result = theMaxOutputValue[band];
   }
   //if(theOutputScalarType != OSSIM_SCALAR_UNKNOWN)
  // {
  //    result = ossim::defaultMax(theOutputScalarType);
  // }

   return result;
}

bool ossimHistogramRemapper::canConnectMyInputTo(ossim_int32 inputIndex,
                                                 const ossimConnectableObject* object)const
{
   // we will alloe the histogram source only to be connected to input slot 2 and
   // the first slot will be reserved for the ossimImageSource.
   //
   bool result = false;
   if(inputIndex == 0) result = dynamic_cast<const ossimImageSource*>(object)!=0;
   if(inputIndex == 1) result = dynamic_cast<const ossimHistogramSource*>(object)!=0;
   
   return result;
}

