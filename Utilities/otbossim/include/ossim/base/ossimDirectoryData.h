//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts (gpotts@imagelinks)
// 
// Description:
// Class designed to be used by ossimDirectory class.  Should not be used
// directly.
//
//*************************************************************************
// $Id: ossimDirectoryData.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimDirectoryData_HEADER
#define ossimDirectoryData_HEADER

#ifndef _MSC_VER

#include <dirent.h>
#include <ossim/base/ossimFilename.h>

class OSSIMDLLEXPORT ossimDirectoryData
{
 public:
   ossimDirectoryData(const ossimFilename& dirname);
   ~ossimDirectoryData();
   
   bool isOk() const { return theDir != NULL; }
   
   void setFlags(int flags);
   int  getFlags() const;
   
   void rewind();
   bool read(ossimFilename &filename);
   
 private:
   DIR*          theDir;
   ossimFilename theDirectoryName;
   int           theFlags;
};

#endif /* #ifndef _MSC_VER */

#endif /* #ifndef ossimDirectoryData_HEADER */
