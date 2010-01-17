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
// $Id: ossimDirectoryData.cpp 9966 2006-11-29 02:01:07Z gpotts $

#ifndef _MSC_VER

#include <sys/types.h>
#include <iostream>
using namespace std;

#include <ossim/base/ossimDirectoryData.h>
#include <ossim/base/ossimDirectory.h>

// ----------------------------------------------------------------------------
// ossimDirectoryData
// ----------------------------------------------------------------------------

#if !defined( __VMS__ ) || ( __VMS_VER >= 70000000 )

ossimDirectoryData::ossimDirectoryData(const ossimFilename& dirname)
         : theDirectoryName(dirname)
{
    theDir = NULL;

    // throw away the trailing slashes
    size_t n = theDirectoryName.length();
    if(n > 0)
    {

       while ( n > 0 && theDirectoryName[--n] == '/' )
        ;

//       theDirectoryName.Truncate(n + 1);

    // do open the dir
       theDir = opendir(theDirectoryName.c_str());
    }
}

ossimDirectoryData::~ossimDirectoryData()
{
   if ( theDir )
   {
      if ( closedir(theDir) != 0 )
      {
         // ERROR closing directory
      }
   }
}

bool ossimDirectoryData::read(ossimFilename &filename)
{
   filename = ossimFilename::NIL;
   dirent *de = (dirent *)NULL;    // just to silent compiler warnings
   bool matches = false;
   ossimFilename temp;

   while ( !matches )
   {
      de = readdir(theDir);
      if ( !de )
      {
         return false;
      }
      
      temp = theDirectoryName.dirCat(de->d_name);

      //***
      // NOTE:
      // The check for "." and ".." did not work so added pattern match check.
      //***
      
      // Check for "." and ".." match at the end of the name.
      ossimString s = temp.match("\\.$|\\.\\.$"); 
      
      // don't return "." and ".." unless asked for
      if ( (temp == "..") ||
           (temp == ".")  ||
           (s    == ".")  ||
           (s    == "..") )
      {
         if (theFlags & ossimDirectory::OSSIM_DIR_DOTDOT)
         {
            matches = true;
         }
      }
      else if((temp.isDir()) &&
              (theFlags & ossimDirectory::OSSIM_DIR_DIRS))
      {
         matches = true;
      }
      else if((temp.isFile()) &&
              (theFlags & ossimDirectory::OSSIM_DIR_FILES))
      {
         matches = true;
      }
      else
      {
         matches = false;
      }
   }
   
   if (matches)
   {
      filename = temp;//de->d_name;
   }
   
   return matches;
}

#else // old VMS (TODO)

ossimDirectoryData::ossimDirectoryData(const ossimFilename& WXUNUSED(dirname))
{
}

ossimDirectoryData::~ossimDirectoryData()
{
}

bool ossimDirectoryData::read(ossimFilename &filename)
{
    return false;
}

#endif // not or new VMS/old VMS

void ossimDirectoryData::setFlags(int flags)
{
   theFlags = flags;
}

int ossimDirectoryData::getFlags() const
{
   return theFlags;
}

void ossimDirectoryData::rewind()
{
   rewinddir(theDir);
}

#endif /* #ifndef _MSC_VER */

 
