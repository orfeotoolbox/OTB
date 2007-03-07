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
// $Id: ossimGeoAnnotationLineObject.h,v 1.7 2004/05/23 17:49:37 dburken Exp $
#ifndef ossimGeoAnnotationLineObject_HEADER
#define ossimGeoAnnotationLineObject_HEADER
#include "base/data_types/ossimGpt.h"
#include "ossimGeoAnnotationObject.h"

class OSSIMDLLEXPORT ossimAnnotationLineObject;

class ossimGeoAnnotationLineObject : public ossimGeoAnnotationObject
{
public:
   ossimGeoAnnotationLineObject(const ossimGpt& start,
                                const ossimGpt& end,
                                unsigned char r=255,
                                unsigned char g=255,
                                unsigned char b=255,
                                long thickness=1);
   ossimGeoAnnotationLineObject(const ossimGeoAnnotationLineObject& rhs);
   virtual ~ossimGeoAnnotationLineObject();

   virtual ossimObject* dup()const
      {
         return new ossimGeoAnnotationLineObject(*this);
      }

   virtual void applyScale(double x, double y);
   virtual void transform(ossimProjection* projection);
   virtual std::ostream& print(std::ostream& out)const;
   virtual bool intersects(const ossimDrect& rect)const;
   virtual ossimAnnotationObject* getNewClippedObject(const ossimDrect& rect)const;
   virtual void draw(ossimRgbImage& anImage)const;
   virtual void getBoundingRect(ossimDrect& rect)const;
   virtual void computeBoundingRect();

protected:
   ossimAnnotationLineObject* theProjectedLineObject;
   ossimGpt                   theStart;
   ossimGpt                   theEnd;

TYPE_DATA
};

#endif
