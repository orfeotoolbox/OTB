//*******************************************************************
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
// $Id: ossimFilename.h 20192 2011-10-25 17:27:25Z dburken $

#ifndef ossimFilename_HEADER
#define ossimFilename_HEADER

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimString.h>

class ossimLocalTm;

class OSSIM_DLL ossimFilename : public ossimString
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
   ossimFilename(const std::string& src);
   ossimFilename(const char* src);

   template <class Iter> ossimFilename(Iter s, Iter e);

   static const ossimFilename NIL;

   bool operator == (const ossimFilename& rhs)const;
   
   bool operator == (const ossimString& rhs)const;
   bool operator == (const char* rhs)const;

   /** @brief Writes f to the output stream os. */
//    friend std::ostream& operator<<(std::ostream& os,
   //                                const ossimFilename& s);
   
   void convertBackToForwardSlashes();
   void convertForwardToBackSlashes();

   bool setTimes(ossimLocalTm* accessTime,
                 ossimLocalTm* modTime,
                 ossimLocalTm* createTime)const;
   bool getTimes(ossimLocalTm* accessTime,
                 ossimLocalTm* modTime,
                 ossimLocalTm* createTime)const;
   
   bool touch()const;
   
   /**
    * Method to do file name expansion.
    *
    * Like: ~/docs will be expanded to /home/user/docs
    *
    * @return The expanded file name.
    *
    * @note METHOD IS NOT COMPLETE YET.
    */
   ossimFilename expand() const;
   
   // Methods to test ossimFilename for various states.
   bool        exists()       const;
   bool        isFile()       const;
   bool        isDir()        const;
   bool        isReadable()   const;
   bool        isWriteable()  const;
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
    * @return @return "t1" if file is "/data/images/t1.tif".
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

   /**
	* Sets the file path and drive.
	* Input: the drive to be set (this should come from the user's preferences)
	* Example: "x:"
	* Given: "/filepath/file.ext"
    * @return @return "c:\filepath\file.ext".
    */
   ossimFilename& setDrive(const ossimString& d);
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
                        int perm=0775)const;

   /*!
    * @brief Removes pathname from filesystem if supported by platform.
    *
    * If pathname is a directory it will only be removed if empty. If supported
    * will set errno and output equivalent string if one occurs.
    *
    * @return true on success, false on error.
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

   bool rename(const ossimFilename& destFile, bool overwriteDestinationFlag=true)const;
   
   bool remove()const;
   bool wildcardRemove()const;

   /**
    * @brief Copies this file to output file.
    *
    * Copies this to output file. If output file is a directory, this->file() is
    * appended to output file.
    * 
    * @param outputFile File name to copy to.
    * 
    * @return True on success, false on error.
    */
   bool copyFileTo(const ossimFilename& ouputFile) const;

   /**
    * @brief Checks whether file name is relative or absolute.
    * 
    * @return true if file name has a relative path, false if absolute.
    *
    * @note This will return true if file name is empty.
    */
   bool isRelative() const;
   
   /**
    * @brief Method to check if expansion is needed.
    *
    * This checks if file isRelative() first if isRelative is false (has
    * absolute path) it then walks the file and looks for '$'.
    * 
    * @return true if file name is relative or has environment vars.
    *
    * @note This will return false if file name is empty.
    */
   bool needsExpansion() const;

   /** @return The path separator. */
   char getPathSeparator() const;
   
protected:

   void convertToNative();

   /*!
    * since windows uses \ for path separation
    * and unix / we need to be consistent.
    */
   static const char thePathSeparator;

};

// inline std::ostream& operator<<(std::ostream& os, const ossimFilename& f)
// {
//   return os << f.string().c_str();
// }

#endif
