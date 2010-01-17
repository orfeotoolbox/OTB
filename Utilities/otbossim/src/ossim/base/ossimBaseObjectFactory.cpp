//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks)
// Description:
//
//*************************************************************************
// $Id: ossimBaseObjectFactory.cpp 9966 2006-11-29 02:01:07Z gpotts $
#include <ossim/base/ossimBaseObjectFactory.h>
#include <ossim/base/ossimConnectableContainer.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimKeywordlist.h>

RTTI_DEF1(ossimBaseObjectFactory, "ossimBaseObjectFactory", ossimObjectFactory);

ossimBaseObjectFactory* ossimBaseObjectFactory::theInstance = NULL;

ossimBaseObjectFactory* ossimBaseObjectFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimBaseObjectFactory;
   }

   return theInstance;
}

ossimObject* ossimBaseObjectFactory::createObject(const ossimString& typeName)const
{
   if(typeName == STATIC_TYPE_NAME(ossimConnectableContainer))
   {
      return new ossimConnectableContainer;
   }

   return (ossimObject*)NULL;
}

ossimObject* ossimBaseObjectFactory::createObject(const ossimKeywordlist& kwl,
                                                  const char* prefix)const
{
   const char* type = kwl.find(prefix,
                               ossimKeywordNames::TYPE_KW);
   ossimString copyPrefix = prefix;
   if(type)
   {
      ossimObject* object = createObject(ossimString(type));
      if(object)
      {
         object->loadState(kwl, copyPrefix.c_str());
      }
      return object;
   }
   return NULL;  
}

void ossimBaseObjectFactory::getTypeNameList(std::vector<ossimString>& typeList)const
{
   typeList.push_back(STATIC_TYPE_NAME(ossimConnectableContainer));
}
