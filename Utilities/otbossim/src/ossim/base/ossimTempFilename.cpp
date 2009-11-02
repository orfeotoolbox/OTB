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
// $Id: ossimTempFilename.cpp 15524 2009-09-30 01:02:26Z dburken $
#include <stdlib.h>
#include <fstream>
#include <ossim/base/ossimTempFilename.h>
#include <ossim/base/ossimEnvironmentUtility.h>
#include <time.h>

ossimTempFilename::ossimTempFilename(const ossimString& tempDir,
                                     const ossimString& prefix,
                                     const ossimString& extension,
                                     bool autodelete,
                                     bool useWildcardDelete)
   :theTempDir(tempDir),
    thePrefix(prefix),
    theExtension(extension),
    theAutoDeleteFlag(autodelete),
    theWildCardDeleteFlag(useWildcardDelete)
{
}

ossimTempFilename::~ossimTempFilename()
{
   if(*((ossimFilename*)this) != "")
   {
      
      if(theAutoDeleteFlag)
      {
         if(theWildCardDeleteFlag)
         {
            wildcardRemove(*this + ".*");
         }
         else
         {
            remove();
         }
      }
   }
}

void  ossimTempFilename::generateRandomFile()
{
   generate(false);
}

void ossimTempFilename::generateRandomDir()
{
   generate(true);
}

void ossimTempFilename::generate(bool createAsDirectoryFlag)
{
   srand(time(0));
   ossimString tempDirCopy = theTempDir;

   if(tempDirCopy == "")
   {
      tempDirCopy = ossimEnvironmentUtility::instance()->getEnvironmentVariable("TEMP");
      if(tempDirCopy=="")
      {
         tempDirCopy  = ossimEnvironmentUtility::instance()->getEnvironmentVariable("TMP");
      }
      if(tempDirCopy == "")
      {
         if(ossimFilename("/tmp").exists())
         {
            tempDirCopy = "/tmp";
         }
      }
   }

   int count = 0;
   int randNumber1 = rand();
   ossimFilename prefixDir = ossimFilename(tempDirCopy);
   ossimFilename result = prefixDir.dirCat(thePrefix+
                                           ossimString::toString(randNumber1));
   
   while((count < RAND_MAX)&&result.exists())
   {
      randNumber1 = rand();
      result = prefixDir.dirCat(thePrefix+
                                ossimString::toString(randNumber1));
      
      ++count;
   }

   if(theExtension != "")
   {
      result = result.setExtension(theExtension);
   }
   *((ossimFilename*)this) = result;
   if(result != "")
   {
      if(createAsDirectoryFlag)
      {
         createDirectory();
      }
      else
      {
         std::ofstream out(result.c_str());
         out.close();
      }
   }
}
