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

   /**
    * @brief Enumerations for mapping inputs to red, green and blue
    * output channels.
    */
   enum ossimTwoColorMultiViewOutputSource
   {
      UNKNOWN = 0,
      MIN     = 1,
      OLD     = 2,
      NEW     = 3
   };
   
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
    * @brief Sets which bands to use from inputs, and which inputs to use for
    * output red, green and blue channels.
    *
    * This method does no error checking as the inputs may or may not be set
    * prior to this call.  ossimTwoColorView::initialize() will verify once
    * inputs are established.  So if an input band is out of range band
    * 0 (first band) will be used.
    *
    * Callers should ensure initialize is called after this.
    *
    * @param oldInputBandIndex Zero based band index for old input. Default=0
    * * 
    * @param newInputBandIndex Zero based band index for new input. Default=0.
    * 
    * @param redOutputSource default=OLD.  Can be:
    *    ossimTwoColorMultiViewOutputSource::OLD
    *    ossimTwoColorMultiViewOutputSource::NEW
    *    ossimTwoColorMultiViewOutputSource::MIN
    *    
    * @param grnOutputSource default=NEW.  Can be:
    *    ossimTwoColorMultiViewOutputSource::OLD
    *    ossimTwoColorMultiViewOutputSource::NEW
    *    ossimTwoColorMultiViewOutputSource::MIN
    *    
    * @param bluOutputSource default=NEW.  Can be:
    *    ossimTwoColorMultiViewOutputSource::OLD
    *    ossimTwoColorMultiViewOutputSource::NEW
    *    ossimTwoColorMultiViewOutputSource::MIN   
    */
   void setBandIndexMapping(
      ossim_uint32 oldInputBandIndex,
      ossim_uint32 newInputBandIndex,
      ossimTwoColorMultiViewOutputSource redOutputSource,
      ossimTwoColorMultiViewOutputSource grnOutputSource,
      ossimTwoColorMultiViewOutputSource bluOutputSource);

   virtual void initialize();
   
protected:
   void allocate();
   void runAlgorithm(ossimImageData* newData, ossimImageData* oldData);
   void runNative8(ossimImageData* newData,   ossimImageData* oldData);
   void runNorm(ossimImageData* newData,   ossimImageData* oldData);
   
   bool                               m_byPassFlag;
   bool                               m_nativeFlag;
   ossimRefPtr<ossimImageData>        m_twoColorTile;
   ossimRefPtr<ossimImageSource>      m_newInput;
   ossimRefPtr<ossimImageSource>      m_oldInput;
   ossim_uint32                       m_newInputBandIndex;
   ossim_uint32                       m_oldInputBandIndex;
   ossimTwoColorMultiViewOutputSource m_redSource;
   ossimTwoColorMultiViewOutputSource m_grnSource;
   ossimTwoColorMultiViewOutputSource m_bluSource;

TYPE_DATA
};

#endif /* #ifndef ossimTwoColorView_HEADER */
