//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks)
// Description:
//
//*************************************************************************
// $Id: ossimAnnotationGdBitmapFont.cpp 12276 2008-01-07 19:58:43Z dburken $
#include <ossim/imaging/ossimAnnotationGdBitmapFont.h>
#include <ossim/imaging/ossimRgbImage.h>
#include <ossim/base/ossimCommon.h>

#include <ossim/imaging/gdfonts.inc>
#include <ossim/imaging/gdfontl.inc>
#include <ossim/imaging/gdfontg.inc>
#include <ossim/imaging/gdfontmb.inc>
#include <ossim/imaging/gdfontt.inc>

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
      for(std::string::size_type character = 0;
          character < theText.size();
          ++character)
      {
         long charOffset = theText[character];

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
                  anImage.slowPlotPixel(ossim::round<int>(position.x + col),
                                        ossim::round<int>(position.y + row),
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
