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
// $Id: ossimObjectFactoryRegistry.h,v 1.10 2003/07/07 16:34:45 gpotts Exp $
#ifndef ossimObjectFactoryRegistry_HEADER
#define ossimObjectFactoryRegistry_HEADER
#include <vector>

#include "base/common/ossimObject.h"
#include "base/data_types/ossimString.h"

class OSSIMDLLEXPORT ossimObjectFactory;
class OSSIMDLLEXPORT ossimKeywordlist;

class OSSIMDLLEXPORT ossimObjectFactoryRegistry : public ossimObject
{
public:
   virtual ~ossimObjectFactoryRegistry();
   static ossimObjectFactoryRegistry* instance();
   /*!
    * Should use the registerFactory to add a factory to the list.
    * This method will call registerFactory.
    */
   bool                addFactory(ossimObjectFactory* factory);

   /*!
    * Registers a factory to the list.
    */
   bool                registerFactory(ossimObjectFactory* factory);
   void                unregisterFactory(ossimObjectFactory* factory);
   
   virtual ossimObject* createObject(const ossimString& name)const;
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const;
   
   /*!
    * Returns a type list of all objects that can be instantiated
    * through the createObjectMethods above.
    */
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;

   /*!
    * returns a list of objects that are of the passed in
    * base type.  for example:
    *
    *  If I want all ossimImageSource objects then I call:
    *
    *  instance()->getTypeNameList(l,
    *                                                          "ossimImageSource");
    */ 
   virtual void getTypeNameList(std::vector<ossimString>& typeList,
                                const ossimString& baseType)const;
   
protected:
   ossimObjectFactoryRegistry(){}//hide
   ossimObjectFactoryRegistry(const ossimObjectFactoryRegistry&):ossimObject(){}//hide
   void operator =(const ossimObjectFactoryRegistry&){}//hide
   
   static ossimObjectFactoryRegistry* theInstance;
   std::vector<ossimObjectFactory*>        theFactoryList;

   ossimObjectFactory* findFactory(ossimObjectFactory* factory)const;

TYPE_DATA
};

extern "C"
{
  OSSIMDLLEXPORT void* ossimObjectFactoryRegistryGetInstance();
}
#endif
