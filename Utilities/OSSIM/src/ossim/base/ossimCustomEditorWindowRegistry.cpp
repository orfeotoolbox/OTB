//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks)
// Description:
//
//*************************************************************************
// $Id: ossimCustomEditorWindowRegistry.cpp 9966 2006-11-29 02:01:07Z gpotts $
#include <ossim/base/ossimCustomEditorWindowRegistry.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimCustomEditorWindowFactoryBase.h>
ossimCustomEditorWindowRegistry* ossimCustomEditorWindowRegistry::theInstance=0;

RTTI_DEF(ossimCustomEditorWindowRegistry, "ossimCustomEditorWindowRegistry");

ossimCustomEditorWindowRegistry::~ossimCustomEditorWindowRegistry()
{
   theInstance = NULL;
}

ossimCustomEditorWindowRegistry* ossimCustomEditorWindowRegistry::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimCustomEditorWindowRegistry;
   }

   return theInstance;
}

bool ossimCustomEditorWindowRegistry::registerFactory(ossimCustomEditorWindowFactoryBase* factory)
{
   bool result = false;
   if(factory)
   {
      theFactoryList.push_back(factory);
      result = true;
   }

   return result;
}

ossimCustomEditorWindow* ossimCustomEditorWindowRegistry::createCustomEditor(ossimObject* obj,
                                                                             void* parent)const
{
   ossimCustomEditorWindow* result = NULL;
   int i = 0;

   for(i = 0; (i < (int)theFactoryList.size())&&(!result); ++i)
   {
      result = theFactoryList[i]->createCustomEditor(obj, parent);
   }

   return result;
}
ossimCustomEditorWindow* ossimCustomEditorWindowRegistry::createCustomEditor(const ossimString& classType,
                                                                             void* parent)const
{
   ossimCustomEditorWindow* result = NULL;
   int i = 0;

   for(i = 0; (i < (int)theFactoryList.size())&&(!result); ++i)
   {
      result = theFactoryList[i]->createCustomEditor(classType, parent);
   }

   return result;
}

ossimObject* ossimCustomEditorWindowRegistry::createObject(const ossimString& typeName)const
{
   ossimObject* result = NULL;
   unsigned long index = 0;
   
   while((index < theFactoryList.size()) &&(!result))
   {
      result = theFactoryList[index]->createObject(typeName);
      ++index;
   }
   return result;
}

ossimObject* ossimCustomEditorWindowRegistry::createObject(const ossimKeywordlist& kwl,
                                                           const char* prefix)const
{
   ossimObject* result = NULL;
   unsigned long index = 0;

   while((index < theFactoryList.size()) &&(!result))
   {
      result = theFactoryList[index]->createObject(kwl,
                                                   prefix);
      ++index;
   }

   return result;
}

void* ossimCustomEditorWindowRegistry::createPopupEditor(ossimObject* obj,
                                                         void* parent)const
{
   void* result = NULL;
   unsigned long index = 0;

   while((index < theFactoryList.size()) &&(!result))
   {
      result = theFactoryList[index]->createPopupEditor(obj,
                                                        parent);
      ++index;
   }

   return result;
}
   
void* ossimCustomEditorWindowRegistry::createPanelEditor(ossimObject* obj,
                                                         void* parent)const
{
   void* result = NULL;
   unsigned long index = 0;

   while((index < theFactoryList.size()) &&(!result))
   {
      result = theFactoryList[index]->createPanelEditor(obj,
                                                        parent);
      ++index;
   }

   return result;
}

void ossimCustomEditorWindowRegistry::getTypeNameList(std::vector<ossimString>& typeList)const
{
   std::vector<ossimCustomEditorWindowFactoryBase*>::const_iterator iter = theFactoryList.begin();
   std::vector<ossimString> result;
   
   while(iter != theFactoryList.end())
   {
      result.clear();
      (*iter)->getTypeNameList(result);

      typeList.insert(typeList.end(),
                      result.begin(),
                      result.end());
      ++iter;
   }
}

extern "C"
{
  void* ossimCustomEditorWindowRegistryGetInstance()
  {
    return ossimCustomEditorWindowRegistry::instance();
  }
}
