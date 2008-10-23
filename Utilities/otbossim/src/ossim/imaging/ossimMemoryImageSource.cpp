//*******************************************************************. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
//
#include <ossim/imaging/ossimMemoryImageSource.h>
RTTI_DEF1(ossimMemoryImageSource, "ossimMemoryImageSource", ossimImageSource);

ossimMemoryImageSource::ossimMemoryImageSource()
   :ossimImageSource(0, 0, 1, true, false)
{
   theBoundingRect.makeNan();
}

void ossimMemoryImageSource::setImage(ossimRefPtr<ossimImageData> image)
{
   theImage = image;
	if(theImage.valid())
	{
		theBoundingRect = theImage->getImageRectangle();
	}
	else
	{
		theBoundingRect.makeNan();
	}
}

ossim_uint32 ossimMemoryImageSource::getNumberOfInputBands() const
{
   if(theImage.valid())
   {
      return theImage->getNumberOfBands();
   }
   return 0;;
}

ossim_uint32 ossimMemoryImageSource::getNumberOfOutputBands() const
{
   return getNumberOfInputBands();
}

ossimScalarType ossimMemoryImageSource::getOutputScalarType() const
{
   if(theImage.valid())
   {
      return theImage->getScalarType();
   }
   
   return ossimImageSource::getOutputScalarType();
}

double ossimMemoryImageSource::getNullPixelValue(ossim_uint32 band)const
{
   if(theImage.valid())
   {
      if(band < getNumberOfInputBands())
      {
         return theImage->getNullPix(band);
      }
   }
   return ossimImageSource::getNullPixelValue(band);
}

double ossimMemoryImageSource::getMinPixelValue(ossim_uint32 band)const
{
   if(theImage.valid())
   {
      if(band < getNumberOfInputBands())
      {
         return theImage->getMinPix(band);
      }
   }
   return ossimImageSource::getMinPixelValue(band);
}

double ossimMemoryImageSource::getMaxPixelValue(ossim_uint32 band)const
{
   if(theImage.valid())
   {
      if(band < getNumberOfInputBands())
      {
         return theImage->getMaxPix(band);
      }
   }
   return ossimImageSource::getMaxPixelValue(band);
}

ossimIrect ossimMemoryImageSource::getBoundingRect(ossim_uint32 resLevel)const
{
   if(resLevel == 0)
   {
      if(theImage.valid())
      {
         return theBoundingRect;
      }
   }

   return ossimImageSource::getBoundingRect(resLevel);
}

ossimRefPtr<ossimImageData> ossimMemoryImageSource::getTile(const ossimIrect& rect,
                                                      ossim_uint32 resLevel)
{
   if(!theImage.valid()) return 0;
   if(theBoundingRect.hasNans()) return 0;
   if(!theResult.valid())
   {
      theResult = new ossimImageData(0, getOutputScalarType(), getNumberOfOutputBands(), rect.width(), rect.height());
      theResult->initialize();       
   }

   theResult->setImageRectangle(rect);
   theResult->makeBlank();
   ossimIrect clampRect = theImage->getImageRectangle().clipToRect(rect);
	
   theResult->loadTile(theImage->getBuf(),
	                    theBoundingRect,
	                    OSSIM_BSQ);

	theResult->validate();
   return theResult;
}


bool ossimMemoryImageSource::canConnectMyInputTo(ossim_int32 myInputIndex,
                                                 const ossimConnectableObject* object)const
{
   return false;
}
   
void ossimMemoryImageSource::initialize()
{
}
