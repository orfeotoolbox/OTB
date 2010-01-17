//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: J2K Info object.
// 
//----------------------------------------------------------------------------
// $Id$
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ossim/support_data/ossimJ2kInfo.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/support_data/ossimJ2kCodRecord.h>
#include <ossim/support_data/ossimJ2kSizRecord.h>
#include <ossim/support_data/ossimJ2kSotRecord.h>

// Static trace for debugging
static ossimTrace traceDebug("ossimJ2kInfo:debug");
static ossimTrace traceDump("ossimJ2kInfo:dump"); // This will dump offsets.

static const ossim_uint16 SOC_MARKER = 0xff4f; // start of codestream marker
static const ossim_uint16 SIZ_MARKER = 0xff51; // size maker
static const ossim_uint16 COD_MARKER = 0xff52; // cod maker
static const ossim_uint16 SOT_MARKER = 0xff90; // start of tile marker
static const ossim_uint16 EOC_MARKER = 0xffd9; // End of codestream marker.

ossimJ2kInfo::ossimJ2kInfo()
   : ossimInfoBase(),
     theFile(),
     theEndian(0)
{
}

ossimJ2kInfo::~ossimJ2kInfo()
{
   if (theEndian)
   {
      delete theEndian;
      theEndian = 0;
   }
}

bool ossimJ2kInfo::open(const ossimFilename& file)
{
   bool result = false;

   //---
   // Open the file.
   //---
   std::ifstream str(file.c_str(), std::ios_base::binary|std::ios_base::in);
   if (str.good()) 
   {
      if (ossim::byteOrder() == OSSIM_LITTLE_ENDIAN)
      {
         if (!theEndian)
         {
            theEndian = new ossimEndian();
         }
      }
      else if (theEndian)
      {
         delete theEndian;
         theEndian = 0;
      }

      //---
      // Check for the Start Of Codestream (SOC) and Size (SIZ) markers which
      // are required as first and second fields in the main header.
      //---
      ossim_uint16 soc;
      ossim_uint16 siz;
      readShort(soc, str);
      readShort(siz, str);

      if ( (soc == SOC_MARKER) && (siz == SIZ_MARKER) )
      {
         result = true; // Is a j2k...
      }
   }

   if (result)
   {
      theFile = file;
   }
   else
   {
      theFile.clear();
      if (theEndian)
      {
         delete theEndian;
         theEndian = 0;
      }
   }

   return result;
}

std::ostream& ossimJ2kInfo::print(std::ostream& out) const
{
   static const char MODULE[] = "ossimJ2kInfo::print";

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " DEBUG Entered...\n";
   }
      
   //---
   // Open the tif file.
   //---
   std::ifstream str(theFile.c_str(), std::ios_base::binary|std::ios_base::in);
   if (str.good())
   {
      ossim_uint16 marker;
      readShort(marker, str); // SOC
      readShort(marker, str); // SIZ

      std::string prefix = "j2k.";

      // SIZ marker required next.
      printSizMarker(out, prefix, str);

      readShort(marker, str);
      
      while ( str.good() && (marker != EOC_MARKER) ) // marker != SOT_MARKER )
      {
         switch(marker)
         {
            case COD_MARKER:
            {
               printCodMarker(out, prefix, str);
               break;
            }
            case SOT_MARKER:
            {
               printSotMarker(out, prefix, str);
               break;
            }
            default:
            {
               printUnknownMarker(out, prefix, str, marker);
            }
         }

         readShort(marker, str);
        
      }
   }
   else
   {
      if (traceDebug())
      {
         out << MODULE << " Cannot open file:  " << theFile << std::endl;
      }
   }

   return out;
}

void ossimJ2kInfo::readShort(ossim_uint16& s, std::ifstream& str) const
{
   str.read((char*)&s, sizeof(s));
   if (theEndian)
   {
      theEndian->swap(s);
   }
}

std::ostream& ossimJ2kInfo::printCodMarker(std::ostream& out,
                                           const std::string& prefix,
                                           std::ifstream& str) const
{
   ossimJ2kCodRecord siz;
   siz.parseStream(str);
   siz.print(out, prefix);
   return out;
}

std::ostream& ossimJ2kInfo::printSizMarker(std::ostream& out,
                                           const std::string& prefix,
                                           std::ifstream& str) const
{
   ossimJ2kSizRecord siz;
   siz.parseStream(str);
   siz.print(out, prefix);
   return out;
}

std::ostream& ossimJ2kInfo::printSotMarker(std::ostream& out,
                                           const std::string& prefix,
                                           std::ifstream& str) const
{
   // Get the stream posistion.
   std::streamoff pos = str.tellg();
   
   ossimJ2kSotRecord sot;
   sot.parseStream(str);
   pos += sot.thePsot - 2;

   // Seek past the tile to the next marker.
   str.seekg(pos, std::ios_base::beg);
   
   sot.print(out,prefix);
   return out;
}


std::ostream& ossimJ2kInfo::printUnknownMarker(std::ostream& out,
                                               const std::string& prefix,
                                               std::ifstream& str,
                                               ossim_uint16 marker) const
{
   // Capture the original flags.
   std::ios_base::fmtflags f = out.flags();

   ossim_uint16 segmentLength;
   readShort(segmentLength, str);

   std::string pfx = prefix;
   pfx += "unkown.";

   out.setf(std::ios_base::hex, std::ios_base::basefield);
   out << pfx << "marker: 0x" << std::setfill('0') << std::setw(4)
       << marker << "\n";
   out.setf(std::ios_base::fmtflags(0), std::ios_base::basefield);
   
   out << pfx << "length: " << segmentLength
       << std::endl;

   // Reset flags.
   out.setf(f);

   // Seek to the next marker.
   str.seekg( (segmentLength-2), std::ios_base::cur);

   return out;
}
      

