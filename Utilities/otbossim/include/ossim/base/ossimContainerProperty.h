//*******************************************************************
//
// LICENSE: See top level LICENSE.txt
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimContainerProperty.h 19917 2011-08-09 11:12:24Z gpotts $
#ifndef ossimContainerProperty_HEADER
#define ossimContainerProperty_HEADER
#include <vector>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimProperty.h>

class OSSIMDLLEXPORT ossimContainerProperty : public ossimProperty
{
public:
   friend class ossimProperty;
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

   virtual void getPropertyList(
      std::vector<ossimRefPtr<ossimProperty> >& children) const;
   
   virtual ossimRefPtr<ossimXmlNode> toXml()const;
   virtual void setReadOnlyFlag(bool flag);

   virtual void accept(ossimVisitor& visitor);
protected:
   std::vector<ossimRefPtr<ossimProperty> > theChildPropertyList;

   virtual void copyList(const ossimContainerProperty& rhs);

   
TYPE_DATA
};

#endif
