//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks)
//
//*************************************************************************
// $Id: ossimViewInterface.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimViewInterface_HEADER
#define ossimViewInterface_HEADER
#include <ossim/base/ossimRtti.h>

class ossimObject;

class OSSIMDLLEXPORT ossimViewInterface
{
public:
   ossimViewInterface();
   ossimViewInterface(ossimObject* base);
   virtual ~ossimViewInterface();

   /*!
    * The derived classes should overrid this method and
    * return true or false whether it was able to cast the
    * past in view to a view that it can interface to.
    * So if you are a projective view you might
    * cast to an ossimProjection.  The ownsTheView specifies
    * whether or not it owns the passed in view pointer.
    * This will specifiy whether or not the derived classes
    * need to re-allocate the pointer or just delete its old
    * pointer and set it to the passed in pointer.
    */
   virtual bool setView(ossimObject* baseObject)=0;
   virtual ossimObject* getView()=0;
   virtual const ossimObject* getView()const=0;
   virtual void refreshView();
   ossimObject* getBaseObject();

protected:
   ossimObject* theObject;
TYPE_DATA
   
};

#endif
