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
// $Id: ossimBaseObjectFactory.cpp,v 1.2 2002/03/26 19:05:22 gpotts Exp $
#include "base/factory/ossimBaseObjectFactory.h"
#include "base/common/ossimConnectableContainer.h"
#include "base/common/ossimKeywordNames.h"
#include "base/data_types/ossimKeywordlist.h"

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
