//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// Contributor: David A. Horner (DAH) - http://dave.thehorners.com
// 
//*************************************************************************
// $Id: ossimImageDataFactory.cpp 10484 2007-02-13 11:54:58Z dburken $
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimU8ImageData.h>
#include <ossim/imaging/ossimU11ImageData.h>
#include <ossim/imaging/ossimU16ImageData.h>
#include <ossim/imaging/ossimS16ImageData.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageSource.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimScalarTypeLut.h>

// Static trace for debugging
static ossimTrace traceDebug("ossimImageDataFactory:debug");

ossimImageDataFactory* ossimImageDataFactory::theInstance = 0;
ossimImageDataFactory::ossimImageDataFactory()
{
   theInstance = 0;
}

ossimImageDataFactory::~ossimImageDataFactory()
{
   if(theInstance)
   {
      delete theInstance;
      theInstance = 0;
   }
}

ossimImageDataFactory* ossimImageDataFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimImageDataFactory;
   }

   return theInstance;
}

ossimRefPtr<ossimImageData> ossimImageDataFactory::create(
   ossimSource* owner,
   ossimScalarType scalar,
   ossim_uint32 bands)const
{
   ossimIpt defaultTileSize;
   ossimGetDefaultTileSize(defaultTileSize);
   ossim_uint32 width  = defaultTileSize.x;
   ossim_uint32 height = defaultTileSize.y;
   
   // do some bounds checking and initialize to a default
   bands  = (bands>0)?bands:1;
   scalar = scalar != OSSIM_SCALAR_UNKNOWN?scalar:OSSIM_UINT8;

   if (traceDebug())
   {
      cout << "ossimImageDataFactory::create DEBUG:"
           << "\nCaller:  "
           << (owner ? owner->getClassName().c_str() : "unknown")
           << "\nbands:   " << bands
           << "\nwidth:   " << width
           << "\nheight:  " << height
           << "\nScalar type:  "
           << (ossimScalarTypeLut::instance()->getEntryString(scalar))
           << endl;
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
      
      // ther are bugs in the 16 bit that I don't have time to find.
      // The default image data object works good though.
      //
      // So for now I will comment these out.  I think it has something to
      // do with the normalization it makes everything black
      //
      //
//       case OSSIM_USHORT16:
//       {
//          result = new ossimU16ImageData(owner, bands, width, height);
//          break;
//       }
//       case OSSIM_SSHORT16:
//       {
//          result = new ossimS16ImageData(owner, bands, width, height);
//          break;
//       }
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
   ossimIpt defaultTileSize;
   ossimGetDefaultTileSize(defaultTileSize);
   // do some bounds checking and initialize to a default
   bands  = bands  > 0?bands:1;
   width  = width  > 0?width:defaultTileSize.x;
   height = height > 0?height:defaultTileSize.y;
   scalar = scalar != OSSIM_SCALAR_UNKNOWN?scalar:OSSIM_UINT8;

   if (traceDebug())
   {
      cout << "ossimImageDataFactory::create DEBUG:"
           << "\nCaller:  "
           << (owner ? owner->getClassName().c_str() : "unknown")
           << "\nbands:   " << bands
           << "\nwidth:   " << width
           << "\nheight:  " << height
           << "\nScalar type:  "
           << (ossimScalarTypeLut::instance()->getEntryString(scalar))
           << endl;
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
      
      // ther are bugs in the 16 bit that I don't have time to find.
      // The default image data object works good though.
      //
      // So for now I will comment these out.  I think it has something to
      // do with the normalization it makes everything black
      //
      //
//       case OSSIM_USHORT16:
//       {
//          result = new ossimU16ImageData(owner, bands, width, height);
//          break;
//       }
//       case OSSIM_SSHORT16:
//       {
//          result = new ossimS16ImageData(owner, bands, width, height);
//          break;
//       }
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
   ossimImageSourceInterface* inputSource)const
{
   ossimRefPtr<ossimImageData> result = 0;

   if(inputSource)
   {
      ossimScalarType scalar = inputSource->getOutputScalarType();
      ossim_uint32 width  = inputSource->getTileWidth();
      ossim_uint32 height = inputSource->getTileHeight();
      
      result = create(owner, scalar, bands, width, height);
      
      for(ossim_uint32 band = 0; band < bands; ++band)
      {
         result->setMinPix(inputSource->getMinPixelValue(band),   band);
         result->setMaxPix(inputSource->getMaxPixelValue(band),   band);
         result->setNullPix(inputSource->getNullPixelValue(band), band);
      }
   }
   else
   {
      cerr << "ossimImageDataFactory::create ERROR:"
           << "\nNULL input source!" << endl;
   }
   
   return result;
}


ossimRefPtr<ossimImageData> ossimImageDataFactory::create(
   ossimSource* owner,
   ossimImageSourceInterface* inputSource)const
{
   ossimRefPtr<ossimImageData> result = 0;

   if(inputSource)
   {
      ossimScalarType scalar = inputSource->getOutputScalarType();
      ossim_uint32 bands  = inputSource->getNumberOfOutputBands();
      ossim_uint32 width  = inputSource->getTileWidth();
      ossim_uint32 height = inputSource->getTileHeight();
      
      result = create(owner, scalar, bands, width, height);
      
      for(ossim_uint32 band = 0; band < bands; ++band)
      {
         result->setMinPix(inputSource->getMinPixelValue(band),   band);
         result->setMaxPix(inputSource->getMaxPixelValue(band),   band);
         result->setNullPix(inputSource->getNullPixelValue(band), band);
      }
   }
   else
   {
      cerr << "ossimImageDataFactory::create ERROR:"
           << "\nNULL input source!" << endl;
   }

   return result;
}
