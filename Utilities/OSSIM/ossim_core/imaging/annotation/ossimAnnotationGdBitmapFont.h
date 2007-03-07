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
// $Id: ossimAnnotationGdBitmapFont.h,v 1.10 2004/05/23 17:49:36 dburken Exp $
#ifndef ossimAnnotationGdBitmapFont_HEADER
#define ossimAnnotationGdBitmapFont_HEADER
#include "ossimAnnotationObject.h"
#include "ossimGdFont.h"
#include "ossimGdFontExterns.h"
#include "base/data_types/ossimString.h"

/*!
 * This is a class that will have hardcoded
 * bitmap fonts.  These fonts will be from
 * the gd library.
 */
class OSSIMDLLEXPORT ossimAnnotationGdBitmapFont : public ossimAnnotationObject
{
public:
   ossimAnnotationGdBitmapFont(const ossimDpt& position=ossimDpt(0,0),
                               const ossimString &text=ossimString(""),
                               ossimGdFontPtr font = ossimGdFontLarge,
                               unsigned char r = 255,
                               unsigned char g = 255,
                               unsigned char b = 255,
                               long thickness = 1);
   ossimAnnotationGdBitmapFont(const ossimAnnotationGdBitmapFont& rhs);
   virtual ~ossimAnnotationGdBitmapFont();

   virtual ossimObject* dup()const
      {
         return new ossimAnnotationGdBitmapFont(*this);
      }
   
   virtual void applyScale(double x, double y);

   /*!
    * Will draw the bitmap to the screen.
    */
   virtual void draw(ossimRgbImage& anImage)const;
   virtual bool intersects(const ossimDrect& rect)const
      {
         return rect.intersects(theBoundingRect);
      }
   virtual ossimAnnotationObject* getNewClippedObject(const ossimDrect& rect)const;

   virtual std::ostream& print(std::ostream& out)const;
   
   /*!
    * Will retrieve the bound rectangle for this
    * object.
    */
   virtual void getBoundingRect(ossimDrect& rect)const;
   
   /*!
    * will compute the bounding rectangle.
    */
   virtual void computeBoundingRect();

   /*!
    * Sets the current bitmap font to use.
    */
   virtual void setFont(ossimGdFontPtr font);

   /*!
    * Set's the text string to render.
    */
   virtual void setText(const ossimString& text);

   /*!
    * Set's the text string to render.  This
    * will assume that the x,y passed in is
    * suppose to be the center.  We will compute
    * the upper left corner from this
    */
   virtual void setCenterText(const ossimDpt& center,
                              const ossimString& text);

   /*!
    * Set's the text string to render.  This
    * will assume that the x,y passed in is
    * suppose to be the upper left.  
    */
   virtual void setUpperLeftText(const ossimDpt& upperLeft,
                                 const ossimString& text);

   /*!
    * Sets the center position of the string.
    * will use this value to compute the actual
    * origin of upper left corner of the first
    * character.  Make sure you have the text
    * set before you call this method
    */
   virtual void setCenterTextPosition(const ossimDpt& position);

   /*!
    * The actual upper left corner of the character.
    */
   virtual void setUpperLeftTextPosition(const ossimDpt& position);

   /*!
    * Will determine if the point is within the font
    */
   virtual bool isPointWithin(const ossimDpt& imagePoint)const;
   
protected:
   ossimGdFontPtr theFontPtr;
   ossimString    theText;
   ossimDrect     theBoundingRect;

   /*!
    * This position is suppose to be the upperleft
    * corner of the first character in the string
    */
   ossimDpt       thePosition;
};

#endif
