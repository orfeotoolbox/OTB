//----------------------------------------------------------------------------
// 
// See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: The base class for overview builders.
//
//----------------------------------------------------------------------------
// $Id: ossimOverviewBuilderFactoryBase.h 9930 2006-11-22 19:23:40Z dburken $

#ifndef ossimOverviewBuilderFactoryBase_HEADER
#define ossimOverviewBuilderFactoryBase_HEADER

#include <vector>
#include <ossim/base/ossimConstants.h>

class ossimOverviewBuilderBase;
class ossimString;

/**
 * @class ossimOverviewBuilderFactoryBase
 * @brief The base class for factory overview builder factories.
 */
class OSSIM_DLL ossimOverviewBuilderFactoryBase
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
    * @return Pointer to ossimOverviewBuilderInterface or NULL is not found
    * within registered factories.
    */
   virtual ossimOverviewBuilderBase* createBuilder(
      const ossimString& typeName) const = 0;

   /**
    * @brief Method to populate a list of supported types for the factory.
    * registered to this registry.  Pure virtual.
    *
    * @param typeList List of ossimStrings to add to.
    */
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const = 0;
};

#endif /* #ifndef ossimOverviewBuilderFactoryBase_HEADER */
