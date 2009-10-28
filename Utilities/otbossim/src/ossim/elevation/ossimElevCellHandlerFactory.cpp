//*****************************************************************************
// FILE: ossimElevCellHandlerFactory.cc
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// DESCRIPTION:
//   Contains implementation of class ossimElevCellHandlerFactory. This is
//   a "super-factory" owning a list of subfactories for each particular DEM
//   format class.
//
//   NOT CURRENTLY UTILIZED -- USE DTED MANAGER
//
// LIMITATIONS:
//   The intention of this factory is to produce individual instances of
//   elevation cell handlers. Presently this is not supported since DTED is
//   the only elevation source being handled. DTED is loaded via the
//   ossimDtedManager class as a DB interface, and not by accessing individual
//   DTED handlers.
//
// SOFTWARE HISTORY:
//>
//   01Aug2001  Oscar Kramer (okramer@imagelinks.com)
//              Initial coding.
//<
//*****************************************************************************
//  $Id: ossimElevCellHandlerFactory.cpp 14800 2009-06-30 08:58:55Z dburken $

#include <ossim/elevation/ossimElevCellHandlerFactory.h>
#include <ossim/base/ossimString.h>

ossimElevCellHandlerFactory* ossimElevCellHandlerFactory::theInstance = 0;

//*****************************************************************************
//  STATIC METHOD: instance()
//  
//*****************************************************************************
ossimElevCellHandlerFactory* ossimElevCellHandlerFactory::instance()
{
   if(!theInstance)
      theInstance = new ossimElevCellHandlerFactory;
 
   return (ossimElevCellHandlerFactory*) theInstance; 
} 

//*****************************************************************************
//  PROTECTED DEFAULT CONSTRUCTOR: ossimElevCellHandlerFactory
//  
//*****************************************************************************
ossimElevCellHandlerFactory::ossimElevCellHandlerFactory()
{
   //***
   // Add default sub factories to this factory's registry list:
   // NOTE: DTED handlers are managed by their own ossimDtedManager so should
   // never be created individually via a factory. This will be the typical
   // pattern for all but custom DEM files.
   //***
//   registerFactory(ossimUsgsDemCellFactory::instance());
}


//*****************************************************************************
//  METHOD: ossimElevCellHandlerFactory::create(kwl, prefix)
//  
//*****************************************************************************
ossimElevCellHandler*
ossimElevCellHandlerFactory::create(const ossimKeywordlist &keywordList,
                                    const char *prefix) const
{
   std::list<ossimFactoryBase<ossimElevCellHandler>*>::const_iterator
      elevCellFactory;

   ossimElevCellHandler* product = 0;
   
   elevCellFactory = theRegistry.begin();
   while((elevCellFactory != theRegistry.end()) && (!product))
   {
      product = (*elevCellFactory)->create(keywordList, prefix);
      elevCellFactory++;
   }

   return product;
}

//*****************************************************************************
//  METHOD: ossimElevCellHandlerFactory::create(proj_name)
//  
//*****************************************************************************
ossimElevCellHandler*
ossimElevCellHandlerFactory::create(const ossimString &name) const 
{
   std::list<ossimFactoryBase<ossimElevCellHandler>*>::const_iterator
      elevCellFactory;

   ossimElevCellHandler* product = 0;
   
   elevCellFactory = theRegistry.begin();
   while((elevCellFactory != theRegistry.end()) && (!product))
   {
      product = (*elevCellFactory)->create(name);
      elevCellFactory++;
   }

   return product;
}

//*****************************************************************************
//  METHOD: ossimElevCellHandlerFactory::getList()
//  
//*****************************************************************************
std::list<ossimString> ossimElevCellHandlerFactory::getList() const 
{
   std::list<ossimString> rtn_list;
   std::list<ossimString> sub_list;
   std::list<ossimFactoryBase<ossimElevCellHandler>*>::const_iterator factory_iter;

   factory_iter = theRegistry.begin();
   while(factory_iter != theRegistry.end())
   {
      sub_list = (*factory_iter)->getList();
      rtn_list.merge(sub_list);
      factory_iter++;
   }

   return rtn_list;
}

