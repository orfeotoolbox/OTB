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
// Author:  David Burken (dburken@imagelinks.com)
//
// Description:
//
// Contains class declaration for ossimImageHandlerFactoryMaker.
//*******************************************************************
//  $Id: ossimImageHandlerFactory.h,v 1.4 2004/05/26 21:43:14 dburken Exp $

#ifndef ossimImageHandlerFactory_HEADER
#define ossimImageHandlerFactory_HEADER
#include "ossimImageHandlerFactoryBase.h"

#include "base/data_types/ossimString.h"

class ossimImageHandler;
class ossimFilename;
class ossimKeywordlist;

//*******************************************************************
// CLASS:  ossimImageHandlerFactory
//*******************************************************************
class OSSIMDLLEXPORT ossimImageHandlerFactory : public ossimImageHandlerFactoryBase
{
public:
   virtual ~ossimImageHandlerFactory();
   static ossimImageHandlerFactory* instance();
   
   virtual ossimImageHandler* open(const ossimFilename& fileName)const;
   virtual ossimImageHandler* open(const ossimKeywordlist& kwl,
                                   const char* prefix=0)const;

   
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
   ossimImageHandlerFactory(){}
   ossimImageHandlerFactory(const ossimImageHandlerFactory&){}
   void operator = (const ossimImageHandlerFactory&){}

   static ossimImageHandlerFactory* theInstance;

TYPE_DATA
};

#endif
