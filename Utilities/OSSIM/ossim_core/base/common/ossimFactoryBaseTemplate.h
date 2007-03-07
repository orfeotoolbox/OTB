//*****************************************************************************
// FILE: ossimFactoryBaseTemplate.h
//
// Copyright (C) 2001 ImageLinks, Inc.
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
// DESCRIPTION:
//   Contains declaration of template class ossimFactory. This is the base
//   type for all factories.
//
// SOFTWARE HISTORY:
//>
//   14Jun2001  Oscar Kramer (okramer@imagelinks.com)
//              Initial coding.
//<
//*****************************************************************************
//  $Id: ossimFactoryBaseTemplate.h,v 1.5 2002/11/25 18:45:47 gpotts Exp $

#ifndef ossimFactory_HEADER
#define ossimFactory_HEADER
#include "ossimConstants.h"
#include <list>
using namespace std;
class OSSIMDLLEXPORT ossimKeywordlist;
class OSSIMDLLEXPORT ossimString;

/*!****************************************************************************
 *
 * TEMPLATE: ossimFactoryBase  
 *
 * NOTE: This is primarily intended as an interface template only to insure
 *       that all factories prescribe to a common type. Concrete classes shall
 *       be derived from an instance of this template as:
 *
 *       class MyFactory : public ossimFactoryBase<MyProductClass>
 *
 *       where MyProductClass is the class type being produced.
 *
 *       This template is applicable to "Registry" type factories that have
 *       one or more sub-factories. Thus, a common interface is provided
 *       without the client distinguishing between registries and factories.
 *
 * EXAMPLE: Refer to the projection factories for a working example of the
 *       use of this base class template. Specifically, refer to the files:
 *          - ../ossim_core/projections/factory/ossimProjectionFactory.h
 *          - ../ossim_core/projections/factory/ossimMapProjectionFactory.h
 *          - ../ossim_core/projections/factory/ossimSensorModelFactory.h
 *
 *****************************************************************************/
template <class Product> class OSSIMDLLEXPORT ossimFactoryBase 
{
public:
   /*!
    * METHOD: instance()
    * For accessing static singleton instance of concrete factory. The derived
    * class implementation of this method invokes the factory's private
    * constructor. MUST BE PROVIDED BY CONCRETE DERIVED CLASS as follows:
    *
    static <ossimFactoryBase-derived class>* instance();
    */
   
   /*!
    * PURE VIRTUAL METHODS: create(string), create(keywordlist, prefix)
    *
    * Attempts to create an instance of the Product given a string or
    * keywordlist. It is up to the concrete factory implementation to decide
    * how to interpret string. It could conceivably represent a "magic
    * number" specification of a product (such as the class name). Or, it could
    * be a filename that the factory must open to decide which product
    * to instantiate. The derived factory can also define other create methods
    * following the model used here.
    *
    * Returns successfully constructed product or NULL.
    */
   virtual Product* create(const ossimString&)     const = 0;
   virtual Product* create(const ossimKeywordlist& kwl,
                           const char* prefix)          const = 0;

   /*!
    * PURE VIRTUAL METHOD: getList()
    * Returns name list of all products represented by this factory:
    */
   virtual list<ossimString> getList() const = 0;

   /*!
    * METHOD: registerFactory()
    * Adds sub-factories to this one. Relevant for the case where the
    * derived factory maintains a registry of sub-factories.
    */
   void registerFactory(ossimFactoryBase<Product>* new_factory)
      { if(new_factory) theRegistry.push_back(new_factory); }
   
protected:
   /*!
    * PROTECTED DEFAULT CONSTRUCTOR
    * Gives concrete class the opportunity to perform initialization
    * tasks since this is called once with first instance() call. If the
    * derived factory is to serve as a sub-factory registry, then have the
    * derived class' constructor call registerFactory() for all of the default
    * sub-factories involved.
    */
   ossimFactoryBase() {};

   virtual ~ossimFactoryBase() {}

   /*!
    * Member contains list of sub-factories associated with this factory:
    */
   list<ossimFactoryBase<Product>*> theRegistry;
   
};

#endif

