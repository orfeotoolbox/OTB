//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// 
//*******************************************************************
//  $Id$

#include <ossim/imaging/ossimNullPixelFlip.h>
#include <ossim/imaging/ossimImageData.h>

RTTI_DEF1(ossimNullPixelFlip, "ossimNullPixelFlip", ossimImageSourceFilter);
ossimNullPixelFlip::ossimNullPixelFlip()
:m_clipMode(ClipMode_BOUNDING_RECT),
m_replacementType(ReplacementType_MINPIX)
{
}

void ossimNullPixelFlip::initialize()
{
}

ossimRefPtr<ossimImageData> ossimNullPixelFlip::getTile(const ossimIrect& tile_rect,
                                                        ossim_uint32 resLevel)
{
   ossimRefPtr<ossimImageData> result = ossimImageSourceFilter::getTile(tile_rect, resLevel);
   if(!isSourceEnabled()||!result.valid())
   {
      return result.get();
   }
   ossimDataObjectStatus status =  result->getDataObjectStatus();
   
   if((status == OSSIM_FULL)||
      (status == OSSIM_NULL))
   {
      return result.get();
   }
   // Call the appropriate load method.
   switch (result->getScalarType())
   {
      case OSSIM_UCHAR:
      {
         flipPixels(ossim_uint8(0), result.get(), resLevel);
         break;
      }
         
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
      {
         flipPixels(ossim_uint16(0), result.get(), resLevel);
         break;
      }
         
      case OSSIM_SSHORT16:
      {
         flipPixels(ossim_sint16(0), result.get(), resLevel);
         break;
      }
      case OSSIM_UINT32:
      {
         flipPixels(ossim_uint32(0), result.get(), resLevel);
         break;
      }
      case OSSIM_SINT32:
      {
         flipPixels(ossim_sint32(0), result.get(), resLevel);
         break;
      }
      case OSSIM_FLOAT32:
      case OSSIM_NORMALIZED_FLOAT:
      {
         flipPixels(ossim_float32(0), result.get(), resLevel);
         break;
      }
         
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
      {
         flipPixels(ossim_float64(0), result.get(), resLevel);
         break;
      }
         
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
//         ossimNotify(ossimNotifyLevel_WARN)
//         << "ossimPixelFlipper::getTile Unsupported scalar type!" << endl;
         break;
      }
   }
   
   return result;
}

template <class T>
void ossimNullPixelFlip::flipPixels(T /* dummy */,
                                   ossimImageData* inputTile,
                                   ossim_uint32 resLevel)
{
   if (!inputTile) return;
   ossimDataObjectStatus status =  inputTile->getDataObjectStatus();
   ossimIrect rect = inputTile->getImageRectangle();
   ossimIrect imageBounds = getBoundingRect(resLevel);
   ossimIrect clipRect;
   if(!rect.intersects(imageBounds))
   {
      return;
   }
   clipRect = rect.clipToRect(imageBounds);
   if((status == OSSIM_EMPTY)||
      (status == OSSIM_PARTIAL))
   {
      if(!rect.completely_within(imageBounds))
      {
         ossim_uint32 bands = inputTile->getNumberOfBands();
         ossimIpt origin = clipRect.ul() - rect.ul();
         ossim_uint32 bandIdx = 0;
         ossim_uint32 inputW = inputTile->getWidth();
         ossim_uint32 originOffset = origin.y*inputW + origin.x;
         ossim_uint32 w = clipRect.width();
         ossim_uint32 h = clipRect.height();
         ossim_uint32 x = 0;
         ossim_uint32 y = 0;
         for(bandIdx = 0; bandIdx < bands; ++bandIdx)
         {
            T* bandPtr = static_cast<T*>(inputTile->getBuf(bandIdx)) + originOffset;
            T  nullValue = static_cast<T>(inputTile->getNullPix(bandIdx));
            T  replaceValue = (m_replacementType==ReplacementType_MINPIX?static_cast<T>(inputTile->getMinPix(bandIdx)):
                               static_cast<T>(inputTile->getMaxPix(bandIdx)));
            for(y = 0; y < h; ++y)
            {
               for(x = 0; x < w; ++x)
               {
                  if(bandPtr[x] == nullValue)
                  {
                     bandPtr[x] = replaceValue;
                  }
               }
               bandPtr += inputW;
            }
         }
      }
      else
      {
         ossim_uint32 bands = inputTile->getNumberOfBands();
         ossim_uint32 bandIdx = 0;
         ossim_uint32 size = inputTile->getWidth()*inputTile->getHeight();
         for(bandIdx = 0; bandIdx < bands; ++bandIdx)
         {
            T* bandPtr = static_cast<T*>(inputTile->getBuf(bandIdx));
            T  nullValue = static_cast<T>(inputTile->getNullPix(bandIdx));
            T  replaceValue = (m_replacementType==ReplacementType_MINPIX?static_cast<T>(inputTile->getMinPix(bandIdx)):
                               static_cast<T>(inputTile->getMaxPix(bandIdx)));
                               
            ossim_uint32 idx = 0;
            for(idx = 0; idx < size;++idx)
            {
               if((*bandPtr) == nullValue)
               {
                  (*bandPtr) = replaceValue;
               }
               ++bandPtr;
            }
         }
         inputTile->setDataObjectStatus(OSSIM_FULL);
      }
   }
}
