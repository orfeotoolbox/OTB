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
// $Id: ossimRectangleCutFilter.cpp 21631 2012-09-06 18:10:55Z dburken $
#include <ossim/imaging/ossimRectangleCutFilter.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageGeometry.h>

static ossimTrace traceDebug("ossimRectangleCutFilter:debug");

RTTI_DEF1(ossimRectangleCutFilter,
          "ossimRectangleCutFilter",
          ossimImageSourceFilter);

ossimRectangleCutFilter::ossimRectangleCutFilter(ossimObject* owner,
                                     ossimImageSource* inputSource)
   :ossimImageSourceFilter(owner, inputSource),
    theCutType(OSSIM_RECTANGLE_NULL_OUTSIDE)
{
   theRectangle.makeNan();
}

ossimRectangleCutFilter::ossimRectangleCutFilter(ossimImageSource* inputSource)
   :ossimImageSourceFilter(NULL, inputSource),
    theCutType(OSSIM_RECTANGLE_NULL_OUTSIDE)
{
   theRectangle.makeNan();
}


ossimRefPtr<ossimImageData> ossimRectangleCutFilter::getTile(
   const ossimIrect& rect,
   ossim_uint32 resLevel)
{
   if(!theInputConnection)
   {
      return NULL;
   }
   
   ossimRefPtr<ossimImageData> tile =
      theInputConnection->getTile(rect, resLevel);
   if(!isSourceEnabled()||theRectangle.hasNans()||!tile.valid())
   {
      return tile;
   }
   if(tile->getDataObjectStatus() == OSSIM_NULL ||
      tile->getDataObjectStatus() == OSSIM_EMPTY)
   {
      return tile;
   }
   ossim_int32 decimationIndex = min((ossim_int32)resLevel,
                                     (ossim_int32)theDecimationList.size()-1);

   ossimIrect cutRect = theRectangle*theDecimationList[decimationIndex];
   
   ossimIrect inputRectangle = tile->getImageRectangle();
   if(theCutType==OSSIM_RECTANGLE_NULL_INSIDE)
   {
      if(cutRect.intersects(inputRectangle))
      {
         if(!inputRectangle.completely_within(cutRect))
         {
            ossim_int32 ulx = inputRectangle.ul().x;
            ossim_int32 uly = inputRectangle.ul().y;
            ossim_int32 w   = tile->getWidth();
            ossim_int32 h   = tile->getHeight();
            ossim_int32 offset = 0;
            ossimIpt tempPoint(ulx, uly);
            
            for(ossim_int32 y = 0;  y < h; ++tempPoint.y,++y)
            {
               tempPoint.x = ulx;
               for(ossim_int32 x = 0; x < w; ++tempPoint.x,++x)
               {
                  if(cutRect.pointWithin(tempPoint))
                  {
                     tile->setNull(offset);
                  }
                  ++offset;
               }
            }
            tile->validate();
         }
         else
         {
            tile->makeBlank();
         }
      }
   }
   else // cut out the outide
   {      
      if(cutRect.intersects(inputRectangle))
      {
         if(!inputRectangle.completely_within(cutRect))
         {
            ossim_int32 ulx = inputRectangle.ul().x;
            ossim_int32 uly = inputRectangle.ul().y;
            ossim_int32 w   = tile->getWidth();
            ossim_int32 h   = tile->getHeight();
            ossim_int32 offset = 0;
            ossimIpt tempPoint(ulx, uly);
            
            for(ossim_int32 y = 0;  y < h; ++tempPoint.y,++y)
            {
               tempPoint.x = ulx;
               for(ossim_int32 x = 0; x < w; ++tempPoint.x,++x)
               {
                  if(!cutRect.pointWithin(tempPoint))
                  {
                     tile->setNull(offset);
                  }
                  ++offset;
               }
            }
            tile->validate();
         }
      }
      else
      {
         tile->makeBlank();
      }
   }
   
   return tile;
}

void ossimRectangleCutFilter::setRectangle(const ossimIrect& rect)
{
   theRectangle = rect;

   if(theRectangle.hasNans())
   {
      if(theInputConnection)
      {
         theRectangle = theInputConnection->getBoundingRect();
      }
   }
}

void ossimRectangleCutFilter::getValidImageVertices(
   vector<ossimIpt>& validVertices,
   ossimVertexOrdering ordering,
   ossim_uint32 /* resLevel */)const
{
   ossimIrect rect = getBoundingRect();

   if(ordering == OSSIM_CLOCKWISE_ORDER)
   {
      validVertices.push_back(rect.ul());
      validVertices.push_back(rect.ur());
      validVertices.push_back(rect.lr());
      validVertices.push_back(rect.ll());
   }
   else
   {
      validVertices.push_back(rect.ul());
      validVertices.push_back(rect.ll());
      validVertices.push_back(rect.lr());
      validVertices.push_back(rect.ur());
   }
}

ossimIrect ossimRectangleCutFilter::getBoundingRect(ossim_uint32 resLevel)const
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimRectangleCutFilter::getBoundingRect DEBUG: entered..." << std::endl;
   }
   ossimIrect result;

   result.makeNan();
   if(!theInputConnection)
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "ossimRectangleCutFilter::getBoundingRect DEBUG: Input connection was not valid so leaving" << std::endl;
      }
      return result;
   }

   result = theInputConnection->getBoundingRect(resLevel);
   if(isSourceEnabled())
   {
      if(theCutType == OSSIM_RECTANGLE_NULL_OUTSIDE)
      {
         ossimDpt decimation;
         getDecimationFactor(resLevel, decimation);
         ossimIrect cutRect = theRectangle;
         if(!decimation.hasNans())
         {
            cutRect = theRectangle*decimation;
         }
         result = cutRect;
      }
   }

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimRectangleCutFilter::getBoundingRect DEBUG: cut rect = " << result  << std::endl;
   }
   return result;
}

void ossimRectangleCutFilter::initialize()
{
   theDecimationList.clear();
   if(theInputConnection)
   {
      getDecimationFactors(theDecimationList);
   }
   if(theDecimationList.empty())
   {
      theDecimationList.push_back(ossimDpt(1,1));
   }
   if(theRectangle.hasNans())
   {
      setRectangle(theRectangle);
   }
}

bool ossimRectangleCutFilter::saveState(ossimKeywordlist& kwl,
                                        const char* prefix)const
{
   ossimString newPrefix = prefix;
   newPrefix+="clip_rect.";
   
   theRectangle.saveState(kwl, newPrefix);

   if(theCutType == OSSIM_RECTANGLE_NULL_INSIDE)
   {
      kwl.add(prefix,
              "cut_type",
              "null_inside",
              true);
   }
   else if(theCutType == OSSIM_RECTANGLE_NULL_OUTSIDE)
   {
      kwl.add(prefix,
              "cut_type",
              "null_outside",
              true);      
   }
   
   return ossimImageSourceFilter::saveState(kwl, prefix);
}

bool ossimRectangleCutFilter::loadState(const ossimKeywordlist& kwl,
                                        const char* prefix)
{
   ossimString newPrefix = prefix;
   
   ossimString rect = kwl.find(prefix, "rect");
   if(!rect.empty())
   {
      theRectangle.toRect(rect);
   }
   else 
   {
      newPrefix+="clip_rect.";
      
      theRectangle.loadState(kwl, newPrefix.c_str());
   }

   const char* cutType = kwl.find(prefix, "cut_type");
   if(cutType)
   {
      ossimString c = cutType;
      if(c == "null_inside")
      {
         theCutType = OSSIM_RECTANGLE_NULL_INSIDE;
      }
      else if(c == "null_outside")
      {
         theCutType = OSSIM_RECTANGLE_NULL_OUTSIDE;         
      }
      else
      {
         theCutType = static_cast<ossimRectangleCutType>(ossimString(cutType).toLong());
      }
   }

   return ossimImageSourceFilter::loadState(kwl, prefix);
}

const ossimIrect& ossimRectangleCutFilter::getRectangle()const
{
   return theRectangle;
}

ossimRectangleCutFilter::ossimRectangleCutType ossimRectangleCutFilter::getCutType()const
{
   return theCutType;
}

void ossimRectangleCutFilter::setCutType(ossimRectangleCutType cutType)
{
   theCutType = cutType;
}
