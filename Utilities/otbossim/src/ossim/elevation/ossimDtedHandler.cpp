//*****************************************************************************
// FILE: ossimDtedHandler.cc
//
// License:  See top level LICENSE.txt file.
//
// DESCRIPTION:
//   Contains implementation of class ossimDtedHandler. This class derives from
//   ossimElevHandler. It is responsible for loading an individual DTED cell
//   from disk. This elevation files are memory mapped.
//
//*****************************************************************************
// $Id: ossimDtedHandler.cpp 16826 2010-03-08 18:47:32Z gpotts $

#include <cstdlib>
#include <cstring> /* for memcpy */
#include <ossim/elevation/ossimDtedHandler.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimNotify.h>
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


ossimDtedHandler::ossimDtedHandler(const ossimFilename& dted_file, bool memoryMapFlag)
   :
      ossimElevCellHandler(dted_file),
      m_fileStr(),
      m_numLonLines(0),
      m_numLatPoints(0),
      m_dtedRecordSizeInBytes(0),
      m_edition(),
      m_productLevel(),
      m_compilationDate(),
      m_offsetToFirstDataRecord(0),
      m_latSpacing(0.0),
      m_lonSpacing(0.0),
      m_swCornerPost(),
      m_swapBytesFlag(false)
{
   static const char MODULE[] = "ossimDtedHandler (Filename) Constructor";
   if (traceExec())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG " << MODULE <<": entering..." << std::endl;
   }
   
   m_swapBytesFlag = ossim::byteOrder() == OSSIM_LITTLE_ENDIAN ? true : false;
   //---
   //  Open the dted file for reading.
   //---
   if (!open(dted_file, memoryMapFlag))
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
   if (traceExec())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG " << MODULE << ": returning..." << std::endl;
   }
}

ossimDtedHandler::~ossimDtedHandler()
{
   close();
}

double ossimDtedHandler::getHeightAboveMSL(const ossimGpt& gpt)
{
   if(m_fileStr.is_open())
   {
      return getHeightAboveMSLFile(gpt);
   }
   else if(m_memoryMap.size())
   {
      return getHeightAboveMSLMemory(gpt);
   }
   
   return ossim::nan();
}

bool ossimDtedHandler::open(const ossimFilename& file, bool memoryMapFlag)
{
   static const char* MODULE = "ossimDtedHandler::open";
   close();
   theFilename = file;
   m_fileStr.clear();

   m_fileStr.open(file.c_str(), 
                  std::ios::in | std::ios::binary);
   if(!m_fileStr.good())
   {
      return false;
   }
   m_numLonLines = 0;
   m_numLatPoints = 0;
   m_dtedRecordSizeInBytes = 0;
   
   // Attempt to parse.
   m_vol.parse(m_fileStr);
   m_hdr.parse(m_fileStr);
   m_uhl.parse(m_fileStr);
   m_dsi.parse(m_fileStr);
   m_acc.parse(m_fileStr);

   //***
   // Check for errors.  Must have uhl, dsi and acc records.  vol and hdr
   // are for magnetic tape only; hence, may or may not be there.
   //***
   if (m_uhl.getErrorStatus() == ossimErrorCodes::OSSIM_ERROR ||
       m_dsi.getErrorStatus() == ossimErrorCodes::OSSIM_ERROR ||
       m_acc.getErrorStatus() == ossimErrorCodes::OSSIM_ERROR)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG " << MODULE << ": "
         << "\nError parsing file:  " << file.c_str()
         << "\nPossibly not a dted file."
         << std::endl;
      }
      
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      close();
      return false;
   }
   if(memoryMapFlag)
   {
      m_fileStr.seekg(0);
      m_fileStr.clear();
      m_memoryMap.resize(theFilename.fileSize());
      m_fileStr.read((char*)(&m_memoryMap.front()), (std::streamsize)m_memoryMap.size());
      m_fileStr.close();
   }
   
   m_numLonLines  = m_uhl.numLonLines();
   m_numLatPoints = m_uhl.numLatPoints();
   m_latSpacing   = m_uhl.latInterval();
   m_lonSpacing   = m_uhl.lonInterval();
   m_dtedRecordSizeInBytes = m_numLatPoints*2+ossimDtedRecord::DATA_LENGTH;
   
   m_edition  = m_dsi.edition();
   m_productLevel = m_dsi.productLevel();
   m_compilationDate = m_dsi.compilationDate();
   
   m_offsetToFirstDataRecord = m_acc.stopOffset();
   
   //***
   //  initialize the bounding rectangle:
   //***
   double south_boundary = m_uhl.latOrigin();
   double west_boundary  = m_uhl.lonOrigin();
   double north_boundary = south_boundary + m_latSpacing*(m_numLatPoints-1);
   double east_boundary  = west_boundary  + m_lonSpacing*(m_numLonLines-1);
   
   // For ossimElevCellHandler::pointHasCoverage method.
   theGroundRect = ossimGrect(ossimGpt(north_boundary, west_boundary, 0.0),
                              ossimGpt(south_boundary, east_boundary, 0.0));
   
   m_swCornerPost.lat = south_boundary;
   m_swCornerPost.lon = west_boundary;
   
   //***
   //  Determine the mean spacing:
   //***
   double center_lat = (south_boundary + north_boundary)/2.0;
   theMeanSpacing = (m_latSpacing + m_lonSpacing*ossim::cosd(center_lat))
                     * ossimGpt().metersPerDegree().x / 2.0;
   
   //  Initialize the accuracy values:
   theAbsLE90 = m_acc.absLE();
   theAbsCE90 = m_acc.absCE();
   
   // Set the base class null height value.
   theNullHeightValue = -32767.0;
   
   // Get the statistics.
   gatherStatistics();

   return true;
}

double ossimDtedHandler::getHeightAboveMSLFile(const ossimGpt& gpt)
{
   // Establish the grid indexes:
   double xi = (gpt.lon - m_swCornerPost.lon) / m_lonSpacing;
   double yi = (gpt.lat - m_swCornerPost.lat) / m_latSpacing;

   // Check for right edge.
   int x0 = static_cast<int>(xi);
   int y0 = static_cast<int>(yi);

   if(x0 == (m_numLonLines-1))
   {
      --x0; // Move over one post.
   }
   
   // Check for top edge.
   //    if (gpt.lat == theGroundRect.ul().lat)
   if(y0 == (m_numLatPoints-1))
   {
      --y0; // Move down one post.
   }

   // Do some error checking.
   if ( xi < 0.0 || yi < 0.0 ||
        x0 > (m_numLonLines  - 2.0) ||
        y0 > (m_numLatPoints - 2.0) )
   {
      return ossim::nan();
   }

   double p00;
   double p01;
   double p10;
   double p11;

   //***
   // Grab the four points from the dted cell needed.  These are big endian,
   // signed magnitude shorts so they must be interpreted accordingly.
   //***
   int offset = m_offsetToFirstDataRecord + x0 * m_dtedRecordSizeInBytes +
                y0 * 2 + DATA_RECORD_OFFSET_TO_POST;

   {
      OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_fileStrMutex);
      // Put the file pointer at the start of the first elevation post.
      m_fileStr.seekg(offset, std::ios::beg);
      
      ossim_sint16 ss;
      ossim_uint16 us;
      
      // Get the first post.
      m_fileStr.read((char*)&us, POST_SIZE);
      ss = convertSignedMagnitude(us);
      p00 = ss;
      
      // Get the second post.
      m_fileStr.read((char*)&us, POST_SIZE);
      ss = convertSignedMagnitude(us);
      p01 = ss;
      
      // Move over to the next column.
      offset += m_dtedRecordSizeInBytes;
      m_fileStr.seekg(offset, std::ios::beg);
      
      // Get the third post.
      m_fileStr.read((char*)&us, POST_SIZE);
      ss = convertSignedMagnitude(us);
      p10 = ss;
      
      // Get the fourth post.
      m_fileStr.read((char*)&us, POST_SIZE);
      ss = convertSignedMagnitude(us);
      p11 = ss;
   }
   
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
   
   return ossim::nan();
}

double ossimDtedHandler::getHeightAboveMSLMemory(const ossimGpt& gpt)
{
   ossim_uint8* buf = &m_memoryMap.front();
   // Establish the grid indexes:
   double xi = (gpt.lon - m_swCornerPost.lon) / m_lonSpacing;
   double yi = (gpt.lat - m_swCornerPost.lat) / m_latSpacing;

   // Check for right edge.
   int x0 = static_cast<int>(xi);
   int y0 = static_cast<int>(yi);

   // Check for right edge.
//    if (gpt.lon == theGroundRect.lr().lon)
   if(x0 == (m_numLonLines-1))
   {
      --x0; // Move over one post.
   }
   
   // Check for top edge.
//    if (gpt.lat == theGroundRect.ul().lat)
   if(y0 == (m_numLatPoints-1))
   {
      --y0; // Move down one post.
   }

   // Do some error checking.
   if ( xi < 0.0 || yi < 0.0 ||
        x0 > (m_numLonLines  - 2.0) ||
        y0 > (m_numLatPoints - 2.0) )
   {
      return ossim::nan();
   }

   double p00;
   double p01;
   double p10;
   double p11;

   //***
   // Grab the four points from the dted cell needed.  These are big endian,
   // signed magnitude shorts so they must be interpreted accordingly.
   //***
   ossim_uint64 offset = m_offsetToFirstDataRecord + x0 * m_dtedRecordSizeInBytes +
                         y0 * 2 + DATA_RECORD_OFFSET_TO_POST;
   {
      ossim_uint16 us;
      
      memcpy(&us, buf+offset, POST_SIZE); 
      p00 = convertSignedMagnitude(us);
      memcpy(&us, buf+offset+POST_SIZE, POST_SIZE); 
      p01 = convertSignedMagnitude(us);
      
      // Move over to the next column.
      offset += m_dtedRecordSizeInBytes;
      memcpy(&us, buf+offset, POST_SIZE); 
      p10 = convertSignedMagnitude(us);
      memcpy(&us, buf+offset+POST_SIZE, POST_SIZE); 
      p11 = convertSignedMagnitude(us);
   }
    
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
   
   return ossim::nan();
}

double ossimDtedHandler::getPostValue(const ossimIpt& gridPt) const
{
   static const char MODULE[] = "ossimDtedHandler::getPostValue";
   
   // Do some error checking.
   if ( gridPt.x < 0.0 || gridPt.y < 0.0 ||
        gridPt.x > (m_numLonLines  - 1) ||
        gridPt.y > (m_numLatPoints - 1) )
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "WARNING " << MODULE << ": No intersection..." << std::endl;
      }
      return ossim::nan();
   }

   if (!isOpen())
   {
      return ossim::nan();
   }

   int offset =
      m_offsetToFirstDataRecord + gridPt.x * m_dtedRecordSizeInBytes +
      gridPt.y * 2 + DATA_RECORD_OFFSET_TO_POST;
   
   // Put the file pointer at the start of the first elevation post.
   m_fileStr.seekg(offset, std::ios::beg);

   ossim_uint16 us;

   // Get the post.
   m_fileStr.read((char*)&us, POST_SIZE);
   
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
   else if (theComputeStatsFlag&&!m_memoryMap.size())  // Scan the cell and gather the statistics...
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
      m_fileStr.seekg(m_offsetToFirstDataRecord, std::ios::beg);
      
      //---
      // Loop through all records and scan for lowest min and highest max.
      // Each record contains a row of latitude points for a given longitude.
      // There are eight bytes in front of the post and four checksum bytes at
      // the end so ignore them.
      //---
      for (ossim_int32 i=0; i<m_numLonLines; ++i)  // longitude direction
      {
         m_fileStr.seekg(DATA_RECORD_OFFSET_TO_POST, std::ios::cur);
         
         for (ossim_int32 j=0; j<m_numLatPoints; ++j) // latitude direction
         {
            ossim_uint16 us;
            ossim_sint16 ss;
            m_fileStr.read((char*)&us, POST_SIZE);
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
         
         m_fileStr.seekg(DATA_RECORD_CHECKSUM_SIZE, std::ios::cur);
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
   return ossimIpt(m_numLonLines, m_numLatPoints);
}

ossimString  ossimDtedHandler::edition() const
{
   return m_edition;
}

ossimString  ossimDtedHandler::productLevel() const
{
   return m_productLevel;
}

ossimString  ossimDtedHandler::compilationDate() const
{
   return m_compilationDate;
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
