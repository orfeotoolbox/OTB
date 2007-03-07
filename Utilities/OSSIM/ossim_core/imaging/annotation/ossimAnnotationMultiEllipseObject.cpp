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
// $Id: ossimAnnotationMultiEllipseObject.cpp,v 1.5 2004/05/23 17:49:37 dburken Exp $

#include <imaging/annotation/ossimAnnotationMultiEllipseObject.h>

ossimAnnotationMultiEllipseObject::ossimAnnotationMultiEllipseObject
   (const std::vector<ossimDpt>& pointList,
    const ossimDpt& widthHeight,
    bool enableFill,
    unsigned char r,
    unsigned char g,
    unsigned char b,
    long thickness)
   :
         ossimAnnotationObject(r, g, b, thickness),
         thePointList(pointList),
         theWidthHeight(widthHeight),
         theFillFlag(enableFill)
{
   computeBoundingRect();
}

ossimAnnotationMultiEllipseObject::ossimAnnotationMultiEllipseObject
   (const ossimDpt& widthHeight,
    bool enableFill,
    unsigned char r,
    unsigned char g,
    unsigned char b,
    long thickness)
   :
         ossimAnnotationObject(r, g, b, thickness),
         theWidthHeight(widthHeight),
         theFillFlag(enableFill)
{
   theBoundingRect.makeNan();
}

ossimAnnotationMultiEllipseObject::ossimAnnotationMultiEllipseObject(const ossimAnnotationMultiEllipseObject& rhs)
      :thePointList(rhs.thePointList),
       theWidthHeight(rhs.theWidthHeight),
       theFillFlag(rhs.theFillFlag),
       theBoundingRect(rhs.theBoundingRect)
{
}

ossimObject* ossimAnnotationMultiEllipseObject::dup()const
{
   return new ossimAnnotationMultiEllipseObject(*this);
}

ossimAnnotationMultiEllipseObject::~ossimAnnotationMultiEllipseObject()
{}

ossimAnnotationObject* ossimAnnotationMultiEllipseObject::getNewClippedObject(const ossimDrect& rect)const
{
   ossimNotify(ossimNotifyLevel_WARN)
      << "ossimAnnotationMultiEllipseObject::getNewClippedObject WARNING: Not implemented"
      << std::endl;
   return NULL;
}

void ossimAnnotationMultiEllipseObject::applyScale(double x, double y)
{
   int i;
   int upper = (int)thePointList.size();
   for(i = 0; i < upper; ++i)
   {
      thePointList[i].x *= x;
      thePointList[i].y *= y;
   }
   theWidthHeight.x *= x;
   theWidthHeight.y *= y;
   
   computeBoundingRect();
}

void ossimAnnotationMultiEllipseObject::draw(ossimRgbImage& anImage)const
{
   anImage.setDrawColor(theRed, theGreen, theBlue);
   anImage.setThickness(theThickness);
   ossimDrect imageRect = anImage.getImageData()->getImageRectangle();

   if(theBoundingRect.intersects(imageRect))
   {
      int i;
      int upper = (int)thePointList.size();
      if(theFillFlag)
      {
         for(i = 0; i < upper; ++i)
         {
            ossimDpt tempPoint = thePointList[i];
            
            ossimDrect tempRect(ossimDpt(tempPoint.x - theWidthHeight.x,
                                         tempPoint.y - theWidthHeight.y),
                                ossimDpt(tempPoint.x + theWidthHeight.x,
                                         tempPoint.y + theWidthHeight.y));
            if(tempRect.intersects(imageRect))
            {
               anImage.drawFilledArc(irint(tempPoint.x),
                                     irint(tempPoint.y),
                                     irint(theWidthHeight.x),
                                     irint(theWidthHeight.y),
                                     0,
                                     360);
            }
         }
      }
      else
      {
         for(i = 0; i < upper; ++i)
         {
            ossimDpt tempPoint = thePointList[i];
            
            ossimDrect tempRect(ossimDpt(tempPoint.x - theWidthHeight.x,
                                         tempPoint.y - theWidthHeight.y),
                                ossimDpt(tempPoint.x + theWidthHeight.x,
                                         tempPoint.y + theWidthHeight.y));
            if(tempRect.intersects(imageRect))
            {
               anImage.drawArc(irint(tempPoint.x),
                               irint(tempPoint.y),
                               irint(theWidthHeight.x),
                               irint(theWidthHeight.y),
                               0,
                               360);
            }
         }
      }
   }
}

bool ossimAnnotationMultiEllipseObject::intersects(const ossimDrect& rect)const
{
   return rect.intersects(theBoundingRect);
}

std::ostream& ossimAnnotationMultiEllipseObject::print(std::ostream& out)const
{
   ossimNotify(ossimNotifyLevel_NOTICE)
      << "ossimAnnotionMultiEllipseObject::print NOT IMPLEMENTED"
      << endl;
   return out;
}

void ossimAnnotationMultiEllipseObject::getBoundingRect(ossimDrect& rect)const
{
   rect =  theBoundingRect;
}

void ossimAnnotationMultiEllipseObject::computeBoundingRect()
{
   theBoundingRect.makeNan();

   int i;
   int upper = (int)thePointList.size();
   for(i = 0; i < upper; ++i)
   {
      ossimDpt tempPoint = thePointList[i];
      if(!tempPoint.hasNans())
      {
         ossimDrect tempRect(ossimDpt(tempPoint.x - theWidthHeight.x,
                                      tempPoint.y - theWidthHeight.y),
                             ossimDpt(tempPoint.x + theWidthHeight.x,
                                      tempPoint.y + theWidthHeight.y));
         if(!theBoundingRect.hasNans())
         {
            theBoundingRect.combine(tempRect);
         }
         else
         {
            theBoundingRect = tempRect;
         }
      }
   }
}

bool ossimAnnotationMultiEllipseObject::isPointWithin(const ossimDpt& imagePoint)const
{
   if(!theBoundingRect.hasNans())
   {
      return theBoundingRect.pointWithin(imagePoint);
   }
   return false;
}

void ossimAnnotationMultiEllipseObject::setFillFlag(bool flag)
{
   theFillFlag = flag;
}

void ossimAnnotationMultiEllipseObject::resize(ossim_uint32 size)
{
   if(size)
   {
      thePointList.resize(size);
   }
   else
   {
      thePointList.clear();
   }
}

void ossimAnnotationMultiEllipseObject::setWidthHeight(const ossimDpt& widthHeight)
{
   theWidthHeight = widthHeight;
   computeBoundingRect();
}

ossimDpt& ossimAnnotationMultiEllipseObject::operator[](int i)
{
   return thePointList[i];
}

const ossimDpt& ossimAnnotationMultiEllipseObject::operator[](int i)const
{
   return thePointList[i];
}
