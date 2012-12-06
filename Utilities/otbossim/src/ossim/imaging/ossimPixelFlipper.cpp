//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:
//
// Filter to toggle pixel values.
//
//*************************************************************************
// $Id: ossimPixelFlipper.cpp 21631 2012-09-06 18:10:55Z dburken $


#include <ossim/imaging/ossimPixelFlipper.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/base/ossimNumericProperty.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/imaging/ossimImageData.h>
#include <OpenThreads/ScopedLock>
#include <cstdlib>

RTTI_DEF1(ossimPixelFlipper, "ossimPixelFlipper", ossimImageSourceFilter)

static ossimTrace traceDebug("ossimPixelFlipper:debug");

const char ossimPixelFlipper::PF_TARGET_VALUE_KW[]      = "target_value";
const char ossimPixelFlipper::PF_TARGET_RANGE_KW[]      = "target_range";
const char ossimPixelFlipper::PF_REPLACEMENT_VALUE_KW[] = "replacement_value";
const char ossimPixelFlipper::PF_REPLACEMENT_MODE_KW[]  = "replacement_mode";
const char ossimPixelFlipper::PF_CLAMP_VALUE_KW[]       = "clamp_value"; // deprecated by clamp_value_hi
const char ossimPixelFlipper::PF_CLAMP_VALUE_LO_KW[]    = "clamp_value_lo";
const char ossimPixelFlipper::PF_CLAMP_VALUE_HI_KW[]    = "clamp_value_hi";
const char ossimPixelFlipper::PF_CLIP_MODE_KW[]         = "border_clip_mode";

static const char TARGET_LOWER_LIMIT_PROP_NAME[] = "target_range_lower_limit";
static const char TARGET_UPPER_LIMIT_PROP_NAME[] = "target_range_upper_limit";

#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimPixelFlipper.cpp 21631 2012-09-06 18:10:55Z dburken $";
#endif

ossimPixelFlipper::ossimPixelFlipper(ossimObject* owner)
   :
      ossimImageSourceFilter(owner),
      theTargetValueLo(0.0),
      theTargetValueHi(0.0),
      theReplacementValue(1.0),
      theReplacementMode(ossimPixelFlipper::REPLACE_BAND_IF_TARGET),
      theClampValueLo(ossim::nan()),
      theClampValueHi(ossim::nan()),
      theClampingMode(DISABLED),
      theClipMode(NONE)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimPixelFlipper::ossimPixelFlipper entered...\n";
#ifdef OSSIM_ID_ENABLED
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "OSSIM_ID:  " << OSSIM_ID << "\n";
#endif      
   }
   
   setDescription(ossimString("Pixel Flipper"));
   enableSource();
}

ossimPixelFlipper::~ossimPixelFlipper()
{}

ossimRefPtr<ossimImageData> ossimPixelFlipper::getTile(
   const ossimIrect& tile_rect, ossim_uint32 resLevel)
{

   if(!theInputConnection)
   {
      return 0;
   }

   // Fetch tile from pointer from the input source.
   ossimRefPtr<ossimImageData> inputTile =
      theInputConnection->getTile(tile_rect, resLevel);
   
   if (!inputTile.valid() || !isSourceEnabled()) return inputTile;
   
   if (!inputTile->getBuf()) return inputTile;
   
   // Lock for the length of this method.
	OpenThreads::ScopedLock<OpenThreads::Mutex> scopeLock(theMutex);
   
   // Call the appropriate load method.
   switch (inputTile->getScalarType())
   {
      
      case OSSIM_UCHAR:
      {
         flipPixels(ossim_uint8(0), inputTile.get(), resLevel);
         break;
      }
      
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
      {
         flipPixels(ossim_uint16(0), inputTile.get(), resLevel);
         break;
      }
      
      case OSSIM_SSHORT16:
      {
         flipPixels(ossim_sint16(0), inputTile.get(), resLevel);
         break;
      }
      case OSSIM_UINT32:
      {
         flipPixels(ossim_uint32(0), inputTile.get(), resLevel);
         break;
      }
      case OSSIM_SINT32:
      {
         flipPixels(ossim_sint32(0), inputTile.get(), resLevel);
         break;
      }
      case OSSIM_FLOAT32:
      case OSSIM_NORMALIZED_FLOAT:
      {
         flipPixels(float(0), inputTile.get(), resLevel);
         break;
      }
      
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
      {
         flipPixels(ossim_float64(0), inputTile.get(), resLevel);
         break;
      }
      
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimPixelFlipper::getTile Unsupported scalar type!" << endl;
         break;
      }
   }
   
   inputTile->validate();
   return inputTile;
}

template <class T>
void ossimPixelFlipper::flipPixels(T /* dummy */,
                                   ossimImageData* inputTile,
                                   ossim_uint32 resLevel)
{
   if (!inputTile) return;

   T targetLo    = static_cast<T>(theTargetValueLo);
   T targetHi    = static_cast<T>(theTargetValueHi);
   T replacement = static_cast<T>(theReplacementValue);
   T clampLo       = static_cast<T>(theClampValueLo);
   T clampHi       = static_cast<T>(theClampValueHi);

   // Get pointers to data for each band.
   ossim_uint32 bands = inputTile->getNumberOfBands();
   ossim_uint32 band;
   T** buf = new T*[bands];
   for(band=0; band<bands; ++band)
      buf[band] = static_cast<T*>(inputTile->getBuf(band));

   ossimIrect rect = inputTile->getImageRectangle();
   ossimIpt ul = rect.ul();
   ossimIpt lr = rect.lr();

   // Check the relation between tile rect and the area of interest (either bounding rect or
   // valid vertices polygons). If completely outside, we can return with empty buffer.get If
   // partially inside, we'll need to test individual pixels.
   bool is_outside_aoi = false;
   bool needsTesting = false;
   if ((theClipMode == BOUNDING_RECT) && (resLevel < theBoundingRects.size()))
   {
      if(!rect.intersects(theBoundingRects[resLevel]))
         is_outside_aoi = true;
      else
         needsTesting = !rect.completely_within(theBoundingRects[resLevel]);
   }
   else if ((theClipMode == VALID_VERTICES) && (resLevel < theValidVertices.size()))
   {
      bool ulFlag = theValidVertices[resLevel].isPointWithin(rect.ul());
      bool urFlag = theValidVertices[resLevel].isPointWithin(rect.ur());
      bool lrFlag = theValidVertices[resLevel].isPointWithin(rect.lr());
      bool llFlag = theValidVertices[resLevel].isPointWithin(rect.ll());
      if((!ulFlag) && (!urFlag) && (!lrFlag) && (!llFlag))
         is_outside_aoi = true;
      else
         needsTesting = !(ulFlag && urFlag && lrFlag && llFlag);
   }
   if (is_outside_aoi)
   {
      // none of the tile is inside so just return with empty tile:
      delete [] buf;
      return; 
   }

   ossim_uint32 i = 0;  // index into band buffers;
   ossimIpt pixel_loc; 
   bool can_replace, found_candidate;

   // Begin loop over each pixel in the tile. The individual bands are handled inside this loop:
   for(pixel_loc.y = ul.y; pixel_loc.y <= lr.y; ++pixel_loc.y)
   {
      for(pixel_loc.x = ul.x; pixel_loc.x <= lr.x; ++pixel_loc.x)
      {
         // First consider if we need to test the pixel for border clipping:
         if (needsTesting)
         {
            bool is_inside = true; // Assume it will pass the border test
            if (theClipMode == BOUNDING_RECT)
               is_inside = theBoundingRects[resLevel].pointWithin(pixel_loc);
            else if (theClipMode == VALID_VERTICES)
               is_inside = theValidVertices[resLevel].isPointWithin(pixel_loc);

            if (!is_inside)
            {
               // Remap this pixel to the replacement value (all bands)
               for (band=0; band<bands; ++band)
                  buf[band][i] = replacement;

               // Proceed to next pixel location:
               ++i;
               continue; 
            }
         }

         // If clamping specified, the target replacement function is disabled:
         if (theClampingMode)
         {
            switch (theReplacementMode)
            {
            case REPLACE_BAND_IF_TARGET:
            case REPLACE_ALL_BANDS_IF_ANY_TARGET:
               for (band=0; band<bands; ++band)
               {
                  if (!ossim::isnan(theClampValueLo) && (buf[band][i] < clampLo))
                     buf[band][i] = clampLo;
                  else if (!ossim::isnan(theClampValueHi) && (buf[band][i] > clampHi))
                     buf[band][i] = clampHi;
               }
               break;

            case REPLACE_BAND_IF_PARTIAL_TARGET: 
            case REPLACE_ALL_BANDS_IF_PARTIAL_TARGET:
               // First band loop to establish if pixel qualifies for replacement (at least one 
               // band must be valid):
               can_replace = false;
               found_candidate = false;
               for (band=0; (band < bands) && !(can_replace && found_candidate); ++band)
               {
                  if ((!ossim::isnan(theClampValueLo) && (buf[band][i] < clampLo)) ||
                     (!ossim::isnan(theClampValueHi) && (buf[band][i] > clampHi)))
                     found_candidate = true;
                  else
                     can_replace = true;
               }
               if (can_replace && found_candidate)
               {
                  // This pixel has at least one band with normal value, so need to rescan bands
                  // to find pixels that need replacing (are within the target range):
                  for (band=0; band<bands; ++band)
                  {
                     if (!ossim::isnan(theClampValueLo) && (buf[band][i] < clampLo))
                        buf[band][i] = clampLo;
                     else if (!ossim::isnan(theClampValueHi) && buf[band][i] > clampHi)
                        buf[band][i] = clampHi;
                  }
               }
               break;

            case REPLACE_ONLY_FULL_TARGETS:
               // First band loop to establish if pixel qualifies for replacement (all 
               // bands must be in target range):
               can_replace = true;
               for (band=0; (band < bands) && can_replace; ++band)
               {
                  if ((ossim::isnan(theClampValueLo) || (buf[band][i] >= clampLo)) &&
                      (ossim::isnan(theClampValueHi) || (buf[band][i] <= clampHi)))
                     can_replace = false;
               }
               if (can_replace)
               {
                  // Map all pixels to replacement value: 
                  for (band=0; band<bands; ++band)
                  {
                     if (!ossim::isnan(theClampValueLo) && (buf[band][i] < clampLo))
                        buf[band][i] = clampLo;
                     else if (!ossim::isnan(theClampValueHi) && buf[band][i] > clampHi)
                        buf[band][i] = clampHi;
                  }
               }
               break;
            } // close switch

            // Proceed to next pixel location:
            ++i;
            continue; 
         }

         // If we got here (the continue statement was not reached) then
         // the pixel value now needs to be checked for possible target replacement:
         switch (theReplacementMode)
         {
         case REPLACE_BAND_IF_TARGET:
            for (band=0; band<bands; ++band)
            {
               if ((buf[band][i] >= targetLo) && (buf[band][i] <=targetHi)) 
                  buf[band][i] = theReplacementValue;
            }
            break;

         case REPLACE_BAND_IF_PARTIAL_TARGET: 

            // First band loop to establish if pixel qualifies for replacement (at least one 
            // band must be valid):
            can_replace = false;
            found_candidate = false;
            for (band=0; (band < bands) && !(can_replace && found_candidate); ++band)
            {
               //  check for target range replacement qualification:
               if ((buf[band][i] < targetLo) || (buf[band][i] > targetHi))
                  can_replace = true; // Has valid band
               else
                  found_candidate = true; // found band within target range
            }
            if (can_replace && found_candidate)
            {
               // This pixel has at least one band with normal value, so need to rescan bands
               // to find pixels that need replacing (are within the target range):
               for (band=0; band<bands; ++band)
               {
                  if ((buf[band][i] >= targetLo) && (buf[band][i] <= targetHi)) 
                     buf[band][i] = theReplacementValue;
               }
            }
            break;

         case REPLACE_ALL_BANDS_IF_PARTIAL_TARGET:

            // First band loop to establish if pixel qualifies for replacement (at least one 
            // band must be valid):
            can_replace = false;
            found_candidate = false;
            for (band=0; (band < bands) && !(can_replace && found_candidate); ++band)
            {
               // check for target range replacement qualification:
               if ((buf[band][i] < targetLo) || (buf[band][i] > targetHi))
                  can_replace = true;
               else
                  found_candidate = true;
            }
            if (can_replace && found_candidate)
            {
               // This pixel has at least one band with normal value and one with target, so 
               // map all bands to target:
               for (band=0; band<bands; ++band)
                  buf[band][i] = theReplacementValue;
            }
            break;

         case REPLACE_ONLY_FULL_TARGETS:

            // First band loop to establish if pixel qualifies for replacement (all 
            // bands must be in target range):
            can_replace = true;
            for (band=0; (band < bands) && can_replace; ++band)
            {
               // check for target range replacement qualification:
               if ((buf[band][i] < targetLo) || (buf[band][i] > targetHi))
                  can_replace = false;
            }
            if (can_replace)
            {
               // Map all pixels to replacement value: 
               for (band=0; band<bands; ++band)
                  buf[band][i] = theReplacementValue;
            }
            break;

         case REPLACE_ALL_BANDS_IF_ANY_TARGET:

            // First band loop to establish if pixel qualifies for replacement (all 
            // bands must be in target range):
            can_replace = false;
            for (band=0; (band < bands) && !can_replace; ++band)
            {
               // check for target range replacement qualification:
               if ((buf[band][i] >= targetLo) && (buf[band][i] <= targetHi))
                  can_replace = true;
            }
            if (can_replace)
            {
               // Map all pixels to replacement value: 
               for (band=0; band<bands; ++band)
                  buf[band][i] = theReplacementValue;
            }
            break;
         } // close switch

         // Reached end of processing for one pixel location. Increment the band buffers index:
         ++i;
      } // end of loop over pixel_loc.x
   } // end of loop over pixel_loc.y
   
   delete [] buf;
   inputTile->validate();
}

template <class T> void ossimPixelFlipper::clipTile(T /* dummy */,
                                                    ossimImageData* inputTile,
                                                    ossim_uint32 resLevel)
{
   if(theClipMode == NONE)
   {
      theClipTileBuffer = 0;
      return;
   }
   if(!inputTile) return;
   if(!inputTile->getBuf()||
      (inputTile->getDataObjectStatus() == OSSIM_EMPTY))
   {
      return;
   }
   allocateClipTileBuffer(inputTile);


   if(theClipTileBuffer.valid())
   {
      ossimIrect tileRect = inputTile->getImageRectangle();
      // force to all nulls
      theClipTileBuffer->setDataObjectStatus(OSSIM_FULL);
      theClipTileBuffer->makeBlank();
      
      switch(theClipMode)
      {
         case NONE:
         {
            break;
         }
         case BOUNDING_RECT:
         {
            if(resLevel < theBoundingRects.size())
            {
               if(tileRect.completely_within(theBoundingRects[resLevel])||
                  theBoundingRects[resLevel].hasNans())
               {
                  return;
               }
               else
               {
                  if(tileRect.intersects(theBoundingRects[resLevel]))
                  {
                     ossimIrect clipRect = tileRect.clipToRect(theBoundingRects[resLevel]);

                     theClipTileBuffer->setImageRectangle(clipRect);
                     
                     theClipTileBuffer->loadTile(inputTile);
                     inputTile->makeBlank();
                     inputTile->loadTile(theClipTileBuffer.get());
                     inputTile->validate();
                  }
                  else
                  {
                     inputTile->makeBlank();
                  }
               }
            }
            break;
         }
         case VALID_VERTICES:
         {
            if(resLevel < theValidVertices.size())
            {
               const ossimPolygon& p = theValidVertices[resLevel];
               bool ulWithin = p.isPointWithin(tileRect.ul());
               bool urWithin = p.isPointWithin(tileRect.ur());
               bool lrWithin = p.isPointWithin(tileRect.lr());
               bool llWithin = p.isPointWithin(tileRect.ll());
               
               if(ulWithin&&
                  urWithin&&
                  lrWithin&&
                  llWithin)
               {
                  return;
               }
               else if(!(ulWithin|| // if none are in
                         urWithin||
                         lrWithin||
                         llWithin))
               {
                  inputTile->makeBlank();
                  return;
               }
               else
               {
                  ossimIpt ul = tileRect.ul();
                  ossimIpt origin;
                  ossim_uint32 x = 0;
                  ossim_uint32 y = 0;
                  ossim_uint32 w = inputTile->getWidth();
                  ossim_uint32 h = inputTile->getHeight();
                  ossim_uint32 offset = 0;
                  origin.y = ul.y;
                  for(y = 0; y < h; ++y)
                  {
                     origin.x = ul.x;
                     for(x = 0; x < w; ++x)
                     {
                        if(!p.isPointWithin(origin))
                        {
                           inputTile->setNull(offset);
                        }
                        ++offset;
                        ++origin.x;
                     }
                     ++origin.y;
                  }
               }
            }
            break;
         }
      }
   }
}


void ossimPixelFlipper::allocateClipTileBuffer(ossimRefPtr<ossimImageData> inputImage)
{
   if(inputImage.valid())
   {
      bool needDupFlag = false;
      if(theClipTileBuffer.valid())
      {
         if((theClipTileBuffer->getScalarType() != inputImage->getScalarType())||
            theClipTileBuffer->getNumberOfBands() != theClipTileBuffer->getNumberOfBands())
         {
            needDupFlag = true;
         }
         else
         {
            theClipTileBuffer->setImageRectangle(inputImage->getImageRectangle());
         }
      }
      else
      {
         needDupFlag = true;
      }
      if(needDupFlag)
      {
         theClipTileBuffer = (ossimImageData*)inputImage->dup();
      }
   }
}

void ossimPixelFlipper::initialize()
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> scopeLock(theMutex);
   ossimImageSourceFilter::initialize();
   theValidVertices.clear();
   theBoundingRects.clear();

   ossim_uint32 rlevels = getNumberOfDecimationLevels();
   ossim_uint32 idx = 0;

   if(rlevels)
   {
      if(theValidVertices.size() != rlevels)
      {
         theValidVertices.resize(rlevels);
      }
      if(theBoundingRects.size() != rlevels)
      {
         theBoundingRects.resize(rlevels);
      }
      for(idx = 0; idx < rlevels; ++idx)
      {
         std::vector<ossimIpt> validVertices;
         getValidImageVertices(validVertices,
                               OSSIM_CLOCKWISE_ORDER,
                               idx);
         theValidVertices[idx] = ossimPolygon(validVertices);
         theBoundingRects[idx] = getBoundingRect(idx);
      }
   }
}

ossimScalarType ossimPixelFlipper::getOutputScalarType() const
{
   if(theInputConnection)
   {
      ossimScalarType scalar = theInputConnection->getOutputScalarType();
      {
         if (scalar == OSSIM_USHORT16 && theClampValueHi == 2047.0)
         {
            //---
            // Special case:
            // We have an unsigned 16 bit type but we want to call it
            // USHORT11 ( (2^11- 1) = 2047 ).
            //---
            return OSSIM_USHORT11;
         }
         return scalar;
      }
   }
   return OSSIM_SCALAR_UNKNOWN;
}

ossim_float64 ossimPixelFlipper::getMaxPixelValue (ossim_uint32 band) const
{
   const ossim_float64 MIN = ossimImageSourceFilter::getMinPixelValue(band);
   const ossim_float64 MAX = ossimImageSourceFilter::getMaxPixelValue(band);

   if ((theClampValueHi > MIN) && (theClampValueHi < MAX))
      return theClampValueHi;

   return MAX;
}

ossim_float64 ossimPixelFlipper::getMinPixelValue (ossim_uint32 band) const
{
   const ossim_float64 MIN = ossimImageSourceFilter::getMinPixelValue(band);
   const ossim_float64 MAX = ossimImageSourceFilter::getMaxPixelValue(band);

   if ((theClampValueLo > MIN) && (theClampValueLo < MAX))
      return theClampValueLo;

   return MIN;
}

bool ossimPixelFlipper::loadState(const ossimKeywordlist& kwl,
                                  const char* prefix)
{
   const char* lookupReturn;
   
   lookupReturn = kwl.find(prefix, PF_TARGET_VALUE_KW);
   if(lookupReturn)
   {
      setTargetValue(atof(lookupReturn));
   }

   lookupReturn = kwl.find(prefix, PF_TARGET_RANGE_KW);
   if(lookupReturn)
   {
      ossimString min_max_string (lookupReturn);
      ossimString separator (" ");
      ossim_float64 min_target = min_max_string.before(separator).toFloat64();
      ossim_float64 max_target = min_max_string.after(separator).toFloat64();
      setTargetRange(min_target, max_target);
   }

   lookupReturn = kwl.find(prefix, PF_REPLACEMENT_VALUE_KW);
   if(lookupReturn)
   {
      setReplacementValue(atof(lookupReturn));
   }

   lookupReturn = kwl.find(prefix, PF_REPLACEMENT_MODE_KW);
   if(lookupReturn)
   {
      ossimString modeString = lookupReturn;
      setReplacementMode(modeString);
   }

   lookupReturn = kwl.find(prefix, PF_CLAMP_VALUE_KW);
   if(lookupReturn)
   {
      setClampValue(atof(lookupReturn), true);
   }

   lookupReturn = kwl.find(prefix, PF_CLAMP_VALUE_LO_KW);
   if(lookupReturn)
   {
      setClampValue(atof(lookupReturn), false);
   }

   lookupReturn = kwl.find(prefix, PF_CLAMP_VALUE_HI_KW);
   if(lookupReturn)
   {
      setClampValue(atof(lookupReturn), true);
   }

   lookupReturn = kwl.find(prefix, PF_CLIP_MODE_KW);
   if(lookupReturn)
   {
      ossimString modeString = lookupReturn;
      setClipMode(modeString);
   }

   bool status = ossimImageSourceFilter::loadState(kwl, prefix);

   if (traceDebug())
   {
      print(ossimNotify(ossimNotifyLevel_DEBUG));
   }

   return status;
}

bool ossimPixelFlipper::saveState(ossimKeywordlist& kwl,
                                 const char* prefix) const
{
   // Call the base class saveState.
   ossimImageSourceFilter::saveState(kwl, prefix);

   if (theTargetValueHi != theTargetValueLo)
   {
      ossimString s (ossimString::toString(theTargetValueLo) + " " + 
         ossimString::toString(theTargetValueHi));
      kwl.add(prefix, PF_TARGET_RANGE_KW, s);
   }
   else
   {
      kwl.add(prefix, PF_TARGET_VALUE_KW, theTargetValueLo);
   }
   kwl.add(prefix, PF_REPLACEMENT_VALUE_KW, theReplacementValue);
   kwl.add(prefix, PF_REPLACEMENT_MODE_KW,  getReplacementModeString().c_str());

   if (theClampingMode)
   {
      kwl.add(prefix, PF_CLAMP_VALUE_LO_KW,    theClampValueLo);
      kwl.add(prefix, PF_CLAMP_VALUE_HI_KW,    theClampValueHi);
   }
   kwl.add(prefix, PF_CLIP_MODE_KW,  getClipModeString().c_str());

   return true;
}

void ossimPixelFlipper::setTargetValue(ossim_float64 target_value)
{
   //---
   // Since this is the value to replace we will allow for any value as it
   // won't affect the output null, min and max ranges.  This will fix a
   // tiled nitf with max of 2047(11bit) with edge tile fill values of 2048.
   //---
   OpenThreads::ScopedLock<OpenThreads::Mutex> scopeLock(theMutex);

   theTargetValueLo = target_value;
   theTargetValueHi = target_value; 
}

void ossimPixelFlipper::setTargetRange(ossim_float64 target_min, ossim_float64 target_max)
{
   //---
   // Since this is the value to replace we will allow for any value as it
   // won't affect the output null, min and max ranges.  This will fix a
   // tiled nitf with max of 2047(11bit) with edge tile fill values of 2048.
   //---
   OpenThreads::ScopedLock<OpenThreads::Mutex> scopeLock(theMutex);

   theTargetValueLo = target_min;
   theTargetValueHi = target_max; 
}

void ossimPixelFlipper::setReplacementValue(ossim_float64 replacement_value)
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> scopeLock(theMutex);

   // Range check to ensure within null, min and max of output radiometry.
   if (inRange(replacement_value))
   {
      theReplacementValue = replacement_value;
   }
}

void ossimPixelFlipper::setClampValue(ossim_float64 clamp_value, bool clamp_max_value)
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> scopeLock(theMutex);

   if (inRange(clamp_value))
   {
      // Stupid MS compiler complains if we do an |= on enum type. (OLK 1/11)
      int temp_int = (int) theClampingMode;
      if (clamp_max_value)
      {
         theClampValueHi = clamp_value;
         temp_int |= (int) CLAMPING_HI;
      }
      else
      {
         theClampValueLo = clamp_value;
         temp_int |= (int) CLAMPING_LO;
      }
      theClampingMode = (ClampingMode) temp_int;
   }
}

void ossimPixelFlipper::setClampValues(ossim_float64 clamp_value_lo, ossim_float64 clamp_value_hi)
{
   theClampingMode = DISABLED; // reset and let next calls set accordingly
   setClampValue(clamp_value_lo, false);
   setClampValue(clamp_value_hi, true);
}

void ossimPixelFlipper::setReplacementMode(ossimPixelFlipper::ReplacementMode mode)
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> scopeLock(theMutex);
   theReplacementMode = mode;
}

bool ossimPixelFlipper::setReplacementMode(const ossimString& modeString)
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> scopeLock(theMutex);

   ossimString mode = modeString;
   mode.upcase();
   if (mode == "REPLACE_BAND_IF_TARGET")
   {
      theReplacementMode = REPLACE_BAND_IF_TARGET;
   }
   else if (mode == "REPLACE_BAND_IF_PARTIAL_TARGET")
   {
      theReplacementMode = REPLACE_BAND_IF_PARTIAL_TARGET;
   }
   else if (mode == "REPLACE_ALL_BANDS_IF_PARTIAL_TARGET")
   {
      theReplacementMode = REPLACE_ALL_BANDS_IF_PARTIAL_TARGET;
   }
   else if (mode == "REPLACE_ONLY_FULL_TARGETS")
   {
      theReplacementMode = REPLACE_ONLY_FULL_TARGETS;
   }
   else if (mode == "REPLACE_ALL_BANDS_IF_ANY_TARGET")
   {
      theReplacementMode = REPLACE_ALL_BANDS_IF_ANY_TARGET;
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimPixelFlipper::setReplacementMode warning:\n"
         << "Invalid mode:  " << modeString
         << endl;
      return false;
   }
   return true;
}

void ossimPixelFlipper::setClipMode(ossimPixelFlipper::ClipMode mode)
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> scopeLock(theMutex);
   theClipMode = mode;
}

void ossimPixelFlipper::setClipMode(const ossimString& modeString)
{
   ossimString mode = modeString;
   mode.downcase();
   if (mode == "none")
   {
      setClipMode(NONE);
   }
   else if (mode == "bounding_rect")
   {
      setClipMode(BOUNDING_RECT);
   }
   else if (mode == "valid_vertices")
   {
      setClipMode(VALID_VERTICES);
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimPixelFlipper::setClipMode warning:\n"
         << "Invalid mode:  " << modeString
         << endl;
   }
}

//ossim_float64 ossimPixelFlipper::getTargetValue() const
//{
//   return theTargetValueLo;
//}

ossim_float64 ossimPixelFlipper::getReplacementValue() const
{
   return theReplacementValue;
}

ossimPixelFlipper::ReplacementMode ossimPixelFlipper::getReplacementMode()  const
{
   return theReplacementMode;
}
   
ossimString ossimPixelFlipper::getReplacementModeString()  const
{
   switch(theReplacementMode)
   {
      case REPLACE_BAND_IF_TARGET:
         return ossimString("REPLACE_BAND_IF_TARGET");
      case REPLACE_BAND_IF_PARTIAL_TARGET:
         return ossimString("REPLACE_BAND_IF_PARTIAL_TARGET");
      case REPLACE_ALL_BANDS_IF_PARTIAL_TARGET:
         return ossimString("REPLACE_ALL_BANDS_IF_PARTIAL_TARGET");
      case REPLACE_ONLY_FULL_TARGETS:
         return ossimString("REPLACE_ONLY_FULL_TARGETS");
      case REPLACE_ALL_BANDS_IF_ANY_TARGET:
         return ossimString("REPLACE_ALL_BANDS_IF_ANY_TARGET");
      default:
         break;
   }

   return ossimString("UNKNOWN_MODE");
}

ossimString ossimPixelFlipper::getClipModeString()  const
{
   if (theClipMode == BOUNDING_RECT)
      return ossimString("bounding_rect");

   if (theClipMode == VALID_VERTICES)
      return ossimString("valid_vertices");

   return ossimString("none");
}

ossimPixelFlipper::ClipMode ossimPixelFlipper::getClipMode() const
{
   return theClipMode;
}

std::ostream& ossimPixelFlipper::print(std::ostream& out) const
{
   out << "ossimPixelFlipper::print:"
       << "\ntarget value Lo:    " << theTargetValueLo
       << "\ntarget value Hi:    " << theTargetValueHi
       << "\nreplacement value:  " << theReplacementValue
       << "\nclamp value Lo:     " << theClampValueLo
       << "\nclamp value Hi:     " << theClampValueHi
       << "\nreplacement mode:   " << getReplacementModeString().c_str()
       << "\nclip_mode:          " << getClipModeString().c_str()
      << endl;
   return out;
}

ossimRefPtr<ossimProperty> ossimPixelFlipper::getProperty(
   const ossimString& name)const
{
   // Lock for the length of this method.
	OpenThreads::ScopedLock<OpenThreads::Mutex> scopeLock(theMutex);

   if (name == PF_TARGET_VALUE_KW)
   {
      ossimProperty* p =
         new ossimNumericProperty(name,
         ossimString::toString(theTargetValueLo));
      p->setCacheRefreshBit();
      return ossimRefPtr<ossimProperty>(p);
   }
   if (name == PF_TARGET_RANGE_KW)
   {
      ossimProperty* p =
         new ossimNumericProperty(name,
         ossimString::toString(theTargetValueLo));
      p->setCacheRefreshBit();
      return ossimRefPtr<ossimProperty>(p);
   }
   if (name == TARGET_UPPER_LIMIT_PROP_NAME)
   {
      ossimProperty* p =
         new ossimNumericProperty(name,
         ossimString::toString(theTargetValueHi));
      p->setCacheRefreshBit();
      return ossimRefPtr<ossimProperty>(p);
   }
   else if (name == PF_REPLACEMENT_VALUE_KW)
   {
      ossimProperty* p =
         new ossimNumericProperty(name,
                                  ossimString::toString(theReplacementValue));
       p->setCacheRefreshBit();
      return ossimRefPtr<ossimProperty>(p);
   }
   else if (name == PF_CLAMP_VALUE_LO_KW)
   {
      ossimProperty* p =
         new ossimNumericProperty(name, ossimString::toString(theClampValueLo));
      p->setCacheRefreshBit();
      return ossimRefPtr<ossimProperty>(p);
   }
   else if (name == PF_CLAMP_VALUE_HI_KW)
   {
      ossimProperty* p =
         new ossimNumericProperty(name, ossimString::toString(theClampValueHi));
      p->setCacheRefreshBit();
      return ossimRefPtr<ossimProperty>(p);
   }
   else if (name == PF_REPLACEMENT_MODE_KW)
   {
      vector<ossimString> constraintList(5);
      constraintList[0] = "REPLACE_BAND_IF_TARGET";
      constraintList[1] = "REPLACE_BAND_IF_PARTIAL_TARGET";
      constraintList[2] = "REPLACE_ALL_BANDS_IF_PARTIAL_TARGET";
      constraintList[3] = "REPLACE_ONLY_FULL_TARGETS";
      constraintList[4] = "REPLACE_ALL_BANDS_IF_ANY_TARGET";
      
      ossimStringProperty* p =
         new ossimStringProperty(name,
                                 getReplacementModeString(),
                                 false,
                                 constraintList);
      p->setCacheRefreshBit();
      return ossimRefPtr<ossimProperty>(p);
   }
   else if (name == PF_CLIP_MODE_KW)
   {
      vector<ossimString> constraintList(3);
      constraintList[0] = "none";
      constraintList[1] = "bounding_rect";
      constraintList[2] = "valid_vertices";
      
      ossimStringProperty* p =
         new ossimStringProperty(name,
                                 getClipModeString(),
                                 false,
                                 constraintList);
      p->setCacheRefreshBit();
      return ossimRefPtr<ossimProperty>(p);
   }
   ossimRefPtr<ossimProperty> result = ossimSource::getProperty(name);

   if(result.valid())
   {
      if(result->getName() == ossimKeywordNames::ENABLED_KW)
      {
         result->clearChangeType();
         
         // we will at least say its a radiometric change
         //
         result->setCacheRefreshBit();
      }
   }

   return result;
}

void ossimPixelFlipper::setProperty(ossimRefPtr<ossimProperty> property)
{
   if (!property) return;

   ossimString os = property->valueToString();
   
   ossimString name = property->getName();
   if (name == PF_TARGET_VALUE_KW)
   {
      setTargetValue(os.toDouble());
   }
   if (name == TARGET_LOWER_LIMIT_PROP_NAME)
   {
      setTargetRange(os.toDouble(), theTargetValueHi);
   }
   if (name == TARGET_UPPER_LIMIT_PROP_NAME)
   {
      setTargetRange(theTargetValueLo, os.toDouble());
   }
   else if  (name == PF_REPLACEMENT_VALUE_KW)
   {
      setReplacementValue(os.toDouble());
   }
   else if  (name == PF_REPLACEMENT_MODE_KW)
   {
      setReplacementMode(os);
   }
   else if  (name == PF_CLAMP_VALUE_LO_KW)
   {
      setClampValue(os.toDouble(), false);
   }
   else if  (name == PF_CLAMP_VALUE_HI_KW)
   {
      setClampValue(os.toDouble(), true);
   }
   else if  (name == PF_CLIP_MODE_KW)
   {
      setClipMode(os);
   }
   else
   {
      ossimImageSourceFilter::setProperty(property);
   }
}

void ossimPixelFlipper::getPropertyNames(
   std::vector<ossimString>& propertyNames)const
{
   propertyNames.push_back(PF_TARGET_VALUE_KW);
   propertyNames.push_back(TARGET_LOWER_LIMIT_PROP_NAME);
   propertyNames.push_back(TARGET_UPPER_LIMIT_PROP_NAME);
   propertyNames.push_back(PF_REPLACEMENT_VALUE_KW);
   propertyNames.push_back(PF_REPLACEMENT_MODE_KW);
   propertyNames.push_back(PF_CLAMP_VALUE_LO_KW);
   propertyNames.push_back(PF_CLAMP_VALUE_HI_KW);
   propertyNames.push_back(PF_CLIP_MODE_KW);
   
   ossimImageSourceFilter::getPropertyNames(propertyNames);
}

ossimString ossimPixelFlipper::getShortName()const
{
   return ossimString("Pixel flipper");
}

bool ossimPixelFlipper::inRange(ossim_float64 value) const
{
   if (!theInputConnection)
   {
      //---
      // Not initialized yet...  We're going to return true so that things
      // like loadState work with the assumption that the caller know's
      // the correct range.  This check is really intended for an uncheck
      // range on the gui side.
      //---
      return true;
   }
   
   const ossim_float64 NULL_PIX = ossimImageSourceFilter::getNullPixelValue();
   const ossim_float64 MIN_PIX  = ossimImageSourceFilter::getMinPixelValue();
   const ossim_float64 MAX_PIX  = ossimImageSourceFilter::getMaxPixelValue();

   if ( (value == NULL_PIX) || ((value >= MIN_PIX) && (value <= MAX_PIX)) )
   {
      return true;
   }

   ossimNotify(ossimNotifyLevel_WARN)
      << "\nossimPixelFlipper::inRange WARNING:"
      << "\nvalue \"" << value
      << "\" is out of range!"
      << "\nInput source null = " << NULL_PIX
      << "\nInput source min  = " << MIN_PIX
      << "\nInput source max  = " << MAX_PIX
      << endl;

   return false;
}
