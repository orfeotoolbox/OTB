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
// Description: A brief description of the contents of the file.
//
//
//*************************************************************************
// $Id: ossimConnectableContainerInterface.h,v 1.12 2005/11/22 19:39:11 dburken Exp $
#ifndef ossimConnectableContainerInterface_HEADER
#define ossimConnectableContainerInterface_HEADER
#include "base/common/ossimConstants.h"
#include "base/common/ossimRtti.h"
#include "base/common/ossimId.h"
#include <vector>

class OSSIMDLLEXPORT ossimObject;
class OSSIMDLLEXPORT ossimConnectableObject;
class OSSIMDLLEXPORT ossimString;

class OSSIMDLLEXPORT ossimConnectableContainerInterface
{
public:
   ossimConnectableContainerInterface(ossimObject* obj):theBaseObject(obj){}
   virtual ~ossimConnectableContainerInterface(){theBaseObject=NULL;}
   
   /*!
    * Will find all objects of the past in type.  Use the RTTI type info.  An optional
    * recurse flag will say if there is another container then recurse it to
    * find the type you are looking for else it just looks within its immediate
    * children
    * 
    * Example: passing STATIC_TYPE_INFO(ossimImageRenderer) as an argument will
    *          look for all ossimImageRenderer's and return the list.
    */
   virtual std::vector<ossimConnectableObject*> findAllObjectsOfType(const RTTItypeid& typeInfo,
                                                                     bool recurse=true)=0;
   virtual std::vector<ossimConnectableObject*> findAllObjectsOfType(const ossimString& className,
                                                                     bool recurse=true)=0;

   /*!
    * Will find the firt object of the past in type.  Use the RTTI type info.An optional
    * recurse flag will say if there is another container then recurse it to
    * find the type you are looking for else it just looks within its immediate
    * children
    * 
    * Example: passing STATIC_TYPE_INFO(ossimImageRenderer) as an argument will
    *          look for the first ossimImageRenderer and return that object.
    */
   virtual ossimConnectableObject* findFirstObjectOfType(const RTTItypeid& typeInfo,
                                                         bool recurse=true)=0;
   
   virtual ossimConnectableObject* findFirstObjectOfType(const ossimString& className,
                                                         bool recurse=true)=0;

   /*!
    * will search for the object given an id.  If recurse is true it will recurse
    * to other containers.
    */ 
   virtual ossimConnectableObject* findObject(const ossimId& id,
                                              bool recurse=true)=0;

   virtual ossimConnectableObject* findObject(const ossimConnectableObject* obj,
                                              bool recurse=true)=0;
   /*!
    * Will cycle through all sources setting their ids. the idLast wlil be updated
    * so we can recurse into other containers.
    */
   virtual void makeUniqueIds()=0;

   /*!
    * Returns the number of objects within this container and all child containers.
    */
   virtual ossim_uint32 getNumberOfObjects(bool recurse=true)const=0;
   
   
   /*!
    * Will add an object to the container and then set the added objects owner
    * to this.
    */
   virtual bool addChild(ossimConnectableObject* attachableObject)=0;

   /*!
    * Will remove the child from the container.  Changes the owner of the
    * child to be NULL;
    */
   virtual bool removeChild(ossimConnectableObject* object)=0;

   /*!
    * Gives access to the this point of the base object that everyone derives
    * from.
    */
   ossimObject*       getObject(){return theBaseObject;}
   const ossimObject* getObject()const{return theBaseObject;}

   /**
    * @param index Index of object to get.
    *
    * @see getNumberOfObjects(false) to get the number of objects inside
    * the container.
    *
    * @return Container's object at that index or NULL if out of range.
    *
    * @note This does not recurse into other containers.
    */
   virtual ossimConnectableObject* getConnectableObject(ossim_uint32 index)=0;

   void deleteAllChildren();
   virtual void getChildren(vector<ossimConnectableObject*>& children,
                            bool immediateChildrenOnlyFlag)=0;
protected:
   ossimObject* theBaseObject;
TYPE_DATA
};

#endif
