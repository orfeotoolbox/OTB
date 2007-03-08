//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks)
//
//*************************************************************************
// $Id: ossimPropertyInterface.cpp 9963 2006-11-28 21:11:01Z gpotts $
#include <ossim/base/ossimPropertyInterface.h>
#include <ossim/base/ossimStringProperty.h>

RTTI_DEF(ossimPropertyInterface, "ossimPropertyInterface");

void ossimPropertyInterface::setProperty(const ossimString& name,
                                         const ossimString& value)
{
   ossimRefPtr<ossimProperty> property = new ossimStringProperty(name, value);

   setProperty(property);
}

void ossimPropertyInterface::setProperty(ossimRefPtr<ossimProperty> property)
{
}

ossimRefPtr<ossimProperty> ossimPropertyInterface::getProperty(const ossimString& name)const
{
   return (ossimProperty*)0;
}

void ossimPropertyInterface::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
}


void ossimPropertyInterface::getPropertyList(std::vector<ossimRefPtr<ossimProperty> >& propertyList)const
{
   std::vector<ossimString> propertyNames;
   int idx = 0;
   getPropertyNames(propertyNames);

   for(idx = 0; idx < (int)propertyNames.size();++idx)
   {
      ossimRefPtr<ossimProperty> prop = getProperty(propertyNames[idx]);
      if(prop.valid())
      {
         propertyList.push_back(prop);  
      }
   }
}

void ossimPropertyInterface::setProperties(std::vector<ossimRefPtr<ossimProperty> >& propertyList)
{
   ossim_uint32 idx = 0;

   for(idx = 0; idx < propertyList.size(); ++idx)
   {
      if(propertyList[idx].valid())
      {
         setProperty(propertyList[idx]);
      }
   }
}
