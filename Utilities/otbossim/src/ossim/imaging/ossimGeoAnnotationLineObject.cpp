//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks)
// Description:
//
//*************************************************************************
// $Id: ossimGeoAnnotationLineObject.cpp 15766 2009-10-20 12:37:09Z gpotts $
#include <ossim/imaging/ossimGeoAnnotationLineObject.h>
#include <ossim/imaging/ossimAnnotationLineObject.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimImageProjectionModel.h>
#include <ossim/base/ossimException.h>


RTTI_DEF1(ossimGeoAnnotationLineObject,
          "ossimGeoAnnotationLineObject",
          ossimGeoAnnotationObject)
   
ossimGeoAnnotationLineObject::ossimGeoAnnotationLineObject(const ossimGpt& start,
                                                           const ossimGpt& end,
                                                           unsigned char r,
                                                           unsigned char g,
                                                           unsigned char b,
                                                           long thickness)
   :ossimGeoAnnotationObject(r, g, b, thickness),
    theProjectedLineObject(0),
    theStart(start),
    theEnd(end)
{
   theProjectedLineObject = new ossimAnnotationLineObject(ossimDpt(0,0),
                                                          ossimDpt(0,0),
                                                          r,
                                                          g,
                                                          b,
                                                          thickness);
}

ossimGeoAnnotationLineObject::ossimGeoAnnotationLineObject(const ossimGeoAnnotationLineObject& rhs)
   :ossimGeoAnnotationObject(rhs),
    theProjectedLineObject(rhs.theProjectedLineObject?(ossimAnnotationLineObject*)rhs.theProjectedLineObject->dup():(ossimAnnotationLineObject*)0),
    theStart(rhs.theStart),
    theEnd(rhs.theEnd)
{
   
}


ossimGeoAnnotationLineObject::~ossimGeoAnnotationLineObject()
{
   if(theProjectedLineObject)
   {
      delete theProjectedLineObject;
      theProjectedLineObject = 0;
   }
}

ossimObject* ossimGeoAnnotationLineObject::dup()const
{
   return new ossimGeoAnnotationLineObject(*this);
}

void ossimGeoAnnotationLineObject::applyScale(double x, double y)
{
   theStart.lond(theStart.lond()*x);
   theStart.latd(theStart.latd()*y);
   theEnd.lond(theEnd.lond()*x);
   theEnd.latd(theEnd.latd()*y);

   if(theProjectedLineObject)
   {
      theProjectedLineObject->applyScale(x, y);
   }
}

void ossimGeoAnnotationLineObject::transform(ossimImageGeometry* projection)
{
   if(projection)
   {
      ossimDpt projectedStart;
      ossimDpt projectedEnd;
      
      projection->worldToLocal(theStart, projectedStart);
      projection->worldToLocal(theEnd,   projectedEnd);

      theProjectedLineObject->setLine(projectedStart, projectedEnd);
   }
}

std::ostream& ossimGeoAnnotationLineObject::print(std::ostream& out)const
{
   out << "start ground:     " << theStart << endl;
   out << "end ground:       " << theEnd   << endl;
   return out;
}

void ossimGeoAnnotationLineObject::draw(ossimRgbImage& anImage)const
{
   theProjectedLineObject->draw(anImage);
}

bool ossimGeoAnnotationLineObject::intersects(const ossimDrect& rect)const
{
   if(theProjectedLineObject)
   {
      return theProjectedLineObject->intersects(rect);
   }

   return false;
}

ossimAnnotationObject* ossimGeoAnnotationLineObject::getNewClippedObject(const ossimDrect& rect)const
{
   if(intersects(rect))
   {
      if(theProjectedLineObject)
      {
         return theProjectedLineObject->getNewClippedObject(rect);
      }
   }
   
   return (ossimAnnotationObject*)0;
}

void ossimGeoAnnotationLineObject::getBoundingRect(ossimDrect& rect)const
{
   theProjectedLineObject->getBoundingRect(rect);
}

void ossimGeoAnnotationLineObject::computeBoundingRect()
{
   theProjectedLineObject->computeBoundingRect();
}
