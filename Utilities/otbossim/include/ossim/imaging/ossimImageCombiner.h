//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimImageCombiner.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimImageCombiner_HEADER
#define ossimImageCombiner_HEADER
#include <vector>

#include <ossim/imaging/ossimImageSource.h>
#include <ossim/base/ossimConnectableObjectListener.h>
#include <ossim/base/ossimPropertyEvent.h>

/**
 * This will be a base for all combiners.  Combiners take N inputs and
 * will produce a single output.
 */
class OSSIMDLLEXPORT ossimImageCombiner : public ossimImageSource,
                                          public ossimConnectableObjectListener
{
public:
   ossimImageCombiner();
   ossimImageCombiner(ossimObject* owner,
                      int numberOfInputs,
                      int numberOfOutputs,
                      bool inputListIsFixedFlag,
                      bool outputListIsFixedFlag);
   
   ossimImageCombiner(const std::vector<ossimImageSource*>& inputSources);
   virtual ~ossimImageCombiner();   

   virtual void getDecimationFactor(ossim_uint32 resLevel,
                                    ossimDpt& result) const;
   virtual void getDecimationFactors(vector<ossimDpt>& decimations) const;
   virtual ossim_uint32 getNumberOfDecimationLevels() const;

   virtual ossimIrect getBoundingRect(ossim_uint32 resLevel=0) const;

   virtual bool getImageGeometry(ossimKeywordlist& kwl,
                                 const char* prefix=NULL);

   virtual void initialize();
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=NULL);
   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix=NULL)const;

   /**
    * This will go through and precompute the
    * bounding rects of each input image.
    */
   virtual void updateRects();

   /**
    * Returns the number of bands available from the input.
    */
   virtual ossim_uint32 getNumberOfInputBands()const;   
   virtual ossimScalarType getOutputScalarType() const;
   virtual ossim_uint32 getTileWidth()const;  
   virtual ossim_uint32 getTileHeight()const;

   virtual double getNullPixelValue(ossim_uint32 band=0)const;
   virtual double getMinPixelValue(ossim_uint32 band=0)const;
   virtual double getMaxPixelValue(ossim_uint32 band=0)const;

  /**
   * Used to retrieve the number of overlapping images withint the given
   * rect.
   */
  virtual ossim_uint32 getNumberOfOverlappingImages(const ossimIrect& rect,
                                                    ossim_uint32 resLevel=0)const;

  /**
   * Used to populate the result with the index of the overlapping images.
   */
  virtual void getOverlappingImages(std::vector<ossim_uint32>& result,
				    const ossimIrect& rect,
                                    ossim_uint32 resLevel=0)const;
   
   virtual ossimRefPtr<ossimImageData> getNextTile(ossim_uint32 index,
                                                   const ossimIpt& origin,
                                                   ossim_uint32 resLevel=0);
   
   virtual ossimRefPtr<ossimImageData> getNextTile(const ossimIpt& origin,
                                                   ossim_uint32 resLevel=0);
   
   virtual ossimRefPtr<ossimImageData> getNextTile(ossim_uint32 index,
                                                   const ossimIrect& origin,
                                                   ossim_uint32 resLevel=0);

   virtual ossimRefPtr<ossimImageData> getNextTile(const ossimIrect& origin,
                                                   ossim_uint32 resLevel=0);
   
   virtual ossimRefPtr<ossimImageData> getNextNormTile(ossim_uint32 index,
                                                       const ossimIpt& origin,
                                                       ossim_uint32 resLevel=0);
   virtual ossimRefPtr<ossimImageData> getNextNormTile(const ossimIpt& origin,
                                                       ossim_uint32 resLevel=0);
   
   virtual ossimRefPtr<ossimImageData> getNextNormTile(ossim_uint32 index,
                                                       const ossimIrect& origin,
                                                       ossim_uint32 resLevel=0);
   virtual ossimRefPtr<ossimImageData> getNextNormTile(const ossimIrect& origin,
                                                       ossim_uint32 resLevel=0);
   
   virtual bool canConnectMyInputTo(ossim_int32 inputIndex,
                                    const ossimConnectableObject* object)const;
         
   virtual void connectInputEvent(ossimConnectionEvent& event);
   virtual void disconnectInputEvent(ossimConnectionEvent& event);
   virtual void propertyEvent(ossimPropertyEvent& event);
   virtual void refreshEvent(ossimRefreshEvent& event);
   virtual bool hasDifferentInputs()const;
   
protected:
   ossim_uint32                theLargestNumberOfInputBands;
   ossim_uint32                theInputToPassThrough;
   bool                        theHasDifferentInputs;
   ossimRefPtr<ossimImageData> theNormTile;
   ossim_int32                 theCurrentIndex;
   mutable std::vector<ossimIrect>     theFullResBounds;
   mutable bool                theComputeFullResBoundsFlag;
   void precomputeBounds()const;
   
TYPE_DATA  
};

#endif /* #ifndef ossimImageCombiner_HEADER */
