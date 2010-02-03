//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// LICENSE: LGPL see top level license.txt
//
// Author: Garrett Potts (gpotts@imagelinks)
//
//*************************************************************************
// $Id: ossimPropertyInterface.h 12989 2008-06-04 17:05:41Z gpotts $
#ifndef ossimPropertyInterface_HEADER
#define ossimPropertyInterface_HEADER
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimProperty.h>


class OSSIM_DLL ossimPropertyInterface
{
public:
   ossimPropertyInterface()
      {}
   
   virtual ~ossimPropertyInterface()
      {}

   virtual void setProperty(const ossimString& name,
                            const ossimString& value);
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const; 
	virtual ossimString getPropertyValueAsString(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
   void getPropertyList(std::vector<ossimRefPtr<ossimProperty> >& propertyList)const;
   void setProperties(std::vector<ossimRefPtr<ossimProperty> >& propertyList);
   
protected:
   
TYPE_DATA
};

#endif
