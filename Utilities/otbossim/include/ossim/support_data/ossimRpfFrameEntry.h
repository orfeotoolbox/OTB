//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
// 
// Author: Garrett Potts
//
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfFrameEntry.h 14241 2009-04-07 19:59:23Z dburken $
#ifndef ossimRpfFrameEntry_HEADER
#define ossimRpfFrameEntry_HEADER

#include <iosfwd>
#include <ossim/base/ossimFilename.h>

class ossimRpfFrameEntry
{
public:
   friend std::ostream& operator<<(std::ostream& out,
                                   const ossimRpfFrameEntry& data);
   ossimRpfFrameEntry(const ossimFilename& rootDirectory=ossimFilename(""),
                      const ossimFilename& pathToFrameFileFromRoot=ossimFilename(""));
   ossimRpfFrameEntry(const ossimRpfFrameEntry& rhs);

   /**
    * @brief print method that outputs a key/value type format adding prefix
    * to keys.
    * @param out String to output to.
    * @param prefix This will be prepended to key.
    * e.g. Where prefix = "nitf." and key is "file_name" key becomes:
    * "nitf.file_name:"
    * @return output stream.
    */
   std::ostream& print(std::ostream& out,
                       const std::string& prefix=std::string()) const;  

   bool exists()const{return theExists;}
   
   void setEntry(const ossimFilename& rootDirectory,
                 const ossimFilename& pathToFrameFileFromRoot);
   
   const ossimFilename& getFullPath()const{return theFullValidPath;}
   const ossimString&   getRootDirectory()const{return theRootDirectory;}
   const ossimString    getPathToFrameFileFromRoot()const
      {
         return thePathToFrameFileFromRoot;
      }
   
private:
   bool theExists;

   ossimFilename theRootDirectory;

   ossimFilename thePathToFrameFileFromRoot;

   ossimFilename theFullValidPath;
};

#endif
