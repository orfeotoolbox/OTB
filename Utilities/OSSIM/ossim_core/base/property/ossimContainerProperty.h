//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// LICENSE: LGPL  see top level LICENSE.txt
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimContainerProperty.h,v 1.7 2004/09/23 17:18:27 gpotts Exp $
#ifndef ossimContainerProperty_HEADER
#define ossimContainerProperty_HEADER
#include <vector>
#include <base/common/ossimRefPtr.h>
#include "ossimProperty.h"

class OSSIMDLLEXPORT ossimContainerProperty : public ossimProperty
{
public:
   ossimContainerProperty(const ossimString& name=ossimString(""));
   ossimContainerProperty(const ossimContainerProperty& rhs);
   virtual ~ossimContainerProperty();
   
   virtual ossimObject* dup()const;

   virtual const ossimProperty& assign(const ossimProperty& rhs);
   /*! 
    * Note:  the container will own the properties passed in.
    */
   virtual void addChildren(std::vector<ossimRefPtr<ossimProperty> >& propertyList);

   /*!
    * Will add the child to the list and take ownership
    */
   virtual void addChild(ossimProperty* propery);
   
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name,
                                                  bool recurse=false);
   virtual void deleteChildren();
   virtual bool setValue(const ossimString& value);
   virtual void valueToString(ossimString& valueResult)const;

   virtual ossim_uint32 getNumberOfProperties()const;
   virtual ossimRefPtr<ossimProperty> getProperty(ossim_uint32 idx);

   virtual void getPropertyList(std::vector<ossimRefPtr<ossimProperty> >& children);
   
protected:
   std::vector<ossimRefPtr<ossimProperty> > theChildPropertyList;

   virtual void copyList(const ossimContainerProperty& rhs);

   
TYPE_DATA
};

#endif
