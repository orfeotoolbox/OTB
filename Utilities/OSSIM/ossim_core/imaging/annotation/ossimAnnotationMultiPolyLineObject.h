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
// $Id: ossimAnnotationMultiPolyLineObject.h,v 1.2 2004/05/23 17:49:37 dburken Exp $
#ifndef ossimAnnotationMultiPolyLineObject_HEADER
#define ossimAnnotationMultiPolyLineObject_HEADER

#include "ossimAnnotationObject.h"
#include "base/data_types/ossimIpt.h"
#include "base/data_types/ossimPolyLine.h"

class OSSIMDLLEXPORT ossimAnnotationMultiPolyLineObject : public ossimAnnotationObject
{
public:
   ossimAnnotationMultiPolyLineObject();
   ossimAnnotationMultiPolyLineObject(const vector<ossimPolyLine>& multiPoly,
                                  unsigned char r,
                                  unsigned char g,
                                  unsigned char b,
                                  long thickness);
   
   ossimAnnotationMultiPolyLineObject(const ossimAnnotationMultiPolyLineObject& rhs);

   virtual ossimObject* dup()const;

   virtual ~ossimAnnotationMultiPolyLineObject();
   virtual void applyScale(double x,
                           double y);
   virtual void draw(ossimRgbImage& anImage)const;
   virtual bool intersects(const ossimDrect& rect)const;
   
   virtual ossimAnnotationObject* getNewClippedObject(const ossimDrect& rect)const;
   virtual std::ostream& print(std::ostream& out)const;
   virtual void getBoundingRect(ossimDrect& rect)const;
   virtual void addPolyLine(const ossimPolyLine& poly);
   virtual void addPoint(ossim_uint32 polygonIndex, const ossimDpt& pt);
   
   virtual void setMultiPolyLine(const vector<ossimPolyLine>& multiPoly);
   virtual void computeBoundingRect();
   virtual bool isPointWithin(const ossimDpt& imagePoint)const;
   
   const std::vector<ossimPolyLine>& getMultiPolyLine()const;
   std::vector<ossimPolyLine>& getMultiPolyLine();
protected:
   vector<ossimPolyLine> theMultiPolyLine;
   ossimDrect           theBoundingRect;

TYPE_DATA
};

#endif
