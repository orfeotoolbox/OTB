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
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimGeoAnnotationMultiEllipseObject.h,v 1.8 2004/05/23 17:49:37 dburken Exp $
#ifndef ossimGeoAnnotationMultiEllipseObject_HEADER
#define ossimGeoAnnotationMultiEllipseObject_HEADER
#include "ossimGeoAnnotationObject.h"
#include <vector>
class ossimAnnotationMultiEllipseObject;
class OSSIMDLLEXPORT ossimGeoAnnotationMultiEllipseObject : public ossimGeoAnnotationObject
{
public:
   ossimGeoAnnotationMultiEllipseObject();
   ossimGeoAnnotationMultiEllipseObject(const std::vector<ossimGpt>& centerPoints,
                                        const ossimDpt& widthHeight,
                                        bool enableFill = false,
                                        unsigned char r = 255,
                                        unsigned char g = 255,
                                        unsigned char b = 255,
                                        long thickness = 1);
   ossimGeoAnnotationMultiEllipseObject(const ossimDpt& widthHeight,
                                        bool enableFill = false,
                                        unsigned char r = 255,
                                        unsigned char g = 255,
                                        unsigned char b = 255,
                                        long thickness = 1);
   ossimGeoAnnotationMultiEllipseObject(const ossimGeoAnnotationMultiEllipseObject& rhs);
   
   virtual ossimObject* dup()const;

   virtual ossimAnnotationObject* getNewClippedObject(const ossimDrect& rect)const;

   virtual void transform(ossimProjection* projection);

   virtual void applyScale(double x, double y);

   virtual void draw(ossimRgbImage& anImage)const;
   virtual bool intersects(const ossimDrect& rect)const;
   
   virtual void addPoint(const ossimGpt& point);

   virtual void setColor(unsigned char r,
                         unsigned char g,
                         unsigned char b);
   virtual void setThickness(long thickness);
   
   virtual void setPoint(int i, const ossimGpt& point);

   virtual void setWidthHeight(const ossimDpt& widthHeight);
   virtual void resize(ossim_uint32 newSize);

   virtual void setFillFlag(bool fillFlag);
   virtual std::ostream& print(std::ostream& out)const;
   virtual void getBoundingRect(ossimDrect& rect)const;
   virtual void computeBoundingRect();
   
protected:
   ossimAnnotationMultiEllipseObject* theProjectedObject;
   
   std::vector<ossimGpt> thePointList;
   ossimDpt              theWidthHeight;
   bool                  theFillFlag;
};

#endif /* #ifndef ossimGeoAnnotationMultiEllipseObject_HEADER */
