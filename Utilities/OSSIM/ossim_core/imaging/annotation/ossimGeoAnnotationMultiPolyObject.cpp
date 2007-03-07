//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimGeoAnnotationMultiPolyObject.cpp,v 1.11 2004/12/21 20:12:40 gpotts Exp $

#include <imaging/annotation/ossimGeoAnnotationMultiPolyObject.h>
#include <imaging/annotation/ossimAnnotationMultiPolyObject.h>
#include <projections/ossimProjection.h>
#include <base/data_types/ossimPolyArea2d.h>

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
   if(!projection) return;
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
