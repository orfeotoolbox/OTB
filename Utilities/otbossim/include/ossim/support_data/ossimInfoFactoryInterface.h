//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Interface for info factories.
// 
//----------------------------------------------------------------------------
// $Id$
#ifndef ossimInfoFactoryInterface_HEADER
#define ossimInfoFactoryInterface_HEADER

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimObjectFactory.h>

class ossimFilename;
class ossimInfoBase;


/**
 * @brief Info factory.
 */
class OSSIM_DLL ossimInfoFactoryInterface : public ossimObjectFactory
{
public:

   /** default constructor */
   ossimInfoFactoryInterface(){}

   /** virtual destructor */
   virtual ~ossimInfoFactoryInterface(){}

   /**
    * @brief Pure virtual create method.  Derived classes must implement to
    * be concrete.
    *
    * @param file Some file you want info for.
    *
    * @return ossimInfoBase* on success 0 on failure.  Caller is responsible
    * for memory.
    */
   virtual ossimInfoBase* create(const ossimFilename& file) const = 0;
   
   virtual ossimObject* createObject(const ossimString& typeName)const
   {
      return 0;
   }
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const
   {
      return 0;
   }
   /*!
    * This should return the type name of all objects in all factories.
    * This is the name used to construct the objects dynamially and this
    * name must be unique.
    */
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const
   {
      
   }
   
};

#endif /* End of "#ifndef ossimInfoFactoryInterface_HEADER" */
