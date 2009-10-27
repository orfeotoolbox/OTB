//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
// 
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimRpfToc.h 14535 2009-05-18 13:11:55Z dburken $
#ifndef osimRpfToc_HEADER
#define osimRpfToc_HEADER

#include <vector>
#include <iosfwd>

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorContext.h>

#include <ossim/base/ossimFilename.h>

class ossimRpfHeader;
class ossimRpfBoundaryRectTable;
class ossimRpfTocEntry;
class ossimRpfFrameEntry;

class OSSIM_DLL ossimRpfToc
{
public:
   friend OSSIM_DLL std::ostream& operator <<(std::ostream& out,
                                              const ossimRpfToc& data);
   ossimRpfToc();
   ~ossimRpfToc();

   ossimErrorCode parseFile(const ossimFilename &fileName);

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
                       const std::string& prefix=std::string(),
                       bool printOverviews=false) const;
   
   unsigned long getNumberOfEntries()const{return (ossim_uint32)theTocEntryList.size();}
   const ossimRpfTocEntry* getTocEntry(unsigned long index)const;
   /*!
    * Returns -1 if not found.
    */
   ossim_int32 getTocEntryIndex(const ossimRpfTocEntry* entry);
   
   const ossimRpfHeader* getRpfHeader()const{return theRpfHeader;}
   
private:
   void deleteAll();
   void clearAll();
   void deleteTocEntryList();
   void buildTocEntryList(ossimRpfHeader* rpfHeader);
   void allocateTocEntryList(unsigned long numberOfEntries);

   /**
    * @brief Method to get the root directory from the a.toc file name.
    * @param dir This initializes dir.
    */
   void getRootDirectory(ossimFilename& dir) const;

   /** @brief Walks through frames to find the first entry that exists... */
   void getFirstEntry(const ossimRpfTocEntry* rpfTocEntry,
                      ossimRpfFrameEntry& frameEntry) const;

   /*!
    * This will hold a list of table of content entries.  There is one entry
    * per directory.  Each entry will have its geographic coverage.
    * Each directory is then divided into frames.  There could be 30 or
    * more frame images that make up an entire image.
    */
   std::vector<ossimRpfTocEntry*> theTocEntryList;
   
   /*!
    * We will remember the file that we opened
    */
   ossimFilename theFilename;

   ossimRpfHeader* theRpfHeader;
};

#endif
