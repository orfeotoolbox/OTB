//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// Description: This is a tmeporary filename class it will create a temporary
//              file and will also delete it upon destruction
//
//*************************************************************************
// $Id: ossimTempFilename.h,v 1.3 2005/10/03 13:20:24 gpotts Exp $
#ifndef ossimTempFilename_HEADER
#define ossimTempFilename_HEADER
#include <base/data_types/ossimFilename.h>

class OSSIM_DLL ossimTempFilename : public ossimFilename
{
public:
   ossimTempFilename(const ossimString& tempDir="",
                     const ossimString& prefix = "",
                     const ossimString& extension = "",
                     bool autodelete=true,
                     bool useWildcardDelete=true);

   void generateRandomFile();
   void generateRandomDir();
   
   ~ossimTempFilename();
   
protected:
   ossimString theTempDir;
   ossimString thePrefix;
   ossimString theExtension;
   bool theAutoDeleteFlag;
   bool theWildCardDeleteFlag;
   

   void generate(bool createAsDirectoryFlag);
};
#endif
