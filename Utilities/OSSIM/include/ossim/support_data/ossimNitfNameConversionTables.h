//*******************************************************************
// Copyright (C) 2004 Garrett Potts.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimNitfNameConversionTables.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimNitfNameConversionTables_HEADER
#define ossimNitfNameConversionTables_HEADER
#include <ossim/base/ossimConstants.h>

class ossimString;

class OSSIM_DLL ossimNitfNameConversionTables
{
public:
   ossimNitfNameConversionTables();

   ossimString convertMapProjectionNameToNitfCode(const ossimString& mapProjectionName)const;
   ossimString convertNitfCodeToOssimProjectionName(const ossimString& nitfProjectionCode)const;
   ossimString convertNitfCodeToNitfProjectionName(const ossimString& nitfProjectionCode)const;
   ossimString convertNitfProjectionNameToNitfCode(const ossimString& nitfProjectionName)const;
};

#endif
