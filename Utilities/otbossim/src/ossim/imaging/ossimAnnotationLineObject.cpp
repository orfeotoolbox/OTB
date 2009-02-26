//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimAnnotationLineObject.cpp 13964 2009-01-14 16:30:07Z gpotts $

#include <ossim/imaging/ossimAnnotationLineObject.h>
#include <ossim/imaging/ossimRgbImage.h>

RTTI_DEF1(ossimAnnotationLineObject,
          "ossimAnnotationLineObject",
          ossimAnnotationObject)

ossimAnnotationLineObject::ossimAnnotationLineObject()
   :ossimAnnotationObject(),
    theStart(0,0),
    theEnd(0,0),
    theBoundingRect(0,0,0,0)
{
}

ossimAnnotationLineObject::ossimAnnotationLineObject(const ossimIpt& start,
                                                     const ossimIpt& end,
                                                     unsigned char r,
                                                     unsigned char g,
                                                     unsigned char b,
                                                     long thickness)
   :ossimAnnotationObject(r, g, b, thickness),
    theStart(start),
    theEnd(end)
{
   computeBoundingRect();
}

ossimAnnotationLineObject::ossimAnnotationLineObject(long x1,
                                                     long y1,
                                                     long x2,
                                                     long y2,
                                                     unsigned char r,
                                                     unsigned char g,
                                                     unsigned char b,
                                                     long thickness)
   :ossimAnnotationObject(r, g, b, thickness),
    theStart(x1, y1),
    theEnd(x2, y2)
{
   computeBoundingRect();
}

ossimAnnotationLineObject::ossimAnnotationLineObject(const ossimAnnotationLineObject& rhs)
      :ossimAnnotationObject(rhs),
       theStart(rhs.theStart),
       theEnd(rhs.theEnd),
       theBoundingRect(rhs.theBoundingRect)
{
}

ossimObject* ossimAnnotationLineObject::dup()const
{
   return new ossimAnnotationLineObject(*this);
}

void ossimAnnotationLineObject::applyScale(double x, double y)
{
   theStart.x = ossim::round<int>(theStart.x*x);
   theStart.y = ossim::round<int>(theStart.y*y);
   theEnd.x   = ossim::round<int>(theEnd.x*x);
   theEnd.y   = ossim::round<int>(theEnd.y*y);
   computeBoundingRect();
}

ossimAnnotationObject* ossimAnnotationLineObject::getNewClippedObject(const ossimDrect& rect)const
{
   ossimAnnotationLineObject* result = (ossimAnnotationLineObject*)NULL;
   ossimDpt start = theStart;
   ossimDpt end   = theEnd;
   
   if(rect.clip(start, end))
   {
      result = (ossimAnnotationLineObject*)dup();
      result->setLine(start, end);
   }
   
   return result;
}

bool ossimAnnotationLineObject::intersects(const ossimDrect& rect)const
{
   ossimDpt start = theStart;
   ossimDpt end   = theEnd;
   
   return rect.clip(start, end);
}

void ossimAnnotationLineObject::draw(ossimRgbImage& anImage)const
{
   if(anImage.getImageData().valid())
   {
      anImage.setDrawColor(theRed, theGreen, theBlue);
      anImage.setThickness(theThickness);
      ossimDrect imageRect = anImage.getImageData()->getImageRectangle();
      ossimDpt start = theStart;
      ossimDpt end   = theEnd;
      
      if(anImage.getImageData().valid())
      {
         // we need to extend it by a couple of pixels since
         // if a pixel lies on the edge and then another pixel is just off
         // the edge we will get a stair step and so for several pixels
         // the line might be inside the image rectangle but the clip
         // algorithm will only draw 1 pixel since it thinks the first
         // point is inside and the second point is outside and will
         // execute the clip algorithm keeping only the first
         // point.
         ossimDrect clipRect(imageRect.ul().x - 10,
                             imageRect.ul().y - 10,
                             imageRect.lr().x + 10,
                             imageRect.lr().y + 10);
         // now we can draw.
         if(clipRect.clip(start, end))
         {
            anImage.drawLine(ossimIpt((int)start.x,
                                      (int)start.y),
                             ossimIpt((int)end.x,
                                      (int)end.y));
         }
      }
   }
}

std::ostream& ossimAnnotationLineObject::print(std::ostream& out)const
{
   out << "line_start:    " << theStart << endl
       << "line_end:      " << theEnd   << endl
       << "bounding_rect: " << theBoundingRect << endl;
   return out;
}

void ossimAnnotationLineObject::getBoundingRect(ossimDrect& rect)const
{
   rect = theBoundingRect;
}

void ossimAnnotationLineObject::computeBoundingRect()
{
   vector<ossimDpt> pts(2);
      
   pts[0] = theStart;
   pts[1] = theEnd;

   theBoundingRect = ossimDrect(pts);
}

void ossimAnnotationLineObject::setLine(const ossimDpt& start,
                                        const ossimDpt& end)
{
   theStart = start;
   theEnd   = end;
   computeBoundingRect();
}
void ossimAnnotationLineObject::getLine(ossimDpt& start, ossimDpt& end)
{
   start = theStart;
   end   = theEnd;
}

bool ossimAnnotationLineObject::isPointWithin(const ossimDpt& point)const
{
  if(theBoundingRect.pointWithin(point))
  {
    // we will use the implicit form of a line to determine if the point is
    // on the line.  For now we will just return true if it is within the 
    // bounding rect.
    return true;
  }

  return false;
}
