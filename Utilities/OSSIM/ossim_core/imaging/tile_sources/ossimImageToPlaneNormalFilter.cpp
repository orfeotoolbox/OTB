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
// $Id: ossimImageToPlaneNormalFilter.cpp,v 1.9 2005/05/09 12:39:40 dburken Exp $
#include <imaging/tile_sources/ossimImageToPlaneNormalFilter.h>
#include <imaging/factory/ossimImageDataFactory.h>
#include <projections/factory/ossimProjectionFactoryRegistry.h>
#include <projections/ossimProjection.h>
#include <base/common/ossimKeywordNames.h>

static const char* SMOOTHNESS_FACTOR_KW="smoothness_factor";

RTTI_DEF1(ossimImageToPlaneNormalFilter, "ossimImageToPlaneNormalFilter", ossimImageSourceFilter);

ossimImageToPlaneNormalFilter::ossimImageToPlaneNormalFilter()
   :ossimImageSourceFilter(),
    theTile(NULL),
    theBlankTile(NULL),
    theTrackScaleFlag(true),
    theXScale(1.0),
    theYScale(1.0),
    theSmoothnessFactor(1.0)
{
}

ossimImageToPlaneNormalFilter::ossimImageToPlaneNormalFilter(ossimImageSource* inputSource)
   :ossimImageSourceFilter(inputSource),
    theTile(NULL),
    theBlankTile(NULL),
    theTrackScaleFlag(true),
    theXScale(1.0),
    theYScale(1.0),
    theSmoothnessFactor(1.0)
{
}

ossimRefPtr<ossimImageData> ossimImageToPlaneNormalFilter::getTile(
   const ossimIrect& tileRect,
   ossim_uint32 resLevel)
{

   if(!isSourceEnabled()||!theInputConnection)
   {
      return ossimImageSourceFilter::getTile(tileRect, resLevel);
   }

   if(!theTile.valid())
   {
      initialize();
   }

   if(!theTile.valid())
   {
      return ossimImageSourceFilter::getTile(tileRect, resLevel);
   }

   theTile->setImageRectangle(tileRect);
   theBlankTile->setImageRectangle(tileRect);

   ossimIrect requestRect(tileRect.ul().x - 1,
                          tileRect.ul().y - 1,
                          tileRect.lr().x + 1,
                          tileRect.lr().y + 1);
   ossimRefPtr<ossimImageData> input =
      theInputConnection->getTile(requestRect, resLevel);

   if(!input||(input->getDataObjectStatus()==OSSIM_EMPTY)||!input->getBuf())
   {
      return theBlankTile;
   }

   double oldScaleX = theXScale;
   double oldScaleY = theYScale;

   if(resLevel > 0)
   {
      ossimDpt scaleFactor;
      theInputConnection->getDecimationFactor(resLevel, scaleFactor);

      if(!scaleFactor.hasNans())
      {
         theXScale /= scaleFactor.x;
         theYScale /= scaleFactor.y;
      }
   }

   computeNormals(input,
                  theTile);

   theXScale = oldScaleX;
   theYScale = oldScaleY;

   theTile->validate();

   return theTile;
}


void ossimImageToPlaneNormalFilter::initialize()
{
   if(theInputConnection)
   {
      theTile      = ossimImageDataFactory::instance()->create(this, this);
      theBlankTile = (ossimImageData*)(theTile->dup());
      theTile->initialize();


      if(theTrackScaleFlag)
      {
         ossimKeywordlist kwl;
         theInputConnection->getImageGeometry(kwl);

         ossimProjection* proj = ossimProjectionFactoryRegistry::instance()->
            createProjection(kwl);

         if(proj)
         {
            ossimDpt pt = proj->getMetersPerPixel();

            if(!pt.hasNans())
            {
               theXScale = pt.x;
               theYScale = pt.y;
            }
         }
      }
   }
}

void ossimImageToPlaneNormalFilter::computeNormals(
   ossimRefPtr<ossimImageData>& inputTile,
   ossimRefPtr<ossimImageData>& outputTile)
{
   switch(inputTile->getScalarType())
   {
      case OSSIM_SSHORT16:
      {
         computeNormalsTemplate((ossim_sint16)0,
                                inputTile,
                                outputTile);
         break;
      }
      case OSSIM_UCHAR:
      {
         computeNormalsTemplate((ossim_uint8)0,
                                inputTile,
                                outputTile);
         break;
      }
      case OSSIM_USHORT11:
      case OSSIM_USHORT16:
      {
         computeNormalsTemplate((ossim_uint16)0,
                                inputTile,
                                outputTile);
         break;
      }
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_DOUBLE:
      {
         computeNormalsTemplate((ossim_float64)0,
                                inputTile,
                                outputTile);
         break;
      }
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT:
      {
         computeNormalsTemplate((ossim_float32)0,
                                inputTile,
                                outputTile);
         break;
      }
      default:
         break;
   }
}

template <class T> void ossimImageToPlaneNormalFilter::computeNormalsTemplate(
   T inputScalarTypeDummy,
   ossimRefPtr<ossimImageData>& inputTile,
   ossimRefPtr<ossimImageData>& outputTile)
{
   T np = (T)inputTile->getNullPix(0);
   double outNp1 = outputTile->getNullPix(0);
   double outNp2 = outputTile->getNullPix(1);
   double outNp3 = outputTile->getNullPix(2);

   double* outX = (double*)outputTile->getBuf(0);
   double* outY = (double*)outputTile->getBuf(1);
   double* outZ = (double*)outputTile->getBuf(2);
   ossim_int32 y = 0;
   ossim_int32 x = 0;
   ossim_int32 iw = inputTile->getWidth();
   ossim_int32 ow = outputTile->getWidth();
   ossim_int32 oh = outputTile->getHeight();
   T* row1 = (T*)inputTile->getBuf();
   T* row2 = row1+iw;
   T* row3 = row2+iw;
   ossimColumnVector3d pX;
   ossimColumnVector3d pY;
   ossimColumnVector3d leftX;
   ossimColumnVector3d rightX;
   ossimColumnVector3d topY;
   ossimColumnVector3d bottomY;

   ossimColumnVector3d normal;


   for(y = 0; y < oh; ++y)
   {
      for(x = 0; x < ow; ++x)
      {
         if((row1[0] == np)||
            (row1[1] == np)||
            (row1[2] == np)||
            (row2[0] == np)||
            (row2[1] == np)||
            (row2[2] == np)||
            (row3[0] == np)||
            (row3[1] == np)||
            (row3[2] == np))
         {
            *outX = outNp1;
            *outY = outNp2;
            *outZ = outNp3;
         }
         else
         {
//             dhX = ((double)row1[2] - (double)row1[0])+
//                   ((double)row2[2] - (double)row2[0])+
//                   ((double)row3[2]  - (double)row3[0]);

//             dhY = ((double)row1[0] - (double)row3[0])+
//                   ((double)row1[1] - (double)row3[1])+
//                   ((double)row1[2] - (double)row3[2]);

//             dhX = (-(double)row1[2] + (double)row1[0])+
//                   (-(double)row2[2] + (double)row2[0])+
//                   (-(double)row3[2] + (double)row3[0]);

//             dhY = (-(double)row1[0] + (double)row3[0])+
//                   (-(double)row1[1] + (double)row3[1])+
//                   (-(double)row1[2] + (double)row3[2]);

            leftX[0] = -theXScale*theSmoothnessFactor;
            leftX[1] = 0.0;
            leftX[2] = ((double)row1[0] + (double)row2[0]+ (double)row3[0])/3.0;

            rightX[0] = theXScale*theSmoothnessFactor;
            rightX[1] = 0.0;
            rightX[2] = ((double)row1[2] + (double)row2[2]+ (double)row3[2])/3.0;

            topY[0]   = 0.0;
            topY[1]   = theYScale*theSmoothnessFactor;
            topY[2]   = ((double)row1[0] + (double)row1[1] + (double)row1[2])/3.0;

            bottomY[0]   = 0.0;
            bottomY[1]   = -theYScale*theSmoothnessFactor;
            bottomY[2]   = ((double)row3[0] + (double)row3[1] + (double)row3[2])/3.0;

            pX = (rightX - leftX).unit();
            pY = (topY - bottomY).unit();
//             pX[0] = theXScale*theSmoothnessFactor;
//             pX[1] = 0.0;
//             pX[2] = dhX;

//             pY[0] = 0.0;
//             pY[1] = theYScale*theSmoothnessFactor;
//             pY[2] = dhY;

//             pX = pX.unit();
//             pY = pY.unit();
            normal = pX.cross(pY).unit();

            normal = normal.unit();
            *outX = normal[0];
            *outY = normal[1];
            *outZ = normal[2];
         }

         ++outX;
         ++outZ;
         ++outY;

         ++row1;
         ++row2;
         ++row3;
      }
      row1+=2;
      row2+=2;
      row3+=2;
   }
}

bool ossimImageToPlaneNormalFilter::loadState(const ossimKeywordlist& kwl,
                                              const char* prefix)
{
   ossimString scaleX     = kwl.find(prefix, ossimKeywordNames::SCALE_PER_PIXEL_X_KW);
   ossimString scaleY     = kwl.find(prefix, ossimKeywordNames::SCALE_PER_PIXEL_Y_KW);
   ossimString trackFlag  = kwl.find(prefix, "track_scale_flag");
   ossimString smoothness = kwl.find(prefix, SMOOTHNESS_FACTOR_KW);

   if(scaleX != "")
   {
      theXScale = scaleX.toDouble();
   }
   if(scaleY != "")
   {
      theYScale = scaleY.toDouble();
   }
   if(trackFlag != "")
   {
      theTrackScaleFlag = trackFlag.toBool();
   }
   if(smoothness!="")
   {
      theSmoothnessFactor = smoothness.toDouble();
   }

   return ossimImageSourceFilter::loadState(kwl, prefix);
}

bool ossimImageToPlaneNormalFilter::saveState(ossimKeywordlist& kwl,
                                              const char* prefix)const
{
   kwl.add(prefix,
           ossimKeywordNames::SCALE_PER_PIXEL_X_KW,
           theXScale,
           true);

   kwl.add(prefix,
           ossimKeywordNames::SCALE_PER_PIXEL_Y_KW,
           theXScale,
           true);

   kwl.add(prefix,
           "track_scale_flag",
           (ossim_uint32)theTrackScaleFlag,
           true);

   kwl.add(prefix,
           SMOOTHNESS_FACTOR_KW,
           theSmoothnessFactor,
           true);

   return ossimImageSourceFilter::saveState(kwl, prefix);
}

double ossimImageToPlaneNormalFilter::getMinPixelValue(ossim_uint32 band)const
{
   if(!isSourceEnabled())
   {
      return ossimImageSourceFilter::getMinPixelValue(band);
   }
   return -1;
}

double ossimImageToPlaneNormalFilter::getMaxPixelValue(ossim_uint32 band)const
{
   if(!isSourceEnabled())
   {
      return ossimImageSourceFilter::getMaxPixelValue(band);
   }
   return 1.0;
}

ossimScalarType ossimImageToPlaneNormalFilter::getOutputScalarType() const
{
   if(isSourceEnabled())
   {
      return OSSIM_DOUBLE;
   }
   
   return ossimImageSourceFilter::getOutputScalarType();
}

ossim_uint32 ossimImageToPlaneNormalFilter::getNumberOfOutputBands() const
{
   if(isSourceEnabled())
   {
      return 3;
   }
   return ossimImageSourceFilter::getNumberOfOutputBands();
}

void ossimImageToPlaneNormalFilter::setXScale(const double& scale)
{
   theXScale = scale;
}

double ossimImageToPlaneNormalFilter::getXScale()const
{
   return theXScale;
}

double ossimImageToPlaneNormalFilter::getYScale()const
{
   return theYScale;
}

void ossimImageToPlaneNormalFilter::setYScale(const double& scale)
{
   theYScale = scale;
}

void ossimImageToPlaneNormalFilter::setTrackScaleFlag(bool flag)
{
   theTrackScaleFlag = flag;
}

bool ossimImageToPlaneNormalFilter::getTrackScaleFlag()const
{
   return theTrackScaleFlag;
}

void ossimImageToPlaneNormalFilter::setSmoothnessFactor(double value)
{
   theSmoothnessFactor = value;
}

double ossimImageToPlaneNormalFilter::getSmoothnessFactor()const
{
   return theSmoothnessFactor;
}
