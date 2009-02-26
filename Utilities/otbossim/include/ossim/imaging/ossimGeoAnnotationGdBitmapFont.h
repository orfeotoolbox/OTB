//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
// Description:
//
//*************************************************************************
// $Id: ossimGeoAnnotationGdBitmapFont.h 13349 2008-07-30 15:34:34Z dburken $
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
   virtual ossimObject* dup()const;
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

   /**
    * @brief Transforms from geographic to image space for a
    * reduced resolution data set (rrds).
    *
    * This will transform any world points to line sample; then, convert any
    * line sample to the correct rrds point.
    *
    * @param model The model to use for transformation.
    *
    * @param rrds Reduced resolution data set to use.
    */
   virtual void transform(const ossimImageProjectionModel& model,
                          ossim_uint32 rrds);

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
