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
// $Id: ossimImageDataHelper.cpp 21184 2012-06-29 15:13:09Z dburken $
#include <ossim/imaging/ossimImageDataHelper.h>
#include <ossim/base/ossimPolyArea2d.h>
#include <ossim/base/ossimLine.h>
#include <vector>
#include <algorithm>

ossimImageDataHelper::ossimImageDataHelper(ossimImageData* imageData)
{
   setImageData(imageData);
}


void ossimImageDataHelper::setImageData(ossimImageData* imageData)
{
   theImageData = imageData;
   if(theImageData)
   {
      theImageRectangle     = theImageData->getImageRectangle();
      thePolyImageRectangle = theImageRectangle;
//       thePolyImageRectangle = ossimPolyArea2d(theImageRectangle.ul(),
//                                               theImageRectangle.ur(),
//                                               theImageRectangle.lr(),
//                                               theImageRectangle.ll());
      theOrigin = theImageRectangle.ul();
   }
}

void ossimImageDataHelper::copyInputToThis(const void* input,
                                           const ossimPolygon& region,
                                           bool clipPoly)
{
   if(!theImageData) return;
   switch(theImageData->getScalarType())
   {
      case OSSIM_UCHAR:
      {
         copyInputToThis(reinterpret_cast<const ossim_uint8*>(input),
                         region,
                         clipPoly);
         break;
      }
      case OSSIM_USHORT16:
      case OSSIM_USHORT11:
      {
         copyInputToThis(reinterpret_cast<const ossim_uint16*>(input),
                         region,
                         clipPoly);
         break;
      }
      case OSSIM_SSHORT16:
      {
         copyInputToThis(reinterpret_cast<const ossim_sint16*>(input),
                         region,
                         clipPoly);
         break;
      }
      case OSSIM_SINT32:
      {
          copyInputToThis(reinterpret_cast<const ossim_sint32*>(input),
                          region,
                          clipPoly);
          break;
      }
      case OSSIM_FLOAT:
      case OSSIM_NORMALIZED_FLOAT:
      {
         copyInputToThis(reinterpret_cast<const float*>(input),
                         region,
                         clipPoly);
         break;
      }
      case OSSIM_DOUBLE:
      case OSSIM_NORMALIZED_DOUBLE:
      {
         copyInputToThis(reinterpret_cast<const double*>(input),
                         region,
                         clipPoly);
         break;
      }
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         break;
      }
   }   
}

void ossimImageDataHelper::fill(const ossimRgbVector& color,
                                std::vector<ossimPolygon>& regionList,
                                bool clipPoly)
{
   int i = 0;

   for(i = 0; i < (int)regionList.size();++i)
   {
      fill(color,
           regionList[i],
           clipPoly);
   }
}


void ossimImageDataHelper::fill(const double* values,
                                std::vector<ossimPolygon>& regionList,
                                bool clipPoly)
{
   int i = 0;

   for(i = 0; i < (int)regionList.size();++i)
   {
      fill(values,
           regionList[i],
           clipPoly);
   }
}

void ossimImageDataHelper::fill(const ossimRgbVector& color,
                                const ossimPolygon& region,
                                bool clipPoly)
{
   double colorArray[3];

   colorArray[0] = color.getR();
   colorArray[1] = color.getG();
   colorArray[2] = color.getB();
   switch(theImageData->getScalarType())
   {
      case OSSIM_UCHAR:
      {
         if(theImageData->getNumberOfBands() <= 3)
         {
            fill(static_cast<ossim_uint8>(0),
                 (const double*)colorArray,
                 region,
                 clipPoly);
            
         }
         break;
      }
      case OSSIM_SCALAR_UNKNOWN:
      case OSSIM_USHORT16:
      case OSSIM_USHORT11:
      case OSSIM_SSHORT16:
      case OSSIM_FLOAT:
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_DOUBLE:
      case OSSIM_NORMALIZED_DOUBLE:
      default:
      {
         break;
      }
   }
}

void ossimImageDataHelper::fill(const ossimRgbVector& color,
                                const ossimIrect& region,
                                bool clipPoly)
{
   double colorArray[3];

   colorArray[0] = color.getR();
   colorArray[1] = color.getG();
   colorArray[2] = color.getB();

   switch(theImageData->getScalarType())
   {
      case OSSIM_UCHAR:
      {
         if(theImageData->getNumberOfBands() <= 3)
         {
            fill(static_cast<ossim_uint8>(0),
                 colorArray,
                 region,
                 clipPoly);
            
         }
         break;
      }
      case OSSIM_SCALAR_UNKNOWN:
      case OSSIM_USHORT16:
      case OSSIM_USHORT11:
      case OSSIM_SSHORT16:
      case OSSIM_FLOAT:
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_DOUBLE:
      case OSSIM_NORMALIZED_DOUBLE:
      {
         break;
      }
      default:
         break;
   }
}

void ossimImageDataHelper::fill(const double*  values,
                                const ossimPolygon& region,
                                bool clipPoly)
{
   switch(theImageData->getScalarType())
   {
      case OSSIM_UCHAR:
      {
         fill(static_cast<ossim_uint8>(0),
              values,
              region,
              clipPoly);
         break;
      }
      case OSSIM_USHORT16:
      case OSSIM_USHORT11:
      {
         fill(static_cast<ossim_uint16>(0),
              values,
              region,
              clipPoly);
         break;
      }
      case OSSIM_SSHORT16:
      {
         fill(static_cast<ossim_sint16>(0),
              values,
              region,
              clipPoly);
         break;
      }
      case OSSIM_FLOAT:
      case OSSIM_NORMALIZED_FLOAT:
      {
         fill(static_cast<float>(0),
              values,
              region,
              clipPoly);
         break;
      }
      case OSSIM_DOUBLE:
      case OSSIM_NORMALIZED_DOUBLE:
      {
         fill(static_cast<double>(0),
              values,
              region,
              clipPoly);
         break;
      }
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         break;
      }
   }
}

void ossimImageDataHelper::fill(const double*  values,
                                const ossimIrect& region,
                                bool clipPoly)
{
   switch(theImageData->getScalarType())
   {
      case OSSIM_UCHAR:
      {
         fill(static_cast<ossim_uint8>(0),
              values,
              region,
              clipPoly);
         break;
      }
      case OSSIM_USHORT16:
      case OSSIM_USHORT11:
      {
         fill(static_cast<ossim_uint16>(0),
              values,
              region,
              clipPoly);
         break;
      }
      case OSSIM_SSHORT16:
      {
         fill(static_cast<ossim_sint16>(0),
              values,
              region,
              clipPoly);
         break;
      }
      case OSSIM_FLOAT:
      case OSSIM_NORMALIZED_FLOAT:
      {
         fill(static_cast<float>(0),
              values,
              region,
              clipPoly);
         break;
      }
      case OSSIM_DOUBLE:
      case OSSIM_NORMALIZED_DOUBLE:
      {
         fill(static_cast<double>(0),
              values,
              region,
              clipPoly);
         break;
      }
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         break;
      }
   }
}



template <class T>
void ossimImageDataHelper::copyInputToThis(const T* inputBuf,
                                           const ossimPolygon& region,
                                           bool clipPoly)
{
   if(clipPoly)
   {
      ossimPolyArea2d polyArea(region);
      ossimPolyArea2d clipArea = polyArea&thePolyImageRectangle;
      
      vector<ossimPolygon> clipList;
//      clipArea.getAllVisiblePolygons(clipList);
      clipArea.getVisiblePolygons(clipList);
      int i = 0;
            
      for(i = 0; i < (int)clipList.size();++i)
      {
         copyInputToThis(inputBuf,
                         clipList[i]);
      }
   }
   else
   {
      copyInputToThis(inputBuf,
                      region);
   }
}

template <class T>
void ossimImageDataHelper::copyInputToThis(const T* inputBuf,
                                           const ossimPolygon& region)
{ 
   int n;
   int i;
   int y;
   ossim_int32 miny, maxy, minx, maxx;
   int x1, y1;
   int x2, y2;
   int ind1, ind2;
   // int ints;
   vector<int> polyInts;
   T* buf = reinterpret_cast<T*>(theImageData->getBuf());
   
   ossim_int32 blockLength=theImageData->getWidth()*theImageData->getHeight();
   ossim_int32 bandOffset = 0;
   
   region.getIntegerBounds(minx, miny, maxx, maxy);
   ossim_int32 rowOffset = (miny-theOrigin.y)*theImageData->getWidth();
   n = region.getVertexCount();
   /* Fix in 1.3: count a vertex only once */
   for (y = miny; (y <= maxy); y++)
   {
      polyInts.clear();
      // ints = 0;
      for (i = 0; (i < n); i++)
      {
         if (!i)
         {
            ind1 = n - 1;
            ind2 = 0;
         }
         else
         {
            ind1 = i - 1;
            ind2 = i;
         }
         y1 = ossim::round<int>(region[ind1].y);
         y2 = ossim::round<int>(region[ind2].y);
         if (y1 < y2)
         {
            x1 = ossim::round<int>(region[ind1].x);
            x2 = ossim::round<int>(region[ind2].x);
         }
         else if (y1 > y2)
         {
            y2 = ossim::round<int>(region[ind1].y);
            y1 = ossim::round<int>(region[ind2].y);
            x2 = ossim::round<int>(region[ind1].x);
            x1 = ossim::round<int>(region[ind2].x);
         }
         else
         {
            continue;
         }
         if ((y >= y1) && (y < y2))
         {
            polyInts.push_back((y - y1) * (x2 - x1) / (y2 - y1) + x1);
         }
         else if ((y == maxy) && (y > y1) && (y <= y2))
         {
            polyInts.push_back((y - y1) * (x2 - x1) / (y2 - y1) + x1);
         }
      }
      std::sort(polyInts.begin(), polyInts.end());
      
      for (i = 0; (i < (int)polyInts.size()); i += 2)
      {
         ossim_int32 startX = polyInts[i]-theOrigin.x;
         ossim_int32 endX   = polyInts[i+1]-theOrigin.x;
         while(startX <= endX)
         {
            bandOffset = 0;
	    int band;
            for(band = 0; band < (ossim_int32)theImageData->getNumberOfBands();++band)
            {
               buf[rowOffset+bandOffset+startX] = (inputBuf[rowOffset+bandOffset+startX]);
               bandOffset += blockLength;
            }
            ++startX;
         }
      }
      rowOffset += theImageData->getWidth();
   }
}

template <class T>
void ossimImageDataHelper::fill(T dummyVariable,
                                const double* values,
                                const ossimPolygon& region,
                                bool clipPoly)
{
   if(clipPoly)
   {
      ossimPolyArea2d polyArea(region);
      ossimPolyArea2d clipArea = polyArea&thePolyImageRectangle;
      
      vector<ossimPolygon> clipList;
//      clipArea.getAllVisiblePolygons(clipList);
      clipArea.getVisiblePolygons(clipList);
            
      int i = 0;
      for(i = 0; i < (int)clipList.size();++i)
      {
         fill(dummyVariable,
              values,
              clipList[i]);
      }
   }
   else
   {
      fill(dummyVariable,
           values,
           region);
   }
}

template <class T>
void ossimImageDataHelper::fill(T dummyVariable,
                                const double* values,
                                const ossimIrect& region,
                                bool clipPoly)
{
   if(clipPoly)
   {
      fill(dummyVariable,
           values,
           region.clipToRect(theImageData->getImageRectangle()));
   }
   else
   {
      fill(dummyVariable,
           values,
           region);
   }
}

template <class T>
void ossimImageDataHelper::fill(T /* dummyVariable */,
                                const double* values,
                                const ossimPolygon& region)
{
   int n;
   int i;
   int y;
   ossim_int32 miny, maxy, minx, maxx;
   int x1, y1;
   int x2, y2;
   int ind1, ind2;
   // int ints;
   vector<int> polyInts;
   T* buf = reinterpret_cast<T*>(theImageData->getBuf());
   
   ossim_int32 blockLength=theImageData->getWidth()*theImageData->getHeight();
   ossim_int32 bandOffset = 0;
   
   region.getIntegerBounds(minx, miny, maxx, maxy);
   ossim_int32 rowOffset = (miny-theOrigin.y)*theImageData->getWidth();
   n = region.getVertexCount();
   /* Fix in 1.3: count a vertex only once */
   for (y = miny; (y <= maxy); y++)
   {
      polyInts.clear();
      // ints = 0;
      for (i = 0; (i < n); i++)
      {
         if (!i)
         {
            ind1 = n - 1;
            ind2 = 0;
         }
         else
         {
            ind1 = i - 1;
            ind2 = i;
         }
         y1 = ossim::round<int>(region[ind1].y);
         y2 = ossim::round<int>(region[ind2].y);
         if (y1 < y2)
         {
            x1 = ossim::round<int>(region[ind1].x);
            x2 = ossim::round<int>(region[ind2].x);
         }
            else if (y1 > y2)
            {
               y2 = ossim::round<int>(region[ind1].y);
               y1 = ossim::round<int>(region[ind2].y);
               x2 = ossim::round<int>(region[ind1].x);
               x1 = ossim::round<int>(region[ind2].x);
            }
         else
         {
            continue;
         }
         if ((y >= y1) && (y < y2))
         {
            polyInts.push_back((y - y1) * (x2 - x1) / (y2 - y1) + x1);
         }
         else if ((y == maxy) && (y > y1) && (y <= y2))
         {
            polyInts.push_back((y - y1) * (x2 - x1) / (y2 - y1) + x1);
         }
      }
      std::sort(polyInts.begin(), polyInts.end());
      
      for (i = 0; (i < (int)polyInts.size()); i += 2)
      {
         ossim_int32 startX = std::abs(polyInts[i]-theOrigin.x);
         ossim_int32 endX   = std::abs(polyInts[i+1]-theOrigin.x);
         while(startX <= endX)
         {
            bandOffset = 0;
	    int band;
            for(band = 0; band < (int)theImageData->getNumberOfBands();++band)
            {
               buf[rowOffset+bandOffset+startX] = (T)(values[band]);
               bandOffset += blockLength;
            }
            ++startX;
         }
      }
      rowOffset += theImageData->getWidth();
   }
}

template <class T>
void ossimImageDataHelper::fill(T /* dummyVariable */,
                                const double* values,
                                const ossimIrect& region)
{
   T* buf = reinterpret_cast<T*>(theImageData->getBuf());
   
   ossim_int32 blockLength=theImageData->getWidth()*theImageData->getHeight();
   ossim_int32 bandOffset = 0;
   ossim_int32 miny,maxy;
   ossim_int32 minx, maxx;
   ossim_int32 y = 0;
   miny = region.ul().y-theOrigin.y;
   maxy = region.lr().y-theOrigin.y;
   minx = region.ul().x-theOrigin.x;
   maxx = region.lr().x-theOrigin.x;
   
   ossim_int32 rowOffset = (miny)*theImageData->getWidth();
   ossim_int32 startX = minx;
   ossim_int32 endX   = maxx;
   ossim_int32 bands  = (ossim_int32)theImageData->getNumberOfBands();

   for (y = miny; (y <= maxy); ++y)
   {
      startX=minx;
      while(startX <= endX)
      {
         bandOffset = 0;
	 int band = 0;
         for(band = 0; band < bands;++band)
         {
            buf[rowOffset+bandOffset+startX] = (T)(values[band]);
            bandOffset += blockLength;
         }
         ++startX;
      }
      rowOffset += theImageData->getWidth();
   }
}
 
