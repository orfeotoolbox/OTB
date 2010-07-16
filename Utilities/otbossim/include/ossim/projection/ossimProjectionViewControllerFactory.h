//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks)
// Description:
//
//*************************************************************************
// $Id: ossimProjectionViewControllerFactory.h 17108 2010-04-15 21:08:06Z dburken $
#ifndef ossimProjectionViewControllerFactory_HEADER
#define ossimProjectionViewControllerFactory_HEADER
#include <ossim/base/ossimBaseObjectFactory.h>
#include <ossim/base/ossimString.h>

class ossimViewController;

class ossimProjectionViewControllerFactory : public ossimObjectFactory
{
public:
   static ossimProjectionViewControllerFactory* instance();

   virtual ossimViewController* createViewController(const ossimString& name)const;
   virtual ossimViewController* createViewController(const ossimKeywordlist& kwl,
                                                     const char* prefix)const;

   virtual ossimObject* createObject(const ossimString& typeName)const;
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix)const;

   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;
   
protected:
   ossimProjectionViewControllerFactory(){}
   ossimProjectionViewControllerFactory(const ossimProjectionViewControllerFactory& /* rhs */){}

   static ossimProjectionViewControllerFactory* theInstance;
TYPE_DATA
};

#endif
