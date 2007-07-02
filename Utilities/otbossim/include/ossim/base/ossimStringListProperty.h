//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimStringListProperty.h 9968 2006-11-29 14:01:53Z gpotts $
//
#ifndef ossimStringListProperty_HEADER
#define ossimStringListProperty_HEADER
#include <ossim/base/ossimProperty.h>

class OSSIMDLLEXPORT ossimStringListProperty : public ossimProperty
{
public:
   ossimStringListProperty(const ossimString& name=ossimString(""),
                           const std::vector<ossimString>& value = std::vector<ossimString>());
   ossimStringListProperty(const ossimStringListProperty& rhs);
   
   ossimObject* dup()const;
   virtual const ossimProperty& assign(const ossimProperty& rhs);
  virtual bool setValue(const ossimString& value);
   virtual void valueToString(ossimString& valueResult)const;

   void clearValueList();
   ossimString getValueAt(int idx)const;
   bool setValueAt(int idx,
                   const ossimString& value);
   bool addValue(const ossimString& value);
   ossim_uint32 getNumberOfValues()const;

   ossim_uint32 getNumberOfContraints()const;
   ossimString getConstraintAt(ossim_uint32 idx)const;
   
   void setConstraints(const std::vector<ossimString>& constraints);
   bool hasConstraints()const;
   void setUniqueFlag(bool flag);
   void setOrderMattersFlag(bool flag);

   /*!
    * You can specify how many values are allowed in the value list
    * If any of the values are less than 0 then now constraints are
    * placed on that value.  So if the min was < 0 then there is
    * no minumum number of values.  If the max is < 0 then there is
    * no maximum number of values.
    */
   void setNumberOfValuesBounds(int minNumber,
                                int maxNumber);
   void getNumberofValuesBounds(int& minNumber,
                                int& maxNumber)const;
protected:
   std::vector<ossimString> theValueList;
   std::vector<ossimString> theConstraintList;
   bool                     theUniqueFlag;
   bool                     theOrderMattersFlag;
   int                      theMinNumberOfValues;
   int                      theMaxNumberOfValues;
   
   bool findValueInConstraintList(const ossimString& value)const;
   bool findValueInValueList(const ossimString& value)const;
   bool canAddValue(const ossimString& value)const;
TYPE_DATA
};

#endif
