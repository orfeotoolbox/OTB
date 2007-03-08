//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// LICENSE: LGPL see top level license.txt
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimDateProperty.cpp 9094 2006-06-13 19:12:40Z dburken $
//

#include <iostream>
#include <iomanip>
#include <sstream>
#include <ossim/base/ossimDateProperty.h>


RTTI_DEF1(ossimDateProperty, "ossimDateProperty", ossimProperty);

ossimDateProperty::ossimDateProperty()
      :ossimProperty("")
{
   setDate(ossimDate());
}

ossimDateProperty::ossimDateProperty(const ossimString& name,
                                     const ossimString& value)
      :ossimProperty(name)
{
   setValue(value);
}

ossimDateProperty::ossimDateProperty(const ossimString& name,
                                     const ossimLocalTm& value)
      :ossimProperty(name),
       theValue(value)
{
}

ossimDateProperty::ossimDateProperty(const ossimDateProperty& src)
   :ossimProperty(src),
    theValue(src.theValue)
{
}

ossimObject* ossimDateProperty::dup()const
{
   return new ossimDateProperty(*this);
}

void ossimDateProperty::setDate(const ossimLocalTm& localTm)
{
   theValue = localTm;
}

const ossimLocalTm& ossimDateProperty::getDate()const
{
   return theValue;
}

bool ossimDateProperty::setValue(const ossimString& value)
{
   if(value.trim() == "")
   {
      theValue = ossimDate();
      return true;
   }
   bool result = value.size() == 14;
   
   ossimString year;
   ossimString month;
   ossimString day;
   ossimString hour;
   ossimString min;
   ossimString sec;

   if(value.size() == 14)
   {
      year = ossimString(value.begin(),
                         value.begin()+4);
      month = ossimString(value.begin()+4,
                          value.begin()+6);
      day = ossimString(value.begin()+6,
                        value.begin()+8);
      hour = ossimString(value.begin()+8,
                        value.begin()+10);
      min = ossimString(value.begin()+10,
                        value.begin()+12);
      sec = ossimString(value.begin()+12,
                        value.begin()+14);

      theValue.setYear(year.toUInt32());
      theValue.setMonth(month.toUInt32());
      theValue.setDay(day.toUInt32());
      theValue.setHour(hour.toUInt32());
      theValue.setMin(min.toUInt32());
      theValue.setSec(sec.toUInt32());
   }

   return result;
}

void ossimDateProperty::valueToString(ossimString& valueResult)const
{
    std::ostringstream out;

   out << std::setw(4)
       << std::setfill('0')
       << theValue.getYear()
       << std::setw(2)
       << std::setfill('0')
       << theValue.getMonth()
       << std::setw(2)
       << std::setfill('0')
       << theValue.getDay()
       << std::setw(2)
       << std::setfill('0')
       << theValue.getHour()
       << std::setw(2)
       << std::setfill('0')
       << theValue.getMin()
       << std::setw(2)
       << std::setfill('0')
       << theValue.getSec();
   
   valueResult =  out.str();
  
}

const ossimProperty& ossimDateProperty::assign(const ossimProperty& rhs)
{
   ossimProperty::assign(rhs);

   ossimDateProperty* rhsPtr = PTR_CAST(ossimDateProperty, &rhs);

   if(rhsPtr)
   {
      theValue = rhsPtr->theValue;
   }
   else
   {
      setValue(rhs.valueToString());
   }

   return *this;
}
