//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
// 
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfFrameFileIndexSectionSubheader.cpp 16997 2010-04-12 18:53:48Z dburken $

#include <istream>
#include <ostream>

#include <ossim/support_data/ossimRpfFrameFileIndexSectionSubheader.h>
#include <ossim/base/ossimCommon.h> /* ossim::byteOrder() */
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimTrace.h>

// Static trace for debugging
static ossimTrace traceDebug("ossimRpfFrameFileIndexSectionSubheader:debug");

std::ostream& operator <<(std::ostream& out, const ossimRpfFrameFileIndexSectionSubheader& data)
{
   return data.print(out);
}

ossimRpfFrameFileIndexSectionSubheader::ossimRpfFrameFileIndexSectionSubheader()
   :
   m_highestSecurityClassification(' '),
   m_indexTableOffset(0),
   m_numberOfIndexRecords(0),
   m_numberOfPathnameRecords(0),
   m_indexRecordLength(0)
{
}

ossimRpfFrameFileIndexSectionSubheader::ossimRpfFrameFileIndexSectionSubheader(
   const ossimRpfFrameFileIndexSectionSubheader& obj)
   :
   m_highestSecurityClassification(obj.m_highestSecurityClassification),
   m_indexTableOffset(obj.m_indexTableOffset),
   m_numberOfIndexRecords(obj.m_numberOfIndexRecords),
   m_numberOfPathnameRecords(obj.m_numberOfPathnameRecords),
   m_indexRecordLength(obj.m_indexRecordLength)
{
}

const ossimRpfFrameFileIndexSectionSubheader& ossimRpfFrameFileIndexSectionSubheader::operator=(
   const ossimRpfFrameFileIndexSectionSubheader& rhs)
{
   if ( this != &rhs )
   {
      m_highestSecurityClassification = rhs.m_highestSecurityClassification;
      m_indexTableOffset = rhs.m_indexTableOffset;
      m_numberOfIndexRecords = rhs.m_numberOfIndexRecords;
      m_numberOfPathnameRecords = rhs.m_numberOfPathnameRecords;
      m_indexRecordLength = rhs.m_indexRecordLength;
   }
   return *this;
}

ossimRpfFrameFileIndexSectionSubheader::~ossimRpfFrameFileIndexSectionSubheader()
{
}

ossimErrorCode ossimRpfFrameFileIndexSectionSubheader::parseStream(std::istream& in,
                                                                   ossimByteOrder byteOrder)
{
   if(in)
   {
      in.read((char*)&m_highestSecurityClassification, 1);
      in.read((char*)&m_indexTableOffset, 4);
      in.read((char*)&m_numberOfIndexRecords, 4);
      in.read((char*)&m_numberOfPathnameRecords, 2);
      in.read((char*)&m_indexRecordLength, 2);

      if( ossim::byteOrder() != byteOrder )
      {
         ossimEndian anEndian;
         anEndian.swap(m_indexTableOffset);
         anEndian.swap(m_numberOfIndexRecords);
         anEndian.swap(m_numberOfPathnameRecords);
         anEndian.swap(m_indexRecordLength);
      }
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }

   if ( traceDebug() )
   {
      print( ossimNotify(ossimNotifyLevel_NOTICE) );
   }
   
   return ossimErrorCodes::OSSIM_OK;
}

void ossimRpfFrameFileIndexSectionSubheader::writeStream(std::ostream& out)
{
   if( ossim::byteOrder() != OSSIM_BIG_ENDIAN)
   {
      // Swap to big endian.
      ossimEndian anEndian;
      anEndian.swap(m_indexTableOffset);
      anEndian.swap(m_numberOfIndexRecords);
      anEndian.swap(m_numberOfPathnameRecords);
      anEndian.swap(m_indexRecordLength);
   } 

   out.write((char*)&m_highestSecurityClassification, 1);
   out.write((char*)&m_indexTableOffset, 4);
   out.write((char*)&m_numberOfIndexRecords, 4);
   out.write((char*)&m_numberOfPathnameRecords, 2);
   out.write((char*)&m_indexRecordLength, 2);
   
   if( ossim::byteOrder() != OSSIM_BIG_ENDIAN)
   {
      // Swap back to native byte order.
      ossimEndian anEndian;
      anEndian.swap(m_indexTableOffset);
      anEndian.swap(m_numberOfIndexRecords);
      anEndian.swap(m_numberOfPathnameRecords);
      anEndian.swap(m_indexRecordLength);
   }
}
std::ostream& ossimRpfFrameFileIndexSectionSubheader::print(std::ostream& out) const
{
   out << "ossimRpfFrameFileIndexSectionSubheader DEBUG:"
       << "\nHighestSecurityClassification:  " << m_highestSecurityClassification
       << "\nIndexTableOffset:               " << m_indexTableOffset
       << "\nNumberOfIndexRecords:           " << m_numberOfIndexRecords
       << "\nNumberOfPathnameRecords:        " << m_numberOfPathnameRecords
       << "\nIndexRecordLength:              " << m_indexRecordLength
       << std::endl;

   return out;
}

ossim_uint32 ossimRpfFrameFileIndexSectionSubheader::getOffset() const
{
   return m_indexTableOffset;
}

ossim_uint32 ossimRpfFrameFileIndexSectionSubheader::getNumberOfIndexRecords() const
{
   return m_numberOfIndexRecords;
}

ossim_uint16 ossimRpfFrameFileIndexSectionSubheader::getNumberOfPathnameRecords() const
{
   return m_numberOfPathnameRecords;
}

ossim_uint16 ossimRpfFrameFileIndexSectionSubheader::getIndexRecordLength() const
{
   return m_indexRecordLength;
}

void ossimRpfFrameFileIndexSectionSubheader::setNumberOfIndexRecords(ossim_uint32 count)
{
   m_numberOfIndexRecords = count;
}

void ossimRpfFrameFileIndexSectionSubheader::setNumberOfPathnameRecords(ossim_uint16 count)
{
   m_numberOfPathnameRecords = count;
}

void ossimRpfFrameFileIndexSectionSubheader::setIndexRecordLength(ossim_uint16 length)
{
   m_indexRecordLength = length;
}

void ossimRpfFrameFileIndexSectionSubheader::clearFields()
{
   m_highestSecurityClassification = ' ';
   m_indexTableOffset              = 0;
   m_numberOfIndexRecords          = 0;
   m_numberOfPathnameRecords       = 0;
   m_indexRecordLength             = 0;
}
