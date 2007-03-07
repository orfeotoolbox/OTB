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
//
//*************************************************************************
// $Id: ossimAnnotationMultiEllipseObject.h,v 1.6 2004/05/23 17:49:37 dburken Exp $
#ifndef ossimAnnotationMultiEllipseObject_HEADER
#define ossimAnnotationMultiEllipseObject_HEADER
#include <imaging/annotation/ossimAnnotationObject.h>
#include <vector>

class OSSIMDLLEXPORT ossimAnnotationMultiEllipseObject : public ossimAnnotationObject
{
public:
   ossimAnnotationMultiEllipseObject(const std::vector<ossimDpt>& pointList,
                                     const ossimDpt& widthHeight = ossimDpt(1,1),
                                     bool enableFill = false,
                                     unsigned char r = 255,
                                     unsigned char g = 255,
                                     unsigned char b = 255,
                                     long thickness = 1);

   ossimAnnotationMultiEllipseObject(const ossimDpt& widthHeight = ossimDpt(1,1),
                                     bool enableFill = false,
                                     unsigned char r = 255,
                                     unsigned char g = 255,
                                     unsigned char b = 255,
                                     long thickness = 1);

   ossimAnnotationMultiEllipseObject(const ossimAnnotationMultiEllipseObject& rhs);

   ossimObject* dup()const;

   virtual ~ossimAnnotationMultiEllipseObject();

   virtual ossimAnnotationObject* getNewClippedObject(const ossimDrect& rect)const;
   virtual void applyScale(double x, double y);
   virtual void draw(ossimRgbImage& anImage)const;
   virtual bool intersects(const ossimDrect& rect)const;
   virtual std::ostream& print(std::ostream& out)const;
   virtual void getBoundingRect(ossimDrect& rect)const;
   virtual void computeBoundingRect();
  /*!
   */
   virtual bool isPointWithin(const ossimDpt& imagePoint)const;
   
   void setFillFlag(bool flag);

   void resize(ossim_uint32 size);

   virtual void setWidthHeight(const ossimDpt& widthHeight);

   ossimDpt& operator[](int i);
   
   const ossimDpt& operator[](int i)const;
   
protected:
   std::vector<ossimDpt>  thePointList;
   ossimDpt               theWidthHeight;
   bool                   theFillFlag;
   ossimDrect             theBoundingRect;
};

#endif
