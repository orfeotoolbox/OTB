//*******************************************************************
//
// LICENSE: LGPL see top level license.txt
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimDateProperty.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimDateProperty_HEADER
#define ossimDateProperty_HEADER
#include <ossim/base/ossimProperty.h>
#include <ossim/base/ossimDate.h>

class OSSIM_DLL ossimDateProperty : public ossimProperty
{
public:
   ossimDateProperty();
   ossimDateProperty(const ossimString& name,
                     const ossimString& value);
   ossimDateProperty(const ossimString& name,
                     const ossimLocalTm& value);
   ossimDateProperty(const ossimDateProperty& src);
   virtual ossimObject* dup()const;
   
   void setDate(const ossimLocalTm& localTm);
   const ossimLocalTm& getDate()const;
   virtual bool setValue(const ossimString& value);
   virtual void valueToString(ossimString& valueResult)const;
   virtual const ossimProperty& assign(const ossimProperty& rhs);
   
protected:
   ossimLocalTm theValue;

TYPE_DATA
};

#endif
