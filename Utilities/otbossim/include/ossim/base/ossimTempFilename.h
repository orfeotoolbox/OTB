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
// $Id: ossimTempFilename.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimTempFilename_HEADER
#define ossimTempFilename_HEADER
#include <ossim/base/ossimFilename.h>

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
