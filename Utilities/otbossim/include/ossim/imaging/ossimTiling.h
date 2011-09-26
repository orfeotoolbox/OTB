//*******************************************************************
// Copyright (C) 2004 Garrett Potts
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
// Description: implementation for image generator
//
//*************************************************************************
// $Id: ossimTiling.h 20103 2011-09-17 16:10:42Z dburken $
#ifndef ossimTiling_HEADER
#define ossimTiling_HEADER
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/base/ossimRefPtr.h>

class OSSIM_DLL ossimTiling : public ossimObject
{
public:

   enum ossimTilingDeltaType
   {
      ossimTilingDeltaType_TOTAL_PIXELS,
      ossimTilingDeltaType_PER_PIXEL
   };

   ossimTiling();

   bool initialize(const ossimMapProjection& proj,
                   const ossimIrect& boundingRect);

   bool next(ossimRefPtr<ossimMapProjection>& resultProjection,
             ossimIrect& resultingBounds,
             ossimString& resultingName)const;
   
   void reset();
   
   /**
    * 
    * @param tilingDistance Distance in the specified units.
    * @param unitType Currently only angular unit degrees,
    *                 and linear unit meters are supported.
    *                 If unitType is pixel unit then the
    *                 delta per unit will be set to 1.
    *                 
    */
   void setTilingDistance(const ossimDpt& tilingDistance,
                          ossimUnitType unitType);

   /**
    * This sets the pixel delta.  The delta can either be TOTAL_PIXELS or
    * UNIT_PER_PIXEL.
    *
    * If the delta type is TOTAL_PIXELS then the unit per pixel is calculated
    * from the theTilingDistance.  This is very useful when wanting to be on
    * even geographic boundaries and would like to have a fixed width and
    * height for
    * the pixels.  Flight simulators use this tiling scheme for power of 2
    * outputs.
    *
    * if the delta type is unit per pixel then the total pixels is computed
    * form the
    * theTilingDistance.  This is useful when you might want whole number
    * GSD's and whole numbered tile boundaries.
    */
   void setDelta(const ossimDpt& delta,
                 ossimTilingDeltaType deltaType);

   /**
    * This sets the tilename mask.  The key tokens that are supported are.
    *
    * %r%         Replaces with the row number of the current tile
    * %c%         Replaces with the column number of the current tile
    * %i%         Replaces with the current tile id.
    * %or%        Replaces with the origin of the row
    * %oc%        Replaces with the origin of the col
    * %SRTM%      Replaces all filenames with the SRTM file name convention
    *
    * 
    * Examples:
    *   tile%r%_%c%  assume r = 0 and c = 100 then
    *              this is replaced with tile0_100
    *   
    */
   void setTileNameMask(const ossimString& mask);
   ossimString getTileNameMask()const;
   
   bool getRowCol(ossim_int64& row,
                  ossim_int64& col,
                  ossim_int64 tileId)const;
   bool getOrigin(ossimDpt& origin,
                  ossim_int64 tileId)const;
   bool getOrigin(ossimDpt& origin,
                  ossim_int64 row,
                  ossim_int64 col)const;
   
   void setPaddingSizeInPixels(const ossimIpt& pizelOverlap);
   ossimDpt getDeltaPerPixel()const;
   
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   virtual std::ostream& print(std::ostream& out) const;

protected:

   /**
    * @return The tiling distance converted to either degrees or meters.
    */
   void getConvertedTilingDistance(ossimDpt& pt) const;

   /** The size of a tile in theTilingDistanceUnitType. */
   ossimDpt             theTilingDistance;

   /** The unit type for "theDelta", and "theTilingDistance". */
   ossimUnitType        theTilingDistanceUnitType;

   /**
    * If (theDeltaType == ossimTilingDeltaType_TOTAL_PIXELS) then
    * this is the size of the tile in pixels like 1024 x 1024.
    *
    * If (theDeltaType == ossimTilingDeltaType_PER_PIXEL) then
    * this is the size of a single pixel like 1.0 meter.
    */
   ossimDpt             theDelta;

   /** @see theDelta */
   ossimTilingDeltaType theDeltaType;

   /**
    * @brief If set tiling size is controlled by the desired output size in
    * bytes.  Set by one of output_size_in_bytes, output_size_in_kilo_bytes, or
    * output_size_in_mega_bytes.  Always stored in bytes.
    *
    * @note kilo_byte = 1024 X 1024, mega_byte = 1024 X 1024 X 1024.
    *
    * @note If set this relys on theNumberOfBands and theNumberOfBytesPerPixel
    * need to alse be set correctly.
    */
   ossim_int64 theOutputSizeInBytes;

   /**
    * @brief Output number of bands.  Needed to tile by output size in bytes.
    *
    * default = 1
    *
    * @see theOutputSizeInBytes
    */
   ossim_uint32 theNumberOfBands;

   /**
    * @brief Output number of bytes per pixel.
    * Needed to tile by output size in bytes.
    *
    * default = 1
    *
    * @see theOutputSizeInBytes
    *
    * @note This bytes per single pixel, not all bands.
    */
   ossim_uint32 theNumberOfBytesPerPixelPerBand;
   
   ossimDpt             thePaddingSizeInPixels;
   mutable ossimRefPtr<ossimMapProjection>  theMapProjection;
   ossimIrect                       theImageRect;
   ossimDrect                       theTilingRect;
   mutable ossim_int64 theTileId;
   ossim_int64         theTotalHorizontalTiles;
   ossim_int64         theTotalVerticalTiles;
   ossim_int64         theTotalTiles;
   ossimString         theTileNameMask;

   /**
    * This flags the code to shift the tie point so that the edge of the pixel
    * falls on the tile boundary.  default=false
    */
   bool theEdgeToEdgeFlag; 
   
   bool validate()const;
   bool isAngularUnit(ossimUnitType unitType)const;
   bool isLinearUnit(ossimUnitType unitType)const;
   void getTileName(ossimString& resultingName,
                    ossim_int64 row,
                    ossim_int64 col,
                    ossim_int64 id)const;
   void clampGeographic(ossimDrect& rect)const;
                    
TYPE_DATA
};

#endif
