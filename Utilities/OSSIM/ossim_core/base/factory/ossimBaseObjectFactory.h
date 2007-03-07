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
// $Id: ossimBaseObjectFactory.h,v 1.5 2002/11/25 19:16:28 gpotts Exp $
#ifndef ossimBaseObjectFactory_HEADER
#define ossimBaseObjectFactory_HEADER
#include "base/factory/ossimObjectFactory.h"

class OSSIMDLLEXPORT ossimBaseObjectFactory : public ossimObjectFactory
{
public:
   virtual ~ossimBaseObjectFactory(){theInstance = NULL;}
   
   static ossimBaseObjectFactory* instance();
   virtual ossimObject* createObject(const ossimString& typeName)const;
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const;
   /*!
    * This should return the type name of all objects in all factories.
    * This is the name used to construct the objects dynamially and this
    * name must be unique.
    */
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;
   
protected:
   static ossimBaseObjectFactory* theInstance;
   
TYPE_DATA
};
#endif
