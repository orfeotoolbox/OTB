//*******************************************************************
//
//  License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfFile.cpp 22440 2013-10-15 12:31:04Z gpotts $

#include <ossim/support_data/ossimNitfFile.h>
#include <ossim/support_data/ossimNitfFileHeader.h>
#include <ossim/support_data/ossimNitfImageHeader.h>
#include <ossim/support_data/ossimNitfFileHeaderV2_0.h>
#include <ossim/support_data/ossimNitfFileHeaderV2_1.h>
#include <ossim/support_data/ossimNitfImageHeaderV2_0.h>
#include <ossim/support_data/ossimNitfImageHeaderV2_1.h>
#include <ossim/support_data/ossimNitfTagFactoryRegistry.h>
#include <ossim/support_data/ossimNitfRegisteredTag.h>
#include <ossim/support_data/ossimRpfToc.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimIoStream.h>
#include <fstream>
#include <iostream>
#include <iomanip>


// Static trace for debugging
static ossimTrace traceDebug("ossimNitfFile:debug");

std::ostream& operator <<(std::ostream& out, const ossimNitfFile& data)
{
   return data.print(out);
}

std::ostream& ossimNitfFile::print(std::ostream& out,
                                   const std::string& prefix,
                                   bool printOverviews) const
{
   if(theNitfFileHeader.valid())
   {
      std::string pfx = prefix;
      pfx += "nitf.";
      theNitfFileHeader->print(out, pfx);
      
      ossim_int32 n = theNitfFileHeader->getNumberOfImages();
      for(ossim_int32 idx = 0; idx < n; ++idx)
      {
         ossimNitfImageHeader* ih = getNewImageHeader(idx);
         if(ih)
         {
            bool printIt = true;
            
            if ( !printOverviews )
            {
               // Check the IMAG field.
               ossim_float64 imag;
               ih->getDecimationFactor(imag);
               if ( !ossim::isnan(imag) )
               {
                  if ( imag < 1.0)
                  {
                     printIt = false;
                  }
               }

               //---
               // Now see if it's a cloud mask image.  Do not print
               // cloud mask images if the printOverviews is false.
               //---
               if ( printIt )
               {
                  if ( (ih->getCategory().trim(ossimString(" ")) ==
                        "CLOUD") &&
                       (ih->getRepresentation().trim(ossimString(" ")) ==
                        "NODISPLY") )
                  {
                     printIt = false;
                  }
               }
            }

            if (printIt)
            {
               // Add our prefix onto prefix.
               std::string s = pfx;
               s += "image";
               s += ossimString::toString(idx).string();
               s += ".";
               
               ih->print(out, s);
            }
            
            delete ih;
            ih = 0;
         }
      }

      //---
      // Check for RPF stuff:
      //---
      ossimNitfTagInformation info; 
      theNitfFileHeader->getTag(info, "RPFHDR");
      if(info.getTagName() == "RPFHDR")
      {
         // Open of the a.toc.
         ossimRefPtr<ossimRpfToc> toc = new ossimRpfToc;
         if ( toc->parseFile(getFilename()) ==
              ossimErrorCodes::OSSIM_OK )
         {
            pfx += "rpf.";
            toc->print(out, pfx, printOverviews);
         }
      }
      
   } // matches:  if(theNitfFileHeader.valid())

   return out;
   
}

bool ossimNitfFile::saveState(ossimKeywordlist& kwl, const ossimString& prefix)const
{
   bool result = theNitfFileHeader.valid(); 
   
   if(theNitfFileHeader.valid())
   {
      theNitfFileHeader->saveState(kwl, prefix);
   }
   ossim_int32 n = theNitfFileHeader->getNumberOfImages();
   for(ossim_int32 idx = 0; idx < n; ++idx)
   {
      ossimRefPtr<ossimNitfImageHeader> ih = getNewImageHeader(idx);
      ossimString newPrefix = prefix + "image" + ossimString::toString(idx) + ".";
#if 1     
      ih->saveState(kwl, newPrefix);
#else
      if ( (ih->getCategory().trim(ossimString(" ")) !=
            "CLOUD") ||
          (ih->getRepresentation().trim(ossimString(" ")) !=
           "NODISPLY") )
      {
         ih->saveState(kwl, newPrefix);
      }
#endif
   }
   return result;
}

ossimNitfFile::ossimNitfFile()
   : theFilename(""),
     theNitfFileHeader(0)
{
}

ossimNitfFile::~ossimNitfFile()
{
   theNitfFileHeader = 0;
}

bool ossimNitfFile::parseFile(const ossimFilename& file)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimNitfFile::parseFile: "
         << "endtered......"
         << std::endl;
   }
   if(!file.exists()) return false;
   // this currently crashes if the open fails so I am going back to
   // using ifstream instead of this one
   //

   //ossimIFStream64 in(file.c_str(), std::ios::in|std::ios::binary);

   std::ifstream in(file.c_str(), std::ios::in|std::ios::binary);
   if (in.fail())
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimNitfFile::parseFile: "
            << "Could not open file:  " << file.c_str()
            << "\nReturning..." << std::endl;
      }
      return false;
   }
      
   if(theNitfFileHeader.valid())
   {
      theNitfFileHeader = 0;
   }

   char temp[10];
   in.read(temp, 9);
   //in.seekg64(0, std::ios::beg);
   in.seekg(0, std::ios::beg);
   temp[9] ='\0';
   
   theFilename = file;

   ossimString s(temp);
   if(s == "NITF02.00")
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG: NITF Version 2.0"
            << std::endl;
      }
      theNitfFileHeader = new ossimNitfFileHeaderV2_0;
   }
   else if ( (s == "NITF02.10") || (s == "NSIF01.00") )
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG: NITF Version 2.1"
            << std::endl;
      }
      theNitfFileHeader = new ossimNitfFileHeaderV2_1;
   }
   else
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimNitfFile::parseFile: "
            << "Not an NITF file!"
            << std::endl;
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimNitfFile::parseFile: returning...........false"
            << "endtered......"
            << std::endl;
      }
      return false;
   }

   if(theNitfFileHeader.valid())
   {
      try
      {
         theNitfFileHeader->parseStream(in);

         // Sanity check the size before going on:

#if 0                                                                   \
   /*
    * Temp removed for release as workaround for VS10 >2gb offset issue.
    * drb 20130418
    */
         if ( file.fileSize() < theNitfFileHeader->getFileSize() )
         {
            if (traceDebug())
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << "ossimNitfFile::parseFile ERROR:\n"
                  << "File size is less than file length in header!"
                  << "\nNITF FL field: " << theNitfFileHeader->getFileSize()
                  << "\nActual file length: " << file.fileSize()
                  << std::endl;
            }
            return false;
         }
#endif
         
      }
      catch( const ossimException& e )
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "ossimNitfFile::parseFile caught exception:\n"
               << e.what()
               << std::endl;
         }
         return false;
      }
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimNitfFile::parseFile: returning...........true"
         << std::endl;
   }
   return true;
}

const ossimNitfFileHeader* ossimNitfFile::getHeader() const
{
   return theNitfFileHeader.get();
}

ossimNitfFileHeader* ossimNitfFile::getHeader() 
{
   return theNitfFileHeader.get();
}

ossimIrect ossimNitfFile::getImageRect()const
{
   if(theNitfFileHeader.valid())
   {
      return theNitfFileHeader->getImageRect();
   }

   return ossimIrect(ossimIpt(0,0), ossimIpt(0,0));
}

ossimNitfImageHeader* ossimNitfFile::getNewImageHeader(
   ossim_uint32 imageNumber)const
{
   ossimNitfImageHeader* result = 0;
   if(theNitfFileHeader.valid())
   {
      try // getNewImageHeader can throw exception on parse.
      {
         std::ifstream in(theFilename.c_str(), std::ios::in|std::ios::binary);
         result = theNitfFileHeader->getNewImageHeader(imageNumber, in);
         in.close();
      }
      catch( const ossimException& e )
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "ossimNitfFile::getNewImageHeader caught exception:\n"
               << e.what()
               << std::endl;
         }
         result = 0;
      }
   }
   return result;
}

ossimNitfSymbolHeader* ossimNitfFile::getNewSymbolHeader(
   ossim_uint32 symbolNumber)const
{
   ossimNitfSymbolHeader* result = 0;
   if(theNitfFileHeader.valid())
   {
      std::ifstream in(theFilename.c_str(), std::ios::in|std::ios::binary);

      result = theNitfFileHeader->getNewSymbolHeader(symbolNumber, in);
      in.close();
   }
   
   return result;
}

ossimNitfLabelHeader* ossimNitfFile::getNewLabelHeader(
   ossim_uint32 labelNumber)const
{
   ossimNitfLabelHeader* result = 0;
   if(theNitfFileHeader.valid())
   {
      std::ifstream in(theFilename.c_str(), std::ios::in|std::ios::binary);

      result = theNitfFileHeader->getNewLabelHeader(labelNumber, in);
      in.close();
   }
   
   return result;
}

ossimNitfTextHeader* ossimNitfFile::getNewTextHeader(
   ossim_uint32 textNumber)const
{
   ossimNitfTextHeader* result = 0;
   if(theNitfFileHeader.valid())
   {
      std::ifstream in(theFilename.c_str(), std::ios::in|std::ios::binary);

      result = theNitfFileHeader->getNewTextHeader(textNumber, in);
      in.close();
   }
   
   return result;
}

ossimNitfDataExtensionSegment* ossimNitfFile::getNewDataExtensionSegment(
   ossim_uint32 dataExtNumber)const
{
   ossimNitfDataExtensionSegment* result = 0;
   if(theNitfFileHeader.valid())
   {
      std::ifstream in(theFilename.c_str(), std::ios::in|std::ios::binary);

      result = theNitfFileHeader->getNewDataExtensionSegment(dataExtNumber, in);
      in.close();
   }
   
   return result;
}

ossimString ossimNitfFile::getVersion()const
{
   if(theNitfFileHeader.valid())
   {
      return ossimString(theNitfFileHeader->getVersion());
   }
   
   return ossimString("");
}

ossimFilename ossimNitfFile::getFilename() const
{
   return theFilename;
}
