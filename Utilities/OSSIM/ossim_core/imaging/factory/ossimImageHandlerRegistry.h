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
// Author:  Garrett Potts (gpott@imagelinks.com)
//
// Description:  Contains class definition for the class
//               ImageHandlerRegistry.
//
//*******************************************************************
//  $Id: ossimImageHandlerRegistry.h,v 1.11 2004/05/19 17:54:51 dburken Exp $

#ifndef ossimImageHandlerRegistry_HEADER
#define ossimImageHandlerRegistry_HEADER

#include <vector>
#include "base/factory/ossimObjectFactory.h"
#include "base/common/ossimRtti.h"

class ossimImageHandler;
class ossimImageHandlerFactoryBase;
class ossimFilename;
class ossimKeywordlist;

class OSSIMDLLEXPORT ossimImageHandlerRegistry : public ossimObjectFactory
{
public:
   ~ossimImageHandlerRegistry();
   static ossimImageHandlerRegistry* instance();
   
   void addFactory(ossimImageHandlerFactoryBase* factory)
      {
         registerFactory(factory);
      }
   void registerFactory(ossimImageHandlerFactoryBase* factory);
   void unregisterFactory(ossimImageHandlerFactoryBase* factory);
   bool findFactory(ossimImageHandlerFactoryBase* factory)const;
   
   ossimImageHandler* open(const ossimFilename& fileName)const;

   
   /*!
    *  Given a keyword list return a pointer to an ImageHandler.  Returns
    *  null if a valid handler cannot be found.
    */
   ossimImageHandler* open(const ossimKeywordlist& kwl, const char* prefix=0)const;

   /*!
    * Creates an object given a type name.
    */
   virtual ossimObject* createObject(const ossimString& typeName)const;

   /*!
    * Creates and object given a keyword list.
    */
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const;
   
   /*!
    * This should return the type name of all objects in all factories.
    * This is the name used to construct the objects dynamially and this
    * name must be unique.
    */
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;
   
protected:
   ossimImageHandlerRegistry();
   ossimImageHandlerRegistry(const ossimImageHandlerRegistry& rhs);
   const ossimImageHandlerRegistry&
      operator=(const ossimImageHandlerRegistry& rhs);

private:
   void clear();
   std::vector<ossimImageHandlerFactoryBase*>   theFactoryList;
   static ossimImageHandlerRegistry*            theInstance;

TYPE_DATA
};

extern "C"
{
 OSSIMDLLEXPORT  void* ossimImageHandlerRegistryGetInstance();
}
#endif
