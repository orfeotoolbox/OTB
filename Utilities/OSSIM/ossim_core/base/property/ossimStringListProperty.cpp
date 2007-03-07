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
// $Id: ossimStringListProperty.cpp,v 1.8 2004/01/29 13:29:31 gpotts Exp $
//
#include <sstream>
#include <algorithm>
#include "ossimStringListProperty.h"
#include "base/data_types/ossimKeywordlist.h"
RTTI_DEF1(ossimStringListProperty, "ossimStringListProperty", ossimProperty);

ossimStringListProperty::ossimStringListProperty(const ossimString& name,
                                                 const std::vector<ossimString>& value)
   :ossimProperty(name),
    theValueList(value),
    theUniqueFlag(false),
    theOrderMattersFlag(false)
{
}

ossimStringListProperty::ossimStringListProperty(const ossimStringListProperty& rhs)
   :ossimProperty(rhs),
    theValueList(rhs.theValueList),
    theConstraintList(rhs.theConstraintList),
    theUniqueFlag(rhs.theUniqueFlag),
    theOrderMattersFlag(rhs.theOrderMattersFlag),
    theMinNumberOfValues(rhs.theMinNumberOfValues),
    theMaxNumberOfValues(rhs.theMaxNumberOfValues)
{
}
   
ossimObject* ossimStringListProperty::dup()const
{
   return new ossimStringListProperty(*this);
}

const ossimProperty& ossimStringListProperty::assign(const ossimProperty& rhs)
{
   ossimProperty::assign(rhs);

   ossimStringListProperty *rhsPtr = PTR_CAST(ossimStringListProperty,
                                              &rhs);

   if(rhsPtr)
   {
      theValueList         = rhsPtr->theValueList;
      theConstraintList    = rhsPtr->theConstraintList;
      theUniqueFlag        = rhsPtr->theUniqueFlag;
      theOrderMattersFlag  = rhsPtr->theOrderMattersFlag;
      theMinNumberOfValues = rhsPtr->theMinNumberOfValues;
      theMaxNumberOfValues = rhsPtr->theMaxNumberOfValues;
   }

   return *this;
}

bool ossimStringListProperty::setValue(const ossimString& value)
{
   ossimKeywordlist kwl;
   istringstream in(value);
   bool result = true;
   
   if(kwl.parseStream(in))
   {
      int idx = 0;
      vector<ossimString> keys =
         kwl.getSubstringKeyList( "^([0-9]*" );
      
      std::vector<int> theNumberList(keys.size());
      for(idx = 0; idx < (int)theNumberList.size();++idx)
      {
         theNumberList[idx] = keys[idx].toInt();
      }
      std::sort(theNumberList.begin(), theNumberList.end());
      clearValueList();
      for(idx = 0; idx < (int)theNumberList.size(); ++idx)
      {
         const char* temp = kwl.find(ossimString::toString(theNumberList[idx]));

         if(temp)
         {
            addValue(temp);
         }
      }
   }
   else
   {
      result = false;
   }

   return result;
}

void ossimStringListProperty::valueToString(ossimString& valueResult)const
{
   ossimKeywordlist kwl;
   int idx = 0;

   for(idx = 0; idx < (int)theValueList.size(); ++idx)
   {
      kwl.add(ossimString::toString(idx),
              theValueList[idx],
              true);
   }
   
   valueResult = kwl.toString();
}


void ossimStringListProperty::clearValueList()
{
   theValueList.clear();
}

ossimString ossimStringListProperty::getValueAt(int idx)const
{
   if((idx >= 0)&&
      (idx < (int)getNumberOfValues()))
   {
      return theValueList[idx];
   }
   
   return ossimString("");;
}

bool ossimStringListProperty::setValueAt(int idx,
                                         const ossimString& value)
{
   bool result = true;
   
   if(canAddValue(value))
   {
      if((idx < (int)getNumberOfValues())&&
         (idx >= 0))
      {
         theValueList[idx] = value;
      }
   }
   else
   {
      result = false;
   }
   return result;
   
}

bool ossimStringListProperty::addValue(const ossimString& value)
{
   bool result = true;
   
   if(canAddValue(value))
   {
      theValueList.push_back(value);
   }
   else
   {
      result = false;
   }

   return result;
}
 
ossim_uint32 ossimStringListProperty::getNumberOfValues()const
{
   return (int)theValueList.size();
}

ossim_uint32 ossimStringListProperty::getNumberOfContraints()const
{
   return theConstraintList.size();
}

ossimString ossimStringListProperty::getConstraintAt(ossim_uint32 idx)const
{
   if(idx < theConstraintList.size())
   {
      return theConstraintList[(int)idx];
   }

   return ossimString("");
}

void ossimStringListProperty::setConstraints(const std::vector<ossimString>& constraints)
{
   theConstraintList =  constraints;
}

bool ossimStringListProperty::hasConstraints()const
{
   return (theConstraintList.size()>0);
}

void ossimStringListProperty::setUniqueFlag(bool flag)
{
   theUniqueFlag = flag;
}

void ossimStringListProperty::setOrderMattersFlag(bool flag)
{
   theOrderMattersFlag = flag;
}

void ossimStringListProperty::setNumberOfValuesBounds(int minNumber,
                                                      int maxNumber)
{
   theMinNumberOfValues = minNumber;
   theMaxNumberOfValues = maxNumber;
}

void ossimStringListProperty::getNumberofValuesBounds(int& minNumber,
                                                      int& maxNumber)const
{
   minNumber = theMinNumberOfValues;
   maxNumber = theMaxNumberOfValues;
}

bool ossimStringListProperty::findValueInConstraintList(const ossimString& value)const
{
   return (std::find(theConstraintList.begin(),
                     theConstraintList.end(),
                     value)!=theConstraintList.end());
}

bool ossimStringListProperty::findValueInValueList(const ossimString& value)const
{
   return (std::find(theValueList.begin(),
                     theValueList.end(),
                     value)!=theConstraintList.end());
}

bool ossimStringListProperty::canAddValue(const ossimString& value)const
{
   bool result = true;

   if(hasConstraints())
   {
      if(findValueInConstraintList(value))
      {
         if(theUniqueFlag)
         {
            if(findValueInValueList(value))
            {
               result = false;
            }
         }
      }
      else
      {
         result = false;
      }
   }

   return result;
}
