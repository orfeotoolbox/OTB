//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimColorProperty.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimColorProperty_HEADER
#define ossimColorProperty_HEADER
#include <ossim/base/ossimProperty.h>
#include <ossim/base/ossimRtti.h>
#include <ossim/base/ossimRgbVector.h>

class OSSIMDLLEXPORT ossimColorProperty : public ossimProperty
{
public:
   ossimColorProperty(const ossimString& name=ossimString(""),
                      const ossimRgbVector& value = ossimRgbVector(0,0,0));
   ossimColorProperty(const ossimColorProperty& rhs);
   virtual ossimObject* dup()const;
   virtual const ossimProperty& assign(const ossimProperty& rhs);
   virtual bool setValue(const ossimString& value);
   virtual void valueToString(ossimString& valueResult)const;
   const ossimRgbVector& getColor()const;
   void setColor(const ossimRgbVector& value);
   ossim_uint8 getRed()const;
   ossim_uint8 getGreen()const;
   ossim_uint8 getBlue()const;
   void setRed(ossim_uint8 r);
   void setGreen(ossim_uint8 r);
   void setBlue(ossim_uint8 r);
protected:
   virtual ~ossimColorProperty();
   ossimRgbVector theValue;

TYPE_DATA
};

#endif
