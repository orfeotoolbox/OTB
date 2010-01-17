//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks)
// Description:
//
//*************************************************************************
// $Id: ossimGeoAnnotationGdBitmapFont.cpp 15766 2009-10-20 12:37:09Z gpotts $
#include <ossim/imaging/ossimGeoAnnotationGdBitmapFont.h>
#include <ossim/imaging/ossimAnnotationGdBitmapFont.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimImageProjectionModel.h>
#include <ossim/base/ossimException.h>

RTTI_DEF1(ossimGeoAnnotationGdBitmapFont,
          "ossimGeoAnnotationGdBitmapFont",
          ossimGeoAnnotationObject)

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
    theProjectedFont(rhs.theProjectedFont.valid()?(ossimAnnotationGdBitmapFont*)theProjectedFont->dup():(ossimAnnotationGdBitmapFont*)0),
    thePosition(rhs.thePosition)
{
}

ossimGeoAnnotationGdBitmapFont::~ossimGeoAnnotationGdBitmapFont()
{
   theProjectedFont=0;
}

ossimObject* ossimGeoAnnotationGdBitmapFont::dup()const
{
   return new ossimGeoAnnotationGdBitmapFont(*this);
}

void ossimGeoAnnotationGdBitmapFont::applyScale(double x, double y)
{
   thePosition.lond(thePosition.lond()*x);
   thePosition.latd(thePosition.latd()*y);

   if(theProjectedFont.valid())
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
   if(theProjectedFont.valid())
   {
      return theProjectedFont->intersects(rect);
   }

   return false;
}

ossimAnnotationObject* ossimGeoAnnotationGdBitmapFont::getNewClippedObject(const ossimDrect& rect)const
{
   if(intersects(rect))
   {
      if(theProjectedFont.valid())
      {
         return theProjectedFont->getNewClippedObject(rect);
      }
   }
   
   return (ossimAnnotationObject*)0;
}

void ossimGeoAnnotationGdBitmapFont::computeBoundingRect()
{
   theProjectedFont->computeBoundingRect();
}

void ossimGeoAnnotationGdBitmapFont::transform(ossimImageGeometry* projection)
{
   ossimDpt position;
   projection->worldToLocal(thePosition, position);
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
