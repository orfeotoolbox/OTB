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
// $Id: ossimGeoAnnotationGdBitmapFont.cpp,v 1.8 2004/05/23 17:49:37 dburken Exp $
#include "ossimGeoAnnotationGdBitmapFont.h"
#include "ossimAnnotationGdBitmapFont.h"
#include "projections/ossimProjection.h"

RTTI_DEF1(ossimGeoAnnotationGdBitmapFont, "ossimGeoAnnotationGdBitmapFont", ossimGeoAnnotationObject)

ossimGeoAnnotationGdBitmapFont::ossimGeoAnnotationGdBitmapFont(const ossimGpt& position,
                                                               const ossimString &text,
                                                               ossimGdFontPtr font,
                                                               unsigned char r,
                                                               unsigned char g,
                                                               unsigned char b,
                                                               long thickness)
   : ossimGeoAnnotationObject(r, g, b, thickness),
     thePosition(position)
{
   theProjectedFont = new ossimAnnotationGdBitmapFont(ossimDpt(0,0),
                                                      text,
                                                      font,
                                                      r,
                                                      g,
                                                      b,
                                                      thickness);
}

ossimGeoAnnotationGdBitmapFont::ossimGeoAnnotationGdBitmapFont(const ossimGeoAnnotationGdBitmapFont& rhs)
   :ossimGeoAnnotationObject(rhs),
    theProjectedFont(rhs.theProjectedFont?(ossimAnnotationGdBitmapFont*)theProjectedFont->dup():(ossimAnnotationGdBitmapFont*)NULL),
    thePosition(rhs.thePosition)
{
}


ossimGeoAnnotationGdBitmapFont::~ossimGeoAnnotationGdBitmapFont()
{
   if(theProjectedFont)
   {
      delete theProjectedFont;
      theProjectedFont=NULL;
   }
}

void ossimGeoAnnotationGdBitmapFont::applyScale(double x, double y)
{
   thePosition.lond(thePosition.lond()*x);
   thePosition.latd(thePosition.latd()*y);

   if(theProjectedFont)
   {
      theProjectedFont->applyScale(x, y);
   }
}

std::ostream& ossimGeoAnnotationGdBitmapFont::print(std::ostream& out)const
{
   out << "position:     " << thePosition;
   theProjectedFont->print(out);
   return out;
}

void ossimGeoAnnotationGdBitmapFont::draw(ossimRgbImage& anImage)const
{
   theProjectedFont->draw(anImage);
}

void ossimGeoAnnotationGdBitmapFont::getBoundingRect(ossimDrect& rect)const
{
   theProjectedFont->getBoundingRect(rect);
}

bool ossimGeoAnnotationGdBitmapFont::intersects(const ossimDrect& rect)const
{
   if(theProjectedFont)
   {
      return theProjectedFont->intersects(rect);
   }

   return false;
}

ossimAnnotationObject* ossimGeoAnnotationGdBitmapFont::getNewClippedObject(const ossimDrect& rect)const
{
   if(intersects(rect))
   {
      if(theProjectedFont)
      {
         return theProjectedFont->getNewClippedObject(rect);
      }
   }
   
   return (ossimAnnotationObject*)NULL;
}

void ossimGeoAnnotationGdBitmapFont::computeBoundingRect()
{
   theProjectedFont->computeBoundingRect();
}

void ossimGeoAnnotationGdBitmapFont::transform(ossimProjection* projection)
{
   ossimDpt position;

   projection->worldToLineSample(thePosition, position);
   
   theProjectedFont->setUpperLeftTextPosition(position);
}

void ossimGeoAnnotationGdBitmapFont::setText(const ossimString& text)
{
   theProjectedFont->setText(text);
}

void ossimGeoAnnotationGdBitmapFont::setFont(ossimGdFontPtr font)
{
   theProjectedFont->setFont(font);
}
