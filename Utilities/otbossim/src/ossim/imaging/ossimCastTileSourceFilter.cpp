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
// $Id: ossimCastTileSourceFilter.cpp 17195 2010-04-23 17:32:18Z dburken $
#include <ossim/imaging/ossimCastTileSourceFilter.h>
#include <ossim/imaging/ossimU8ImageData.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimScalarTypeLut.h>
#include <ossim/base/ossimStringProperty.h>

RTTI_DEF1( ossimCastTileSourceFilter,
           "ossimCastTileSourceFilter",
           ossimImageSourceFilter );

ossimCastTileSourceFilter::ossimCastTileSourceFilter(ossimImageSource* input,
						     ossimScalarType scalarType)
   :ossimImageSourceFilter(NULL,input),
    theTile(NULL),
    theOutputScalarType(scalarType)
{
}

ossimCastTileSourceFilter::~ossimCastTileSourceFilter()
{
}

ossimRefPtr<ossimImageData> ossimCastTileSourceFilter::getTile(
   const ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
   ossimRefPtr<ossimImageData> inputTile;
   
   if(theInputConnection)
   {
      inputTile = theInputConnection->getTile(tileRect, resLevel);
   }
   return applyCast(inputTile);
}

ossimScalarType ossimCastTileSourceFilter::getOutputScalarType() const
{
   if(isSourceEnabled())
   {
      return theOutputScalarType;
   }
   else if(theInputConnection)
   {
      return theInputConnection->getOutputScalarType();
   }

   return OSSIM_SCALAR_UNKNOWN;
}

double ossimCastTileSourceFilter::getNullPixelValue(ossim_uint32 band)const
{
   if(!isSourceEnabled())
   {
      if(theInputConnection)
      {
         return theInputConnection->getNullPixelValue(band);
      }
   }
   return ossim::defaultNull(getOutputScalarType());
}

double ossimCastTileSourceFilter::getMinPixelValue(ossim_uint32 band)const
{
   if(!isSourceEnabled())
   {
      if(theInputConnection)
      {
         return theInputConnection->getMinPixelValue(band);
      }
   }

   if(theInputConnection)
   {
      double tempMin    = theInputConnection->getMinPixelValue(band);
      
      double defaultMin = ossim::defaultMin(theOutputScalarType);
      double defaultMax = ossim::defaultMax(theOutputScalarType);

      if((tempMin >= defaultMin) && (tempMin <= defaultMax))
      {
         defaultMin = tempMin;
      }

      return defaultMin;
   }
   
   return ossimImageSource::getMinPixelValue(band);
}

double ossimCastTileSourceFilter::getMaxPixelValue(ossim_uint32 band)const
{
   if(!isSourceEnabled())
   {
      if(theInputConnection)
      {
         return theInputConnection->getMaxPixelValue(band);
      }
   }
   if(theInputConnection)
   {
      double tempMax    = theInputConnection->getMaxPixelValue(band);
      
      double defaultMax = ossim::defaultMax(theOutputScalarType);
      double defaultMin = ossim::defaultMin(theOutputScalarType);

      if((tempMax >= defaultMin) && (tempMax <= defaultMax))
      {
         defaultMax = tempMax;
      }

      return defaultMax;
   }
   return ossimImageSource::getMaxPixelValue(band);
}

ossimRefPtr<ossimImageData> ossimCastTileSourceFilter::applyCast(
   ossimRefPtr<ossimImageData> inputTile)
{
   if(!inputTile.valid())
   {
      return inputTile;
   }
   
   if(!theTile.valid() ||
      (theTile->getNumberOfBands() != inputTile->getNumberOfBands()))
   {
      allocate(inputTile);
   }
   else
   {
      ossim_int32 tw =inputTile->getWidth(); 
      ossim_int32 th =inputTile->getHeight(); 
      ossim_int32 w = theTile->getWidth();
      ossim_int32 h = theTile->getHeight();
      theTile->setWidthHeight(tw, th);
      
      if((w*h != tw*th))
      {
         theTile->initialize();
      }
      else
      {
         theTile->makeBlank();
      }
      theTile->setOrigin(inputTile->getOrigin());
   }
   
   if( (inputTile->getDataObjectStatus()==OSSIM_EMPTY) ||
       (inputTile->getDataObjectStatus()==OSSIM_NULL))
   {
      return theTile;
   }
   
   switch(inputTile->getScalarType())
   {
      case OSSIM_UINT8:
      {
         ossim_uint8** bands = new ossim_uint8*[inputTile->getNumberOfBands()];
         
         for(ossim_uint32 band = 0;
             band < inputTile->getNumberOfBands();
             ++band)
         {
            bands[band] = static_cast<ossim_uint8*>(inputTile->getBuf(band));
         }
         castInputToOutput(bands,
                           inputTile->getNullPix(),
                           inputTile->getDataObjectStatus()==OSSIM_PARTIAL);
         delete [] bands;
         break;
      }
      case OSSIM_SINT8:
      {
         ossim_sint8** bands = new ossim_sint8*[inputTile->getNumberOfBands()];
         
         for(ossim_uint32 band = 0;
             band < inputTile->getNumberOfBands();
             ++band)
         {
            bands[band] = static_cast<ossim_sint8*>(inputTile->getBuf(band));
         }
         castInputToOutput(bands,
                           inputTile->getNullPix(),
                           inputTile->getDataObjectStatus()==OSSIM_PARTIAL);
         delete [] bands;
         break;
      }
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
      {
         ossim_uint16** bands =
            new ossim_uint16*[inputTile->getNumberOfBands()];
         
         for(ossim_uint32 band = 0;
             band < inputTile->getNumberOfBands();
             ++band)
         {
            bands[band] = static_cast<ossim_uint16*>(inputTile->getBuf(band));
         }
         castInputToOutput(bands,
                           inputTile->getNullPix(),
                           inputTile->getDataObjectStatus()==OSSIM_PARTIAL);
         delete [] bands;
         break;
      }
      case OSSIM_SINT16:
      {
         ossim_sint16** bands =
            new ossim_sint16*[inputTile->getNumberOfBands()];
         
         for(ossim_uint32 band = 0;
             band < inputTile->getNumberOfBands();
             ++band)
         {
            bands[band] = static_cast<ossim_sint16*>(inputTile->getBuf(band));
         }
         castInputToOutput(bands,
                           inputTile->getNullPix(),
                           inputTile->getDataObjectStatus()==OSSIM_PARTIAL);
         delete [] bands;
         break;
      }
      case OSSIM_SINT32:
      {
         ossim_sint32** bands =
            new ossim_sint32*[inputTile->getNumberOfBands()];
         
         for(ossim_uint32 band = 0;
             band < inputTile->getNumberOfBands();
             ++band)
         {
            bands[band] = static_cast<ossim_sint32*>(inputTile->getBuf(band));
         }
         castInputToOutput(bands,
                           inputTile->getNullPix(),
                           inputTile->getDataObjectStatus()==OSSIM_PARTIAL);
         delete [] bands;
         break;
      }
      case OSSIM_UINT32:
      {
         ossim_uint32** bands =
            new ossim_uint32*[inputTile->getNumberOfBands()];
         
         for(ossim_uint32 band = 0;
             band < inputTile->getNumberOfBands();
             ++band)
         {
            bands[band] = static_cast<ossim_uint32*>(inputTile->getBuf(band));
         }
         castInputToOutput(bands,
                           inputTile->getNullPix(),
                           inputTile->getDataObjectStatus()==OSSIM_PARTIAL);
         delete [] bands;
         break;
      }
      case OSSIM_FLOAT32: 
      case OSSIM_NORMALIZED_FLOAT:
      {
         float** bands = new float*[inputTile->getNumberOfBands()];
         
         for(ossim_uint32 band = 0;
             band < inputTile->getNumberOfBands();
             ++band)
         {
            bands[band] = static_cast<float*>(inputTile->getBuf(band));
         }
         castInputToOutput(bands,
                           inputTile->getNullPix(),
                           inputTile->getDataObjectStatus()==OSSIM_PARTIAL);
         delete [] bands;
         break;
      }
      case OSSIM_FLOAT64:
      case OSSIM_NORMALIZED_DOUBLE:
      {
         double** bands = new double*[inputTile->getNumberOfBands()];
         
         for(ossim_uint32 band = 0;
             band < inputTile->getNumberOfBands();
             ++band)
         {
            bands[band] = static_cast<double*>(inputTile->getBuf(band));
         }
         castInputToOutput(bands,
                           inputTile->getNullPix(),
                           inputTile->getDataObjectStatus()==OSSIM_PARTIAL);
         delete [] bands;
         break;
      }
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         break;
      }
   }

//   theTile->setDataObjectStatus(inputTile->getDataObjectStatus());
   theTile->validate();
   return theTile;
}

void ossimCastTileSourceFilter::setOutputScalarType(ossimScalarType scalarType)
{
   ossimScalarType old_type = theOutputScalarType;
   theOutputScalarType = scalarType;

   if ( (theOutputScalarType != old_type))
   {
      // must reinitialize...
      theTile = NULL;
      initialize();
   }
}

void ossimCastTileSourceFilter::setOutputScalarType(ossimString scalarType)
{
   int scalar =
      ossimScalarTypeLut::instance()->getEntryNumber(scalarType.c_str());
   
   if (scalar != ossimLookUpTable::NOT_FOUND)
   {
      setOutputScalarType(static_cast<ossimScalarType>(scalar));
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimCastTileSourceFilter::setOutputScalarType WARN:"
         << "\nUnknown scalar type:  " << scalarType.c_str() << endl;
   }
}

ossimString ossimCastTileSourceFilter::getOutputScalarTypeString()const
{
   return ossimScalarTypeLut::instance()->getEntryString(theOutputScalarType);
}

void ossimCastTileSourceFilter::initialize()
{
   ossimImageSourceFilter::initialize();

   if (theOutputScalarType == OSSIM_SCALAR_UNKNOWN)
   {
      // default to OSSIM_UCHAR
      theOutputScalarType = OSSIM_UCHAR;
   }
}

bool ossimCastTileSourceFilter::saveState(ossimKeywordlist& kwl,
                                          const char* prefix) const
{
   ossimImageSourceFilter::saveState(kwl, prefix);

   kwl.add(prefix,
           ossimKeywordNames::SCALAR_TYPE_KW,
           ossimScalarTypeLut::instance()->getEntryString(theOutputScalarType),
           true);

   return true;
}

bool ossimCastTileSourceFilter::loadState(const ossimKeywordlist& kwl,
                                          const char* prefix)
{
   ossimImageSourceFilter::loadState(kwl, prefix);

   if (kwl.getErrorStatus() == ossimErrorCodes::OSSIM_ERROR)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimScalarRemapper::loadState WARN:\n"
         << " ERROR detected in keyword list!  State not loaded."
         << endl;
      return false;
   }

   int scalar = ossimScalarTypeLut::instance()->getEntryNumber(kwl, prefix);

   if (scalar != ossimLookUpTable::NOT_FOUND)
   {
      theOutputScalarType = static_cast<ossimScalarType>(scalar);
   }

   return true;
}

template<class inType>
void ossimCastTileSourceFilter::castInputToOutput(inType **inBuffer,
                                                  const double  *nullPix,
                                                  bool   inPartialFlag)
{
   switch(theTile->getScalarType())
   {
      case OSSIM_UINT8:
      {
         ossim_uint8** bands = new ossim_uint8*[theTile->getNumberOfBands()];

         for(ossim_uint32 band = 0; band < theTile->getNumberOfBands(); ++band)
         {
            bands[band] = static_cast<ossim_uint8*>(theTile->getBuf(band));
         }
      
         castInputToOutput(inBuffer,
                           nullPix,
                           inPartialFlag,
                           bands,
                           theTile->getNullPix(),
                           theTile->getNumberOfBands());
         delete [] bands;
         break;
      }
      case OSSIM_SINT8:
      {
         ossim_sint8** bands = new ossim_sint8*[theTile->getNumberOfBands()];

         for(ossim_uint32 band = 0; band < theTile->getNumberOfBands(); ++band)
         {
            bands[band] = static_cast<ossim_sint8*>(theTile->getBuf(band));
         }
      
         castInputToOutput(inBuffer,
                           nullPix,
                           inPartialFlag,
                           bands,
                           theTile->getNullPix(),
                           theTile->getNumberOfBands());
         delete [] bands;
         break;
      }
      case OSSIM_SINT16:
      {
         ossim_sint16** bands = new ossim_sint16*[theTile->getNumberOfBands()];

         for(ossim_uint32 band = 0; band < theTile->getNumberOfBands(); ++band)
         {
            bands[band] = static_cast<ossim_sint16*>(theTile->getBuf(band));
         }
      
         castInputToOutput(inBuffer,
                           nullPix,
                           inPartialFlag,
                           bands,
                           theTile->getNullPix(),
                           theTile->getNumberOfBands());
         delete [] bands;
         break;
      }
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
      {
         ossim_uint16** bands = new ossim_uint16*[theTile->getNumberOfBands()];

         for(ossim_uint32 band = 0; band < theTile->getNumberOfBands(); ++band)
         {
            bands[band] = static_cast<ossim_uint16*>(theTile->getBuf(band));
         }
      
         castInputToOutput(inBuffer,
                           nullPix,
                           inPartialFlag,
                           bands,
                           theTile->getNullPix(),
                           theTile->getNumberOfBands());
         delete [] bands;
         break;
      }
      case OSSIM_UINT32:
      {
         ossim_uint32** bands = new ossim_uint32*[theTile->getNumberOfBands()];

         for(ossim_uint32 band = 0; band < theTile->getNumberOfBands(); ++band)
         {
            bands[band] = static_cast<ossim_uint32*>(theTile->getBuf(band));
         }
      
         castInputToOutput(inBuffer,
                           nullPix,
                           inPartialFlag,
                           bands,
                           theTile->getNullPix(),
                           theTile->getNumberOfBands());
         delete [] bands;
         break;
      }
      case OSSIM_SINT32:
      {
         ossim_sint32** bands = new ossim_sint32*[theTile->getNumberOfBands()];

         for(ossim_uint32 band = 0; band < theTile->getNumberOfBands(); ++band)
         {
            bands[band] = static_cast<ossim_sint32*>(theTile->getBuf(band));
         }
      
         castInputToOutput(inBuffer,
                           nullPix,
                           inPartialFlag,
                           bands,
                           theTile->getNullPix(),
                           theTile->getNumberOfBands());
         delete [] bands;
         break;
      }
      
      case OSSIM_FLOAT32: 
      case OSSIM_NORMALIZED_FLOAT:
      {
         float** bands = new float*[theTile->getNumberOfBands()];

         for(ossim_uint32 band = 0; band < theTile->getNumberOfBands(); ++band)
         {
            bands[band] = static_cast<float*>(theTile->getBuf(band));
         }
      
         castInputToOutput(inBuffer,
                           nullPix,
                           inPartialFlag,
                           bands,
                           theTile->getNullPix(),
                           theTile->getNumberOfBands());
         delete [] bands;
         break;
      }
      case OSSIM_FLOAT64:
      case OSSIM_NORMALIZED_DOUBLE:
      {
         double** bands = new double*[theTile->getNumberOfBands()];

         for(ossim_uint32 band = 0; band < theTile->getNumberOfBands(); ++band)
         {
            bands[band] = static_cast<double*>(theTile->getBuf(band));
         }
      
         castInputToOutput(inBuffer,
                           nullPix,
                           inPartialFlag,
                           bands,
                           theTile->getNullPix(),
                           theTile->getNumberOfBands());
         delete [] bands;
         break;
      }
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimCastTileSourceFilter::castInputToOutput WARN:\n"
            << "Scalar not set in ossimCastTileSourceFilter" << endl;
         break;
      }
   }
}

template<class inType, class outType>
void ossimCastTileSourceFilter::castInputToOutput(inType *inBuffer[],
                                                  const double  *nullInPix,
                                                  bool   inPartialFlag,
                                                  outType *outBuffer[],
                                                  const double  *nullOutPix,
                                                  ossim_uint32   numberOfBands)
{
   ossim_uint32 size = theTile->getWidth()*theTile->getHeight();
   if(!inPartialFlag)
   {
      for(ossim_uint32 band = 0; band < numberOfBands; ++band)
      {
         outType outMin = static_cast<outType>(theTile->getMinPix(band));
         outType outMax = static_cast<outType>(theTile->getMaxPix(band));
         
         for(ossim_uint32 offset = 0; offset < size; ++offset)
         {
            double temp = inBuffer[band][offset];
            if(temp < outMin) temp = outMin;
            if(temp > outMax) temp = outMax;
            outBuffer[band][offset] = static_cast<outType>(temp);
         }
      }
   }
   else
   {
      for(ossim_uint32 band = 0; band < numberOfBands; ++band)
      {
         inType np      = static_cast<inType>(nullInPix[band]);
	     outType outNp  = static_cast<outType>(nullOutPix[band]);
         outType outMin = static_cast<outType>(theTile->getMinPix(band));
         outType outMax = static_cast<outType>(theTile->getMaxPix(band));
         
         for(ossim_uint32 offset = 0; offset < size; ++offset)
         {
            if(inBuffer[band][offset] != np)
            {
               double temp = inBuffer[band][offset];
               if(temp < outMin) temp = outMin;
               if(temp > outMax) temp = outMax;
               outBuffer[band][offset] = static_cast<outType>(temp);
            }
	    else
	    {
		outBuffer[band][offset] = static_cast<outType>(outNp);
		
	    }
         }
      }
   }
}

void ossimCastTileSourceFilter::allocate(const ossimRefPtr<ossimImageData> inputTile)
{
   theTile = ossimImageDataFactory::instance()->create(
      this,
      getOutputScalarType(),
      inputTile->getNumberOfBands(),
      inputTile->getWidth(),
      inputTile->getHeight());
   theTile->initialize();
   theTile->setOrigin(inputTile->getOrigin());
}

void ossimCastTileSourceFilter::propertyEvent(ossimPropertyEvent& /* event */)
{
}

void ossimCastTileSourceFilter::refreshEvent(ossimRefreshEvent& /* event */)
{
}

void ossimCastTileSourceFilter::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(property->getName() == ossimKeywordNames::SCALAR_TYPE_KW)
   {
      int scalar = ossimScalarTypeLut::instance()->getEntryNumber(property->valueToString());
      
      if (scalar != ossimLookUpTable::NOT_FOUND)
      {
         setOutputScalarType(static_cast<ossimScalarType>(scalar));
      }
   }
   else
   {
      ossimImageSourceFilter::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimCastTileSourceFilter::getProperty(const ossimString& name)const
{
   ossimRefPtr<ossimProperty> resultProperty;
   if(name == ossimKeywordNames::SCALAR_TYPE_KW)
   {
      ossimStringProperty* stringProperty = new ossimStringProperty(name,
                                                                    ossimScalarTypeLut::instance()->getEntryString(theOutputScalarType),
                                                                    false);

      stringProperty->addConstraint(ossimScalarTypeLut::instance()->getEntryString(OSSIM_UINT8));
      stringProperty->addConstraint(ossimScalarTypeLut::instance()->getEntryString(OSSIM_SINT8));
      stringProperty->addConstraint(ossimScalarTypeLut::instance()->getEntryString(OSSIM_UINT16));
      stringProperty->addConstraint(ossimScalarTypeLut::instance()->getEntryString(OSSIM_SINT16));
      stringProperty->addConstraint(ossimScalarTypeLut::instance()->getEntryString(OSSIM_UINT32));
      stringProperty->addConstraint(ossimScalarTypeLut::instance()->getEntryString(OSSIM_SINT32));
      stringProperty->addConstraint(ossimScalarTypeLut::instance()->getEntryString(OSSIM_FLOAT32));
      stringProperty->addConstraint(ossimScalarTypeLut::instance()->getEntryString(OSSIM_FLOAT64));

      stringProperty->setCacheRefreshBit();
      resultProperty = stringProperty;
   }
   else
   {
      resultProperty = ossimImageSourceFilter::getProperty(name);
   }
   return resultProperty;
}

void ossimCastTileSourceFilter::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimImageSourceFilter::getPropertyNames(propertyNames);
   
   propertyNames.push_back(ossimKeywordNames::SCALAR_TYPE_KW);
}
   
