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
// $Id: ossimGeoAnnotationMultiEllipseObject.cpp,v 1.7 2004/05/23 17:49:37 dburken Exp $

#include <imaging/annotation/ossimGeoAnnotationMultiEllipseObject.h>
#include <imaging/annotation/ossimAnnotationMultiEllipseObject.h>
#include <projections/ossimProjection.h>

ossimGeoAnnotationMultiEllipseObject::ossimGeoAnnotationMultiEllipseObject()
   :ossimGeoAnnotationObject(),
    theProjectedObject(new ossimAnnotationMultiEllipseObject()),
    theWidthHeight(1,1),
    theFillFlag(false)
{
   theProjectedObject->setFillFlag(false);
}

ossimGeoAnnotationMultiEllipseObject::ossimGeoAnnotationMultiEllipseObject(const std::vector<ossimGpt>& pointList,
                                                                           const ossimDpt& widthHeight,
                                                                           bool enableFill,
                                                                           unsigned char r,
                                                                           unsigned char g,
                                                                           unsigned char b,
                                                                           long thickness)
   :ossimGeoAnnotationObject(r, g, b, thickness),
    theProjectedObject(new ossimAnnotationMultiEllipseObject(widthHeight,enableFill, r, g, b, thickness )),
    thePointList(pointList),
    theWidthHeight(widthHeight),
    theFillFlag(enableFill)
{
   
}

ossimGeoAnnotationMultiEllipseObject::ossimGeoAnnotationMultiEllipseObject(const ossimDpt& widthHeight,
                                                                           bool enableFill,
                                                                           unsigned char r,
                                                                           unsigned char g,
                                                                           unsigned char b,
                                                                           long thickness)
   :ossimGeoAnnotationObject(r, g, b,thickness),
    theProjectedObject(new ossimAnnotationMultiEllipseObject(widthHeight,enableFill, r, g, b, thickness )),
    theWidthHeight(widthHeight),
    theFillFlag(enableFill)
{
}

ossimGeoAnnotationMultiEllipseObject::ossimGeoAnnotationMultiEllipseObject(const ossimGeoAnnotationMultiEllipseObject& rhs)
   :ossimGeoAnnotationObject(rhs),
    theProjectedObject((ossimAnnotationMultiEllipseObject*)rhs.theProjectedObject->dup()),
    thePointList(rhs.thePointList),
    theWidthHeight(rhs.theWidthHeight),
    theFillFlag(rhs.theFillFlag)
{
}
   
void ossimGeoAnnotationMultiEllipseObject::transform(ossimProjection* projection)
{
   theProjectedObject->resize(thePointList.size());
   int i = 0;
   int bounds = (int)thePointList.size();

   for(i = 0; i < bounds; ++i)
   {
      projection->worldToLineSample(thePointList[i], (*theProjectedObject)[i]);
      
   }
   computeBoundingRect();
}

void ossimGeoAnnotationMultiEllipseObject::setFillFlag(bool fillFlag)
{
   theProjectedObject->setFillFlag(fillFlag);
   theFillFlag = fillFlag;
}

void ossimGeoAnnotationMultiEllipseObject::setColor(unsigned char r,
                                                    unsigned char g,
                                                    unsigned char b)
{
   ossimAnnotationObject::setColor(r,g,b);
   if(theProjectedObject)
   {
      theProjectedObject->setColor(r,g,b);
   }
}

void ossimGeoAnnotationMultiEllipseObject::setThickness(long thickness)
{
   ossimAnnotationObject::setThickness(thickness);
   if(theProjectedObject)
   {
      theProjectedObject->setThickness(thickness);
   }
}


void ossimGeoAnnotationMultiEllipseObject::applyScale(double x,
                                                      double y)
{
   ossimNotify(ossimNotifyLevel_NOTICE)
      << "ossimGeoAnnotationMultiEllipseObject::applyScale NOT IMPLEMENTED"
      << endl;
}

void ossimGeoAnnotationMultiEllipseObject::draw(ossimRgbImage& anImage)const
{
   theProjectedObject->draw(anImage); 
}

bool ossimGeoAnnotationMultiEllipseObject::intersects(const ossimDrect& rect)const
{
   return theProjectedObject->intersects(rect);
}

void ossimGeoAnnotationMultiEllipseObject::setWidthHeight(const ossimDpt& widthHeight)
{
   theWidthHeight = widthHeight;
   theProjectedObject->setWidthHeight(widthHeight);
}

std::ostream& ossimGeoAnnotationMultiEllipseObject::print(std::ostream& out)const
{
   theProjectedObject->print(out);
   return out;
}

void ossimGeoAnnotationMultiEllipseObject::getBoundingRect(ossimDrect& rect)const
{
   theProjectedObject->getBoundingRect(rect);
}

void ossimGeoAnnotationMultiEllipseObject::computeBoundingRect()
{
   theProjectedObject->computeBoundingRect();
}

ossimObject* ossimGeoAnnotationMultiEllipseObject::dup()const
{
   return new ossimGeoAnnotationMultiEllipseObject(*this);
}

ossimAnnotationObject* ossimGeoAnnotationMultiEllipseObject::getNewClippedObject(const ossimDrect& rect)const
{
   ossimNotify(ossimNotifyLevel_NOTICE)
      << "ossimGeoAnnotationMultiEllipseObject::getNewClippedObject "
      << "NOT IMPLEMENTED" << endl;
   return (ossimAnnotationObject*)dup();
}

void ossimGeoAnnotationMultiEllipseObject::addPoint(const ossimGpt& point)
{
   thePointList.push_back(point);
}

void ossimGeoAnnotationMultiEllipseObject::setPoint(int i,
                                                    const ossimGpt& point)
{
   thePointList[i] = point;
}

void ossimGeoAnnotationMultiEllipseObject::resize(ossim_uint32 newSize)
{
   if(newSize)
   {
      thePointList.resize(newSize);
   }
}
