//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimAnnotationObject.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimAnnotationObject_HEADER
#define ossimAnnotationObject_HEADER
#include <iostream>

#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/imaging/ossimRgbImage.h>
#include <ossim/base/ossimString.h>

class OSSIMDLLEXPORT ossimAnnotationObject : public ossimObject
{
public:
   ossimAnnotationObject(ossim_uint8 r=255,
                         ossim_uint8 g=255,
                         ossim_uint8 b=255,
                         ossim_uint8 thickness=1);
   virtual void applyScale(double x,
                           double y)=0;
   virtual void applyScale(const ossimDpt& scale);
   
   virtual std::ostream& print(std::ostream& out)const;

   virtual void draw(ossimRgbImage& anImage)const=0;
   virtual bool intersects(const ossimDrect& rect)const=0;

   /*!
    * Will allocate a new object that is clipped to the passed.
    * in rect.  Look at derived classes to see if the object is actually
    * clipped or not.  A new object is returned if it clips to the rect
    * else NULL should be returned it doesn't clip.
    */
   virtual ossimAnnotationObject* getNewClippedObject(const ossimDrect& rect)const=0;
   
   virtual void getBoundingRect(ossimDrect& rect)const=0;
   virtual ossimDrect getBoundingRect()const;

   virtual void computeBoundingRect()=0;
   
   virtual void setColor(ossim_uint8 r,
                         ossim_uint8 g,
                         ossim_uint8 b);

   void getColor(ossim_uint8 &r,
                 ossim_uint8 &g,
                 ossim_uint8 &b)const;

   ossim_uint8  getThickness()const;
   virtual void setThickness(ossim_uint8 thickness);
   
   /*!
    * This method allows you to pick an object.  It will
    * return true or false if the point falls somewhere
    * on an object.  Look at derived classes for further
    * information.
    */
   virtual bool isPointWithin(const ossimDpt& imagePoint) const;

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

   virtual ossimString getName() const;
   virtual void setName(const ossimString& name);

  ossim_uint32 getId()const;
  virtual ossim_uint32 setId();

  static ossim_uint32 getNextId();
  static void restIdCounter();

protected:
   virtual ~ossimAnnotationObject();
   ossim_uint8 theRed;
   ossim_uint8 theGreen;
   ossim_uint8 theBlue;
   ossim_uint8 theThickness;

   ossimString  theName;
   ossim_uint32 theId;

  static ossim_uint32 theIdCounter;
TYPE_DATA
};

#endif
