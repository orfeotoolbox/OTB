//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimGeoAnnotationMultiPolyObject.cpp 10464 2007-02-08 19:27:58Z dburken $

#include <ossim/imaging/ossimGeoAnnotationMultiPolyObject.h>
#include <ossim/imaging/ossimAnnotationMultiPolyObject.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/base/ossimPolyArea2d.h>

RTTI_DEF1(ossimGeoAnnotationMultiPolyObject, "ossimGeoAnnotationMultiPolyObject", ossimGeoAnnotationObject);

ossimGeoAnnotationMultiPolyObject::ossimGeoAnnotationMultiPolyObject()
   :
      ossimGeoAnnotationObject(),
      theMultiPolygon(),
      theBoundingRect(),
      theFillEnabled(false),
      theProjectedPolyObject(NULL)
{
   allocateProjectedPolygon();   
   theBoundingRect.makeNan();
}

ossimGeoAnnotationMultiPolyObject::ossimGeoAnnotationMultiPolyObject(const vector<ossimGeoPolygon>& multiPoly,
                                                                     bool enableFill,
                                                                     unsigned char r,
                                                                     unsigned char g,
                                                                     unsigned char b,
                                                                     long thickness)
   :
      ossimGeoAnnotationObject(r, g, b, thickness),
      theMultiPolygon(multiPoly),
      theBoundingRect(),
      theFillEnabled(enableFill),
      theProjectedPolyObject(NULL)
{
   allocateProjectedPolygon();   
   theBoundingRect.makeNan();
}

ossimGeoAnnotationMultiPolyObject::ossimGeoAnnotationMultiPolyObject(const ossimGeoAnnotationMultiPolyObject& rhs)
   :
      ossimGeoAnnotationObject(rhs),
      theMultiPolygon(rhs.theMultiPolygon),
      theBoundingRect(rhs.theBoundingRect),
      theFillEnabled(rhs.theFillEnabled),
      theProjectedPolyObject(rhs.theProjectedPolyObject?(ossimAnnotationMultiPolyObject*)rhs.theProjectedPolyObject->dup():NULL)
{
}

ossimGeoAnnotationMultiPolyObject::~ossimGeoAnnotationMultiPolyObject()
{
   if(theProjectedPolyObject)
   {
      delete theProjectedPolyObject;
      theProjectedPolyObject = NULL;
   }
}

void ossimGeoAnnotationMultiPolyObject::applyScale(double x,
                                              double y)
{
   ossimNotify(ossimNotifyLevel_NOTICE)
      << "ossimGeoAnnotationPolyObject::applyScale NOT IMPLEMENTED!!!!"
      << endl;
}

void ossimGeoAnnotationMultiPolyObject::transform(ossimProjection* projection)
{
   if(!projection)
   {
      return;
   }
   
   allocateProjectedPolygon();   
   int polyI  = 0;
   int pointI = 0;

   ossimDpt temp;
   std::vector<ossimPolygon> visiblePolygons;
   ossimPolygon polygon;
   for(polyI = 0; polyI < (int)theMultiPolygon.size(); ++polyI)
   {
      polygon.clear();
      for(pointI = 0; pointI < (int)theMultiPolygon[polyI].size(); ++pointI)
      {
         projection->worldToLineSample(theMultiPolygon[polyI][pointI],
                                       temp);
         if(!temp.hasNans())
         {
            polygon.addPoint(temp);
         }
      }
      theProjectedPolyObject->addPolygon(polygon);
   }

   //---
   // Update the bounding rect.
   //---
   theProjectedPolyObject->computeBoundingRect();
}

std::ostream& ossimGeoAnnotationMultiPolyObject::print(std::ostream& out)const
{
   ossimNotify(ossimNotifyLevel_NOTICE)
      << "ossimGeoAnnotationMultiPolyObject::print NOT IMPLEMENTED"
      << endl;
   return out;
}

bool ossimGeoAnnotationMultiPolyObject::intersects(const ossimDrect& rect)const
{
   ossimNotify(ossimNotifyLevel_NOTICE)
      << "ossimGeoAnnotationMultiPolyObject::intersects NOT IMPLEMENTED"
      << endl;
  return false;
}

ossimAnnotationObject* ossimGeoAnnotationMultiPolyObject::getNewClippedObject(const ossimDrect& rect)const
{
   ossimNotify(ossimNotifyLevel_NOTICE)
      << "ossimGeoAnnotationMultiPolyObject::getNewClippedObject "
      << "NOT IMPLEMENTED" << endl;
   
  return (ossimAnnotationObject*)(dup());
}

void ossimGeoAnnotationMultiPolyObject::draw(ossimRgbImage& anImage)const
{
   if(theProjectedPolyObject)
   {
      theProjectedPolyObject->draw(anImage);
   }
}

void ossimGeoAnnotationMultiPolyObject::getBoundingRect(ossimDrect& rect)const
{
   rect = theBoundingRect;
}
 
void ossimGeoAnnotationMultiPolyObject::computeBoundingRect()
{
   theBoundingRect.makeNan();
   if(theProjectedPolyObject)
   {
      theProjectedPolyObject->computeBoundingRect();
      theProjectedPolyObject->getBoundingRect(theBoundingRect);
   }
}

bool ossimGeoAnnotationMultiPolyObject::isPointWithin(const ossimDpt& imagePoint)const
{
   if(theProjectedPolyObject)
   {
      return theProjectedPolyObject->isPointWithin(imagePoint);
   }

   return false;
}

void ossimGeoAnnotationMultiPolyObject::setFillFlag(bool flag)
{
   theFillEnabled = flag;
   if(theProjectedPolyObject)
   {
      theProjectedPolyObject->setFillFlag(flag);
   }
}

void ossimGeoAnnotationMultiPolyObject::allocateProjectedPolygon()
{
   if(theProjectedPolyObject)
   {
      delete theProjectedPolyObject;
      theProjectedPolyObject = NULL;
   }

   vector<ossimPolygon> polyList;
   
   if(theMultiPolygon.size())
   {
      theProjectedPolyObject = new ossimAnnotationMultiPolyObject(polyList,
                                                                  theFillEnabled,
                                                                  theRed,
                                                                  theGreen,
                                                                  theBlue,
                                                                  theThickness);
   }
}
