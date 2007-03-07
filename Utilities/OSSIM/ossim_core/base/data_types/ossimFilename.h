//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Ken Melero
//
// Description: This class provides manipulation of filenames.
//
//*************************************************************************
// $Id: ossimFilename.h,v 1.21 2005/11/25 16:14:08 dburken Exp $

#ifndef ossimFilename_HEADER
#define ossimFilename_HEADER

#include "base/common/ossimConstants.h"
#include "ossimString.h"

class OSSIMDLLEXPORT ossimFilename : public ossimString
{
public:
   enum AccessModes
   {
      OSSIM_READ_WRITE = 6,
      OSSIM_READ       = 4,
      OSSIM_WRITE      = 2,
      OSSIM_EXE	       = 1, // The PC docs say that this is ignored
      OSSIM_EXIST      = 0
   };
   ossimFilename();
   ossimFilename(const ossimFilename& src);
   ossimFilename(const ossimString& src);
   ossimFilename(const char* src);

   template <class Iter> ossimFilename(Iter s, Iter e);

   static const ossimFilename NIL;

   bool operator == (const ossimFilename& rhs)const;
   
   bool operator == (const ossimString& rhs)const;
   bool operator == (const char* rhs)const;
   
   void convertBackToForwardSlashes();
   void convertForwardToBackSlashes();
   
   // Methods to test ossimFilename for various states.
   bool        exists()       const;
   bool        isFile()       const;
   bool        isDir()        const;
   bool        isReadable()   const;
   bool        isWritable()   const;
   bool        isExecutable() const;
   ossim_int64 fileSize()     const;
   
   // Methods to access parts of the ossimFilename.

   /**
    * @return "tif" if file is "/data/images/t1.tif".
    *
    * @note The '.' (dot) is not returned.
    */
   ossimString   ext() const;

   /**
    * @return @return "/data/images" if file is "/data/images/t1.tif".
    */
   ossimFilename path() const;

   /**
    * @return @return "c:" if file is "c:\data\images\t1.tif".
    */
   ossimFilename drive() const;
   
   /**
    * @return @return "t1.tif" if file is "/data/images/t1.tif".
    */
   ossimFilename file() const;

   /**
    * @return @return "t1.tif" if file is "/data/images/t1.tif".
    */
   ossimFilename fileNoExtension() const;

   /**
    * @return @return "/data/images/t1" if file is "/data/images/t1.tif".
    *
    * @note  The '.' (dot) is not returned.
    */
   ossimFilename noExtension() const;

   /**
    * Sets the extension of a file name.
    *
    * Given:
    * ossimFilename f = "foo";
    * f.setExtension("tif");
    * f now equals "foo.tif"
    *
    * Given: 
    * ossimFilename f = "foo.jpg";
    * f.setExtension("tif");
    * f now equals "foo.tif"
    * 
    * Given: 
    * ossimFilename f = "foo.jpg";
    * f.setExtension(".tif");
    * f now equals "foo.tif"
    *
    * Given: 
    * ossimFilename f = "foo.";
    * ossimFilename f2;
    * f2 = f.setExtension("tif");
    * f now equals "foo.tif"
    * f2 now equals "foo.tif"
    *
    * @param e Extension to add or replace.
    *
    * @returns a reference to this.
    */
   ossimFilename& setExtension(const ossimString& e);
   
   ossimFilename& setPath(const ossimString& p);
   ossimFilename& setFile(const ossimString& f);
   
   void split(ossimString& drivePart,
              ossimString& pathPart,
              ossimString& filePart,
              ossimString& extPart)const;
   
   void merge(const ossimString& drivePart,
              const ossimString& pathPart,
              const ossimString& filePart,
              const ossimString& extPart);
   /*!
    * Returns file appended onto this string. Path seperator is always placed
    * between this and file.  Returns file if this string is empty.
    * Given: this = /foo  and file  = bar output  = /foo/bar
    * Given: this = /foo/ and file  = bar output  = /foo/bar
    */
   ossimFilename dirCat(const ossimFilename& file) const;

   /*!
    */
   bool createDirectory(bool recurseFlag=true,
                        int perm=0775);

   /*!
    * Removes pathname from filesystem if supported by platform.
    * Return true on success, false on error.
    * If supported will set errno and output equivalent string if one occurs.
    */
   static bool remove(const ossimFilename& pathname);

   /**
    * Will use a wildcard remove.  NOTE:  This is in
    * a regular expression format so if you want all
    * files with prefix myfile to be erased then you would give
    * it <path>/myfile.*
    *
    * The . here is a regular expression '.' character which says any character
    * and the * "is any number of"
    * 
    */ 
   static bool wildcardRemove(const ossimFilename& pathname);
   
   bool remove()const;
   bool wildcardRemove()const;
   
   /*!
    * since windows uses \ for path separation
    * and unix / we need to be consistent.
    */
   static const char thePathSeparator;
};

#endif
