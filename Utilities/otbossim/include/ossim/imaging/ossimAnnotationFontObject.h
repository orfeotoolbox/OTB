//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimAnnotationFontObject.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimAnnotationFontObject_HEADER
#define ossimAnnotationFontObject_HEADER
#include <ossim/imaging/ossimAnnotationObject.h>
#include <ossim/base/ossimFontInformation.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/font/ossimFont.h>

/**
 * Class to draw fonts onto an image.
 * @note  This class requires a font path to be set for the factory:
 * "ossimFontFactoryRegistry".  You should set at a minimum the keyword
 * "font.dir1" in your ossim_prefernces file.
 * Example from a RedHat 9.0 system:
 * font.dir1: /usr/share/fonts/default/Type1
 *
 * @see file ossim/etc/templates/ossim_prefernces_template
 */
class OSSIMDLLEXPORT ossimAnnotationFontObject : public ossimAnnotationObject
{

public:
  friend class ossimGeoAnnotationFontObject;
   ossimAnnotationFontObject();
   ossimAnnotationFontObject(const ossimIpt& upperLeft,
                             const ossimString& s,
                             const ossimIpt& pixelSize = ossimIpt(12, 12),
                             double rotation = 0.0,
                             const ossimDpt& scale = ossimDpt(1.0, 1.0),
                             const ossimDpt& shear = ossimDpt(0.0,0.0),
                             unsigned char r=255,
                             unsigned char g=255,
                             unsigned char b=255);
   ossimAnnotationFontObject(const ossimAnnotationFontObject& rhs);
                             
   

   virtual ossimObject* dup()const;

   virtual void draw(ossimRgbImage& anImage)const;
   virtual std::ostream& print(std::ostream& out)const;
   virtual void getBoundingRect(ossimDrect& rect)const;
   virtual void setString(const ossimString& s);
   virtual ossimString getString()const;
   virtual void computeBoundingRect();
   virtual ossimAnnotationObject* getNewClippedObject(const ossimDrect& rect)const;
   virtual bool intersects(const ossimDrect& rect)const;
   virtual bool isPointWithin(const ossimDpt& imagePoint)const;
   virtual void setFont(ossimFont* font);
   virtual void setCenterPosition(const ossimIpt& position);
   virtual void setUpperLeftPosition(const ossimIpt& position);
   virtual void setPointSize(const ossimIpt& size);
   virtual void setRotation(double rotation);
   virtual void setScale(const ossimDpt& scale);
   virtual void setShear(const ossimDpt& shear);
   virtual void setGeometryInformation(const ossimFontInformation& info);
   virtual void applyScale(double x, double y);
                           
protected:
   virtual ~ossimAnnotationFontObject();
   mutable ossimRefPtr<ossimFont>   theFont;
   ossimIpt             thePosition;
   ossimString          theString;
   ossimIpt             thePixelSize;
   double               theRotation;
   double               theHorizontalScale;
   double               theVerticalScale;
   double               theHorizontalShear;
   double               theVerticalShear;
   ossimIrect           theBoundingRect;

   void setFontInfo()const;
TYPE_DATA   
};

#endif
