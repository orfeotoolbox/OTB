//----------------------------------------------------------------------------
//
// File: ossimLasHdr.cpp
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

#include <ossim/support_data/ossimLasHdr.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimKeywordlist.h>

#include <bitset>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>

ossimLasHdr::ossimLasHdr() :
   m_fileSourceId(0),
   m_globalEncoding(0),
   m_projectIdGuidData1(0),
   m_projectIdGuidData2(0),
   m_projectIdGuidData3(0),
   m_versionMajor(0),
   m_versionMinor(0),
   m_fileCreationDay(0),
   m_fileCreateionYear(0),
   m_headerSize(0),
   m_offsetToPointData(0),
   m_numberOfVariableLengthRecords(0),
   m_pointDataFormatId(0),
   m_pointDataRecordLength(0),
   m_legacyNumberOfPointRecords(0),
   m_xScaleFactor(0.0),
   m_yScaleFactor(0.0),
   m_zScaleFactor(0.0),
   m_xOffset(0.0),
   m_yOffset(0.0),
   m_zOffset(0.0),
   m_maxX(0.0),
   m_minX(0.0),
   m_maxY(0.0),
   m_minY(0.0),
   m_maxZ(0.0),
   m_minZ(0.0),
   m_startOfWaveformDataPacket(0),
   m_startOfExtendedVariableLengthRecords(0),
   m_numberOfExtendedVariableLengthRecords(0),
   m_numberOfPointRecords(0)
{
   // Initialize arrays:
   strncpy(m_fileSignature, "LASF", 4);
   memset(m_projectIdGuidData4, 0, 8);
   memset(m_systemIndentifier, 0, 32);
   memset(m_generatingSoftware, 0, 32);
   memset(m_legacyNumberOfPointsByReturn, 0, 20); // 5 uint32s(5 x 4)
   memset(m_numberOfPointsByReturn, 0, 120);   // 15 unint64s(15 x 8)
}

ossimLasHdr::ossimLasHdr(const ossimLasHdr& hdr) :
   m_fileSourceId(hdr.m_fileSourceId),
   m_globalEncoding(hdr.m_globalEncoding),
   m_projectIdGuidData1(hdr.m_projectIdGuidData1),
   m_projectIdGuidData2(hdr.m_projectIdGuidData2),
   m_projectIdGuidData3(hdr.m_projectIdGuidData3),
   m_versionMajor(hdr.m_versionMajor),
   m_versionMinor(hdr.m_versionMinor),
   m_fileCreationDay(hdr.m_fileCreationDay),
   m_fileCreateionYear(hdr.m_fileCreateionYear),
   m_headerSize(hdr.m_headerSize),
   m_offsetToPointData(hdr.m_offsetToPointData),
   m_numberOfVariableLengthRecords(hdr.m_numberOfVariableLengthRecords),
   m_pointDataFormatId(hdr.m_pointDataFormatId),
   m_pointDataRecordLength(hdr.m_pointDataRecordLength),
   m_legacyNumberOfPointRecords(hdr.m_legacyNumberOfPointRecords),
   m_xScaleFactor(hdr.m_xScaleFactor),
   m_yScaleFactor(hdr.m_yScaleFactor),
   m_zScaleFactor(hdr.m_zScaleFactor),
   m_xOffset(hdr.m_xOffset),
   m_yOffset(hdr.m_yOffset),
   m_zOffset(hdr.m_zOffset),
   m_maxX(hdr.m_maxX),
   m_minX(hdr.m_minX),
   m_maxY(hdr.m_maxY),
   m_minY(hdr.m_minY),
   m_maxZ(hdr.m_maxZ),
   m_minZ(hdr.m_minZ),
   m_startOfWaveformDataPacket(hdr.m_startOfWaveformDataPacket),
   m_startOfExtendedVariableLengthRecords(hdr.m_startOfExtendedVariableLengthRecords),
   m_numberOfExtendedVariableLengthRecords(hdr.m_numberOfExtendedVariableLengthRecords),
   m_numberOfPointRecords(hdr.m_numberOfPointRecords)
{
   // Copy arrays.
   memcpy((void*)m_fileSignature, (void*)(hdr.m_fileSignature), 4);
   memcpy((void*)m_projectIdGuidData4, (void*)(hdr.m_projectIdGuidData4), 8);
   memcpy((void*)m_systemIndentifier, (void*)(hdr.m_systemIndentifier), 32);
   memcpy((void*)m_generatingSoftware, (void*)(hdr.m_generatingSoftware), 32);
   memcpy((void*)m_legacyNumberOfPointsByReturn, (void*)(hdr.m_legacyNumberOfPointsByReturn), 20);
   memcpy((void*)m_numberOfPointsByReturn, (void*)(hdr.m_numberOfPointsByReturn), 120);
}

const ossimLasHdr& ossimLasHdr::operator=(const ossimLasHdr& hdr)
{
   if (this != &hdr)
   {
      m_fileSourceId = hdr.m_fileSourceId;
      m_globalEncoding = hdr.m_globalEncoding;
      m_projectIdGuidData1 = hdr.m_projectIdGuidData1;
      m_projectIdGuidData2 = hdr.m_projectIdGuidData2;
      m_projectIdGuidData3 = hdr.m_projectIdGuidData3;
      m_versionMajor = hdr.m_versionMajor;
      m_versionMinor = hdr.m_versionMinor;
      strncpy(m_systemIndentifier, hdr.m_systemIndentifier, 32);
      strncpy(m_generatingSoftware, hdr.m_generatingSoftware, 32);      
      m_fileCreationDay = hdr.m_fileCreationDay;
      m_fileCreateionYear = hdr.m_fileCreateionYear;
      m_headerSize = hdr.m_headerSize;
      m_offsetToPointData = hdr.m_offsetToPointData;
      m_numberOfVariableLengthRecords = hdr.m_numberOfVariableLengthRecords;
      m_pointDataFormatId = hdr.m_pointDataFormatId;
      m_pointDataRecordLength = hdr.m_pointDataRecordLength;
      m_legacyNumberOfPointRecords = hdr.m_legacyNumberOfPointRecords;
      m_xScaleFactor = hdr.m_xScaleFactor;
      m_yScaleFactor = hdr.m_yScaleFactor;
      m_zScaleFactor = hdr.m_zScaleFactor;
      m_xOffset = hdr.m_xOffset;
      m_yOffset = hdr.m_yOffset;
      m_zOffset = hdr.m_zOffset;
      m_maxX = hdr.m_maxX;
      m_minX = hdr.m_minX;
      m_maxY = hdr.m_maxY;
      m_minY = hdr.m_minY;
      m_maxZ = hdr.m_maxZ;
      m_minZ = hdr.m_minZ;
      m_startOfWaveformDataPacket = hdr.m_startOfWaveformDataPacket;
      m_startOfExtendedVariableLengthRecords = hdr.m_startOfExtendedVariableLengthRecords;
      m_numberOfExtendedVariableLengthRecords = hdr.m_numberOfExtendedVariableLengthRecords;
      m_numberOfPointRecords = hdr.m_numberOfPointRecords;

      // Copy arrays.
      memcpy((void*)m_fileSignature,(void*)(hdr.m_fileSignature), 4);
      memcpy((void*)m_projectIdGuidData4, (void*)(hdr.m_projectIdGuidData4), 8);
      memcpy((void*)m_systemIndentifier, (void*)(hdr.m_systemIndentifier), 32);
      memcpy((void*)m_generatingSoftware, (void*)(hdr.m_generatingSoftware), 32);
      memcpy((void*)m_legacyNumberOfPointsByReturn,
             (void*)(hdr.m_legacyNumberOfPointsByReturn), 20);
      memcpy((void*)m_numberOfPointsByReturn, (void*)(hdr.m_numberOfPointsByReturn), 120);
   }
   return *this;
}

ossimLasHdr::~ossimLasHdr()
{
}

bool ossimLasHdr::checkSignature(std::istream& in) const
{
   bool result = false;
   char SIG[4];
   in.read(SIG, 4);
   if ( (SIG[0] == 'L') && (SIG[1] == 'A') &&
        (SIG[2] == 'S') && (SIG[3] == 'F') )
   {
      result = true;
   }
   return result;
}

void ossimLasHdr::readStream(std::istream& in)
{
   in.read(m_fileSignature, 4);
   in.read((char*)&m_fileSourceId, 2);
   in.read((char*)&m_globalEncoding, 2);
   in.read((char*)&m_projectIdGuidData1, 4);
   in.read((char*)&m_projectIdGuidData2, 2);
   in.read((char*)&m_projectIdGuidData3, 2);
   in.read((char*)&m_projectIdGuidData4, 8);
   in.read((char*)&m_versionMajor, 1);
   in.read((char*)&m_versionMinor, 1);
   in.read(m_systemIndentifier, 32);
   in.read(m_generatingSoftware, 32);
   in.read((char*)&m_fileCreationDay, 2);
   in.read((char*)&m_fileCreateionYear, 2);
   in.read((char*)&m_headerSize, 2);
   in.read((char*)&m_offsetToPointData, 4);
   in.read((char*)&m_numberOfVariableLengthRecords, 4);
   in.read((char*)&m_pointDataFormatId, 1);
   in.read((char*)&m_pointDataRecordLength, 2);
   in.read((char*)&m_legacyNumberOfPointRecords, 4);
   in.read((char*)&m_legacyNumberOfPointsByReturn, 20);
   in.read((char*)&m_xScaleFactor, 8);
   in.read((char*)&m_yScaleFactor, 8);
   in.read((char*)&m_zScaleFactor, 8);
   in.read((char*)&m_xOffset, 8);
   in.read((char*)&m_yOffset, 8);
   in.read((char*)&m_zOffset, 8);
   in.read((char*)&m_maxX, 8);
   in.read((char*)&m_minX, 8);
   in.read((char*)&m_maxY, 8);
   in.read((char*)&m_minY, 8);
   in.read((char*)&m_maxZ, 8);
   in.read((char*)&m_minZ, 8);

   // Version specific:
   if ( versionGreaterThan( 1, 2 ) )
   {
      // Added in 1.3:
      in.read((char*)&m_startOfWaveformDataPacket, 8);
   }
   if ( versionGreaterThan( 1, 3 ) )
   {
      // Added in 1.4:
      in.read((char*)&m_startOfExtendedVariableLengthRecords, 8);
      in.read((char*)&m_numberOfExtendedVariableLengthRecords, 4);
      in.read((char*)&m_numberOfPointRecords, 8);
      in.read((char*)&m_numberOfPointsByReturn, 120);
   }

   if ( ossim::byteOrder() == OSSIM_BIG_ENDIAN )
   {
      swap();
   }

   if ( !versionGreaterThan( 1, 3 ) ) // Less than 1.4
   {
      //---
      // Copy legacy point count to 64 bit data members. This will allow LAS code
      // to always go through same methods for point counts.
      //---
      m_numberOfPointRecords = m_legacyNumberOfPointRecords;
      for ( ossim_uint32 i = 0; i < 5; ++i )
      {
         m_numberOfPointsByReturn[i] = m_legacyNumberOfPointsByReturn[i];
      }
   }
}

void ossimLasHdr::writeStream(std::ostream& out)
{
   if ( ossim::byteOrder() == OSSIM_BIG_ENDIAN )
   {
      // Write little endian per spec.
      swap();
   }
   
   out.write(m_fileSignature, 4);
   out.write((char*)&m_fileSourceId, 2);
   out.write((char*)&m_globalEncoding, 2);
   out.write((char*)&m_projectIdGuidData1, 4);
   out.write((char*)&m_projectIdGuidData2, 2);
   out.write((char*)&m_projectIdGuidData3, 2);
   out.write((char*)&m_projectIdGuidData4, 8);
   out.write((char*)&m_versionMajor, 1);
   out.write((char*)&m_versionMinor, 1);
   out.write(m_systemIndentifier, 32);
   out.write(m_generatingSoftware, 32);
   out.write((char*)&m_fileCreationDay, 2);
   out.write((char*)&m_fileCreateionYear, 2);
   out.write((char*)&m_headerSize, 2);
   out.write((char*)&m_offsetToPointData, 4);
   out.write((char*)&m_numberOfVariableLengthRecords, 4);
   out.write((char*)&m_pointDataFormatId, 1);
   out.write((char*)&m_pointDataRecordLength, 2);
   out.write((char*)&m_legacyNumberOfPointRecords, 4);
   out.write((char*)&m_legacyNumberOfPointsByReturn, 20);
   out.write((char*)&m_xScaleFactor, 8);
   out.write((char*)&m_yScaleFactor, 8);
   out.write((char*)&m_zScaleFactor, 8);
   out.write((char*)&m_xOffset, 8);
   out.write((char*)&m_yOffset, 8);
   out.write((char*)&m_zOffset, 8);
   out.write((char*)&m_maxX, 8);
   out.write((char*)&m_minX, 8);
   out.write((char*)&m_maxY, 8);
   out.write((char*)&m_minY, 8);
   out.write((char*)&m_maxZ, 8);
   out.write((char*)&m_minZ, 8);

   if ( versionGreaterThan( 1, 2 ) )
   {
      // Added in 1.3:
      out.write((char*)&m_startOfWaveformDataPacket, 8);
   }
   if ( versionGreaterThan( 1, 3 ) )
   {
      // Added in 1.4:
      out.write((char*)&m_startOfExtendedVariableLengthRecords, 8);
      out.write((char*)&m_numberOfExtendedVariableLengthRecords, 4);
      out.write((char*)&m_numberOfPointRecords, 8);
      out.write((char*)&m_numberOfPointsByReturn, 120);
   }

   if ( ossim::byteOrder() == OSSIM_BIG_ENDIAN )
   {
      // Swap back to native byte order if needed:
      swap();
   }
}

std::ostream& ossimLasHdr::print(std::ostream& out) const
{
   ossimKeywordlist kwl;
   getKeywordlist( kwl );
   return out << kwl;
}

std::ostream& operator<<(std::ostream& out, const ossimLasHdr& hdr)
{
   return hdr.print(out);
}

void ossimLasHdr::getKeywordlist(ossimKeywordlist& kwl) const
{
   kwl.add("las.file_source_id", m_fileSourceId);

   //---
   // m_globalEncoding is a bit encoding so we output bit at a time:
   // At the time of this coding only first four bits used.
   //---

   // 1.2:
   kwl.add("las.gps_time_type", (getGpsTimeTypeBit()?"1":"0") );

   if ( versionGreaterThan( 1, 2 ) )
   {
      // Added in 1.3:
      kwl.add("las.waveform_data_packets_internal", (getWaveforDataPacketsInternalBit()?"1":"0") );
      kwl.add("las.waveform_data_packets_external", (getWaveforDataPacketsExternalBit()?"1":"0") );
      kwl.add("las.synthetically_generated", (getReturnsSyntheticallyGeneratedBit()?"1":"0") );
   }

   if (  versionGreaterThan( 1, 3 ) )
   {
      // 1.4:
      kwl.add("las.wkt", (getWktBit()?"1":"0") );
   }
   
   kwl.add("las.project_id_guid", getProjectIdGuid().c_str());

   kwl.add("las.version", getVersion().c_str());

   kwl.add("las.system_indentifier", getSystemIndentifier().c_str());
   kwl.add("las.generating_software", getGeneratingSoftware().c_str());

   kwl.add("las.file_creation_day", m_fileCreationDay);
   kwl.add("las.file_creation_year", m_fileCreateionYear);

   kwl.add("las.header_size", m_headerSize);
   kwl.add("las.offset_to_point_data", m_offsetToPointData );
   kwl.add("las.number_of_variable_length_records", m_numberOfVariableLengthRecords);
   kwl.add("las.point_data_format_id", m_pointDataFormatId);
   kwl.add("las.point_data_record_length", m_pointDataRecordLength);

   kwl.add("las.x_scale_factor", m_xScaleFactor);
   kwl.add("las.y_scale_factor", m_yScaleFactor);
   kwl.add("las.z_scale_factor", m_zScaleFactor);

   kwl.add("las.x_offset", m_xOffset);
   kwl.add("las.y_offset", m_yOffset);
   kwl.add("las.z_offset", m_zOffset);
   
   kwl.add("las.max_x", m_maxX);
   kwl.add("las.min_x", m_minX);
   kwl.add("las.max_y", m_maxY);
   kwl.add("las.min_y", m_minY);
   kwl.add("las.max_z", m_maxZ);
   kwl.add("las.min_z", m_minZ);

   kwl.add("las.start_of_wave_form_data", m_startOfWaveformDataPacket);

   if ( versionGreaterThan( 1, 3 ) )
   {
      kwl.add("las.number_of_point_records", m_numberOfPointRecords);
      for (ossim_uint32 i = 0; i < 15; ++i)
      {
         std::ostringstream os;
         os << "las.number_of_points_return" << (i+1);
         kwl.add( os.str().c_str(), m_numberOfPointsByReturn[i] );
      }
   }
   else
   {
      // Prior to 1.4:
      kwl.add("las.number_of_point_records", m_legacyNumberOfPointRecords);
      for (ossim_uint32 i = 0; i < 5; ++i)
      {
         std::ostringstream os;
         os << "las.number_of_points_return" << (i+1);
         kwl.add( os.str().c_str(), m_legacyNumberOfPointsByReturn[i] );
      } 
   }
}

bool ossimLasHdr::getGpsTimeTypeBit() const
{
   // Test the first bit:
   std::bitset<1> bs( m_globalEncoding );
   return bs.test(0);
}

bool ossimLasHdr::getWaveforDataPacketsInternalBit() const
{
   // Test the second bit:
   std::bitset<2> bs( m_globalEncoding );
   return bs.test(1);
}

bool ossimLasHdr::getWaveforDataPacketsExternalBit() const
{
   // Test the third bit:
   std::bitset<3> bs( m_globalEncoding );
   return bs.test(2);
}

bool ossimLasHdr::getReturnsSyntheticallyGeneratedBit() const
{
   // Test the fourth bit:
   std::bitset<4> bs( m_globalEncoding );
   return bs.test(3);
}

bool ossimLasHdr::getWktBit() const
{
   // Test the fith bit:
   std::bitset<5> bs( m_globalEncoding );
   return bs.test(4); 
}

std::string ossimLasHdr::getProjectIdGuid() const
{
   std::ostringstream os;
   os << std::hex
      << m_projectIdGuidData1 << "-"
      << m_projectIdGuidData2 << "-"
      << m_projectIdGuidData3 << "-";
   for (int i=0; i<8; ++i)
   {
      ossim_uint16 uid = m_projectIdGuidData4[i];
      os << uid;
   }
   return os.str();
}

std::string ossimLasHdr::getVersion() const
{
   std::ostringstream os;
   os << int(m_versionMajor) << "." << int(m_versionMinor);
   return os.str();
}

std::string  ossimLasHdr::getSystemIndentifier() const
{
   std::string s;
   s.reserve(32);
   for (int i=0; i<32; ++i)
   {
      s.push_back(m_systemIndentifier[i]);
   }
   return s;
}

std::string  ossimLasHdr::getGeneratingSoftware() const
{
   std::string s;
   s.reserve(32);
   for (int i=0; i<32; ++i)
   {
      s.push_back(m_generatingSoftware[i]);
   }
   return s;
}

ossim_uint16 ossimLasHdr::getHeaderSize() const
{
   return m_headerSize;
}

ossim_uint32 ossimLasHdr::getOffsetToPointData() const
{
   return m_offsetToPointData;
}

ossim_uint32 ossimLasHdr::getNumberOfVlrs() const
{
   return m_numberOfVariableLengthRecords;
}

ossim_uint8 ossimLasHdr::getPointDataFormatId() const
{
   return m_pointDataFormatId;
}

ossim_uint64 ossimLasHdr::getNumberOfPoints() const
{
   return m_numberOfPointRecords;
}

ossim_uint64 ossimLasHdr::getNumberOfPoints(ossim_uint32 entry) const
{
   ossim_uint64 result = 0;
   if ( entry < 15 )
   {
      result = m_numberOfPointsByReturn[entry];
   }
   return result;
}

const ossim_float64& ossimLasHdr::getScaleFactorX() const
{
   return m_xScaleFactor;
}

const ossim_float64& ossimLasHdr::getScaleFactorY() const
{
   return m_yScaleFactor;
}

const ossim_float64& ossimLasHdr::getScaleFactorZ() const
{
   return m_zScaleFactor;
}

const ossim_float64& ossimLasHdr::getOffsetX() const
{
   return m_xOffset;
}

const ossim_float64& ossimLasHdr::getOffsetY() const
{
   return m_yOffset;
}

const ossim_float64& ossimLasHdr::getOffsetZ() const
{
   return m_zOffset;
}

const ossim_float64& ossimLasHdr::getMinX() const
{
   return m_minX;
}

const ossim_float64& ossimLasHdr::getMinY() const
{
   return m_minY;
}

const ossim_float64& ossimLasHdr::getMinZ() const
{
   return m_minZ;
}

const ossim_float64& ossimLasHdr::getMaxX() const
{
   return m_maxX;
}

const ossim_float64& ossimLasHdr::getMaxY() const
{
   return m_maxY;
}

const ossim_float64& ossimLasHdr::getMaxZ() const
{
   return m_maxZ;
}

void ossimLasHdr::swap()
{
   ossimEndian endian;
   endian.swap(m_fileSourceId);
   endian.swap(m_globalEncoding);
   endian.swap(m_projectIdGuidData1);
   endian.swap(m_projectIdGuidData2);
   endian.swap(m_projectIdGuidData3);
   endian.swap(m_fileCreationDay);
   endian.swap(m_fileCreateionYear);
   endian.swap(m_headerSize);
   endian.swap(m_offsetToPointData);
   endian.swap(m_numberOfVariableLengthRecords);
   endian.swap(m_pointDataRecordLength);
   endian.swap(m_legacyNumberOfPointRecords);
   endian.swap(m_legacyNumberOfPointsByReturn, 5);
   endian.swap(m_xScaleFactor);
   endian.swap(m_yScaleFactor);
   endian.swap(m_zScaleFactor);
   endian.swap(m_xOffset);
   endian.swap(m_yOffset);
   endian.swap(m_zOffset);
   endian.swap(m_maxX);
   endian.swap(m_minX);
   endian.swap(m_maxY);
   endian.swap(m_minY);
   endian.swap(m_maxZ);
   endian.swap(m_minZ);
   endian.swap(m_startOfWaveformDataPacket);
   endian.swap(m_startOfExtendedVariableLengthRecords);
   endian.swap(m_numberOfExtendedVariableLengthRecords);
   endian.swap(m_numberOfPointRecords);
   endian.swap(m_numberOfPointsByReturn, 15);
}

bool ossimLasHdr::versionGreaterThan( ossim_uint8 major, ossim_uint8 minor ) const
{
   bool result = false;
   if ( m_versionMajor == major )
   {
      result = m_versionMinor > minor;
   }
   else if ( m_versionMajor > major )
   {
      result = true;
   }
   return result;
}
