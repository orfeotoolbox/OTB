//----------------------------------------------------------------------------
// Copyright (c) 2005, David Burken, all rights reserved.
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Class definition of ossimWatermarkFilter.
// Applies an image or watermark to image.  Positioning is based on mode.
// Density is base on alpha weight.
//
//----------------------------------------------------------------------------
// $Id: ossimWatermarkFilter.cpp 15766 2009-10-20 12:37:09Z gpotts $

#include <vector>

#include <ossim/imaging/ossimWatermarkFilter.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimFilenameProperty.h>
#include <ossim/base/ossimNumericProperty.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimImageDataFactory.h>

#include <ossim/imaging/ossimScalarRemapper.h>


RTTI_DEF1(ossimWatermarkFilter, "ossimWatermarkFilter", ossimImageSourceFilter)

static ossimTrace traceDebug(ossimString("ossimWatermarkFilter:debug"));

// Keywords:
static const char WATERMARK_MODE_KW[] = "watermark_mode";
static const char WEIGHT_KW[]         = "weight";

#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimWatermarkFilter.cpp 15766 2009-10-20 12:37:09Z gpotts $";
#endif

const ossim_float64 DEFAULT_WEIGHT = 0.20;

ossimWatermarkFilter::ossimWatermarkFilter()
   :
   theFilename(ossimFilename::NIL),
   theWatermarkWeight(DEFAULT_WEIGHT),
   theTile(NULL),
   theWatermark(NULL),
   theMode(ossimWatermarkFilter::UPPER_LEFT),
   theInputBoundingRect(),
   theDirtyFlag(true)
{
   theEnableFlag = true;

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimPixelFlipper::ossimPixelFlipper" << std::endl;
#ifdef OSSIM_ID_ENABLED
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "OSSIM_ID:  " << OSSIM_ID << std::endl;
#endif      
   }
}

ossimWatermarkFilter::~ossimWatermarkFilter()
{
   // Dereference tiles and force a delete if last reference.
   theWatermark = NULL;
   theTile      = NULL;
}

ossimString ossimWatermarkFilter::getShortName() const
{
   return ossimString("Watermark Filter");
}

ossimString ossimWatermarkFilter::getLongName() const
{
   return ossimString("Watermark Filter - Applies watermark image onto getTile Request.");
}

ossimString ossimWatermarkFilter::getDescription() const
{
   ossimString description;
   description = getLongName();
   description += "\n";
   description += "Valid modes:\n";

   vector<ossimString> list;
   getModeList(list);

   vector<ossimString>::const_iterator i = list.begin();
   while (i != list.end())
   {
      description +=(*i);
      description += "\n";
      ++i;
   }

   return description;
}

ossimRefPtr<ossimImageData> ossimWatermarkFilter::getTile(
   const ossimIrect& tile_rect, ossim_uint32 resLevel)
{
   // Lock for the length of this method.
   // Check for input.
   if (!theInputConnection)
   {
      if (theTile.valid())
      {
         theTile->setImageRectangle(tile_rect);
         theTile->makeBlank();
      }
      return theTile;
   }

   // Fetch a tile from from the input source.
   ossimRefPtr<ossimImageData> inputTile =
      theInputConnection->getTile(tile_rect, resLevel);

   // Check for bypass.
   if (theEnableFlag == false) return inputTile;

   // Check for weight being 0.0.
   if (theWatermarkWeight == 0.0) return inputTile;

   //---
   // Check for dirty state.
   // Note: This is set in initialize if something changes.
   //---
   if (theDirtyFlag == true)
   {
      if (allocate() == false) // Something not right if false.
      {
         return inputTile;
      }
   }   
   
   // We will only watermark (process) within the input bounding rectangle.
   if (tile_rect.intersects(theInputBoundingRect) == false)
   {
      return inputTile;
   }

   // Capture the rectangle and blank out theTile.
   theTile->setImageRectangle(tile_rect);

   if (inputTile.valid() &&
       (inputTile->getDataObjectStatus() != OSSIM_NULL))
   {
      // Copy the inputTile to theTile.
      theTile->loadTile(inputTile.get());
   }
   else 
   {
      theTile->makeBlank();
   }

   // Write the watermarks...
   switch(theTile->getScalarType())
   {
      case OSSIM_UINT8:
      {
         fill(ossim_uint8(0));
         break;
      }
      case OSSIM_SINT8:
      {
         fill(ossim_sint8(0));
         break;
      }
      case OSSIM_USHORT11:
      case OSSIM_UINT16:
      {
         fill(ossim_uint16(0));
         break;
      }
      case OSSIM_SINT16:
      {
         fill(ossim_sint16(0));
         break;
      }
      case OSSIM_UINT32:
      {
         fill(ossim_uint32(0));
         break;
      }
      case OSSIM_SINT32:
      {
         fill(ossim_sint32(0));
         break;
      }
      case OSSIM_FLOAT32: 
      case OSSIM_NORMALIZED_FLOAT:
      {
         fill(ossim_float32(0));
         break;
      }
      case OSSIM_FLOAT64: 
      case OSSIM_NORMALIZED_DOUBLE:
      {
         fill(ossim_float32(0));
         break;
      }
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "Scalar type = " << theTile->getScalarType()
            << " Not supported by ossimWatermarkFilter" << std::endl;
         return inputTile;
      }
   }

   return theTile;
}

template <class T> void ossimWatermarkFilter::fill(T /* dummy */)
{
   const ossimIrect TILE_RECT = theTile->getImageRectangle();

   // We will only fill data within the input bounding rect.
   const ossimIrect CLIPPED_TILE_RECT =
      TILE_RECT.clipToRect(theInputBoundingRect);

   // Get the bounding rectangles.
   vector<ossimIrect> rects(0);
   getIntersectingRects(rects);

   if (rects.size() == 0)
   {
      return;
   }

   //---
   // Have watermark rectangles that intersect this tile so we need to process.
   //---
   ossim_uint32 band = 0;
   ossim_float64 inputPixWeight = 1.0 - theWatermarkWeight;

   // Get a pointers to the watermark buffers (wmBuf) and nulls wn.
   T** wmBuf = new T*[theWatermarkNumberOfBands];
   for (band = 0; band < theWatermarkNumberOfBands; ++band)
   {
      wmBuf[band] = static_cast<T*>(theWatermark->getBuf(band));
   }
   
   // Get a pointers to the output tile buffers and nulls in.
   T** otBuf = new T*[theInputNumberOfBands];
   for (band = 0; band < theInputNumberOfBands; ++band)
   {
      otBuf[band] = static_cast<T*>(theTile->getBuf(band));
   }

   // Get the width of the buffers for indexing.
   ossim_int32 wmWidth = static_cast<ossim_int32>(theWatermark->getWidth());
   ossim_int32 otWidth = static_cast<ossim_int32>(theTile->getWidth());

   const ossim_float64* wmNull = theWatermark->getNullPix();
   const ossim_float64* otMin  = theTile->getMinPix();
   const ossim_float64* otMax  = theTile->getMaxPix();
   const ossim_float64* otNull = theTile->getNullPix();
   
      
   // Control loop through intersecting rectangles.
   vector<ossimIrect>::const_iterator i = rects.begin();
   while (i != rects.end())
   {
      if ( (*i).intersects(CLIPPED_TILE_RECT) )
      {
         //---
         // This is the rectangle we want to fill relative to requesting
         // image space.
         //---
         const ossimIrect CLIPPED_WATERMARRK_RECT =
            (*i).clipToRect(CLIPPED_TILE_RECT);

         ossim_int32 clipHeight = CLIPPED_WATERMARRK_RECT.height();
         ossim_int32 clipWidth  = CLIPPED_WATERMARRK_RECT.width();

         // Compute the starting offset into the wmBuf and otBuf.
         ossim_int32 wmOffset =
            (CLIPPED_WATERMARRK_RECT.ul().y - (*i).ul().y) * wmWidth +
            CLIPPED_WATERMARRK_RECT.ul().x  - (*i).ul().x;
         ossim_int32 otOffset =
            (CLIPPED_WATERMARRK_RECT.ul().y - TILE_RECT.ul().y)* otWidth +
             CLIPPED_WATERMARRK_RECT.ul().x - TILE_RECT.ul().x;
         
         // Line loop...
         for (ossim_int32 line = 0; line < clipHeight; ++line)
         {
            // Sample loop...
            for (ossim_int32 sample = 0; sample < clipWidth; ++sample)
            {
               // Output band control loop until all output bands are filled.
               ossim_uint32 otBand = 0;
               while (otBand < theInputNumberOfBands)
               {
                  // Band loop through the watermark.
                  for (ossim_uint32 wmBand = 0;
                       wmBand < theWatermarkNumberOfBands;
                       ++wmBand)
                  {
                     if (wmBuf[wmBand][wmOffset+sample] != wmNull[wmBand])
                     {
                        // Apply the weight to the input pixel.
                        ossim_float64 p1 =
                           (otBuf[otBand][otOffset+sample] != otNull[otBand]) ?
                           otBuf[otBand][otOffset+sample] * inputPixWeight :
                           0.0;

                        // Apply the Weight to the watermark pixel.
                        ossim_float64 p2 =
                           wmBuf[wmBand][wmOffset+sample]*theWatermarkWeight;

                        // Add them up.
                        ossim_float64 p3 = p1 + p2;

                        // Cast to output type with range checking.
                        otBuf[otBand][otOffset+sample] = static_cast<T>(
                           ( (p3 >= otMin[otBand]) ?
                             (p3 < otMax[otBand] ? p3 : otMax[otBand]) :
                             otNull[otBand]) );
                     }
                     ++otBand;
                     
                     // We stop when we reach here.  All output bands filled.
                     if (otBand == theInputNumberOfBands)
                     {
                        break;
                     }
                     
                  } // End of band through watermark.
                  
               } // End of outer band loop.
               
            } // End of sample loop.

            wmOffset += wmWidth;
            otOffset += otWidth;
            
         } // End of line loop.

      } // End "if ( (*i).intersects(TILE_RECT) )"
      
      ++i; // Go to next rectangle to fill if any.
      
   } // End of "while (i != rects.end())"

   // Clean up.
   delete [] wmBuf;
   delete [] otBuf;
   
   theTile->validate();
}

void ossimWatermarkFilter::getIntersectingRects(vector<ossimIrect>& rects)
{
   switch(theMode)
   {
      case UPPER_LEFT:
         getUpperLeftRect(rects);
         break;
      case UPPER_CENTER:
         getUpperCenterRect(rects);
         break;
      case UPPER_RIGHT:
         getUpperRightRect(rects);
         break;
      case CENTER:
         getCenterRect(rects);
         break;
      case LOWER_LEFT:
         getLowerLeftRect(rects);
         break;
      case LOWER_CENTER:
         getLowerCenterRect(rects);
         break;
      case LOWER_RIGHT:
         getLowerRightRect(rects);
         break;
      case UNIFORM_DENSE:
         getUniformDenseRects(rects);
         break;
      case UNIFORM_SPARSE:
         getUniformSparceRects(rects);
         break;
      default:
         break;
   }
}

void ossimWatermarkFilter::getUpperLeftRect(vector<ossimIrect>& rects)
{
   // First clip the rect to the bounding image rectangle.
   const ossimIrect CLIP_RECT =
      theTile->getImageRectangle().clipToRect(theInputBoundingRect);
   theWatermark->setOrigin(theInputBoundingRect.ul());
   ossimIrect r = theWatermark->getImageRectangle();
   if (r.intersects(CLIP_RECT))
   {
      rects.push_back(r);
   }
}

void ossimWatermarkFilter::getUpperCenterRect(vector<ossimIrect>& rects)
{
   // First clip the rect to the bounding image rectangle.
   const ossimIrect CLIP_RECT =
      theTile->getImageRectangle().clipToRect(theInputBoundingRect);
   ossim_uint32 watermarkWidth = theWatermark->getImageRectangle().width();
   ossim_uint32 inputWidth = theInputBoundingRect.width();
   ossimIpt origin = theInputBoundingRect.ul();
   
   // Input wider than watermark so center.
   if (inputWidth > watermarkWidth)
   {
      ossim_int32 offset =
         static_cast<ossim_int32>((inputWidth - watermarkWidth) / 2);
      origin.x = origin.x + offset;
   }
   
   theWatermark->setOrigin(origin);
   
   ossimIrect r = theWatermark->getImageRectangle();
   if (r.intersects(CLIP_RECT))
   {
      rects.push_back(r);
   }
}  

void ossimWatermarkFilter::getUpperRightRect(vector<ossimIrect>& rects)
{
   // First clip the rect to the bounding image rectangle.
   const ossimIrect CLIP_RECT =
      theTile->getImageRectangle().clipToRect(theInputBoundingRect);
   ossim_uint32 watermarkWidth =
      theWatermark->getImageRectangle().width();
   ossim_uint32 inputWidth = theInputBoundingRect.width();
   
   ossimIpt origin = theInputBoundingRect.ul();
   
   // Input wider than watermark so center.
   if (inputWidth > watermarkWidth)
   {
      ossim_int32 offset =
         static_cast<ossim_int32>(inputWidth - watermarkWidth);
      origin.x = origin.x + offset;
   }
   
   theWatermark->setOrigin(origin);
   
   ossimIrect r = theWatermark->getImageRectangle();
   if (r.intersects(CLIP_RECT))
   {
      rects.push_back(r);
   }
}  

void ossimWatermarkFilter::getCenterRect(vector<ossimIrect>& rects)
{
   // First clip the rect to the bounding image rectangle.
   const ossimIrect CLIP_RECT =
      theTile->getImageRectangle().clipToRect(theInputBoundingRect);
   ossim_uint32 watermarkWidth = theWatermark->getImageRectangle().width();
   ossim_uint32 watermarkHeight = theWatermark->getImageRectangle().height();
   ossim_uint32 inputWidth = theInputBoundingRect.width();
   ossim_uint32 inputHeight = theInputBoundingRect.height();
   ossimIpt origin = theInputBoundingRect.ul();
   
   // Input wider than watermark so center.
   if (inputWidth > watermarkWidth)
   {
      ossim_int32 offset =
         static_cast<ossim_int32>((inputWidth - watermarkWidth) / 2);
      origin.x = origin.x + offset;
   }
   // Input higher than watermark so center.
   if (inputHeight > watermarkHeight)
   {
      ossim_int32 offset =
         static_cast<ossim_int32>((inputHeight - watermarkHeight) / 2);
      origin.y = origin.y + offset;
   }
   
   theWatermark->setOrigin(origin);
   
   ossimIrect r = theWatermark->getImageRectangle();
   if (r.intersects(CLIP_RECT))
   {
      rects.push_back(r);
   }
}

void ossimWatermarkFilter::getLowerLeftRect(vector<ossimIrect>& rects)
{
   // First clip the rect to the bounding image rectangle.
   const ossimIrect CLIP_RECT =
      theTile->getImageRectangle().clipToRect(theInputBoundingRect);
   ossim_uint32 watermarkHeight = theWatermark->getImageRectangle().height();
   ossim_uint32 inputHeight = theInputBoundingRect.height();
   ossimIpt origin = theInputBoundingRect.ul();
   
   // Input higher than watermark so apply offset.
   if (inputHeight > watermarkHeight)
   {
      ossim_int32 offset =
         static_cast<ossim_int32>(inputHeight - watermarkHeight);
      origin.y = origin.y + offset;
   }
   
   theWatermark->setOrigin(origin);
   
   ossimIrect r = theWatermark->getImageRectangle();
   if (r.intersects(CLIP_RECT))
   {
      rects.push_back(r);
   }
}

void ossimWatermarkFilter::getLowerCenterRect(vector<ossimIrect>& rects)
{
   // First clip the rect to the bounding image rectangle.
   const ossimIrect CLIP_RECT =
      theTile->getImageRectangle().clipToRect(theInputBoundingRect);
   ossim_uint32 watermarkWidth = theWatermark->getImageRectangle().width();
   ossim_uint32 watermarkHeight = theWatermark->getImageRectangle().height();
   ossim_uint32 inputWidth = theInputBoundingRect.width();
   ossim_uint32 inputHeight = theInputBoundingRect.height();
   ossimIpt origin = theInputBoundingRect.ul();
   
   // Input wider than watermark so center.
   if (inputWidth > watermarkWidth)
   {
      ossim_int32 offset =
         static_cast<ossim_int32>((inputWidth - watermarkWidth) / 2);
      origin.x = origin.x + offset;
   }
   // Input higher than watermark so apply offset.
   if (inputHeight > watermarkHeight)
   {
      ossim_int32 offset =
         static_cast<ossim_int32>(inputHeight - watermarkHeight);
      origin.y = origin.y + offset;
   }
   
   theWatermark->setOrigin(origin);
   ossimIrect r = theWatermark->getImageRectangle();
   if (r.intersects(CLIP_RECT))
   {
      rects.push_back(r);
   }
}

void ossimWatermarkFilter::getLowerRightRect(vector<ossimIrect>& rects)
{
   // First clip the rect to the bounding image rectangle.
   const ossimIrect CLIP_RECT =
      theTile->getImageRectangle().clipToRect(theInputBoundingRect);
   ossim_uint32 watermarkWidth = theWatermark->getImageRectangle().width();
   ossim_uint32 watermarkHeight = theWatermark->getImageRectangle().height();
   ossim_uint32 inputWidth = theInputBoundingRect.width();
   ossim_uint32 inputHeight = theInputBoundingRect.height();
   ossimIpt origin = theInputBoundingRect.ul();
   
   // Input wider than watermark so center.
   if (inputWidth > watermarkWidth)
   {
      ossim_int32 offset =
         static_cast<ossim_int32>(inputWidth - watermarkWidth);
      origin.x = origin.x + offset;
   }
   // Input higher than watermark so apply offset.
   if (inputHeight > watermarkHeight)
   {
      ossim_int32 offset =
         static_cast<ossim_int32>(inputHeight - watermarkHeight);
      origin.y = origin.y + offset;
   }
   
   theWatermark->setOrigin(origin);
   ossimIrect r = theWatermark->getImageRectangle();
   if (r.intersects(CLIP_RECT))
   {
      rects.push_back(r);
   }
   
}

void ossimWatermarkFilter::getUniformDenseRects(vector<ossimIrect>& rects)
{
   // First clip the rect to the bounding image rectangle.
   const ossimIrect CLIP_RECT =
      theTile->getImageRectangle().clipToRect(theInputBoundingRect);
   ossim_uint32 watermarkWidth = theWatermark->getImageRectangle().width();
   ossim_uint32 watermarkHeight = theWatermark->getImageRectangle().height();
   ossim_uint32 inputWidth = theInputBoundingRect.width();
   ossim_uint32 inputHeight = theInputBoundingRect.height();

   ossim_uint32 watermarksHigh = inputHeight / watermarkHeight;
   if (inputHeight % watermarkHeight) ++watermarksHigh;
   
   ossim_uint32 watermarksWide = inputWidth / watermarkWidth;
   if (inputWidth % watermarkWidth) ++watermarksWide;

   ossim_int32 xOffset = static_cast<ossim_int32>(watermarkWidth);
   ossim_int32 yOffset = static_cast<ossim_int32>(watermarkHeight);
   
   ossimIpt origin = theInputBoundingRect.ul();
   
   for (ossim_uint32 y = 0; y < watermarksHigh; ++y)
   {
      for (ossim_uint32 x = 0; x < watermarksWide; ++x)
      {
         theWatermark->setOrigin(origin);
         ossimIrect r = theWatermark->getImageRectangle();
         if (r.intersects(CLIP_RECT))
         {
            rects.push_back(r);
         }
         origin.x = origin.x + xOffset;
      }
      origin.y = origin.y + yOffset;
      origin.x = theInputBoundingRect.ul().x;
   }
}

void ossimWatermarkFilter::getUniformSparceRects(vector<ossimIrect>& rects)
{
   // First clip the rect to the bounding image rectangle.
   const ossimIrect CLIP_RECT =
      theTile->getImageRectangle().clipToRect(theInputBoundingRect);
   ossim_uint32 watermarkWidth  = theWatermark->getImageRectangle().width();
   ossim_uint32 watermarkHeight = theWatermark->getImageRectangle().height();
   ossim_uint32 inputWidth      = theInputBoundingRect.width();
   ossim_uint32 inputHeight     = theInputBoundingRect.height();
   ossim_uint32 gapWidth   = watermarkWidth/2;
   ossim_uint32 gapHeight  = watermarkHeight/2;

   ossim_uint32 watermarksHigh = inputHeight/(watermarkHeight+gapHeight);
   if (inputHeight % watermarkHeight) ++watermarksHigh;
   
   ossim_uint32 watermarksWide = inputWidth/(watermarkWidth+gapWidth);
   if (inputWidth % watermarkWidth) ++watermarksWide;

   ossim_int32 xOffset = static_cast<ossim_int32>(watermarkWidth  + gapWidth);
   ossim_int32 yOffset = static_cast<ossim_int32>(watermarkHeight + gapHeight);

   ossimIpt origin = theInputBoundingRect.ul();
   
   for (ossim_uint32 y = 0; y < watermarksHigh; ++y)
   {
      for (ossim_uint32 x = 0; x < watermarksWide; ++x)
      {
         theWatermark->setOrigin(origin);
         ossimIrect r = theWatermark->getImageRectangle();
         if (r.intersects(CLIP_RECT))
         {
            rects.push_back(r);
         }
         origin.x = origin.x + xOffset;
      }
      origin.y = origin.y + yOffset;
      origin.x = theInputBoundingRect.ul().x;
   }
}

void ossimWatermarkFilter::initialize()
{
   //---
   // If state is not already dirty and there is an input connection
   // check for:
   // 
   // 1) Scalar change
   // 2) band number change
   // 3) bounding box change
   //
   // Set state to dirty on a change.
   //
   // NOTE:  This method intentionally only sets the dirty state and doesn't do
   // anything else as it is called repetitively during chain setup or chain
   // state changes.
   //
   // The first getTile call will do the real work, call allocate(),
   // if the state is dirty.
   //---

   // Lock for the length of this method.
   // Set the input connection.
   ossimImageSourceFilter::initialize();

   // Once dirty flag is set no need to do it again.
   if (theDirtyFlag == false)
   {
      if (theInputConnection)
      {
         // Check for scalar type change.
         if (theInputScalarType != theInputConnection->getOutputScalarType())
         {
            theDirtyFlag = true;
            return;
         }

         // Check for band change.
         if (theInputNumberOfBands !=
             theInputConnection->getNumberOfOutputBands())
         {
            theDirtyFlag = true;
            return;
         }

         // Check for bounding rectangle change.
         if ( theInputBoundingRect != theInputConnection->getBoundingRect() )
         {
            theDirtyFlag = true;
            return;
         }
      }
   }
}

bool ossimWatermarkFilter::allocate()
{
   // Capture the bounding rect:
   theInputBoundingRect = theInputConnection->getBoundingRect();

   // Capture the scalar type:
   theInputScalarType = theInputConnection->getOutputScalarType();

   // Capture the number of bands:
   theInputNumberOfBands = theInputConnection->getNumberOfOutputBands();

   //---
   // Check the watermark scalar type.
   //---
   if (theWatermark.valid())
   {
      if (theWatermark->getScalarType() != theInputScalarType)
      {
         //---
         // We'll need to make a new one with a scalar remapper after it.
         //---
         theWatermark = NULL; // We'll need to make a new one.
      }
   }

   //---
   // Make a new watermark tile.  This will do a scalar remap if needed.
   // If we don't have a watermark no point in going on...
   //---
   if (!theWatermark)
   {
      if (openWatermarkFile() == false)
      {
         return false;
      }
   }

   if (theTile.valid())
   {
      if ( (theTile->getScalarType()    != theInputScalarType) ||
           (theTile->getNumberOfBands() != theInputNumberOfBands) )
      {
         theTile = NULL;  // We'll need to make a new one.
      }
   }

   // Make a new output tile if we need to.
   if (!theTile)
   {
      theTile = ossimImageDataFactory::instance()->create(this,
                                                          theInputConnection);
      if (theTile.valid())
      {
         theTile->initialize();
      }
      else
      {
         return false;
      }
   }

   //---
   // If we get here things are good so clear the dirty flag so we don't
   // get called again needlessly.
   //---
   theDirtyFlag = false;
   
   return true;
}

bool ossimWatermarkFilter::openWatermarkFile()
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimWatermarkFilter::openWatermarkFile DEBUG: entered..."
         << std::endl;
   }

   if (!theInputConnection || (theFilename == ossimFilename::NIL))
   {
      return false;
   }

   theWatermark = NULL; // This will destroy any previous tiles.

   // Open the watermark image.
   ossimRefPtr<ossimImageHandler> ih =
      ossimImageHandlerRegistry::instance()->open(theFilename);
   if (!ih)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimWatermarkFilter::openWatermarkFile"
            << "\nCould not open: " << theFilename 
            << std::endl;
      }
      
      return false;
   }
   if (ih->getErrorStatus() == ossimErrorCodes::OSSIM_ERROR)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimWatermarkFilter::openWatermarkFile"
            << "\nError reading image:  " << theFilename
            << std::endl; 
         return false;
      }
   }

   ih->initialize();
   ossimRefPtr<ossimImageSource> imageSource = ih.get();
   ossimRefPtr<ossimScalarRemapper> remapper;
   
   if (ih->getOutputScalarType() != theInputConnection->getOutputScalarType())
   {
      // Remap the watemark to the same scalar type as the input.
      remapper = new ossimScalarRemapper(imageSource.get(),
                                         theInputConnection->
                                         getOutputScalarType());
      remapper->initialize();
      imageSource = remapper.get();
   }
   
   // Get the full image rectangle.
   theWatermark = imageSource->getTile(ih->getImageRectangle(), 0);

   // Cleanup...
   if (remapper.valid())
   {
      remapper->disconnect();
      remapper = NULL;
   }
   if(ih.valid())
   {
      ih->disconnect();
      ih = 0;
   }
   imageSource = 0;
   
   if (theWatermark.valid() == false)
   {
      return false;
   }

   // Capture the bands as we will need this repetitively.
   theWatermarkNumberOfBands = theWatermark->getNumberOfBands();

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimWatermarkFilter::openWatermarkFile DEBUG:"
         << *(theWatermark.get())
         << endl;
   }

   return true;
}

bool ossimWatermarkFilter::saveState(ossimKeywordlist& kwl,
                                     const char* prefix) const
{
   kwl.add(prefix,
           ossimKeywordNames::FILENAME_KW,
           theFilename.c_str());
   kwl.add(prefix,
           WATERMARK_MODE_KW,
           getModeString().c_str());
   kwl.add(prefix,
           WEIGHT_KW,
           theWatermarkWeight);

   return ossimImageSourceFilter::saveState(kwl, prefix);
}

bool ossimWatermarkFilter::loadState(const ossimKeywordlist& kwl,
                                     const char* prefix)
{
   // Do this first so connections get set up.
   if (ossimImageSourceFilter::loadState(kwl, prefix) == false)
   {
      return false;
   }

   const char* lookupReturn;

   lookupReturn = kwl.find(prefix, WEIGHT_KW);
   if(lookupReturn)
   {
      setWeight(ossimString(lookupReturn).toDouble());
   }

   lookupReturn = kwl.find(prefix, WATERMARK_MODE_KW);
   if(lookupReturn)
   {
      setMode(ossimString(lookupReturn));
   }

   lookupReturn = kwl.find(prefix, ossimKeywordNames::FILENAME_KW);
   if(lookupReturn)
   {
      setFilename(lookupReturn);
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimWatermarkFilter::loadState DEBUG:"
         << std::endl;
      print(ossimNotify(ossimNotifyLevel_DEBUG));
   }

   return true;
}

ossimRefPtr<ossimProperty> ossimWatermarkFilter::getProperty(
   const ossimString& name) const
{
   // Lock for the length of this method.
   if (name == ossimKeywordNames::FILENAME_KW)
   {
      ossimFilenameProperty* ofp =
         new ossimFilenameProperty(name, theFilename);
      ofp->setIoType(ossimFilenameProperty::ossimFilenamePropertyIoType_INPUT);
      ofp->setCacheRefreshBit();
      return ossimRefPtr<ossimProperty>((ossimProperty*)ofp);
   }
   else if (name == WATERMARK_MODE_KW)
   {
      vector<ossimString> constraintList;
      getModeList(constraintList);

      ossimStringProperty* p =
         new ossimStringProperty(name,
                                 getModeString(),
                                 false,
                                 constraintList);
       p->setCacheRefreshBit();
      return ossimRefPtr<ossimProperty>(p);
   }
   else if (name == WEIGHT_KW)
   {
      ossimProperty* p =
         new ossimNumericProperty(name,
                                  ossimString::toString(theWatermarkWeight));
      p->setCacheRefreshBit();
      return ossimRefPtr<ossimProperty>(p);
   }

   return ossimImageSourceFilter::getProperty(name);
}

void ossimWatermarkFilter::setProperty(ossimRefPtr<ossimProperty> property)
{
   if (!property) return;

   ossimString os = property->valueToString();
   
   ossimString name = property->getName();
   if (name == ossimKeywordNames::FILENAME_KW)
   {
      setFilename(os);
   }
   else if  (name == WATERMARK_MODE_KW)
   {
      setMode(os);
   }
   else if  (name == WEIGHT_KW)
   {
      setWeight(os.toDouble());
   }
   else
   {
      ossimImageSourceFilter::setProperty(property);
   }
}

void ossimWatermarkFilter::getPropertyNames(
   std::vector<ossimString>& propertyNames) const
{
   propertyNames.push_back(ossimKeywordNames::FILENAME_KW);
   propertyNames.push_back(WATERMARK_MODE_KW);
   propertyNames.push_back(WEIGHT_KW);
   ossimImageSourceFilter::getPropertyNames(propertyNames);
}

void ossimWatermarkFilter::getModeList(vector<ossimString>& list) const
{
   list.clear();
   list.resize(ossimWatermarkFilter::END);
   list[0] = ossimString("upper_left");
   list[1] = ossimString("upper_center");
   list[2] = ossimString("upper_right");
   list[3] = ossimString("center");
   list[4] = ossimString("lower_left");
   list[5] = ossimString("lower_center");
   list[6] = ossimString("lower_right");
   list[7] = ossimString("uniform_dense");
   list[8] = ossimString("uniform_sparse");
}

ossimWatermarkFilter::WatermarkMode ossimWatermarkFilter::getMode() const
{
   return theMode;
}

ossimString ossimWatermarkFilter::getModeString() const
{
   switch(theMode)
   {
      case UPPER_LEFT:
         return ossimString("upper_left");
      case UPPER_CENTER:
         return ossimString("upper_center");
      case UPPER_RIGHT:
         return ossimString("upper_right");
      case CENTER:
         return ossimString("center");
      case LOWER_LEFT:
         return ossimString("lower_left");
      case LOWER_CENTER:
         return ossimString("lower_center");
      case LOWER_RIGHT:
         return ossimString("lower_right");
      case UNIFORM_DENSE:
         return ossimString("uniform_dense");
      case UNIFORM_SPARSE:
         return ossimString("uniform_sparse");
      default:
         break;
   }

   return ossimString("UNKNOWN_MODE");
}

void ossimWatermarkFilter::setFilename(const ossimFilename& file)
{
   if (file != theFilename)
   {
      theFilename = file;
      theWatermark = NULL; // Will be reallocated next getTile.
      theDirtyFlag = true;
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimWatermarkFilter::setFilename DEBUG:" << std::endl;
      print(ossimNotify(ossimNotifyLevel_DEBUG));
   }
}

void ossimWatermarkFilter::setMode(const ossimString& mode)
{
   ossimString os = mode;
   os.downcase();
   
   if (os == "upper_left")
   {
      theMode = UPPER_LEFT;
   }
   else if (os == "upper_center")
   {
      theMode = UPPER_CENTER;
   }
   else if (os == "upper_right")
   {
      theMode = UPPER_RIGHT;
   }
   else if (os == "center")
   {
      theMode = CENTER;
   }
   else if (os == "lower_left")
   {
      theMode = LOWER_LEFT;
   }
   else if (os == "lower_center")
   {
      theMode = LOWER_CENTER;
   }
   else if (os == "lower_right")
   {
      theMode = LOWER_RIGHT;
   }
   else if (os == "uniform_dense")
   {
      theMode = UNIFORM_DENSE;
   }
   else if (os == "uniform_sparse")
   {
      theMode = UNIFORM_SPARSE;
   }
   else
   {
      // Invalid mode...
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimWatermarkFilter::setMode DEBUG:"
            << "\nInvalid mode!  " << mode
            << std::endl;
      }
   }
}

void ossimWatermarkFilter::setWeight(ossim_float64 weight)
{
   if ( (weight >= 0.0) && (weight <= 1.0) )
   {
      theWatermarkWeight = weight;
   }
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimWatermarkFilter::setWeight DEBUG:" << std::endl;
      print(ossimNotify(ossimNotifyLevel_DEBUG));
   }
   
}

std::ostream& ossimWatermarkFilter::print(std::ostream& out) const
{
   out << "ossimWatermarkFilter::print"
       << "\ntheFilename:        " << theFilename
       << "\ntheWatermarkWeight: " << theWatermarkWeight
       << "\ntheMode:            " << getModeString()
       << std::endl;
   return ossimImageSourceFilter::print(out);
}

