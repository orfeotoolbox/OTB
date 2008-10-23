//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
// Description: A brief description of the contents of the file.
//
//*************************************************************************
// $Id: ossimMaskFilter.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef  ossimMaskFilter_HEADER
#define  ossimMaskFilter_HEADER
#include <ossim/imaging/ossimImageCombiner.h>

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
 *      mask_type:
 *
 * keywords description
 *      mask_type  This keyword can have the following values:
 *                 select, invert, or weighted.
 *                  - select will use the input data and every where
 *                    the mask is greater than 0 it will copy the input to the output.
 *                  - invert will use the input data and every where the mask is 0 it
 *                    will copy the input to the output else it will place a null in
 *                    the output.
 *                  - weighted will normalize the mask between 0 and 1 and then multiply
 *                    the input by that normalized value and copy to the output.
 *
 * example of keywords:
 *
 *      mask_type: select
 *
 * </pre>
 */
class OSSIMDLLEXPORT ossimMaskFilter : public ossimImageCombiner
{
public:
   /**
    * Enumeration used to identify what the selection type for this mask is to be
    *  A list of events:
    *    - OSSIM_MASK_TYPE_SELECT
    *      is used as a true false mask.  If the mask is non zero
    *      then the input is kept if it's 0 then the input is nulled out.
    *    - OSSIM_MASK_TYPE_INVERT
    *      has the inverted rule of the above.  If the input is tru then the output is nulled
    *      else the input is kept
    *    - OSSIM_MASK_TYPE_WEIGHTED
    *      works as a multiplier of the input data.  The mask is normalized to be between 0 and 1
    *      and multiplies the input by that normalized value.
    */
  enum ossimFileSelectionMaskType
  {
     OSSIM_MASK_TYPE_SELECT   = 1, /**< standard select if mask is true then keep */
     OSSIM_MASK_TYPE_INVERT   = 2, /**< standard invert if mask is true the don't keep */
     OSSIM_MASK_TYPE_WEIGHTED = 3  /**< weighted operation.  Normalize the mask and multiply the input */
  };

   /**
    * Constructor.
    */
  ossimMaskFilter(ossimObject* owner=NULL);

   /**
    * Constructs with two inputs
    */
  ossimMaskFilter(ossimImageSource* imageSource,
		  ossimImageSource* maskSource);

   
   /**
    * Constructs with two inputs and an owner
    */
  ossimMaskFilter(ossimObject* owner,
		  ossimImageSource* imageSource,
		  ossimImageSource* maskSource);

   /**
    * Will delete its owned tile.
    */
   virtual ~ossimMaskFilter();

   /**
    * Sets the mask type.
    * \param type The mask algorithm type to be used.  see ossimFileSelectionMaskType
    */
   virtual void setMaskType(ossimFileSelectionMaskType type)
      {
         theMaskType = type;
      }

   /**
    * Returns the current mask algorithm used.
    * \return the current ossimFileSelectionMaskType used.
    */
   virtual ossimFileSelectionMaskType getMaskType()const
      {
         return theMaskType;
      }

   /**
    * Returns the bounding rect in pixel space for this object.
    * \param resLevel optional res level argument is given.
    * \return The bounding rect.  It will pass the inputs bounding
    *         rect since it does not update the bounds.
    */
   virtual ossimIrect getBoundingRect(ossim_uint32 resLevel=0)const;

   /**
    * Main entry point for the algorithm.
    * \param rect 
    *
    */
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& rect,
                                   ossim_uint32 resLevel=0);

   virtual void initialize();
   virtual void getDecimationFactor(ossim_uint32 resLevel,
                                    ossimDpt& result)const;
   virtual void getDecimationFactors(vector<ossimDpt>& decimations)const;

   virtual ossim_uint32 getNumberOfDecimationLevels()const;

   virtual ossim_uint32 getNumberOfOutputBands() const;
   
   virtual ossimScalarType getOutputScalarType() const;

   virtual void getValidImageVertices(vector<ossimIpt>& validVertices,
                                      ossimVertexOrdering ordering=OSSIM_CLOCKWISE_ORDER,
                                      ossim_uint32 resLevel=0)const;

   virtual ossim_uint32 getTileWidth() const;

   virtual ossim_uint32 getTileHeight() const;

   virtual double getNullPixelValue(ossim_uint32 band)const;
   
   virtual double getMinPixelValue(ossim_uint32 band=0)const;
   
   virtual double getMaxPixelValue(ossim_uint32 band=0)const;
   
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
   
   virtual bool canConnectMyInputTo(ossim_int32 index,
                                    const ossimConnectableObject* object)const;
   
   virtual ossimRefPtr<ossimImageData> executeMaskFilter(
      ossimRefPtr<ossimImageData> imageSourceData,
      ossimRefPtr<ossimImageData> maskSourceData);
   
protected:
   
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

   /**
    * Member variable that holds the algorithm type to run on the calls to getTile.
    */
   ossimFileSelectionMaskType  theMaskType;

   /**
    * Member used to store the result of the applied algorithm type.  This is returned
    * from the call to getTile.
    */
   ossimRefPtr<ossimImageData> theTile;
   
TYPE_DATA
};

#endif
