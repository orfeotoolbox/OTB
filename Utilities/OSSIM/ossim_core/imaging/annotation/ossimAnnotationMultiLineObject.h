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
// $Id: ossimAnnotationMultiLineObject.h,v 1.5 2004/05/23 17:49:37 dburken Exp $
#ifndef ossimAnnotationMultiLineObject_HEADER
#define ossimAnnotationMultiLineObject_HEADER
#include "ossimAnnotationObject.h"
#include "base/data_types/ossimIpt.h"
#include "base/data_types/ossimPolyLine.h"

class OSSIMDLLEXPORT ossimAnnotationMultiLineObject : public ossimAnnotationObject
{
public:
   ossimAnnotationMultiLineObject();
   ossimAnnotationMultiLineObject(const std::vector<ossimPolyLine>& lineList,
                                  unsigned char r=255,
                                  unsigned char g=255,
                                  unsigned char b=255,
                                  long thickness=1)
      :ossimAnnotationObject(r, g, b, thickness),
       thePolyLineList(lineList)
      {
         computeBoundingRect();
      }

   ossimAnnotationMultiLineObject(const ossimPolyLine& lineList,
                                  unsigned char r=255,
                                  unsigned char g=255,
                                  unsigned char b=255,
                                  long thickness=1)
      :ossimAnnotationObject(r, g, b, thickness)
      {
         thePolyLineList.push_back(lineList);
         computeBoundingRect();
      }
   

   ossimAnnotationMultiLineObject(const ossimAnnotationMultiLineObject& rhs)
      :ossimAnnotationObject(rhs),
       thePolyLineList(rhs.thePolyLineList),
       theBoundingRect(rhs.theBoundingRect)
      {
      }
       
   virtual ossimObject* dup()const
      {
         return new ossimAnnotationMultiLineObject(*this);
      }
   
   virtual void applyScale(double x,
                           double y);
   virtual ossimAnnotationObject* getNewClippedObject(const ossimDrect& rect)const;
   virtual bool intersects(const ossimDrect& rect)const;   
   virtual void draw(ossimRgbImage& anImage)const;
   virtual std::ostream& print(std::ostream& out)const;
   virtual void getBoundingRect(ossimDrect& rect)const
      {
         rect = theBoundingRect;
      }
   virtual bool isPointWithin(const ossimDpt& imagePoint)const;
   virtual void computeBoundingRect();

   virtual const vector<ossimPolyLine>& getPolyLineList()const
      {
         return thePolyLineList;
      }

   virtual vector<ossimPolyLine>& getPolyLineList()
      {
         return thePolyLineList;
      }
   virtual void setPolyLine(const ossimPolyLine& line)
      {
         thePolyLineList.clear();
         thePolyLineList.push_back(line);
      }
protected:
   vector<ossimPolyLine> thePolyLineList;
   ossimDrect            theBoundingRect;

TYPE_DATA
};

#endif
