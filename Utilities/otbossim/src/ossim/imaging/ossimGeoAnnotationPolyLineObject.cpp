//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimGeoAnnotationPolyLineObject.cpp 13348 2008-07-30 15:33:53Z dburken $
#include <ossim/imaging/ossimGeoAnnotationPolyLineObject.h>
#include <ossim/imaging/ossimAnnotationMultiLineObject.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimImageProjectionModel.h>
#include <ossim/base/ossimException.h>


RTTI_DEF1(ossimGeoAnnotationPolyLineObject,
          "ossimGeoAnnotationPolyLineObject",
          ossimGeoAnnotationObject)
   
ossimGeoAnnotationPolyLineObject::ossimGeoAnnotationPolyLineObject(
   const vector<ossimGpt>& groundPts,
   ossim_uint8 r,
   ossim_uint8 g,
   ossim_uint8 b,
   ossim_uint8 thickness)
   :ossimGeoAnnotationObject(r, g, b, thickness),
    theProjectedMultiLineObject(0)
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
      theProjectedMultiLineObject = 0;
   }
}

 ossimObject* ossimGeoAnnotationPolyLineObject::dup()const
{
   return new ossimGeoAnnotationPolyLineObject(*this);
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
   vector<ossimPolyLine>& polyList =
      theProjectedMultiLineObject->getPolyLineList();

   if(polyList.size())
   {
      vector<ossimDpt>& poly = polyList[0].getVertexList();
      
      const std::vector<ossimGpt>::size_type BOUNDS = thePolygon.size();
      
      for(std::vector<ossimGpt>::size_type index=0; index < BOUNDS; ++index)
      {
         projection->worldToLineSample(thePolygon[index], poly[index]);
      }
      
      // update the bounding rect
      theProjectedMultiLineObject->computeBoundingRect();
   }
}

void ossimGeoAnnotationPolyLineObject::transform(
   const ossimImageProjectionModel& model, ossim_uint32 rrds)
{
   const ossimProjection* projection = model.getProjection();
   if (projection)
   {
      std::vector<ossimPolyLine>& polyList =
         theProjectedMultiLineObject->getPolyLineList();
      
      if(polyList.size())
      {
         std::vector<ossimDpt>& poly = polyList[0].getVertexList();
         
         const std::vector<ossimGpt>::size_type BOUNDS = thePolygon.size();
         
         for(std::vector<ossimGpt>::size_type index = 0;
             index < BOUNDS; ++index)
         {
            ossimDpt r0Pt;
            projection->worldToLineSample(thePolygon[index], r0Pt);
            
            if (rrds)
            {
               // Transform r0 point to new rrds level.
               try
               {
                  ossimDpt rnPt;
                  model.r0ToRn(rrds, r0Pt, rnPt);
                  poly[index] = rnPt;
               }
               catch (const ossimException& e)
               {
                  ossimNotify(ossimNotifyLevel_WARN) << e.what() << std::endl;
               } 
            }
            else
            {
               poly[index] = r0Pt;
            }
            
         } // End loop though polygon points.
      }
      
      // update the bounding rect
      theProjectedMultiLineObject->computeBoundingRect();
      
   } // End if (projection)
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

void ossimGeoAnnotationPolyLineObject::setColor(ossim_uint8 r,
                                                ossim_uint8 g,
                                                ossim_uint8 b)
{
   if(theProjectedMultiLineObject)
   {
      theProjectedMultiLineObject->setColor(r, g, b);
   }
}

void ossimGeoAnnotationPolyLineObject::setThickness(ossim_uint8 thickness)
{
   if(theProjectedMultiLineObject)
   {
      theProjectedMultiLineObject->setThickness(thickness);
   }
}
