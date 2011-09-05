//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// Contributor: David A. Horner (DAH) - http://dave.thehorners.com
// 
//*************************************************************************
// $Id: ossimImageDataFactory.cpp 19900 2011-08-04 14:19:57Z dburken $

#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimU8ImageData.h>
#include <ossim/imaging/ossimU11ImageData.h>
#include <ossim/imaging/ossimU16ImageData.h>
#include <ossim/imaging/ossimS16ImageData.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageSource.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimScalarTypeLut.h>

// Static trace for debugging
static ossimTrace traceDebug("ossimImageDataFactory:debug");

ossimImageDataFactory* ossimImageDataFactory::theInstance = 0;
OpenThreads::Mutex ossimImageDataFactory::theInstanceMutex;
ossimImageDataFactory::ossimImageDataFactory() 
{
   theInstance = 0;
}

ossimImageDataFactory::~ossimImageDataFactory()
{
   if(theInstance)
   {
      //delete theInstance;
      theInstance = 0;
   }
}

ossimImageDataFactory* ossimImageDataFactory::instance()
{
   theInstanceMutex.lock();
   if(!theInstance)
   {
      theInstance = new ossimImageDataFactory;
   }
   theInstanceMutex.unlock();
   return theInstance;
}

ossimRefPtr<ossimImageData> ossimImageDataFactory::create(
   ossimSource* owner,
   ossimScalarType scalar,
   ossim_uint32 bands)const
{
   ossimIpt tileSize;
   ossim::defaultTileSize(tileSize);
   ossim_uint32 width  = tileSize.x;
   ossim_uint32 height = tileSize.y;
   
   // do some bounds checking and initialize to a default
   bands  = (bands>0)?bands:1;
   scalar = scalar != OSSIM_SCALAR_UNKNOWN?scalar:OSSIM_UINT8;

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimImageDataFactory::create DEBUG:"
         << "\nCaller:  "
         << (owner ? owner->getClassName().c_str() : "unknown")
         << "\nbands:   " << bands
         << "\nwidth:   " << width
         << "\nheight:  " << height
         << "\nScalar type:  "
         << (ossimScalarTypeLut::instance()->getEntryString(scalar))
         << std::endl;
   }
   
   ossimRefPtr<ossimImageData> result = 0;
   switch(scalar)
   {
      case OSSIM_UINT8:
      {
         result = new ossimU8ImageData(owner, bands, width, height);
         break;
      }
      case OSSIM_USHORT11:
      {
         result = new ossimU11ImageData(owner, bands, width, height);
         break;
      }
      case OSSIM_UINT16:
      {
         result = new ossimU16ImageData(owner, bands, width, height);
         break;
      }
      case OSSIM_SINT16:
      {
         result = new ossimS16ImageData(owner, bands, width, height);
         break;
      }
      default:
      {
         // create a generic image data implementation.
         result = new ossimImageData(owner, scalar, bands, width, height);
         break;
      }
   }

   return result;
}

ossimRefPtr<ossimImageData> ossimImageDataFactory::create(
   ossimSource* owner,
   ossimScalarType scalar,
   ossim_uint32 bands,
   ossim_uint32 width,
   ossim_uint32 height)const
{
   ossimIpt tileSize;
   ossim::defaultTileSize(tileSize);
   // do some bounds checking and initialize to a default
   bands  = bands  > 0?bands:1;
   width  = width  > 0?width:tileSize.x;
   height = height > 0?height:tileSize.y;
   scalar = scalar != OSSIM_SCALAR_UNKNOWN?scalar:OSSIM_UINT8;

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimImageDataFactory::create DEBUG:"
         << "\nCaller:  "
         << (owner ? owner->getClassName().c_str() : "unknown")
         << "\nbands:   " << bands
         << "\nwidth:   " << width
         << "\nheight:  " << height
         << "\nScalar type:  "
         << (ossimScalarTypeLut::instance()->getEntryString(scalar))
         << std::endl;
   }
   
   ossimRefPtr<ossimImageData> result = 0;
   switch(scalar)
   {
      case OSSIM_UINT8:
      {
         result = new ossimU8ImageData(owner, bands, width, height);
         break;
      }
      case OSSIM_USHORT11:
      {
         result = new ossimU11ImageData(owner, bands, width, height);
         break;
      }
      case OSSIM_UINT16:
      {
         result = new ossimU16ImageData(owner, bands, width, height);
         break;
      }
      case OSSIM_SINT16:
      {
         result = new ossimS16ImageData(owner, bands, width, height);
         break;
      }
      default:
      {
         // create a generic image data implementation.
         result = new ossimImageData(owner, scalar, bands, width, height);
         break;
      }
   }

   return result;
}

ossimRefPtr<ossimImageData> ossimImageDataFactory::create(
   ossimSource* owner,
   ossim_uint32 bands,
   ossimImageSource* inputSource)const
{
   ossimRefPtr<ossimImageData> result = 0;

   if(inputSource)
   {
      ossimScalarType scalar = inputSource->getOutputScalarType();
      ossim_uint32 width  = inputSource->getTileWidth();
      ossim_uint32 height = inputSource->getTileHeight();

      result = create(owner, scalar, bands, width, height);
      if ( result.valid() )
      {
         for(ossim_uint32 band = 0; band < bands; ++band)
         {
            result->setMinPix(inputSource->getMinPixelValue(band),   band);
            result->setMaxPix(inputSource->getMaxPixelValue(band),   band);
            result->setNullPix(inputSource->getNullPixelValue(band), band);
         }
      }
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimImageDataFactory::create ERROR:"
         << "\nNULL input source!" << std::endl;
   }
   
   return result;
}


ossimRefPtr<ossimImageData> ossimImageDataFactory::create(
   ossimSource* owner,
   ossimImageSource* inputSource)const
{
   ossimRefPtr<ossimImageData> result = 0;

   if(inputSource)
   {
      ossimScalarType scalar = inputSource->getOutputScalarType();
      ossim_uint32 bands  = inputSource->getNumberOfOutputBands();
      ossim_uint32 width  = inputSource->getTileWidth();
      ossim_uint32 height = inputSource->getTileHeight();

      result = create(owner, scalar, bands, width, height);
      if ( result.valid() )
      {
         for(ossim_uint32 band = 0; band < bands; ++band)
         {
            result->setMinPix(inputSource->getMinPixelValue(band),   band);
            result->setMaxPix(inputSource->getMaxPixelValue(band),   band);
            result->setNullPix(inputSource->getNullPixelValue(band), band);
         }
      }
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimImageDataFactory::create ERROR:"
         << "\nNULL input source!" << std::endl;
   }

   return result;
}
