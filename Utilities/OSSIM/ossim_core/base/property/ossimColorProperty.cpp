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
// $Id: ossimColorProperty.cpp,v 1.5 2005/05/23 17:52:23 gpotts Exp $
#include <sstream>
#include "ossimColorProperty.h"

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
   istringstream in(value);

   int r,g,b;

   in >> r >> g >> b;

   theValue = ossimRgbVector(r,g,b);
   
   return in.good();
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
