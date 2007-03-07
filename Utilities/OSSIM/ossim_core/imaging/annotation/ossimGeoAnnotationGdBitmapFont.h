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
// $Id: ossimGeoAnnotationGdBitmapFont.h,v 1.8 2004/05/23 17:49:37 dburken Exp $
#ifndef ossimGeoAnnotationGdBitmapFont_HEADER
#define ossimGeoAnnotationGdBitmapFont_HEADER

#include "ossimGeoAnnotationObject.h"
#include "ossimGdFont.h"
#include "ossimGdFontExterns.h"
class ossimAnnotationGdBitmapFont;

class OSSIMDLLEXPORT ossimGeoAnnotationGdBitmapFont : public ossimGeoAnnotationObject
{
public:
   ossimGeoAnnotationGdBitmapFont(const ossimGpt& position=ossimGpt(0,0,0),
                                  const ossimString &text=ossimString(""),
                                  ossimGdFontPtr font = ossimGdFontLarge,
                                  unsigned char r = 255,
                                  unsigned char g = 255,
                                  unsigned char b = 255,
                                  long thickness = 1);
   ossimGeoAnnotationGdBitmapFont(const ossimGeoAnnotationGdBitmapFont& rhs);
   virtual ~ossimGeoAnnotationGdBitmapFont();
   virtual ossimObject* dup()const
      {
         return new ossimGeoAnnotationGdBitmapFont(*this);
      }
   virtual void applyScale(double x, double y);
   virtual std::ostream& print(std::ostream& out)const;
   virtual void draw(ossimRgbImage& anImage)const;
   virtual bool intersects(const ossimDrect& rect)const;
   virtual ossimAnnotationObject* getNewClippedObject(const ossimDrect& rect)const;
   virtual void getBoundingRect(ossimDrect& rect)const;
   virtual void computeBoundingRect();
   /*!
    * Add the projection interface to all
    * geographically defined objects.
    */
   virtual void transform(ossimProjection* projection);

   virtual void setText(const ossimString& text);
   virtual void setFont(ossimGdFontPtr font);

protected:
   ossimAnnotationGdBitmapFont* theProjectedFont;

   /*!
    * Will be the upper left corner of text;
    */
   ossimGpt                     thePosition;
TYPE_DATA
};

#endif
