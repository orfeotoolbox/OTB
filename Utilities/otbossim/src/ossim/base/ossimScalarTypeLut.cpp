//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
// 
// Author:  David Burken
//
// Description:
//
// Contains class definition for ossimScalarTypeLUT.
// 
//*******************************************************************
//  $Id: ossimScalarTypeLut.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <ossim/base/ossimScalarTypeLut.h>
#include <ossim/base/ossimString.h>

ossimScalarTypeLut* ossimScalarTypeLut::theInstance = NULL;

static const int TABLE_SIZE = 21;

ossimScalarTypeLut* ossimScalarTypeLut::instance()
{
   if (!theInstance)
   {
      theInstance = new ossimScalarTypeLut;
   }
   return theInstance;
}

ossimScalarTypeLut::ossimScalarTypeLut()
   :
      ossimLookUpTable(TABLE_SIZE)
{
   //---
   // Complete initialization of data member "theTable".
   // Note:  Scalar types defined in constants.h file.
   //---
   theTable[0].theKey   = OSSIM_SCALAR_UNKNOWN;
   theTable[0].theValue = "unknown";
   
   theTable[1].theKey   = OSSIM_UINT8;
   theTable[1].theValue = "ossim_uint8";

   theTable[2].theKey   = OSSIM_SINT8;
   theTable[2].theValue = "ossim_sint8";

   theTable[3].theKey   = OSSIM_UINT16;
   theTable[3].theValue = "ossim_uint16";

   theTable[4].theKey   = OSSIM_SINT16;
   theTable[4].theValue = "ossim_sint16";

   theTable[5].theKey   = OSSIM_UINT32;
   theTable[5].theValue = "ossim_uint32";

   theTable[6].theKey   = OSSIM_SINT32;
   theTable[6].theValue = "ossim_sint32";

   theTable[7].theKey   = OSSIM_FLOAT32;
   theTable[7].theValue = "ossim_float32";

   theTable[8].theKey   = OSSIM_FLOAT64;
   theTable[8].theValue = "ossim_float64";

   theTable[9].theKey   = OSSIM_CINT16;
   theTable[9].theValue = "ossim_cint16";

   theTable[10].theKey   = OSSIM_CINT32;
   theTable[10].theValue = "ossim_cint32";

   theTable[11].theKey   = OSSIM_CFLOAT32;
   theTable[11].theValue = "ossim_cfloat32";

   theTable[12].theKey   = OSSIM_CFLOAT64;
   theTable[12].theValue = "ossim_cfloat64";
   
   theTable[13].theKey   = OSSIM_UCHAR;
   theTable[13].theValue = "uchar";

   theTable[14].theKey   = OSSIM_USHORT16;
   theTable[14].theValue = "ushort16";

   theTable[15].theKey   = OSSIM_SSHORT16;
   theTable[15].theValue = "sshort16";

   theTable[16].theKey   = OSSIM_USHORT11;
   theTable[16].theValue = "ushort11";

   theTable[17].theKey   = OSSIM_FLOAT;
   theTable[17].theValue = "float";   

   theTable[18].theKey   = OSSIM_NORMALIZED_FLOAT;
   theTable[18].theValue = "normalized_float";   

   theTable[19].theKey   = OSSIM_NORMALIZED_DOUBLE;
   theTable[19].theValue = "normalized_double";

   theTable[20].theKey   = OSSIM_DOUBLE;
   theTable[20].theValue = "double";
}

ossimScalarTypeLut::~ossimScalarTypeLut()
{
   theInstance = NULL;
}

ossimScalarType
ossimScalarTypeLut::getScalarTypeFromString(const ossimString& s) const
{
   int scalar_type = getEntryNumber(s.c_str());
   
   if (scalar_type == -1)
   {
      return OSSIM_SCALAR_UNKNOWN;
   }
   
   return static_cast<ossimScalarType>(scalar_type);
}   

ossimKeyword ossimScalarTypeLut::getKeyword() const
{
   return ossimKeyword((ossimKeywordNames::SCALAR_TYPE_KW), "");
}
