#ifndef ossimDateProperty_HEADER
#define ossimDateProperty_HEADER
#include "ossimProperty.h"
#include <base/common/ossimDate.h>

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
