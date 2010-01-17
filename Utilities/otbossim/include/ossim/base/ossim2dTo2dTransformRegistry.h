//**************************************************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Description: Class declaration of ossim2dTo2dTransformRegistry.
//
//**************************************************************************************************
// $Id$
#ifndef ossim2dTo2dTransformRegistry_HEADER
#define ossim2dTo2dTransformRegistry_HEADER
#include <ossim/base/ossimObjectFactory.h>
#include <ossim/base/ossimRtti.h>
#include <ossim/base/ossim2dTo2dTransformFactoryBase.h>
#include <ossim/base/ossim2dTo2dTransform.h>
#include <vector>
#include <ossim/base/ossimFactoryListInterface.h>

class OSSIM_DLL ossim2dTo2dTransformRegistry : public ossimObjectFactory,
                                               public ossimFactoryListInterface<ossim2dTo2dTransformFactoryBase,
                                                                                ossim2dTo2dTransform>
{
public:
   virtual ~ossim2dTo2dTransformRegistry(){}
   static ossim2dTo2dTransformRegistry* instance();
   
   /*!
    * Creates an object given a type name.
    */
   virtual ossimObject* createObject(const ossimString& typeName)const
   {
      return createObjectFromRegistry(typeName);
   }
   
   /*!
    * Creates and object given a keyword list.
    */
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const
   {
      return createObjectFromRegistry(kwl, prefix);
   }
   /*!
    * Creates an object given a type name.
    */
   virtual ossim2dTo2dTransform* createTransform(const ossimString& typeName)const
   {
      return createNativeObjectFromRegistry(typeName);
   }
   
   /*!
    * Creates and object given a keyword list.
    */
   virtual ossim2dTo2dTransform* createTransform(const ossimKeywordlist& kwl,
                                                 const char* prefix=0)const
   {
      return createNativeObjectFromRegistry(kwl, prefix);
   }
   
   /*!
    * This should return the type name of all objects in all factories.
    * This is the name used to construct the objects dynamially and this
    * name must be unique.
    */
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const
   {
      getAllTypeNamesFromRegistry(typeList);
   }
   
protected:
   ossim2dTo2dTransformRegistry()
   :ossimObjectFactory()
   {}
   
   ossim2dTo2dTransformRegistry( const ossim2dTo2dTransformRegistry& rhs )
   :ossimObjectFactory(rhs)
   {}
   void operator =(const ossim2dTo2dTransformRegistry&){}
   static ossim2dTo2dTransformRegistry* m_instance;
   
   TYPE_DATA
};
#endif
