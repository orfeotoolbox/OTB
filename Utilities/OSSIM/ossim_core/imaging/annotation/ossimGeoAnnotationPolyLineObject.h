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
// $Id: ossimGeoAnnotationPolyLineObject.h,v 1.4 2005/05/27 12:17:43 gpotts Exp $
#ifndef ossimGeoAnnotationPolyLineObject_HEADER
#define ossimGeoAnnotationPolyLineObject_HEADER
#include "base/data_types/ossimGpt.h"
#include "ossimGeoAnnotationObject.h"

class ossimAnnotationMultiLineObject;

class OSSIMDLLEXPORT ossimGeoAnnotationPolyLineObject : public ossimGeoAnnotationObject
{
public:
   ossimGeoAnnotationPolyLineObject(const vector<ossimGpt>& groundPts,
                                     unsigned char r=255,
                                     unsigned char g=255,
                                     unsigned char b=255,
                                     long thickness=1);
   ossimGeoAnnotationPolyLineObject(const ossimGeoAnnotationPolyLineObject& rhs);
   virtual ~ossimGeoAnnotationPolyLineObject();
   virtual ossimObject* dup()const
      {
         return new ossimGeoAnnotationPolyLineObject(*this);
      }
   
   virtual void applyScale(double x,
                           double y);

   virtual void transform(ossimProjection* projection);
   virtual std::ostream& print(std::ostream& out)const;
   virtual void draw(ossimRgbImage& anImage)const;
   virtual ossimAnnotationObject* getNewClippedObject(const ossimDrect& rect)const;
   virtual bool intersects(const ossimDrect& rect)const;
   
   virtual void getBoundingRect(ossimDrect& rect)const;
   virtual void computeBoundingRect();
   virtual void setColor(unsigned char r,
                         unsigned char g,
                         unsigned char b);
   virtual void setThickness(long thickness);

protected:
   vector<ossimGpt>                thePolygon;
   ossimAnnotationMultiLineObject* theProjectedMultiLineObject;

TYPE_DATA
};

#endif
