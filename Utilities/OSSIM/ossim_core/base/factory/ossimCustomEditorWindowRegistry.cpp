//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks)
// Description:
//
//*************************************************************************
// $Id: ossimCustomEditorWindowRegistry.cpp,v 1.8 2005/10/17 18:37:16 gpotts Exp $
#include "ossimCustomEditorWindowRegistry.h"
#include "base/factory/ossimCustomEditorWindowFactoryBase.h"
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
