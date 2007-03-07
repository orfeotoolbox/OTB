//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimGeoAnnotationEllipseObject.h,v 1.12 2005/12/09 21:49:02 dburken Exp $
#ifndef ossimGeoAnnotationEllipseObject_HEADER
#define ossimGeoAnnotationEllipseObject_HEADER
#include <imaging/annotation/ossimGeoAnnotationObject.h>

class ossimAnnotationEllipseObject;

class OSSIMDLLEXPORT ossimGeoAnnotationEllipseObject : public ossimGeoAnnotationObject
{
public:
   
   /*!
    * It is important to note that the width and height
    * should be specified in width = deltaLon and
    * height = deltaLat.  Use the ossimDpt .lat and .lon
    * attributes to set it.
    */
   ossimGeoAnnotationEllipseObject(const ossimGpt& center = ossimGpt(0,0),
                                   const ossimDpt& widthHeight = ossimDpt(0,0),
                                   bool enableFill = false,
                                   ossim_uint8 r = 255,
                                   ossim_uint8 g = 255,
                                   ossim_uint8 b = 255,
                                   ossim_uint8 thickness = 1);
   ossimGeoAnnotationEllipseObject(const ossimGeoAnnotationEllipseObject& rhs);
   virtual ~ossimGeoAnnotationEllipseObject();

   virtual ossimObject* dup()const;

   virtual void applyScale(double x, double y);
   virtual std::ostream& print(std::ostream& out)const;
   virtual void draw(ossimRgbImage& anImage)const;
   virtual bool intersects(const ossimDrect& rect)const;
   virtual ossimAnnotationObject* getNewClippedObject(const ossimDrect& rect)const;
   virtual void getBoundingRect(ossimDrect& rect)const;
   virtual void computeBoundingRect();
   virtual void transform(ossimProjection* projection);

   /**
    * Saves the current state of this object.
    *
    * Keywords:
    *
    * center:  ( 42.403388263727003, -90.708557648541046, 0.0, WGE )
    *
    * ellipse_width: 500
    *
    * ellipse_height: 500
    *
    * units: meters
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   virtual void setEllipseWidthHeightUnitType(ossimUnitType type);

protected:

   void getWidthHeightInPixels(ossimDpt& widthHeight,
                               const ossimProjection* projection) const;
   
   ossimAnnotationEllipseObject* theProjectedEllipse;
   ossimGpt                      theCenter;
   ossimDpt                      theWidthHeight;

   ossimUnitType                 theEllipseWidthHeightUnitType;
   
TYPE_DATA   
};

#endif
