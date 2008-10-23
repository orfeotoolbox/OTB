//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimRpfToc.h 12912 2008-05-28 15:05:54Z gpotts $
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

class OSSIM_DLL ossimRpfToc
{
public:
   friend OSSIM_DLL std::ostream& operator <<(std::ostream& out,
                                              const ossimRpfToc& data);
   ossimRpfToc();
   virtual ~ossimRpfToc();

   ossimErrorCode parseFile(const ossimFilename &fileName);
   void print(std::ostream& out)const;
   
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

   ossimRpfHeader*                          theRpfHeader;
};

#endif
