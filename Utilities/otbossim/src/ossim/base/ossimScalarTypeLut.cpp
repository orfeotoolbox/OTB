//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  David Burken
//
// Description:
//
// Contains class definition for ossimScalarTypeLUT.
// 
//*******************************************************************
//  $Id: ossimScalarTypeLut.cpp 22072 2013-01-04 13:46:52Z dburken $

#include <ossim/base/ossimScalarTypeLut.h>
#include <ossim/base/ossimString.h>

ossimScalarTypeLut* ossimScalarTypeLut::theInstance = NULL;

ossimScalarTypeLut* ossimScalarTypeLut::instance()
{
   if (!theInstance)
   {
      theInstance = new ossimScalarTypeLut;
   }
   return theInstance;
}

ossimScalarTypeLut::ossimScalarTypeLut()
{
   //---
   // Complete initialization of data member "theTable".
   // Note:  Scalar types defined in constants.h file.
   //---
   ossimKeyValueMap entry;
   entry.init(OSSIM_SCALAR_UNKNOWN, "unknown");
   theTable.push_back(entry);
   
   entry.init(OSSIM_UINT8, "ossim_uint8");
   theTable.push_back(entry);

   entry.init(OSSIM_SINT8, "ossim_sint8");
   theTable.push_back(entry);

   entry.init(OSSIM_UINT16, "ossim_uint16");
   theTable.push_back(entry);

   entry.init(OSSIM_SINT16, "ossim_sint16");
   theTable.push_back(entry);

   entry.init(OSSIM_UINT32, "ossim_uint32");
   theTable.push_back(entry);

   entry.init(OSSIM_SINT32, "ossim_sint32");
   theTable.push_back(entry);

   entry.init(OSSIM_UINT64, "ossim_uint64");
   theTable.push_back(entry);

   entry.init(OSSIM_SINT64, "ossim_sint64");
   theTable.push_back(entry);

   entry.init(OSSIM_FLOAT32, "ossim_float32");
   theTable.push_back(entry);

   entry.init(OSSIM_FLOAT64, "ossim_float64");
   theTable.push_back(entry);

   entry.init(OSSIM_CINT16, "ossim_cint16");
   theTable.push_back(entry);

   entry.init(OSSIM_CINT32, "ossim_cint32");
   theTable.push_back(entry);

   entry.init(OSSIM_CFLOAT32, "ossim_cfloat32");
   theTable.push_back(entry);

   entry.init(OSSIM_CFLOAT64, "ossim_cfloat64");
   theTable.push_back(entry);
   
   entry.init(OSSIM_UCHAR, "uchar");
   theTable.push_back(entry);

   entry.init(OSSIM_USHORT16, "ushort16");
   theTable.push_back(entry);

   entry.init(OSSIM_SSHORT16, "sshort16");
   theTable.push_back(entry);

   entry.init(OSSIM_USHORT11, "ushort11");
   theTable.push_back(entry);

   entry.init(OSSIM_FLOAT, "float");
   theTable.push_back(entry);

   entry.init(OSSIM_NORMALIZED_FLOAT, "normalized_float");
   theTable.push_back(entry);

   entry.init(OSSIM_NORMALIZED_DOUBLE, "normalized_double");
   theTable.push_back(entry);

   entry.init(OSSIM_DOUBLE, "double");
   theTable.push_back(entry);

   // Short forms:

   entry.init(OSSIM_UINT8, "U8");
   theTable.push_back(entry);

   entry.init(OSSIM_SINT8, "S8");
   theTable.push_back(entry);

   entry.init(OSSIM_USHORT11, "U11");
   theTable.push_back(entry);

   entry.init(OSSIM_UINT16, "U16");
   theTable.push_back(entry);

   entry.init(OSSIM_SINT16, "S16");
   theTable.push_back(entry);

   entry.init(OSSIM_FLOAT32, "F32");
   theTable.push_back(entry);

   entry.init(OSSIM_FLOAT64, "F64");
   theTable.push_back(entry);
   
   entry.init(OSSIM_NORMALIZED_FLOAT, "N32");
   theTable.push_back(entry);

   entry.init(OSSIM_NORMALIZED_DOUBLE, "N64");
   theTable.push_back(entry);

   // Forms from old code "radiometry" key in ossimImageMetaData:
   
   entry.init(OSSIM_UINT8, "8-bit");
   theTable.push_back(entry);

   entry.init(OSSIM_USHORT11, "11-bit");
   theTable.push_back(entry);

   entry.init(OSSIM_UINT16, "16-bit unsigned");
   theTable.push_back(entry);

   entry.init(OSSIM_SINT16, "16-bit signed");
   theTable.push_back(entry);
   
   entry.init(OSSIM_UINT32, "32-bit unsigned");
   theTable.push_back(entry);

   entry.init(OSSIM_FLOAT32, "float");
   theTable.push_back(entry);
   
   entry.init(OSSIM_NORMALIZED_FLOAT, "normalized float");
   theTable.push_back(entry);

   entry.init(OSSIM_FLOAT64, "double");
   theTable.push_back(entry);
   
   entry.init(OSSIM_NORMALIZED_DOUBLE, "normalized double");
   theTable.push_back(entry);
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
