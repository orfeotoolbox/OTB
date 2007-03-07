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
// $Id: ossimAnnotationObjectFactory.h,v 1.1 2001/09/21 19:45:08 gpotts Exp $
#ifndef ossimAnnotationObjectFactory_HEADER
#define ossimAnnotationObjectFactory_HEADER
#include "base/common/ossimFactoryBaseTemplate.h"
#include "imaging/annotation/ossimAnnotationObject.h"
class ossimAnnotationObjectFactory : public ossimFactoryBase<ossimAnnotationObject>
{
public:
   /*!
    * METHOD: instance()
    * For accessing static instance of concrete factory.
    */
   static ossimAnnotationObjectFactory* instance();
   
   /*!
    * METHOD: create()
    * Attempts to create an instance of the Product given a Specifier or
    * keywordlist. Returns successfully constructed product or NULL.
    */
   virtual ossimAnnotationObject* create(const ossimString& spec) const;
   virtual ossimAnnotationObject* create(const ossimKeywordlist& kwl,
                                         const char* prefix=0)    const;

   /*!
    * METHOD: getList()
    * Returns name list of all products represented by this factory:
    */
   virtual list<ossimString> getList() const;

protected:
   ossimAnnotationObjectFactory();
   static ossimAnnotationObjectFactory*  theInstance;
};

#endif
