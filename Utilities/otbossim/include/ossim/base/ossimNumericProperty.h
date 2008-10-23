//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimNumericProperty.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimNumericProperty_HEADER
#define ossimNumericProperty_HEADER
#include <ossim/base/ossimProperty.h>

class OSSIMDLLEXPORT ossimNumericProperty : public ossimProperty
{
public:
   enum ossimNumericPropertyType
   {
      ossimNumericPropertyType_INT   = 0,
      ossimNumericPropertyType_UINT,
      ossimNumericPropertyType_FLOAT32,
      ossimNumericPropertyType_FLOAT64
   };
   
   ossimNumericProperty(const ossimString& name=ossimString(""),
                        const ossimString& value=ossimString("0"));
   ossimNumericProperty(const ossimString& name,
                        const ossimString& value,
                        double minValue,
                        double maxValue);
   ossimNumericProperty(const ossimNumericProperty& rhs);
   virtual ossimObject* dup()const;
   virtual const ossimProperty& assign(const ossimProperty& rhs);
   
   virtual bool hasConstraints()const;
   double getMinValue()const;
   double getMaxValue()const;
   
   virtual void clearConstraints();
   virtual void setConstraints(double minValue,
                               double maxValue);
  virtual bool setValue(const ossimString& value);
   virtual void valueToString(ossimString& valueResult)const;

   virtual ossimNumericPropertyType getNumericType()const;
   virtual void setNumericType(ossimNumericPropertyType type);
   
   virtual ossim_float64 asFloat64()const;
   virtual ossim_float32 asFloat32()const;
   virtual ossim_uint32  asUInt32()const;
   virtual ossim_uint16  asUInt16()const;
   virtual ossim_uint8   asUInt8()const;
   virtual ossim_sint32  asInt32()const;
   virtual ossim_sint16  asInt16()const;
   virtual ossim_sint8   asInt8()const;
   
protected:
   ossimString              theValue;
   ossimNumericPropertyType theType;
   std::vector<double>      theRangeConstraint;
TYPE_DATA
};


#endif
