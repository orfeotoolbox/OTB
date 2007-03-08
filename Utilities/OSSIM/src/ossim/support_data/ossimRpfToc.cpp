//*******************************************************************
//
// LICENSE:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfToc.cpp 10278 2007-01-15 19:28:50Z dburken $

#include <iostream>

#include <ossim/support_data/ossimRpfToc.h>
#include <ossim/base/ossimErrorCodes.h>
#include <ossim/base/ossimErrorContext.h>
#include <ossim/support_data/ossimRpfHeader.h>
#include <ossim/support_data/ossimRpfFrameFileIndexSectionSubheader.h>
#include <ossim/support_data/ossimRpfFrameFileIndexSubsection.h>
#include <ossim/support_data/ossimRpfBoundaryRectTable.h>
#include <ossim/support_data/ossimRpfTocEntry.h>
#include <ossim/support_data/ossimRpfFrameFileIndexRecord.h>
#include <ossim/support_data/ossimRpfBoundaryRectSectionSubheader.h>
#include <ossim/support_data/ossimRpfPathnameRecord.h>
#include <ossim/support_data/ossimNitfFile.h>
#include <ossim/support_data/ossimNitfFileHeader.h>
#include <ossim/support_data/ossimNitfEmbeddedRpfHeader.h>
#include <ossim/base/ossimTrace.h>

static ossimTrace traceDebug("ossimRpfToc:debug");

std::ostream& operator <<(std::ostream& out,
                          const ossimRpfToc& data)
{
   data.print(out);
   
   return out;
}

ossimRpfToc::ossimRpfToc()
   :theRpfHeader(NULL)
{
}

ossimRpfToc::~ossimRpfToc()
{
   deleteAll();
}

ossimErrorCode ossimRpfToc::parseFile(const ossimFilename &fileName)
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimRpfToc::parseFile: entered....." << std::endl;
   }
   ossimNitfFile *nitfFile = new ossimNitfFile;

   clearAll();

   nitfFile->parseFile(fileName);

   const ossimRefPtr<ossimNitfFileHeader> nitfFileHeader =
      nitfFile->getHeader();
   
   if(theRpfHeader)
   {
      delete theRpfHeader;
      theRpfHeader = NULL;
   }
   
   if(!nitfFileHeader)
   {
      delete nitfFile;
      nitfFile = NULL;
      
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "ossimRpfToc::parseFile: leaving with no nitf header found....." << std::endl;
      }
      return ossimErrorCodes::OSSIM_ERROR;
   }
   
   ossimNitfTagInformation info; 
   nitfFileHeader->getTag(info, "RPFHDR");
   
   // we no longer need access to the nitf header.  We got what we needed
   delete nitfFile;
   nitfFile = NULL;
   theFilename = fileName;

   if(info.getTagName() == "RPFHDR")
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG: Found RPF HEADER tag" << std::endl;
      }
      theRpfHeader = new ossimRpfHeader;

      std::ifstream in(theFilename.c_str(), std::ios::in|std::ios::binary);
      
      // set the get pointer for the stream to the start
      // of the Rpf header data
      in.seekg(info.getTagDataOffset(), std::ios::beg);
      
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG: parsing  RPFHDR tag" << std::endl;
      }
      // now get the header data.  We do not need to pass in the byte order.
      // this is grabbed from the first byte of the stream.  To see this,
      // Look at the RpfHeader implementation.
      theRpfHeader->parseStream(in);
   }
   else
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "ossimRpfToc::parseFile: Leaving No RPFHDR tag found" << std::endl;
      }
      return ossimErrorCodes::OSSIM_ERROR;
   }
   
   if(theRpfHeader)
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG: Building toc list" << std::endl;
      }
      buildTocEntryList(theRpfHeader);
   }
   else
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "ossimRpfToc::parseFile: Leaving no RPFHDR " << __LINE__ << std::endl;
      }
      return ossimErrorCodes::OSSIM_ERROR;
   }
   
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "ossimRpfToc::parseFile: Returning with OK." << std::endl;
      }
   return ossimErrorCodes::OSSIM_OK;
}

void ossimRpfToc::print(std::ostream& out)const
{
   if(theRpfHeader)
   {
      out << *theRpfHeader << endl;

      std::vector< ossimRpfTocEntry*>::const_iterator tocEntry =
         theTocEntryList.begin();
      while(tocEntry != theTocEntryList.end())
      {
         out << *(*tocEntry) << endl;
         ++tocEntry;
      }
      
   }
}

const ossimRpfTocEntry* ossimRpfToc::getTocEntry(unsigned long index)const
{
   if(index < theTocEntryList.size())
   {
      return theTocEntryList[index];
   }
   return NULL;
}

ossim_int32 ossimRpfToc::getTocEntryIndex(const ossimRpfTocEntry* entry)

{
   for(ossim_int32 i = 0; i < (ossim_int32)theTocEntryList.size(); ++i)
   {
      if(theTocEntryList[i] == entry)
      {
         return i;
      }
   }

   return -1;
}

void ossimRpfToc::deleteAll()
{
   if(theRpfHeader)
   {
      delete theRpfHeader;
      theRpfHeader = NULL;
   }

   deleteTocEntryList();
}

void ossimRpfToc::clearAll()
{
   deleteTocEntryList();
   theFilename = "";
}

void ossimRpfToc::deleteTocEntryList()
{
   std::vector< ossimRpfTocEntry*>::iterator tocEntry =
      theTocEntryList.begin();
   while(tocEntry != theTocEntryList.end())
   {
      if(*tocEntry)
      {
         delete (*tocEntry);
         *tocEntry = NULL;
      }
      ++tocEntry;
   }
   theTocEntryList.clear();
}

void ossimRpfToc::buildTocEntryList(ossimRpfHeader* rpfHeader)
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimRpfToc::buildTocEntryList: entered....." << std::endl;
   }
   std::ifstream in(theFilename.c_str(), std::ios::in | std::ios::binary);

   if(!in)
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "ossimRpfToc::buildTocEntryList: invalid input leaving..... " << std::endl;
      }
      return;
   }
   
   if(rpfHeader)
   {
      deleteTocEntryList();
      ossimRpfBoundaryRectSectionSubheader *boundaryRect = rpfHeader->getNewBoundaryRectSectSubheader(in);
      
      if(boundaryRect)
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG: Got boundary rect " << std::endl;
         }
         long current = in.tellg();

         // they give the offset from the
         // end of the boundary subheader to the start of the 
         // entry table.  We have to create an absolute
         // offset.
         current += boundaryRect->getTableOffset();

         // take to the start of the table entries
         in.seekg(current, ios::beg);
         allocateTocEntryList(boundaryRect->getNumberOfEntries());

         // now we can read the entries
         if(theTocEntryList.size() > 0)
         {
            for(unsigned long index = 0; index < theTocEntryList.size(); index++)
            {
               theTocEntryList[index]->parseStream(in, rpfHeader->getByteOrder());
            }
         }
         
         ossimRpfFrameFileIndexSectionSubheader     *frameFileIndexHead = rpfHeader->getNewFrameFileIndexSectionSubheader(in);
         // get the offset to the table
         long offsetToIndexSubsection = in.tellg();
         if(frameFileIndexHead)
         {
            ossimRpfFrameFileIndexRecord tempIndexRec;
            ossimRpfPathnameRecord       tempPathNameRec;
            
            long count = frameFileIndexHead->getNumberOfIndexRecords();
            while(count > 0)
            {
               tempIndexRec.parseStream(in, rpfHeader->getByteOrder() );

               // get the path information.  we must seek to a different location
               // within the file.  So we must remember where we currently are at
               long currentPosition = in.tellg();
               
               in.seekg(offsetToIndexSubsection + tempIndexRec.getPathnameRecordOffset(), ios::beg);
               tempPathNameRec.parseStream(in, rpfHeader->getByteOrder());

               // We have the root directory where all frame files are subfiles of
               ossimString rootDirectory(ossimFilename(theFilename.path())+
                                           ossimFilename(ossimFilename::thePathSeparator));

               // we have the actual path from the root directory to the frame file.  We must
               // separate the two.  There have been occurrences where the path in the A.TOC file
               // is upper case but the path in the directory on the file system is lower case.  This
               // will fool the system in thinking the file doesnot exist when it actually does.
               ossimString pathToFrameFile( ossimFilename(tempPathNameRec.getPathname()) +
                                              tempIndexRec.getFilename());
               ossimRpfFrameEntry entry(rootDirectory,
                                        pathToFrameFile);
               theTocEntryList[tempIndexRec.getBoundaryRecNumber()]->setEntry(entry,
                                                                              tempIndexRec.getLocationRowNumber(),
                                                                              tempIndexRec.getLocationColNumber());
               // now go back to where we were
               in.seekg(currentPosition, ios::beg);
               
               --count;
            }
         }
      }
      delete boundaryRect;
      boundaryRect = NULL;
   }
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimRpfToc::buildTocEntryList: leaving..... " << std::endl;
   }
}

void ossimRpfToc::allocateTocEntryList(unsigned long numberOfEntries)
{
   if(theTocEntryList.size() > 0)
   {
      deleteTocEntryList();
   }
   for(unsigned long index = 0; index < numberOfEntries; index++)
   {
      theTocEntryList.push_back(new ossimRpfTocEntry);
   }   
}
