//*******************************************************************
// Copyright (C) 2001 ImageLinks Inc.  All rights reserved.
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Oscar Kramer (ossim port by D. Burken)
//
// Description:  
//
// Contains declaration of class ossimAttribute.
//
//*****************************************************************************
// $Id: ossimXmlAttribute.h 12521 2008-02-28 20:09:25Z gpotts $
#ifndef ossimXmlAttribute_HEADER
#define ossimXmlAttribute_HEADER

#include <ossim/base/ossimErrorStatusInterface.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimObject.h>

class OSSIMDLLEXPORT ossimXmlAttribute : public ossimObject,
                                         public ossimErrorStatusInterface
{
public:
   ossimXmlAttribute(ossimString& spec);  // attribute removed from spec string
   ossimXmlAttribute();
   ossimXmlAttribute(const ossimXmlAttribute& src);
   ossimXmlAttribute(const ossimString& name,
                     const ossimString& value);
   virtual ossimObject* dup() const
   {
      return new ossimXmlAttribute(*this);
   }
   ~ossimXmlAttribute();

   bool read(std::istream& in);
   const ossimString& getName()  const;
   const ossimString& getValue() const;
   void setNameValue(const ossimString& name,
                     const ossimString& value);
   void setName(const ossimString& name);
   void setValue(const ossimString& value);
   
   OSSIMDLLEXPORT friend std::ostream& operator<<(
      std::ostream& os, const ossimXmlAttribute* xml_attr);

protected:
   ossimString theName;
   ossimString theValue;

   bool readName(std::istream& in);
   bool readValue(std::istream& in);
TYPE_DATA
};

#endif /* #ifndef ossimXmlAttribute_HEADER */
