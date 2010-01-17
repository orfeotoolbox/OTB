//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimColorProperty.cpp 13667 2008-10-02 19:59:55Z gpotts $
#include <sstream>
#include <ossim/base/ossimColorProperty.h>

RTTI_DEF1(ossimColorProperty, "ossimColorProperty", ossimProperty);

ossimColorProperty::ossimColorProperty(const ossimString& name,
                                       const ossimRgbVector& value)
   :ossimProperty(name),
    theValue(value)
{
}

ossimColorProperty::ossimColorProperty(const ossimColorProperty& rhs)
   :ossimProperty(rhs),
    theValue(rhs.theValue)
{
}

ossimColorProperty::~ossimColorProperty()
{
}

ossimObject* ossimColorProperty::dup()const
{
   return new ossimColorProperty(*this);
}

const ossimProperty& ossimColorProperty::assign(const ossimProperty& rhs)
{
   ossimProperty::assign(rhs);

   ossimColorProperty* rhsPtr = PTR_CAST(ossimColorProperty, &rhs);

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

bool ossimColorProperty::setValue(const ossimString& value)
{
   bool result = false;
   std::vector<ossimString> splitArray;
   
   value.split(splitArray, " ");
   if(splitArray.size() == 3)
   {
      int r,g,b;
      r = splitArray[0].toInt32();
      g = splitArray[1].toInt32();
      b = splitArray[2].toInt32();
      result = true;
      
      theValue = ossimRgbVector(r,g,b);
   }
   
   return result;
}

void ossimColorProperty::valueToString(ossimString& valueResult)const
{
   ostringstream out;

   out << (int)theValue.getR() << " " << (int)theValue.getG() << " " << (int)theValue.getB() << endl;

   valueResult = out.str().c_str();
}

const ossimRgbVector& ossimColorProperty::getColor()const
{
   return theValue;
}

void ossimColorProperty::setColor(const ossimRgbVector& value)
{
   theValue = value;
}

ossim_uint8 ossimColorProperty::getRed()const
{
   return theValue.getR();
}

ossim_uint8 ossimColorProperty::getGreen()const
{
   return theValue.getG();
}

ossim_uint8 ossimColorProperty::getBlue()const
{
   return theValue.getB();
}

void ossimColorProperty::setRed(ossim_uint8 r)
{
   theValue.setR(r);
}

void ossimColorProperty::setGreen(ossim_uint8 g)
{
   theValue.setG(g);
}

void ossimColorProperty::setBlue(ossim_uint8 b)
{
   theValue.setB(b);
}
