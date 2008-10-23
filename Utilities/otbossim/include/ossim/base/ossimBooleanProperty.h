//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimBooleanProperty.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimBoolenaProperty_HEADER
#define ossimBoolenaProperty_HEADER
#include <ossim/base/ossimProperty.h>

class OSSIMDLLEXPORT ossimBooleanProperty : public ossimProperty
{
public:
   ossimBooleanProperty(const ossimString& name=ossimString(""),
                        bool value = false);
   ossimBooleanProperty(const ossimBooleanProperty& rhs);
   virtual ossimObject* dup()const;
   virtual const ossimProperty& assign(const ossimProperty& rhs);
   virtual bool setValue(const ossimString& value);
   bool setBooleanValue(bool value,
			ossimString& msg);
   void valueToString(ossimString& valueResult)const;
   bool getBoolean()const;
   
protected:
   bool theValue;

TYPE_DATA
};

#endif
