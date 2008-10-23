//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimTextProperty.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimTextProperty_HEADER
#define ossimTextProperty_HEADER
#include <ossim/base/ossimProperty.h>

class OSSIMDLLEXPORT ossimTextProperty : public ossimProperty
{
public:
   ossimTextProperty(const ossimString& name  = ossimString(""),
                     const ossimString& value = ossimString(""),
                     bool isMultiLine = false);
   ossimTextProperty(const ossimTextProperty& rhs);
   virtual ossimObject* dup()const;

   bool isMulitLine()const;
   void setMultiLineFlag(bool flag);

   virtual void valueToString(ossimString& valueResult)const;
  virtual bool setValue(const ossimString& value);
   virtual const ossimProperty& assign(const ossimProperty& rhs);
   
protected:
   ossimString theValue;
   bool theMultiLineFlag;

TYPE_DATA
};

#endif
