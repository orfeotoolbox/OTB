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
// $Id: ossimAnnotationMultiLineObject.cpp 9094 2006-06-13 19:12:40Z dburken $
#include <ossim/imaging/ossimAnnotationMultiLineObject.h>

RTTI_DEF1(ossimAnnotationMultiLineObject,
          "ossimAnnotationMultiLineObject",
          ossimAnnotationObject);

void ossimAnnotationMultiLineObject::draw(ossimRgbImage& anImage)const
{
   if(anImage.getImageData().valid())
   {
      anImage.setDrawColor(theRed, theGreen, theBlue);
      anImage.setThickness(theThickness);
      ossimDrect imageRect = anImage.getImageData()->getImageRectangle();

      
      
      if(theBoundingRect.intersects(imageRect))
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
         
         for(ossim_uint32 i = 0; i < thePolyLineList.size(); ++i)
         {
            const vector<ossimDpt>& vList = thePolyLineList[i].getVertexList();
            
            if(vList.size() == 1)
            {
               anImage.drawLine(ossimIpt(vList[0]),
                                ossimIpt(vList[0]));
            }
            else
            {
               for(ossim_uint32 i2 = 0; i2 < (vList.size()-1); ++i2)
               {
                  ossimDpt start = vList[i2];
                  ossimDpt end   = vList[i2+1];
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
      }
   }
}

bool ossimAnnotationMultiLineObject::intersects(const ossimDrect& rect)const
{

   for(ossim_uint32 i = 0; i < thePolyLineList.size(); ++i)
   {
      if(thePolyLineList[i].isWithin(rect))
      {
         return true;
      }
   }
   return false;
}


void ossimAnnotationMultiLineObject::applyScale(double x,
                                                double y)
{
   for(ossim_uint32 i=0; i < thePolyLineList.size(); ++i)
   {
      vector<ossimDpt>& vList = thePolyLineList[i].getVertexList();
      
      for(ossim_uint32 i2 = 0; i2 < vList.size(); ++i2)
      {
         vList[i].x *= x;
         vList[i].y *= y;
         vList[i].x *= x;
         vList[i].y *= y;
      }
   }
   theBoundingRect *= ossimDpt(x, y);
}

ossimAnnotationObject* ossimAnnotationMultiLineObject::getNewClippedObject(const ossimDrect& rect)const
{
   ossimAnnotationMultiLineObject* result = (ossimAnnotationMultiLineObject*)NULL;

   if(intersects(rect))
   {
      vector<ossimPolyLine> lineList;
      vector<ossimPolyLine> tempResult;
      
      ossimDrect clipRect(rect.ul().x - 10,
                          rect.ul().y - 10,
                          rect.lr().x + 10,
                          rect.lr().y + 10);

      for(ossim_uint32 i =0; i< thePolyLineList.size();++i)
      {
         if(thePolyLineList[i].clipToRect(tempResult, clipRect))
         {
            lineList.insert(lineList.end(),
                            tempResult.begin(),
                            tempResult.end());
         }
      }
      
      if(lineList.size() > 0)
      {
         result = new ossimAnnotationMultiLineObject(lineList,
                                                     theRed,
                                                     theGreen,
                                                     theBlue,
                                                     theThickness);
      }
   }
   
   return result;
}

bool ossimAnnotationMultiLineObject::isPointWithin(const ossimDpt& imagePoint)const
{
   for(ossim_uint32 i=0; i < thePolyLineList.size(); ++i)
   {
      if(thePolyLineList[i].isPointWithin(imagePoint))
      {
         return true;
      }
      
   }

   return false;
}

void ossimAnnotationMultiLineObject::computeBoundingRect()
{
   theBoundingRect.makeNan();
   
   if(thePolyLineList.size() == 1)
   {
      theBoundingRect = thePolyLineList[0].getBoundingRect();
   }
   else if(thePolyLineList.size() > 1)
   {
      for(ossim_uint32 i = 0; i < thePolyLineList.size(); ++i)
      {
         ossimDrect rect = thePolyLineList[i].getBoundingRect();
         if(theBoundingRect.hasNans())
         {
            theBoundingRect = rect;
         }
         else if(!rect.hasNans())
         {
            theBoundingRect = theBoundingRect.combine(rect);
         }
      }
   }
}

std::ostream& ossimAnnotationMultiLineObject::print(std::ostream& out)const
{
   ossimAnnotationObject::print(out);

   for(ossim_uint32 i = 0; i < thePolyLineList.size();++i)
   {
      out << thePolyLineList[i] << endl
          <<"______________________________________"<<endl;
   }
   return out;
}
