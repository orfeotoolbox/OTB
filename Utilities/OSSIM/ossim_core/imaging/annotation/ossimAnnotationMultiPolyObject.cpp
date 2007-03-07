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
//
//*************************************************************************
// $Id: ossimAnnotationMultiPolyObject.cpp,v 1.18 2004/12/21 20:12:40 gpotts Exp $
#include "ossimAnnotationMultiPolyObject.h"
#include "ossimAnnotationMultiLineObject.h"
#include "imaging/ossimRgbImage.h"
#include "base/data_types/ossimIrect.h"
#include "base/data_types/ossimDrect.h"
#include "base/data_types/ossimLine.h"

RTTI_DEF1(ossimAnnotationMultiPolyObject, "ossimAnnotationMultiPolyObject", ossimAnnotationObject)

ossimAnnotationMultiPolyObject::ossimAnnotationMultiPolyObject()
   :ossimAnnotationObject(),
    theFillEnabled(false)
{
   computeBoundingRect();
}

ossimAnnotationMultiPolyObject::ossimAnnotationMultiPolyObject(const vector<ossimPolygon>& multiPoly,
                                                               bool enableFill,
                                                               unsigned char r,
                                                               unsigned char g,
                                                               unsigned char b,
                                                               long thickness)
   :ossimAnnotationObject(r, g, b, thickness),
    theFillEnabled(enableFill)
{
   theMultiPolygon = multiPoly;
   computeBoundingRect();
}

ossimAnnotationMultiPolyObject::~ossimAnnotationMultiPolyObject()
{
}

void ossimAnnotationMultiPolyObject::applyScale(double x,
                                                double y)
{
   for(ossim_uint32 i =0; i<theMultiPolygon.size(); ++i)
   {
      theMultiPolygon[i] *= ossimDpt(x, y);
   }
   
   theBoundingRect *= ossimDpt(x,y);
}

bool ossimAnnotationMultiPolyObject::intersects(const ossimDrect& rect)const
{
   // do the quick checks first
   //
   if(rect.hasNans()) return false;
   if(!rect.intersects(theBoundingRect)) return false;
   if(theMultiPolygon.size()<1) return false;
   
   for(ossim_uint32 i =0; i < theMultiPolygon.size(); ++i)
   {
      vector<ossimPolygon> result;
      
      if(theMultiPolygon[i].clipToRect(result, rect))
      {
         return true;
      }
   }

   return false;
}

ossimAnnotationObject* ossimAnnotationMultiPolyObject::getNewClippedObject(const ossimDrect& rect)const
{
   ossimAnnotationObject* result = (ossimAnnotationObject*)NULL;

   if(theBoundingRect.intersects(rect))
   {
      ossimDrect clipRect(rect.ul().x - 10,
                          rect.ul().y - 10,
                          rect.lr().x + 10,
                          rect.lr().y + 10);
      
      if(theFillEnabled)
      {
         vector<ossimPolygon> polyListResult;
         
         for(ossim_uint32 i = 0; i < theMultiPolygon.size(); ++i)
         {
            vector<ossimPolygon> tempList;
            
            if(theMultiPolygon[i].clipToRect(tempList, rect))
            {
               polyListResult.insert(polyListResult.end(),
                                     tempList.begin(),
                                     tempList.end());
            }
         }
         
         if(polyListResult.size())
         {
            result = new ossimAnnotationMultiPolyObject(polyListResult,
                                                        theFillEnabled,
                                                        theRed,
                                                        theGreen,
                                                        theBlue,
                                                        theThickness);
         }
      }
      else
      {
         vector<ossimPolyLine> lineListResult;
         vector<ossimPolyLine> tempResult;

         for(ossim_uint32 i = 0; i< theMultiPolygon.size();++i)
         {
            ossimPolyLine polyLine = theMultiPolygon[i];

            if(polyLine.clipToRect(tempResult,
                                   clipRect))
            {
               lineListResult.insert(lineListResult.end(),
                                     tempResult.begin(),
                                     tempResult.end());
            }
         }
         
         if(lineListResult.size())
         {
            result = new ossimAnnotationMultiLineObject(lineListResult,
                                                        theRed,
                                                        theGreen,
                                                        theBlue,
                                                        theThickness);
         }
      }
   }
   
   return result;
}

void ossimAnnotationMultiPolyObject::draw(ossimRgbImage& anImage)const
{
   if(theMultiPolygon.size()<1) return;
   if(theBoundingRect.hasNans()) return;
   
   
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

      int j = 0;
      for(ossim_uint32 i = 0; i < theMultiPolygon.size(); ++i)
      {
         const ossimPolygon& poly = theMultiPolygon[i];
         int vertexCount          = poly.getVertexCount();
         if(!theFillEnabled)
         {
            if(poly.getNumberOfVertices() == 1)
            {
               ossimDpt start, end;
               start = poly[0];
               end   = poly[0];
               if(clipRect.clip(start, end))
               {
                  anImage.drawLine(ossimIpt(start),
                                   ossimIpt(end));
               }
            }
            else if(poly.getNumberOfVertices() == 2)
            {
               ossimDpt start, end;
               start = poly[0];
               end   = poly[1];
               if(clipRect.clip(start, end))
               {
                  anImage.drawLine(ossimIpt(start),
                                   ossimIpt(end));
               }
            }
            else if(vertexCount > 2)
            {
               ossimDpt start, end;
               start = poly[vertexCount-1];
               end   = poly[0];
               j = 0;
               do
               {
                  if(clipRect.clip(start, end))
                  {
                     anImage.drawLine(ossimIpt(start),
                                      ossimIpt(end));
                  }
                  ++j;
                  start = poly[j-1];
                  end   = poly[j];
               }while(j < vertexCount);
            }
         }
         else
         {
            vector<ossimPolygon> result;
            if(poly.clipToRect(result, imageRect))
            {
               for(j = 0; j < (int)result.size();++j)
               {
                  anImage.drawFilledPolygon(result[j].getVertexList());
               }
            }
         }
      }
   }
}

std::ostream& ossimAnnotationMultiPolyObject::print(std::ostream& out)const
{
   ossimAnnotationObject::print(out);
   out << endl;
   out << setw(15)<<setiosflags(ios::left)<<"type:"<<getClassName() << endl
       << setw(15)<<setiosflags(ios::left)<<"polygons:"<<theMultiPolygon.size()<<endl
       << setw(15)<<setiosflags(ios::left)<<"fill enable: " << theFillEnabled << endl;
   
   for(ossim_uint32 i = 0; i < theMultiPolygon.size(); ++i)
   {
      out << "____________________________________________________"<<endl
          << theMultiPolygon[i] << endl;
   }
   out << "____________________________________________________"<<endl;
   return out;
}

void ossimAnnotationMultiPolyObject::getBoundingRect(ossimDrect& rect)const
{   
   rect = theBoundingRect;
}

void ossimAnnotationMultiPolyObject::clear()
{
   theMultiPolygon.clear();
}

void ossimAnnotationMultiPolyObject::computeBoundingRect()
{
   if(theMultiPolygon.size())
   {
      ossimDrect rect(theMultiPolygon[0]);
      
      for(ossim_uint32 i = 1; i < theMultiPolygon.size(); ++i)
      {
         ossimDrect rect2(theMultiPolygon[i]);

         if(rect.hasNans())
         {
            rect = rect2;
         }
         else
         {
            if(!rect2.hasNans())
            {
               rect = rect.combine(rect2);
            }
         }
      }
      
      theBoundingRect = rect;
   }
   else
   {
      theBoundingRect.makeNan();
   }
   if(!theBoundingRect.hasNans())
   {
      ossimIpt origin = theBoundingRect.ul();
      theBoundingRect = ossimDrect(origin.x - theThickness/2,
                                   origin.y - theThickness/2,
                                   origin.x + (theBoundingRect.width()-1) + theThickness/2,
                                   origin.y + (theBoundingRect.height()-1) + theThickness/2);
   }
}

bool ossimAnnotationMultiPolyObject::isPointWithin(const ossimDpt& imagePoint)const
{
   for(ossim_uint32 i = 0; i < theMultiPolygon.size(); ++i)
   {
      if(theMultiPolygon[i].isPointWithin(imagePoint))
      {
         return true;
      }
   }

   return false;
}
