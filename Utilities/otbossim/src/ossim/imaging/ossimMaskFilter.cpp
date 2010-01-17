//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
// Description: A brief description of the contents of the file.
//
//*************************************************************************
// $Id: ossimMaskFilter.cpp 13312 2008-07-27 01:26:52Z gpotts $

#include <ossim/imaging/ossimMaskFilter.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageSource.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimNotifyContext.h>

static const char * MASK_FILTER_MASK_TYPE_KW = "mask_type";

RTTI_DEF1(ossimMaskFilter, "ossimMaskFilter", ossimImageCombiner);

ossimMaskFilter::ossimMaskFilter(ossimObject* owner)
  : ossimImageCombiner(owner,
                       2,
                       0,
                       true,
                       false),
    theMaskType(OSSIM_MASK_TYPE_SELECT),
    theTile(NULL)
{
}

ossimMaskFilter::ossimMaskFilter(ossimImageSource* imageSource,
				 ossimImageSource* maskSource)
  :ossimImageCombiner(NULL,
                      2,
                      0,
                      true,
                      false),
   theMaskType(OSSIM_MASK_TYPE_SELECT),
   theTile(NULL)
{
  connectMyInputTo(0, imageSource);
  connectMyInputTo(1, maskSource);
} 

ossimMaskFilter::ossimMaskFilter(ossimObject* owner,
				 ossimImageSource* imageSource,
				 ossimImageSource* maskSource)
  :ossimImageCombiner(owner,
                      2,
                      0,
                      true,
                      false),
   theMaskType(OSSIM_MASK_TYPE_SELECT),
   theTile(NULL)

{
  connectMyInputTo(0, imageSource);
  connectMyInputTo(1, maskSource);
}

ossimMaskFilter::~ossimMaskFilter()
{
}

ossimRefPtr<ossimImageData> ossimMaskFilter::getTile(const ossimIrect& rect,
                                                     ossim_uint32 resLevel)
{
   ossimImageSource* imageSource = PTR_CAST(ossimImageSource,
                                                     getInput(0));
   
   // we will check to see if it's a fileMaskSource
   //
   ossimImageSource* maskSource = PTR_CAST(ossimImageSource,
                                                    getInput(1));
   
   ossimRefPtr<ossimImageData> imageSourceData;
   ossimRefPtr<ossimImageData> maskSourceData;
   
   if(imageSource)
   {
      imageSourceData = imageSource->getTile(rect, resLevel);
   }

   if(!isSourceEnabled())
   {
      return imageSourceData;
   }
   
   if(maskSource)
   {
      maskSourceData = maskSource->getTile(rect, resLevel);
   }

   if (!theTile.valid())
   {
      allocate();
   }

   if(!imageSourceData.valid() || !theTile.valid())
   {
      return ossimRefPtr<ossimImageData>();
   }
   
   theTile->setOrigin(rect.ul());
   if(theTile->getImageRectangle() != rect)
   {
      theTile->setImageRectangle(rect);
      theTile->initialize();
   }
   if(!imageSourceData.valid())
   {
      return theTile;
   }
   if(!maskSourceData.valid()) 
   {
      return imageSourceData;
   }
   
   if(imageSourceData->getDataObjectStatus() != OSSIM_NULL)
   {
      return executeMaskFilter(imageSourceData, maskSourceData);  
   }
   
   return theTile;
}

bool ossimMaskFilter::canConnectMyInputTo(ossim_int32 index,
                                          const ossimConnectableObject* object)const
{
   return (PTR_CAST(ossimImageSource, object)!= NULL);
}

void ossimMaskFilter::initialize()
{
   ossimImageCombiner::initialize();
   if(getInput(0))
   {
      // Force an allocate on the next getTile.
      theTile = NULL;
   }
}

void ossimMaskFilter::allocate()
{
   if(getInput(0))
   {
      theTile = ossimImageDataFactory::instance()->create(this, this);
      theTile->initialize();
   }
}

ossimRefPtr<ossimImageData> ossimMaskFilter::executeMaskFilter(ossimRefPtr<ossimImageData> imageSourceData,
                                                               ossimRefPtr<ossimImageData> maskSourceData)
{
   if(!theTile)
   {
      theTile = (ossimImageData*)imageSourceData->dup();
      if(!theTile->getBuf())
      {
         theTile->initialize();
      }
   }
   else
   {
      ossim_uint32 tw = theTile->getWidth();
      ossim_uint32 th = theTile->getHeight();
      ossim_uint32 dw = imageSourceData->getWidth();
      ossim_uint32 dh = imageSourceData->getHeight();
      
      theTile->setWidthHeight(imageSourceData->getWidth(),
			      imageSourceData->getHeight());
      theTile->setOrigin(imageSourceData->getOrigin());
      if((tw*th) != (dw*dh))
      {
         theTile->initialize();
      }
     theTile->setDataObjectStatus(imageSourceData->getDataObjectStatus());
   }
   theTile->loadTile(imageSourceData.get());
   theTile->setDataObjectStatus(imageSourceData->getDataObjectStatus());
  switch(theMaskType)
  {
  case OSSIM_MASK_TYPE_SELECT:
  {
     return executeMaskFilterSelect(theTile,
                                    maskSourceData);
  }
  case OSSIM_MASK_TYPE_INVERT:
  {
     return executeMaskFilterInvertSelect(theTile,
                                          maskSourceData);
  }
  case OSSIM_MASK_TYPE_WEIGHTED:
  {
     return executeMaskFilterWeighted(theTile,
                                      maskSourceData);
  }
  }
  
  return imageSourceData;
}

ossimRefPtr<ossimImageData> ossimMaskFilter::executeMaskFilterSelect(ossimRefPtr<ossimImageData> imageSourceData,
                                                                     ossimRefPtr<ossimImageData> maskSourceData)
{
   if(maskSourceData->getScalarType() != OSSIM_UCHAR)
   {
      ossimNotify(ossimNotifyLevel_WARN) << "ossimMaskFilter::executeMaskFilterSelect WARNING: Only uchar masks are supported" << endl;
      return imageSourceData;
   }

   switch(imageSourceData->getScalarType())
   {
   case OSSIM_UCHAR:
   {
      executeMaskFilterSelection((ossim_uint8)0,
                                 (ossim_uint8)0,
                                 imageSourceData,
                                 maskSourceData);
      break;
   }
   case OSSIM_USHORT11:
   case OSSIM_USHORT16:
   {
      executeMaskFilterSelection((ossim_uint16)0,
                                 (ossim_uint8)0,
                                 imageSourceData,
                                 maskSourceData);
      break;
   }
   case OSSIM_SSHORT16:
   {
      executeMaskFilterSelection((ossim_sint16)0,
                                 (ossim_uint8)0,
                                 imageSourceData,
                                 maskSourceData);
      break;
   }
   case OSSIM_FLOAT:
   case OSSIM_NORMALIZED_FLOAT:
   {
      executeMaskFilterSelection((float)0,
                                 (ossim_uint8)0,
                                 imageSourceData,
                                 maskSourceData);
      break;
   }
   case OSSIM_DOUBLE:
   case OSSIM_NORMALIZED_DOUBLE:
   {
      executeMaskFilterSelection((double)0,
                                 (ossim_uint8)0,
                                 imageSourceData,
                                 maskSourceData);
      break;
   }
   default:
   {
      ossimNotify(ossimNotifyLevel_WARN) << "ossimMaskFilter::executeMaskFilterSelect WARNING: Unknown scalar type" << endl;
      break;
   }   
   }

   return theTile;
}

ossimRefPtr<ossimImageData> ossimMaskFilter::executeMaskFilterInvertSelect(ossimRefPtr<ossimImageData> imageSourceData,
                                                                           ossimRefPtr<ossimImageData> maskSourceData)
{
   if(maskSourceData->getScalarType() != OSSIM_UCHAR)
   {
      ossimNotify(ossimNotifyLevel_WARN) << "ossimMaskFilter::executeMaskFilterSelect WARNING: Only uchar masks are supported" << endl;
      return imageSourceData;
   }

   switch(imageSourceData->getScalarType())
   {
   case OSSIM_UCHAR:
   {
      executeMaskFilterInvertSelection((ossim_uint8)0,
                                       (ossim_uint8)0,
                                       imageSourceData,
                                       maskSourceData);
      break;
   }
   case OSSIM_USHORT11:
   case OSSIM_USHORT16:
   {
      executeMaskFilterInvertSelection((ossim_uint16)0,
                                       (ossim_uint8)0,
                                       imageSourceData,
                                       maskSourceData);
      break;
   }
   case OSSIM_SSHORT16:
   {
      executeMaskFilterInvertSelection((ossim_sint16)0,
                                       (ossim_uint8)0,
                                       imageSourceData,
                                       maskSourceData);
      break;
   }
   case OSSIM_FLOAT:
   case OSSIM_NORMALIZED_FLOAT:
   {
      executeMaskFilterInvertSelection((float)0,
                                       (ossim_uint8)0,
                                       imageSourceData,
                                       maskSourceData);
      break;
   }
   case OSSIM_DOUBLE:
   case OSSIM_NORMALIZED_DOUBLE:
   {
      executeMaskFilterInvertSelection((double)0,
                                       (ossim_uint8)0,
                                       imageSourceData,
                                       maskSourceData);
      break;
   }
   default:
   {
      ossimNotify(ossimNotifyLevel_WARN) << "ossimMaskFilter::executeMaskFilterSelect WARNING: Unknown scalar type" << endl;
      break;
   }   
   }

   return theTile;
}

ossimRefPtr<ossimImageData> ossimMaskFilter::executeMaskFilterWeighted(ossimRefPtr<ossimImageData> imageSourceData,
                                                                       ossimRefPtr<ossimImageData> maskSourceData)
{
   if(maskSourceData->getScalarType() != OSSIM_UCHAR)
   {
      ossimNotify(ossimNotifyLevel_WARN) << "ossimMaskFilter::executeMaskFilterSelect WARNING: Only uchar masks are supported" << endl;
      return imageSourceData;
   }

   switch(imageSourceData->getScalarType())
   {
   case OSSIM_UCHAR:
   {
      executeMaskFilterWeighted((ossim_uint8)0,
                                (ossim_uint8)0,
                                imageSourceData,
                                maskSourceData);
      break;
   }
   case OSSIM_USHORT11:
   case OSSIM_USHORT16:
   {
      executeMaskFilterWeighted((ossim_uint16)0,
                                (ossim_uint8)0,
                                imageSourceData,
                                maskSourceData);
      break;
   }
   case OSSIM_SSHORT16:
   {
      executeMaskFilterWeighted((ossim_sint16)0,
                                (ossim_uint8)0,
                                imageSourceData,
                                maskSourceData);
      break;
   }
   case OSSIM_FLOAT:
   case OSSIM_NORMALIZED_FLOAT:
   {
      executeMaskFilterWeighted((float)0,
                                (ossim_uint8)0,
                                imageSourceData,
                                maskSourceData);
      break;
   }
   case OSSIM_DOUBLE:
   case OSSIM_NORMALIZED_DOUBLE:
   {
      executeMaskFilterWeighted((double)0,
                                (ossim_uint8)0,
                                imageSourceData,
                                maskSourceData);
      break;
   }
   default:
   {
      ossimNotify(ossimNotifyLevel_WARN) << "ossimMaskFilter::executeMaskFilterSelect WARNING: Unknown scalar type" << endl;
      break;
   }   
   }

   return theTile;
}

template <class inputT, class maskT>
ossimRefPtr<ossimImageData> ossimMaskFilter::executeMaskFilterSelection(inputT dummyInput,
                                                            maskT  dummyMask,
                                                            ossimRefPtr<ossimImageData> imageSourceData,
                                                            ossimRefPtr<ossimImageData> maskSourceData)
{
   ossimDataObjectStatus maskDataStatus  = maskSourceData->getDataObjectStatus();
   ossimDataObjectStatus inputDataStatus = imageSourceData->getDataObjectStatus();

   if( (maskDataStatus == OSSIM_NULL)||
       (maskDataStatus == OSSIM_EMPTY))
   {
      theTile->makeBlank();
      return theTile;
      
   }
   if( (inputDataStatus == OSSIM_NULL)||
       (inputDataStatus == OSSIM_EMPTY)||
       (maskDataStatus == OSSIM_FULL))
   {
      return theTile;
   }
   if(maskDataStatus == OSSIM_FULL)
   {
      return theTile;
   }
   
   ossim_uint32 maskBands  = maskSourceData->getNumberOfBands();
   ossim_uint32 inputBands = imageSourceData->getNumberOfBands();
   if(maskBands&&inputBands)
   {
      ossim_uint32 maxOffset = theTile->getWidth()*theTile->getHeight();
      for(ossim_uint32 band = 0; band < inputBands; ++band)
      {
         maskT*    bufMask = (maskT*)maskSourceData->getBuf();
         inputT* bufIn     = (inputT*)imageSourceData->getBuf(band);
         inputT* bufOut    = (inputT*)theTile->getBuf(band);
         inputT  np        = (inputT)theTile->getNullPix(band);
         ossim_uint32 offset = 0;
         for(offset = 0; offset < maxOffset; ++offset)
         {
            if(*bufMask)
            {
               *bufOut = *bufIn;
            }
            else
            {
               *bufOut = np;
            }
            ++bufOut;
            ++bufIn;
            ++bufMask;
         }
      }
      theTile->validate();
   }
   
   return theTile;
}

template <class inputT, class maskT>
ossimRefPtr<ossimImageData> ossimMaskFilter::executeMaskFilterInvertSelection(inputT dummyInput,
                                                                  maskT  dummyMask,
                                                                  ossimRefPtr<ossimImageData> imageSourceData,
                                                                  ossimRefPtr<ossimImageData> maskSourceData)
{
   ossimDataObjectStatus maskDataStatus  = maskSourceData->getDataObjectStatus();
   ossimDataObjectStatus inputDataStatus = imageSourceData->getDataObjectStatus();

   if((maskDataStatus == OSSIM_NULL) ||
      (maskDataStatus == OSSIM_EMPTY))
   {
      return theTile;
   }
   if( (inputDataStatus == OSSIM_NULL)||
       (inputDataStatus == OSSIM_EMPTY))
   {
      return theTile;
   }
      
   if(maskDataStatus == OSSIM_FULL)
   {
      theTile->makeBlank();
      
      return theTile;
   }
   
   ossim_uint32 maskBands  = maskSourceData->getNumberOfBands();
   ossim_uint32 inputBands = imageSourceData->getNumberOfBands();
   if(maskBands&&inputBands)
   {
      ossim_uint32 maxOffset = theTile->getWidth()*theTile->getHeight();
      for(ossim_uint32 band = 0; band < inputBands; ++band)
      {
         maskT*    bufMask = (maskT*)maskSourceData->getBuf();
         inputT* bufIn     = (inputT*)imageSourceData->getBuf(band);
         inputT* bufOut    = (inputT*)theTile->getBuf(band);
         inputT  np        = (inputT)theTile->getNullPix(band);
         ossim_uint32 offset = 0;
         for(offset = 0; offset < maxOffset; ++offset)
         {
            if(!*bufMask)
            {
               *bufOut = *bufIn;
            }
            else
            {
               *bufOut = np;
            }
            ++bufOut;
            ++bufIn;
            ++bufMask;
         }
      }
      theTile->validate();
   }
   
   return theTile;
}

template <class inputT, class maskT>
ossimRefPtr<ossimImageData> ossimMaskFilter::executeMaskFilterWeighted(inputT dummyInput,
                                                           maskT  dummyMask,
                                                           ossimRefPtr<ossimImageData> imageSourceData,
                                                           ossimRefPtr<ossimImageData> maskSourceData)
{
   ossimDataObjectStatus maskDataStatus  = maskSourceData->getDataObjectStatus();
   ossimDataObjectStatus inputDataStatus = imageSourceData->getDataObjectStatus();

   if( (maskDataStatus == OSSIM_NULL)||
       (maskDataStatus == OSSIM_EMPTY))
   {
      theTile->makeBlank();
      return theTile;
      
   }
   if( (inputDataStatus == OSSIM_NULL)||
       (inputDataStatus == OSSIM_EMPTY))
   {
      return theTile;
   }
   
   ossim_uint32 maskBands  = maskSourceData->getNumberOfBands();
   ossim_uint32 inputBands = imageSourceData->getNumberOfBands();
   if(maskBands&&inputBands)
   {
      ossim_uint32 maxOffset = theTile->getWidth()*theTile->getHeight();
      for(ossim_uint32 band = 0; band < inputBands; ++band)
      {
         maskT*    bufMask = (maskT*)maskSourceData->getBuf();
         inputT* bufIn     = (inputT*)imageSourceData->getBuf(band);
         inputT* bufOut    = (inputT*)theTile->getBuf(band);
         inputT  np        = (inputT)theTile->getNullPix(band);
         inputT  minp      = (inputT)theTile->getMinPix(band);
         ossim_uint32 offset = 0;

         if(inputDataStatus == OSSIM_PARTIAL)
         {
            for(offset = 0; offset < maxOffset; ++offset)
            {
               if(*bufIn != np)
               {
                  *bufOut = (inputT)((*bufIn)*((double)(*bufMask)/255.0));
                  if((*bufOut != np)&&
                     (*bufOut < minp))
                  {
                     *bufOut = minp;
                  }
               }
               else
               {
                  *bufOut = np;
               }
               ++bufOut;
               ++bufIn;
               ++bufMask;
            }
         }
         else
         {
            for(offset = 0; offset < maxOffset; ++offset)
            {
               *bufOut = (inputT)((*bufIn)*((double)(*bufMask)/255.0));
               if((*bufOut != np)&&
                  (*bufOut < minp))
               {
                  *bufOut = minp;
               }
               ++bufOut;
               ++bufIn;
               ++bufMask;
            }
         }
      }
      theTile->validate();
   }
   
   return theTile;
}


ossimIrect ossimMaskFilter::getBoundingRect(ossim_uint32 resLevel)const
{
   ossimIrect result;
   result.makeNan();

   ossimImageSource* imageSource = PTR_CAST(ossimImageSource, getInput(0));
   if(imageSource)
   {
      result = imageSource->getBoundingRect(resLevel);
   }
   return result;
}

bool ossimMaskFilter::loadState(const ossimKeywordlist& kwl,
                                const char* prefix)
{
   bool result = ossimImageCombiner::loadState(kwl, prefix);
   
   theInputListIsFixedFlag  = true;
   theOutputListIsFixedFlag = false;
   setNumberOfInputs(2);

   ossimString selectionType = kwl.find(prefix, MASK_FILTER_MASK_TYPE_KW);

   selectionType = selectionType.upcase();
   if(selectionType != "")
   {
      if(selectionType == "SELECT")
      {
         theMaskType = OSSIM_MASK_TYPE_SELECT;
      }
      else if(selectionType == "INVERT")
      {
         theMaskType = OSSIM_MASK_TYPE_INVERT;
      }
      else if(selectionType == "WEIGHTED")
      {
         theMaskType = OSSIM_MASK_TYPE_WEIGHTED;
      }
   }

   return result;
}

bool ossimMaskFilter::saveState(ossimKeywordlist& kwl,
                                const char* prefix)const
{
   ossimString maskType;

   switch(theMaskType)
   {
   case OSSIM_MASK_TYPE_SELECT:
   {
      maskType = "select";
      break;
   }
   case OSSIM_MASK_TYPE_INVERT:
   {
      maskType = "invert";
      break;
   }
   case OSSIM_MASK_TYPE_WEIGHTED:
   {
      maskType = "weighted";
      break;
   }
   }
   
   kwl.add(prefix,
           MASK_FILTER_MASK_TYPE_KW,
           maskType.c_str(),
           true);
   
   return ossimImageCombiner::saveState(kwl, prefix);
}
void ossimMaskFilter::getDecimationFactor(ossim_uint32 resLevel,
                                          ossimDpt& result)const
{
   ossimImageSource* input = PTR_CAST(ossimImageSource,
                                               getInput(0));
   if(input)
   {
      input->getDecimationFactor(resLevel, result);
   }
}

void ossimMaskFilter::getDecimationFactors(vector<ossimDpt>& decimations)const
{
   ossimImageSource* input = PTR_CAST(ossimImageSource,
                                               getInput(0));
   if(input)
   {
      input->getDecimationFactors(decimations);
   }     
}

ossim_uint32 ossimMaskFilter::getNumberOfDecimationLevels()const
{
   ossimImageSource* input = PTR_CAST(ossimImageSource,
                                               getInput(0));
   if(input)
   {
      return input->getNumberOfDecimationLevels();
   } 
   return 0;
}

ossim_uint32 ossimMaskFilter::getNumberOfOutputBands() const
{
   ossimImageSource* input = PTR_CAST(ossimImageSource,
                                               getInput(0));
   if(input)
   {
      return input->getNumberOfOutputBands();
   }
   return 0;
}

ossimScalarType ossimMaskFilter::getOutputScalarType() const
{
   ossimImageSource* input = PTR_CAST(ossimImageSource,
                                               getInput(0));
   if(input)
   {
      return input->getOutputScalarType();
   }
   
   return OSSIM_SCALAR_UNKNOWN;
}

void ossimMaskFilter::getValidImageVertices(
   vector<ossimIpt>& validVertices,
   ossimVertexOrdering ordering,
   ossim_uint32 resLevel)const
{
   ossimImageSource* input = PTR_CAST(ossimImageSource,
                                               getInput(0));
   if(input)
   {
      input->getValidImageVertices(validVertices, ordering, resLevel);
   }
}

ossim_uint32 ossimMaskFilter::getTileWidth() const
{
   ossimImageSource* input = PTR_CAST(ossimImageSource,
                                               getInput(0));
   if(input)
   {
      return input->getTileWidth();
   }
   
   return 0;
}

ossim_uint32 ossimMaskFilter::getTileHeight() const
{
   ossimImageSource* input = PTR_CAST(ossimImageSource,
                                               getInput(0));
   if(input)
   {
      return input->getTileHeight();
   }
   return 0;
}

double ossimMaskFilter::getNullPixelValue(ossim_uint32 band)const
{
   ossimImageSource* input = PTR_CAST(ossimImageSource,
                                               getInput(0));
   if(input)
   {
      return input->getNullPixelValue(band);
   }
   return 0;
}

double ossimMaskFilter::getMinPixelValue(ossim_uint32 band)const
{
   ossimImageSource* input = PTR_CAST(ossimImageSource,
                                               getInput(0));
   if(input)
   {
      return input->getMinPixelValue(band);
   }
   return 0;
}

double ossimMaskFilter::getMaxPixelValue(ossim_uint32 band)const
{
   ossimImageSource* input = PTR_CAST(ossimImageSource,
                                               getInput(0));
   if(input)
   {
      return input->getMaxPixelValue(band);
   }
   return 0;
}
