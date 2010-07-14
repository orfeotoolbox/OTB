//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
// 
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfFrameFileIndexRecord.cpp 17501 2010-06-02 11:14:55Z dburken $

#include <cstring> /* for memset, memcpy */
#include <istream>
#include <ostream>

#include <ossim/support_data/ossimRpfFrameFileIndexRecord.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimErrorCodes.h>

std::ostream& operator <<(std::ostream& out, const ossimRpfFrameFileIndexRecord& data)
{
   return data.print(out);
}

ossimRpfFrameFileIndexRecord::ossimRpfFrameFileIndexRecord()
   :
   m_boundaryRectRecordNumber(0),
   m_locationRowNumber(0),
   m_locationColumnNumber(0),
   m_pathnameRecordOffset(0)
{
   clearFields();
}

ossimRpfFrameFileIndexRecord::ossimRpfFrameFileIndexRecord(const ossimRpfFrameFileIndexRecord& obj)
   :
   m_boundaryRectRecordNumber(obj.m_boundaryRectRecordNumber),
   m_locationRowNumber(obj.m_locationRowNumber),
   m_locationColumnNumber(obj.m_locationColumnNumber),
   m_pathnameRecordOffset(obj.m_pathnameRecordOffset),
   m_securityClassification(obj.m_securityClassification)
{
   memcpy(m_filename, obj.m_filename, 13);
   memcpy(m_geographicLocation, obj.m_geographicLocation, 7);
   memcpy(m_fileSecurityCountryCode, obj.m_fileSecurityCountryCode, 3);
   memcpy(m_fileSecurityReleaseMarking, obj.m_fileSecurityReleaseMarking, 3);
}

const ossimRpfFrameFileIndexRecord& ossimRpfFrameFileIndexRecord::operator=(
   const ossimRpfFrameFileIndexRecord& rhs)
{
   if ( this != &rhs )
   {
      m_boundaryRectRecordNumber = rhs.m_boundaryRectRecordNumber;
      m_locationRowNumber = rhs.m_locationRowNumber;
      m_locationColumnNumber = rhs.m_locationColumnNumber;
      m_pathnameRecordOffset = rhs.m_pathnameRecordOffset;
      m_securityClassification = rhs.m_securityClassification;
      
      memcpy(m_filename, rhs.m_filename, 13);
      memcpy(m_geographicLocation, rhs.m_geographicLocation, 7);
      memcpy(m_fileSecurityCountryCode, rhs.m_fileSecurityCountryCode, 3);
      memcpy(m_fileSecurityReleaseMarking, rhs.m_fileSecurityReleaseMarking, 3);
   }
   return *this;
}

ossimErrorCode ossimRpfFrameFileIndexRecord::parseStream(
   std::istream& in, ossimByteOrder byteOrder)
{
   if(in)
   {
      ossimEndian anEndian;

      clearFields();
            
      in.read((char*)&m_boundaryRectRecordNumber, 2);
      in.read((char*)&m_locationRowNumber, 2);
      in.read((char*)&m_locationColumnNumber, 2);
      in.read((char*)&m_pathnameRecordOffset, 4);
      in.read((char*)m_filename, 12);
      in.read((char*)m_geographicLocation, 6);
      in.read((char*)&m_securityClassification, 1);
      in.read((char*)m_fileSecurityCountryCode, 2);
      in.read((char*)m_fileSecurityReleaseMarking, 2);

      if(anEndian.getSystemEndianType() != byteOrder)
      {
         anEndian.swap(m_boundaryRectRecordNumber);
         anEndian.swap(m_locationRowNumber);
         anEndian.swap(m_locationColumnNumber);
         anEndian.swap(m_pathnameRecordOffset);
      }
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }
   
   return ossimErrorCodes::OSSIM_OK;
}

void ossimRpfFrameFileIndexRecord::writeStream(std::ostream& out)
{
   ossimEndian anEndian;
   
   if( anEndian.getSystemEndianType() != OSSIM_BIG_ENDIAN )
   {
      // Always write out big endian.
      anEndian.swap(m_boundaryRectRecordNumber);
      anEndian.swap(m_locationRowNumber);
      anEndian.swap(m_locationColumnNumber);
      anEndian.swap(m_pathnameRecordOffset);
   }
   
   out.write((char*)&m_boundaryRectRecordNumber, 2);
   out.write((char*)&m_locationRowNumber, 2);
   out.write((char*)&m_locationColumnNumber, 2);
   out.write((char*)&m_pathnameRecordOffset, 4);
   out.write((char*)m_filename, 12);
   out.write((char*)m_geographicLocation, 6);
   out.write((char*)&m_securityClassification, 1);
   out.write((char*)m_fileSecurityCountryCode, 2);
   out.write((char*)m_fileSecurityReleaseMarking, 2);
   
   if( anEndian.getSystemEndianType() != OSSIM_BIG_ENDIAN )
   {
      // Swap back to native.
      anEndian.swap(m_boundaryRectRecordNumber);
      anEndian.swap(m_locationRowNumber);
      anEndian.swap(m_locationColumnNumber);
      anEndian.swap(m_pathnameRecordOffset);
   }
}

std::ostream& ossimRpfFrameFileIndexRecord::print(std::ostream& out)const
{
   out << "BoundaryRectRecordNumber:      " << m_boundaryRectRecordNumber
       << "\nLocationRowNumber:             " << m_locationRowNumber
       << "\nLocationColumnNumber:          " << m_locationColumnNumber
       << "\nPathnameRecordOffset:          " << m_pathnameRecordOffset
       << "\nFilename:                      " << m_filename
       << "\nGeographicLocation:            " << m_geographicLocation
       << "\nSecurityClassification:        " << m_securityClassification
       << "\nFileSecurityCountryCode:       " << m_fileSecurityCountryCode
       << "\nFileSecurityReleaseMarking:    " << m_fileSecurityReleaseMarking
       << std::endl;
   return out;
}

ossim_uint16  ossimRpfFrameFileIndexRecord::getBoundaryRecNumber()const
{
   return m_boundaryRectRecordNumber;
}

ossim_uint16  ossimRpfFrameFileIndexRecord::getLocationRowNumber()const
{
   return m_locationRowNumber;
}

ossim_uint16  ossimRpfFrameFileIndexRecord::getLocationColNumber()const
{
   return m_locationColumnNumber;
}

ossim_uint32  ossimRpfFrameFileIndexRecord::getPathnameRecordOffset()const
{
   return m_pathnameRecordOffset;
}

ossimFilename ossimRpfFrameFileIndexRecord::getFilename()const
{
   return ossimFilename(m_filename);
}

void ossimRpfFrameFileIndexRecord::setBoundaryRecNumber(ossim_uint16 entry)
{
   m_boundaryRectRecordNumber = entry;
}

void ossimRpfFrameFileIndexRecord::setPathnameRecordOffset(ossim_uint32 offset)
{
   m_pathnameRecordOffset = offset;
}

void ossimRpfFrameFileIndexRecord::clearFields()
{
   m_boundaryRectRecordNumber = 0;
   m_locationRowNumber = 0;
   m_locationColumnNumber= 0;
   m_pathnameRecordOffset = 0;
   memset(m_filename, ' ', 12);
   memset(m_geographicLocation, ' ', 6);
   m_securityClassification = ' ';
   memset(m_fileSecurityCountryCode, ' ', 2);
   memset(m_fileSecurityReleaseMarking, ' ', 2);

   m_filename[12] = '\0';
   m_geographicLocation[6] = '\0';
   m_fileSecurityCountryCode[2] = '\0';
   m_fileSecurityReleaseMarking[2] = '\0';
}
