//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
//*************************************************************************

#include <ossim/base/ossimBinaryDataProperty.h>

RTTI_DEF1(ossimBinaryDataProperty, "ossimBinaryDataProperty", ossimProperty);

ossimBinaryDataProperty::ossimBinaryDataProperty(const ossimString& name,
   const std::vector<unsigned char>& binaryData)
   :ossimProperty(name),
    theBinaryData(binaryData)
{
}

ossimBinaryDataProperty::~ossimBinaryDataProperty()
{
}

bool ossimBinaryDataProperty::setValue(const ossimString& value)
{
   return false;
}

void ossimBinaryDataProperty::valueToString(ossimString& valueResult)const
{
   valueResult = "Unsupported";
}

const vector<unsigned char>& ossimBinaryDataProperty::getBinaryData()const
{
   return theBinaryData;
}

void ossimBinaryDataProperty::setBinaryData(const vector<unsigned char>& binaryData)
{
   theBinaryData = binaryData;
}
