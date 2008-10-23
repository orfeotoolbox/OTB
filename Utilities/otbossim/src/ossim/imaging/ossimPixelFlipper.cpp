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
// $Id: ossimPixelFlipper.cpp 12983 2008-06-04 01:15:08Z dburken $

#include <cstdlib>
#include <ossim/imaging/ossimPixelFlipper.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/base/ossimNumericProperty.h>
#include <ossim/base/ossimKeywordNames.h>
#include <OpenThreads/ScopedLock>

RTTI_DEF1(ossimPixelFlipper, "ossimPixelFlipper", ossimImageSourceFilter)

static ossimTrace traceDebug("ossimPixelFlipper:debug");

static const char TARGET_VALUE_KW[]      = "target_value";
static const char REPLACEMENT_VALUE_KW[] = "replacement_value";
static const char CLAMP_VALUE_KW[]       = "clamp_value";
static const char REPLACEMENT_MODE_KW[]  = "replacement_mode";
static const char CLIP_MODE_KW[]  = "clip_mode";

#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimPixelFlipper.cpp 12983 2008-06-04 01:15:08Z dburken $";
#endif

ossimPixelFlipper::ossimPixelFlipper(ossimObject* owner)
   :
      ossimImageSourceFilter(owner),
      theTargetValue(0.0),
      theReplacementValue(1.0),
      theClampValue(0.0),
      theReplacementMode(ossimPixelFlipper::REPLACE_ALL_TARGETS),
      theClipMode(ossimPixelFlipper::ossimPixelFlipperClipMode_NONE)
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
   
   return inputTile;
}

template <class T>
void ossimPixelFlipper::flipPixels(T dummy,
                                   ossimImageData* inputTile,
                                   ossim_uint32 resLevel)
{
   if (!inputTile) return;

   T target      = static_cast<T>(theTargetValue);
   T replacement = static_cast<T>(theReplacementValue);
   T clamp       = static_cast<T>(theClampValue);
   ossim_uint32 size   = inputTile->getSizePerBand();
   ossim_uint32 bands  = inputTile->getNumberOfBands();
   ossim_uint32 band;

   bool do_clamp = (clamp > 0) ? true : false;

   // Get pointers to data for each band.
   T** buf = new T*[bands];

   for(band=0; band<bands; ++band)
   {
      buf[band] = static_cast<T*>(inputTile->getBuf(band));
   }

   ossimIrect rect = inputTile->getImageRectangle();
   ossimIpt ul = rect.ul();
   ossimIpt origin = ul;
   ossim_uint32 x=0;
   ossim_uint32 y=0;
   ossim_uint32 w = rect.width();
   ossim_uint32 h = rect.height();
   ossim_uint32 i = 0;   
   bool replace = false;
   bool needsTesting = false;
   switch(theClipMode)
   {
      case  ossimPixelFlipperClipMode_BOUNDING_RECT:
      {
         if(resLevel < theBoundingRects.size())
         {
            if(!rect.intersects(theBoundingRects[resLevel]))
            {
               return;
            }
            else
            {
               needsTesting = !rect.completely_within(theBoundingRects[resLevel]);
            }
         }
         break;
      }
      case  ossimPixelFlipperClipMode_VALID_VERTICES:
      {
         if(resLevel < theValidVertices.size())
         {
            bool ulFlag = theValidVertices[resLevel].isPointWithin(rect.ul());
            bool urFlag = theValidVertices[resLevel].isPointWithin(rect.ur());
            bool lrFlag = theValidVertices[resLevel].isPointWithin(rect.lr());
            bool llFlag = theValidVertices[resLevel].isPointWithin(rect.ll());
            if((!ulFlag)&&
               (!urFlag)&&
               (!lrFlag)&&
               (!llFlag))
            {
               return; // none of the tile is inside so return
            }
            else
            {
               needsTesting = !(ulFlag&&urFlag&&lrFlag&&llFlag);
            }
         }
         break;
      }
      default:
      {
         break;
      }
   }
   
   switch (theReplacementMode)
   {
      case ossimPixelFlipper::REPLACE_PARTIAL_TARGETS:
      {
         i = 0;
         if((theClipMode == ossimPixelFlipperClipMode_NONE)||(!needsTesting))
         {
            for (i=0; i<size; ++i)
            {
               // At least one band must NOT have target for replace to = true;
               replace = false;
               for (band=0; band<bands; ++band)
               {
                  if (buf[band][i] != target)
                  {
                     replace = true;
                     break;
                  }
               }
               if (replace)
               {
                  for (band=0; band<bands; ++band)
                  {
                     if (buf[band][i] == target) buf[band][i] = replacement;
                  }
               }
            }
         }
         else
         {
            bool active;
            origin.y = ul.y;
            for(y = 0; y < h; ++y)
            {
               origin.x = ul.x;
               for(x =0; x < w; ++x)
               {
                  active = true;
                  switch(theClipMode)
                  {
                     case  ossimPixelFlipperClipMode_BOUNDING_RECT:
                     {
                        if(resLevel < theBoundingRects.size())
                        {
                           active = theBoundingRects[resLevel].pointWithin(origin);
                        }
                        break;
                     }
                     case  ossimPixelFlipperClipMode_VALID_VERTICES:
                     {
                        if(resLevel < theValidVertices.size())
                        {
                           active = theValidVertices[resLevel].isPointWithin(origin);
                        }
                        break;
                     }
                     default:
                     {
                        break;
                     }
                  }
                  replace = false;
                  if(active)
                  {
                     for (band=0; band<bands; ++band)
                     {
                        if (buf[band][i] != target)
                        {
                           replace = true;
                           break;
                        }
                     }
                  }
                  if (replace)
                  {
                     for (band=0; band<bands; ++band)
                     {
                        if (buf[band][i] == target) buf[band][i] = replacement;
                     }
                  }
                  
                  ++i;
                  ++origin.x;
               }
               ++origin.y;
            }
         }
         break;
      }
      case ossimPixelFlipper::REPLACE_PARTIAL_TARGETS_ALL_BANDS:
      {
         i = 0;
         if((theClipMode == ossimPixelFlipperClipMode_NONE)||(!needsTesting))
         {
            for (i=0; i<size; ++i)
            {
               // Must be at least one target but not all to replace all bands.
               bool has_a_target = false;
               replace      = false;
               for (band=0; band<bands; ++band)
               {
                  if (buf[band][i] == target)
                  {
                     has_a_target = true;
                     break;
                  }
               }
               if (has_a_target)
               {
                  for (band=0; band<bands; ++band)
                  {
                     if (buf[band][i] != target)
                     {
                        replace = true;
                        break;
                     }
                  }
                  if (replace)
                  {
                     for (band=0; band<bands; ++band)
                     {
                        buf[band][i] = replacement;
                     }
                  }
               }
            }
         }
         else
         {
            bool active = true;
            origin.y = ul.y;
            for(y = 0; y < h; ++y)
            {
               origin.x = ul.x;
               for(x =0; x < w; ++x)
               {
                  switch(theClipMode)
                  {
                     case  ossimPixelFlipperClipMode_BOUNDING_RECT:
                     {
                        if(resLevel < theBoundingRects.size())
                        {
                           active = theBoundingRects[resLevel].pointWithin(origin);
                        }
                        break;
                     }
                     case  ossimPixelFlipperClipMode_VALID_VERTICES:
                     {
                        if(resLevel < theValidVertices.size())
                        {
                           active = theValidVertices[resLevel].isPointWithin(origin);
                        }
                        break;
                     }
                     default:
                     {
                        break;
                     }
                  }
                  bool has_a_target = false;
                  replace      = false;
                  if(active)
                  {
                     for (band=0; band<bands; ++band)
                     {
                        if (buf[band][i] == target)
                        {
                           has_a_target = true;
                           break;
                        }
                     }
                  }
                  if (has_a_target)
                  {
                     for (band=0; band<bands; ++band)
                     {
                        if (buf[band][i] != target)
                        {
                           replace = true;
                           break;
                        }
                     }
                     if (replace)
                     {
                        for (band=0; band<bands; ++band)
                        {
                           buf[band][i] = replacement;
                        }
                     }
                  }
                  
                  ++i;
                  ++origin.x;
               }
               ++origin.y;
            }
         }
         break;
      }
      case ossimPixelFlipper::REPLACE_FULL_TARGETS:
      {
         i = 0;
         if((theClipMode == ossimPixelFlipperClipMode_NONE)||(!needsTesting))
         {
            for (i=0; i<size; ++i)
            {
               // All bands must have target for replace to = true.
               replace = true;
               
               for (band=0; band<bands; ++band)
               {
                  if (buf[band][i] != target)
                  {
                     replace = false;
                     break;
                  }
               }
               if (replace)
               {
                  for (band=0; band<bands; ++band)
                  {
                     buf[band][i] = replacement;
                  }
               }
            }
         }
         else
         {
            origin.y = ul.y;
            for(y = 0; y < h; ++y)
            {
               origin.x = ul.x;
               for(x =0; x < w; ++x)
               {
                  replace = true;
                  switch(theClipMode)
                  {
                     case  ossimPixelFlipperClipMode_BOUNDING_RECT:
                     {
                        if(resLevel < theBoundingRects.size())
                        {
                           replace = theBoundingRects[resLevel].pointWithin(origin);
                        }
                        break;
                     }
                     case  ossimPixelFlipperClipMode_VALID_VERTICES:
                     {
                        if(resLevel < theValidVertices.size())
                        {
                           replace = theValidVertices[resLevel].isPointWithin(origin);
                        }
                        break;
                     }
                     default:
                     {
                        break;
                     }
                  }
                  if(replace)
                  {
                     for (band=0; band<bands; ++band)
                     {
                        if (buf[band][i] != target)
                        {
                           replace = false;
                           break;
                        }
                     }
                  }
                  
                  if (replace)
                  {
                     for (band=0; band<bands; ++band)
                     {
                        buf[band][i] = replacement;
                     }
                  }
                  ++i;
                  ++origin.x;
               }
               ++origin.y;
            }
         }
            
         break;
      }
      default:  // REPLACE_ALL_TARGETS
      {
         i = 0;
         if((theClipMode == ossimPixelFlipperClipMode_NONE)||(!needsTesting))
         {
            for(i = 0; i < size;++i)
            {
               for (band=0; band<bands; ++band)
               {
                  if (buf[band][i] == target) buf[band][i] = replacement;
               }
            }
         }
         else
         {
            origin.y = ul.y;
            for(y = 0; y < h; ++y)
            {
               origin.x = ul.x;
               for(x =0; x < w; ++x)
               {
                  replace = true;
                  
                  switch(theClipMode)
                  {
                     case  ossimPixelFlipperClipMode_BOUNDING_RECT:
                     {
                        if(resLevel < theBoundingRects.size())
                        {
                           replace = theBoundingRects[resLevel].pointWithin(origin);
                        }
                        break;
                     }
                     case  ossimPixelFlipperClipMode_VALID_VERTICES:
                     {
                        if(resLevel < theValidVertices.size())
                        {
                           replace = theValidVertices[resLevel].isPointWithin(origin);
                        }
                        break;
                     }
                     default:
                     {
                        break;
                     }
                  }
                  if(replace)
                  {
                     for (band=0; band<bands; ++band)
                     {
                        if (buf[band][i] == target) buf[band][i] = replacement;
                     }
                  }
                  ++i;
                  ++origin.x;
               }
               ++origin.y;
            }
         }
         break;
      }

      if (do_clamp)
      {
         for (i=0; i<size; ++i)
         { 
            for (band=0; band<bands; ++band)
            {
               if (buf[band][i] > clamp)
               {
                  buf[band][i] = clamp;
               }
            }
         }
      }
   }
   
   delete [] buf;
   inputTile->validate();
}

template <class T> void ossimPixelFlipper::clipTile(T dummy,
                                                    ossimImageData* inputTile,
                                                    ossim_uint32 resLevel)
{
   if(theClipMode == ossimPixelFlipperClipMode_NONE)
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
         case ossimPixelFlipperClipMode_NONE:
         {
            break;
         }
         case ossimPixelFlipperClipMode_BOUNDING_RECT:
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
         case ossimPixelFlipperClipMode_VALID_VERTICES:
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
         if (scalar == OSSIM_USHORT16 && theClampValue == 2047.0)
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

   if (theClampValue > MIN && theClampValue < MAX)
   {
      return theClampValue;
   }

   return MAX;
}

bool ossimPixelFlipper::loadState(const ossimKeywordlist& kwl,
                                 const char* prefix)
{
   const char* lookupReturn;
   
   lookupReturn = kwl.find(prefix, TARGET_VALUE_KW);
   if(lookupReturn)
   {
      setTargetValue(atof(lookupReturn));
   }

   lookupReturn = kwl.find(prefix, REPLACEMENT_VALUE_KW);
   if(lookupReturn)
   {
      setReplacementValue(atof(lookupReturn));
   }

   lookupReturn = kwl.find(prefix, CLAMP_VALUE_KW);
   if(lookupReturn)
   {
      setClampValue(atof(lookupReturn));
   }

   lookupReturn = kwl.find(prefix, REPLACEMENT_MODE_KW);
   if(lookupReturn)
   {
      ossimString modeString = lookupReturn;
      setReplacementMode(modeString);
   }
   lookupReturn = kwl.find(prefix, CLIP_MODE_KW);
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

   kwl.add(prefix, TARGET_VALUE_KW,      theTargetValue);
   kwl.add(prefix, REPLACEMENT_VALUE_KW, theReplacementValue);
   kwl.add(prefix, CLAMP_VALUE_KW,       theClampValue);
   kwl.add(prefix, REPLACEMENT_MODE_KW,  getReplacementModeString().c_str());
   kwl.add(prefix, CLIP_MODE_KW,  getClipModeString().c_str());
   
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

   theTargetValue = target_value;
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

void ossimPixelFlipper::setClampValue(ossim_float64 clamp_value)
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> scopeLock(theMutex);
   
   if (inRange(clamp_value))
   {
      theClampValue = clamp_value;
   }

}

void ossimPixelFlipper::setReplacementMode(
   ossimPixelFlipper::ReplacementMode mode)
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> scopeLock(theMutex);
   theReplacementMode = mode;
}

void ossimPixelFlipper::setReplacementMode(const ossimString& modeString)
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> scopeLock(theMutex);

   ossimString mode = modeString;
   mode.downcase();
   if (mode == "replace_partial_targets")
   {
      theReplacementMode = ossimPixelFlipper::REPLACE_PARTIAL_TARGETS;
   }
   else if (mode == "replace_partial_targets_all_bands")
   {
      theReplacementMode =
         ossimPixelFlipper::REPLACE_PARTIAL_TARGETS_ALL_BANDS;
   }
   else if (mode == "replace_full_targets")
   {
      theReplacementMode = ossimPixelFlipper::REPLACE_FULL_TARGETS;
   }
   else if (mode == "replace_all_targets")
   {
      theReplacementMode = ossimPixelFlipper::REPLACE_ALL_TARGETS;
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimPixelFlipper::setReplacementMode warning:\n"
         << "Invalide mode:  " << modeString
         << endl;
   }

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
      setClipMode(ossimPixelFlipperClipMode_NONE);
   }
   else if (mode == "bounding_rect")
   {
      setClipMode(ossimPixelFlipperClipMode_BOUNDING_RECT);
   }
   else if (mode == "valid_vertices")
   {
      setClipMode(ossimPixelFlipperClipMode_VALID_VERTICES);
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimPixelFlipper::setClipMode warning:\n"
         << "Invalide mode:  " << modeString
         << endl;
   }
}

ossim_float64 ossimPixelFlipper::getClampValue() const
{
   return theClampValue;
}


ossim_float64 ossimPixelFlipper::getTargetValue() const
{
   return theTargetValue;
}

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
      case REPLACE_ALL_TARGETS:
         return ossimString("replace_all_targets");
      case REPLACE_PARTIAL_TARGETS:
         return ossimString("replace_partial_targets");
      case REPLACE_PARTIAL_TARGETS_ALL_BANDS:
         return ossimString("replace_partial_targets_all_bands");
      case REPLACE_FULL_TARGETS:
         return ossimString("replace_full_targets");
      default:
         break;
   }

   return ossimString("UNKNOWN_MODE");
}

ossimString ossimPixelFlipper::getClipModeString()  const
{
   switch(theClipMode)
   {
      case ossimPixelFlipperClipMode_NONE:
      {
         return ossimString("none");
      }
      case ossimPixelFlipperClipMode_BOUNDING_RECT:
      {
         return ossimString("bounding_rect");
      }
      case REPLACE_PARTIAL_TARGETS_ALL_BANDS:
      {
         return ossimString("valid_vertices");
      }
      default:
      {
         break;
      }
   }

   return ossimString("none");
}

ossimPixelFlipper::ClipMode ossimPixelFlipper::getClipMode() const
{
   return theClipMode;
}

std::ostream& ossimPixelFlipper::print(std::ostream& out) const
{
   out << "ossimPixelFlipper::print:"
       << "\ntarget value:       " << theTargetValue
       << "\nreplacement value:  " << theReplacementValue
       << "\nclamp value:        " << theClampValue
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

   if (name == TARGET_VALUE_KW)
   {
      ossimProperty* p =
         new ossimNumericProperty(name,
                                  ossimString::toString(theTargetValue));
      p->setCacheRefreshBit();
      return ossimRefPtr<ossimProperty>(p);
   }
   else if (name == REPLACEMENT_VALUE_KW)
   {
      ossimProperty* p =
         new ossimNumericProperty(name,
                                  ossimString::toString(theReplacementValue));
       p->setCacheRefreshBit();
      return ossimRefPtr<ossimProperty>(p);
   }
   else if (name == CLAMP_VALUE_KW)
   {
      ossimProperty* p =
         new ossimNumericProperty(name, ossimString::toString(theClampValue));
       p->setCacheRefreshBit();
      return ossimRefPtr<ossimProperty>(p);
   }
   else if (name == REPLACEMENT_MODE_KW)
   {
      vector<ossimString> constraintList(4);
      constraintList[0] = "replace_all_targets";
      constraintList[1] = "replace_partial_targets";
      constraintList[2] = "replace_partial_targets_all_bands";
      constraintList[3] = "replace_full_targets";
      
      ossimStringProperty* p =
         new ossimStringProperty(name,
                                 getReplacementModeString(),
                                 false,
                                 constraintList);
      p->setCacheRefreshBit();
      return ossimRefPtr<ossimProperty>(p);
   }
   else if (name == CLIP_MODE_KW)
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
   if (name == TARGET_VALUE_KW)
   {
      setTargetValue(os.toDouble());
   }
   else if  (name == REPLACEMENT_VALUE_KW)
   {
      setReplacementValue(os.toDouble());
   }
   else if  (name == CLAMP_VALUE_KW)
   {
      setClampValue(os.toDouble());
   }
   else if  (name == REPLACEMENT_MODE_KW)
   {
      setReplacementMode(os);
   }
   else if  (name == CLIP_MODE_KW)
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
   propertyNames.push_back(TARGET_VALUE_KW);
   propertyNames.push_back(REPLACEMENT_VALUE_KW);
   propertyNames.push_back(CLAMP_VALUE_KW);
   propertyNames.push_back(REPLACEMENT_MODE_KW);
   propertyNames.push_back(CLIP_MODE_KW);
   
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
