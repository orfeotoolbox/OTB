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
// $Id: ossimPolyCutter.cpp 21518 2012-08-22 21:15:56Z dburken $
#include <ossim/imaging/ossimPolyCutter.h>
#include <ossim/base/ossimPolyArea2d.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimRgbImage.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimActiveEdgeTable.h>
static const char* NUMBER_POLYGONS_KW = "number_polygons";

RTTI_DEF1(ossimPolyCutter, "ossimPolyCutter", ossimImageSourceFilter)

ossimPolyCutter::ossimPolyCutter()
   : ossimImageSourceFilter(),
     theTile(NULL),
     theCutType(OSSIM_POLY_NULL_OUTSIDE),
     m_boundingOverwrite(false)
{
   thePolygonList.push_back(ossimPolygon());
   theBoundingRect.makeNan();
}

// ossimPolyCutter::ossimPolyCutter(ossimImageSource* inputSource,
//                                  ossimPolyArea2d* polygon)
ossimPolyCutter::ossimPolyCutter(ossimImageSource* inputSource,
                                 const ossimPolygon& polygon)
   : ossimImageSourceFilter(inputSource),
     theTile(NULL),
     theCutType(OSSIM_POLY_NULL_INSIDE),
     m_boundingOverwrite(false)
{
   thePolygonList.push_back(polygon);
   computeBoundingRect();
   initialize();
}

ossimPolyCutter::~ossimPolyCutter()
{
}

ossimRefPtr<ossimImageData> ossimPolyCutter::getTile(const ossimIrect& tileRect,
                                                     ossim_uint32 resLevel)
{
   if(!theInputConnection)
   {
      return theTile;
   }
   ossimRefPtr<ossimImageData> input = theInputConnection->getTile(tileRect,
                                                                   resLevel);
   
   if(!isSourceEnabled() || theBoundingRect.hasNans() || !input.valid())
   {
      return input;
   }
   
   if((input->getDataObjectStatus() == OSSIM_EMPTY) ||
      (input->getDataObjectStatus() == OSSIM_NULL))
   {
      return input;
   }

   if(!theTile.valid())
   {
      allocate(); // First time through...
   }

   if(!theTile.valid())
   {
      return input;
   }
   
   theTile->setImageRectangle(tileRect);
   
   theTile->loadTile(input.get());
   theTile->setDataObjectStatus(input->getDataObjectStatus());
   vector<ossimPolygon>* polyList = &thePolygonList;
   vector<ossimPolygon>  scaledPoly;
   
   ossimIrect boundingRect = getBoundingRect(resLevel);
   if(resLevel)
   {
      ossimDpt decimation;
      getDecimationFactor(resLevel, decimation);

      if(!decimation.hasNans())
      {
         for(int polyIndex = 0;
             polyIndex < (int)thePolygonList.size();
             ++polyIndex)
         {
            scaledPoly.push_back(thePolygonList[polyIndex]*decimation);
         }
         polyList = &scaledPoly;
      }
   }
   
   if(polyList->size()&&
      theTile->getDataObjectStatus()!=OSSIM_NULL)
   {
//       ossimActiveEdgeTable aet;
      
      
      if(theCutType == OSSIM_POLY_NULL_OUTSIDE)
      {
         if(boundingRect.intersects(tileRect))
         {
            theTile->makeBlank();
            theHelper.setImageData(theTile.get());
            for(int polyIndex = 0; polyIndex < (int)polyList->size(); ++polyIndex)
            {
               theHelper.copyInputToThis(input->getBuf(),
                                      (*polyList)[polyIndex]);
            }
            theTile->validate();
         }
         else
         {
            theTile->makeBlank();
         }
      }
      else if(theCutType == OSSIM_POLY_NULL_INSIDE)
      {
         if(boundingRect.intersects(tileRect))
         {
            theHelper.setImageData(theTile.get());
            for(int polyIndex = 0;
                polyIndex < (int)polyList->size();
                ++polyIndex)
            {
               theHelper.fill(theTile->getNullPix(),
                           (*polyList)[polyIndex]);
            }
         }
         theTile->validate();
      }
      
   }
   return theTile;
}

ossimIrect ossimPolyCutter::getBoundingRect(ossim_uint32 resLevel)const
{
   ossimIrect result;

   result.makeNan();
   if(!theInputConnection)
   {
      return result;
   }
   if(!isSourceEnabled())
   {
      return ossimImageSourceFilter::getBoundingRect(resLevel);
   }
   
   result = theInputConnection->getBoundingRect(resLevel);

   if(isSourceEnabled()&&(!theBoundingRect.hasNans()))
   {
      if(theCutType == OSSIM_POLY_NULL_OUTSIDE)
      {
         ossimDpt decimation;
         getDecimationFactor(resLevel, decimation);
         if(decimation.hasNans())
         {
            result =  theBoundingRect;
         }
         else
         {
            result = theBoundingRect*decimation;
         }
      }
   }
   else if(isSourceEnabled())
   {
      return theBoundingRect;
   }
   
   return result;
}

void ossimPolyCutter::initialize()
{
   // Capture the input connection.
   ossimImageSourceFilter::initialize();

   // Force an allocate on next getTile.
   theTile = NULL;
}

void ossimPolyCutter::allocate()
{
   theTile=NULL;
   if(theInputConnection)
   {
      theTile = ossimImageDataFactory::instance()->create(this,
                                                          theInputConnection);
      theTile->initialize();

   }
}

bool ossimPolyCutter::saveState(ossimKeywordlist& kwl,
                                const char* prefix)const
{
   ossimString newPrefix = prefix;

   for(int i = 0; i < (int)thePolygonList.size();++i)
   {
      newPrefix = ossimString(prefix) + "polygon" + ossimString::toString(i)+".";

      thePolygonList[i].saveState(kwl, newPrefix.c_str());
   }
   kwl.add(prefix,
           NUMBER_POLYGONS_KW,
           (int)thePolygonList.size(),
           true);
   
   ossimString fillType = "null_inside";
   if(theCutType == OSSIM_POLY_NULL_OUTSIDE)
   {
      fillType = "null_outside";
   }
   kwl.add(prefix,
           "cut_type",
           fillType.c_str(),
           true);   
  
   return ossimImageSourceFilter::saveState(kwl, prefix);;
}

bool ossimPolyCutter::loadState(const ossimKeywordlist& kwl,
                                const char* prefix)
{
   const char* numberPolygons = kwl.find(prefix, NUMBER_POLYGONS_KW);

   ossimString newPrefix;
   if(numberPolygons)
   {
      thePolygonList.clear();
      int npolys = ossimString(numberPolygons).toLong();
      for(int i = 0; i < npolys;++i)
      {
         thePolygonList.push_back(ossimPolygon());
         
         newPrefix = ossimString(prefix) + "polygon" + ossimString::toString(i)+".";
         
         thePolygonList[i].loadState(kwl, newPrefix.c_str());
      }
   }

   const char* lookup = kwl.find(prefix,
                                 "cut_type");
   if(lookup)
   {
      theCutType = OSSIM_POLY_NULL_INSIDE;
      ossimString test = lookup;
      if(test == "null_outside")
      {
         theCutType = OSSIM_POLY_NULL_OUTSIDE;
      }
   }
   else
   {
      theCutType = OSSIM_POLY_NULL_OUTSIDE;
   }

   computeBoundingRect();

   
   return ossimImageSourceFilter::loadState(kwl, prefix);
}

void ossimPolyCutter::setPolygon(const vector<ossimDpt>& polygon,
                                 ossim_uint32 index)
{
   if(index < polygon.size())
   {
      thePolygonList[index] = polygon;

      computeBoundingRect();
   }

}

void ossimPolyCutter::setPolygon(const vector<ossimIpt>& polygon,
                                 ossim_uint32 index)
{
   if(polygon.size())
   {
      thePolygonList[index] = polygon;
      computeBoundingRect();
   }
}

void ossimPolyCutter::addPolygon(const vector<ossimIpt>& polygon)
{
   thePolygonList.push_back(ossimPolygon(polygon));
   computeBoundingRect();
}

void ossimPolyCutter::addPolygon(const vector<ossimDpt>& polygon)
{
   thePolygonList.push_back(ossimPolygon(polygon));
   computeBoundingRect();
}

void ossimPolyCutter::addPolygon(const ossimPolygon& polygon)
{
   thePolygonList.push_back(ossimPolygon(polygon));
   computeBoundingRect();
}


void ossimPolyCutter::setCutType(ossimPolyCutterCutType cutType)
{
   theCutType = cutType;
}

ossimPolyCutter::ossimPolyCutterCutType ossimPolyCutter::getCutType()const
{
   return theCutType;
}

void ossimPolyCutter::clear()
{
   setNumberOfPolygons(0);
}

void ossimPolyCutter::setProperty(ossimRefPtr<ossimProperty> property)
{
   ossimImageSourceFilter::setProperty(property);
}

ossimRefPtr<ossimProperty> ossimPolyCutter::getProperty(const ossimString& name)const
{

   return ossimImageSourceFilter::getProperty(name);
}

void ossimPolyCutter::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimImageSourceFilter::getPropertyNames(propertyNames);
} 

void ossimPolyCutter::computeBoundingRect()
{
   ossimIrect bounds;
   theBoundingRect.makeNan();

   for(int i = 0; i < (int)thePolygonList.size(); ++i)
   {
      thePolygonList[i].getBoundingRect(bounds);

      if(!bounds.hasNans())
      {
         if(theBoundingRect.hasNans())
         {
            theBoundingRect = bounds;
         }
         else
         {
            theBoundingRect = theBoundingRect.combine(bounds);
         }
      }
   }
}

const ossimIrect& ossimPolyCutter::getRectangle() const
{
   return theBoundingRect;
}

void ossimPolyCutter::setRectangle(const ossimIrect& rect)
{
   theBoundingRect = rect;
   m_boundingOverwrite = true;
}

void ossimPolyCutter::setNumberOfPolygons(ossim_uint32 count)
{
   if(!count)
   {
      thePolygonList.clear();
      thePolygonList.clear();
   }
   else
   {
      thePolygonList.resize(count);
   }
}

std::vector<ossimPolygon>& ossimPolyCutter::getPolygonList()
{
   return thePolygonList;
}

const std::vector<ossimPolygon>& ossimPolyCutter::getPolygonList()const
{
   return thePolygonList;
}

