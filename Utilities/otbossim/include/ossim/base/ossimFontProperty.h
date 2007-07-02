//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimFontProperty.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimFontProperty_HEADER
#define ossimFontProperty_HEADER
#include <ossim/base/ossimProperty.h>
#include <ossim/base/ossimFontInformation.h>

class OSSIMDLLEXPORT ossimFontProperty : public ossimProperty
{
public:
   ossimFontProperty(const ossimString& name,
                     const ossimFontInformation& value);
   ossimFontProperty(const ossimFontProperty& rhs);
   virtual ~ossimFontProperty();
   virtual ossimObject* dup()const;
   virtual const ossimProperty& assign(const ossimProperty& rhs);
  virtual bool setValue(const ossimString& value);
   virtual void valueToString(ossimString& valueResult)const;

   void setFontInformation(ossimFontInformation& fontInfo);
   const ossimFontInformation& getFontInformation()const;
protected:
   ossimFontInformation theValue;

TYPE_DATA
};

#endif
