//-------------------------------------------------------------------
// License:  LGPL.  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
//-------------------------------------------------------------------
//  $Id$
#ifndef ossimTwoColorView_HEADER
#define ossimTwoColorView_HEADER 1
#include <ossim/base/ossimConstants.h>
#include <ossim/imaging/ossimImageCombiner.h>

/**
 * This is a 2 color view of the input.  It basically allows for a change detection and by default will take the first input and map
 * it to the blue channel and takes the second input and maps to the red channel.  The unused channel is mapped to a min pixel value.
 */
class OSSIM_DLL ossimTwoColorView : public ossimImageCombiner
{
public:
   ossimTwoColorView();
   virtual ossim_uint32 getNumberOfOutputBands() const;
   /**
    * @return the requested region of interest
    */
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& rect,
                                               ossim_uint32 resLevel=0);
   ossimScalarType getOutputScalarType() const;
   
   double getNullPixelValue(ossim_uint32 band)const;
   double getMinPixelValue(ossim_uint32 band)const;
   double getMaxPixelValue(ossim_uint32 band)const;
   
   /**
    * Will allow you to change the mapping of the new input and old input to the output band.  
    * Indexing from 0 based band numbering, by default the new channel (input 0) will be mapped to 
    * the output tile's blue channel (band 2) and the input 1 will be mapped to the red channel (band 0) 
    * and the green will have the value of min pix.  Use this method to change the new and old default output
    * mappings.
    */
   void setIndexMapping(ossim_uint32 newIndex,
                        ossim_uint32 oldIndex);
   virtual void initialize();
   
protected:
   void allocate();
   void runAlgorithm(ossimImageData* newData, ossimImageData* oldData);
   void runNative8(ossimImageData* newData,   ossimImageData* oldData);
   void runNorm(ossimImageData* newData,   ossimImageData* oldData);
   
   bool theByPassFlag;
   bool theNativeFlag;
   ossimRefPtr<ossimImageData> theTwoColorTile;
   ossimImageSource* theNewInput;
   ossimImageSource* theOldInput;
   ossim_uint32 theNewBufferDestinationIndex;
   ossim_uint32 theOldBufferDestinationIndex;
   ossim_uint32 theMinBufferDestinationIndex;

TYPE_DATA
};

#endif
