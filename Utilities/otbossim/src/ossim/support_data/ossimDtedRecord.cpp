//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Ken Melero
// 
// Description:  This class gives access to the Data Record Description
//               of a DTED Level 1 file.
//
// Notes:  Each elevation is a true value referenced to meas sea level
//         (MSL) datum recorded to the nearest meter.  The horizontal
//         position is referenced to precise longitude-latitiude
//         locations in terms of the current World Geodetic System
//         (WGS84) determined for each file by reference to the origin
//         at the southwest corner.  The elevations are evenly spaced
//         in latitude and longitude at the interval designated in the
//         User Header Label (UHL) in South to North profile sequence.
//
//********************************************************************
// $Id: ossimDtedRecord.cpp 20611 2012-02-27 12:21:51Z gpotts $

#include <iostream>

#include <ossim/support_data/ossimDtedRecord.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimNotifyContext.h>

static const ossim_uint16 DATA_NULL_VALUE = 0xffff; // -32767
static const ossim_uint16 DATA_MIN_VALUE  = 0xfffe; // -32766
static const ossim_uint16 DATA_MAX_VALUE  = 0x7fff; // +32767
static const ossim_uint16 DATA_VALUE_MASK = 0x7fff; // 0111 1111 1111 1111
static const ossim_uint16 DATA_SIGN_MASK  = 0x8000; // 1000 0000 0000 0000
static const ossim_uint16 DATA_RECOGNITION_SENTINEL = 0xAA;  // 170

//***
// Offsets from start of data record (one record per longitude line):
//***
static const int BLOCK_COUNT_OFFSET = 2;
static const int LON_INDEX_OFFSET   = 4;
static const int LAT_INDEX_OFFSET   = 6;
static const int ELEV_DATA_OFFSET   = 8;
static const int RECORD_HDR_LENGTH  = 12;
static const int BYTES_PER_POINT    = 2;

//**************************************************************************
// CONSTRUCTOR
//**************************************************************************
ossimDtedRecord::ossimDtedRecord(std::istream& in,
                                 ossim_int32 offset,
                                 ossim_int32 num_points)
   :
      theRecSen("170"),
      theDataBlockCount(0),
      theLonCount(0),
      theLatCount(0),
      theCheckSum(0),
      thePoints(new ossim_int32[num_points]),
      thePointsData(new ossim_uint16[num_points]),
      theComputedCheckSum(0),
      theNumPoints(num_points),
      theStartOffset(offset),
      theStopOffset(offset + RECORD_HDR_LENGTH + (num_points*BYTES_PER_POINT))
{
   // Verify we are at a cell record by checking the Recognition Sentinel.
   ossim_uint8 buf[1];
   in.seekg(theStartOffset, std::ios::beg);
   in.read((char*)buf, 1);
 
#if 0
   printf("\nBuf: %02X", (int)buf[0]);  // Display in HEX
#endif
   
   if(buf[0] != DATA_RECOGNITION_SENTINEL)
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimDtedRecord::ossimDtedRecord: reading DTED's data record at: " << theStartOffset
                                          << std::endl;
      return;
   }

   // Valid data record, so let's process on.
   parse(in);

   //***
   // Note:  The validateCheckSum method works; however, our in-house
   //        dted has bad stored check sums even though the post are good.
   //        So this in temporarily shut off.
   //***
   // Verify Check Sum for uncorrupted elevation data.
   if(validateCheckSum(in) == false)
   {
      //***
      // Note:  The validateCheckSum method works; however, our in-house
      //        dted has bad stored check sums even though the posts are good.
      //        So this in temporarily shut off.
      //***
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      ossimNotify(ossimNotifyLevel_FATAL)
         << "FATAL ossimDtedRecord::ossimDtedRecord:"
         << "\nInvalid checksum in data record at: "
         << theStartOffset
         << "\nParsed Check Sum = " << theCheckSum
         << "\nComputed Check Sum = " << theComputedCheckSum
         << "\nDTED Elevation File is considered corrupted."
         << std::endl;
      return;
   }
}

ossimDtedRecord::~ossimDtedRecord()
{
   if (thePoints)
   {
      delete [] thePoints;
      thePoints = NULL;
   }
   if (thePointsData)
   {
      delete [] thePointsData;
      thePointsData = NULL;
   }
}

//**************************************************************************
// parse()
//**************************************************************************
void ossimDtedRecord::parse(std::istream& in)
{
   // DTED is stored in big endian byte order so swap the bytes if needed.
   bool swap_bytes = ossim::byteOrder() == OSSIM_LITTLE_ENDIAN ? true : false;
   
   ossim_sint16 s;
   
   // parse data block count
   in.seekg(theStartOffset + BLOCK_COUNT_OFFSET, std::ios::beg);
   in.read((char*)&s, 2);
   theDataBlockCount =
      (swap_bytes ? ( ( (s & 0x00ff) << 8) | ( (s & 0xff00) >> 8) ) : s);
   
   // parse lon count
   in.seekg(theStartOffset + LON_INDEX_OFFSET, std::ios::beg);
   in.read((char*)&s, 2);
   theLonCount =
      (swap_bytes ? ( ( (s & 0x00ff) << 8) | ( (s & 0xff00) >> 8) ) : s);
   
   // parse lat count
   in.seekg(theStartOffset + LAT_INDEX_OFFSET, std::ios::beg);
   in.read((char*)&s, 2);
   theLatCount =
      (swap_bytes ? ( ( (s & 0x00ff) << 8) | ( (s & 0xff00) >> 8) ) : s);
   
   // Parse all elevation points.
   in.seekg(theStartOffset + ELEV_DATA_OFFSET, std::ios::beg);
   for(int i = 0; i < theNumPoints; ++i)
   {
      in.read((char*)&s, 2);
      s = (swap_bytes ? ( ( (s & 0x00ff) << 8) | ( (s & 0xff00) >> 8) ) : s);
      if (s & DATA_SIGN_MASK)
      {
         s = (s & DATA_VALUE_MASK) * -1;
      }
      thePoints[i] = static_cast<ossim_int32>(s);
      thePointsData[i] = s;
   }
}

//**************************************************************************
// validateCheckSum()
//**************************************************************************
bool ossimDtedRecord::validateCheckSum(std::istream& in)
{
   // DTED is stored in big endian byte order so swap the bytes if needed.
   bool swap_bytes = ossim::byteOrder() == OSSIM_LITTLE_ENDIAN ? true : false;
   
   // Compute the check sum.
   in.seekg(theStartOffset, std::ios::beg);
   theComputedCheckSum = 0;
   ossim_int32 bytesToRead = (theNumPoints * 2) + ELEV_DATA_OFFSET;
   int i = 0;
   
   for(i = 0; i < bytesToRead; i++)
   {
      ossim_uint8 c;
      in.read((char*)&c, 1);
      theComputedCheckSum += static_cast<ossim_uint32>(c);
   }   

   // Read the stored check sum and swap the byte if needed.
   in.read((char*)&theCheckSum, 4); 
   if (swap_bytes)
   {
      ossimEndian swapper;
      swapper.swap(theCheckSum);
   }
   
   // Compare computed and parsed checksums.
   if(theCheckSum != theComputedCheckSum)
   {
      return false;
   }
   return true;
}

ossimString ossimDtedRecord::recognitionSentinel() const
{
   return theRecSen;
}

ossim_int32 ossimDtedRecord::dataBlockCount() const
{
   return theDataBlockCount;
}

ossim_int32 ossimDtedRecord::lonCount() const
{
   return theLonCount;
}

ossim_int32 ossimDtedRecord::latCount() const
{
   return theLatCount;
}

ossim_uint32 ossimDtedRecord::checkSum() const
{
   return theCheckSum;
}

ossim_uint32 ossimDtedRecord::computedCheckSum() const
{
   return theComputedCheckSum;
}

ossim_int32 ossimDtedRecord::numPoints() const
{
   return theNumPoints;
}

ossim_int32 ossimDtedRecord::getPoint(ossim_int32 i) const
{
   return thePoints[i];
}

ossim_uint16 ossimDtedRecord::getPointData(ossim_int32 i) const
{
   return thePointsData[i];
}

ossim_int32*  ossimDtedRecord::points() const
{
   return thePoints;
}

ossim_uint16* ossimDtedRecord::pointsData() const
{
   return thePointsData;
}

ossim_int32 ossimDtedRecord::startOffset() const
{
   return theStartOffset;
}

ossim_int32 ossimDtedRecord::stopOffset() const
{
   return theStopOffset;
}

//**************************************************************************
// operator <<
//**************************************************************************
std::ostream& operator<<( std::ostream& os, const ossimDtedRecord& rec)
{
   os << "\nDTED Record:"
      << "\n-------------------------------"
      << "\n Recognition Sentinel: " << rec.theRecSen
      << "\nData Block Count:      " << rec.theDataBlockCount
      << "\nLon Count:             " << rec.theLonCount
      << "\nLat Count:             " << rec.theLatCount
      << "\nNum Points:            " << rec.theNumPoints
      << "\nParsed Check Sum:      " << rec.theCheckSum
      << "\nComputed Check Sum     " << rec.theComputedCheckSum
      << "\nStart Offset:          " << rec.theStartOffset
      << "\nStop Offset:           " << rec.theStopOffset
      << std::endl;
   int i = 0;

   for(i = 0; i < rec.theNumPoints; i++)
   {
      os << "\nPoint[" << i << "]: " << rec.thePoints[i];
   }

   os << std::endl;

   return os;
}
