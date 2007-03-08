//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// LICENSE: LGPL see top level license.txt
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimStringProperty.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimStringProperty_HEADER
#define ossimStringProperty_HEADER
#include <ossim/base/ossimProperty.h>
#include <vector>

class OSSIMDLLEXPORT ossimStringProperty : public ossimProperty
{
public:
   ossimStringProperty(const ossimString& name = ossimString(""),
                       const ossimString& value = ossimString(""),
                       bool editableFlag = true,
                       const std::vector<ossimString>& constraintList = std::vector<ossimString>());
   
   ossimStringProperty(const ossimStringProperty& rhs);

   /*!
    * Duplicates the object and returns a new one
    */
   virtual ossimObject* dup()const;

   virtual const ossimProperty& assign(const ossimProperty& rhs);
   
   void setEditableFlag(bool flag);
   bool getEditableFlag()const;
   bool isEditable()const;
   
   /*!
    * Clears the constraints for this string.  You can constrain
    * it to a string list by calling setConstraints.
    */
   void clearConstraints();

   /*!
    * Sets the constraints for this property.  If the editable
    * flag is present then this says the editing is not readonly
    * and one can hand type the value in without only selecting
    * from a choice list.
    */
   void setConstraints(const std::vector<ossimString>& constraintList);

   void addConstraint(const ossimString& value);
   const std::vector<ossimString>& getConstraints()const;
   
   bool hasConstraints()const;
   
   /*!
    * will try to set the value.  If the constraints are set
    * it will make sure that the value is in the list and
    * if its not then it will return false 
    */
   virtual bool setValue(const ossimString& value);
   
   virtual void valueToString(ossimString& valueResult)const;
  
   
protected:
   ossimString theValue;
   bool        theEditableFlag;
   std::vector<ossimString> theConstraints;

TYPE_DATA
};

#endif
