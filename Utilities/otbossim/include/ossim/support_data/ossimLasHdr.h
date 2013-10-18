//----------------------------------------------------------------------------
//
// File: ossimLasHdr.h
// 
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Container class for LAS LIDAR format public header block.
//
//----------------------------------------------------------------------------
// $Id$
#ifndef ossimLasHdr_HEADER
#define ossimLasHdr_HEADER 1

#include <ossim/base/ossimConstants.h>

#include <istream>
#include <ostream>

class ossimKeywordlist;

class OSSIM_DLL ossimLasHdr
{
public:
   
   /* @brief default constructor */
   ossimLasHdr();

   /* @brief copy constructor */
   ossimLasHdr(const ossimLasHdr& hdr);

   /* @brief assignment operator= */
   const ossimLasHdr& operator=(const ossimLasHdr& copy_this);

   /* destructor */
   ~ossimLasHdr();

   /**
    * @brief Checks for LASF in first four bytes.
    * @return true if first four bytes are LASF.
    */
   bool checkSignature(std::istream& in) const;   

   /**
    * @brief Method to initialize from input stream.
    * @param in Stream to read from.
    * @note The first four bytes are not read in so stream should be
    * positioned at the 5th byte.
    */
   void readStream(std::istream& in);

   /** @brief Writes header to stream. */
   void writeStream(std::ostream& out);

   /**
    * @brief print method.
    * @return std::ostream&
    */
   std::ostream& print(std::ostream& out) const;

   /** @brief Convenience operator<< method. */
   friend OSSIM_DLL std::ostream& operator<<(std::ostream& out, const ossimLasHdr& hdr);
   
   void getKeywordlist(ossimKeywordlist& kwl) const;

   /** @return True if bit 0 of m_globalEncoding is set, false if not. */
   bool getGpsTimeTypeBit() const;

   /** @return True if bit 1 of m_globalEncoding is set, false if not. */
   bool getWaveforDataPacketsInternalBit() const;

   /** @return True if bit 2 of m_globalEncoding is set, false if not. */
   bool getWaveforDataPacketsExternalBit() const;
   
   /** @return True if bit 3 of m_globalEncoding is set, false if not. */
   bool getReturnsSyntheticallyGeneratedBit() const;
   
   /**
    * @return The project ID GUID data, e.g.: 55b44da7-7c23-4f86-a54ec39e8f1d1ea1
    */
   std::string getProjectIdGuid() const;

   /** @return The major and minor version, e.g.: 1.2 */
   std::string getVersion() const;

   /** @return The system identifier string. */
   std::string  getSystemIndentifier() const;

   /** @return The generating software string. */
   std::string  getGeneratingSoftware() const;

   /** @return Size of this header in bytes. */
   ossim_uint16 getHeaderSize() const;

   /** @return Offset to point data. */
   ossim_uint32 getOffsetToPointData() const;

   /** @return The number fo variable length records. */
   ossim_uint32 getNumberOfVlrs() const;

   /** @return Point data format ID */
   ossim_uint8 getPointDataFormatId() const;

   /** @return The number of total points. */
   ossim_uint32 getNumberOfPoints() const;

   /**
    * @brief Gets number of points for entry where entry is synonymous returns.
    * @return The number of points for entries 0 through 4.
    */
   ossim_uint32 getNumberOfPoints(ossim_uint32 entry) const;

   const ossim_float64& getScaleFactorX() const;
   const ossim_float64& getScaleFactorY() const;
   const ossim_float64& getScaleFactorZ() const;
   const ossim_float64& getOffsetX() const;
   const ossim_float64& getOffsetY() const;
   const ossim_float64& getOffsetZ() const;
   const ossim_float64& getMinX() const;
   const ossim_float64& getMinY() const;
   const ossim_float64& getMinZ() const;
   const ossim_float64& getMaxX() const;
   const ossim_float64& getMaxY() const;
   const ossim_float64& getMaxZ() const;
   
   
private:

   /** @brief Performs a swap if system byte order is not little endian. */
   void swap();

   char          m_fileSignature[4];
   ossim_uint16  m_fileSourceId;
   ossim_uint16  m_globalEncoding;
   ossim_uint32  m_projectIdGuidData1;
   ossim_uint16  m_projectIdGuidData2;
   ossim_uint16  m_projectIdGuidData3;
   ossim_uint8   m_projectIdGuidData4[8];
   ossim_uint8   m_versionMajor;
   ossim_uint8   m_versionMinor;
   char          m_systemIndentifier[32];
   char          m_generatingSoftware[32];
   ossim_uint16  m_fileCreationDay;
   ossim_uint16  m_fileCreateionYear;
   ossim_uint16  m_headerSize;
   ossim_uint32  m_offsetToPointData;
   ossim_uint32  m_numberOfVariableLengthRecords;
   ossim_uint8   m_pointDataFormatId;
   ossim_uint16  m_pointDataRecordLength;
   ossim_uint32  m_numberOfPointRecords;
   ossim_uint32  m_numberOfPointsReturn1;
   ossim_uint32  m_numberOfPointsReturn2;
   ossim_uint32  m_numberOfPointsReturn3;
   ossim_uint32  m_numberOfPointsReturn4;
   ossim_uint32  m_numberOfPointsReturn5;
   ossim_float64 m_xScaleFactor;
   ossim_float64 m_yScaleFactor;
   ossim_float64 m_zScaleFactor;
   ossim_float64 m_xOffset;
   ossim_float64 m_yOffset;
   ossim_float64 m_zOffset;
   ossim_float64 m_maxX;
   ossim_float64 m_minX;
   ossim_float64 m_maxY;
   ossim_float64 m_minY;
   ossim_float64 m_maxZ;
   ossim_float64 m_minZ;
   ossim_uint64  m_startOfWaveformData;
};



#endif /* End of "#ifndef ossimLasHdr_HEADER" */
