//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
// 
// Author: Garrett Potts
//
// Description: 
//
//*************************************************************************
// $Id: ossimDirectory.h 14776 2009-06-25 14:34:06Z dburken $
#ifndef ossimDirectory_HEADER
#define ossimDirectory_HEADER

#include <ossim/base/ossimFilename.h>

#ifndef _WIN32
class ossimDirectoryData;
#endif

class  OSSIMDLLEXPORT ossimDirectory
{
public:
   
   /*!
    * These flags define what kind of filenames is included in the list
    * of files enumerated by GetFirst/GetNext.
    */
   enum
   {
      OSSIM_DIR_FILES     = 0x0001,       // include files
      OSSIM_DIR_DIRS      = 0x0002,       // include directories
      OSSIM_DIR_HIDDEN    = 0x0004,       // include hidden files
      OSSIM_DIR_DOTDOT    = 0x0008,       // include '.' and '..'
      
      // by default, enumerate everything except '.' and '..'
      OSSIM_DIR_DEFAULT   = OSSIM_DIR_FILES | OSSIM_DIR_DIRS | OSSIM_DIR_HIDDEN
   };


   ossimDirectory();
   
   ossimDirectory(const ossimFilename& dir);
   
   ~ossimDirectory();
   
   bool open(const ossimFilename& dir);
   
   bool isOpened() const;
   
   bool getFirst(ossimFilename &filename,
                 int flags = OSSIM_DIR_DEFAULT);
   
   /*!
    * Get next file in the enumeration started with either GetFirst() or
    * GetFirstNormal().
    */
   bool getNext(ossimFilename &filename) const;
   
   void findAllFilesThatMatch(std::vector<ossimFilename>& result,
                              const ossimString& regularExpressionPattern,
                              int flags = OSSIM_DIR_DEFAULT);
   
#if defined (_WIN32)
   void setFlags(int flags) { theFlags = flags; };
   bool fileMatched(ossimFilename &filename) const;
#endif    

   // ESH 07/2008, Trac #234: OSSIM is case sensitive 
   // when using worldfile templates during ingest
   /*!
    * Case insensitive search for files with the same name but 
    * with letters that have different case than the input name. 
    *
    * On Windows there can only be one match, but the case can be 
    * different than the input name. On UNIX there can be multiple 
    * matches.
    *
    * If the bExcludeExactMatch input parameter is set to false, 
    * the original input name will be included in the 'result'
    * vector if it is found in the directory. Otherwise (the default),
    * the input name is excluded from the 'result' vector even if it
    * is found in the directory.
    *
    * Returns true if a name has been added to the result vector.
    */
   bool findCaseInsensitiveEquivalents( const ossimFilename &filename, 
                                        std::vector<ossimFilename>& result,
                                        bool bExcludeExactMatch = true );
   
   // TODO using scandir() when available later, emulating it otherwise
   
private:
#if defined (_WIN32)
   long          theData;
   ossimFilename theDirectoryName;
   int           theFlags;
#else
   ossimDirectoryData *theData;
#endif
};

#endif /* #ifndef ossimDirectory_HEADER */
