//*******************************************************************
// Copyright (C) 2002 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks)
// 
// Description:
// Class designed to be used by ossimDirectory class.  Should not be used
// directly.
//
//*************************************************************************
// $Id: ossimDirectoryData.cpp,v 1.4 2004/05/12 17:34:04 dburken Exp $

#ifndef _MSC_VER

#include <sys/types.h>
#include <iostream>
using namespace std;

#include "ossimDirectoryData.h"
#include "ossimDirectory.h"  /* for flag enumerations */

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

 
