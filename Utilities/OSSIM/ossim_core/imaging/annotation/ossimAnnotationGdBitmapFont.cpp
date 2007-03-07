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
// $Id: ossimAnnotationGdBitmapFont.cpp,v 1.11 2004/05/23 17:49:36 dburken Exp $
#include "ossimAnnotationGdBitmapFont.h"
#include "imaging/ossimRgbImage.h"
#include "base/common/ossimCommon.h"

#include "gdfonts.inc"
#include "gdfontl.inc"
#include "gdfontg.inc"
#include "gdfontmb.inc"
#include "gdfontt.inc"

ossimAnnotationGdBitmapFont::ossimAnnotationGdBitmapFont(const ossimDpt& position,
                                                         const ossimString &text,
                                                         ossimGdFontPtr font,
                                                         unsigned char r,
                                                         unsigned char g,
                                                         unsigned char b,
                                                         long thickness)
   :ossimAnnotationObject(r, g, b, thickness),
    theFontPtr(font),
    theText(text),
    thePosition(position)
{
   computeBoundingRect();
}

ossimAnnotationGdBitmapFont::ossimAnnotationGdBitmapFont(const ossimAnnotationGdBitmapFont& rhs)
   : ossimAnnotationObject(rhs),
     theFontPtr(rhs.theFontPtr),
     theText(rhs.theText),
     theBoundingRect(rhs.theBoundingRect),
     thePosition(rhs.thePosition)
{
}

ossimAnnotationGdBitmapFont::~ossimAnnotationGdBitmapFont()
{
}

void ossimAnnotationGdBitmapFont::getBoundingRect(ossimDrect& rect)const
{
   rect = theBoundingRect;
}

void ossimAnnotationGdBitmapFont::applyScale(double x, double y)
{
   thePosition.x *= x;
   thePosition.y *= y;

   theBoundingRect *= ossimDpt(x, y);
}

void ossimAnnotationGdBitmapFont::draw(ossimRgbImage& anImage)const
{
   if(!theFontPtr||
   !anImage.getImageData())
   {
      return;
   }
   ossimDpt origin = anImage.getImageData()->getOrigin();

   ossimDrect rect = anImage.getImageData()->getImageRectangle();

   if(theBoundingRect.intersects(rect))
   {
      anImage.setDrawColor(theRed, theGreen, theBlue);
      ossimDpt position(thePosition.x - origin.x,
                        thePosition.y - origin.y);
      for(ossim_uint32 character = 0; character < theText.size(); ++character)
      {
         long charOffset = theText[(int)character];

         // we need to shift the offset to a positive value.
         // if it goes negative we just add 256 to the value.
         if(charOffset < 0)
         {
            charOffset += 256;
         }
         // get the starting offset to the bitmap
         long offset = charOffset*theFontPtr->w*theFontPtr->h;
         for(long row = 0; row < theFontPtr->h; ++row)
         {
            for(long col = 0; col < theFontPtr->w; ++col)
            {
               if(theFontPtr->data[(int)offset])
               {
                  anImage.slowPlotPixel(irint(position.x + col),
                                        irint(position.y + row),
                                        theRed,
                                        theGreen,
                                        theBlue);
               }
               ++offset;
            }
         }
         position.x += theFontPtr->w;
      }
   }
}

ossimAnnotationObject* ossimAnnotationGdBitmapFont::getNewClippedObject(const ossimDrect& rect)const
{
   ossimAnnotationGdBitmapFont* result = (ossimAnnotationGdBitmapFont*)NULL;

   if(intersects(rect))
   {
      result = (ossimAnnotationGdBitmapFont*)dup();
   }

   return result;
}

std::ostream& ossimAnnotationGdBitmapFont::print(std::ostream& out)const
{
   out << "text_value:        " << theText << endl
       << "text_bouding_rect: " << theBoundingRect << endl
       << "text_position:     " << thePosition << endl;
   return out;
}

void ossimAnnotationGdBitmapFont::computeBoundingRect()
{
   if(theFontPtr)
   {
      theBoundingRect = ossimDrect(thePosition.x,
                                   thePosition.y,
                                   thePosition.x + theText.size()*theFontPtr->w,
                                   thePosition.y + theFontPtr->h);
   }
}

void ossimAnnotationGdBitmapFont::setFont(ossimGdFontPtr font)
{
   theFontPtr = font;
}

void ossimAnnotationGdBitmapFont::setText(const ossimString& text)
{
   theText = text;
   computeBoundingRect();
}

void ossimAnnotationGdBitmapFont::setUpperLeftText(const ossimDpt& upperLeft,
                                                   const ossimString& text)
{
   setText(text);
   setUpperLeftTextPosition(upperLeft);
}

void ossimAnnotationGdBitmapFont::setUpperLeftTextPosition(const ossimDpt& position)
{
   thePosition = position;
   computeBoundingRect();
}

void ossimAnnotationGdBitmapFont::setCenterText(const ossimDpt& upperLeft,
                                                   const ossimString& text)
{
   setText(text);
   setCenterTextPosition(upperLeft);
}

void ossimAnnotationGdBitmapFont::setCenterTextPosition(const ossimDpt& position)
{
   thePosition.x = position.x - theBoundingRect.width()/2.0;
   thePosition.y = position.y - theBoundingRect.height()/2.0;
   computeBoundingRect();
}

bool ossimAnnotationGdBitmapFont::isPointWithin(const ossimDpt& imagePoint)const
{
   return theBoundingRect.pointWithin(imagePoint);
}
