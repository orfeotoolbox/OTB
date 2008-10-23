//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
// Description:
//
//*************************************************************************
// $Id: ossimGeoAnnotationGdBitmapFont.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimGeoAnnotationGdBitmapFont_HEADER
#define ossimGeoAnnotationGdBitmapFont_HEADER

#include <ossim/imaging/ossimGeoAnnotationObject.h>
#include <ossim/imaging/ossimGdFont.h>
#include <ossim/imaging/ossimGdFontExterns.h>
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
