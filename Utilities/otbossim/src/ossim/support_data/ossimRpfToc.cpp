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
// $Id: ossimRpfToc.cpp 21214 2012-07-03 16:20:11Z dburken $

#include <ossim/support_data/ossimRpfToc.h>
#include <ossim/base/ossimErrorCodes.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimProperty.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/support_data/ossimNitfFileHeaderV2_X.h>
#include <ossim/support_data/ossimNitfFileHeaderV2_0.h>
#include <ossim/support_data/ossimRpfFrame.h>
#include <ossim/support_data/ossimRpfFrameFileIndexSectionSubheader.h>
#include <ossim/support_data/ossimRpfFrameFileIndexSubsection.h>
#include <ossim/support_data/ossimRpfBoundaryRectTable.h>
#include <ossim/support_data/ossimRpfLocationSection.h>
#include <ossim/support_data/ossimRpfTocEntry.h>
#include <ossim/support_data/ossimRpfFrameFileIndexRecord.h>
#include <ossim/support_data/ossimRpfBoundaryRectSectionSubheader.h>
#include <ossim/support_data/ossimRpfPathnameRecord.h>
#include <ossim/support_data/ossimNitfFile.h>
#include <ossim/base/ossimTrace.h>
#include <iostream>
#include <fstream>

static ossimTrace traceDebug("ossimRpfToc:debug");

std::ostream& operator <<(std::ostream& out, const ossimRpfToc& data)
{
   return data.print(out);
}

ossimRpfToc::ossimRpfToc()
   :m_tocEntryList(),
    m_filename(),
    m_nitfFileHeader(0),
    m_rpfHeader(0)
{
}

ossimRpfToc::~ossimRpfToc()
{
   deleteAll();
}

ossimErrorCode ossimRpfToc::parseFile(const ossimFilename &fileName, bool keepFileHeader)
{
   static const char MODULE[] = "ossimRpfToc::parseFile";
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered....." << std::endl;
   }

   ossimRefPtr<ossimNitfFile> nitfFile = new ossimNitfFile;

   clearAll();

   nitfFile->parseFile(fileName);

   m_nitfFileHeader = nitfFile->getHeader();

   m_rpfHeader = 0; // ossimRefPtr
   
   if( !m_nitfFileHeader.valid() )
   {
      nitfFile = 0;
      
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " leaving with no nitf header found....." << std::endl;
      }
      return ossimErrorCodes::OSSIM_ERROR;
   }
   
   ossimNitfTagInformation info; 
   m_nitfFileHeader->getTag(info, "RPFHDR");

   if(traceDebug() && (info.getTagName() == "RPFHDR") )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " nitf file header print\n";
      m_nitfFileHeader->print( ossimNotify(ossimNotifyLevel_DEBUG) );
   }
   
   if ( !keepFileHeader )
   {
      // we no longer need access to the nitf header.  We got what we needed.
      m_nitfFileHeader = 0;
   }
   nitfFile = 0;
   
   m_filename = fileName;

   if(info.getTagName() == "RPFHDR")
   {
      m_rpfHeader = new ossimRpfHeader;

      std::ifstream in(m_filename.c_str(), std::ios::in|std::ios::binary);
      
      // set the get pointer for the stream to the start
      // of the Rpf header data
      in.seekg(info.getTagDataOffset(), std::ios::beg);
      
      // now get the header data.  We do not need to pass in the byte order.
      // this is grabbed from the first byte of the stream.  To see this,
      // Look at the RpfHeader implementation.
      m_rpfHeader->parseStream(in);

      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " DEBUG: Found RPF HEADER tag\n";
         m_rpfHeader->print( ossimNotify(ossimNotifyLevel_DEBUG) );
      }

   }
   else
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimRpfToc::parseFile: Leaving No RPFHDR tag found" << "\n";
      }
      return ossimErrorCodes::OSSIM_ERROR;
   }
   
   if( m_rpfHeader.valid() )
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG: Building toc list" << "\n";
      }
      buildTocEntryList( m_rpfHeader.get() );
   }
   else
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimRpfToc::parseFile: Leaving no RPFHDR " << __LINE__ << "\n";
      }
      return ossimErrorCodes::OSSIM_ERROR;
   }
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimRpfToc::parseFile: Returning with OK." << std::endl;
   }
   
   return ossimErrorCodes::OSSIM_OK;
}

void ossimRpfToc::createTocAndCopyFrames( const ossimFilename& dotRpfFile,
                                          const ossimFilename& outputDir )
{
   static const char MODULE[] = "ossimRpfToc::createTocAndCopyFrames";

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " entered..."
         << "\ndot rpf file:      " << dotRpfFile
         << "\noutput directory:  " << outputDir
         << "\n";
   }

   if ( outputDir.expand().exists() == false )
   {
      if ( !outputDir.createDirectory(true, 0775) )
      {
         std::string e = MODULE;
         e += " ERROR:\nCould not create directory: ";
         e+= outputDir.c_str();
         throw ossimException(e);
      }
   }

   // Open the dot rpf file.
   std::ifstream* dotRpfStr = new std::ifstream;
   dotRpfStr->open(dotRpfFile.c_str(), ios_base::in);
   if ( !dotRpfStr->good() )
   {
      delete dotRpfStr;
      dotRpfStr = 0;

      std::string e = MODULE;
      e += " ERROR:\nCould not open: ";
      e += dotRpfFile.c_str();
      throw ossimException(e);
   }

   ossimFilename sourceADotTocFile = getSourceTocFile(*dotRpfStr);
   if ( sourceADotTocFile.empty() )
   {
      delete dotRpfStr;
      dotRpfStr = 0;

      std::string e = MODULE;
      e += " ERROR:\nCould not deduce source a.toc file!";
      throw ossimException(e);
   }
   
   // Open the source a.toc file. Note the true flag is to keep the file header.
   ossimRefPtr<ossimRpfToc> sourceADotToc = new ossimRpfToc;
   if ( sourceADotToc->parseFile(sourceADotTocFile, true) != ossimErrorCodes::OSSIM_OK )
   {
      delete dotRpfStr;
      dotRpfStr = 0;
 
      std::string e = MODULE;
      e += " ERROR:\nCould not open: ";
      e += sourceADotTocFile.c_str();
      throw ossimException(e);
   }

   ossimRefPtr<const ossimNitfFileHeader> sourceNitfFileHdr = sourceADotToc->getNitfFileHeader();
   if ( !sourceNitfFileHdr.valid() )
   {
      delete dotRpfStr;
      dotRpfStr = 0;        

      std::string e = MODULE;
      e += " ERROR:\nCould not get nitf file header from: ";
      e += sourceADotTocFile.c_str();
      throw ossimException(e);
   }
   
   ossimRefPtr<const ossimRpfHeader> sourceRpfHdr = sourceADotToc->getRpfHeader();
   if ( !sourceRpfHdr.valid() )
   {
      delete dotRpfStr;
      dotRpfStr = 0;
      
      std::string e = MODULE;
      e += " ERROR:\nCould not get rpf header from: ";
      e += sourceADotTocFile.c_str();
      throw ossimException(e);
   }

   // Get the boundary rect sub header from the source a.toc.
   ossimRefPtr<ossimRpfBoundaryRectSectionSubheader> boundaryRectSectionSubheader =
      sourceRpfHdr->getNewBoundaryRectSectSubheader(sourceADotTocFile);
   if ( !boundaryRectSectionSubheader.valid() )
   {
      delete dotRpfStr;
      dotRpfStr = 0;

      std::string e = MODULE;
      e += " ERROR:\nCould not pull boundary rect sub header from source file: ";
      e += sourceADotTocFile.c_str();
      throw ossimException(e);
   }   

   // Get the boundary rect table from the source a.toc.
   ossimRefPtr<ossimRpfBoundaryRectTable> boundaryRectTable =
      sourceRpfHdr->getNewBoundaryRectTable(sourceADotTocFile);
   if ( !boundaryRectTable.valid() )
   {
      delete dotRpfStr;
      dotRpfStr = 0;
      
      std::string e = MODULE;
      e += " ERROR:\nCould not pull boundary rect table from source file: ";
      e += sourceADotTocFile.c_str();
      throw ossimException(e);
   }
   
   // Get the frame file subheader from the source a.toc.
   ossimRefPtr<ossimRpfFrameFileIndexSectionSubheader> frameFileSubHeader =
      sourceRpfHdr->getNewFrameFileIndexSectionSubheader(sourceADotTocFile);
   if ( !frameFileSubHeader.valid() )
   {
      delete dotRpfStr;
      dotRpfStr = 0;

      std::string e = MODULE;
      e += " ERROR:\nCould not pull frame file sub header from source file: ";
      e += sourceADotTocFile.c_str();
      throw ossimException(e);
   }

   // Get the frame file subsection from the source a.toc.
   ossimRefPtr<ossimRpfFrameFileIndexSubsection> frameFileSubSection =
      sourceRpfHdr->getNewFileIndexSubsection(sourceADotTocFile);
   if ( !frameFileSubSection.valid() )
   {
      delete dotRpfStr;
      dotRpfStr = 0;

      std::string e = MODULE;
      e += " ERROR:\nCould not pull frame file sub section from source file: ";
      e += sourceADotTocFile.c_str();
      throw ossimException(e); 
   }
   
   // Open the output file to write to.
   const ossimFilename A_DOT_TOC_FILE = "a.toc";
   ossimFilename dotTocFile = outputDir.dirCat(A_DOT_TOC_FILE);
   std::ofstream* dotTocStr = new std::ofstream;
   dotTocStr->open( dotTocFile.c_str(), ios::out|ios::binary );
   if ( !dotTocStr->good() )
   {
      delete dotRpfStr;
      dotRpfStr = 0;
      delete dotTocStr;
      dotTocStr =0;

      std::string e = MODULE;
      e += " ERROR:\nCould not open: ";
      e += dotTocFile.c_str();
      throw ossimException(e);
   }
   
   // Variables used throughout:
   ossimRefPtr<ossimProperty> prop = new ossimStringProperty();
   ossimString field;
   ossimString s;
   // std::streampos fileHeaderLength = 0;
   // std::streampos fileLength = 0;
      
   ossimRefPtr<ossimNitfFileHeaderV2_0> fileHdr = new ossimNitfFileHeaderV2_0();
   
   // Set the CLEVEL:
   s = "01";
   fileHdr->setComplexityLevel(s);
   
   // Set the OSTAID:
   prop = sourceNitfFileHdr->getProperty(ossimNitfFileHeaderV2_X::OSTAID_KW);
   fileHdr->setProperty(prop);
   
   // Set the FDT (date):
   fileHdr->setDate();
   
   // Set the FTITLE:
   s = "a.toc";
   fileHdr->setTitle(s);
   
   // Set the FSCLAS:
   prop = sourceNitfFileHdr->getProperty(ossimNitfFileHeaderV2_X::FSCLAS_KW);
   fileHdr->setProperty(prop);
   
   // Set the FSCODE:
   prop = sourceNitfFileHdr->getProperty(ossimNitfFileHeaderV2_X::FSCODE_KW);
   fileHdr->setProperty(prop);
   
   // Set the FSCTLH:
   prop = sourceNitfFileHdr->getProperty(ossimNitfFileHeaderV2_X::FSCTLH_KW);
   fileHdr->setProperty(prop);
   
   // Set the ONAME:
   prop = sourceNitfFileHdr->getProperty(ossimNitfFileHeaderV2_X::ONAME_KW);
   fileHdr->setProperty(prop);
   
   // Set the OPHONE:
   prop = sourceNitfFileHdr->getProperty(ossimNitfFileHeaderV2_X::OPHONE_KW);
   fileHdr->setProperty(prop);
   
   // Add the rpf header.
   ossimRpfHeader* rpfHdr = new ossimRpfHeader( *(sourceRpfHdr.get()) );
   
   ossimRefPtr<ossimNitfRegisteredTag> rpfHdrRp = rpfHdr;
   ossimNitfTagInformation rpfHdrInfo(rpfHdrRp);
   fileHdr->addTag(rpfHdrInfo);
   
   //---
   // Write it out...
   // The first write will be with an rpfheader with no location sections just
   // to see where the end of the file header is.
   //---
   fileHdr->writeStream(*dotTocStr);
   
   //---
   // End of file header. Get the header length. This will also be the
   // start of the location section.
   //---
   std::streampos pos = dotTocStr->tellp();
   std::streamoff locationSectionOffset = pos;
   
   // Set the header length:
   fileHdr->setHeaderLength( static_cast<ossim_uint64>(locationSectionOffset) );
   
   // Set the location of the location section.
   rpfHdr->setLocationSectionPos(locationSectionOffset);

   // Set the file name.
   rpfHdr->setFilename(A_DOT_TOC_FILE);
   
   // Add the component location records to the header.
   ossimRpfLocationSection* locSec = rpfHdr->getLocationSection();
   
   // Clear the records copied from the source a.toc.
   locSec->clearFields();
   
   //---
   // Set the length of the locSec to 74.  The record itself is 14 bytes plus
   // an additional 60 bytes for six location records ten bytes each.
   //---
   const ossim_uint16 LOCATION_SECTION_SIZE = 74;
   locSec->setLocationSectionLength(LOCATION_SECTION_SIZE);
   
   // Set the offset which 14 bytes to get to the first record.
   locSec->setLocationTableOffset(14);
   
   // Six records:
   locSec->setNumberOfComponentLocationRecords(6);
   
   // Each record 10 bytes:
   locSec->setLocationRecordLength(10);
   
   // Don't know the aggregate length yet.
   
   ossimRpfComponentLocationRecord locRec;
   
   // Note: See ossimRpfConstants for enum ossimRpfComponentId
   
   const ossim_uint32 RPFHDR_SIZE = 48;
   const ossim_uint32 LOCATION_SECTION_OFFSET = static_cast<ossim_uint32>(locationSectionOffset);
   const ossim_uint32 BOUNDARY_SUBHEADER_SIZE = 8;
   const ossim_uint32 BOUNDARY_RECORD_SIZE = 132;
   const ossim_uint32 FILE_SUBHEADER_SIZE = 13;
   // const ossim_uint32 = ;
   
   // Record 1 RPFHDR location:
   ossim_uint32 rpfHdrOffset = 0;
   if ( fileHdr->getTag(rpfHdrInfo, "RPFHDR") )
   {
      rpfHdrOffset = rpfHdrInfo.getTagDataOffset();
   }

   locRec.m_componentId = OSSIM_RPF_HEADER_COMPONENT; // 128
   locRec.m_componentLength = RPFHDR_SIZE;
   locRec.m_componentLocation = static_cast<ossim_uint32>(rpfHdrInfo.getTagDataOffset());
   locSec->addComponentRecord(locRec);

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< "rpf hdr offset: " << rpfHdrOffset << "\n";
      locRec.print( ossimNotify(ossimNotifyLevel_DEBUG) );
   }
   
   // Record 2 location section:
   locRec.m_componentId = OSSIM_RPF_LOCATION_COMPONENT; // 129
   locRec.m_componentLength = LOCATION_SECTION_SIZE;
   locRec.m_componentLocation = LOCATION_SECTION_OFFSET;
   locSec->addComponentRecord(locRec);

   if ( traceDebug() )
   {
      locRec.print( ossimNotify(ossimNotifyLevel_DEBUG) );
   }
   
   // Record 3 boundary rect sub header section:
   locRec.m_componentId = OSSIM_RPF_BOUNDARY_RECT_SECTION_SUBHEADER; // 148
   locRec.m_componentLength = BOUNDARY_SUBHEADER_SIZE;
   locRec.m_componentLocation = locRec.m_componentLocation + LOCATION_SECTION_SIZE;
   locSec->addComponentRecord(locRec);

   if ( traceDebug() )
   {
      locRec.print( ossimNotify(ossimNotifyLevel_DEBUG) );
   }
   
   // Capture the location.
   std::streamoff boundaryRectPosition = locRec.m_componentLocation;
   
   // Record 4 boundary rect table:
   locRec.m_componentId = OSSIM_RPF_BOUNDARY_RECT_TABLE; // 149
   locRec.m_componentLength = BOUNDARY_RECORD_SIZE;
   locRec.m_componentLocation = locRec.m_componentLocation + BOUNDARY_SUBHEADER_SIZE;
   locSec->addComponentRecord(locRec);

   if ( traceDebug() )
   {
      locRec.print( ossimNotify(ossimNotifyLevel_DEBUG) );
   }
   
   // Record 5 file index sub header:
   locRec.m_componentId = OSSIM_RPF_FRAME_FILE_INDEX_SECTION_SUBHEADER; // 150
   locRec.m_componentLength = FILE_SUBHEADER_SIZE;
   locRec.m_componentLocation = locRec.m_componentLocation + BOUNDARY_RECORD_SIZE;
   locSec->addComponentRecord(locRec);

   if ( traceDebug() )
   {
      locRec.print( ossimNotify(ossimNotifyLevel_DEBUG) );
   }
    
   // Record 6 file index sub header:
   locRec.m_componentId = OSSIM_RPF_FRAME_FILE_INDEX_SUBSECTION; // 151
   locRec.m_componentLength = 0;  // need to calculate.
   locRec.m_componentLocation = locRec.m_componentLocation + FILE_SUBHEADER_SIZE;
   locSec->addComponentRecord(locRec);

   if ( traceDebug() )
   {
      locRec.print( ossimNotify(ossimNotifyLevel_DEBUG) );
   }
   
   // Seek back and re-write...
   dotTocStr->seekp(0, ios::beg);
   fileHdr->writeStream(*dotTocStr);
   
   dotTocStr->seekp(boundaryRectPosition, ios::beg);

   // Only writing one entry:
   boundaryRectSectionSubheader->setNumberOfEntries(1);

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "writing boundaryRectSectionSubheader:\n" << *(boundaryRectSectionSubheader.get())
         << "\n";
   }

   //---
   // Write the boundary rectangle section.  This includes the subheader and subsection.
   // These coorespond to location records 3 and 4 above.
   //---
   boundaryRectSectionSubheader->writeStream(*dotTocStr);

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) 
         << "Original boundaryRectTable:\n" << *(boundaryRectTable.get()) << "\n";
   }

   ossim_uint32 entry;
   if ( getCorespondingEntry( frameFileSubSection.get(), *dotRpfStr, entry ) )
   {
      ossimRpfBoundaryRectRecord boundaryRectRecord;
      if ( boundaryRectTable->getEntry( entry, boundaryRectRecord) )
      {
         if ( traceDebug() )
         {
            ossimNotify(ossimNotifyLevel_DEBUG) 
               << "writing boundaryRectTable:\n" << boundaryRectRecord << "\n";
         }
         
         boundaryRectRecord.writeStream(*dotTocStr);
      }
      else
      {
         std::string e = MODULE;
         e += " ERROR:\nCould not get bounding rect record for entry: ";
         e += ossimString::toString(entry).c_str();
         throw ossimException(e);
      }
   }
   else
   {
      std::string e = MODULE;
      e += " ERROR:\nCould not deduce entry from frame list!";
      throw ossimException(e);
   }

   frameFileSubHeader->setNumberOfIndexRecords( getNumberOfFrames(*dotRpfStr) );
   frameFileSubHeader->setNumberOfPathnameRecords(1);
   const ossim_uint16 FRAME_FILE_INDEX_RECORD_LENGTH = 33;
   frameFileSubHeader->setIndexRecordLength( FRAME_FILE_INDEX_RECORD_LENGTH );

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "writing frameFileSubHeader:\n" << *(frameFileSubHeader.get()) << "\n";
   }
   frameFileSubHeader->writeStream( *dotTocStr );

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "writing frameFileSubSection:\n";
   }

   std::streamoff frameFileIndexSectionStartPos = dotTocStr->tellp();
   
   writeFrameFileIndexSection(frameFileSubSection.get(), *dotRpfStr, *dotTocStr);
   
   std::streamoff endOfFilePos = dotTocStr->tellp();

   // Update the location section length for the frame file index section.
   locSec->getLocationRecordList()[5].m_componentLength =
      static_cast<ossim_uint32>(endOfFilePos - frameFileIndexSectionStartPos);

   // Update the length of all location sections.
   locSec->setComponentAggregateLength(
      static_cast<ossim_uint32>(endOfFilePos) - rpfHdr->getLocationSectionLocation() );
   

   fileHdr->setFileLength(static_cast<ossim_uint64>(endOfFilePos));
   dotTocStr->seekp(0, ios::beg);
   fileHdr->writeStream(*dotTocStr);

   ossimNotify(ossimNotifyLevel_DEBUG) << "Wrote file: " << dotTocFile << "\n";

   // Copy the frames to the output directory.
   copyFrames(*dotRpfStr, outputDir);

   // Cleanup:
   delete dotRpfStr;
   dotRpfStr = 0;
   delete dotTocStr;
   dotTocStr =0;
}

std::ostream& ossimRpfToc::print(std::ostream& out,
                                 const std::string& prefix,
                                 bool printOverviews) const
{
   if( m_rpfHeader.valid() )
   {
      m_rpfHeader->print(out, prefix);

      //---
      // Go through the entries...  We're going to skip overviews here.
      //---
      ossim_uint32 prefixIndex = 0;
      std::vector< ossimRpfTocEntry*>::const_iterator tocEntry =
         m_tocEntryList.begin();
      while(tocEntry != m_tocEntryList.end())
      {
         if (*tocEntry)
         {
            if ( traceDebug() )
            {
               (*tocEntry)->print(out, prefix);
            }
            
            const ossimRpfBoundaryRectRecord REC =
               (*tocEntry)->getBoundaryInformation();

            ossimString scale = REC.getScale();
            if ( (scale.contains("OVERVIEW")) == false ||
                 printOverviews )
            {
               ossimString entryPrefix = prefix;
               entryPrefix += "image";
               entryPrefix += ossimString::toString(prefixIndex);
               entryPrefix += ".";
               REC.print(out, entryPrefix);

               //---
               // Get the first frame that exists so we can get to
               // the attributes.
               //---
               ossimRpfFrameEntry frameEntry;
               getFirstEntry((*tocEntry), frameEntry);

               if (frameEntry.exists())
               {
                  ossimRpfFrame rpfFrame;
                  if ( rpfFrame.parseFile(frameEntry.getFullPath())
                       == ossimErrorCodes::OSSIM_OK )
                  {
                     rpfFrame.print(out, entryPrefix);
                  }
               }
            }
         }
         ++prefixIndex;
         ++tocEntry;
      }
   }
   return out;
}

ossim_uint32 ossimRpfToc::getNumberOfEntries()const
{
   return static_cast<ossim_uint32>(m_tocEntryList.size());
}

const ossimRpfTocEntry* ossimRpfToc::getTocEntry(ossim_uint32 index)const
{
   if(index < m_tocEntryList.size())
   {
      return m_tocEntryList[index];
   }
   return 0;
}

ossim_int32 ossimRpfToc::getTocEntryIndex(const ossimRpfTocEntry* entry)

{
   for(ossim_int32 i = 0; i < (ossim_int32)m_tocEntryList.size(); ++i)
   {
      if(m_tocEntryList[i] == entry)
      {
         return i;
      }
   }

   return -1;
}

const ossimNitfFileHeader* ossimRpfToc::getNitfFileHeader()const
{
   return m_nitfFileHeader.get();
}

const ossimRpfHeader* ossimRpfToc::getRpfHeader()const
{
   return m_rpfHeader.get();
}

ossim_uint32 ossimRpfToc::getNumberOfFramesHorizontal(ossim_uint32 idx) const
{
   ossim_uint32 nFrames = 0;
   const ossimRpfTocEntry* pEntry = getTocEntry( idx );
   if ( pEntry != NULL )
   {
      nFrames = pEntry->getNumberOfFramesHorizontal();
   }
   return nFrames;
}

ossim_uint32 ossimRpfToc::getNumberOfFramesVertical(ossim_uint32 idx) const
{
   ossim_uint32 nFrames = 0;
   const ossimRpfTocEntry* pEntry = getTocEntry( idx );
   if ( pEntry != NULL )
   {
      nFrames = pEntry->getNumberOfFramesVertical();
   }
   return nFrames;
}

bool ossimRpfToc::getRpfFrameEntry(ossim_uint32 entryIdx, 
                                   ossim_uint32 row,
                                   ossim_uint32 col,
                                   ossimRpfFrameEntry& result)const
{
   const ossimRpfTocEntry* pEntry = getTocEntry( entryIdx );
   if ( pEntry != NULL )
   {
      return pEntry->getEntry( row, col, result );
   }
   return false;
}

const ossimString ossimRpfToc::getRelativeFramePath( ossim_uint32 entryIdx,
                                                     ossim_uint32 row,
                                                     ossim_uint32 col) const
{
   ossimRpfFrameEntry frameEntry;
   bool bResult = getRpfFrameEntry( entryIdx, row, col, frameEntry );
   if ( bResult == true )
   {
      return frameEntry.getPathToFrameFileFromRoot();
   }
   return ossimString("");
}

void ossimRpfToc::deleteAll()
{
   m_rpfHeader = 0;

   deleteTocEntryList();
}

void ossimRpfToc::clearAll()
{
   deleteTocEntryList();
   m_filename = "";
}

void ossimRpfToc::deleteTocEntryList()
{
   std::vector< ossimRpfTocEntry*>::iterator tocEntry =
      m_tocEntryList.begin();
   while(tocEntry != m_tocEntryList.end())
   {
      if(*tocEntry)
      {
         delete (*tocEntry);
         *tocEntry = NULL;
      }
      ++tocEntry;
   }
   m_tocEntryList.clear();
}

void ossimRpfToc::buildTocEntryList(ossimRpfHeader* rpfHeader)
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimRpfToc::buildTocEntryList: entered.....\n";
   }
   std::ifstream in(m_filename.c_str(), std::ios::in | std::ios::binary);

   if(!in)
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimRpfToc::buildTocEntryList: invalid input leaving..... " << std::endl;
      }
      return;
   }
   
   if(rpfHeader)
   {
      if(traceDebug())
      {
         rpfHeader->print(ossimNotify(ossimNotifyLevel_DEBUG));
      }
      
      deleteTocEntryList();
      ossimRpfBoundaryRectSectionSubheader *boundaryRect =
         rpfHeader->getNewBoundaryRectSectSubheader(in);

      if(boundaryRect)
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG: Got boundary rect\n";
         }
         std::streamoff current = in.tellg();

         // they give the offset from the
         // end of the boundary subheader to the start of the 
         // entry table.  We have to create an absolute
         // offset.
         current += boundaryRect->getTableOffset();

         // take to the start of the table entries
         in.seekg(current, ios::beg);
         allocateTocEntryList(boundaryRect->getNumberOfEntries());

         // now we can read the entries
         if(m_tocEntryList.size() > 0)
         {
            for(ossim_uint32 index = 0; index < m_tocEntryList.size(); index++)
            {
               m_tocEntryList[index]->parseStream(in, rpfHeader->getByteOrder());
            }
         }
         
         ossimRpfFrameFileIndexSectionSubheader* frameFileIndexHead = rpfHeader->getNewFrameFileIndexSectionSubheader(in);
         // get the offset to the table
         long offsetToIndexSubsection = in.tellg();
         if(frameFileIndexHead)
         {
            ossimRpfFrameFileIndexRecord tempIndexRec;
            ossimRpfPathnameRecord       tempPathNameRec;
            
            ossim_int32 count = frameFileIndexHead->getNumberOfIndexRecords();
            while(count > 0)
            {
               tempIndexRec.parseStream(in, rpfHeader->getByteOrder() );

               // get the path information.  we must seek to a different location
               // within the file.  So we must remember where we currently are at
               std::streamoff currentPosition = in.tellg();
               
               in.seekg(offsetToIndexSubsection + tempIndexRec.getPathnameRecordOffset(), ios::beg);
               tempPathNameRec.parseStream(in, rpfHeader->getByteOrder());

               // We have the root directory where all frame files are subfiles of
//               ossimString rootDirectory(ossimFilename(m_filename.path())+
               // ossimFilename(ossimFilename::thePathSeparator));
               ossimFilename rootDirectory;
               getRootDirectory(rootDirectory);

               // we have the actual path from the root directory to the
               // frame file.  We must separate the two.  There have been
               // occurrences where the path in the A.TOC file
               // is upper case but the path in the directory on the file
               // system is lower case.  This
               // will fool the system in thinking the file does not exist
               // when it actually does.
               ossimString pathToFrameFile( ossimFilename(tempPathNameRec.getPathname()) +
                                              tempIndexRec.getFilename());

               ossimRpfFrameEntry entry(rootDirectory,
                                        pathToFrameFile);
               m_tocEntryList[tempIndexRec.getBoundaryRecNumber()]->setEntry(entry,
                                                                              tempIndexRec.getLocationRowNumber(),
                                                                              tempIndexRec.getLocationColNumber());
               // now go back to where we were
               in.seekg(currentPosition, ios::beg);
               
               --count;
            }
            delete frameFileIndexHead;
            frameFileIndexHead = 0;
         }
      }
      delete boundaryRect;
      boundaryRect = NULL;
   }
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimRpfToc::buildTocEntryList: leaving....." << std::endl;
   }
}

void ossimRpfToc::allocateTocEntryList(ossim_uint32 numberOfEntries)
{
   if(m_tocEntryList.size() > 0)
   {
      deleteTocEntryList();
   }
   for(ossim_uint32 index = 0; index < numberOfEntries; index++)
   {
      m_tocEntryList.push_back(new ossimRpfTocEntry);
   }   
}

void ossimRpfToc::getRootDirectory(ossimFilename& dir) const
{
   dir = m_filename.expand().path();
}

void ossimRpfToc::getFirstEntry(const ossimRpfTocEntry* rpfTocEntry,
                                ossimRpfFrameEntry& frameEntry) const
{
   if (rpfTocEntry)
   {
      ossim_int32 framesHorizontal =
         rpfTocEntry->getNumberOfFramesHorizontal();
      ossim_int32 framesVertical =
         rpfTocEntry->getNumberOfFramesVertical();
   
      bool foundEntry = false;
      for (ossim_int32 v = 0; v < framesVertical; ++v)
      {
         for (ossim_int32 h = 0; h < framesHorizontal; ++h)
         {
            rpfTocEntry->getEntry(v, h, frameEntry);
            if (frameEntry.exists())
            {
               foundEntry = true;
               break;
            }
         }
         if (foundEntry)
         {
            break;
         }
      }
   }
}

void ossimRpfToc::writeFrameFileIndexSection(ossimRpfFrameFileIndexSubsection* frameFileSubSection,
                                             std::ifstream& dotRpfStr,
                                             std::ofstream& dotTocStr)
{
   static const char MODULE[] = "ossimRpfToc::writeFrameFileIndexSection";

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }

   const ossim_uint16 FRAME_FILE_INDEX_RECORD_LENGTH = 33;
   ossim_uint32 frames = getNumberOfFrames(dotRpfStr);
   ossim_uint32 pathnameRecordOffset = FRAME_FILE_INDEX_RECORD_LENGTH * frames;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "frames: " << frames << "\n";
   }
   
   if ( !dotRpfStr.good() )
   {
      // see if we can clear it.  Someone might have hit end of file(eof).
      dotRpfStr.clear();
   }

   dotRpfStr.seekg(0, ios_base::beg);

   std::string line;
   ossimFilename file;
   ossimRpfFrameFileIndexRecord record;
   ossim_uint32 framesWritten = 0;

   // Eat the first line which is the bounding rect line
   std::getline(dotRpfStr, line);

   while( dotRpfStr.good() )
   {
      std::getline(dotRpfStr, line);

      if ( dotRpfStr.good() )
      {
         if ( getFile(line, file) )
         {
            if ( frameFileSubSection->getFrameFileIndexRecordFromFile(file.file(), record) )
            {
               // Always single entry.
               record.setBoundaryRecNumber(0);
               
               record.setPathnameRecordOffset(pathnameRecordOffset);
               record.writeStream(dotTocStr);
               ++framesWritten;

               if ( traceDebug() )
               {
                  ossimNotify(ossimNotifyLevel_DEBUG) << "wrote record:\n" << record << "\n";
               }
            }
         }
      }
   }

   // Now set the path record.
   ossimFilename d = file.path();
   ossimString s = "./";
   s += d.file();
   s += "/";
   ossimRpfPathnameRecord pathRecord;
   pathRecord.setPathName(s);
   pathRecord.writeStream(dotTocStr);

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "frames written: " << framesWritten
         << "\nwrote record:\n" << pathRecord
         << "\n";
   }

   dotRpfStr.clear();
   dotRpfStr.seekg(0, ios_base::beg);

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " exited..." << std::endl;
   }
}

void ossimRpfToc::copyFrames(std::ifstream& dotRpfStr, const ossimFilename& outputDir)
{
   static const char MODULE[] = "ossimRpfToc::copyFrames";

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }

   ossim_uint32 frames = getNumberOfFrames(dotRpfStr);

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "frames to copy: " << frames << "\n";
   }
   
   if ( !dotRpfStr.good() )
   {
      // see if we can clear it.  Someone might have hit end of file(eof).
      dotRpfStr.clear();
   }

   dotRpfStr.seekg(0, ios_base::beg);

   std::string line;
   ossimFilename file;
   ossimFilename destinationFile;
   ossimFilename subDir;
   ossim_uint32 framesCopied = 0;
            
   // Eat the first line which is the bounding rect line
   std::getline(dotRpfStr, line);

   // Get the second line which is first file. 
   std::getline(dotRpfStr, line);

   // Get the file name and make the sub directory if needed.
   if ( getFile(line, file) )
   {
      destinationFile = outputDir;
      subDir = file.path();
      subDir = subDir.file();
      destinationFile = destinationFile.dirCat( subDir );
      
      // This is output_dir/subdir.  See if subdir exist:
      if ( !destinationFile.exists() )
      {
         destinationFile.createDirectory();
      }
   }

   // Start over:
   if ( !dotRpfStr.good() )
   {
      // see if we can clear it.  Someone might have hit end of file(eof).
      dotRpfStr.clear();
   }
   dotRpfStr.seekg(0, ios_base::beg);
   
   // Eat the first line which is the bounding rect line
   std::getline(dotRpfStr, line);
   
   while( dotRpfStr.good() )
   {
      std::getline(dotRpfStr, line);

      if ( dotRpfStr.good() )
      {
         if ( getFile(line, file) )
         {
            destinationFile = outputDir;
            subDir = file.path();
            subDir = subDir.file();
            destinationFile = destinationFile.dirCat( subDir );
            destinationFile = destinationFile.dirCat( file.file() );

            if ( file.copyFileTo(destinationFile) )
            {
               ++framesCopied;
            }
            if ( traceDebug() )
            {
               ossimNotify(ossimNotifyLevel_DEBUG) << "Copied frame: " << destinationFile << "\n";
            }
         }
      }
   }

   ossimNotify(ossimNotifyLevel_NOTICE) << "Frames copied: " << framesCopied << std::endl;

   dotRpfStr.clear();
   dotRpfStr.seekg(0, ios_base::beg);

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " exited..." << std::endl;
   }
}

ossim_uint32 ossimRpfToc::getNumberOfFrames(std::ifstream& dotRpfStr) const
{
   ossim_uint32 result = 0;

   if ( !dotRpfStr.good() )
   {
      // see if we can clear it.  Someone might have hit end of file(eof).
      dotRpfStr.clear();
   }

   dotRpfStr.seekg(0, ios_base::beg);

   // Eat the first line which is the bounding rect.
   std::string line;
   std::getline(dotRpfStr, line);
   
   while( dotRpfStr.good() )
   {
      std::getline(dotRpfStr, line);

      if ( dotRpfStr.good() )
      {
         ++result;
      }
   }

   dotRpfStr.clear();
   dotRpfStr.seekg(0, ios_base::beg);

   return result;
}

ossimFilename ossimRpfToc::getSourceTocFile(std::ifstream& dotRpfStr) const
{
   ossimFilename tocFile;
   
   if ( !dotRpfStr.good() )
   {
      dotRpfStr.clear();
   }

   dotRpfStr.seekg(0, ios_base::beg);

   // Eat the first line which is the bounding rect.
   std::string line;
   std::getline(dotRpfStr, line);

   // Get the second line which is first file. 
   std::getline(dotRpfStr, line);

   // Get the file name and make the sub directory if needed.
   ossimFilename file;
   if ( getFile(line, file) )
   {
      ossimFilename subDir = file.path();
      tocFile = subDir.dirCat("a.toc");
      if ( !tocFile.exists() )
      {
         tocFile = subDir.dirCat("A.TOC");
         if ( !tocFile.exists() )
         {
            subDir = subDir.path();
            tocFile = subDir.dirCat("a.toc");
            if ( !tocFile.exists() )
            {
               tocFile = subDir.dirCat("A.TOC");
               if ( !tocFile.exists() )
               {
                  tocFile.clear();
               }
            }
         }
      }
   }

   if ( !dotRpfStr.good() )
   {
      dotRpfStr.clear();
   }
   dotRpfStr.seekg(0, ios_base::beg);

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimRpfToc::getSourceTocFile result: " << tocFile << "\n";
   }

   return tocFile;
}

bool ossimRpfToc::getCorespondingEntry(
   ossimRpfFrameFileIndexSubsection* frameFileSubSection,
   std::ifstream& dotRpfStr, ossim_uint32& entry) const
{
   bool result = false;
   
   if ( !dotRpfStr.good() )
   {
      // see if we can clear it.  Someone might have hit end of file(eof).
      dotRpfStr.clear();
   }
   
   dotRpfStr.seekg(0, ios_base::beg);

   std::string line;
   ossimFilename file;
   ossimRpfFrameFileIndexRecord record;

   // Eat the first line which is the bounding rect line
   std::getline(dotRpfStr, line);

   std::getline(dotRpfStr, line);

   if ( getFile(line, file) )
   {
      if ( frameFileSubSection->getFrameFileIndexRecordFromFile(file.file(), record) )
      {
         entry = record.getBoundaryRecNumber();
         result = true;
      }
   }

   if ( !dotRpfStr.good() )
   {
      dotRpfStr.clear();
   }

   dotRpfStr.seekg(0, ios_base::beg);
   return result;
}

bool ossimRpfToc::getFile(const std::string& line, ossimFilename& file) const
{
   //---
   // line form:
   // /some_path/N03E030/006EZC1B.I21|30.4654240835311,3.59074642049858|30.5401678385356,3.65984532949882
   //           <file>                    <lr_lon>           <lr_lat>        <ur_lon>          <ur_lat>
   //---
   
   bool result = false;
   if ( line.size() )
   {
      ossimString os(line);
      std::vector<ossimString> lineArray;
      os.split(lineArray, "|");
      if ( lineArray.size() )
      {
         file = lineArray[0];
         result = true;
      }
   }
   return result;
}
