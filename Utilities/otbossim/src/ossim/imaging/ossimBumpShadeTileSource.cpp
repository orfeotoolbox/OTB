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
// $Id: ossimBumpShadeTileSource.cpp 20198 2011-11-03 13:23:40Z dburken $

#include <ossim/imaging/ossimBumpShadeTileSource.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimTilePatch.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/base/ossimColumnVector3d.h>
#include <ossim/base/ossimNumericProperty.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimKeyword.h>
#include <ossim/base/ossimMatrix3x3.h>
#include <ossim/base/ossimRgbVector.h>

static const char COLOR_RED_KW[]   = "color_red";
static const char COLOR_GREEN_KW[] = "color_green";
static const char COLOR_BLUE_KW[]  = "color_blue";

RTTI_DEF1(ossimBumpShadeTileSource,
          "ossimBumpShadeTileSource",
          ossimImageCombiner);


ossimBumpShadeTileSource::ossimBumpShadeTileSource()
   :ossimImageCombiner(0, 2, 0, true, false),
    m_tile(0),
    m_lightSourceElevationAngle(45.0),
    m_lightSourceAzimuthAngle(45.0),
    m_lightDirection(3),
    m_r(255),
    m_g(255),
    m_b(255)
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
   if(!getInput(0)) return 0;

   if(!m_tile.get())
   {
      allocate();
   }
   if(!m_tile.valid())
   {
      return m_tile;
   }
   ossimImageSource* normalSource =
      PTR_CAST(ossimImageSource, getInput(0));
   ossimImageSource* colorSource =
      PTR_CAST(ossimImageSource, getInput(1));
   
   if(!m_tile.get())
   {
      return ossimRefPtr<ossimImageData>();
   }

   m_tile->setImageRectangle(tileRect);
 
   ossimRefPtr<ossimImageData> inputTile = 0;

   if(isSourceEnabled())
   {
      m_tile->makeBlank();
      
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
               resultBuf[0] = static_cast<ossim_uint8*>(m_tile->getBuf(0));
               resultBuf[1] = static_cast<ossim_uint8*>(m_tile->getBuf(1));
               resultBuf[2] = static_cast<ossim_uint8*>(m_tile->getBuf(2));
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
            
               long h = m_tile->getHeight();
               long w = m_tile->getWidth();
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
                                        m_r,
                                        m_g,
                                        m_b);
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
         resultBuf[0] = static_cast<ossim_uint8*>(m_tile->getBuf(0));
         resultBuf[1] = static_cast<ossim_uint8*>(m_tile->getBuf(1));
         resultBuf[2] = static_cast<ossim_uint8*>(m_tile->getBuf(2));
         long h = m_tile->getHeight();
         long w = m_tile->getWidth();
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
                                  m_r,
                                  m_g,
                                  m_b);
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
   m_tile->validate();
   return m_tile;
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
   double c = /*fabs*/(normalX*m_lightDirection[0] +
                   normalY*m_lightDirection[1] +
                   normalZ*m_lightDirection[2]);
   
   r = ossimRgbVector::clamp(ossim::round<int>(c*dr), 1, 255);
   g = ossimRgbVector::clamp(ossim::round<int>(c*dg), 1, 255);
   b = ossimRgbVector::clamp(ossim::round<int>(c*db), 1, 255);
}

void ossimBumpShadeTileSource::initialize()
{
   ossimImageCombiner::initialize();

   ossimImageSource* normalSource = dynamic_cast<ossimImageSource*>( getInput(0) );
   if ( normalSource )
   {
      normalSource->initialize();
   }
   
   ossimImageSource* colorSource = dynamic_cast<ossimImageSource*>( getInput(1) );
   if ( colorSource )
   {
      colorSource->initialize();
   }
   
   m_tile = 0;
   
   computeLightDirection();
}

void ossimBumpShadeTileSource::allocate()
{
   m_tile = ossimImageDataFactory::instance()->create(this, this);
   m_tile->initialize();
}

void ossimBumpShadeTileSource::computeLightDirection()
{
   NEWMAT::Matrix m = ossimMatrix3x3::createRotationMatrix(m_lightSourceElevationAngle,
                                                           0.0,
                                                           -m_lightSourceAzimuthAngle); // make positive rotation clockwise for azimuth
   NEWMAT::ColumnVector v(3);
   v[0] = 0;
   v[1] = 1;
   v[2] = 0;
   v = m*v;
   // reflect Z.  We need the Z pointing up from the surface and not into it.
   //
   ossimColumnVector3d d(v[0], v[1], -v[2]);
   d = d.unit();
   m_lightDirection[0] = d[0];
   m_lightDirection[1] = d[1];
   m_lightDirection[2] = d[2];
}

bool ossimBumpShadeTileSource::loadState(const ossimKeywordlist& kwl,
                                         const char* prefix)
{
   const char* elevationAngle = kwl.find(prefix, ossimKeywordNames::ELEVATION_ANGLE_KW);
   const char* azimuthAngle   = kwl.find(prefix, ossimKeywordNames::AZIMUTH_ANGLE_KW);
   
   
   if(elevationAngle)
   {
      m_lightSourceElevationAngle = ossimString(elevationAngle).toDouble();
   }

   if(azimuthAngle)
   {
      m_lightSourceAzimuthAngle = ossimString(azimuthAngle).toDouble();
   }

   const char* lookup = kwl.find(prefix, COLOR_RED_KW);
   if (lookup)
   {
      m_r = ossimString(lookup).toUInt8();
   }
   
   lookup = kwl.find(prefix, COLOR_GREEN_KW);
   if (lookup)
   {
      m_g = ossimString(lookup).toUInt8();
   }
   
   lookup = kwl.find(prefix, COLOR_BLUE_KW);
   if (lookup)
   {
      m_b = ossimString(lookup).toUInt8();
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
           m_lightSourceElevationAngle,
           true);

   kwl.add(prefix,
           ossimKeywordNames::AZIMUTH_ANGLE_KW,
           m_lightSourceAzimuthAngle,
           true);

   kwl.add(prefix, COLOR_RED_KW,   m_r, true);
   kwl.add(prefix, COLOR_GREEN_KW, m_g, true);
   kwl.add(prefix, COLOR_BLUE_KW,  m_b, true);
   
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
   return m_lightSourceAzimuthAngle;
}

double ossimBumpShadeTileSource::getElevationAngle()const
{
   return m_lightSourceElevationAngle;
}

void ossimBumpShadeTileSource::setAzimuthAngle(double angle)
{
   m_lightSourceAzimuthAngle = angle;
}

void ossimBumpShadeTileSource::setElevationAngle(double angle)
{
   m_lightSourceElevationAngle = angle;
}

bool ossimBumpShadeTileSource::canConnectMyInputTo(ossim_int32 inputIndex,
                                                   const ossimConnectableObject* object)const
{
   return (object&&
           ( (inputIndex>=0) && inputIndex < 2)&&
           PTR_CAST(ossimImageSource, object));
   
}

void ossimBumpShadeTileSource::connectInputEvent(ossimConnectionEvent& /* event */)
{
   initialize();
}

void ossimBumpShadeTileSource::disconnectInputEvent(ossimConnectionEvent& /* event */)
{
   initialize();
}

void ossimBumpShadeTileSource::propertyEvent(ossimPropertyEvent& /* event */)
{
   initialize();
}

void ossimBumpShadeTileSource::refreshEvent(ossimRefreshEvent& /* event */)
{
   initialize();
}

void ossimBumpShadeTileSource::setProperty(ossimRefPtr<ossimProperty> property)
{
   ossimString name = property->getName();
   if(name == "lightSourceElevationAngle")
   {
      m_lightSourceElevationAngle = property->valueToString().toDouble();
   }
   else if(name == "lightSourceAzimuthAngle")
   {
      m_lightSourceAzimuthAngle = property->valueToString().toDouble();
   }
   else
   {
      ossimImageCombiner::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimBumpShadeTileSource::getProperty(const ossimString& name)const
{
   if(name == "lightSourceElevationAngle")
   {
      ossimProperty* prop = new ossimNumericProperty(name, ossimString::toString(m_lightSourceElevationAngle), 0.0, 90.0);
      prop->setCacheRefreshBit();
      return prop;
   }
   else if(name == "lightSourceAzimuthAngle")
   {
      ossimProperty* prop = new ossimNumericProperty(name, ossimString::toString(m_lightSourceAzimuthAngle), 0, 360);
      prop->setCacheRefreshBit();
      return prop;
   }
   
   return ossimImageCombiner::getProperty(name);
}

void ossimBumpShadeTileSource::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimImageCombiner::getPropertyNames(propertyNames);
   propertyNames.push_back("lightSourceElevationAngle");
   propertyNames.push_back("lightSourceAzimuthAngle");
}

void ossimBumpShadeTileSource::setRgbColorSource(ossim_uint8 r,
                                                 ossim_uint8 g,
                                                 ossim_uint8 b)
{
   m_r = r;
   m_g = g;
   m_b = b;
}

void ossimBumpShadeTileSource::getRgbColorSource(ossim_uint8& r,
                                                 ossim_uint8& g,
                                                 ossim_uint8& b) const
{
   r = m_r;
   g = m_g;
   b = m_b;
}
