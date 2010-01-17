//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks)
// Description:
//
//*************************************************************************
// $Id: ossimAnnotationObjectFactory.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimAnnotationObjectFactory_HEADER
#define ossimAnnotationObjectFactory_HEADER
#include <ossim/base/ossimFactoryBaseTemplate.h>
#include <ossim/imaging/ossimAnnotationObject.h>
class ossimAnnotationObjectFactory : public ossimFactoryBase<ossimAnnotationObject>
{
public:
   /*!
    * METHOD: instance()
    * For accessing static instance of concrete factory.
    */
   static ossimAnnotationObjectFactory* instance();
   
   /*!
    * METHOD: create()
    * Attempts to create an instance of the Product given a Specifier or
    * keywordlist. Returns successfully constructed product or NULL.
    */
   virtual ossimAnnotationObject* create(const ossimString& spec) const;
   virtual ossimAnnotationObject* create(const ossimKeywordlist& kwl,
                                         const char* prefix=0)    const;

   /*!
    * METHOD: getList()
    * Returns name list of all products represented by this factory:
    */
   virtual list<ossimString> getList() const;

protected:
   ossimAnnotationObjectFactory();
   static ossimAnnotationObjectFactory*  theInstance;
};

#endif
