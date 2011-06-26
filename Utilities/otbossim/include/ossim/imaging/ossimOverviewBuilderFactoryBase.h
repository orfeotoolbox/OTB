//----------------------------------------------------------------------------
// 
// See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: The base class for overview builders.
//
//----------------------------------------------------------------------------
// $Id: ossimOverviewBuilderFactoryBase.h 19187 2011-03-23 12:21:58Z gpotts $

#ifndef ossimOverviewBuilderFactoryBase_HEADER
#define ossimOverviewBuilderFactoryBase_HEADER

#include <vector>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimObjectFactory.h>

class ossimOverviewBuilderBase;
class ossimString;

/**
 * @class ossimOverviewBuilderFactoryBase
 * @brief The base class for factory overview builder factories.
 */
class OSSIM_DLL ossimOverviewBuilderFactoryBase : public ossimObjectFactory
{
public:
   /** default constructor hidden from use */
   ossimOverviewBuilderFactoryBase();

   /** virtual destructor */
   virtual ~ossimOverviewBuilderFactoryBase();

   /**
    * @brief Creates a builder from a string.  This should match a string from
    * the getTypeNameList() method.  Pure virtual.
    * 
    * @return Pointer to ossimOverviewBuilderBase or NULL is not found
    * within registered factories.
    */
   virtual ossimOverviewBuilderBase* createBuilder(const ossimString& typeName) const = 0;

   /**
    * @brief Method to populate a list of supported types for the factory.
    * registered to this registry.  Pure virtual.
    *
    * @param typeList List of ossimStrings to add to.
    */
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const = 0;

   /*!
    * Creates an object given a type name.
    */
   virtual ossimObject* createObject(const ossimString& typeName)const;
   
   /*!
    * Creates and object given a keyword list.
    */
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const;
   
   
};

#endif /* #ifndef ossimOverviewBuilderFactoryBase_HEADER */
