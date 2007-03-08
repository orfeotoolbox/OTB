//----------------------------------------------------------------------------
// 
// See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: The factory registry for overview builders.
//
//----------------------------------------------------------------------------
// $Id: ossimOverviewBuilderFactoryRegistry.h 9930 2006-11-22 19:23:40Z dburken $

#ifndef ossimOverviewBuilderFactoryRegistry_HEADER
#define ossimOverviewBuilderFactoryRegistry_HEADER

#include <vector>

#include <ossim/base/ossimConstants.h>

class ossimOverviewBuilderFactoryBase;
class ossimOverviewBuilderBase;
class ossimString;

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
class OSSIM_DLL ossimOverviewBuilderFactoryRegistry
{
public:

   /**
    * @brief instance method for access to theInstance pointer.
    * @return Pointer to this class.
    */
   static ossimOverviewBuilderFactoryRegistry* instance();

   /**
    * @brief Method to register a factory.
    * @param factory The factory to register.
    * @param pushToFrontFlag If true, factory will be pushed to the front of
    * the factory list. default = false.
    * @return true on success, false on error.
    */
   bool registerFactory(ossimOverviewBuilderFactoryBase* factory,
                        bool pushToFrontFlag = false);

   /**
    * @brief Method to unregistrer a factory.
    * @param factory The factory to unregister.
    */
   void unregisterFactory(ossimOverviewBuilderFactoryBase* factory);

   /**
    * @brief Creates a builder from a string.  This should match a string from
    * the getTypeNameList() method.
    * @return Pointer to ossimOverviewBuilderInterface or NULL is not found
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
   void getTypeNameList(std::vector<ossimString>& typeList)const;

private:
   /** default constructor hidden from use */
   ossimOverviewBuilderFactoryRegistry();

   /** copy constructor hidden from use */
   ossimOverviewBuilderFactoryRegistry(const ossimOverviewBuilderFactoryRegistry& obj);

   /** operator= hidden from use. */
   void operator=(const ossimOverviewBuilderFactoryRegistry& rhs);

   /** The static instance of this class. */
   static ossimOverviewBuilderFactoryRegistry* theInstance;

   /** The list of factories. */
   std::vector<ossimOverviewBuilderFactoryBase*> theFactoryList;
   
};

#endif /* #ifndef ossimOverviewBuilderFactoryRegistry_HEADER */
