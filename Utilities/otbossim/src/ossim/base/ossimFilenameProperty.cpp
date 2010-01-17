//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimFilenameProperty.cpp 15833 2009-10-29 01:41:53Z eshirschorn $
#include <ossim/base/ossimFilenameProperty.h>

RTTI_DEF1(ossimFilenameProperty, "ossimFilenameProperty", ossimProperty);

ossimFilenameProperty::ossimFilenameProperty(const ossimString& name,
                                             const ossimFilename& value,
                                             const std::vector<ossimString>& filterList)
   :ossimProperty(name),
    theValue(value),
    theFilterList(filterList),
    theIoType(ossimFilenamePropertyIoType_NOT_SPECIFIED)
{
}

ossimFilenameProperty::ossimFilenameProperty(const ossimFilenameProperty& rhs)
   :ossimProperty(rhs),
    theValue(rhs.theValue),
    theFilterList(rhs.theFilterList),
    theIoType(rhs.theIoType)
{
}

ossimFilenameProperty::~ossimFilenameProperty()
{
}

ossimObject* ossimFilenameProperty::dup()const
{
   return new ossimFilenameProperty(*this);
}

void ossimFilenameProperty::valueToString(ossimString& valueResult)const
{
   valueResult = theValue;
}

bool ossimFilenameProperty::setValue(const ossimString& value)
{
   theValue = value;

   return true;
}

const ossimFilenameProperty& ossimFilenameProperty::operator = (ossimFilenameProperty& rhs)
{
   assign(rhs);

   return *this;
}

const ossimProperty& ossimFilenameProperty::assign(const ossimProperty& rhs)
{
   ossimProperty::assign(rhs);
   theValue = rhs.valueToString();

   ossimFilenameProperty* rhsPtr = PTR_CAST(ossimFilenameProperty,
                                            &rhs);

   if(rhsPtr)
   {
      theFilterList = rhsPtr->theFilterList;
      theIoType     = rhsPtr->theIoType;
   }

   return *this;
}

void ossimFilenameProperty::clearFilterList()
{
   theFilterList.clear();
}

ossim_uint32 ossimFilenameProperty::getNumberOfFilters()const
{
   return (ossim_uint32)theFilterList.size();
}

void ossimFilenameProperty::setFilter(ossim_uint32 idx,
                                      const ossimString& filterValue)
{
   if(idx < getNumberOfFilters())
   {
      theFilterList[idx] = filterValue;
   }
}

ossimString ossimFilenameProperty::getFilter(ossim_uint32 idx)const
{
   if(idx < getNumberOfFilters())
   {
      return theFilterList[idx];
   }

   return ossimString("");
}


ossimString ossimFilenameProperty::getFilterListAsString(const ossimString& separator)const
{
   ossimString result;
   ossim_uint32 idx = 0;

   if(getNumberOfFilters() > 0)
   {
      for(idx = 0; idx < getNumberOfFilters()-1;++idx)
      {
         result += getFilter(idx);
         result += separator;
      }
      result += getFilter(getNumberOfFilters()-1);
   }

   return result;
}


const std::vector<ossimString>& ossimFilenameProperty::getFilterList()const
{
   return theFilterList;
}

void ossimFilenameProperty::addFilter(const ossimString& filter)
{
   theFilterList.push_back(filter);
}


void ossimFilenameProperty::setIoType(ossimFilenamePropertyIoType ioType)
{
   theIoType = ioType;
}

ossimFilenameProperty::ossimFilenamePropertyIoType ossimFilenameProperty::getIoType()const
{
   return theIoType;
}

bool ossimFilenameProperty::isIoTypeInput()const
{
   return (theIoType==ossimFilenamePropertyIoType_INPUT);
}

bool ossimFilenameProperty::isIoTypeOutput()const
{
   return (theIoType==ossimFilenamePropertyIoType_OUTPUT);
}
