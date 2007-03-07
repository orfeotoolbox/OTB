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
// $Id: ossimProjectionViewControllerFactory.cpp,v 1.5 2005/09/14 19:23:14 gpotts Exp $

#include "ossimProjectionViewControllerFactory.h"
#include "base/factory/ossimObjectFactoryRegistry.h"
#include "projections/ossimMapViewController.h"
#include "base/common/ossimKeywordNames.h"
#include "base/data_types/ossimKeywordlist.h"

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
