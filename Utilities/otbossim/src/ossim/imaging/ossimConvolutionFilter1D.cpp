//*******************************************************************
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//*************************************************************************
// $Id: ossimConvolutionFilter1D.cpp 15833 2009-10-29 01:41:53Z eshirschorn $


#include <ossim/imaging/ossimConvolutionFilter1D.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimMatrixProperty.h>
#include <ossim/base/ossimBooleanProperty.h>
#include <ossim/base/ossimNumericProperty.h>

RTTI_DEF1(ossimConvolutionFilter1D, "ossimConvolutionFilter1D", ossimImageSourceFilter);

#define PROPNAME_KERNELSIZE "KernelSize"
#define PROPNAME_KERNEL "Kernel"
#define PROPNAME_ISHZ   "Horizontal"
#define PROPNAME_CENTEROFFSET "CenterOffset"
#define PROPNAME_STRICTNODATA "StrictNoData"

ossimConvolutionFilter1D::ossimConvolutionFilter1D(ossimObject* owner)
   :ossimImageSourceFilter(owner),
    theCenterOffset(0),
    theTile(NULL),
    theIsHz(true),
    theStrictNoData(true),
    theNullPixValue(0),
    theMinPixValue(0),
    theMaxPixValue(0)
{
   //sets up an identity convolution
   theKernel.push_back(1.0);
}

ossimConvolutionFilter1D::~ossimConvolutionFilter1D()
{
}

ossimRefPtr<ossimImageData> ossimConvolutionFilter1D::getTile(
   const ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
   if(!theInputConnection)
   {
      return theTile;
   }

   if(!isSourceEnabled())
   {
      return theInputConnection->getTile(tileRect, resLevel);
   }

   //---
   // We have a 1xn or nx1 matrix, + a center offset
   // so stretch the input rect out to cover the required pixels
   //---
   ossimIrect  newRect;
   ossim_uint32 kl = (ossim_uint32)theKernel.size(); //kernel length
   if (theIsHz)
   {
      //horizontal kernel (row kernel)
      newRect = ossimIrect(ossimIpt(tileRect.ul().x - theCenterOffset,
                                    tileRect.ul().y),
                           ossimIpt(tileRect.lr().x - theCenterOffset + kl -1,
                                    tileRect.lr().y));
   } else {
      //vertical kernel
      newRect = ossimIrect(ossimIpt(tileRect.ul().x,
                                    tileRect.ul().y - theCenterOffset),
                           ossimIpt(tileRect.lr().x,
                                    tileRect.lr().y - theCenterOffset + kl -1));
   }
   ossimRefPtr<ossimImageData> data = theInputConnection->getTile(newRect,
                                                                  resLevel);

   if(!data.valid() || !data->getBuf())
   {
      return data;
   }

    // First time through or after an initialize()...
   if (!theTile.valid())
   {
      allocate();
      if (!theTile.valid()) // Should never happen!
      {
         return data;
      }
   }

   // First time through, after an initialize() or a setKernel()...
   if (!theNullPixValue.size())
   {
      computeNullMinMax();
      if (!theNullPixValue.size()) // Should never happen!
      {
         return data;
      }
   }

   theTile->setImageRectangle(tileRect);
   theTile->makeBlank();
   
   switch(data->getScalarType())
   {
      case OSSIM_UCHAR:
      {
         if(data->getDataObjectStatus() == OSSIM_FULL)
         {
            convolveFull(static_cast<ossim_uint8>(0), data, theTile);
         }
         else
         {
            convolvePartial(static_cast<ossim_uint8>(0), data, theTile);
         }
         break;
      }
      case OSSIM_FLOAT: 
      case OSSIM_NORMALIZED_FLOAT:
      {
         if(data->getDataObjectStatus() == OSSIM_FULL)
         {
            convolveFull(static_cast<float>(0), data, theTile);
         }
         else
         {
            convolvePartial(static_cast<float>(0), data, theTile);
         }
         break;
      }
      case OSSIM_USHORT16:
      case OSSIM_USHORT11:
      {
         if(data->getDataObjectStatus() == OSSIM_FULL)
         {
            convolveFull(static_cast<ossim_uint16>(0), data, theTile);
         }
         else
         {
            convolvePartial(static_cast<ossim_uint16>(0), data, theTile);
         }
         break;
      }
      case OSSIM_SSHORT16:
      {
         if(data->getDataObjectStatus() == OSSIM_FULL)
         {
            convolveFull(static_cast<ossim_sint16>(0), data, theTile);
         }
         else
         {
            convolvePartial(static_cast<ossim_sint16>(0), data, theTile);
         }
         break;
      }
      case OSSIM_DOUBLE:
      case OSSIM_NORMALIZED_DOUBLE:
      {
         if(data->getDataObjectStatus() == OSSIM_FULL)
         {
            convolveFull(static_cast<double>(0), data, theTile);
      }
      else
      {
         convolvePartial(static_cast<double>(0), data, theTile);
      }
         break;
      }
      default:
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimConvolutionFilter1D::getTile WARNING:\n"
            << "Scalar type = " << theTile->getScalarType()
            << " Not supported by ossimConvolutionFilter1D" << endl;
         break;
      }
   }
   theTile->validate();
   
   return theTile;
}


template<class T> void ossimConvolutionFilter1D::convolvePartial(
   T,
   ossimRefPtr<ossimImageData> inputData,
   ossimRefPtr<ossimImageData> outputData)
{
   // there may be NULL pixels in the input
   ossim_int32 inputW = static_cast<ossim_int32>(inputData->getWidth());
   ossim_uint32 outputW       = outputData->getWidth();
   ossim_uint32 outputH       = outputData->getHeight();
   ossim_uint32 numberOfBands = inputData->getNumberOfBands();
   
   ossim_uint32 klength=(ossim_uint32)theKernel.size(),k;
   ossim_int32 incrK = theIsHz ? 1 : inputW; //kernel increment in input : one col or one row
   ossim_int32 iRow  = inputW - outputW; //jump to go to next row

   double sum=0.0;
   T* iKP; //kernel covered input pointer

   ossim_int32 iCo = incrK*theCenterOffset; //center  pixel offset within the input buffer

   for(ossim_uint32 band = 0; band < numberOfBands; ++band)
   {
      T* inputBuf  = static_cast<T*>(inputData->getBuf(band));
      T* outputBuf = static_cast<T*>(outputData->getBuf(band));
      T maxPix     = static_cast<T>(getMaxPixelValue(band));
      T minPix     = static_cast<T>(getMinPixelValue(band));
      T nullPix    = static_cast<T>(inputData->getNullPix(band));
      T oNullPix   = static_cast<T>(getNullPixelValue(band));
      
      if(inputBuf&&outputBuf)
      {
         for(ossim_uint32 row = 0; row < outputH; ++row)
         {
            for(ossim_uint32 col = 0; col < outputW; ++col)
            {
               //center pixel may not be NULL
               if (inputBuf[iCo] != nullPix)
               {
                  iKP = inputBuf; 
                  sum = 0.0;
                  for(k=0;k<klength;++k)
                  {
                     if (*iKP != nullPix) //just sum on non-NULL pixels (potential normailzation issue)
                     {
                        sum += theKernel[k] * (double)(*iKP);                   
                     } else if (theStrictNoData)
                     {
                        break;
                     }                     
                     iKP += incrK;
                  }
                  if (k==klength)
                  {
                     if(sum > maxPix)
                     {
                        *outputBuf = maxPix;
                     }
                     else if(sum < minPix)
                     {
                        *outputBuf = minPix;
                     }
                     else
                     {
                        *outputBuf = static_cast<T>(sum);
                     }
                  } else {
                     //theStrictNoData
                     *outputBuf = oNullPix;
                  }
               } else {
                  *outputBuf = oNullPix;
               }
               // move curent center position
               inputBuf  += 1;
               outputBuf += 1;
            }
            //move current center position
            inputBuf  += iRow;
            //outputBuf += 0;
         }
      }
   }
    
   
}

template<class T> void ossimConvolutionFilter1D::convolveFull(
   T,
   ossimRefPtr<ossimImageData> inputData,
   ossimRefPtr<ossimImageData> outputData)
{
   //inputData must be displaced according to the Kernel size and theCenterOffset +isHz
   ossim_int32 inputW = static_cast<ossim_int32>(inputData->getWidth());
   ossim_uint32 outputW       = outputData->getWidth();
   ossim_uint32 outputH       = outputData->getHeight();
   ossim_uint32 numberOfBands = inputData->getNumberOfBands();
   
   ossim_uint32 klength=(ossim_uint32)theKernel.size(), k;
   ossim_int32 incrK = theIsHz ? 1 : inputW; //kernel increment in input : one col or one row
   ossim_int32 iRow  = inputW - outputW; //jump to go to next row

   double sum=0.0;
   T* iKP; //kernel covered input pointer

   for(ossim_uint32 band = 0; band < numberOfBands; ++band)
   {
      T* inputBuf  = static_cast<T*>(inputData->getBuf(band));
      T* outputBuf = static_cast<T*>(outputData->getBuf(band));
      T maxPix     = static_cast<T>(getMaxPixelValue(band));
      T minPix     = static_cast<T>(getMinPixelValue(band));
      
      if(inputBuf&&outputBuf)
      {
         for(ossim_uint32 row = 0; row < outputH; ++row)
         {
            for(ossim_uint32 col = 0; col < outputW; ++col)
            {
               sum = 0.0;
               iKP = inputBuf;
               for(k=0;k<klength;++k)
               {
                  sum += theKernel[k] * (double)(*iKP);
                  iKP += incrK;
               }
               
               if(sum > maxPix)
               {
                  *outputBuf = maxPix;
               }
               else if(sum < minPix)
               {
                  *outputBuf = minPix;
               }
               else
               {
                  *outputBuf = static_cast<T>(sum);
               }
               // move curent center position
               inputBuf  += 1;
               outputBuf += 1;
            }
            //move current center position
            inputBuf  += iRow;
            //outputBuf += 0;
         }
      }
   }
}

void ossimConvolutionFilter1D::initialize()
{
   //---
   // NOTE:
   // Since initialize get called often sequentially we will wipe things slick
   // but not reallocate to avoid multiple delete/allocates.
   //
   // On the first getTile call things will be reallocated/computed.
   //---
   theTile = NULL;
   clearNullMinMax();
}

void ossimConvolutionFilter1D::allocate()
{   
   if(theInputConnection)
   {
      ossimImageDataFactory* idf = ossimImageDataFactory::instance();
      
      theTile = idf->create(this,
                            this);
      
      theTile->initialize();
   }
}

void ossimConvolutionFilter1D::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(!property) return;

   if(property->getName() == PROPNAME_KERNEL)
   {
      ossimMatrixProperty* matrixProperty = PTR_CAST(ossimMatrixProperty,
                                                     property.get());
      if(matrixProperty)
      {
         ossim_uint32 nc=matrixProperty->getNumberOfCols();
         theKernel.resize(nc);
         for(ossim_uint32 i=0;i<nc;++i)
         {
             theKernel[i] = (*matrixProperty)(0,i);
         }
         clearNullMinMax();
      }
      else
      {
         ossimImageSourceFilter::setProperty(property);
      }
   } else if (property->getName() == PROPNAME_ISHZ)  {
      ossimBooleanProperty* booleanProperty = PTR_CAST(ossimBooleanProperty,
                                                     property.get());
      if(booleanProperty)
      {
         theIsHz = booleanProperty->getBoolean();
      }
   } else if (property->getName() == PROPNAME_STRICTNODATA)  {
      ossimBooleanProperty* booleanProperty = PTR_CAST(ossimBooleanProperty,
                                                     property.get());
      if(booleanProperty)
      {
         theStrictNoData = booleanProperty->getBoolean();
      }
   } else if (property->getName() == PROPNAME_CENTEROFFSET)  {
      ossimNumericProperty* numProperty = PTR_CAST(ossimNumericProperty,
                                                     property.get());
      if(numProperty)
      {
         theCenterOffset = numProperty->asInt32();
      }
   } else if (property->getName() == PROPNAME_KERNELSIZE)  {
      ossimNumericProperty* numProperty = PTR_CAST(ossimNumericProperty,
                                                     property.get());
      if(numProperty)
      {
         theKernel.resize(numProperty->asUInt32());
         clearNullMinMax();
      }
   } else {
      ossimImageSourceFilter::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimConvolutionFilter1D::getProperty(const ossimString& name)const
{
   if(name == PROPNAME_KERNEL)
   {
      ossimMatrixProperty* property = new ossimMatrixProperty(name);
      property->resize(1,(int)theKernel.size());
      for(ossim_uint32 i=0;i<theKernel.size();++i)
      {
        (*property)(0,i) = theKernel[i];
      }
      property->setCacheRefreshBit();
      return property;
   } else if (name==PROPNAME_ISHZ) {
      ossimBooleanProperty* property = new ossimBooleanProperty(name,theIsHz);
      property->setCacheRefreshBit();
      return property;
   } else if (name==PROPNAME_STRICTNODATA) {
      ossimBooleanProperty* property = new ossimBooleanProperty(name,theStrictNoData);
      property->setCacheRefreshBit();
      return property;
   } else if (name==PROPNAME_CENTEROFFSET) {
      ossimNumericProperty* property = new ossimNumericProperty(name,ossimString::toString(theCenterOffset));
      property->setNumericType(ossimNumericProperty::ossimNumericPropertyType_INT); //restrict to int type
      property->setCacheRefreshBit();
      return property;
   } else if (name == PROPNAME_KERNELSIZE) {
      ossimNumericProperty* property = new ossimNumericProperty(name,ossimString::toString((ossim_uint32)theKernel.size()),1.0,100.0);
      property->setNumericType(ossimNumericProperty::ossimNumericPropertyType_INT); //restrict to int type
      property->setCacheRefreshBit();
      return property;
   }
   return ossimImageSourceFilter::getProperty(name);
}

void ossimConvolutionFilter1D::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimImageSourceFilter::getPropertyNames(propertyNames);
   propertyNames.push_back(PROPNAME_KERNELSIZE);
   propertyNames.push_back(PROPNAME_KERNEL);
   propertyNames.push_back(PROPNAME_CENTEROFFSET);
   propertyNames.push_back(PROPNAME_ISHZ);
   propertyNames.push_back(PROPNAME_STRICTNODATA);
}

bool ossimConvolutionFilter1D::saveState(ossimKeywordlist& kwl,
                                         const char* prefix)const
{   
   kwl.add(prefix,
           PROPNAME_KERNELSIZE,
           static_cast<ossim_uint32>(theKernel.size()),
           true);
   kwl.add(prefix,
          PROPNAME_CENTEROFFSET,
           theCenterOffset,
           true);
   kwl.add(prefix,
          PROPNAME_ISHZ,
           theIsHz?"true":"false", //use string instead of boolean
           true);
   kwl.add(prefix,
          PROPNAME_STRICTNODATA,
           theStrictNoData?"true":"false", //use string instead of boolean
           true);

   for(ossim_uint32 row = 0; row < theKernel.size(); ++row)
   {
         ossimString newPrefix = "k" + ossimString::toString(row);
         kwl.add(prefix,
                 newPrefix,
                 theKernel[row],
                 true);          
   }
   
   return ossimImageSourceFilter::saveState(kwl, prefix);
}


bool ossimConvolutionFilter1D::loadState(const ossimKeywordlist& kwl,
                                          const char* prefix)
{
   //find kernel size
   ossim_uint32 iks=0;
   const char* ks = kwl.find(prefix, PROPNAME_KERNELSIZE);
   if(ks)
   {
      iks = ossimString(ks).toUInt32();
      if (iks<1)
      {
         cerr<<"ossimConvolutionFilter1D : warning bad "<< PROPNAME_KERNELSIZE <<" in state"<<endl;
         iks=0;
      }
   } else {
      iks=0;
      cerr<<"ossimConvolutionFilter1D : warning no "<< PROPNAME_KERNELSIZE <<" in state"<<endl;
   }

   //load other props
   const char* co = kwl.find(prefix, PROPNAME_CENTEROFFSET);
   if(co)
   {      
      setCenterOffset(ossimString(co).toUInt32());
   } else {
      cerr<<"ossimConvolutionFilter1D : warning no "<< PROPNAME_CENTEROFFSET<<" in state"<<endl;
   }
   const char* ih = kwl.find(prefix, PROPNAME_ISHZ);
   if(ih)
   {      
      setIsHorizontal(ossimString(ih).toBool());
   } else {
      cerr<<"ossimConvolutionFilter1D : warning no "<<PROPNAME_ISHZ<<" in state"<<endl;
   }
   const char* sn = kwl.find(prefix, PROPNAME_STRICTNODATA);
   if(sn)
   {      
      setStrictNoData(ossimString(sn).toBool());
   } else {
      cerr<<"ossimConvolutionFilter1D : warning no "<<PROPNAME_STRICTNODATA<<" in state"<<endl;
   }
   //load kernel values
   ossimString newPrefix = prefix;
   newPrefix += ossimString("k");
   theKernel.resize(iks);
   for(ossim_uint32 r=0; r<iks; ++r)
   {      
      ossimString value = ossimString::toString(r);
      
      const char* v = kwl.find(newPrefix.c_str(),
                               value.c_str());
      if(v)
      {
         theKernel[r] = ossimString(v).toDouble();
      } else {
         cerr<<"ossimConvolutionFilter1D : warning no value for "<<newPrefix<<" in state"<<endl;
         theKernel[r] = 0.0;
      }
   }
   clearNullMinMax();
   
   return ossimImageSourceFilter::loadState(kwl, prefix);
}

void ossimConvolutionFilter1D::setKernel(const std::vector<ossim_float64>& aKernel)
{
   theKernel = aKernel;
   // Will be recomputed first getTile call.
   clearNullMinMax();
}

double ossimConvolutionFilter1D::getNullPixelValue(ossim_uint32 band)const
{
   if( isSourceEnabled() && (band < theNullPixValue.size()) )
   {
      return theNullPixValue[band];
   }

   return ossim::defaultNull(getOutputScalarType());
}

double ossimConvolutionFilter1D::getMinPixelValue(ossim_uint32 band)const
{
   if( isSourceEnabled() && (band < theMinPixValue.size()) )
   {
      return theMinPixValue[band];
   }
   
   return ossimImageSource::getMinPixelValue(band);
}

double ossimConvolutionFilter1D::getMaxPixelValue(ossim_uint32 band)const
{
   if( isSourceEnabled() && (band < theMaxPixValue.size()) )
   {
      return theMaxPixValue[band];
   }

   return ossimImageSource::getMaxPixelValue(band);
}

void ossimConvolutionFilter1D::clearNullMinMax()
{
   theNullPixValue.clear();
   theMinPixValue.clear();
   theMaxPixValue.clear();
}

void ossimConvolutionFilter1D::computeNullMinMax()
{
   const ossim_uint32 BANDS = getNumberOfOutputBands();

   theNullPixValue.resize(BANDS);
   theMinPixValue.resize(BANDS);
   theMaxPixValue.resize(BANDS);

   ossim_float64 defaultNull = ossim::defaultNull(getOutputScalarType());
   ossim_float64 defaultMin = ossim::defaultMin(getOutputScalarType());
   ossim_float64 defaultMax = ossim::defaultMax(getOutputScalarType());
  
   for (ossim_uint32 band = 0; band < BANDS; ++band)
   {
      if(theInputConnection)
      {
         ossim_float64 inputNull = theInputConnection->getNullPixelValue(band);
         ossim_float64 inputMin  = theInputConnection->getMinPixelValue(band);
         ossim_float64 inputMax  = theInputConnection->getMaxPixelValue(band);
         ossim_float64 tempMin   = 0.0;
         ossim_float64 tempMax   = 0.0;
         ossim_float64 k         = 0.0;
         for(ossim_uint32 i=0;i<theKernel.size();++i)
         {          
               k=theKernel[i];
               tempMin += (k<0.0) ? k*inputMax : k*inputMin;
               tempMax += (k>0.0) ? k*inputMax : k*inputMin;          
         }
         
         if((tempMin >= defaultMin) && (tempMin <= defaultMax))
         {
            theMinPixValue[band] = tempMin;
         }
         else
         {
            theMinPixValue[band] = defaultMin;
         }

         if((tempMax >= defaultMin) && (tempMax <= defaultMax))
         {
            theMaxPixValue[band] = tempMax;
         }
         else
         {
            theMaxPixValue[band] = defaultMax;
         }
         if((inputNull < theMinPixValue[band]) ||
            (inputNull > theMaxPixValue[band]))
         {
            theNullPixValue[band] = inputNull;
         }
         else
         {
            theNullPixValue[band] = defaultNull;
         }
         
      }
      else // No input connection...
      {
         theNullPixValue[band] = defaultNull;
         theMinPixValue[band]  = defaultMin;
         theMaxPixValue[band]  = defaultMax;
      }
      
   } // End of band loop.
}
