//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author: Garrett Potts
//
//*******************************************************************
// $Id: ossimGeoAnnotationSource.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimGeoAnnotationSource_HEADER
#define ossimGeoAnnotationSource_HEADER

#include <iostream>
#include <ossim/imaging/ossimAnnotationSource.h>
#include <ossim/base/ossimViewInterface.h>

class ossimGeoAnnotationObject;
class ossimProjection;

class OSSIM_DLL ossimGeoAnnotationSource :
   public ossimAnnotationSource,
   public ossimViewInterface
{
public:
   friend std::ostream& operator <<(std::ostream& out, const ossimGeoAnnotationSource& rhs);
   ossimGeoAnnotationSource(ossimProjection* projection=NULL,
                            bool ownsProjectionFlag=false);

   ossimGeoAnnotationSource(ossimImageSource* inputSource,
                            ossimProjection* projection=NULL,
                            bool ownsProjectionFlag=false);

   virtual ~ossimGeoAnnotationSource();
   
   /*!
    * Will add an object to the list.
    * it will own the object added.  If the
    * object is not geographic 
    */
   virtual bool addObject(ossimAnnotationObject* anObject);
   virtual void computeBoundingRect();
   virtual void transformObjects(ossimProjection* projection=0);
   virtual void setProjection(ossimProjection* projection,
                              bool ownsProjectionFlag=false);
   virtual bool setView(ossimObject* baseObject,
                        bool ownsTheView = false);
    virtual bool getImageGeometry(ossimKeywordlist& kwl,
                                 const char* prefix=0);
  
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
   ossimProjection* theProjection;
   bool           theOwnsProjectionFlag;
   
   void removeProjection();
   
TYPE_DATA
};

#endif
