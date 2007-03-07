//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimAnnotationEllipseObject.h,v 1.12 2005/12/07 19:08:55 dburken Exp $
#ifndef ossimAnnotationEllipseObject_HEADER
#define ossimAnnotationEllipseObject_HEADER

#include <imaging/annotation/ossimAnnotationObject.h>

class OSSIMDLLEXPORT ossimAnnotationEllipseObject : public ossimAnnotationObject
{
public:
   ossimAnnotationEllipseObject(const ossimDpt& center = ossimDpt(0,0),
                                const ossimDpt& widthHeight = ossimDpt(1,1),
                                bool enableFill = false,
                                ossim_uint8 r = 255,
                                ossim_uint8 g = 255,
                                ossim_uint8 b = 255,
                                ossim_uint8 thickness = 1);
   ossimAnnotationEllipseObject(const ossimAnnotationEllipseObject& rhs);

   virtual ossimObject* dup()const;

   virtual ~ossimAnnotationEllipseObject();

   virtual void applyScale(double x, double y);

   virtual void draw(ossimRgbImage& anImage)const;

   virtual bool intersects(const ossimDrect& rect)const;

   virtual ossimAnnotationObject* getNewClippedObject(
      const ossimDrect& rect)const;
   
   virtual std::ostream& print(std::ostream& out)const;
   virtual void getBoundingRect(ossimDrect& rect)const;
   virtual void computeBoundingRect();
  /*!
   */
   virtual bool isPointWithin(const ossimDpt& imagePoint)const;
   virtual void setCenterWidthHeight(const ossimDpt& center,
                                     const ossimDpt& widthHeight);
   virtual void setFill(bool enabled);

   /** @return theFillEnabled flag. */
   virtual bool getFill() const;

   /**
    * Saves the current state of this object.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   
protected:
   ossimDpt   theCenter;
   ossimDpt   theWidthHeight;
   bool       theFillEnabled;
   ossimDrect theBoundingRect;

TYPE_DATA
};

#endif
