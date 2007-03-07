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
// $Id: ossimXmlAttribute.h,v 1.6 2005/12/16 14:26:21 dburken Exp $
#ifndef ossimXmlAttribute_HEADER
#define ossimXmlAttribute_HEADER

#include <base/common/ossimErrorStatusInterface.h>
#include "ossimString.h"
#include <base/common/ossimObject.h>

class OSSIMDLLEXPORT ossimXmlAttribute : public ossimObject,
                                         public ossimErrorStatusInterface
{
public:
   ossimXmlAttribute(ossimString& spec);  // attribute removed from spec string
   ossimXmlAttribute();
   ossimXmlAttribute(const ossimXmlAttribute& src);
   ossimXmlAttribute(const ossimString& name,
                     const ossimString& value);
   ~ossimXmlAttribute();

   bool read(std::istream& in);
   ossimString getName()  const;
   ossimString getValue() const;
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
