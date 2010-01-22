//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimAnnotationPolyObject.cpp 13964 2009-01-14 16:30:07Z gpotts $
#include <ossim/imaging/ossimAnnotationPolyObject.h>
#include <ossim/imaging/ossimAnnotationMultiPolyObject.h>
#include <ossim/imaging/ossimAnnotationMultiLineObject.h>
#include <ossim/imaging/ossimRgbImage.h>
#include <ossim/base/ossimIrect.h>

RTTI_DEF1(ossimAnnotationPolyObject,
          "ossimAnnotationPolyObject",
          ossimAnnotationObject)

   ossimAnnotationPolyObject::ossimAnnotationPolyObject(bool enableFill,
                                                        ossim_uint8 r,
                                                        ossim_uint8 g,
                                                        ossim_uint8 b,
                                                        ossim_uint8 thickness)
      :ossimAnnotationObject(r, g, b, thickness),
       thePolygon(),
       theBoundingRect(),
       theFillEnabled(enableFill)
{
}

ossimAnnotationPolyObject::ossimAnnotationPolyObject(
   const vector<ossimDpt>& imagePts,
   bool enableFill,
   ossim_uint8 r,
   ossim_uint8 g,
   ossim_uint8 b,
   ossim_uint8 thickness)
   :ossimAnnotationObject(r, g, b, thickness),
    theFillEnabled(enableFill)
{
   thePolygon = imagePts;
   computeBoundingRect();
}

ossimAnnotationPolyObject::ossimAnnotationPolyObject(
   const ossimAnnotationPolyObject& rhs)
   : ossimAnnotationObject(rhs),
     thePolygon(rhs.thePolygon),
     theBoundingRect(rhs.theBoundingRect),
     theFillEnabled(rhs.theFillEnabled)
{
}

ossimObject* ossimAnnotationPolyObject::dup()const
{
   return new ossimAnnotationPolyObject(*this);
}

ossimAnnotationPolyObject::~ossimAnnotationPolyObject()
{
}

void ossimAnnotationPolyObject::applyScale(double x, double y)
{
   thePolygon      *= ossimDpt(x, y);
   computeBoundingRect();
}

bool ossimAnnotationPolyObject::intersects(const ossimDrect& rect)const
{
   // do the quick checks first
   //
   if(rect.hasNans()) return false;
   if(!rect.intersects(theBoundingRect)) return false;
   
   if(!theFillEnabled)
   {
      int vertexCount = thePolygon.getVertexCount();
      ossimDpt start, end;
      int j = 0;
      while(j<vertexCount)
      {
         start = thePolygon[j];
         end   = thePolygon[(j+1)%vertexCount];
         if(rect.clip(start, end))
         {
            return true;
         }
         ++j;
      }
//       start = thePolygon[vertexCount-1];
//       end   = thePolygon[0];
//       int i = 0;
//       do
//       {
//          if(rect.clip(start, end))
//          {
//             return true;
//          }
//          ++i;
//          start = thePolygon[i];
//          end   = thePolygon[i-1];
//       }while(i < vertexCount);
   }
   else
   {
      vector<ossimPolygon> result;
      return thePolygon.clipToRect(result, rect);
   }

   return false;
}

ossimAnnotationObject* ossimAnnotationPolyObject::getNewClippedObject(const ossimDrect& rect)const
{
   ossimAnnotationObject* result = (ossimAnnotationObject*)NULL;

   if(theBoundingRect.intersects(rect))
   {
      if(theFillEnabled)
      {
         vector<ossimPolygon> resultPoly;
         
         if(thePolygon.clipToRect(resultPoly, rect))
         {
            if(resultPoly.size() == 1)
            {
               result = new ossimAnnotationPolyObject(resultPoly[0].getVertexList(),
                                                      theFillEnabled,
                                                      theRed,
                                                      theGreen,
                                                      theBlue,
                                                      theThickness);
            }
            else
            {
               result = new ossimAnnotationMultiPolyObject(resultPoly,
                                                           theFillEnabled,
                                                           theRed,
                                                           theGreen,
                                                           theBlue,
                                                           theThickness);
            }
         }
      }
      else
      {
         vector<ossimPolyLine> lineListResult;
         ossimPolyLine polyLine = thePolygon;
         
         if(polyLine.clipToRect(lineListResult,
                                rect))
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

void ossimAnnotationPolyObject::draw(ossimRgbImage& anImage)const
{
   if(thePolygon.getVertexCount() < 2) return;
   if(theBoundingRect.hasNans()) return;
   int vertexCount = thePolygon.getVertexCount();
   
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
      
      if(!theFillEnabled)
      {
         ossimDpt start, end;
         if(thePolygon.getNumberOfVertices() == 1)
         {
            start = thePolygon[0];
            end   = thePolygon[0];
            if(clipRect.clip(start, end))
            {
               anImage.drawLine(ossimIpt(start),
                                ossimIpt(end));
            }
         }
         else if(thePolygon.getNumberOfVertices() == 2)
         {
            start = thePolygon[0];
            end   = thePolygon[1];
            if(clipRect.clip(start, end))
            {
               anImage.drawLine(ossimIpt(start),
                                ossimIpt(end));
            }
         }
         else
         {
            int j = 0;
            while(j<vertexCount)
            {
               start = thePolygon[j];
               end   = thePolygon[(j+1)%vertexCount];
               if(clipRect.clip(start, end))
               {
                  anImage.drawLine(ossimIpt(start),
                                   ossimIpt(end));
               }
               ++j;
            }
         }
#if 0
               ossimDpt start, end;
               start = thePolygon[vertexCount-1];
               end   = thePolygon[0];
               int i = 0;
               do
               {
                  if(clipRect.clip(start, end))
                  {
                     anImage.drawLine(ossimIpt((int)start.x, (int)start.y),
                                      ossimIpt((int)end.x, (int)end.y));
                  }
                  ++i;
                  start = thePolygon[i];
                  end   = thePolygon[i-1];
               }while(i < vertexCount);
#endif    
      }
      else
      {
         vector<ossimPolygon> result;
         if(thePolygon.clipToRect(result, imageRect))
         {
            for(int i = 0; i < (int)result.size();++i)
            {
               anImage.drawFilledPolygon(result[i].getVertexList());
            }
         }
      }
   }
}

std::ostream& ossimAnnotationPolyObject::print(std::ostream& out)const
{
   out << "number_of_points:  " << thePolygon.getVertexCount();
   if(thePolygon.getVertexCount() > 0)
   {
      for(long index =0; index < (long)(thePolygon.getVertexCount()-1); ++index)
      {
         out << thePolygon[index] << endl;
            
      }
      out << thePolygon[thePolygon.getVertexCount()-1] << endl;
   }
   out << "fill_enabled:      " << theFillEnabled << endl
       << "bounding_rect:     " << theBoundingRect << endl;
   return out;
}

void ossimAnnotationPolyObject::getBoundingRect(ossimDrect& rect)const
{
   rect = theBoundingRect;
}

void ossimAnnotationPolyObject::addPoint(const ossimDpt& pt)
{
   thePolygon.addPoint(pt);
}

void ossimAnnotationPolyObject::setPolygon(const vector<ossimDpt>& imagePoints)
{
   thePolygon = imagePoints;
}

void ossimAnnotationPolyObject::setPolygon(const ossimPolygon& polygon)
{
   thePolygon = polygon;
}

void ossimAnnotationPolyObject::setPolygon(const ossimIrect& rect)
{
   thePolygon = rect;
}

void ossimAnnotationPolyObject::setPolygon(const ossimDrect& rect)
{
   thePolygon = rect;
}

void ossimAnnotationPolyObject::computeBoundingRect()
{
   theBoundingRect = ossimDrect(thePolygon);
}

bool ossimAnnotationPolyObject::isPointWithin(const ossimDpt& imagePoint)const
{
   return theBoundingRect.pointWithin(imagePoint);
}

const ossimPolygon& ossimAnnotationPolyObject::getPolygon() const
{
   return thePolygon;
}

ossimPolygon& ossimAnnotationPolyObject::getPolygon()
{
   return thePolygon;
}

void ossimAnnotationPolyObject::setFillFlag(bool flag)
{
   theFillEnabled = flag;
}
