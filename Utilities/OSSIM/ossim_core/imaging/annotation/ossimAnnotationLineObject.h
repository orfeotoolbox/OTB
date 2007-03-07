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
// $Id: ossimAnnotationLineObject.h,v 1.12 2004/05/23 17:49:37 dburken Exp $
#ifndef ossimAnnotationLineObject_HEADER
#define ossimAnnotationLineObject_HEADER
#include "ossimAnnotationObject.h"
#include "base/data_types/ossimIpt.h"

class OSSIMDLLEXPORT ossimAnnotationLineObject : public ossimAnnotationObject
{
public:
   ossimAnnotationLineObject();
   ossimAnnotationLineObject(const ossimIpt& start,
                             const ossimIpt& end,
                             unsigned char r=255,
                             unsigned char g=255,
                             unsigned char b=255,
                             long thickness=1);
   
   ossimAnnotationLineObject(long x1,
                             long y1,
                             long x2,
                             long y2,
                             unsigned char r=255,
                             unsigned char g=255,
                             unsigned char b=255,
                             long thickness=1);

   ossimAnnotationLineObject(const ossimAnnotationLineObject& rhs);
       
   virtual ossimObject* dup()const;
   
   virtual void applyScale(double x, double y);

   virtual ossimAnnotationObject* getNewClippedObject(const ossimDrect& rect)const;

   virtual bool intersects(const ossimDrect& rect)const;
   
   virtual void draw(ossimRgbImage& anImage)const;
   virtual std::ostream& print(std::ostream& out)const;
   virtual void getBoundingRect(ossimDrect& rect)const;
   virtual bool isPointWithin(const ossimDpt& imagePoint)const;
   virtual void computeBoundingRect();

   void setLine(const ossimDpt& start, const ossimDpt& end);
   void getLine(ossimDpt& start, ossimDpt& end);

protected:
   ossimIpt theStart;
   ossimIpt theEnd;
   ossimDrect theBoundingRect;

TYPE_DATA
};

#endif
