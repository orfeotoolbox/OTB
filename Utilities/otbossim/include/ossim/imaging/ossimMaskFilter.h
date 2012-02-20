//*******************************************************************
  // Copyright (C) 2000 ImageLinks Inc. 
  //
  // License:  LGPL
  // 
  // See LICENSE.txt file in the top level directory for more details.
  //
  // Author: Garrett Potts
  // Modified by: Elan Sharghi (1/20/2009)
  // Description: Class declaration for ossimMaskFilter.
  //
  //*************************************************************************
    // $Id: ossimMaskFilter.h 20409 2011-12-22 16:57:05Z dburken $
#ifndef  ossimMaskFilter_HEADER
#define  ossimMaskFilter_HEADER 1
#include <ossim/imaging/ossimImageSource.h>

    /**
     * <pre>
     * class ossimMaskFilter
     *
     *    Requires 2 inputs.  The first input is assumed to be the image input
     *    and the second input is assumed to be the mask data.  It will only
     *    use one band of the mask and multi band masks are not supported.
     *
     *    the number of bands, min pix, max pix, null pix ...etc are mapped
     *    to the first input.
     *
     * Keywords:
     *    mask_type:
     *
     * keywords description: This keyword can have the following values:
     *    select, select_clamp, invert, weighted, binary, or binary_inverse
     *         
     *    - select will use the input data and every where
     *      the mask is greater than 0 it will copy the input to the output.
     *
     *    - select_clamp_min If mask pixel is non zero, output pixel is input
     *      pixel; else, null pixel value. Differs from select in that output
     *      pixel is clamped to min pixel if input pixel is used. 
     *      
     *    - invert will use the input data and every where the mask is 0 it
     *      will copy the input to the output else it will place a null in
     *      the output.
     *      
     *    - weighted will normalize the mask between 0 and 1 and then multiply
     *      the input by that normalized value and copy to the output.
     *
     *    - binary If mask pixel is non zero, output pixel is max pixel value;
     *      else, null.
     *
     *    - binary_inverse If mask pixel is non zero, output pixel is max pixel
     *      value; else, null pixel value. 
     *    
     * example of keywords:
     *
     *      mask_type: select
     *
     * </pre>
     */
class OSSIMDLLEXPORT ossimMaskFilter : public ossimImageSource
{
public:
   /**
    * Enumeration used to identify what the selection type for this mask is to
    * be used:
    * 
    * - OSSIM_MASK_TYPE_SELECT
    *   If the mask is non zero then the input is kept if it's 0 then the input
    *   is nulled out.
    * - OSSIM_MASK_TYPE_INVERT
    *   If the input is non zero then the output is nulled else the input is kept
    * - OSSIM_MASK_TYPE_WEIGHTED
    *   works as a multiplier of the input data.  The mask is normalized to be
    *   between 0 and 1 and multiplies the input by that normalized value.
    * - OSSIM_MASK_TYPE_BINARY
    *   If mask pixel is non zero, output pixel is null pixel value; else, max
    *   pixel value.
    * - OSSIM_MASK_TYPE_BINARY_INVERSE
    *   If mask pixel is non zero, output pixel is max pixel value; else, null
    *   pixel value.
    * - OSSIM_MASK_TYPE_SELECT_CLAMP_MIN
    *   If mask pixel is non zero, output pixel is input pixel; else, null pixel
    *   value. Differs from OSSIM_MASK_TYPE_SELECT in that output pixel is
    *   clamped to min pixel if input pixel is used. The clampling has the
    *   affect of flipping null pixels to min pixel value if the mask pixel is
    *   non zero and input pixel is a null.
    */
  enum ossimFileSelectionMaskType
  {
     /**< standard select if mask is true then keep */
     OSSIM_MASK_TYPE_SELECT         = 1,
     /**< standard invert if mask is true the don't keep */
     OSSIM_MASK_TYPE_INVERT         = 2,
     /**< weighted operation.  Normalize the mask and multiply the input */
     OSSIM_MASK_TYPE_WEIGHTED       = 3,
     /**< binary image> */
     OSSIM_MASK_TYPE_BINARY         = 4,
     /**< inverse binary image> */
     OSSIM_MASK_TYPE_BINARY_INVERSE = 5,
     /**< Standard select if mask is true then keep with min. */
     OSSIM_MASK_TYPE_SELECT_CLAMP_MIN = 6,
  };

   /**
    * Default Constructor.
    */
   ossimMaskFilter(ossimObject* owner=NULL);

   /**
    * This set method is necessary when this object is being added to an ossimImageChain because
    * ossimImageChain::addLast() performs a disconnect of all the input sources, thus losing the
    * assignments made via constructor accepting source pointers. If the intent is to insert this
    * object in place of the image handler in a chain, First remove the handler from the chain, then
    * add a default-constructed mask filter object, then call this method to assign the inputs.
    */
   void setMaskSource(ossimImageSource* maskSource);

   /**
    * Sets the mask type.
    * @param type The mask algorithm type to be used.
    * see ossimFileSelectionMaskType
    */
   virtual void setMaskType(ossimFileSelectionMaskType type);
   
   /**
    * @brief Sets the mask type from string.
    *
    * Valid strings are:  "select", "invert" and "weighted".
    * 
    * @param type The mask algorithm type to be used.
    */
   virtual void setMaskType(const ossimString& type);

   /**
    * Returns the current mask algorithm used.
    * \return the current ossimFileSelectionMaskType used.
    */
   virtual ossimFileSelectionMaskType getMaskType() const;

   /**
    * @brief Returns the current mask algorithm used as a string.
    * @return the current ossimFileSelectionMaskType used.
    */
   virtual ossimString getMaskTypeString() const;

   /**
    * Main entry point for the algorithm.
    * \param rect 
    *
    */
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& rect,
                                               ossim_uint32 resLevel=0);

   virtual void initialize();
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
   
   virtual bool canConnectMyInputTo(ossim_int32 index, const ossimConnectableObject* object)const;
   
   virtual ossim_uint32 getNumberOfInputBands() const;
   

   virtual ossimRefPtr<ossimImageData> executeMaskFilter(
      ossimRefPtr<ossimImageData> imageSourceData,
      ossimRefPtr<ossimImageData> maskSourceData);

   /** @brief Interface to set the mask type. */
   virtual void setProperty(ossimRefPtr<ossimProperty> property);

   /** @return "mask_type" property. */
   virtual ossimRefPtr<ossimProperty> getProperty(
      const ossimString& name)const;

   /** @brief Adds "mask_type" to list. */
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

protected:
   /**
    * Will delete its owned tile.
    */
   virtual ~ossimMaskFilter();
   
   void allocate();
   
   /**
    * will execute the section algorithm.  everywhere the mask is not 0 it
    * copies the input data to the output and everywhere the input is null it
    * writes a null pixel to the output.
    */
   ossimRefPtr<ossimImageData> executeMaskFilterSelect(
      ossimRefPtr<ossimImageData> imageSourceData,
      ossimRefPtr<ossimImageData> maskSourceData);

   /**
    * will execute the invert selection algorithm.  everywhere the mask is 0
    * it copies the input data to the output else it outputs null.
    */
   ossimRefPtr<ossimImageData> executeMaskFilterInvertSelect(
      ossimRefPtr<ossimImageData> imageSourceData,
      ossimRefPtr<ossimImageData> maskSourceData);

   /**
    * will execute the weighted algorithm.  It normalizes the mask value to
    * be between 0 and 1 and multiplies the input data by that value and
    * outputs it.  
    */
   ossimRefPtr<ossimImageData> executeMaskFilterWeighted(
      ossimRefPtr<ossimImageData> imageSourceData,
      ossimRefPtr<ossimImageData> maskSourceData);

   /**
    * Will execute the binary algorithm. Copies the values in the mask to each 
    * tile. Values will be either NULL or MAX pixel value, typically 0 or 255.
    */
   ossimRefPtr<ossimImageData> executeMaskFilterBinary(
      ossimRefPtr<ossimImageData> imageSourceData,
      ossimRefPtr<ossimImageData> maskSourceData);
   
   template <class inputT, class maskT>
      ossimRefPtr<ossimImageData> executeMaskFilterSelection(
         inputT dummyInput,
         maskT  dummyMask,
         ossimRefPtr<ossimImageData> imageSourceData,
         ossimRefPtr<ossimImageData> maskSourceData);
   
   template <class inputT, class maskT>
      ossimRefPtr<ossimImageData> executeMaskFilterInvertSelection(
         inputT dummyInput,
         maskT  dummyMask,
         ossimRefPtr<ossimImageData> imageSourceData,
         ossimRefPtr<ossimImageData> maskSourceData);
   
   template <class inputT, class maskT>
      ossimRefPtr<ossimImageData> executeMaskFilterWeighted(
         inputT dummyInput,
         maskT  dummyMask,
         ossimRefPtr<ossimImageData> imageSourceData,
         ossimRefPtr<ossimImageData> maskSourceData);

   template <class inputT, class maskT>
   ossimRefPtr<ossimImageData> executeMaskFilterBinarySelection(
      inputT dummyInput,
      maskT  dummyMask,
      ossimRefPtr<ossimImageData> imageSourceData,
      ossimRefPtr<ossimImageData> maskSourceData);

   /**
    * Member variable that holds the algorithm type to run on the calls to getTile.
    */
   ossimFileSelectionMaskType  theMaskType;

   /**
    * Member used to store the result of the applied algorithm type.  This is returned
    * from the call to getTile.
    */
   ossimRefPtr<ossimImageData> theTile;
   ossimRefPtr<ossimImageSource> theMaskSource;

   TYPE_DATA
};

#endif /* #ifndef ossimMaskFilter_HEADER */
