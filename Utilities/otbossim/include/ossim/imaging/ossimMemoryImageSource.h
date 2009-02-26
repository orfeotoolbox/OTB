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
   void setImage(ossimScalarType scalarType,
                 ossim_uint32 numberOfBands,
                 ossim_uint32 width,
                 ossim_uint32 height);
   void setRect(ossim_uint32 ulx,
                ossim_uint32 uly,
                ossim_uint32 width,
                ossim_uint32 height);
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
   virtual void setImageGeometry(const ossimKeywordlist& kwl)
   {
      theImageGeometry = kwl;
   }
   virtual bool getImageGeometry(ossimKeywordlist& kwl,
                                 const char* prefix=0)
   {
      kwl.add(prefix, theImageGeometry);
      return (theImageGeometry.getSize() > 0);
   }
	virtual ossim_uint32 getNumberOfDecimationLevels() const;
   virtual void getDecimationFactor(ossim_uint32 resLevel,
                                    ossimDpt& result) const;
  
protected:
   ossimRefPtr<ossimImageData> theImage;
   ossimRefPtr<ossimImageData> theResult;
   ossimKeywordlist theImageGeometry;
	ossimIrect theBoundingRect;
TYPE_DATA
};

#endif
