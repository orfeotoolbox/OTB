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
// $Id: ossimPixelFlipper.h 19728 2011-06-06 21:31:17Z dburken $
#ifndef ossimPixelFlipper_HEADER
#define ossimPixelFlipper_HEADER

#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/base/ossimPolygon.h>
#include <OpenThreads/ReentrantMutex>

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
class OSSIM_DLL ossimPixelFlipper : public ossimImageSourceFilter
{
public:
   static const char PF_TARGET_VALUE_KW[];     
   static const char PF_TARGET_RANGE_KW[];     
   static const char PF_REPLACEMENT_VALUE_KW[];
   static const char PF_REPLACEMENT_MODE_KW[];
   static const char PF_CLAMP_VALUE_KW[];
   static const char PF_CLAMP_VALUE_LO_KW[];   
   static const char PF_CLAMP_VALUE_HI_KW[];   
   static const char PF_CLIP_MODE_KW[];

   /**
    * Target Replacement Mode:
    * 
    * Examples given for 3-band pixel values as (R, G, B) with target = 0, and replacement = 1

    * If mode is REPLACE_BAND_IF_TARGET (default):
    * Any pixel band with value of target will be replaced.
    * (0, 0, 0) becomes (1, 1, 1)
    * (0, 3, 2) becomes (1, 3, 2)
    *
    * If mode is REPLACE_BAND_IF_PARTIAL_TARGET:
    * A band with target value will be replaced only if at least one other band in the pixel does 
    * not have the target.
    * (0, 0, 0) remains (0, 0, 0)
    * (0, 3, 2) becomes (1, 3, 2)
    *
    * If mode is REPLACE_ALL_BANDS_IF_PARTIAL_TARGET:
    * All bands of the pixel will be replaced if any but not all bands in the pixel have the
    * target value.
    * (0, 0, 0) remains (0, 0, 0)
    * (0, 3, 2) becomes (1, 1, 1)
    *
    * If mode is REPLACE_ONLY_FULL_TARGETS:
    * All bands in the pixel will be replaced only if they all have the target.
    * (0, 0, 0) becomes (1, 1, 1)
    * (0, 3, 2) remains (0, 3, 2)
    *
    * If mode is REPLACE_ALL_BANDS_IF_ANY_TARGET:
    * All bands in the pixel will be replaced if even one band has the target.
    * (0, 0, 0) becomes (1, 1, 1)
    * (0, 3, 2) remains (1, 1, 1)
    */
   enum ReplacementMode
   {
      REPLACE_BAND_IF_TARGET               = 0,
      REPLACE_BAND_IF_PARTIAL_TARGET       = 1,
      REPLACE_ALL_BANDS_IF_PARTIAL_TARGET  = 2,
      REPLACE_ONLY_FULL_TARGETS            = 3,
      REPLACE_ALL_BANDS_IF_ANY_TARGET      = 4,
   };

   /** 
    * When either a lo and/or hi clamp value is set, the clamping mode will be enabled accordingly
    * and override any target replacement defined
    */
   enum ClampingMode
   {
      DISABLED                           = 0,
      CLAMPING_LO                        = 1,
      CLAMPING_HI                        = 2,
      CLAMPING_LO_AND_HI                 = 3,
   };

   enum ClipMode
   {
      NONE = 0,
      BOUNDING_RECT  = 1,
      VALID_VERTICES = 2
   };

   /** default constructor */
   ossimPixelFlipper(ossimObject* owner=NULL);


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
   virtual ossim_float64 getMinPixelValue (ossim_uint32 band = 0 ) const;
      
   virtual std::ostream& print(std::ostream& out) const;

   /** 
    * @param target_value This is the value to flip. 
    * @note If clamping is specified, it will take precedence over any target value (or range) test
    */
   void setTargetValue(ossim_float64 target_value);

   /** 
    * Instead of a single value for a target, this method allows for specifying a range of values
    * to flip to the replacement. The replacement mode is still referenced.
    * @param  This is the value to flip. 
    * @note If clamping is specified, it will take precedence over any target range test.
    */
   void setTargetRange(ossim_float64 target_min, ossim_float64 target_max);

   /**
    * @param replacement_value This is the value to flip target to.
    * @note If clamping is specified, it will take precedence over any target replacement.
    */
 void setReplacementValue(ossim_float64 replacement_value);

   /**
    * @param clamp_value If set all pixel values above this range will (or below if clamp_max_value
    * = false) be clamped to clamp_value. Must be less than max pixel (or greater than the min 
    * pixel) value of the input and cannot be null. 
    * @note If any clamp limit is defined, it will take precedence over any target value (or range)
    * replacement. The replacement mode is referenced when deciding whether a pixel should be 
    * clamped or left alone.
    */
   void setClampValue(ossim_float64 clamp_value, bool is_high_clamp_value=true);
   void setClampValues(ossim_float64 clamp_value_lo, ossim_float64 clamp_value_hi);

   /** @see enum ReplacementMode */
   void setReplacementMode(ossimPixelFlipper::ReplacementMode mode);

   /** Accepts a string that must match the enumerator's label (can be lower case) and sets the 
    * replacement mode accordingly. If the string is not understood, the mode remains unchanged and
    * FALSE is returned. */
   bool setReplacementMode(const ossimString& modeString);

   /** 
    * Clipping here refers to bounding rect or valid polygon (spacial) clipping, where all pixels
    * outside the valid area are mapped to the replacement value.
    */
   void setClipMode(const ossimString& modeString);
   void setClipMode(ClipMode mode);

   //ossim_float64 getTargetValue()      const;
   ossim_float64 getReplacementValue() const;
   //ossim_float64 getClampValue() const;
   ossimPixelFlipper::ReplacementMode getReplacementMode()  const;
   ossimString getReplacementModeString() const;
   ossimString getClipModeString() const;
   ClipMode getClipMode() const;

   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
   
   //! This object can be used outside of an image chain for offline processing of existing tile.
   template <class T> void flipPixels(T dummy, ossimImageData* inpuTile, ossim_uint32 resLevel);

protected:
   /** destructor */
   virtual ~ossimPixelFlipper();
   template <class T> void clipTile(T dummy,
                                    ossimImageData* inpuTile,
                                    ossim_uint32 resLevel);

   /**
    * Verifies pixel is in range.
    * @return Returns true if in range else false.
    */
   bool inRange(ossim_float64 value) const;

   void allocateClipTileBuffer(ossimRefPtr<ossimImageData> inputImage);
   
   /** The value range to replace. For a single value replacement, both Lo and Hi are equal. Any
    * pixel within this range will be remapped to the replacement value */
   ossim_float64 theTargetValueLo;
   ossim_float64 theTargetValueHi;

    /** When target values are defined, this is the value the pixel will assume if the pixel falls 
     *  within the target range (according to the rules for replacement mode) */
   ossim_float64 theReplacementValue;
   ReplacementMode theReplacementMode; //!< See documentation for ReplacementMode enum above

  /** The range of desired pixel values. Any pixels outside this range are set to the corresponding
    * clamp value. Note that theReplacementValue is not referenced when clamping. */
   ossim_float64 theClampValueLo;
   ossim_float64 theClampValueHi;
   ClampingMode  theClampingMode;

   /**
    * Border Clip mode
    *
    * This will flip to nulls any pixel value outside the specified mode.
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
   mutable OpenThreads::ReentrantMutex      theMutex;

   mutable std::vector<ossimPolygon> theValidVertices;
   mutable std::vector<ossimIrect>   theBoundingRects;

   ossimRefPtr<ossimImageData> theClipTileBuffer;
   
   TYPE_DATA
};


#endif
