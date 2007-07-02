//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimGeoAnnotationPolyObject.cpp 9479 2006-08-31 20:46:39Z gpotts $

#include <sstream>

#include <ossim/imaging/ossimGeoAnnotationPolyObject.h>
#include <ossim/imaging/ossimAnnotationPolyObject.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/base/ossimTrace.h>

static const ossimTrace
traceDebug(ossimString("ossimGeoAnnotationPolyObject:debug"));

RTTI_DEF1(ossimGeoAnnotationPolyObject,
          "ossimGeoAnnotationPolyObject",
          ossimGeoAnnotationObject)

ossimGeoAnnotationPolyObject::ossimGeoAnnotationPolyObject(bool enableFill,
                                                           ossim_uint8 r,
                                                           ossim_uint8 g,
                                                           ossim_uint8 b,
                                                           ossim_uint8 thickness)
   :ossimGeoAnnotationObject(r, g, b, thickness),
    thePolygon(),
    theProjectedPolyObject(NULL)
{
   theProjectedPolyObject = new ossimAnnotationPolyObject(enableFill,
                                                          r,
                                                          g,
                                                          b,
                                                          thickness);
}
   
ossimGeoAnnotationPolyObject::ossimGeoAnnotationPolyObject(
   const std::vector<ossimGpt>& groundPts,
   bool enableFill,
   ossim_uint8 r,
   ossim_uint8 g,
   ossim_uint8 b,
   ossim_uint8 thickness)
   :ossimGeoAnnotationObject(r, g, b, thickness),
    thePolygon(),
    theProjectedPolyObject(NULL)
{
   
   thePolygon = groundPts;
   
   // we will initialize the projected polygon's size
   // so we don't have to reset it every time we do a
   // projection
   //
   std::vector<ossimDpt> projectedPoints(thePolygon.size());
   
   theProjectedPolyObject = new ossimAnnotationPolyObject(projectedPoints,
                                                          enableFill,
                                                          r,
                                                          g,
                                                          b,
                                                          thickness);
}

ossimGeoAnnotationPolyObject::ossimGeoAnnotationPolyObject(
   const ossimGeoAnnotationPolyObject& rhs)
   :ossimGeoAnnotationObject(rhs),
    thePolygon(rhs.thePolygon),
    theProjectedPolyObject(rhs.theProjectedPolyObject?(ossimAnnotationPolyObject*)rhs.theProjectedPolyObject->dup():(ossimAnnotationPolyObject*)NULL)
{
}

ossimGeoAnnotationPolyObject::~ossimGeoAnnotationPolyObject()
{
   if(theProjectedPolyObject)
   {
      delete theProjectedPolyObject;
      theProjectedPolyObject = NULL;
   }
}

ossimObject* ossimGeoAnnotationPolyObject::dup()const
{
   return new ossimGeoAnnotationPolyObject(*this);
}

void ossimGeoAnnotationPolyObject::applyScale(double x,
                                              double y)
{
   for(int i = 0; i < (int)thePolygon.size(); ++i)
   {
      thePolygon[i].lond(thePolygon[i].lond()*x);
      thePolygon[i].latd(thePolygon[i].latd()*y);
   }
   if(theProjectedPolyObject)
   {
      theProjectedPolyObject->applyScale(x, y);
   }
      
}

void ossimGeoAnnotationPolyObject::transform(ossimProjection* projection)
{
   // make sure it's not null
   if(!projection)
   {
      return;
   }
   ossimPolygon& poly = theProjectedPolyObject->getPolygon();
   long upperBound = thePolygon.size();

   for(long index=0; index < upperBound; ++index)
   {
      projection->worldToLineSample(thePolygon[index], poly[index]);
   }

   // update the bounding rect
   //
   theProjectedPolyObject->computeBoundingRect();
}

std::ostream& ossimGeoAnnotationPolyObject::print(std::ostream& out)const
{
   out << "number_of_points:  " << thePolygon.size() << std::endl;
   if(thePolygon.size() > 0)
   {
      for(long index =0; index < (long)(thePolygon.size()-1); ++index)
      {
         out << thePolygon[index] << std::endl;
      }
      out << thePolygon[thePolygon.size()-1] << std::endl;
   }
   out << "Projected Polygon" << std::endl;
   theProjectedPolyObject->print(out);
   return out;
}

void ossimGeoAnnotationPolyObject::draw(ossimRgbImage& anImage)const
{
   if(theProjectedPolyObject)
   {
      theProjectedPolyObject->draw(anImage);
   }
}

ossimAnnotationObject* ossimGeoAnnotationPolyObject::getNewClippedObject(
   const ossimDrect& rect)const
{
   if(theProjectedPolyObject)
   {
      return theProjectedPolyObject->getNewClippedObject(rect);
   }
   
   return (ossimAnnotationObject*)NULL;
}

bool ossimGeoAnnotationPolyObject::intersects(const ossimDrect& rect)const
{
   if(theProjectedPolyObject)
   {
      return theProjectedPolyObject->intersects(rect);
   }
   
   return false;
}

void ossimGeoAnnotationPolyObject::getBoundingRect(ossimDrect& rect)const
{
   rect.makeNan();
   if(theProjectedPolyObject)
   {
      theProjectedPolyObject->getBoundingRect(rect);
   }
}

void ossimGeoAnnotationPolyObject::computeBoundingRect()
{
   if(theProjectedPolyObject)
   {
      theProjectedPolyObject->computeBoundingRect();
   }
}

const std::vector<ossimGpt>& ossimGeoAnnotationPolyObject::getPolygon()const
{
   return thePolygon;
}

void ossimGeoAnnotationPolyObject::setPolygon(const std::vector<ossimGpt>& poly)
{
   thePolygon = poly;
   std::vector<ossimDpt> projectedPoints(thePolygon.size());
   if(theProjectedPolyObject)
   {
      theProjectedPolyObject->setPolygon(projectedPoints);
   }
   
}


void ossimGeoAnnotationPolyObject::setFillFlag(bool flag)
{
   if(theProjectedPolyObject)
   {
      theProjectedPolyObject->setFillFlag(flag);
   }
}

void ossimGeoAnnotationPolyObject::setColor(ossim_uint8 r,
                                            ossim_uint8 g,
                                            ossim_uint8 b)
{
   ossimAnnotationObject::setColor(r, g, b);
   if(theProjectedPolyObject)
   {
      theProjectedPolyObject->setColor(r, g, b);
   }
}

void ossimGeoAnnotationPolyObject::setThickness(ossim_uint8 thickness)
{
   ossimAnnotationObject::setThickness(thickness);
   if(theProjectedPolyObject)
   {
      theProjectedPolyObject->setThickness(thickness);
   }
}

bool ossimGeoAnnotationPolyObject::saveState(ossimKeywordlist& kwl,
                                             const char* prefix) const
{
   ossim_uint32 vIdx = 0;
   
   for(ossim_uint32 i = 0; i < thePolygon.size(); ++i)
   {
      ossimString key = "v";
      key += ossimString::toString(vIdx);

      kwl.add(prefix, key.c_str(), thePolygon[i].toString().c_str());

      ++vIdx;
   }
   
   return ossimGeoAnnotationObject::saveState(kwl, prefix);
}

bool ossimGeoAnnotationPolyObject::loadState(const ossimKeywordlist& kwl,
                                             const char* prefix)
{
   if (!theProjectedPolyObject)
   {
      return false;
   }
   
   //---
   // Base class state must be called first to pick up colors...
   //---
   bool status =  ossimGeoAnnotationObject::loadState(kwl, prefix);

   ossim_uint32 index = 0;
   ossimString copyPrefix = prefix;
   ossim_uint32 count = kwl.getNumberOfSubstringKeys(copyPrefix+"v[0-9]*");

   ossim_uint32 numberOfMatches = 0;
   const ossim_uint32 MAX_INDEX = count + 100;
   
   while(numberOfMatches < count)
   {
      ossimString key = "v";
      key += ossimString::toString(index);

      const char* lookup = kwl.find(prefix, key.c_str());
      if (lookup)
      {
         ++numberOfMatches;
         ossimGpt gpt;

         std::istringstream is(lookup);
         is >> gpt;
         thePolygon.push_back(gpt);
      }

      ++index;
      
      if (index > MAX_INDEX) // Avoid infinite loop...
      {
         break;
      }
   }

   // Set up the annotator.
   theProjectedPolyObject->setColor(theRed, theGreen, theBlue);
   theProjectedPolyObject->setThickness(theThickness);
   
   std::vector<ossimDpt> projectedPoints(thePolygon.size());
   theProjectedPolyObject->setPolygon(projectedPoints);
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimGeoAnnotationPolyObject::loadState DEBUG:" << std::endl;
      for (ossim_uint32 i=0; i<thePolygon.size(); ++i)
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "thePolygon[" << i << "]:  " << thePolygon[i] << std::endl;
      }
   }
   
   return status;
}
