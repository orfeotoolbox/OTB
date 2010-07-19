//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimFftFilter.cpp 17206 2010-04-25 23:20:40Z dburken $

#include <ossim/imaging/ossimFftFilter.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/matrix/newmat.h>
#include <ossim/matrix/newmatap.h>
#include <ossim/imaging/ossimScalarRemapper.h>
#include <ossim/base/ossimStringProperty.h>

RTTI_DEF1(ossimFftFilter, "ossimFftFilter", ossimImageSourceFilter);

ossimFftFilter::ossimFftFilter(ossimObject* owner)
   :ossimImageSourceFilter(owner),
    theTile(0),
    theDirectionType(ossimFftFilterDirectionType_FORWARD),
    theScalarRemapper(new ossimScalarRemapper())
{
   theScalarRemapper->setOutputScalarType(OSSIM_DOUBLE);
}

ossimFftFilter::ossimFftFilter(ossimImageSource* inputSource)
   :ossimImageSourceFilter(inputSource),
    theTile(0),
    theDirectionType(ossimFftFilterDirectionType_FORWARD),
    theScalarRemapper(new ossimScalarRemapper())
{
   theScalarRemapper->setOutputScalarType(OSSIM_DOUBLE);
}

ossimFftFilter::ossimFftFilter(ossimObject* owner,
                               ossimImageSource* inputSource)
   :ossimImageSourceFilter(owner, inputSource),
    theTile(0),
    theDirectionType(ossimFftFilterDirectionType_FORWARD),
    theScalarRemapper(new ossimScalarRemapper())
{
   theScalarRemapper->setOutputScalarType(OSSIM_DOUBLE);
}

ossimFftFilter::~ossimFftFilter()
{
   if(theScalarRemapper.valid())
   {
      theScalarRemapper->disconnect();
      theScalarRemapper = 0;
   }
}

ossimRefPtr<ossimImageData> ossimFftFilter::getTile(const ossimIrect& rect,
                                                    ossim_uint32 resLevel)
{
   if(!isSourceEnabled())
   {
      return ossimImageSourceFilter::getTile(rect, resLevel);
   }
   
   ossimIrect tempRequest = rect;

   ossim_uint32 w = rect.width();
   ossim_uint32 h = rect.height();
   
   if(w & 1)
   {
      ++w;
   }
   if(h&1)
   {
      ++h;
   }

   tempRequest = ossimIrect(rect.ul().x,
                            rect.ul().y,
                            rect.ul().x + (w-1),
                            rect.ul().y + (h-1));
   
   ossimRefPtr<ossimImageData> input;
   input  = theScalarRemapper->getTile(tempRequest, resLevel);

   if(!input.valid()) return input;
   
   if(!theTile.valid()) initialize();
   if(!theTile.valid()) return theTile;
   
   theTile->setImageRectangle(rect);
   ossimRefPtr<ossimImageData> tempTile = theTile;
   
   if(!input->getBuf())
   {
      return theTile;
   }

   if(rect != tempRequest)
   {
      tempTile = (ossimImageData*)theTile->dup();
      tempTile->setImageRectangle(tempRequest);
   }

   runFft((ossim_float64)0, input, tempTile);
          
   if(tempTile != theTile)
   {
      theTile->loadTile(tempTile.get());
   }
   
   theTile->validate();

   return theTile;
}

void ossimFftFilter::initialize()
{
   ossimImageSourceFilter::initialize();

   theTile = ossimImageDataFactory::instance()->create(this, this);
   
   if(theTile.valid())
   {
      theTile->initialize();
   }
   if(theDirectionType == ossimFftFilterDirectionType_FORWARD)
   {
      theScalarRemapper->setOutputScalarType(OSSIM_NORMALIZED_DOUBLE);
   }
   else
   {
      theScalarRemapper->setOutputScalarType(OSSIM_DOUBLE);
   }
   theScalarRemapper->connectMyInputTo(0, getInput());
}

ossimScalarType ossimFftFilter::getOutputScalarType() const
{
   if(!isSourceEnabled())
   {
      return ossimImageSourceFilter::getOutputScalarType();
   }
   
   if(theDirectionType == ossimFftFilterDirectionType_FORWARD)
   {
      return OSSIM_DOUBLE;
   }
   
   return OSSIM_NORMALIZED_DOUBLE;
}

double ossimFftFilter::getNullPixelValue(ossim_uint32 band)const
{
   if(!isSourceEnabled())
   {
      return ossimImageSourceFilter::getNullPixelValue(band);
   }
   if(theDirectionType == ossimFftFilterDirectionType_FORWARD)
   {
      return ossim::nan();
   }

   // it will invert to a normalized float output
   return 0.0;
}

double ossimFftFilter::getMinPixelValue(ossim_uint32 band)const
{
   if(!isSourceEnabled())
   {
      return ossimImageSourceFilter::getMinPixelValue(band);
   }
   if(theDirectionType == ossimFftFilterDirectionType_FORWARD)
   {
      return OSSIM_DEFAULT_MIN_PIX_DOUBLE;
   }
   return OSSIM_DEFAULT_MIN_PIX_NORM_DOUBLE;
}

double ossimFftFilter::getMaxPixelValue(ossim_uint32 band)const
{
   if(!isSourceEnabled())
   {
      return ossimImageSourceFilter::getMaxPixelValue(band);
   }
   if(theDirectionType == ossimFftFilterDirectionType_FORWARD)
   {
      return OSSIM_DEFAULT_MAX_PIX_DOUBLE;
   }
   return OSSIM_DEFAULT_MAX_PIX_NORM_DOUBLE;
}

ossim_uint32 ossimFftFilter::getNumberOfOutputBands() const
{
   if(!isSourceEnabled())
   {
      return ossimImageSourceFilter::getNumberOfOutputBands();
   }
   ossim_uint32 bands = ossimImageSourceFilter::getNumberOfOutputBands();
   
   if(theDirectionType == ossimFftFilterDirectionType_FORWARD)
   {
      bands *=2;
   }
   else 
   {
      bands /=2;
   }

   return bands;
}

void ossimFftFilter::setForward()
{
   theDirectionType = ossimFftFilterDirectionType_FORWARD;
}

void ossimFftFilter::setInverse()
{
   theDirectionType = ossimFftFilterDirectionType_INVERSE;
}

ossimString ossimFftFilter::getDirectionTypeAsString()const
{
   if(theDirectionType == ossimFftFilterDirectionType_FORWARD)
   {
      return "Forward";
   }

   return "Inverse";
}

void ossimFftFilter::setDirectionType(const ossimString& directionType)
{
   ossimString tempDirectionType = directionType;
   tempDirectionType = tempDirectionType.downcase();
   
   if(tempDirectionType.contains("forward"))
   {
      setDirectionType(ossimFftFilterDirectionType_FORWARD);
   }
   else
   {
      setDirectionType(ossimFftFilterDirectionType_INVERSE);
   }
}

void ossimFftFilter::setDirectionType(ossimFftFilterDirectionType directionType)
{
   theDirectionType = directionType;
   if(theTile.valid())
   {
      theTile = NULL;
   }
}

ossimRefPtr<ossimProperty> ossimFftFilter::getProperty(const ossimString& name)const
{
   if(name == "FFT Direction")
   {
      std::vector<ossimString> filterNames;
      filterNames.push_back("Forward");
      filterNames.push_back("Inverse");
      ossimStringProperty* stringProp = new ossimStringProperty("FFT Direction",
								getDirectionTypeAsString(),
								false,
								filterNames);
      stringProp->setCacheRefreshBit();

      return stringProp;
   }

   return ossimImageSourceFilter::getProperty(name);
}

void ossimFftFilter::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(!property) return;
   
   if(property->getName() == "FFT Direction")
   {
      if(theTile.valid())
      {
         theTile = NULL;
      }
      setDirectionType(property->valueToString());
   }
   else
   {
      ossimImageSourceFilter::setProperty(property);
   }
}

void ossimFftFilter::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimImageSourceFilter::getPropertyNames(propertyNames);
   propertyNames.push_back("FFT Direction");
}


template <class T>
void ossimFftFilter::runFft(T /* dummy */,
                            ossimRefPtr<ossimImageData>& input,
                            ossimRefPtr<ossimImageData>& output)
{
   // T*             inputBand  = 0;

   NEWMAT::Matrix* realIn = new NEWMAT::Matrix(input->getHeight(),
                                               input->getWidth());
   NEWMAT::Matrix* imgIn = new NEWMAT::Matrix(input->getHeight(),
                                              input->getWidth());
   NEWMAT::Matrix* realOut = new NEWMAT::Matrix(input->getHeight(),
                                                input->getWidth());
   NEWMAT::Matrix* imgOut = new NEWMAT::Matrix(input->getHeight(),
                                               input->getWidth());
   ossim_uint32 bandIdx = 0;
   ossim_uint32 w = input->getWidth();
   ossim_uint32 h = input->getHeight();
   ossim_uint32 x = 0;
   ossim_uint32 y = 0;
   if(theDirectionType == ossimFftFilterDirectionType_FORWARD)
   {
      ossim_uint32 bands = input->getNumberOfBands();
      for(bandIdx = 0; bandIdx < bands; ++bandIdx)
      {
         ossim_float64* bandReal = 0;
         ossim_float64* bandImg  = 0;
         fillMatrixForward((T*)input->getBuf(bandIdx),
                           (T)input->getNullPix(bandIdx),
                           *realIn,
                           *imgIn);
         NEWMAT::FFT2(*realIn, *imgIn, *realOut, *imgOut);
         bandReal = (ossim_float64*)output->getBuf(2*bandIdx);
         bandImg  = (ossim_float64*)output->getBuf(2*bandIdx + 1);
         if(bandReal&&bandImg)
         {
            for(y = 0; y < h; ++y)
            {
               for(x = 0; x < w; ++x)
               {
                  *bandReal = (ossim_float64)((*realOut)[y][x]);
                  *bandImg  = (ossim_float64)((*imgOut)[y][x]);
                  ++bandReal;
                  ++bandImg;
               }
            }
         }
      }
   }
   else
   {
      ossim_float64* bandReal = 0;
      ossim_uint32 bands = input->getNumberOfBands();
      for(bandIdx = 0; bandIdx < bands; bandIdx+=2)
      {
         bandReal = (ossim_float64*)output->getBuf(bandIdx/2);
         if(input->getBuf(bandIdx)&&
            input->getBuf(bandIdx+1))
         {
            fillMatrixInverse((T*)input->getBuf(bandIdx),
                              (T*)input->getBuf(bandIdx+1),
                              *realIn,
                              *imgIn);
            NEWMAT::FFT2I(*realIn, *imgIn, *realOut, *imgOut);
            for(y = 0; y < h; ++y)
            {
               for(x = 0; x < w; ++x)
               {
                  *bandReal = (ossim_float64)((*realOut)[y][x]);
                  if(*bandReal > 1.0)
                  {
                     *bandReal = 1.0;
               }
                  if(*bandReal < 0.0)
                  {
                     *bandReal = 0.0;
                  }
                  ++bandReal;
               }
            }
         }
      }
   }
   
   delete realIn;
   delete imgIn;
   delete realOut;
   delete imgOut;
}

template <class T>
void ossimFftFilter::fillMatrixForward(T *realPart,
                                       T nullPix,
                                       NEWMAT::Matrix& real,
                                       NEWMAT::Matrix& img)const
{
   ossim_uint32 w = real.Ncols();
   ossim_uint32 h = real.Nrows();
   ossim_uint32 yIdx = 0;
   ossim_uint32 xIdx = 0;
   
   for(yIdx = 0; yIdx < h; ++yIdx)
   {
      for(xIdx = 0; xIdx < w; ++xIdx)
      {
         if((double)(*realPart) != nullPix)
         {
            real[yIdx][xIdx] = (double)(*realPart);
         }
         else
         {
            real[yIdx][xIdx] = 0.0;
         }
         
         img[yIdx][xIdx]  = 0.0;
         
         ++realPart;
      }
   }
}

template <class T>
void ossimFftFilter::fillMatrixInverse(T *realPart,
                                       T *imgPart,
                                       NEWMAT::Matrix& real,
                                       NEWMAT::Matrix& img)const
{
   ossim_uint32 w = real.Ncols();
   ossim_uint32 h = real.Nrows();
   ossim_uint32 yIdx = 0;
   ossim_uint32 xIdx = 0;
   
   for(yIdx = 0; yIdx < h; ++yIdx)
   {
      for(xIdx = 0; xIdx < w; ++xIdx)
      {
         real[yIdx][xIdx] = (double)(*realPart);
         img[yIdx][xIdx]  = (double)(*imgPart);
         
         ++realPart;
         ++imgPart;
      }
   }
}


bool ossimFftFilter::loadState(const ossimKeywordlist& kwl,
                               const char* prefix)
{
   const char* direction = kwl.find(prefix, "fft_direction");
   if(direction)
   {
      setDirectionType(ossimString(direction));
   }
   
   return ossimImageSourceFilter::loadState(kwl, prefix);
}

bool ossimFftFilter::saveState(ossimKeywordlist& kwl,
                               const char* prefix)const
{
   kwl.add(prefix,
           "fft_direction",
           getDirectionTypeAsString(),
           true);
   
   return ossimImageSourceFilter::saveState(kwl, prefix);
}
