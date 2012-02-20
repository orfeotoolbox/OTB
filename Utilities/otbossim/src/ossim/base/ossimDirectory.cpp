//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
// 
// Author: Garrett Potts
// 
// Description: A brief description of the contents of the file.
//
//*************************************************************************
// $Id: ossimDirectory.cpp 20229 2011-11-08 17:01:17Z oscarkramer $

#include <cstring> /* for strncasecmp */
#include <iostream>

#if defined (_WIN32)
#include <io.h>
#include <direct.h>
#else
#include <ossim/base/ossimDirectoryData.h>
#endif
#ifdef __BORLANDC__
# include <dir.h>
#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#define _chdir chdir
#endif

#include <ossim/base/ossimDirectory.h>
#include <ossim/base/ossimRegExp.h>

//#include "wx/filefn.h"          // for wxMatchWild

// #include <sys/types.h>

// #include <dirent.h>

// ----------------------------------------------------------------------------
// ossimDirectory construction/destruction
// ----------------------------------------------------------------------------

#if defined (_WIN32)
ossimDirectory::ossimDirectory()
   :
      theData(0),
      theDirectoryName(),
      theFlags(0)
{}

ossimDirectory::ossimDirectory(const ossimFilename &dirname)
   :
      theData(0),
      theDirectoryName(dirname),
      theFlags(0)
{
   open(dirname);
}

ossimDirectory::~ossimDirectory()
{
   if (theData != 0) _findclose( theData );
}

bool ossimDirectory::open(const ossimFilename &dirname)
{
   // close out currently open directory
   if (theData != 0)
   {
      _findclose( theData );
      theData = 0;
   }
   
   // set new directory name
   theDirectoryName = dirname;

	return (dirname.isDir());
   // cd to the new directory
//   if (_chdir( theDirectoryName) == -1)
//   {
//      return false;
//   }
   
//   return true;
}

bool ossimDirectory::getFirst(ossimFilename &filename, int flags)
{
   struct _finddata_t c_file;
   ossimFilename temp;
   //long theData1 = _findfirst( "*.*", &c_file );
   ossimFilename dirName = theDirectoryName.dirCat("*");
   if( (theData = _findfirst( dirName.c_str(), &c_file )) != 0L )
   {
      setFlags(flags);
      
      temp = theDirectoryName.dirCat(c_file.name);
      
	  
      while (!fileMatched(temp))
      {
         // look for next file in the directory
         if (_findnext( theData, &c_file ) == 0 )
         {
            temp = theDirectoryName.dirCat(c_file.name);
         }
         else
         {
            // no more file in the directory
            filename.clear();
            return false;
         }
      }
   }

   // set the filenane that matches
   filename = temp.trim();

   return (filename!="");
}

bool ossimDirectory::getNext(ossimFilename &filename) const
{
   struct _finddata_t c_file;
   bool matches = false;
   ossimFilename temp;

   while (!matches )
   {
      // look for next file in the directory
      if (_findnext( theData, &c_file ) == 0 )
      {
         temp = theDirectoryName.dirCat(c_file.name);
         matches = fileMatched(temp);
      }
      else
      {
         // no more file in the directory
         filename.clear();
         return false;
      }
   }

   // set the filenane that matches
   if (matches)
   {
      filename = temp.trim();
   }

   return (matches&&(filename!=""));
}

bool ossimDirectory::fileMatched(ossimFilename &filename) const
{
   bool matches = false;

   // Don't return "." and ".." unless asked for.
   if ( (filename.file() == "..") || (filename.file() == ".")   )
   {
      if (theFlags & ossimDirectory::OSSIM_DIR_DOTDOT)
      {
         matches = true;
      }
   }
   else if((filename.isDir()) && (theFlags & ossimDirectory::OSSIM_DIR_DIRS))
   {
      matches = true;
   }
   else if((filename.isFile()) && (theFlags & ossimDirectory::OSSIM_DIR_FILES))
   {
      matches = true;
   }

   return matches;
}

bool ossimDirectory::isOpened() const
{
   return theDirectoryName.isDir();
    //return theData != 0;
}

#else

ossimDirectory::ossimDirectory()
   :
      theData(NULL)
{}

ossimDirectory::ossimDirectory(const ossimFilename &dirname)
{
    theData = NULL;
    open(dirname);
}

bool ossimDirectory::open(const ossimFilename &dirname)
{
    delete theData;
    theData = new ossimDirectoryData(dirname);

    if ( theData &&
         (!theData->isOk()) )
    {
        delete theData;
        theData = NULL;

        return false;
    }

    return true;
}

ossimDirectory::~ossimDirectory()
{
    delete theData;
}

// ----------------------------------------------------------------------------
// ossimDirectory enumerating
// ----------------------------------------------------------------------------

bool ossimDirectory::getFirst(ossimFilename &filename,
                              int flags)
{
   if(theData && isOpened())
   {
      theData->rewind();

      theData->setFlags(flags);

      return getNext(filename);
   }

   return false;
}

bool ossimDirectory::getNext(ossimFilename &filename) const
{
   if(theData && isOpened())
   {
      return theData->read(filename);
   }

   return false;
}

bool ossimDirectory::isOpened() const
{
    return theData != NULL;
}

#endif

void ossimDirectory::findAllFilesThatMatch(std::vector<ossimFilename>& result,
					   const ossimString& regularExpressionPattern,
					   int flags)
{
   ossimFilename filename;
   ossimRegExp   regExpr;
   regExpr.compile(regularExpressionPattern.c_str());
   if(getFirst(filename, flags))
   {
      do
      {
         ossimString fileOnly = filename.file();
         if(regExpr.find(fileOnly.c_str()))
         {
            result.push_back(filename);
         }
      }while(getNext(filename));
   }
}

// ESH 07/2008, Trac #234: OSSIM is case sensitive 
// when using worldfile templates during ingest
bool ossimDirectory::findCaseInsensitiveEquivalents(
   const ossimFilename &filename, 
   std::vector<ossimFilename>& result,
   bool bExcludeExactMatch )
{
   bool bSuccess = false;
   ossimFilename candidate;
   bool bFoundCandidate = getFirst( candidate );
   int compareSize = static_cast<int>( filename.length() );
   
   while( bFoundCandidate == true )
   {
      // Do a case insensitive string compare
#if defined (_WIN32)
      bool bFoundEquivalent = _strnicmp( filename.c_str(), candidate.c_str(), 
                                         compareSize ) == 0 ? true : false;
#else
      //bool bFoundEquivalent =  strnicmp( filename.c_str(), candidate.c_str(), // 
      //                                         compareSize ) == 0 ? true : false;
      bool bFoundEquivalent =  strncasecmp( filename.c_str(), candidate.c_str(), 
                                            compareSize ) == 0 ? true : false;
#endif
      
      if ( bFoundEquivalent == true )
      {
         bool bFoundExact = ( filename == candidate.c_str() ) ? true : false;
         bool bShouldExclude = ( bFoundExact == true && 
                                 bExcludeExactMatch == true ) ? true : false;
         
         if ( bShouldExclude == false )
         {
            bSuccess = true;
            result.push_back( candidate );
         }
      }
      
      bFoundCandidate = getNext( candidate );
   }
   
   return bSuccess;
}
