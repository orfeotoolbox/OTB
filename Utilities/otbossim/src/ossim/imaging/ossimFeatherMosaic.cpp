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
// $Id: ossimFeatherMosaic.cpp 15766 2009-10-20 12:37:09Z gpotts $

#include <ossim/imaging/ossimFeatherMosaic.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimLine.h>
#include <ossim/base/ossimTrace.h>

static ossimTrace traceDebug("ossimFeatherMosaic:debug");

RTTI_DEF1(ossimFeatherMosaic, "ossimFeatherMosaic", ossimImageMosaic);

ossimFeatherMosaic::ossimFeatherMosaic()
   :ossimImageMosaic(),
    theInputFeatherInformation(NULL),
    theAlphaSum(NULL),
    theResult(NULL),
    theFeatherInfoSize(0)
{
}

ossimFeatherMosaic::ossimFeatherMosaic(ossimConnectableObject::ConnectableObjectList& inputSources)
   :ossimImageMosaic(inputSources),
    theInputFeatherInformation(NULL),
    theAlphaSum(NULL),
    theResult(NULL),
    theFeatherInfoSize(0)
{
   initialize();
}

ossimFeatherMosaic::~ossimFeatherMosaic()
{
   if(theInputFeatherInformation)
   {
      delete [] theInputFeatherInformation;
      theInputFeatherInformation = NULL;
   }
   theFeatherInfoSize = 0;
}

ossimRefPtr<ossimImageData> ossimFeatherMosaic::getTile(const ossimIrect& tileRect,
                                            ossim_uint32 resLevel)
{
   long w = tileRect.width();
   long h = tileRect.height();
   ossimIpt origin = tileRect.ul();
   
   if(!isSourceEnabled())
   {
      return ossimImageMosaic::getTile(tileRect, resLevel);
   }
   if(!theTile||!theAlphaSum||!theResult||!theInputFeatherInformation)
   {
      initialize();

      if(!theTile||!theAlphaSum||!theResult||!theInputFeatherInformation)
      {
         return ossimImageMosaic::getTile(tileRect, resLevel);
      }
   }
   ossim_uint32 size = getNumberOfInputs();
   theAlphaSum->setImageRectangle(tileRect);
   theResult->setImageRectangle(tileRect);
   
   if(size == 0)
   {
      return ossimRefPtr<ossimImageData>();
   }

   if(size == 1)
   {
      return ossimImageMosaic::getTile(tileRect, resLevel);
   }

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
   theTile->setOrigin(origin);
   theTile->makeBlank();
      
   switch(theTile->getScalarType())
   {
      case OSSIM_UCHAR:
      {
         return combine(static_cast<ossim_uint8>(0),
                        tileRect, resLevel);
      }
      case OSSIM_USHORT16:
      case OSSIM_USHORT11:
      {
         return combine(static_cast<ossim_uint16>(0),
                        tileRect, resLevel);
      }
      case OSSIM_SSHORT16:
      {
         return combine(static_cast<ossim_sint16>(0),
                        tileRect, resLevel);
      }
      case OSSIM_DOUBLE:
      case OSSIM_NORMALIZED_DOUBLE:
      {
         return combine(static_cast<double>(0),
                        tileRect, resLevel);
      }
      case OSSIM_FLOAT:
      case OSSIM_NORMALIZED_FLOAT:
      {
         return combine(static_cast<float>(0),
                        tileRect, resLevel);
      }
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimFeatherMosaic::getTile: error, unknown scalar type!!!"
            << std::endl;
      }
   }
   return ossimRefPtr<ossimImageData>();
}


template <class T> ossimRefPtr<ossimImageData> ossimFeatherMosaic::combine(
   T,
   const ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
   ossimRefPtr<ossimImageData> currentImageData;
   ossim_uint32 band;
   long upperBound = theTile->getWidth()*theTile->getHeight();
   long offset = 0;
   long row    = 0;
   long col    = 0;
   long numberOfTilesProcessed = 0;
   float *sumBand       = static_cast<float*>(theAlphaSum->getBuf());
   float         *bandRes = NULL;
   ossimIpt point;
   
   theAlphaSum->fill(0.0);
   theResult->fill(0.0);
   
   T** srcBands  = new T*[theLargestNumberOfInputBands];
   T** destBands = new T*[theLargestNumberOfInputBands];
   ossim_uint32 layerIdx = 0;
   currentImageData  = getNextTile(layerIdx,
                                   0,
                                   tileRect,
                                   resLevel);
   if(!currentImageData.valid())
   {
      delete [] srcBands;
      delete [] destBands;
      return currentImageData;
   }
   ossim_uint32 minNumberOfBands = currentImageData->getNumberOfBands();
   for(band = 0; band < minNumberOfBands; ++band)
   {
      srcBands[band]  = static_cast<T*>(currentImageData->getBuf(band));
      destBands[band] = static_cast<T*>(theTile->getBuf(band));
   }
   // if the src is smaller than the destination in number
   // of bands we will just duplicate the last band.
   for(;band < theLargestNumberOfInputBands; ++band)
   {
      srcBands[band]  = srcBands[minNumberOfBands - 1];
      destBands[band] = static_cast<T*>(theTile->getBuf(band));
   }

   // most of the time we will not overlap so let's
   // copy the first tile into destination and check later.
   //
   for(band = 0; band < theTile->getNumberOfBands();++band)
   {
      T* destBand = destBands[band];
      T* srcBand  = srcBands[band];
      if(destBand&&srcBand)
      {
         for(offset = 0; offset < upperBound;++offset)
         {
            *destBand = *srcBand;
            ++srcBand; ++destBand;
         }
      }
   }
   theTile->setDataObjectStatus(currentImageData->getDataObjectStatus());

   while(currentImageData.valid())
   {
      ossimDataObjectStatus currentStatus     = currentImageData->getDataObjectStatus();
      point = currentImageData->getOrigin();
      long h = (long)currentImageData->getHeight();
      long w = (long)currentImageData->getWidth();
      if( (currentStatus != OSSIM_EMPTY) &&
          (currentStatus != OSSIM_NULL))
      {
         ++numberOfTilesProcessed;
         offset = 0;
         minNumberOfBands = currentImageData->getNumberOfBands();
         for(band = 0; band < minNumberOfBands; ++band)
         {
            srcBands[band]  = static_cast<T*>(currentImageData->getBuf(band));
         }
         // if the src is smaller than the destination in number
         // of bands we will just duplicate the last band.
         for(;band < theLargestNumberOfInputBands; ++band)
         {
            srcBands[band]  = srcBands[minNumberOfBands - 1];
         }
          if(currentStatus == OSSIM_PARTIAL)
          {
            for(row = 0; row < h; ++row)
            {
               for(col = 0; col < w; ++col)
               {
                  if(!currentImageData->isNull(offset))
                  {
                     double weight = computeWeight(layerIdx,
                                                   ossimDpt(point.x+col,
                                                            point.y+row));
                     
                     for(band = 0; band < theLargestNumberOfInputBands; ++band)
                     {
                        bandRes = static_cast<float*>(theResult->getBuf(band));
                        bandRes[offset] += (srcBands[band][offset]*weight);
                     }
                     sumBand[offset] += weight;
                  }
                  ++offset;
               }
            }
         }
         else
         {
            offset = 0;
            
            for(row = 0; row < h; ++row)
            {
               for(col = 0; col < w; ++col)
               {
                     double weight = computeWeight(layerIdx,
                                                   ossimDpt(point.x+col,
                                                            point.y+row));
                     
                     for(band = 0; band < theLargestNumberOfInputBands; ++band)
                     {
                        bandRes     = static_cast<float*>(theResult->getBuf(band));
                        
                        bandRes[offset] += (srcBands[band][offset]*weight);
                     }
                     sumBand[offset] += weight;
                     ++offset;
               }
            }
         }
      }
      currentImageData = getNextTile(layerIdx, tileRect, resLevel);
   }
   upperBound = theTile->getWidth()*theTile->getHeight();

   if(numberOfTilesProcessed > 1)
   {
      const double* minPix = theTile->getMinPix();
      const double* maxPix = theTile->getMaxPix();
      const double* nullPix= theTile->getNullPix();
      for(offset = 0; offset < upperBound;++offset)
      {
         for(band = 0; band < theTile->getNumberOfBands();++band)
         {
            T* destBand      = static_cast<T*>(theTile->getBuf(band));
            float* weightedBand = static_cast<float*>(theResult->getBuf(band));

            // this should be ok to test 0.0 instead of
            // FLT_EPSILON range for 0 since we set it.
            if(sumBand[offset] != 0.0)
            {            
               weightedBand[offset] = (weightedBand[offset])/sumBand[offset];
               if(weightedBand[offset]<minPix[band])
               {
                  weightedBand[offset] = minPix[band];   
               }
               else if(weightedBand[offset] > maxPix[band])
               {
                  weightedBand[offset] = maxPix[band];                   
               }
            }
            else
            {
               weightedBand[offset] = nullPix[band];
            }
            destBand[offset] = static_cast<T>(weightedBand[offset]);
         }
      }
      theTile->validate();
   }

   delete [] srcBands;
   delete [] destBands;

   return theTile;
}

double ossimFeatherMosaic::computeWeight(long index,
                                         const ossimDpt& point)const
{
   ossimFeatherInputInformation& info = theInputFeatherInformation[index];
   double result = 0.0;
   ossimDpt delta = point-info.theCenter;
   
   double length1 = fabs(delta.x*info.theAxis1.x + delta.y*info.theAxis1.y)/info.theAxis1Length;
   double length2 = fabs(delta.x*info.theAxis2.x + delta.y*info.theAxis2.y)/info.theAxis2Length;

   if(length1 > length2)
   {
      result = (1.0 - length1);
   }
   else
   {
      result = (1.0 - length2);
   }
   if(result < 0) result = 0;
   
   return result;
}

void ossimFeatherMosaic::initialize()
{
   ossimImageMosaic::initialize();

   allocate();
   if(theTile.valid())
   {
      theAlphaSum = new ossimImageData(this,
                                       OSSIM_FLOAT,
                                       1,
                                       theTile->getWidth(),
                                       theTile->getHeight());
      theResult = new ossimImageData(this,
                                     OSSIM_FLOAT,
                                     theLargestNumberOfInputBands,
                                     theTile->getWidth(),
                                     theTile->getHeight());
      theAlphaSum->initialize();
      theResult->initialize();
   }
   std::vector<ossimIpt> validVertices;
   if(!getNumberOfInputs()) return;

   if(theInputFeatherInformation)
   {
      delete [] theInputFeatherInformation;
      theInputFeatherInformation = NULL;
   }
   theFeatherInfoSize = getNumberOfInputs();
   theInputFeatherInformation = new ossimFeatherInputInformation[theFeatherInfoSize];
   for(long index = 0; index < theFeatherInfoSize; ++ index)
   {
      validVertices.clear();
      ossimImageSource* temp = PTR_CAST(ossimImageSource, getInput(index));
      if(temp)
      {
         temp->getValidImageVertices(validVertices, OSSIM_CLOCKWISE_ORDER);
         theInputFeatherInformation[index].setVertexList(validVertices);
      }
   }
}


ossimFeatherMosaic::ossimFeatherInputInformation::ossimFeatherInputInformation(const std::vector<ossimIpt>& validVertices)
{
   setVertexList(validVertices);
}


void ossimFeatherMosaic::ossimFeatherInputInformation::setVertexList(const std::vector<ossimIpt>& validVertices)
{
   const char* MODULE = "ossimFeatherMosaic::ossimFeatherInputInformation::setVertexList()";
   
   theValidVertices = validVertices;

   theCenter       = ossimDpt(0,0);
   theAxis1        = ossimDpt(1, 0);
   theAxis2        = ossimDpt(0, 1);
   theAxis1Length  = 1;
   theAxis2Length  = 1;

   double xSum=0.0, ySum=0.0;
   ossim_uint32 upperBound = (ossim_uint32)validVertices.size();
   if(upperBound)
   {
      for(ossim_uint32 index = 0; index < upperBound; ++index)
      {
         xSum += validVertices[index].x;
         ySum += validVertices[index].y;
      }

      theCenter.x = xSum/upperBound;
      theCenter.y = ySum/upperBound;

      // for now we just want a quick implementation of something
      // and we know that we have 4 vertices for the bounding valid
      // vertices.
      //
      if(upperBound == 4)
      {
         ossimDpt edgeDirection1 = validVertices[1] - validVertices[0];
         ossimDpt edgeDirection2 = validVertices[2] - validVertices[1];

         theAxis1 = ossimDpt(-edgeDirection1.y, edgeDirection1.x);
         
         theAxis2 = ossimDpt(-edgeDirection2.y, edgeDirection2.x);

         theAxis1 = theAxis1/theAxis1.length();
         theAxis2 = theAxis2/theAxis2.length();

         ossimLine line1(theCenter,
                         theCenter + theAxis1*2);
         ossimLine line2(validVertices[1],
                         validVertices[0]);
         ossimLine line3(theCenter,
                         theCenter + theAxis2*2);
         ossimLine line4(validVertices[2],
                         validVertices[1]);
         
         ossimDpt intersectionPoint1 = line1.intersectInfinite(line2);
         ossimDpt intersectionPoint2 = line3.intersectInfinite(line4);

         
         theAxis1Length = ossim::round<int>((theCenter-intersectionPoint1).length());
         theAxis2Length = ossim::round<int>((theCenter-intersectionPoint2).length());

          if(traceDebug())
          {
             CLOG << "theAxis1Length:       " << theAxis1Length << endl
                  << "theAxis2Length:       " << theAxis2Length << endl
                  << "center:               " << theCenter      << endl;
          }
      }
   }
}

ostream& operator<<(ostream& out,
                    const ossimFeatherMosaic::ossimFeatherInputInformation& data)
{
   out << "center: " << data.theCenter << endl
       << "axis1:  " << data.theAxis1  << endl
       << "axis2:  " << data.theAxis2  << endl
       << "axis1_length: " << data.theAxis1Length << endl
       << "axis2_length: " << data.theAxis2Length << endl
       << "valid vertices: " << endl;
   std::copy(data.theValidVertices.begin(),
             data.theValidVertices.end(),
             std::ostream_iterator<ossimDpt>(out, "\n"));
   return out;
}
