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
// $Id: ossimAnnotationMultiPolyObject.h,v 1.7 2004/05/23 17:49:37 dburken Exp $
#ifndef ossimAnnotationMultiPolyObject_HEADER
#define ossimAnnotationMultiPolyObject_HEADER
#include "ossimAnnotationObject.h"
#include "base/data_types/ossimIpt.h"
#include "base/data_types/ossimPolygon.h"

class OSSIMDLLEXPORT ossimAnnotationMultiPolyObject : public ossimAnnotationObject
{
public:
   ossimAnnotationMultiPolyObject();
   ossimAnnotationMultiPolyObject(const vector<ossimPolygon>& multiPoly,
                                  bool enableFill,
                                  unsigned char r,
                                  unsigned char g,
                                  unsigned char b,
                                  long thickness);
   
   ossimAnnotationMultiPolyObject(const ossimAnnotationMultiPolyObject& rhs)
      : ossimAnnotationObject(rhs),
        theMultiPolygon(rhs.theMultiPolygon),
        theBoundingRect(rhs.theBoundingRect),
        theFillEnabled(rhs.theFillEnabled)
      {
      }
   virtual ossimObject* dup()const
      {
         return new ossimAnnotationMultiPolyObject(*this);
      }
   virtual ~ossimAnnotationMultiPolyObject();
   virtual void applyScale(double x,
                           double y);
   virtual void draw(ossimRgbImage& anImage)const;
   virtual bool intersects(const ossimDrect& rect)const;
   
   virtual ossimAnnotationObject* getNewClippedObject(const ossimDrect& rect)const;
   virtual std::ostream& print(std::ostream& out)const;
   virtual void getBoundingRect(ossimDrect& rect)const;
   virtual void clear();
   virtual void addPolygon(const ossimPolygon& poly)
      {
         theMultiPolygon.push_back(poly);
      }
   virtual void addPoint(ossim_uint32 polygonIndex,
                         const ossimDpt& pt)
      {
         if(polygonIndex < theMultiPolygon.size())
         {
            theMultiPolygon[polygonIndex].addPoint(pt);
         }
      }
   
   virtual void setMultiPolygon(const vector<ossimPolygon>& multiPoly)
      {
         theMultiPolygon = multiPoly;
         computeBoundingRect();
      }
   virtual void computeBoundingRect();
   virtual bool isPointWithin(const ossimDpt& imagePoint)const;
   
   const std::vector<ossimPolygon>& getMultiPolygon()const{return theMultiPolygon;}
   std::vector<ossimPolygon>& getMultiPolygon(){return theMultiPolygon;}
   void setFillFlag(bool flag)
      {
         theFillEnabled = flag;
      }
protected:
   vector<ossimPolygon> theMultiPolygon;
   ossimDrect           theBoundingRect;
   bool                 theFillEnabled;

TYPE_DATA
};

#endif
