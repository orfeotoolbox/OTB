//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimNumericProperty.h,v 1.7 2004/05/19 18:29:38 dburken Exp $
#ifndef ossimNumericProperty_HEADER
#define ossimNumericProperty_HEADER
#include "ossimProperty.h"

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
