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
// Author: Garrett Potts (gpotts@imagelinks)
// Description:
//
//*************************************************************************
// $Id: ossimGeoAnnotationPolyLineObject.cpp,v 1.3 2005/05/27 12:17:43 gpotts Exp $
#include "ossimGeoAnnotationPolyLineObject.h"
#include "ossimAnnotationMultiLineObject.h"
#include "projections/ossimProjection.h"

RTTI_DEF1(ossimGeoAnnotationPolyLineObject, "ossimGeoAnnotationPolyLineObject", ossimGeoAnnotationObject)
   
ossimGeoAnnotationPolyLineObject::ossimGeoAnnotationPolyLineObject(const vector<ossimGpt>& groundPts,
                                                           unsigned char r,
                                                           unsigned char g,
                                                           unsigned char b,
                                                           long thickness)
   :ossimGeoAnnotationObject(r, g, b, thickness),
    theProjectedMultiLineObject(NULL)
{

   thePolygon = groundPts;

   // we will initialize the projected polygon's size
   // so we don't have to reset it every time we do a
   // projection
   //
   vector<ossimDpt> projectedPoints(thePolygon.size());
   
   theProjectedMultiLineObject = new ossimAnnotationMultiLineObject(ossimPolyLine(projectedPoints),
                                                                    r,
                                                                    g,
                                                                    b,
                                                                    thickness);
}

ossimGeoAnnotationPolyLineObject::ossimGeoAnnotationPolyLineObject(const ossimGeoAnnotationPolyLineObject& rhs)
   :ossimGeoAnnotationObject(rhs),
    thePolygon(rhs.thePolygon),
    theProjectedMultiLineObject(rhs.theProjectedMultiLineObject?(ossimAnnotationMultiLineObject*)rhs.theProjectedMultiLineObject->dup():(ossimAnnotationMultiLineObject*)NULL)
{
}

ossimGeoAnnotationPolyLineObject::~ossimGeoAnnotationPolyLineObject()
{
   if(theProjectedMultiLineObject)
   {
      delete theProjectedMultiLineObject;
      theProjectedMultiLineObject = NULL;
   }
}

void ossimGeoAnnotationPolyLineObject::applyScale(double x,
                                                   double y)
{
   for(int i = 0; i < (int)thePolygon.size(); ++i)
   {
      thePolygon[i].lond(thePolygon[i].lond()*x);
      thePolygon[i].latd(thePolygon[i].latd()*y);
   }
   if(theProjectedMultiLineObject)
   {
      theProjectedMultiLineObject->applyScale(x, y);
   }
      
}

void ossimGeoAnnotationPolyLineObject::transform(ossimProjection* projection)
{
   // make sure it's not null
   if(!projection)
   {
      return;
   }
   vector<ossimPolyLine>& polyList =theProjectedMultiLineObject->getPolyLineList();

   if(polyList.size())
   {
      vector<ossimDpt>& poly = polyList[0].getVertexList();
      
      long upperBound = thePolygon.size();
      
      for(long index=0; index < upperBound; ++index)
      {
         projection->worldToLineSample(thePolygon[index], poly[index]);
      }
      
      // update the bounding rect
      //
      theProjectedMultiLineObject->computeBoundingRect();
   }
}

std::ostream& ossimGeoAnnotationPolyLineObject::print(std::ostream& out)const
{
   out << "number_of_points:  " << thePolygon.size() << endl;
   if(thePolygon.size() > 0)
   {
      for(long index =0; index < (long)(thePolygon.size()-1); ++index)
      {
         out << thePolygon[index] << endl;
      }
      out << thePolygon[thePolygon.size()-1] << endl;
   }
   out << "Projected Polygon" << endl;
   theProjectedMultiLineObject->print(out);
   return out;
}

void ossimGeoAnnotationPolyLineObject::draw(ossimRgbImage& anImage)const
{
   theProjectedMultiLineObject->draw(anImage);
}

bool ossimGeoAnnotationPolyLineObject::intersects(const ossimDrect& rect)const
{
   if(theProjectedMultiLineObject)
   {
      return theProjectedMultiLineObject->intersects(rect);
   }
   
   return false;
}

ossimAnnotationObject* ossimGeoAnnotationPolyLineObject::getNewClippedObject(const ossimDrect& rect)const
{
   if(theProjectedMultiLineObject)
   {
      return theProjectedMultiLineObject->getNewClippedObject(rect);
   }
   
   return (ossimAnnotationObject*)NULL;;
}

void ossimGeoAnnotationPolyLineObject::getBoundingRect(ossimDrect& rect)const
{
   theProjectedMultiLineObject->getBoundingRect(rect);
}

void ossimGeoAnnotationPolyLineObject::computeBoundingRect()
{
   theProjectedMultiLineObject->computeBoundingRect();
}

void ossimGeoAnnotationPolyLineObject::setColor(unsigned char r,
                                                unsigned char g,
                                                unsigned char b)
{
   if(theProjectedMultiLineObject)
   {
      theProjectedMultiLineObject->setColor(r, g, b);
   }
}

void ossimGeoAnnotationPolyLineObject::setThickness(long thickness)
{
   if(theProjectedMultiLineObject)
   {
      theProjectedMultiLineObject->setThickness(thickness);
   }
}
