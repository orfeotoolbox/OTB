//----------------------------------------------------------------------------
// 
// See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: The ossim overview builder factory.
//
//----------------------------------------------------------------------------
// $Id: ossimOverviewBuilderFactory.h 15833 2009-10-29 01:41:53Z eshirschorn $

#ifndef ossimOverviewBuilderFactory_HEADER
#define ossimOverviewBuilderFactory_HEADER

#include <ossim/base/ossimConstants.h>
#include <ossim/imaging/ossimOverviewBuilderFactoryBase.h>


class ossimOverviewBuilderBase;
class ossimString;

/**
 * @class ossimOverviewBuilderFactory
 * @brief The ossim overview builder factory.
 */
class OSSIM_DLL ossimOverviewBuilderFactory : public ossimOverviewBuilderFactoryBase
{
public:

   /** @brief static instance method. */
   static ossimOverviewBuilderFactory* instance();
   
   /** virtual destructor */
   virtual ~ossimOverviewBuilderFactory();

   /**
    * @brief Creates a builder from a string.  This should match a string from
    * the getTypeNameList() method.  Pure virtual.
    * 
    * @return Pointer to ossimOverviewBuilderBase or NULL is not found
    * within registered factories.
    */
   virtual ossimOverviewBuilderBase* createBuilder(
      const ossimString& typeName) const;

   /**
    * @brief Method to populate a list of supported types for the factory.
    * registered to this registry.  Pure virtual.
    *
    * @param typeList List of ossimStrings to add to.
    */
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;

private:
   /** default constructor hidden from use */
   ossimOverviewBuilderFactory();

   /** copy constructor hidden from use */
   ossimOverviewBuilderFactory(const ossimOverviewBuilderFactory& obj);

   /** operator= hidden from use. */
   void operator=(const ossimOverviewBuilderFactory& rhs);

   static ossimOverviewBuilderFactory* theInstance;
};

#endif /* #ifndef ossimOverviewBuilderFactory_HEADER */
