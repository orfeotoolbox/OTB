//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// LICENSE: LGPL see top level license.txt
//
// Author: Garrett Potts (gpotts@imagelinks)
//
//*************************************************************************
// $Id: ossimPropertyInterface.h,v 1.13 2004/12/21 14:06:21 gpotts Exp $
#ifndef ossimPropertyInterface_HEADER
#define ossimPropertyInterface_HEADER
#include "base/data_types/ossimKeywordlist.h"
#include "base/common/ossimObject.h"
#include <base/common/ossimRefPtr.h>
#include <base/property/ossimProperty.h>


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
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
   void getPropertyList(std::vector<ossimRefPtr<ossimProperty> >& propertyList)const;
   void setProperties(std::vector<ossimRefPtr<ossimProperty> >& propertyList);
   
protected:
   
TYPE_DATA
};

#endif
