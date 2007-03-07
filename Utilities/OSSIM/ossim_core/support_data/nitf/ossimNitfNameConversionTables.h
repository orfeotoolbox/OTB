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
//  $Id: ossimNitfNameConversionTables.h,v 1.3 2004/10/29 12:40:12 gpotts Exp $
#ifndef ossimNitfNameConversionTables_HEADER
#define ossimNitfNameConversionTables_HEADER
#include <base/common/ossimConstants.h>
#include <base/data_types/ossimString.h>

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
