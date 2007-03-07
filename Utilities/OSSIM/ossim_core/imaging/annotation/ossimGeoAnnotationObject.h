//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimGeoAnnotationObject.h,v 1.4 2005/12/07 19:05:11 dburken Exp $
#ifndef ossimGeoAnnotationObject_HEADER
#define ossimGeoAnnotationObject_HEADER
#include <imaging/annotation/ossimAnnotationObject.h>
#include <base/data_types/ossimGpt.h>

class ossimProjection;

class OSSIM_DLL ossimGeoAnnotationObject : public ossimAnnotationObject
{
public:
   ossimGeoAnnotationObject(unsigned char r=255,
                            unsigned char g=255,
                            unsigned char b=255,
                            long thickness=1);

   ossimGeoAnnotationObject(const ossimGeoAnnotationObject& rhs);

   virtual ~ossimGeoAnnotationObject();
   
   /*!
    * Add the projection interface to all
    * geographically defined objects.
    */
   virtual void transform(ossimProjection* projection)=0;

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

TYPE_DATA
};

#endif
