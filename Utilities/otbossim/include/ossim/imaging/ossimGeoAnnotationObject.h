//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimGeoAnnotationObject.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimGeoAnnotationObject_HEADER
#define ossimGeoAnnotationObject_HEADER
#include <ossim/imaging/ossimAnnotationObject.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/imaging/ossimImageGeometry.h>

class ossimImageProjectionModel;

class OSSIM_DLL ossimGeoAnnotationObject : public ossimAnnotationObject
{
public:
   ossimGeoAnnotationObject(unsigned char r=255,
                            unsigned char g=255,
                            unsigned char b=255,
                            long thickness=1);

   ossimGeoAnnotationObject(const ossimGeoAnnotationObject& rhs);

   
   /*!
    * Add the projection interface to all
    * geographically defined objects.
    */
   virtual void transform(ossimImageGeometry* projection)=0;

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
   virtual ~ossimGeoAnnotationObject();

TYPE_DATA
};

#endif
