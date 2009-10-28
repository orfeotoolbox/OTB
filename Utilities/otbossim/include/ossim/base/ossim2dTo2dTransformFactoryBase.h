#ifndef ossim2dTo2dTransformFactoryBase_HEADER
#define ossim2dTo2dTransformFactoryBase_HEADER
#include <ossim/base/ossimObjectFactory.h>
//**************************************************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Description: Class declaration of ossim2dTo2dTransformFactoryBase.
//
//**************************************************************************************************
// $Id$
#include <ossim/base/ossim2dTo2dTransform.h>

class OSSIM_DLL ossim2dTo2dTransformFactoryBase : public ossimObjectFactory
{
public:
   
   /**
    * Creates an object given a type name.
    */
   virtual ossimObject* createObject(const ossimString& typeName)const
   {
      return createTransform(typeName);
   }
   
   /**
    * Creates and object given a keyword list.
    */
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const
   {
      return createTransform(kwl, prefix);
   }
   
   /**
    * Take a transform type name.
    */
   virtual ossim2dTo2dTransform* createTransform(const ossimString& name)const=0;
   
   /**
    * Take a keywordlist.
    */
   virtual ossim2dTo2dTransform* createTransform(const ossimKeywordlist& kwl,
                                                 const char* prefix)const=0;
protected:
};
#endif
