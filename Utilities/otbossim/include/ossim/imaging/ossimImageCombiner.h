//*******************************************************************
//
// License:  See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimImageCombiner.h 17932 2010-08-19 20:34:35Z dburken $
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
   
   ossimImageCombiner(ossimConnectableObject::ConnectableObjectList& inputSources);

   virtual ossimIrect getBoundingRect(ossim_uint32 resLevel=0) const;

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
   
   
/*    virtual ossimRefPtr<ossimImageData> getNextTile(ossim_uint32& returnedIdx, */
/*                                                    ossim_uint32 startIdx, */
/*                                                    const ossimIpt& origin, */
/*                                                    ossim_uint32 resLevel=0); */
   
/*    virtual ossimRefPtr<ossimImageData> getNextTile(ossim_uint32& returnedIdx, */
/*                                                    const ossimIpt& origin, */
/*                                                    ossim_uint32 resLevel=0); */
   
   virtual ossimRefPtr<ossimImageData> getNextTile(ossim_uint32& returnedIdx,
                                                   const ossim_uint32 startIdx,
                                                   const ossimIrect& tileRect,
                                                   ossim_uint32 resLevel=0);

   virtual ossimRefPtr<ossimImageData> getNextTile(ossim_uint32& returnedIdx,
                                                   const ossimIrect& tileRect,
                                                   ossim_uint32 resLevel=0);

   
   virtual ossimRefPtr<ossimImageData> getNextNormTile(ossim_uint32& returnedIdx,
                                                       const ossim_uint32 index,
                                                       const ossimIrect& tileRect,
                                                       ossim_uint32 resLevel=0);
   
   virtual ossimRefPtr<ossimImageData> getNextNormTile(ossim_uint32& returnedIdx,
                                                       const ossimIrect& tileRect,
                                                       ossim_uint32 resLevel=0);
   
/*    virtual ossimRefPtr<ossimImageData> getNextNormTile(ossim_uint32& returnedIdx, */
/*                                                        ossim_uint32 index, */
/*                                                        const ossimIpt& origin, */
/*                                                        ossim_uint32 resLevel=0); */
/*    virtual ossimRefPtr<ossimImageData> getNextNormTile(ossim_uint32& returnedIdx, */
/*                                                        const ossimIpt& origin, */
/*                                                        ossim_uint32 resLevel=0); */
   
   
   virtual bool canConnectMyInputTo(ossim_int32 inputIndex,
                                    const ossimConnectableObject* object)const;
         
   virtual void connectInputEvent(ossimConnectionEvent& event);
   virtual void disconnectInputEvent(ossimConnectionEvent& event);
   virtual void propertyEvent(ossimPropertyEvent& event);
   virtual void refreshEvent(ossimRefreshEvent& event);
   virtual bool hasDifferentInputs()const;

   
protected:
   virtual ~ossimImageCombiner();   
   void precomputeBounds()const;

   ossim_uint32                theLargestNumberOfInputBands;
   ossim_uint32                theInputToPassThrough;
   bool                        theHasDifferentInputs;
   ossimRefPtr<ossimImageData> theNormTile;
   mutable std::vector<ossimIrect>     theFullResBounds;
   mutable bool                theComputeFullResBoundsFlag;
   ossim_uint32                theCurrentIndex;
   
TYPE_DATA  
};

#endif /* #ifndef ossimImageCombiner_HEADER */
