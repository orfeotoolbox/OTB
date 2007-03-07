//*******************************************************************
// Copyright (C) 2002 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
//
// Class to scan pixels and flip target dn pixel value to new dn pixel value.
// This was written to fix partial null pixels.
// 
//*************************************************************************
// $Id: ossimPixelFlipper.h,v 1.16 2005/12/09 12:43:39 gpotts Exp $
#ifndef ossimPixelFlipper_HEADER
#define ossimPixelFlipper_HEADER

#include <imaging/tile_sources/ossimImageSourceFilter.h>
#include <base/data_types/ossimPolygon.h>
#include <base/common/ossimMutex.h>

/**
 *  Class to scan pixels and flip target dn value to a replacement dn
 *  value.
 *
 *  This was written to fix problems with null pixels, i.e. an image has a
 *  digital number(dn) of 255 for null and it is desired to use a dn of 0 as
 *  a null.  This can really be used to flip any pixel value to another.
 *
 *  @note This filter currently works on the input tile directly and does
 *  not copy the data.
 *
 *  @see theReplacementMode data member documentation for more info.
 */
class ossimPixelFlipper : public ossimImageSourceFilter
{
public:
   
   enum ReplacementMode
   {
      /**
       * Any pixel with dn of target will be replaced. (default)
       */
      REPLACE_ALL_TARGETS                = 0,
      
      /**
       * Target will be replaced only at least one subpixel(band) does not have
       * the target.
       */
      REPLACE_PARTIAL_TARGETS            = 1,

      /**
       * All bands will be replaced if any band has target but not all.
       */
      REPLACE_PARTIAL_TARGETS_ALL_BANDS  = 2,

      /**
       * Target will be repaced only if all subpixels(bands) have the target.
       */
      REPLACE_FULL_TARGETS               = 3
   };
   enum ClipMode
   {
      ossimPixelFlipperClipMode_NONE = 0,
      ossimPixelFlipperClipMode_BOUNDING_RECT  = 1,
      ossimPixelFlipperClipMode_VALID_VERTICES = 2
   };
   /** default constructor */
   ossimPixelFlipper(ossimObject* owner=NULL);

   /** destructor */
   virtual ~ossimPixelFlipper();

   /** @return "Pixel flipper" as an ossimString. */
   virtual ossimString getShortName()const;

   /** Initializes the state of the object from theInputConnection. */
   virtual void initialize();

   /**
    * @param tile_rect Rectangle to fill tile with.
    * 
    * @param resLevel Reduced resolution level to grab from.
    * 
    * @return ossimRefPtr<ossimImageData> This is tile that was filled with
    * tile_rect.
    *
    * @note Callers should check the ossimRefPtr::valid method.
    * The internal pointer of the ossimRefPtr<ossimImageData> can be
    * null if the tile_rect did not intersect the input connection's
    * bounding rectangle.
    */
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tile_rect,
                                               ossim_uint32 resLevel=0);

   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
   
   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   
   virtual ossimScalarType getOutputScalarType() const;
   virtual ossim_float64 getMaxPixelValue (ossim_uint32 band = 0 ) const;
      
   virtual std::ostream& print(std::ostream& out) const;

   /** @param target_value This is the value to flip. */
   void setTargetValue(ossim_float64 target_value);

   /** @param replacement_value This is the value to flip target to. */
   void setReplacementValue(ossim_float64 replacement_value);


   /** @see enum ReplacementMode */
   void setReplacementMode(ossimPixelFlipper::ReplacementMode mode);
   void setReplacementMode(const ossimString& modeString);

   void setClipMode(const ossimString& modeString);
   void setClipMode(ossimPixelFlipper::ClipMode mode);
   
   /**
    * @param clamp_value If set all pixel values above this range will
    * be clamped to clamp_value.  Must be less than max pixel value of the
    * input and cannot be null.
    */
   void setClampValue(ossim_float64 clamp_value);

   ossim_float64 getTargetValue()      const;
   ossim_float64 getReplacementValue() const;
   ossim_float64 getClampValue() const;
   ossimPixelFlipper::ReplacementMode getReplacementMode()  const;
   ossimString getReplacementModeString() const;
   ossimString getClipModeString() const;
   ossimPixelFlipper::ClipMode getClipMode() const;

   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
   
protected:
   template <class T> void flipPixels(T dummy,
                                      ossimImageData* inpuTile,
                                      ossim_uint32 resLevel);

   template <class T> void clipTile(T dummy,
                                    ossimImageData* inpuTile,
                                    ossim_uint32 resLevel);

   /**
    * Verifies pixel is in range.
    * @return Returns true if in range else false.
    */
   bool inRange(ossim_float64 value) const;

   void allocateClipTileBuffer(ossimRefPtr<ossimImageData> inputImage);
   /** The value to replace. */
   ossim_float64 theTargetValue;

   /** The replacement value. */
   ossim_float64 theReplacementValue;

   /**
    * If set and not > max pixel values any values higher than theClampValue
    * will be clamped to theClampValue.
    */
   ossim_float64 theClampValue;

   
   /**
    * Target Replacement Mode:
    * 
    * If mode is REPLACE_ALL_TARGETS (default):
    * Any pixel with dn of target will be replaced.
    *
    * If mode is REPLACE_PARTIAL_TARGETS:
    * Target will be replaced only at least one subpixel(band) does not have
    * the target.
    *
    * If mode is REPLACE_BANDS_PARTIAL_TARGETS_ALL_BANDS:
    * All bands will be replaced if any band has target but not all.
    *
    * If mode is REPLACE_FULL_TARGETS:
    * Target will be repaced only if all subpixels(bands) have the target.
    *
    * Example:
    * target      = 0
    * replacement = 1
    * Pixel at (0, 0) r=0, g=0,  b=0
    * Pixel at (0, 1) r=0, g=30, b=21
    * 
    * Mode is REPLACE_ALL_TARGETS:
    * Pixel at (0, 0) becomes  r=1, g=1,  b=1
    * Pixel at (0, 1) becomes  r=1, g=30, b=21
    * 
    * Mode is REPLACE_PARTIAL_TARGETS:
    * Pixel at (0, 0) remains  r=0, g=0,  b=0
    * Pixel at (0, 1) becomes  r=1, g=30, b=21
    *
    * Mode is REPLACE_PARTIAL_TARGETS_ALL_BANDS:
    * Pixel at (0, 0) remains  r=0, g=0,  b=0
    * Pixel at (0, 1) becomes  r=1, g=1,  b=1
    *
    * Mode is REPLACE_FULL_TARGETS:
    * Pixel at (0, 0) becomes  r=1, g=1,  b=1
    * Pixel at (0, 1) remains  r=0, g=30, b=21
    *
    * Default mode is REPLACE_ALL_TARGETS.
    */
   ReplacementMode theReplacementMode;

   /**
    * Clip mode
    *
    * This will flip nulls any pixel value outside the specified mode.
    *
    * Valid modes are:
    *
    * none
    * bounding_rect
    * valid_vertices
    *
    * if the mode is "none" then nothing is done.
    * if the mode is "bounding_rect" then the bounding rect for the requested rlevel
    *                is used and every pixel outside that   
    */
   ClipMode        theClipMode;
   
   /** For lock and unlock. */
   mutable ossimMutex      theMutex;

   mutable std::vector<ossimPolygon> theValidVertices;
   mutable std::vector<ossimIrect>   theBoundingRects;

   ossimRefPtr<ossimImageData> theClipTileBuffer;
   
   TYPE_DATA
};


#endif
