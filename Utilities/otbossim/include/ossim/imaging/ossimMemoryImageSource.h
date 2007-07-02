//*******************************************************************. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
//
#ifndef ossimMemoryImageSource_HEADER
#define ossimMemoryImageSource_HEADER

#include <ossim/base/ossimConstants.h>
#include <ossim/imaging/ossimImageSource.h>

class OSSIM_DLL ossimMemoryImageSource : public ossimImageSource
{
public:
   ossimMemoryImageSource();

   void setImage(ossimRefPtr<ossimImageData> image);
   virtual ossim_uint32 getNumberOfInputBands() const;
   virtual ossim_uint32 getNumberOfOutputBands() const;
   virtual ossimScalarType getOutputScalarType() const;
   virtual double getNullPixelValue(ossim_uint32 band=0)const;
   virtual double getMinPixelValue(ossim_uint32 band=0)const;
   virtual double getMaxPixelValue(ossim_uint32 band=0)const;

   virtual ossimIrect getBoundingRect(ossim_uint32 resLevel=0)const;
  
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& rect,
                                               ossim_uint32 resLevel=0);

   virtual bool canConnectMyInputTo(ossim_int32 myInputIndex,
                                    const ossimConnectableObject* object)const;
   
   virtual void initialize();
protected:
   ossimRefPtr<ossimImageData> theImage;
   ossimRefPtr<ossimImageData> theResult;
TYPE_DATA
};

#endif
