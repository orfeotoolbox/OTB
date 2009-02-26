//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimGeoAnnotationMultiPolyObject.cpp 13348 2008-07-30 15:33:53Z dburken $

#include <ossim/imaging/ossimGeoAnnotationMultiPolyObject.h>
#include <ossim/imaging/ossimAnnotationMultiPolyObject.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimImageProjectionModel.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimPolyArea2d.h>

RTTI_DEF1(ossimGeoAnnotationMultiPolyObject, "ossimGeoAnnotationMultiPolyObject", ossimGeoAnnotationObject);

ossimGeoAnnotationMultiPolyObject::ossimGeoAnnotationMultiPolyObject()
   :
      ossimGeoAnnotationObject(),
      theMultiPolygon(),
      theBoundingRect(),
      theFillEnabled(false),
      theProjectedPolyObject(0)
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
      theProjectedPolyObject(0)
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
      theProjectedPolyObject(rhs.theProjectedPolyObject?(ossimAnnotationMultiPolyObject*)rhs.theProjectedPolyObject->dup():0)
{
}

ossimObject* ossimGeoAnnotationMultiPolyObject::dup()const
{
   return new ossimGeoAnnotationMultiPolyObject(*this);
}

ossimGeoAnnotationMultiPolyObject::~ossimGeoAnnotationMultiPolyObject()
{
   if(theProjectedPolyObject)
   {
      delete theProjectedPolyObject;
      theProjectedPolyObject = 0;
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

   //---
   // NOTE:
   // allocateProjectedPolygon() will set theProjectedPolyObject to 0 if
   // theMultiPolygon is empty (theMultiPolygon.size() == 0).  So check before
   // accessing pointer to avoid a core dump.
   //---
   if (!theProjectedPolyObject)
   {
      return;
   }

   ossimDpt temp;
   std::vector<ossimPolygon> visiblePolygons;
   ossimPolygon polygon;
   for(ossim_uint32 polyI = 0; polyI < theMultiPolygon.size(); ++polyI)
   {
      polygon.clear();
      for(ossim_uint32 pointI = 0;
          pointI < theMultiPolygon[polyI].size();
          ++pointI)
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

void ossimGeoAnnotationMultiPolyObject::transform(
   const ossimImageProjectionModel& model, ossim_uint32 rrds)
{
   const ossimProjection* projection = model.getProjection();
   if (projection)
   {
      allocateProjectedPolygon();

      //---
      // NOTE:
      // allocateProjectedPolygon() will set theProjectedPolyObject to 0 if
      // theMultiPolygon is empty (theMultiPolygon.size() == 0).
      // So check before
      // accessing pointer to avoid a core dump.
      //---
      if (theProjectedPolyObject)
      {
         ossimDpt temp;
         std::vector<ossimPolygon> visiblePolygons;
         ossimPolygon polygon;
         for(std::vector<ossimGeoPolygon>::size_type polyI = 0;
             polyI < theMultiPolygon.size(); ++polyI)
         {
            polygon.clear();
            for(ossim_uint32 pointI = 0;
                pointI < theMultiPolygon[polyI].size();
                ++pointI)
            {
               ossimDpt r0Pt;
               projection->worldToLineSample(theMultiPolygon[polyI][pointI],
                                             r0Pt);
               if(!r0Pt.hasNans())
               {
                  if (rrds)
                  {
                     // Transform r0 point to new rrds level.
                     try
                     {
                        ossimDpt rnPt;
                        model.r0ToRn(rrds, r0Pt, rnPt);
                        polygon.addPoint(rnPt);
                     }
                     catch (const ossimException& e)
                     {
                        ossimNotify(ossimNotifyLevel_WARN)
                           << e.what() << std::endl;
                     } 
                  }
                  else
                  {
                     polygon.addPoint(r0Pt);
                  }
               }
               
            }  // End of point loop.
            
            theProjectedPolyObject->addPolygon(polygon);
            
         } // End fo polygon loop.
         
         // Update the bounding rect.
         theProjectedPolyObject->computeBoundingRect();
         
      } // End: if (theProjectedPolyObject)
      
   } // End: if (projection)
   
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

void ossimGeoAnnotationMultiPolyObject::addPoint(ossim_uint32 polygonIndex,
                                                 const ossimGpt& pt)
{
   if(polygonIndex < theMultiPolygon.size())
   {
      theMultiPolygon[polygonIndex].addPoint(pt);
      
      // we will have to reset the projected polygon
      if(theProjectedPolyObject)
      {
         delete theProjectedPolyObject;
         theProjectedPolyObject = 0;
      }
   }
}

void ossimGeoAnnotationMultiPolyObject::setMultiPolygon(
   const vector<ossimGeoPolygon>& multiPoly)
{
   theMultiPolygon = multiPoly;
   if(theProjectedPolyObject)
   {
      delete theProjectedPolyObject;
      theProjectedPolyObject = 0;
   }
}

void ossimGeoAnnotationMultiPolyObject::setColor(unsigned char r,
                                                 unsigned char g,
                                                 unsigned char b)
{
   ossimAnnotationObject::setColor(r, g, b);
   if(theProjectedPolyObject)
   {
      theProjectedPolyObject->setColor(r, g, b);
   }
}

void ossimGeoAnnotationMultiPolyObject::setThickness(ossim_uint8 thickness)
{
   ossimAnnotationObject::setThickness(thickness);
   if(theProjectedPolyObject)
   {
      theProjectedPolyObject->setThickness(thickness);
   }
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
      theProjectedPolyObject = 0;
   }
   
   if(theMultiPolygon.size())
   {
      vector<ossimPolygon> polyList( theMultiPolygon.size() );
      theProjectedPolyObject =
         new ossimAnnotationMultiPolyObject(polyList,
                                            theFillEnabled,
                                            theRed,
                                            theGreen,
                                            theBlue,
                                            theThickness);
   }
}
