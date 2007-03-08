//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks)
// Description:
//
//*************************************************************************
// $Id: ossimProjectionViewControllerFactory.cpp 9963 2006-11-28 21:11:01Z gpotts $

#include <ossim/projection/ossimProjectionViewControllerFactory.h>
#include <ossim/base/ossimObjectFactoryRegistry.h>
#include <ossim/projection/ossimMapViewController.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimKeywordlist.h>

RTTI_DEF1(ossimProjectionViewControllerFactory, "ossimProjectionViewControllerFactory", ossimObjectFactory);

ossimProjectionViewControllerFactory* ossimProjectionViewControllerFactory::theInstance = NULL;

ossimProjectionViewControllerFactory* ossimProjectionViewControllerFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimProjectionViewControllerFactory;
   }

   return theInstance;
}

ossimViewController* ossimProjectionViewControllerFactory::createViewController(const ossimString& name)const
{
   if(name == STATIC_TYPE_NAME(ossimMapViewController))
   {
      return new ossimMapViewController;
   }

   return (ossimViewController*)NULL;
}

ossimViewController* ossimProjectionViewControllerFactory::createViewController(const ossimKeywordlist& kwl,
                                                                                const char* prefix)const
{
   const char* type = kwl.find(prefix,  ossimKeywordNames::TYPE_KW);
   ossimViewController* result = NULL;
   if(type)
   {
      result = createViewController(ossimString(type));
      if(result)
      {
         if(!result->loadState(kwl, prefix))
         {
            delete result;
            result = NULL;
         }
      }
   }
   
   return result;
}

ossimObject* ossimProjectionViewControllerFactory::createObject(const ossimString& typeName)const
{
   return createViewController(typeName);
}

ossimObject* ossimProjectionViewControllerFactory::createObject(const ossimKeywordlist& kwl,
                                                                const char* prefix)const
{
   return createViewController(kwl, prefix);
}

void ossimProjectionViewControllerFactory::getTypeNameList(std::vector<ossimString>& typeList)const
{
   typeList.push_back("ossimMapViewController");
}
