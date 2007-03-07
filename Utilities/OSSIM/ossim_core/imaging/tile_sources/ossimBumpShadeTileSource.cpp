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
// $Id: ossimBumpShadeTileSource.cpp,v 1.43 2005/07/22 14:57:09 dburken Exp $

#include <imaging/tile_sources/ossimBumpShadeTileSource.h>
#include <imaging/factory/ossimImageDataFactory.h>
#include <imaging/ossimImageData.h>
#include <imaging/patch/ossimTilePatch.h>
#include <imaging/ossimImageData.h>
#include <base/data_types/ossimKeywordlist.h>
#include <base/common/ossimKeywordNames.h>
#include <base/data_types/ossimKeyword.h>
#include <base/data_types/ossimMatrix3x3.h>
#include <base/data_types/color_space/ossimRgbVector.h>
#include <imaging/tile_sources/ossimImageToPlaneNormalFilter.h>

RTTI_DEF1(ossimBumpShadeTileSource,
          "ossimBumpShadeTileSource",
          ossimImageCombiner);


ossimBumpShadeTileSource::ossimBumpShadeTileSource()
   :ossimImageCombiner(NULL, 2, 0, true, false),
    theTile(NULL),
    theLightSourceElevationAngle(45.0),
    theLightSourceAzimuthAngle(45.0),
    theLightDirection(3)
{
   initialize();
}

ossimBumpShadeTileSource::~ossimBumpShadeTileSource()
{
}

ossimRefPtr<ossimImageData> ossimBumpShadeTileSource::getTile(
   const  ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
   if(!getInput(0)) return NULL;

   if(!theTile.get())
   {
      initialize();
   }
   
   ossimImageSourceInterface* normalSource =
      PTR_CAST(ossimImageSourceInterface, getInput(0));
   ossimImageSourceInterface* colorSource =
      PTR_CAST(ossimImageSourceInterface, getInput(1));
   
   if(!theTile.get())
   {
      return ossimRefPtr<ossimImageData>();
   }

   long w = tileRect.width();
   long h = tileRect.height();
   ossimIpt origin = tileRect.ul();
   
   theTile->setOrigin(origin);

   long tileW = theTile->getWidth();
   long tileH = theTile->getHeight();
   if((w != tileW)||
      (h != tileH))
   {
      theTile->setWidth(w);
      theTile->setHeight(h);
      if((w*h)!=(tileW*tileH))
      {
         theTile->initialize();
      }
   }
 
   ossimRefPtr<ossimImageData> inputTile = NULL;

   if(isSourceEnabled())
   {
      theTile->makeBlank();
      
      if(colorSource)
      {
         
         inputTile = colorSource->getTile(tileRect, resLevel);
      }
      ossimRefPtr<ossimImageData> normalData = normalSource->getTile(tileRect, resLevel);
      if(!normalData)
      {
         return ossimRefPtr<ossimImageData>();
      }
      if ( (normalData->getDataObjectStatus() == OSSIM_NULL) ||
           (normalData->getDataObjectStatus() == OSSIM_EMPTY)||
           (normalData->getNumberOfBands() != 3)||
           (normalData->getScalarType() != OSSIM_DOUBLE))
      {
         return ossimRefPtr<ossimImageData>();
      }
      ossim_float64* normalBuf[3];
      normalBuf[0] = static_cast<ossim_float64*>(normalData->getBuf(0));
      normalBuf[1] = static_cast<ossim_float64*>(normalData->getBuf(1));
      normalBuf[2] = static_cast<ossim_float64*>(normalData->getBuf(2));
      ossim_float64 normalNp = normalData->getNullPix(0);
      // if we have some color data then use it for the bump
      // else we will default to a grey scale bump shade
      //
      if(inputTile.get() &&
         (inputTile->getDataObjectStatus() != OSSIM_EMPTY) &&
         (inputTile->getDataObjectStatus() != OSSIM_NULL))
         
      {
         switch(inputTile->getScalarType())
         {
            case OSSIM_UCHAR:
            {
               ossim_uint8* resultBuf[3];
               ossim_uint8* colorBuf[3];
               resultBuf[0] = static_cast<ossim_uint8*>(theTile->getBuf(0));
               resultBuf[1] = static_cast<ossim_uint8*>(theTile->getBuf(1));
               resultBuf[2] = static_cast<ossim_uint8*>(theTile->getBuf(2));
               colorBuf[0]  = static_cast<ossim_uint8*>(inputTile->getBuf(0));
               if(inputTile->getBuf(1))
               {
                  colorBuf[1] = static_cast<ossim_uint8*>(inputTile->getBuf(1));
               }
               else
               {
                  colorBuf[1] = colorBuf[0];
               }
               if(inputTile->getBuf(2))
               {
                  colorBuf[2] = static_cast<ossim_uint8*>(inputTile->getBuf(2));
               }
               else
               {
                  colorBuf[2] = colorBuf[0];
               }
            
               long h = theTile->getHeight();
               long w = theTile->getWidth();
               for(long y = 0; y < h; ++y)
               {
                  for(long x = 0; x < w; ++x)
                  {
                     if((*normalBuf[0] != normalNp) &&
                        (*normalBuf[1] != normalNp) &&
                        (*normalBuf[2] != normalNp) )
                     {
                        if((*colorBuf[0])||(*colorBuf[1])||(*colorBuf[2]))
                        {
                           computeColor(*resultBuf[0],
                                        *resultBuf[1],
                                        *resultBuf[2],
                                        *normalBuf[0],
                                        *normalBuf[1],
                                        *normalBuf[2],
                                        *colorBuf[0],
                                        *colorBuf[1],
                                        *colorBuf[2]);
                        }
                        else 
                        {
                           computeColor(*resultBuf[0],
                                        *resultBuf[1],
                                        *resultBuf[2],
                                        *normalBuf[0],
                                        *normalBuf[1],
                                        *normalBuf[2],
                                        255,
                                        255,
                                        255);
                        }
                     }
                     else
                     {
                        *resultBuf[0] = *colorBuf[0];
                        *resultBuf[1] = *colorBuf[1];
                        *resultBuf[2] = *colorBuf[2];
                     }
                     resultBuf[0]++;
                     resultBuf[1]++;
                     resultBuf[2]++;
                     colorBuf[0]++;
                     colorBuf[1]++;
                     colorBuf[2]++;
                     normalBuf[0]++;
                     normalBuf[1]++;
                     normalBuf[2]++;
                  }
               }
               break;
            }
            default:
            {
               ossimNotify(ossimNotifyLevel_NOTICE)
                  << "ossimBumpShadeTileSource::getTile NOTICE:\n"
                  << "only 8-bit unsigned char is supported." << endl;
            }
         }
      }
      else
      {
         ossim_uint8* resultBuf[3];
         resultBuf[0] = static_cast<ossim_uint8*>(theTile->getBuf(0));
         resultBuf[1] = static_cast<ossim_uint8*>(theTile->getBuf(1));
         resultBuf[2] = static_cast<ossim_uint8*>(theTile->getBuf(2));
         long h = theTile->getHeight();
         long w = theTile->getWidth();
         for(long y = 0; y < h; ++y)
         {
            for(long x = 0; x < w; ++x)
            {
               if((*normalBuf[0] != normalNp) &&
                  (*normalBuf[1] != normalNp) &&
                  (*normalBuf[2] != normalNp) )
               {
                     computeColor(*resultBuf[0],
                                  *resultBuf[1],
                                  *resultBuf[2],
                                  *normalBuf[0],
                                  *normalBuf[1],
                                  *normalBuf[2],
                                  (ossim_uint8)255,
                                  (ossim_uint8)255,
                                  (ossim_uint8)255);
               }
               else
               {
                  *resultBuf[0] = 0;
                  *resultBuf[1] = 0;
                  *resultBuf[2] = 0;
               }
               resultBuf[0]++;
               resultBuf[1]++;
               resultBuf[2]++;
               normalBuf[0]++;
               normalBuf[1]++;
               normalBuf[2]++;
            }
         }
      }      
   }
   theTile->validate();
   return theTile;
}

void ossimBumpShadeTileSource::computeColor(ossim_uint8& r,
                                            ossim_uint8& g,
                                            ossim_uint8& b,
                                            ossim_float64 normalX,
                                            ossim_float64 normalY,
                                            ossim_float64 normalZ,
                                            ossim_uint8 dr,
                                            ossim_uint8 dg,
                                            ossim_uint8 db)const
{
   double c = normalX*theLightDirection[0] +
              normalY*theLightDirection[1] +
              normalZ*theLightDirection[2];
   
   if(fabs(c) > FLT_EPSILON)
   {
      r = ossimRgbVector::clamp(irint(c*dr), 1, 255);
      g = ossimRgbVector::clamp(irint(c*dg), 1, 255);
      b = ossimRgbVector::clamp(irint(c*db), 1, 255);
   }
   else
   {
      r = 1;
      g = 1;
      b = 1;
   }
}

ossimIrect ossimBumpShadeTileSource::getBoundingRect(ossim_uint32 resLevel)const
{
   ossimImageSourceInterface* source = PTR_CAST(ossimImageSourceInterface, getInput(0));
   if(source)
   {
      return source->getBoundingRect(resLevel);
   }
   ossimDrect rect;
   rect.makeNan();
   return rect;
}


void ossimBumpShadeTileSource::getDecimationFactor(ossim_uint32 resLevel,
                                                   ossimDpt& result) const
{
   ossimImageSourceInterface* source = PTR_CAST(ossimImageSourceInterface, getInput(0));
   if(source)
   {
      return source->getDecimationFactor(resLevel, result);
   }
   
   result.makeNan();
}

void ossimBumpShadeTileSource::getDecimationFactors(vector<ossimDpt>& decimations) const
{
   ossimImageSourceInterface* source = PTR_CAST(ossimImageSourceInterface, getInput(0));
   
   if(source)
   {
      return source->getDecimationFactors(decimations);
   }
}

ossim_uint32 ossimBumpShadeTileSource::getNumberOfDecimationLevels()const
{
   ossimImageSourceInterface* source = PTR_CAST(ossimImageSourceInterface, getInput(0));
   
   if(source)
   {
      return source->getNumberOfDecimationLevels();
   }
   
   return 0;
}

ossim_uint32 ossimBumpShadeTileSource::getTileWidth()const
{
   ossimImageSourceInterface* source = PTR_CAST(ossimImageSourceInterface, getInput(0));
   if(source)
   {
      return source->getTileWidth();
   }
   
   return 128;
}

ossim_uint32 ossimBumpShadeTileSource::getTileHeight()const
{
   ossimImageSourceInterface* source = PTR_CAST(ossimImageSourceInterface, getInput(0));
   if(source)
   {
      return source->getTileHeight();
   }
   
   return 128;
}

void ossimBumpShadeTileSource::initialize()
{
  ossimImageCombiner::initialize();
  
  theTile = ossimImageDataFactory::instance()->create(this, this);
  theTile->initialize();
  computeLightDirection();
  
}

void ossimBumpShadeTileSource::computeLightDirection()
{
   NEWMAT::Matrix m = ossimMatrix3x3::createRotationMatrix(theLightSourceElevationAngle,
                                                           0.0,
                                                           theLightSourceAzimuthAngle);
   NEWMAT::ColumnVector v(3);
   v[0] = 0;
   v[1] = 1;
   v[2] = 0;
   v = m*v;
   // reflect Z.  We need the Z pointing up from the surface and not into it.
   //
   ossimColumnVector3d d(v[0], v[1], -v[2]);
   d = d.unit();
   theLightDirection[0] = d[0];
   theLightDirection[1] = d[1];
   theLightDirection[2] = d[2];
}

bool ossimBumpShadeTileSource::loadState(const ossimKeywordlist& kwl,
                                         const char* prefix)
{
   const char* elevationAngle = kwl.find(prefix, ossimKeywordNames::ELEVATION_ANGLE_KW);
   const char* azimuthAngle   = kwl.find(prefix, ossimKeywordNames::AZIMUTH_ANGLE_KW);

   
   if(elevationAngle)
   {
      theLightSourceElevationAngle = ossimString(elevationAngle).toDouble();
   }

   if(azimuthAngle)
   {
      theLightSourceAzimuthAngle = ossimString(azimuthAngle).toDouble();
   }

   computeLightDirection();

   bool result = ossimImageSource::loadState(kwl, prefix);

   theInputListIsFixedFlag  = true;
   theOutputListIsFixedFlag = false;
   if(!getNumberOfInputs()) setNumberOfInputs(2);
   
   return result;
}

bool ossimBumpShadeTileSource::saveState(ossimKeywordlist& kwl,
                                         const char* prefix)const
{
   kwl.add(prefix,
           ossimKeywordNames::ELEVATION_ANGLE_KW,
           theLightSourceElevationAngle,
           true);

   kwl.add(prefix,
           ossimKeywordNames::AZIMUTH_ANGLE_KW,
           theLightSourceAzimuthAngle,
           true);
   
   return ossimImageSource::saveState(kwl, prefix);
}

ossimString ossimBumpShadeTileSource::getShortName()const
{
   return ossimString("bump shader");
}

ossimString ossimBumpShadeTileSource::getLongName()const
{
   return ossimString("Blinn's bump map filter");
}

ossim_uint32 ossimBumpShadeTileSource::getNumberOfOutputBands() const
{
   return 3;
}

ossimScalarType ossimBumpShadeTileSource::getOutputScalarType() const
{
   return OSSIM_UCHAR;
}

double ossimBumpShadeTileSource::getNullPixelValue()const
{
   return 0.0;
}

double ossimBumpShadeTileSource::getMinPixelValue(ossim_uint32 /* band */)const
{
   return 1.0;
}

double ossimBumpShadeTileSource::getMaxPixelValue(ossim_uint32 /* band */)const
{
   return 255.0;
}

double ossimBumpShadeTileSource::getAzimuthAngle()const
{
   return theLightSourceAzimuthAngle;
}

double ossimBumpShadeTileSource::getElevationAngle()const
{
   return theLightSourceElevationAngle;
}

void ossimBumpShadeTileSource::setAzimuthAngle(double angle)
{
   theLightSourceAzimuthAngle = angle;
}

void ossimBumpShadeTileSource::setElevationAngle(double angle)
{
   theLightSourceElevationAngle = angle;
}

bool ossimBumpShadeTileSource::canConnectMyInputTo(ossim_int32 inputIndex,
                                                   const ossimConnectableObject* object)const
{
   return (object&&
           ( (inputIndex>=0) && inputIndex < 2)&&
           PTR_CAST(ossimImageSourceInterface, object));
   
}

void ossimBumpShadeTileSource::connectInputEvent(ossimConnectionEvent& event)
{
   initialize();
}

void ossimBumpShadeTileSource::disconnectInputEvent(ossimConnectionEvent& event)
{
   initialize();
}

void ossimBumpShadeTileSource::propertyEvent(ossimPropertyEvent& event)
{
   initialize();
}

void ossimBumpShadeTileSource::refreshEvent(ossimRefreshEvent& event)
{
   initialize();
}
