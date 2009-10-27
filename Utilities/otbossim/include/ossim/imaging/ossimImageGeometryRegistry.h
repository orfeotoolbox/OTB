//**************************************************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Description: Class declaration of ossimImageGeometryRegistry. See .h file for class documentation.
//
//**************************************************************************************************
// $Id$
#ifndef ossimImageGeometryRegistry_HEADER
#define ossimImageGeometryRegistry_HEADER
#include <ossim/base/ossimConstants.h>
#include <ossim/imaging/ossimImageGeometryFactoryBase.h>
#include <ossim/imaging/ossimImageGeometry.h>
#include <ossim/base/ossimFactoryListInterface.h>
class OSSIM_DLL ossimImageGeometryRegistry : public ossimBaseObjectFactory,
                                             public ossimFactoryListInterface<ossimImageGeometryFactoryBase,
                                                                              ossimImageGeometry>
{
public:
   virtual ~ossimImageGeometryRegistry(){m_instance=0;}
   static ossimImageGeometryRegistry* instance();
   
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
   
   virtual bool extendGeometry(ossimImageHandler* handler)const;
   
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
   ossimImageGeometryRegistry()
   :ossimBaseObjectFactory()
   {}
   
   ossimImageGeometryRegistry( const ossimImageGeometryRegistry& rhs )
   :ossimBaseObjectFactory(rhs)
   {}
   void operator =(const ossimImageGeometryRegistry&){}
   static ossimImageGeometryRegistry* m_instance;
   
   TYPE_DATA
};
#endif
