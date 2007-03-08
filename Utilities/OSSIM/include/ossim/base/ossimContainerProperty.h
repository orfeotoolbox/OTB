//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// LICENSE: LGPL  see top level LICENSE.txt
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimContainerProperty.h 10174 2007-01-03 18:24:31Z gpotts $
#ifndef ossimContainerProperty_HEADER
#define ossimContainerProperty_HEADER
#include <vector>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimProperty.h>

class OSSIMDLLEXPORT ossimContainerProperty : public ossimProperty
{
public:
   ossimContainerProperty(const ossimString& name=ossimString(""));
   ossimContainerProperty(const ossimContainerProperty& rhs);
   virtual ~ossimContainerProperty();
   
   virtual ossimObject* dup()const;

   virtual const ossimProperty& assign(const ossimProperty& rhs);

   virtual void addChildren(std::vector<ossimRefPtr<ossimProperty> >& propertyList);

   virtual void addChild(ossimProperty* propery);

   virtual void addStringProperty(const ossimString& name,
                                  const ossimString& value,
                                  bool readOnlyFlag = false);
   
   virtual const ossimContainerProperty* asContainer()const;
   virtual ossimContainerProperty* asContainer();
   
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name,
                                                  bool recurse=false);
   virtual void deleteChildren();
   virtual bool setValue(const ossimString& value);
   virtual void valueToString(ossimString& valueResult)const;

   virtual ossim_uint32 getNumberOfProperties()const;
   virtual ossimRefPtr<ossimProperty> getProperty(ossim_uint32 idx);

   virtual void getPropertyList(std::vector<ossimRefPtr<ossimProperty> >& children);
   ossimRefPtr<ossimXmlNode> toXml()const;

protected:
   std::vector<ossimRefPtr<ossimProperty> > theChildPropertyList;

   virtual void copyList(const ossimContainerProperty& rhs);

   
TYPE_DATA
};

#endif
