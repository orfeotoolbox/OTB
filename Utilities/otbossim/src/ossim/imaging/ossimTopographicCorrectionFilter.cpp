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
// $Id: ossimTopographicCorrectionFilter.cpp 21184 2012-06-29 15:13:09Z dburken $
#include <algorithm>
#include <sstream>
#include <ossim/imaging/ossimTopographicCorrectionFilter.h>
#include <ossim/imaging/ossimImageToPlaneNormalFilter.h>
#include <ossim/imaging/ossimScalarRemapper.h>
#include <ossim/imaging/ossimScalarRemapper.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/matrix/newmat.h>
#include <ossim/base/ossimMatrix3x3.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimKeywordNames.h>

static const char* CORRECTION_TYPE_KW        = "correction_type";
static const char* C_COMPUTED_FLAG_KW        = "c_computed_flag";
static const char* NDVI_RANGE_KW             = "ndvi_range";

RTTI_DEF1(ossimTopographicCorrectionFilter, "ossimTopographicCorrectionFilter", ossimImageCombiner);
ossimTopographicCorrectionFilter::ossimTopographicCorrectionFilter()
   :ossimImageCombiner(NULL, 2, 0, true, false),
    theTile(NULL),
//    theScalarRemapper(NULL),
    theLightSourceElevationAngle(45.0),
    theLightSourceAzimuthAngle(45.0),
    theJulianDay(0),
    theCComputedFlag(false),
    theTopoCorrectionType(TOPO_CORRECTION_COSINE),
//    theTopoCorrectionType(TOPO_CORRECTION_MINNAERT),
    theNdviLowTest(-0.1),
    theNdviHighTest(0.1)
{
//    theScalarRemapper = new ossimScalarRemapper();
//    theScalarRemapper->setOutputScalarType(OSSIM_NORMALIZED_DOUBLE);
//    theScalarRemapper->initialize();
}

ossimTopographicCorrectionFilter::ossimTopographicCorrectionFilter(ossimImageSource* colorSource,
                                                                   ossimImageSource* elevSource)
   :ossimImageCombiner(NULL, 2, 0, true, false),
    theTile(NULL),
    theLightSourceElevationAngle(45.0),
    theLightSourceAzimuthAngle(45.0),
    theJulianDay(0),
    theCComputedFlag(false),
    theTopoCorrectionType(TOPO_CORRECTION_COSINE),
    theNdviLowTest(-0.1),
    theNdviHighTest(0.1)
{
   connectMyInputTo(colorSource);
   connectMyInputTo(elevSource);
}

ossimTopographicCorrectionFilter::~ossimTopographicCorrectionFilter()
{
}

ossimRefPtr<ossimImageData> ossimTopographicCorrectionFilter::getTile(
   const  ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
   ossimImageSource* colorSource = PTR_CAST(ossimImageSource,
                                                     getInput(0));
   ossimImageSource* normalSource  = PTR_CAST(ossimImageSource,
                                                       getInput(1));

   if(!isSourceEnabled()||!normalSource||!colorSource)
   {
      if(colorSource)
      {
         return colorSource->getTile(tileRect, resLevel);
      }
   }

   if(!theTile.valid())
   {
      allocate();
   }

   if(!theTile)
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
      else
      {
         theTile->makeBlank();
      }
   }
   else
   {
      theTile->makeBlank();
   }

   // ossimImageData* inputTile = NULL;

   ossimRefPtr<ossimImageData> normalData = normalSource->getTile(tileRect,
                                                                  resLevel);

   ossimRefPtr<ossimImageData> colorData  = colorSource->getTile(tileRect,
                                                                 resLevel);

   if(!colorData.valid() || !normalData.valid())
   {
      return theTile;
   }

   if((normalData->getNumberOfBands() != 3)||
      (normalData->getScalarType() != OSSIM_DOUBLE)||
      !normalData->getBuf()||
      !colorData->getBuf()||
      (colorData->getDataObjectStatus() == OSSIM_EMPTY)||
      (normalData->getDataObjectStatus()==OSSIM_EMPTY))
   {
      return colorData;
   }

   executeTopographicCorrection(theTile,
                                colorData,
                                normalData);
   theTile->validate();
   return theTile;
}

void ossimTopographicCorrectionFilter::initialize()
{
   ossimImageCombiner::initialize();

   // Force an "allocate()" on the first getTile.
   theTile      = NULL;
}

void ossimTopographicCorrectionFilter::allocate()
{
   if(!getInput(0) || !getInput(1)) return;
   theBandMapping.clear();
   if(isSourceEnabled())
   {
      // ossimImageSource* colorSource = PTR_CAST(ossimImageSource, getInput(0));

      theTile = ossimImageDataFactory::instance()->create(this, this);
      theTile->initialize();

      int arraySize = theTile->getNumberOfBands();
      if(theGain.size() > 0)
      {
         arraySize = (int)theGain.size();
      }
      // we will do a non destructive resize onf the arrays
      //
      resizeArrays(arraySize);

      ossimImageSource* input1 = PTR_CAST(ossimImageSource,
                                                   getInput(0));
      if(input1)
      {
         input1->getOutputBandList(theBandMapping);
         for(ossim_uint32 idx = 0; idx < theBandMapping.size(); ++idx)
         {
            if(theBias.size())
            {
               if(theBandMapping[idx] >= theBias.size())
               {
                  theBandMapping[idx] = (unsigned int)theBias.size()-1;
               }
            }
            else
            {
               theBandMapping[idx] = 0;
            }
         }
      }
   }

   computeLightDirection();
}

void ossimTopographicCorrectionFilter::computeLightDirection()
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

void ossimTopographicCorrectionFilter::executeTopographicCorrection(
   ossimRefPtr<ossimImageData>& outputData,
   ossimRefPtr<ossimImageData>& colorData,
   ossimRefPtr<ossimImageData>& normalData)
{
   switch(colorData->getScalarType())
   {
   case OSSIM_UCHAR:
   {
      if(theTopoCorrectionType!=TOPO_CORRECTION_MINNAERT)
      {
         executeTopographicCorrectionTemplate((ossim_uint8)0,
                                              outputData,
                                              colorData,
                                              normalData);
      }
      else
      {
         executeTopographicCorrectionMinnaertTemplate((ossim_uint8)0,
                                                      outputData,
                                                      colorData,
                                                      normalData);
      }
      break;
   }
   case OSSIM_USHORT11:
   case OSSIM_USHORT16:
   {
      if(theTopoCorrectionType!=TOPO_CORRECTION_MINNAERT)
      {
         executeTopographicCorrectionTemplate((ossim_uint16)0,
                                              outputData,
                                              colorData,
                                              normalData);
      }
      else
      {
         executeTopographicCorrectionMinnaertTemplate((ossim_uint16)0,
                                                      outputData,
                                                      colorData,
                                                      normalData);
      }
      break;
   }
   case OSSIM_SSHORT16:
   {
      if(theTopoCorrectionType!=TOPO_CORRECTION_MINNAERT)
      {
         executeTopographicCorrectionTemplate((ossim_sint16)0,
                                              outputData,
                                              colorData,
                                              normalData);
      }
      else
      {
         executeTopographicCorrectionMinnaertTemplate((ossim_sint16)0,
                                                      outputData,
                                                      colorData,
                                                      normalData);
      }
      break;
   }
   case OSSIM_DOUBLE:
   case OSSIM_NORMALIZED_DOUBLE:
   {
      if(theTopoCorrectionType!=TOPO_CORRECTION_MINNAERT)
      {
         executeTopographicCorrectionTemplate((ossim_float64)0,
                                              outputData,
                                              colorData,
                                              normalData);
      }
      else
      {
         executeTopographicCorrectionMinnaertTemplate((ossim_float64)0,
                                                      outputData,
                                                      colorData,
                                                      normalData);
      }
      break;
   }
   case OSSIM_FLOAT:
   case OSSIM_NORMALIZED_FLOAT:
   {
      if(theTopoCorrectionType!=TOPO_CORRECTION_MINNAERT)
      {
         executeTopographicCorrectionTemplate((ossim_float32)0,
                                              outputData,
                                              colorData,
                                              normalData);
      }
      else
      {
         executeTopographicCorrectionMinnaertTemplate((ossim_float32)0,
                                                      outputData,
                                                      colorData,
                                                      normalData);
      }
      break;
   }
   default:
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimTopographicCorrectionFilter::executeTopographicCorrection WARN: Not handling scalar type"
         << endl;
   }
   }
}

template <class T>
void ossimTopographicCorrectionFilter::executeTopographicCorrectionTemplate(
   T /* dummy */,
   ossimRefPtr<ossimImageData>& outputData,
   ossimRefPtr<ossimImageData>& colorData,
   ossimRefPtr<ossimImageData>& normalData)
{
   ossim_int32 y = 0;
   ossim_int32 x = 0;
   ossim_int32 colorW = colorData->getWidth();
   ossim_int32 colorH = colorData->getHeight();

   T* colorDataBand = 0;
   T* outputDataBand = 0;
   ossim_float64 colorNp = 0;
   ossim_float64 colorMin = 0;
   ossim_float64 colorMax = 0;
   // ossim_float64 outputDelta = 0;;
   // ossim_float64 outputMin = 0;
   // ossim_float64 outputMax = 0;
   ossim_float64 outputNp;
   double  normalNp = normalData->getNullPix(0);
   double  LPrime = 0.0;
   double  LNew   = 0.0;
   double  dn     = 0.0;
   double cosineZenith = ossim::cosd(90 - theLightSourceElevationAngle);

   if(!colorData->getBuf()||
      !normalData->getBuf()||
      (colorData->getDataObjectStatus() == OSSIM_EMPTY))
   {
      return;
   }
   for(ossim_uint32 b = 0; b < outputData->getNumberOfBands();++b)
   {
      int mappedBand = theBandMapping[b];
      double* normalX  = (double*)normalData->getBuf(0);
      double* normalY  = (double*)normalData->getBuf(1);
      double* normalZ  = (double*)normalData->getBuf(2);
      double numerator = cosineZenith + theC[mappedBand];
      outputNp  = (ossim_float64)outputData->getNullPix(b);
      // outputMin = (ossim_float64)outputData->getMinPix(b);
      // outputMax = (ossim_float64)outputData->getMaxPix(b);
      // outputDelta = outputMax - outputMin;
      colorDataBand    = (T*)(colorData->getBuf(b));
      colorNp          = (ossim_float64)(colorData->getNullPix(b));
      colorMin         = (ossim_float64)(colorData->getMinPix(b));
      colorMax         = (ossim_float64)(colorData->getMaxPix(b));
      outputDataBand   = (T*)(outputData->getBuf(b));
      bool theValuesAreGood = b < theC.size();
      double c = theC[mappedBand];
      if(theTopoCorrectionType != TOPO_CORRECTION_COSINE_C)
      {
         c = 0;
      }
      for(y = 0; y < colorH; ++y)
      {
         for(x = 0; x < colorW; ++x)
         {
            if((*colorDataBand) != colorNp)
            {
               if((*normalX != normalNp)&&
                  (*normalY != normalNp)&&
                  (*normalZ != normalNp)&&
                  theValuesAreGood)
               {

                  double cosineNewI = ((*normalX)*theLightDirection[0] +
                                       (*normalY)*theLightDirection[1] +
                                       (*normalZ)*theLightDirection[2]);
                  double cosineRatioDenom = (cosineNewI + c);
                  if((fabs(cosineRatioDenom) > FLT_EPSILON)&&(cosineNewI >= 0.0))
                  {
                     double cosineRatio = numerator/cosineRatioDenom;

                     LPrime = theGain[mappedBand]*((ossim_float64)(*colorDataBand)) + theBias[mappedBand];

                     LNew = LPrime*cosineRatio;

                     dn = ((LNew-theBias[mappedBand])/theGain[mappedBand]);

                     if(dn < colorMin) dn = colorMin;
                     if(dn > colorMax) dn = colorMax;

                     *outputDataBand = (T)(dn);
                  }
                  else
                  {
                     *outputDataBand = (T)(*colorDataBand);
                  }
               }
               else
               {
                  *outputDataBand = (T)(*colorDataBand);
               }
            }
            else
            {
               *outputDataBand = (T)outputNp;
            }

            ++outputDataBand;
            ++colorDataBand;
            ++normalX;
            ++normalY;
            ++normalZ;
         }
      }
   }
}

template <class T>
void ossimTopographicCorrectionFilter::executeTopographicCorrectionMinnaertTemplate(
   T /* dummy */,
   ossimRefPtr<ossimImageData>& outputData,
   ossimRefPtr<ossimImageData>& colorData,
   ossimRefPtr<ossimImageData>& normalData)
{

   ossim_int32 y = 0;
   ossim_int32 x = 0;
   ossim_int32 colorW = colorData->getWidth();
   ossim_int32 colorH = colorData->getHeight();

   T* colorDataBand = 0;
   T* outputDataBand = 0;
   ossim_float64 colorNp = 0;
   ossim_float64 colorMin = 0;
   ossim_float64 colorMax = 0;
   // ossim_float64 outputDelta = 0;;
   // ossim_float64 outputMin = 0;
   // ossim_float64 outputMax = 0;
   ossim_float64 outputNp;
   double  normalNp = normalData->getNullPix(0);
   double  LPrime = 0.0;
   double  LNew   = 0.0;
   double  dn     = 0.0;

   if(!colorData->getBuf()||
      !normalData->getBuf()||
      (colorData->getDataObjectStatus() == OSSIM_EMPTY))
   {
      return;
   }
   int maxBands = ossim::min((int)theK.size(), (int)outputData->getNumberOfBands());
   for(int b = 0; b < maxBands;++b)
   {
      int mappedBand = theBandMapping[b];
      double* normalX  = (double*)normalData->getBuf(0);
      double* normalY  = (double*)normalData->getBuf(1);
      double* normalZ  = (double*)normalData->getBuf(2);
      outputNp  = (ossim_float64)outputData->getNullPix(b);
      // outputMin = (ossim_float64)outputData->getMinPix(b);
      // outputMax = (ossim_float64)outputData->getMaxPix(b);
      // outputDelta = outputMax - outputMin;
      colorDataBand    = (T*)(colorData->getBuf(b));
      colorNp          = (ossim_float64)(colorData->getNullPix(b));
      colorMin         = (ossim_float64)(colorData->getMinPix(b));
      colorMax         = (ossim_float64)(colorData->getMaxPix(b));
      outputDataBand   = (T*)(outputData->getBuf(b));
      for(y = 0; y < colorH; ++y)
      {
         for(x = 0; x < colorW; ++x)
         {
            if((*colorDataBand) != colorNp)
            {
               if((*normalX != normalNp)&&
                  (*normalY != normalNp)&&
                  (*normalZ != normalNp))
               {

//                   if(fabs(*normalZ) < FLT_EPSILON)
//                   {
//                      *normalZ = 0.0;
//                   }
                  double cosineNewI = (((*normalX)*theLightDirection[0] +
                                        (*normalY)*theLightDirection[1] +
                                        (*normalZ)*theLightDirection[2]));
                  double slopeAngle = acos(*normalZ);
                  double cosineSlope = cos(slopeAngle);
                  double k  = theK[mappedBand];
                  double cosineSlopeKPower = pow(cosineSlope, k);
                  double denom = pow((double)cosineNewI, k)*cosineSlopeKPower;
                  double numerator = cosineSlope;

//                  double slopeAngle = asin(*normalZ);
//                  double tempK = theK[mappedBand]*cosineNewI;
//                double denom = pow((double)cosineNewI*cosineSlope, theK[mappedBand]);
//                  double numerator = pow((double)cosineSlope, 1-tempK);
//                  if((fabs(denom) > .0001)&&(cosineNewI >= 0.0))
//                  if((cosineNewI >= 0.0) &&fabs(denom) > .000001)
                  if(fabs(denom) > .00000001)
                  {
//                     double cosineRatio = cosineSlope/denom;
                     double cosineRatio = numerator/denom;

                     LPrime = theGain[mappedBand]*((ossim_float64)(*colorDataBand)) + theBias[mappedBand];

                     LNew = LPrime*cosineRatio;

                     dn = ((LNew-theBias[mappedBand])/theGain[mappedBand]);

                     if(dn < colorMin) dn = colorMin;
                     if(dn > colorMax) dn = colorMax;

                     *outputDataBand = (T)(dn);
                  }
                  else
                  {
                     *outputDataBand = (T)(*colorDataBand);
                  }
               }
               else
               {
                  *outputDataBand = (T)(*colorDataBand);
               }
            }
            else
            {
               *outputDataBand = (T)outputNp;
            }

            ++outputDataBand;
            ++colorDataBand;
            ++normalX;
            ++normalY;
            ++normalZ;
         }
      }
   }
}


#if 0
void ossimTopographicCorrectionFilter::computeC()
{
   theCComputedFlag = false;
   int b = 0;
   int tileCount = 0;
   bool done = false;
   if(theC.size()<1) return;

   for(b = 0; b < (int) theC.size(); ++b)
   {
      theC[b] = 0.0;
   }


   theNdviLowTest  = 0.1;
   theNdviHighTest = 1;

   ossimImageSource* colorSource  = PTR_CAST(ossimImageSource, getInput(0));
   ossimImageSource* normalSource = PTR_CAST(ossimImageSource, getInput(1));

   if(!colorSource || !normalSource)
   {
      return;
   }
   std::vector<ossim2dLinearRegression> linearRegression(theC.size());
   ossimIrect normalRect = normalSource->getBoundingRect();
   ossimIrect colorRect  = colorSource->getBoundingRect();
   ossimIrect clipRect   = normalRect.clipToRect(colorRect);
   ossimIpt ul = clipRect.ul();
   ossimIpt lr = clipRect.lr();
   ossimIpt tileSize(128,128);
   ossim_int32 tilesHoriz = clipRect.width()/tileSize.x;
   ossim_int32 tilesVert  = clipRect.height()/tileSize.y;


   if(!normalRect.intersects(colorRect))
   {
      return;
   }
   ossim_int32 maxSize = tilesHoriz*tilesVert;//ossim::min(200, );

   int idx = 0;

   idx = 0;
   std::vector<int> cosineIBucketCount(10);
   std::fill(cosineIBucketCount.begin(), cosineIBucketCount.end(), 0);
   const int maxBucketCount = 1000;
   bool goodCoefficients = false;
   long numberOfRuns = 0;
   while((!goodCoefficients)&&(numberOfRuns < 2))
   {
      while((idx < maxSize)&&
            (!done))
      {
         ossim_int32 ty = idx/tilesHoriz;
         ossim_int32 tx = idx%tilesHoriz;


         ossim_int32 x = (ul.x + tx*tileSize.x);
         ossim_int32 y = (ul.y + ty*tileSize.y);

         ossimIrect requestRect(x,
                                y,
                                x+tileSize.x-1,
                                y+tileSize.y-1);

         ossimRefPtr<ossimImageData> colorData  = colorSource->getTile(requestRect);
         ossimRefPtr<ossimImageData> normalData = normalSource->getTile(requestRect);

         switch(colorData->getScalarType())
         {
         case OSSIM_UCHAR:
         {
            addRegressionPointsTemplate((ossim_uint8)0,
                                        linearRegression, cosineIBucketCount, maxBucketCount, colorData, normalData);
            break;
         }
         case OSSIM_USHORT11:
         case OSSIM_USHORT16:
         {
            addRegressionPointsTemplate((ossim_uint16)0,
                                        linearRegression, cosineIBucketCount, maxBucketCount, colorData, normalData);
            break;
         }
         case OSSIM_SSHORT16:
         {
            addRegressionPointsTemplate((ossim_sint16)0,
                                        linearRegression, cosineIBucketCount, maxBucketCount, colorData, normalData);
            break;
         }
         case OSSIM_FLOAT:
         case OSSIM_NORMALIZED_FLOAT:
         {
            addRegressionPointsTemplate((ossim_float32)0,
                                        linearRegression, cosineIBucketCount, maxBucketCount, colorData, normalData);
            break;
         }
         case OSSIM_DOUBLE:
         case OSSIM_NORMALIZED_DOUBLE:
         {
            addRegressionPointsTemplate((ossim_float64)0,
                                        linearRegression, cosineIBucketCount, maxBucketCount, colorData, normalData);
            break;
         }
         }
         if(((double)linearRegression[0].getNumberOfPoints()/(double)(maxBucketCount*10.0))>=.7)
         {
            done = true;
         }
         ++idx;
      }

      double intercept, m;
      long numberOfPositiveSlopes=0;
      for(b=0;b<theC.size();++b)
      {
         if(linearRegression[b].getNumberOfPoints()>2)
         {
            linearRegression[b].solve();
            linearRegression[b].getEquation(m, intercept);
            theC[b] = intercept/m;

            ossimNotify(ossimNotifyLevel_INFO) << "equation for b = " << b <<" is y = " << m << "*x + " << intercept << endl
                                               << "with c =         " << theC[b] << endl;
            if(m >=0.0)
            {
               numberOfPositiveSlopes++;
            }
         }
      }
      for(idx = 0; idx < cosineIBucketCount.size(); ++idx)
      {
         ossimNotify(ossimNotifyLevel_INFO) << "bucket " << idx << " = " << cosineIBucketCount[idx] << endl;
      }
      if(numberOfPositiveSlopes > .5*theC.size())
      {
         goodCoefficients = true;
      }
      else
      {
         ossimNotify(ossimNotifyLevel_WARN) << "ossimTopographicCorrectionFilter::computeC() WARN: not enough positive slopes" << endl
                                             << "changing test to look for dirt areas" << endl;
         theNdviLowTest = -1.0;
         theNdviHighTest = .1;
         for(b=0;b<theC.size();++b)
         {
            linearRegression[b].clear();
         }
      }
      ++numberOfRuns;
   }
   theCComputedFlag = true;
}

template<class T>
void ossimTopographicCorrectionFilter::addRegressionPointsTemplate(
   T, //dummy
   std::vector<ossim2dLinearRegression>& regressionPoints,
   std::vector<int>& cosineIBucketCount,
   ossim_int32 maxCountPerBucket,
   ossimRefPtr<ossimImageData>& colorData,
   ossimRefPtr<ossimImageData>& normalData)
{
   if(!colorData||!normalData)
   {
      return;
   }
   if((colorData->getDataObjectStatus() == OSSIM_EMPTY)||
      (!colorData->getBuf())||
      (!normalData->getBuf())||
      (normalData->getDataObjectStatus()==OSSIM_EMPTY))
   {
      return;
   }

   ossim_float64* normalBands[3];
   ossim_float64 normalBandsNp[3];
   ossim_uint32 count=0;
   ossim_uint32 b = 0;
   std::vector<T*> colorBands(colorData->getNumberOfBands());
   std::vector<T> colorBandsNp(colorData->getNumberOfBands());

   normalBands[0]   = (ossim_float64*)normalData->getBuf(0);
   normalBands[1]   = (ossim_float64*)normalData->getBuf(1);
   normalBands[2]   = (ossim_float64*)normalData->getBuf(2);
   normalBandsNp[0] = normalData->getNullPix(0);
   normalBandsNp[1] = normalData->getNullPix(1);
   normalBandsNp[2] = normalData->getNullPix(2);

   for(b=0;b<colorData->getNumberOfBands();++b)
   {
      colorBands[b]   = (T*)colorData->getBuf(b);
      colorBandsNp[b] = (T)colorData->getNullPix(b);
   }

   ossim_uint32 maxOffset = colorData->getWidth()*colorData->getHeight();
   ossim_uint32 offset=0;
   ossim_float64 ndviTest=0.0;
   ossim_uint32 numberOfTests = 0;
   if(maxOffset)
   {
      maxOffset-=1;
   }

   ossim_int32 percent = (ossim_int32)(colorData->getWidth()*colorData->getHeight()*.2);
   offset = 0;
   ossim_uint32 countPixels = 0;
   while(offset < maxOffset)
   {
      bool nullBandsExist = false;
      for(b = 0; ((b < colorData->getNumberOfBands())&&(!nullBandsExist));++b)
      {
         if(*colorBands[b] == colorBandsNp[b])
         {
            nullBandsExist = true;
         }
      }

      if((!nullBandsExist)&&
         (*normalBands[0] != normalBandsNp[0])&&
         (*normalBands[1] != normalBandsNp[1])&&
         (*normalBands[2] != normalBandsNp[2]))
      {
         if(computeNdvi((T)0,
                        ndviTest,
                        offset,
                        colorBands))
         {
            if((ndviTest >= theNdviLowTest) && (ndviTest <= theNdviHighTest))
            {
               double cosineI = ((*(normalBands[0]+offset))*theLightDirection[0] +
                                 (*(normalBands[1]+offset))*theLightDirection[1] +
                                 (*(normalBands[2]+offset))*theLightDirection[2]);
               if(cosineI >= 0.0)
               {
                  long bucketIdx = (long)(cosineI*cosineIBucketCount.size());

                  // we will try to disperse the normals out and so we don't clump everything
                  // in one place
                  //
                  if(cosineIBucketCount[bucketIdx] < maxCountPerBucket)
                  {
                     for(b = 0; b < colorData->getNumberOfBands();++b)
                     {
                        regressionPoints[b].addPoint(ossimDpt(cosineI, *(colorBands[b]+offset)));
                     }
                     ++cosineIBucketCount[bucketIdx];
                     ++count;
                  }
               }
            }
         }
      }
      ++offset;
   }
}
#endif

template <class T> bool ossimTopographicCorrectionFilter::computeNdvi(
   T,
   ossim_float64& result,
   ossim_uint32 offset,
   const std::vector<T*>& bands)const
{
   if(bands.size() > 3)
   {
      result = (((double)*(bands[3]+offset) - (double)*(bands[2]+offset))/
                ((double)*(bands[3]+offset) + (double)*(bands[2]+offset)));
      return true;
   }

   return false;
}

void ossimTopographicCorrectionFilter::resizeArrays(ossim_uint32 newSize)
{
  if(!getInput(0) || !getInput(1)) return;


   vector<double> tempC    = theC;
   vector<double> tempK    = theK;
   vector<double> tempBias = theBias;
   vector<double> tempGain = theGain;

   theC.resize(newSize);
   theK.resize(newSize);
   theBias.resize(newSize);
   theGain.resize(newSize);
   ossim_uint32 tempIdx = 0;
   if(tempC.size() > 0 && (theC.size() > 0))
   {
      int numberOfElements = ossim::min((int)tempC.size(),(int)theC.size());

      std::copy(tempC.begin(), tempC.begin()+numberOfElements,
                theC.begin());
      std::copy(tempK.begin(), tempK.begin()+numberOfElements,
                theK.begin());
      std::copy(tempBias.begin(), tempBias.begin()+numberOfElements,
                theBias.begin());
      std::copy(tempGain.begin(), tempGain.begin()+numberOfElements,
                theGain.begin());

      if(theC.size() > tempC.size())
      {
         std::fill(theC.begin()+numberOfElements, theC.end(), (double)0.0);
         std::fill(theBias.begin()+numberOfElements, theBias.end(), (double)0.0);
         std::fill(theGain.begin()+numberOfElements, theGain.end(), (double)1.0);
         for(tempIdx = numberOfElements; tempIdx < theK.size(); ++tempIdx)
         {
            theK[tempIdx] = 0.3 + .4*(tempIdx/(double)(theK.size()));
         }
      }
   }
   else
   {
      std::fill(theC.begin(), theC.end(), (double)0.0);
      std::fill(theBias.begin(), theBias.end(), (double)0.0);
      std::fill(theGain.begin(), theGain.end(), (double)1.0);
      for(tempIdx = 0; tempIdx < theK.size(); ++tempIdx)
      {
         theK[tempIdx] = 0.3 + .4*(tempIdx/(double)(theK.size()));
      }
   }
}

bool ossimTopographicCorrectionFilter::loadState(const ossimKeywordlist& kwl,
                                                 const char* prefix)
{

   ossimString elevAngle      = kwl.find(prefix, ossimKeywordNames::ELEVATION_ANGLE_KW);
   ossimString azimuthAngle   = kwl.find(prefix, ossimKeywordNames::AZIMUTH_ANGLE_KW);
   ossimString bands          = kwl.find(prefix, ossimKeywordNames::NUMBER_BANDS_KW);
   ossimString correctionType = kwl.find(prefix, CORRECTION_TYPE_KW);
   ossimString julianDay      = kwl.find(prefix, ossimKeywordNames::JULIAN_DAY_KW);
   ossimString cComputedFlag  = kwl.find(prefix, C_COMPUTED_FLAG_KW);
   ossimString ndviRange      = kwl.find(prefix, NDVI_RANGE_KW);

   theCComputedFlag = cComputedFlag.toBool();
   correctionType = correctionType.downcase();
   int numberOfBands = bands.toInt();
   theLightSourceElevationAngle = elevAngle.toDouble();
   theLightSourceAzimuthAngle   = azimuthAngle.toDouble();
   theJulianDay = julianDay.toDouble();

   if(ndviRange != "")
   {
      std::istringstream input(ndviRange.c_str());

      input >> theNdviLowTest >> theNdviHighTest;
   }
   if(numberOfBands>0)
   {
     theGain.resize(numberOfBands);
     theBias.resize(numberOfBands);
     theC.resize(numberOfBands);
     theK.resize(numberOfBands);
      for(int b = 0; b < numberOfBands; ++b)
      {
         ossimString k    = "k"+ossimString::toString(b);
         ossimString c    = "c"+ossimString::toString(b);
         ossimString bias = "bias"+ossimString::toString(b);
         ossimString gain = "gain"+ossimString::toString(b);

         const char* kValue    = kwl.find(prefix, k);
         const char* cValue    = kwl.find(prefix, c);
         const char* biasValue = kwl.find(prefix, bias);
         const char* gainValue = kwl.find(prefix, gain);
	 if(kValue)
	   {
	     theK[b] = ossimString(kValue).toDouble();
	   }
	 else
	   {
	     theK[b] = 1.0;
	   }
         if(cValue)
         {
            theC[b] = ossimString(cValue).toDouble();
         }
         else
         {
            theC[b] = 0.0;;
         }
         if(biasValue)
         {
            theBias[b] = ossimString(biasValue).toDouble();
         }
         else
         {
            theBias[b] = 0.0;
         }
         if(gainValue)
         {
            theGain[b] = ossimString(gainValue).toDouble();
         }
         else
         {
            theGain[b] = 1.0;
         }
      }
   }

   if(correctionType.contains("cosine_c"))
   {
      theTopoCorrectionType = TOPO_CORRECTION_COSINE_C;
   }
   else if(correctionType.contains("minnaert"))
   {
      theTopoCorrectionType = TOPO_CORRECTION_MINNAERT;
   }
   else
   {
      theTopoCorrectionType = TOPO_CORRECTION_COSINE;
   }


   bool result =  ossimImageCombiner::loadState(kwl, prefix);

   computeLightDirection();

   return result;
}

bool ossimTopographicCorrectionFilter::saveState(ossimKeywordlist& kwl,
                                                 const char* prefix)const
{
   // we can use any of the arrays theC, theBias, or theGain since
   // they mirror the number of input bands
   //
   int numberOfBands = (int)theC.size();

   kwl.add(prefix,
           ossimKeywordNames::NUMBER_BANDS_KW,
           numberOfBands,
           true);

   kwl.add(prefix,
           ossimKeywordNames::ELEVATION_ANGLE_KW,
           theLightSourceElevationAngle,
           true);

   kwl.add(prefix,
           ossimKeywordNames::AZIMUTH_ANGLE_KW,
           theLightSourceAzimuthAngle,
           true);

   kwl.add(prefix,
           C_COMPUTED_FLAG_KW,
           (ossim_uint32)theCComputedFlag,
           true);
   kwl.add(prefix,
           NDVI_RANGE_KW,
           ossimString::toString(theNdviLowTest) +
           " " + ossimString::toString(theNdviHighTest),
           true);

   if(theTopoCorrectionType == TOPO_CORRECTION_COSINE_C)
   {
      kwl.add(prefix,
              CORRECTION_TYPE_KW,
              "cosine_c",
              true);
   }
   else if(theTopoCorrectionType == TOPO_CORRECTION_MINNAERT)
   {
      kwl.add(prefix,
              CORRECTION_TYPE_KW,
              "minnaert",
              true);
   }
   else
   {
      kwl.add(prefix,
              CORRECTION_TYPE_KW,
              "cosine",
              true);
   }

   for(int b = 0; b < numberOfBands; ++b)
   {
      ossimString k    = "k"+ossimString::toString(b);
      ossimString c    = "c"+ossimString::toString(b);
      ossimString bias = "bias"+ossimString::toString(b);
      ossimString gain = "gain"+ossimString::toString(b);
      kwl.add(prefix,
              k,
              theK[b],
              true);
      kwl.add(prefix,
              c,
              theC[b],
              true);
      kwl.add(prefix,
              bias,
              theBias[b],
              true);
      kwl.add(prefix,
              gain,
              theGain[b],
              true);
   }

   return ossimImageCombiner::saveState(kwl, prefix);
}

ossim_uint32 ossimTopographicCorrectionFilter::getNumberOfOutputBands() const
{
   ossimImageSource* colorSource = PTR_CAST(ossimImageSource, getInput(0));
   if(colorSource)
   {
      return colorSource->getNumberOfOutputBands();
   }
   return ossimImageCombiner::getNumberOfOutputBands();
}

ossimScalarType ossimTopographicCorrectionFilter::getOutputScalarType() const
{
   ossimImageSource* colorSource = PTR_CAST(ossimImageSource, getInput(0));
   if(colorSource)
   {
      return colorSource->getOutputScalarType();
   }
   return ossimImageCombiner::getOutputScalarType();
}

double ossimTopographicCorrectionFilter::getNullPixelValue(ossim_uint32 band)const
{
   ossimImageSource* colorSource = PTR_CAST(ossimImageSource, getInput(0));
   if(colorSource)
   {
      return colorSource->getNullPixelValue(band);
   }
   return ossimImageCombiner::getNullPixelValue(band);
}

double ossimTopographicCorrectionFilter::getMinPixelValue(ossim_uint32 band)const
{
   ossimImageSource* colorSource = PTR_CAST(ossimImageSource, getInput(0));
   if(colorSource)
   {
      return colorSource->getMinPixelValue(band);
   }
   return ossimImageCombiner::getMinPixelValue(band);
}

double ossimTopographicCorrectionFilter::getMaxPixelValue(ossim_uint32 band)const
{
   ossimImageSource* colorSource = PTR_CAST(ossimImageSource, getInput(0));
   if(colorSource)
   {
      return colorSource->getMaxPixelValue(band);
   }
   return ossimImageCombiner::getMaxPixelValue(band);
}

ossimIrect ossimTopographicCorrectionFilter::getBoundingRect(ossim_uint32 resLevel)const
{
   ossimIrect result;
   result.makeNan();
   ossimImageSource* colorSource = PTR_CAST(ossimImageSource, getInput(0));

   if(colorSource)
   {
      result = colorSource->getBoundingRect(resLevel);
   }

   return result;
}

void ossimTopographicCorrectionFilter::getDecimationFactor(ossim_uint32 resLevel,
                                                           ossimDpt& result) const
{
   result.makeNan();
   ossimImageSource* colorSource = PTR_CAST(ossimImageSource, getInput(0));

   if(colorSource)
   {
      colorSource->getDecimationFactor(resLevel,
                                       result);
   }
}

void ossimTopographicCorrectionFilter::getDecimationFactors(vector<ossimDpt>& decimations) const
{
   ossimImageSource* colorSource = PTR_CAST(ossimImageSource, getInput(0));
   if(colorSource)
   {
      colorSource->getDecimationFactors(decimations);
   }
}

ossim_uint32 ossimTopographicCorrectionFilter::getNumberOfDecimationLevels()const
{
   ossimImageSource* colorSource = PTR_CAST(ossimImageSource, getInput(0));
   if(colorSource)
   {
      return colorSource->getNumberOfDecimationLevels();
   }
   return 0;
}

double ossimTopographicCorrectionFilter::getAzimuthAngle()const
{
   return theLightSourceAzimuthAngle;
}

double ossimTopographicCorrectionFilter::getElevationAngle()const
{
   return theLightSourceElevationAngle;
}

void ossimTopographicCorrectionFilter::setAzimuthAngle(double angle)
{
   theLightSourceAzimuthAngle = angle;
}

void ossimTopographicCorrectionFilter::setElevationAngle(double angle)
{
   theLightSourceElevationAngle = angle;
}
bool ossimTopographicCorrectionFilter::canConnectMyInputTo(ossim_int32 inputIndex,
                                                           const ossimConnectableObject* object)const
{
   return (object&&
           ( (inputIndex>=0) && inputIndex < 2)&&
           PTR_CAST(ossimImageSource, object));

}

void ossimTopographicCorrectionFilter::connectInputEvent(ossimConnectionEvent& /* event */)
{
   initialize();
}

void ossimTopographicCorrectionFilter::disconnectInputEvent(ossimConnectionEvent& /* event */)
{
   initialize();
}

void ossimTopographicCorrectionFilter::propertyEvent(ossimPropertyEvent& /* event */)
{
   initialize();
}

void ossimTopographicCorrectionFilter::refreshEvent(ossimRefreshEvent& /* event */)
{
   initialize();
}

ossimTopographicCorrectionFilter::ossimTopoCorrectionType ossimTopographicCorrectionFilter::getTopoCorrectionType()const
{
   return theTopoCorrectionType;
}

void ossimTopographicCorrectionFilter::setTopoCorrectionType(ossimTopoCorrectionType topoType)
{
   theTopoCorrectionType = topoType;
}

const std::vector<double>& ossimTopographicCorrectionFilter::getGainValues()const
{
   return theGain;
}

void ossimTopographicCorrectionFilter::setGainValues(const std::vector<double>& gainValues)
{
   theGain = gainValues;
}

const vector<double>& ossimTopographicCorrectionFilter::getBiasValues()const
{
   return theBias;
}

void ossimTopographicCorrectionFilter::setBiasValues(const std::vector<double>& biasValues)
{
   theBias = biasValues;
}

const vector<double>& ossimTopographicCorrectionFilter::getKValues()const
{
   return theK;
}

double ossimTopographicCorrectionFilter::getK(int idx)const
{
   if(idx < (int)theK.size())
   {
      return theK[idx];
   }
   else
   {
      return 1.0;
   }
}

void ossimTopographicCorrectionFilter::setK(int idx, double value)
{
   if(idx < (int)theK.size())
   {
      theK[idx] = value;
   }
}

void ossimTopographicCorrectionFilter::setKValues(const vector<double>& kValues)
{
   theK = kValues;
}

double ossimTopographicCorrectionFilter::getC(int idx)const
{
   if(idx < (int)theC.size())
   {
      return theC[idx];
   }
   else
   {
      return 0.0;
   }
}

void ossimTopographicCorrectionFilter::setC(int idx, double value)
{
   if(idx < (int)theC.size())
   {
      theC[idx] = value;
   }
}

void ossimTopographicCorrectionFilter::setCValues(const vector<double>& cValues)
{
   theC = cValues;
}
