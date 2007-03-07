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
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description:
//
//*************************************************************************
// $Id: ossimGeoAnnotationMultiPolyObject.h,v 1.5 2004/05/23 17:49:37 dburken Exp $
#ifndef ossimGeoAnnotationMultiPolyObject_HEADER
#define ossimGeoAnnotationMultiPolyObject_HEADER
#include "ossimGeoAnnotationObject.h"
#include "base/data_types/ossimIpt.h"
#include "base/data_types/ossimGeoPolygon.h"
#include "ossimAnnotationMultiPolyObject.h"

class OSSIMDLLEXPORT ossimGeoAnnotationMultiPolyObject : public ossimGeoAnnotationObject
{
public:
   ossimGeoAnnotationMultiPolyObject();
   ossimGeoAnnotationMultiPolyObject(const vector<ossimGeoPolygon>& multiPoly,
                                     bool enableFill=false,
                                     unsigned char r=255,
                                     unsigned char g=255,
                                     unsigned char b=255,
                                     long thickness=1);
   
   ossimGeoAnnotationMultiPolyObject(const ossimGeoAnnotationMultiPolyObject& rhs);
   virtual ossimObject* dup()const
      {
         return new ossimGeoAnnotationMultiPolyObject(*this);
      }
   virtual ~ossimGeoAnnotationMultiPolyObject();
   virtual void transform(ossimProjection* projection);
   virtual void applyScale(double x,
                                double y);
   virtual void draw(ossimRgbImage& anImage)const;
   virtual bool intersects(const ossimDrect& rect)const;
   
   virtual ossimAnnotationObject* getNewClippedObject(const ossimDrect& rect)const;
   virtual std::ostream& print(std::ostream& out)const;
   virtual void getBoundingRect(ossimDrect& rect)const;
   virtual void addPoint(ossim_uint32 polygonIndex,
                         const ossimGpt& pt)
      {
         if(polygonIndex < theMultiPolygon.size())
         {
            theMultiPolygon[polygonIndex].addPoint(pt);

            // we will have to reset the projected polygon
            if(theProjectedPolyObject)
            {
               delete theProjectedPolyObject;
               theProjectedPolyObject = NULL;
            }
         }
      }
   
   virtual void setMultiPolygon(const vector<ossimGeoPolygon>& multiPoly)
      {
         theMultiPolygon = multiPoly;
         if(theProjectedPolyObject)
         {
            delete theProjectedPolyObject;
            theProjectedPolyObject = NULL;
         }
      }
   virtual void setColor(unsigned char r,
                         unsigned char g,
                         unsigned char b)
      {
         ossimAnnotationObject::setColor(r, g, b);
         if(theProjectedPolyObject)
         {
            theProjectedPolyObject->setColor(r, g, b);
         }
      }
   
   virtual void setThickness(long thickness)
      {
         ossimAnnotationObject::setThickness(thickness);
         if(theProjectedPolyObject)
         {
            theProjectedPolyObject->setThickness(thickness);
         }
      }
   virtual void computeBoundingRect();
   virtual bool isPointWithin(const ossimDpt& imagePoint)const;
   virtual void setFillFlag(bool flag);
  
   const std::vector<ossimGeoPolygon>& getMultiPolygon()const{return theMultiPolygon;}
   std::vector<ossimGeoPolygon>& getMultiPolygon(){return theMultiPolygon;}
   
protected:
   vector<ossimGeoPolygon> theMultiPolygon;
   ossimDrect              theBoundingRect;
   bool                   theFillEnabled;
   ossimAnnotationMultiPolyObject* theProjectedPolyObject;
   
   void allocateProjectedPolygon();
TYPE_DATA
};

#endif
