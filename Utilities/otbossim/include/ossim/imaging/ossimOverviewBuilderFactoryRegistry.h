//----------------------------------------------------------------------------
// 
// See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: The factory registry for overview builders.
//
//----------------------------------------------------------------------------
// $Id: ossimOverviewBuilderFactoryRegistry.h 19184 2011-03-23 11:59:03Z gpotts $

#ifndef ossimOverviewBuilderFactoryRegistry_HEADER
#define ossimOverviewBuilderFactoryRegistry_HEADER

#include <vector>

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimString.h>
#include <ossim/imaging/ossimOverviewBuilderFactoryBase.h>
#include <ossim/imaging/ossimOverviewBuilderBase.h>
#include <ossim/base/ossimFactoryListInterface.h>

/**
 * @class ossimOverviewBuilderFactoryRegistry
 * @brief The factory registry for overview builders.
 *
 * Typical usage:
 * std::vector<ossimString> typeList;
 * ossimOverviewBuilderFactoryRegistry::instance()->getTypeNameList(typeList);
 * ossimOverviewBuilderBase* builder =
 * ossimOverviewBuilderFactoryRegistry::instance()->createBuilder(
 * someStringFromTypeList);
 */
class OSSIM_DLL ossimOverviewBuilderFactoryRegistry : public ossimObjectFactory, 
                                                      public ossimFactoryListInterface<ossimOverviewBuilderFactoryBase, ossimOverviewBuilderBase>
{
public:

   /**
    * @brief instance method for access to theInstance pointer.
    * @return Pointer to this class.
    */
   static ossimOverviewBuilderFactoryRegistry* instance();


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
                                     const char* prefix=0)const;
   /**
    * @brief Creates a builder from a string.  This should match a string from
    * the getTypeNameList() method.
    * @return Pointer to ossimOverviewBuilderBase or NULL is not found
    * within registered factories.
    */
   ossimOverviewBuilderBase* createBuilder(const ossimString& typeName) const;

   /**
    * @brief Method to populate a list of supported types of all factories
    * registered to this registry.
    * 
    * Example:
    * 
    * ossimTiffOverviewNearest
    * ossimTiffOverviewBox
    *
    * @param typeList List of ossimStrings to add to.
    */
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const
   {
      getAllTypeNamesFromRegistry(typeList);
   }
   
private:
   /** default constructor hidden from use */
   ossimOverviewBuilderFactoryRegistry();

   /** copy constructor hidden from use */
   ossimOverviewBuilderFactoryRegistry(const ossimOverviewBuilderFactoryRegistry& obj);

   /** operator= hidden from use. */
   void operator=(const ossimOverviewBuilderFactoryRegistry& rhs);

   /** The static instance of this class. */
   static ossimOverviewBuilderFactoryRegistry* m_instance;

   
};

#endif /* #ifndef ossimOverviewBuilderFactoryRegistry_HEADER */
