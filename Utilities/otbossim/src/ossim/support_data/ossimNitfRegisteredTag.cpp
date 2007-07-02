//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfRegisteredTag.cpp 10177 2007-01-03 19:22:09Z gpotts $
#include <ossim/support_data/ossimNitfRegisteredTag.h>

RTTI_DEF2(ossimNitfRegisteredTag, "ossimNitfRegisteredTag", ossimObject, ossimPropertyInterface)

void ossimNitfRegisteredTag::setProperty(ossimRefPtr<ossimProperty> property)
{
}

ossimRefPtr<ossimProperty> ossimNitfRegisteredTag::getProperty(const ossimString& name)const
{
   return 0;
}

void ossimNitfRegisteredTag::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
}
