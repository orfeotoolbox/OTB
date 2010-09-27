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
// $Id: ossimRpfFrameEntry.h 18052 2010-09-06 14:33:08Z dburken $
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

   /** @brief copy constructor */
   ossimRpfFrameEntry(const ossimRpfFrameEntry& obj);

   /** @brief operator= */
   const ossimRpfFrameEntry& operator=(const ossimRpfFrameEntry& rhs);
   
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

   bool exists() const;
   
   void setEntry(const ossimFilename& rootDirectory,
                 const ossimFilename& pathToFrameFileFromRoot);
   
   const ossimFilename& getFullPath() const;
   const ossimString&   getRootDirectory() const;
   const ossimString    getPathToFrameFileFromRoot() const;
   
private:
   bool          m_exists;
   ossimFilename m_rootDirectory;
   ossimFilename m_pathToFrameFileFromRoot;
   ossimFilename m_fullValidPath;
};

#endif
