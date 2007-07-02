//*****************************************************************************
// FILE: ossimDtedHandler.cc
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// DESCRIPTION:
//   Contains implementation of class ossimDtedHandler. This class derives from
//   ossimElevHandler. It is responsible for loading an individual DTED cell
//   from disk. This elevation files are memory mapped.
//
//*****************************************************************************
// $Id: ossimDtedHandler.cpp 10215 2007-01-09 16:56:41Z dburken $

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ossim/elevation/ossimDtedHandler.h>
#include <ossim/support_data/ossimDtedVol.h>
#include <ossim/support_data/ossimDtedHdr.h>
#include <ossim/support_data/ossimDtedUhl.h>
#include <ossim/support_data/ossimDtedDsi.h>
#include <ossim/support_data/ossimDtedAcc.h>
#include <ossim/support_data/ossimDtedRecord.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimKeywordlist.h>

RTTI_DEF1(ossimDtedHandler, "ossimDtedHandler" , ossimElevCellHandler)

//***
// Define Trace flags for use within this file:
//***
#include <ossim/base/ossimTrace.h>
static ossimTrace traceExec  ("ossimDtedHandler:exec");
static ossimTrace traceDebug ("ossimDtedHandler:debug");

static const char ENABLE_STATS_KW[] = "elevation.compute_statistics.enabled";


ossimDtedHandler::ossimDtedHandler(const ossimFilename& dted_file)
   :
      ossimElevCellHandler(dted_file),
      theFileStr(),
      theNumLonLines(0),
      theNumLatPoints(0),
      theDtedRecordSizeInBytes(0),
      theEdition(),
      theProductLevel(),
      theCompilationDate(),
      theOffsetToFirstDataRecord(0),
      theLatSpacing(0.0),
      theLonSpacing(0.0),
      theSwCornerPost(),
      theSwapBytesFlag(false)
{
   static const char MODULE[] = "ossimDtedHandler (Filename) Constructor";
   if (traceExec())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG " << MODULE <<": entering..." << std::endl;
   }
 
   //---
   //  Open the dted file for reading.
   //---
   if (!open())
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_FATAL)
            << "FATAL " << MODULE << ": "
            << "\nCould not open file:  " << dted_file.c_str()
            << "\nReturning..." << std::endl;
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG " << MODULE << ": returning with error..." << std::endl;
      }
      return;
   }
   else
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG " << MODULE <<": Loading dted file: " << dted_file
            << std::endl;
      }
   }

   // DTED is stored in big endian.
   theSwapBytesFlag = ossimGetByteOrder() == OSSIM_LITTLE_ENDIAN ? true : false;

   // Attempt to parse.
   ossimDtedVol vol(theFileStr, 0);
   ossimDtedHdr hdr(theFileStr, vol.stopOffset());
   ossimDtedUhl uhl(theFileStr, hdr.stopOffset());
   ossimDtedDsi dsi(theFileStr, uhl.stopOffset());
   ossimDtedAcc acc(theFileStr, dsi.stopOffset());

   //***
   // Check for errors.  Must have uhl, dsi and acc records.  vol and hdr
   // are for magnetic tape only; hence, may or may not be there.
   //***
   if (uhl.getErrorStatus() == ossimErrorCodes::OSSIM_ERROR ||
       dsi.getErrorStatus() == ossimErrorCodes::OSSIM_ERROR ||
       acc.getErrorStatus() == ossimErrorCodes::OSSIM_ERROR)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG " << MODULE << ": "
            << "\nError parsing file:  " << dted_file.c_str()
            << "\nPossibly not a dted file."
            << std::endl;
      }
      
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      theFileStr.close();
      return;
   }
   
   theNumLonLines  = uhl.numLonLines();
   theNumLatPoints = uhl.numLatPoints();
   theLatSpacing   = uhl.latInterval();
   theLonSpacing   = uhl.lonInterval();
   theDtedRecordSizeInBytes = theNumLatPoints*2+ossimDtedRecord::DATA_LENGTH;

   theEdition  = dsi.edition();
   theProductLevel = dsi.productLevel();
   theCompilationDate = dsi.compilationDate();

   theOffsetToFirstDataRecord = acc.stopOffset();
   
   //***
   //  initialize the bounding rectangle:
   //***
   double south_boundary = uhl.latOrigin();
   double west_boundary  = uhl.lonOrigin();
   double north_boundary = south_boundary + theLatSpacing*(theNumLatPoints-1);
   double east_boundary  = west_boundary  + theLonSpacing*(theNumLonLines-1);
                         
   theGroundRect = ossimGrect(ossimGpt(north_boundary, west_boundary, 0.0),
                              ossimGpt(south_boundary, east_boundary, 0.0));

   theSwCornerPost.lat = south_boundary;
   theSwCornerPost.lon = west_boundary;

   //***
   //  Determine the mean spacing:
   //***
   double center_lat = (south_boundary + north_boundary)/2.0;
   theMeanSpacing = (theLatSpacing + theLonSpacing*cosd(center_lat))
                    * ossimGpt().metersPerDegree().x / 2.0;

   //  Initialize the accuracy values:
   theAbsLE90 = (double) acc.absLE();
   theAbsCE90 = (double) acc.absCE();

   // Set the base class null height value.
   theNullHeightValue = -32767.0;

   // Get the statistics.
   gatherStatistics();
   
   if (traceExec())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG " << MODULE << ": returning..." << std::endl;
   }
}

ossimDtedHandler::~ossimDtedHandler()
{
   theFileStr.close();
}

double ossimDtedHandler::getHeightAboveMSL(const ossimGpt& gpt)
{
   if(theMemoryMap.size() < 1)
   {
      return getHeightAboveMSLFile(gpt);
   }

   return getHeightAboveMSLMemory(gpt);
}

double ossimDtedHandler::getHeightAboveMSLFile(const ossimGpt& gpt)
{
   // Establish the grid indexes:
   double xi = (gpt.lon - theSwCornerPost.lon) / theLonSpacing;
   double yi = (gpt.lat - theSwCornerPost.lat) / theLatSpacing;

   // Check for right edge.
   int x0 = static_cast<int>(xi);
   int y0 = static_cast<int>(yi);

   // Check for right edge.
   if (gpt.lon == theGroundRect.lr().lon)
   {
      --x0; // Move over one post.
   }
   
   // Check for top edge.
   if (gpt.lat == theGroundRect.ul().lat)
   {
      --y0; // Move down one post.
   }

   // Do some error checking.
   if ( xi < 0.0 || yi < 0.0 ||
        x0 > (theNumLonLines  - 2.0) ||
        y0 > (theNumLatPoints - 2.0) )
   {
      return theNullHeightValue;
   }

   // We need an open stream to file from here on out...
   if (!open())
   {
      return theNullHeightValue;
   }

   double p00;
   double p01;
   double p10;
   double p11;

   //***
   // Grab the four points from the dted cell needed.  These are big endian,
   // signed magnitude shorts so they must be interpreted accordingly.
   //***
   int offset = theOffsetToFirstDataRecord + x0 * theDtedRecordSizeInBytes +
                y0 * 2 + DATA_RECORD_OFFSET_TO_POST;

   // Put the file pointer at the start of the first elevation post.
   theFileStr.seekg(offset, std::ios::beg);

   ossim_sint16 ss;
   ossim_uint16 us;

   // Get the first post.
   theFileStr.read((char*)&us, POST_SIZE);
   ss = convertSignedMagnitude(us);
   p00 = ss;

   // Get the second post.
   theFileStr.read((char*)&us, POST_SIZE);
   ss = convertSignedMagnitude(us);
   p01 = ss;

   // Move over to the next column.
   offset += theDtedRecordSizeInBytes;
   theFileStr.seekg(offset, std::ios::beg);

   // Get the third post.
   theFileStr.read((char*)&us, POST_SIZE);
   ss = convertSignedMagnitude(us);
   p10 = ss;

   // Get the fourth post.
   theFileStr.read((char*)&us, POST_SIZE);
   ss = convertSignedMagnitude(us);
   p11 = ss;
   
   // Perform bilinear interpolation:
   double wx1 = xi  - x0;
   double wy1 = yi  - y0;
   double wx0 = 1.0 - wx1;
   double wy0 = 1.0 - wy1;
   
   double w00 = wx0*wy0;
   double w01 = wx0*wy1;
   double w10 = wx1*wy0;
   double w11 = wx1*wy1;

   //***
   // Test for null posts and set the corresponding weights to 0:
   //***
   if (p00 == NULL_POST)
      w00 = 0.0;
   if (p01 == NULL_POST)
      w01 = 0.0;
   if (p10 == NULL_POST)
      w10 = 0.0;
   if (p11 == NULL_POST)
      w11 = 0.0;

#if 0 /* Serious debug only... */
   cout << "\np00:  " << p00
        << "\np01:  " << p01
        << "\np10:  " << p10
        << "\np11:  " << p11
        << "\nw00:  " << w00
        << "\nw01:  " << w01
        << "\nw10:  " << w10
        << "\nw11:  " << w11
        << std::endl;
#endif

   double sum_weights = w00 + w01 + w10 + w11;

   if (sum_weights)
   {
      return (p00*w00 + p01*w01 + p10*w10 + p11*w11) / sum_weights;
   }
   
   return NULL_POST;
}

double ossimDtedHandler::getHeightAboveMSLMemory(const ossimGpt& gpt)
{
   ossim_uint8* buf = &theMemoryMap.front();
   // Establish the grid indexes:
   double xi = (gpt.lon - theSwCornerPost.lon) / theLonSpacing;
   double yi = (gpt.lat - theSwCornerPost.lat) / theLatSpacing;

   // Check for right edge.
   int x0 = static_cast<int>(xi);
   int y0 = static_cast<int>(yi);

   // Check for right edge.
//    if (gpt.lon == theGroundRect.lr().lon)
   if(x0 == (theNumLonLines-1))
   {
      --x0; // Move over one post.
   }
   
   // Check for top edge.
//    if (gpt.lat == theGroundRect.ul().lat)
   if(y0 == (theNumLatPoints-1))
   {
      --y0; // Move down one post.
   }

   // Do some error checking.
   if ( xi < 0.0 || yi < 0.0 ||
        x0 > (theNumLonLines  - 2.0) ||
        y0 > (theNumLatPoints - 2.0) )
   {
      return theNullHeightValue;
   }



   double p00;
   double p01;
   double p10;
   double p11;

   //***
   // Grab the four points from the dted cell needed.  These are big endian,
   // signed magnitude shorts so they must be interpreted accordingly.
   //***
   ossim_uint64 offset = theOffsetToFirstDataRecord + x0 * theDtedRecordSizeInBytes +
                         y0 * 2 + DATA_RECORD_OFFSET_TO_POST;

   // Put the file pointer at the start of the first elevation post.
//   theFileStr.seekg(offset, std::ios::beg);

//    ossim_sint16 ss;
    ossim_uint16 us;

   // Get the first post.
   //theFileStr.read((char*)&us, POST_SIZE);
//   ss = convertSignedMagnitude(us);
    memcpy(&us, buf+offset, POST_SIZE); 
    p00 = convertSignedMagnitude(us);
    memcpy(&us, buf+offset+POST_SIZE, POST_SIZE); 
    p01 = convertSignedMagnitude(us);
    
    // Move over to the next column.
    offset += theDtedRecordSizeInBytes;
    memcpy(&us, buf+offset, POST_SIZE); 
    p10 = convertSignedMagnitude(us);
    memcpy(&us, buf+offset+POST_SIZE, POST_SIZE); 
    p11 = convertSignedMagnitude(us);
    // Get the fourth post.
//    theFileStr.read((char*)&us, POST_SIZE);
//    ss = convertSignedMagnitude(us);
//    p11 = ss;
    
   // Perform bilinear interpolation:
   double wx1 = xi  - x0;
   double wy1 = yi  - y0;
   double wx0 = 1.0 - wx1;
   double wy0 = 1.0 - wy1;
   
   double w00 = wx0*wy0;
   double w01 = wx0*wy1;
   double w10 = wx1*wy0;
   double w11 = wx1*wy1;

   //***
   // Test for null posts and set the corresponding weights to 0:
   //***
   if (p00 == NULL_POST)
      w00 = 0.0;
   if (p01 == NULL_POST)
      w01 = 0.0;
   if (p10 == NULL_POST)
      w10 = 0.0;
   if (p11 == NULL_POST)
      w11 = 0.0;

#if 0 /* Serious debug only... */
   cout << "\np00:  " << p00
        << "\np01:  " << p01
        << "\np10:  " << p10
        << "\np11:  " << p11
        << "\nw00:  " << w00
        << "\nw01:  " << w01
        << "\nw10:  " << w10
        << "\nw11:  " << w11
        << std::endl;
#endif

   double sum_weights = w00 + w01 + w10 + w11;

   if (sum_weights)
   {
      return (p00*w00 + p01*w01 + p10*w10 + p11*w11) / sum_weights;
   }
   
   return NULL_POST;
}

double ossimDtedHandler::getPostValue(const ossimIpt& gridPt) const
{
   static const char MODULE[] = "ossimDtedHandler::getPostValue";
   
   // Do some error checking.
   if ( gridPt.x < 0.0 || gridPt.y < 0.0 ||
        gridPt.x > (theNumLonLines  - 1) ||
        gridPt.y > (theNumLatPoints - 1) )
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "WARNING " << MODULE << ": No intersection..." << std::endl;
      }
      return theNullHeightValue;
   }

   if (!open())
   {
      return theNullHeightValue;
   }

   int offset =
      theOffsetToFirstDataRecord + gridPt.x * theDtedRecordSizeInBytes +
      gridPt.y * 2 + DATA_RECORD_OFFSET_TO_POST;

   // Put the file pointer at the start of the first elevation post.
   theFileStr.seekg(offset, std::ios::beg);

   ossim_uint16 us;

   // Get the post.
   theFileStr.read((char*)&us, POST_SIZE);
   
   return double(convertSignedMagnitude(us));
}

void ossimDtedHandler::gatherStatistics()
{
   //***
   // Check to see if there is a statistics file already.  If so; do a lookup
   // for the min and max values.
   //***
   ossimFilename stats_file = theFilename;//theFilename.path();
   stats_file.setExtension("statistics");
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimDtedHandler::gatherStatistics(): Looking for "
         << stats_file << " statistics file..." << std::endl;
   }

   ossimKeywordlist kwl;
   const char* min_str = NULL;
   const char* max_str = NULL;

   if (stats_file.exists())
   {
      if (kwl.addFile(stats_file))
      {
         // Look for the min_pixel_value keyword.
         min_str = kwl.find(ossimKeywordNames::MIN_VALUE_KW);
         max_str = kwl.find(ossimKeywordNames::MAX_VALUE_KW);
      }
   }

   if (min_str && max_str)
   {
      theMinHeightAboveMSL = atoi(min_str);
      theMaxHeightAboveMSL = atoi(max_str);
   }
   else if (theComputeStatsFlag)  // Scan the cell and gather the statistics...
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_NOTICE)
            << "NOTICE ossimDtedHandler::gatherStatistics():"
            << " scanning for min/max"
            << "\nThis may take a while..." << std::endl;
      }
      // Start off with the min and max pegged.
      theMinHeightAboveMSL =  32767;
      theMaxHeightAboveMSL = -32767;
      
      // Put the file pointer at the start of the first elevation post.
      theFileStr.seekg(theOffsetToFirstDataRecord, std::ios::beg);
      
      //---
      // Loop through all records and scan for lowest min and highest max.
      // Each record contains a row of latitude points for a given longitude.
      // There are eight bytes in front of the post and four checksum bytes at
      // the end so ignore them.
      //---
      for (ossim_int32 i=0; i<theNumLonLines; ++i)  // longitude direction
      {
         theFileStr.seekg(DATA_RECORD_OFFSET_TO_POST, std::ios::cur);
         
         for (ossim_int32 j=0; j<theNumLatPoints; ++j) // latitude direction
         {
            ossim_uint16 us;
            ossim_sint16 ss;
            theFileStr.read((char*)&us, POST_SIZE);
            ss = convertSignedMagnitude(us);
            if (ss < theMinHeightAboveMSL && ss != NULL_POST)
            {
               theMinHeightAboveMSL = ss;
            }
            if (ss > theMaxHeightAboveMSL)
            {
               theMaxHeightAboveMSL = ss;
            }
         }
         
         theFileStr.seekg(DATA_RECORD_CHECKSUM_SIZE, std::ios::cur);
      }
      
      // Add the stats to the keyword list.
      kwl.add(ossimKeywordNames::MIN_VALUE_KW, theMinHeightAboveMSL);
      kwl.add(ossimKeywordNames::MAX_VALUE_KW, theMaxHeightAboveMSL);
      
      // Write out the statistics file.
      kwl.write(stats_file.c_str());
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimDtedHandler::gatherStatistics:"
         << "\ntheMinHeightAboveMSL:  " << theMinHeightAboveMSL
         << "\ntheMaxHeightAboveMSL:  " << theMaxHeightAboveMSL
         << std::endl;
   }
}

ossimIpt ossimDtedHandler::getSizeOfElevCell() const
{
   return ossimIpt(theNumLonLines, theNumLatPoints);
}

ossimString  ossimDtedHandler::edition() const
{
   return theEdition;
}

ossimString  ossimDtedHandler::productLevel() const
{
   return theProductLevel;
}

ossimString  ossimDtedHandler::compilationDate() const
{
   return theCompilationDate;
}

void ossimDtedHandler::setMemoryMapFlag(bool flag)const
{
   if(!flag)
   {
      theMemoryMap.clear();
   }
   else
   {
      if(!theMemoryMap.size())
      {
         std::ifstream in(theFilename.c_str(), std::ios::in|std::ios::binary);
         theMemoryMap.resize(theFilename.fileSize());
         in.read((char*)(&theMemoryMap.front()), theMemoryMap.size());
         in.close();
      }
   }
}

const ossimDtedHandler& ossimDtedHandler::operator=(
   const ossimDtedHandler& rhs)
{
   return rhs;
}

ossimDtedHandler::ossimDtedHandler(const ossimDtedHandler&)
   :
   ossimElevCellHandler()
{}
