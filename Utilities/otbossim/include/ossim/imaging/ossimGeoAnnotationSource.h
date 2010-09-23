//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author: Garrett Potts
//
//*******************************************************************
// $Id: ossimGeoAnnotationSource.h 17932 2010-08-19 20:34:35Z dburken $
#ifndef ossimGeoAnnotationSource_HEADER
#define ossimGeoAnnotationSource_HEADER

#include <iostream>
#include <ossim/imaging/ossimAnnotationSource.h>
#include <ossim/imaging/ossimImageGeometry.h>
#include <ossim/base/ossimViewInterface.h>

class ossimGeoAnnotationObject;
class ossimProjection;

class OSSIM_DLL ossimGeoAnnotationSource :
   public ossimAnnotationSource,
   public ossimViewInterface
{
public:
   friend std::ostream& operator <<(std::ostream& out, const ossimGeoAnnotationSource& rhs);
   ossimGeoAnnotationSource(ossimImageGeometry* geom=NULL,
                            bool ownsProjectionFlag=false);

   ossimGeoAnnotationSource(ossimImageSource* inputSource,
                            ossimImageGeometry* geom=NULL,
                            bool ownsProjectionFlag=false);

   
   /*!
    * Will add an object to the list.
    * it will own the object added.  If the
    * object is not geographic 
    */
   virtual bool addObject(ossimAnnotationObject* anObject);
   virtual void computeBoundingRect();
   virtual void transformObjects(ossimImageGeometry* geom=0);
   virtual void setGeometry(ossimImageGeometry* projection);
   virtual bool setView(ossimObject* baseObject);
   virtual ossimRefPtr<ossimImageGeometry> getImageGeometry();
  
   virtual ossimObject*       getView();
   virtual const ossimObject* getView()const;

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
   virtual ~ossimGeoAnnotationSource();
   
   ossimRefPtr<ossimImageGeometry> m_geometry;
   
TYPE_DATA
};

#endif
