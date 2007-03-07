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
// $Id: ossimProjectionViewControllerFactory.h,v 1.2 2005/09/14 19:23:30 gpotts Exp $
#ifndef ossimProjectionViewControllerFactory_HEADER
#define ossimProjectionViewControllerFactory_HEADER
#include "base/factory/ossimBaseObjectFactory.h"
#include "base/data_types/ossimString.h"

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
   ossimProjectionViewControllerFactory(const ossimProjectionViewControllerFactory& rhs ){}

   static ossimProjectionViewControllerFactory* theInstance;
TYPE_DATA
};

#endif
