//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimSource.cpp,v 1.45 2005/12/08 13:50:20 gpotts Exp $
#include <base/common/ossimSource.h>
#include <base/common/ossimDataObject.h>
#include <base/common/ossimIdManager.h>
#include <base/common/ossimKeywordNames.h>
#include <base/data_types/ossimKeywordlist.h>
#include <base/property/ossimBooleanProperty.h>

RTTI_DEF2(ossimSource, "ossimSource",
          ossimConnectableObject, ossimErrorStatusInterface)


ossimSource::ossimSource(ossimObject* owner)
   :
      ossimConnectableObject(owner),
      theEnableFlag(true),
      theInitializedFlag(false)
{
}

ossimSource::ossimSource(const ossimSource &rhs)
   :
      ossimConnectableObject(rhs),
      theEnableFlag(rhs.theEnableFlag),
      theInitializedFlag(rhs.theInitializedFlag)
{
}

ossimSource::ossimSource(ossimObject* owner,
                         ossim_uint32 inputListSize,
                         ossim_uint32 outputListSize,
                         bool inputListIsFixedFlag,
                         bool outputListIsFixedFlag)
   :
      ossimConnectableObject(owner, inputListSize, outputListSize,
                             inputListIsFixedFlag, outputListIsFixedFlag),
      theEnableFlag(true),
      theInitializedFlag(false)
{}

ossimSource::~ossimSource()   
{
}

bool ossimSource::saveState(ossimKeywordlist& kwl,
                            const char* prefix)const
{
   kwl.add(prefix,
           ossimKeywordNames::ENABLED_KW,
           (int)theEnableFlag,
           true);

   return ossimConnectableObject::saveState(kwl, prefix);
}

bool ossimSource::loadState(const ossimKeywordlist& kwl,
                            const char* prefix)
{
   const char* lookup = kwl.find(prefix,
                                 ossimKeywordNames::ENABLED_KW);
   if(lookup)
   {
      theEnableFlag = ossimString(lookup).toBool();
   }

   return ossimConnectableObject::loadState(kwl, prefix);
}

bool ossimSource::isSourceEnabled()const
{
   return theEnableFlag;
}

void ossimSource::enableSource()
{
   setEnableFlag(true);
}

void ossimSource::disableSource()
{
   setEnableFlag(false);
}

bool ossimSource::getEnableFlag() const
{
   return theEnableFlag;
}

void ossimSource::setEnableFlag(bool flag)
{
   theEnableFlag = flag;
}

bool ossimSource::isInitialized() const
{
   return theInitializedFlag;
}

void ossimSource::setInitializedFlag(bool flag)
{
   theInitializedFlag = flag;
}

void ossimSource::initialize()
{
}

void ossimSource::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(!property.valid()) return;

   ossimString name = property->getName();
   name = name.downcase();
   
   if(name == ossimKeywordNames::ENABLED_KW)
   {
      ossimString value;
      
      property->valueToString(value);
      setEnableFlag(value.toBool());
   }
   else
   {
      ossimConnectableObject::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimSource::getProperty(const ossimString& name)const
{
   if( (name == "Enabled") || (name == ossimKeywordNames::ENABLED_KW) )
   {
      return new ossimBooleanProperty(ossimKeywordNames::ENABLED_KW,theEnableFlag);
   }
   return ossimConnectableObject::getProperty(name);
}

void ossimSource::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimConnectableObject::getPropertyNames(propertyNames);
   
   propertyNames.push_back(ossimKeywordNames::ENABLED_KW);
}

const ossimSource& ossimSource::operator=(const ossimSource &rhs)
{
   return *this;
}

std::ostream& ossimSource::print(std::ostream& out) const
{
   out << "ossimSource::print:\n"
      << "theEnableFlag:       " << theEnableFlag
      << "\ntheInitializedFlag:  " << theInitializedFlag
      << endl;

   return ossimErrorStatusInterface::print(out);
}
