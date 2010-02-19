//-------------------------------------------------------------------
// License:  LGPL.  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
//-------------------------------------------------------------------
//  $Id$
#include <ossim/imaging/ossimTwoColorView.h>
#include <ossim/imaging/ossimImageDataFactory.h>

RTTI_DEF1(ossimTwoColorView,
          "ossimTwoColorView" ,
          ossimImageCombiner);

ossimTwoColorView::ossimTwoColorView()
:ossimImageCombiner(0, 2, 0, true, false) ,
theByPassFlag(true),
theNativeFlag(false),
theNewInput(0),
theOldInput(0),
theNewBufferDestinationIndex(2),
theOldBufferDestinationIndex(0),
theMinBufferDestinationIndex(1){
}

ossim_uint32 ossimTwoColorView::getNumberOfOutputBands() const
{
   if(theByPassFlag||!isSourceEnabled())
   {
      return ossimImageCombiner::getNumberOfOutputBands();
   }
   return 3;
}

ossimScalarType ossimTwoColorView::getOutputScalarType() const
{
   if(theByPassFlag||!isSourceEnabled())
   {
      return ossimImageCombiner::getOutputScalarType();
   }
   return OSSIM_UINT8;
}

void ossimTwoColorView::setIndexMapping(ossim_uint32 newIndex,
                                        ossim_uint32 oldIndex)
{
   if(((newIndex < 3)&&(oldIndex < 3))&&
      (newIndex != oldIndex))
   {
      theNewBufferDestinationIndex = newIndex;
      theOldBufferDestinationIndex = oldIndex;
      if((theNewBufferDestinationIndex != 0)&&
         (theOldBufferDestinationIndex != 0))
      {
         theMinBufferDestinationIndex = 0;
      }
      else if((theNewBufferDestinationIndex != 1)&&
              (theOldBufferDestinationIndex != 1))
      {
         theMinBufferDestinationIndex = 1;
      }
      else if((theNewBufferDestinationIndex != 2)&&
              (theOldBufferDestinationIndex != 2))
      {
         theMinBufferDestinationIndex = 2;
      }
   }
}

double ossimTwoColorView::getNullPixelValue(ossim_uint32 band)const
{
   if(theByPassFlag||!isSourceEnabled())
   {
      return ossimImageCombiner::getNullPixelValue(band);
   }
   return 0;
}

double ossimTwoColorView::getMinPixelValue(ossim_uint32 band)const
{
   if(theByPassFlag||!isSourceEnabled())
   {
      return ossimImageCombiner::getMinPixelValue(band);
   }
   return 1;
}

double ossimTwoColorView::getMaxPixelValue(ossim_uint32 band)const
{
   if(theByPassFlag||!isSourceEnabled())
   {
      return ossimImageCombiner::getMaxPixelValue(band);
   }
   return 256;
}

ossimRefPtr<ossimImageData> ossimTwoColorView::getTile(const ossimIrect& rect,
                                                       ossim_uint32 resLevel)
{
   ossim_uint32 tileIdx = 0;
   if(theByPassFlag||!isSourceEnabled())
   {
      return getNextTile(tileIdx, 0, rect, resLevel);
   }
   if(!theTwoColorTile.valid())
   {
      allocate();
   }
   if(!theTwoColorTile.valid())
   {
      return theTwoColorTile;
   }
   theTwoColorTile->setImageRectangle(rect);
   theTwoColorTile->makeBlank();
   
   ossimRefPtr<ossimImageData> newData = theNewInput->getTile(rect, resLevel);
   ossimRefPtr<ossimImageData> oldData = theOldInput->getTile(rect, resLevel);
   runAlgorithm(newData.get(), oldData.get());
   
#if 0
   // do the new band first
   ossimRefPtr<ossimImageData> newData = getNextNormTile(tileIdx, 0, rect, resLevel);
   newData = newData.valid()?(ossimImageData*)newData->dup():(ossimImageData*)0;
   ossimRefPtr<ossimImageData> oldData = getNextNormTile(tileIdx,rect, resLevel);
   oldData = oldData.valid()?(ossimImageData*)oldData->dup():(ossimImageData*)0;
   
   ossim_float32 newNullPix = 0.0;
   ossim_float32 oldNullPix = 0.0;
   const ossim_float32* newBuf = 0;
   const ossim_float32* oldBuf = 0;
   ossim_float32 tempValue = 0.0;
   ossim_uint32 idx = 0;
   ossim_uint32 maxIdx = theTwoColorTile->getWidth()*theTwoColorTile->getHeight();
   ossim_uint8* newDestBuf = static_cast<ossim_uint8*>(theTwoColorTile->getBuf(theNewBufferDestinationIndex));
   ossim_uint8* oldDestBuf = static_cast<ossim_uint8*>(theTwoColorTile->getBuf(theOldBufferDestinationIndex));
   ossim_uint8* minDestBuf = static_cast<ossim_uint8*>(theTwoColorTile->getBuf(theMinBufferDestinationIndex));
   
   
   if(newData.valid())
   {
      newBuf     = static_cast<ossim_float32*>(newData->getBuf(0));
      newNullPix = static_cast<ossim_float32>(newData->getNullPix(0));
   }
   if(oldData.valid())
   {
      oldBuf = static_cast<ossim_float32*>(oldData->getBuf(0));   
      oldNullPix    = static_cast<ossim_float32>(oldData->getNullPix(0));
   }
   
   if(!newBuf&&!oldBuf)
   {
      return theTwoColorTile;
   }
   if(newBuf&&oldBuf)
   {
      
      for(idx = 0; idx < maxIdx;++idx)
      {
         if((*newBuf == newNullPix)&&
            (*oldBuf == oldNullPix))
         {
            *newDestBuf = 0;
            *oldDestBuf = 0;
            *minDestBuf = 0;
         }
         else
         {
            if(*newBuf == newNullPix)
            {
               *newDestBuf = 1;
            }
            else
            {
               tempValue = (*newBuf)*255;
               if(tempValue < 1) tempValue = 1;
               else if(tempValue > 255) tempValue = 255;
               
               *newDestBuf = (ossim_uint8)(tempValue);
            }
            if(*oldBuf == oldNullPix)
            {
               *oldDestBuf = 1;
            }
            else
            {
               // do old buffer channel
               tempValue = (*oldBuf)*255;
               if(tempValue < 1) tempValue = 1;
               else if(tempValue > 255) tempValue = 255;
               
               *oldDestBuf = (ossim_uint8)(tempValue);
            }
            *minDestBuf = 1;
         }
         ++newBuf;
         ++oldBuf;
         ++minDestBuf;
         ++newDestBuf;
         ++oldDestBuf;
      }
   }
   else if(newBuf)
   {
      for(idx = 0; idx < maxIdx;++idx)
      {
         if(*newBuf == newNullPix)
         {
            *newDestBuf = 0;
            *oldDestBuf = 0;
            *minDestBuf = 0;
         }
         else
         {
            tempValue = (*newBuf)*255;
            if(tempValue < 1) tempValue = 1;
            else if(tempValue > 255) tempValue = 255;
            
            *newDestBuf = (ossim_uint8)(tempValue);
            *oldDestBuf = 1;
            *minDestBuf = 1;
         }
         ++newBuf;
         ++minDestBuf;
         ++newDestBuf;
         ++oldDestBuf;
      }
   }
   else if(oldBuf)
   {
      for(idx = 0; idx < maxIdx;++idx)
      {
         if(*oldBuf == oldNullPix)
         {
            *newDestBuf = 0;
            *oldDestBuf = 0;
            *minDestBuf = 0;
         }
         else
         {
            tempValue = (*oldBuf)*255;
            if(tempValue < 1) tempValue = 1;
            else if(tempValue > 255) tempValue = 255;
            
            *oldDestBuf = (ossim_uint8)(tempValue);
            *newDestBuf = 1;
            *minDestBuf = 1;
         }
         ++oldBuf;
         ++minDestBuf;
         ++newDestBuf;
         ++oldDestBuf;
      }
   }
   
#endif
   theTwoColorTile->validate();
   return theTwoColorTile;
   
}
void ossimTwoColorView::runAlgorithm(ossimImageData* newData, ossimImageData* oldData)
{
   if(theNativeFlag)
   {
      runNative8(newData, oldData);
   }
   else 
   {
      runNorm(newData, oldData);
   }
}

void ossimTwoColorView::runNative8(ossimImageData* newData, ossimImageData* oldData)
{
   // do the new band first
   ossim_uint8 newNullPix = 0;
   ossim_uint8 oldNullPix = 0;
   const ossim_uint8* newBuf = 0;
   const ossim_uint8* oldBuf = 0;
   ossim_uint32 idx = 0;
   ossim_uint32 maxIdx = theTwoColorTile->getWidth()*theTwoColorTile->getHeight();
   ossim_uint8* newDestBuf = static_cast<ossim_uint8*>(theTwoColorTile->getBuf(theNewBufferDestinationIndex));
   ossim_uint8* oldDestBuf = static_cast<ossim_uint8*>(theTwoColorTile->getBuf(theOldBufferDestinationIndex));
   ossim_uint8* minDestBuf = static_cast<ossim_uint8*>(theTwoColorTile->getBuf(theMinBufferDestinationIndex));
   
   if(newData)
   {
      newBuf     = static_cast<ossim_uint8*>(newData->getBuf(0));
      newNullPix = static_cast<ossim_uint8>(newData->getNullPix(0));
      
   }
   if(oldData)
   {
      oldBuf = static_cast<ossim_uint8*>(oldData->getBuf(0));   
      oldNullPix    = static_cast<ossim_uint8>(oldData->getNullPix(0));
   }
   if(!newBuf&&!oldBuf)
   {
      return;
   }
   if(newBuf&&oldBuf)
   {
      for(idx = 0; idx < maxIdx;++idx)
      {
         if((*newBuf == newNullPix)&&
            (*oldBuf == oldNullPix))
         {
            *newDestBuf = 0;
            *oldDestBuf = 0;
            *minDestBuf = 0;
         }
         else
         {
            if(*newBuf == newNullPix)
            {
               *newDestBuf = 1;
            }
            else
            {
               *newDestBuf = *newBuf;
            }
            if(*oldBuf == oldNullPix)
            {
               *oldDestBuf = 1;
            }
            else
            {
               *oldDestBuf = *oldBuf;
            }
            *minDestBuf = 1;
         }
         ++newBuf;
         ++oldBuf;
         ++minDestBuf;
         ++newDestBuf;
         ++oldDestBuf;
      }
   }
   else if(newBuf)
   {
      for(idx = 0; idx < maxIdx;++idx)
      {
         if(*newBuf == newNullPix)
         {
            *newDestBuf = 0;
            *oldDestBuf = 0;
            *minDestBuf = 0;
         }
         else
         {
            *newDestBuf = *newBuf;
            *oldDestBuf = 1;
            *minDestBuf = 1;
         }
         ++newBuf;
         ++minDestBuf;
         ++newDestBuf;
         ++oldDestBuf;
      }
   }
   else if(oldBuf)
   {
      for(idx = 0; idx < maxIdx;++idx)
      {
         if(*oldBuf == oldNullPix)
         {
            *newDestBuf = 0;
            *oldDestBuf = 0;
            *minDestBuf = 0;
         }
         else
         {
            *oldDestBuf = *oldBuf;
            *newDestBuf = 1;
            *minDestBuf = 1;
         }
         ++oldBuf;
         ++minDestBuf;
         ++newDestBuf;
         ++oldDestBuf;
      }
   }
}

void ossimTwoColorView::runNorm(ossimImageData* newData, ossimImageData* oldData)
{
   // do the new band first
   ossim_float32 tempValue=0.0;
   ossim_uint8 newNullPix = 0;
   ossim_uint8 oldNullPix = 0;
   std::vector<ossim_float32> newDataBuffer;
   std::vector<ossim_float32> oldDataBuffer;
   
   ossim_float32* newBuf = 0;
   ossim_float32* oldBuf = 0;
   ossim_uint32 idx = 0;
   ossim_uint32 maxIdx = theTwoColorTile->getWidth()*theTwoColorTile->getHeight();
   ossim_uint8* newDestBuf = static_cast<ossim_uint8*>(theTwoColorTile->getBuf(theNewBufferDestinationIndex));
   ossim_uint8* oldDestBuf = static_cast<ossim_uint8*>(theTwoColorTile->getBuf(theOldBufferDestinationIndex));
   ossim_uint8* minDestBuf = static_cast<ossim_uint8*>(theTwoColorTile->getBuf(theMinBufferDestinationIndex));

   if(newData&&newData->getBuf())
   {
      newDataBuffer.resize(theTwoColorTile->getWidth()*theTwoColorTile->getHeight());
      newBuf     = &newDataBuffer.front();
      newData->copyTileBandToNormalizedBuffer(0, newBuf);
      newNullPix = 0;
   }
   if(oldData&&oldData->getBuf())
   {
      oldDataBuffer.resize(theTwoColorTile->getWidth()*theTwoColorTile->getHeight());
      oldBuf     = &oldDataBuffer.front();
      oldData->copyTileBandToNormalizedBuffer(0, oldBuf);
      oldNullPix = 0;
   }
   if(!newBuf&&!oldBuf)
   {
      return;
   }
   if(newBuf&&oldBuf)
   {
      for(idx = 0; idx < maxIdx;++idx)
      {
         if((*newBuf == newNullPix)&&
            (*oldBuf == oldNullPix))
         {
            *newDestBuf = 0;
            *oldDestBuf = 0;
            *minDestBuf = 0;
         }
         else
         {
            if(*newBuf == newNullPix)
            {
               *newDestBuf = 1;
            }
            else
            {
               tempValue = (*newBuf)*255;
               if(tempValue < 1) tempValue = 1;
               else if(tempValue > 255) tempValue = 255;
               
               *newDestBuf = (ossim_uint8)(tempValue);
            }
            if(*oldBuf == oldNullPix)
            {
               *oldDestBuf = 1;
            }
            else
            {
               // do old buffer channel
               tempValue = (*oldBuf)*255;
               if(tempValue < 1) tempValue = 1;
               else if(tempValue > 255) tempValue = 255;
               
               *oldDestBuf = (ossim_uint8)(tempValue);
            }
            *minDestBuf = 1;
         }
         ++newBuf;
         ++oldBuf;
         ++minDestBuf;
         ++newDestBuf;
         ++oldDestBuf;
      }
   }
   else if(newBuf)
   {
      for(idx = 0; idx < maxIdx;++idx)
      {
         if(*newBuf == newNullPix)
         {
            *newDestBuf = 0;
            *oldDestBuf = 0;
            *minDestBuf = 0;
         }
         else
         {
            tempValue = (*newBuf)*255;
            if(tempValue < 1) tempValue = 1;
            else if(tempValue > 255) tempValue = 255;
            
            *newDestBuf = (ossim_uint8)(tempValue);
            *oldDestBuf = 1;
            *minDestBuf = 1;
         }
         ++newBuf;
         ++minDestBuf;
         ++newDestBuf;
         ++oldDestBuf;
      }
   }
   else if(oldBuf)
   {
      for(idx = 0; idx < maxIdx;++idx)
      {
         if(*oldBuf == oldNullPix)
         {
            *newDestBuf = 0;
            *oldDestBuf = 0;
            *minDestBuf = 0;
         }
         else
         {
            tempValue = (*oldBuf)*255;
            if(tempValue < 1) tempValue = 1;
            else if(tempValue > 255) tempValue = 255;
            
            *oldDestBuf = (ossim_uint8)(tempValue);
            *newDestBuf = 1;
            *minDestBuf = 1;
         }
         ++oldBuf;
         ++minDestBuf;
         ++newDestBuf;
         ++oldDestBuf;
      }
   }
}

void ossimTwoColorView::allocate()
{
   theTwoColorTile = ossimImageDataFactory::instance()->create(this, this);
   if(theTwoColorTile.valid())
   {
      theTwoColorTile->initialize();
   }
}


void ossimTwoColorView::initialize()
{
   ossimImageCombiner::initialize();
   theNewInput = 0;
   theOldInput = 0;
   theTwoColorTile = 0;
   theNativeFlag = false;
   theByPassFlag = false;
   if(getNumberOfInputs() < 2)
   {
      theByPassFlag = true;
   }
   else 
   {
      theNewInput = PTR_CAST(ossimImageSource, getInput(0));
      theOldInput = PTR_CAST(ossimImageSource, getInput(1));
      if(!theNewInput||!theOldInput)
      {
         theByPassFlag = true;
      }
      else
      {
         if((theNewInput->getOutputScalarType() == OSSIM_UINT8)&&
            (theOldInput->getOutputScalarType() == OSSIM_UINT8))
         {
            theNativeFlag = true;
         }
      }
   }
}
