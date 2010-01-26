//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimNumericProperty.cpp 9963 2006-11-28 21:11:01Z gpotts $
#include <algorithm>
#include <ossim/base/ossimNumericProperty.h>


RTTI_DEF1(ossimNumericProperty, "ossimNumericProperty", ossimProperty);

ossimNumericProperty::ossimNumericProperty(const ossimString& name,
                                           const ossimString& value)
   :ossimProperty(name),
    theValue(value),
    theType(ossimNumericPropertyType_FLOAT64)
{
}

ossimNumericProperty::ossimNumericProperty(const ossimString& name,
                                           const ossimString& value,
                                           double minValue,
                                           double maxValue)
   :ossimProperty(name),
    theValue(value),
    theType(ossimNumericPropertyType_FLOAT64)
{
   setConstraints(minValue,
                  maxValue);
}

ossimNumericProperty::ossimNumericProperty(const ossimNumericProperty& rhs)
   :ossimProperty(rhs),
    theValue(rhs.theValue),
    theType(rhs.theType),
    theRangeConstraint(rhs.theRangeConstraint)
{
}

ossimObject* ossimNumericProperty::dup()const
{
   return new ossimNumericProperty(*this);
}

const ossimProperty& ossimNumericProperty::assign(const ossimProperty& rhs)
{
   ossimProperty::assign(rhs);

   ossimNumericProperty* numericProperty = PTR_CAST(ossimNumericProperty,
                                                   &rhs);
   if(numericProperty)
   {
      theValue           = numericProperty->theValue;
      theType            = numericProperty->theType;
      theRangeConstraint = numericProperty->theRangeConstraint;
   }
   else
   {
      ossimString value;
      rhs.valueToString(value);
      setValue(value);
   }
   return *this;
}

bool ossimNumericProperty::hasConstraints()const
{
   return (theRangeConstraint.size() == 2);
}

double ossimNumericProperty::getMinValue()const
{
   if(hasConstraints())
   {
      return theRangeConstraint[0];
   }

   return 0.0;
}

double ossimNumericProperty::getMaxValue()const
{
   if(hasConstraints())
   {
      return theRangeConstraint[1];
   }

   return 0.0;
}

void ossimNumericProperty::clearConstraints()
{
   theRangeConstraint.clear();
}

void ossimNumericProperty::setConstraints(double minValue,
                                          double maxValue)
{
   theRangeConstraint.resize(2);
   theRangeConstraint[0] = minValue;
   theRangeConstraint[1] = maxValue;

   if(minValue > maxValue)
   {
      std::swap(theRangeConstraint[0],
                theRangeConstraint[1]);
   }
}

bool ossimNumericProperty::setValue(const ossimString& value)
{
   bool result = true;
   if(hasConstraints())
   {
      ossim_float64 tempV = (ossim_float64)value.toDouble();
      if((tempV >= theRangeConstraint[0])&&
         (tempV <= theRangeConstraint[1]))
      {
         theValue = value;
      }
   }
   else
   {
     theValue = value;
   }

   return result;
}

void ossimNumericProperty::valueToString(ossimString& valueResult)const
{
   switch(theType)
   {
   case ossimNumericPropertyType_INT:
   {
      valueResult = ossimString::toString(asInt32());
      break;
   }
   case ossimNumericPropertyType_UINT:
   {
      valueResult = ossimString::toString(asUInt32());
      break;
   }
   case ossimNumericPropertyType_FLOAT32:
   {
      valueResult = ossimString::toString(asFloat32());
      
      break;
   }
   case ossimNumericPropertyType_FLOAT64:
   {
      valueResult = ossimString::toString(asFloat64());
      break;
   }
   };
}


ossimNumericProperty::ossimNumericPropertyType ossimNumericProperty::getNumericType()const
{
   return theType;
}

void ossimNumericProperty::setNumericType(ossimNumericPropertyType type)
{
   theType = type;
}

ossim_float64 ossimNumericProperty::asFloat64()const
{
   return (ossim_float64)theValue.toDouble();
}

ossim_float32 ossimNumericProperty::asFloat32()const
{
   return (ossim_float32)theValue.toDouble();
}

ossim_uint32  ossimNumericProperty::asUInt32()const
{
   return theValue.toUInt32();
}

ossim_uint16 ossimNumericProperty::asUInt16()const
{
   return (ossim_uint16)theValue.toUInt32();
}

ossim_uint8 ossimNumericProperty::asUInt8()const
{
   return (ossim_uint8)theValue.toUInt32();
}

ossim_sint32 ossimNumericProperty::asInt32()const
{
   return (ossim_int32)theValue.toInt32();
}

ossim_sint16 ossimNumericProperty::asInt16()const
{
   return (ossim_int16)theValue.toInt32();
}

ossim_sint8 ossimNumericProperty::asInt8()const
{
   return (ossim_int8)theValue.toInt32();
}
