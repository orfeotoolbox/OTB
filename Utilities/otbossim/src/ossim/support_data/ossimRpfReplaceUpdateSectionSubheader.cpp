//----------------------------------------------------------------------------
//
// File:     ossimRpfReplaceUpdateSectionSubheader.cpp
// 
// License:  See top level LICENSE.txt file.
// 
// Author:   David Burken
//
// Description:
//
// RPF replace/update section subheader record.
//
// See MIL-STD-2411 for detailed information.
// 
//----------------------------------------------------------------------------
// $Id: ossimRpfReplaceUpdateSectionSubheader.cpp 20324 2011-12-06 22:25:23Z dburken $

#include <ossim/support_data/ossimRpfReplaceUpdateSectionSubheader.h>
#include <ossim/base/ossimCommon.h> /* ossim::byteOrder() */
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimTrace.h>
#include <istream>
#include <ostream>

// Static trace for debugging
static ossimTrace traceDebug("ossimRpfReplaceUpdateSectionSubheader:debug");

std::ostream& operator <<(std::ostream& out, const ossimRpfReplaceUpdateSectionSubheader& data)
{
   return data.print(out);
}

ossimRpfReplaceUpdateSectionSubheader::ossimRpfReplaceUpdateSectionSubheader()
   :
   m_tableOffset(0),
   m_numberOfRecords(0),
   m_recordLength(0)
{
}

ossimRpfReplaceUpdateSectionSubheader::ossimRpfReplaceUpdateSectionSubheader(
   const ossimRpfReplaceUpdateSectionSubheader& obj)
   :
   m_tableOffset(obj.m_tableOffset),
   m_numberOfRecords(obj.m_numberOfRecords),
   m_recordLength(obj.m_recordLength)
{
}

const ossimRpfReplaceUpdateSectionSubheader& ossimRpfReplaceUpdateSectionSubheader::operator=(
   const ossimRpfReplaceUpdateSectionSubheader& rhs)
{
   if ( this != &rhs )
   {
      m_tableOffset     = rhs.m_tableOffset;
      m_numberOfRecords = rhs.m_numberOfRecords;
      m_recordLength    = rhs.m_recordLength;
   }
   return *this;
}

ossimRpfReplaceUpdateSectionSubheader::~ossimRpfReplaceUpdateSectionSubheader()
{
}

ossimErrorCode ossimRpfReplaceUpdateSectionSubheader::parseStream(std::istream& in,
                                                                  ossimByteOrder byteOrder)
{
   if(in)
   {
      in.read((char*)&m_tableOffset, 4);
      in.read((char*)&m_numberOfRecords, 2);
      in.read((char*)&m_recordLength, 2);

      if( ossim::byteOrder() != byteOrder )
      {
         ossimEndian anEndian;
         anEndian.swap(m_tableOffset);
         anEndian.swap(m_numberOfRecords);
         anEndian.swap(m_recordLength);
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

void ossimRpfReplaceUpdateSectionSubheader::writeStream(std::ostream& out)
{
   if( ossim::byteOrder() != OSSIM_BIG_ENDIAN)
   {
      // Swap to big endian.
      ossimEndian anEndian;
      anEndian.swap(m_tableOffset);
      anEndian.swap(m_numberOfRecords);
      anEndian.swap(m_recordLength);
   } 

   out.write((char*)&m_tableOffset, 4);
   out.write((char*)&m_numberOfRecords, 2);
   out.write((char*)&m_recordLength, 2);
   
   if( ossim::byteOrder() != OSSIM_BIG_ENDIAN)
   {
      // Swap back to native byte order.
      ossimEndian anEndian;
      anEndian.swap(m_tableOffset);
      anEndian.swap(m_numberOfRecords);
      anEndian.swap(m_recordLength);
   }
}
std::ostream& ossimRpfReplaceUpdateSectionSubheader::print(std::ostream& out) const
{
   out << "ossimRpfReplaceUpdateSectionSubheader DEBUG:"
       << "\ntable_offset:      " << m_tableOffset
       << "\nnumber_of_records: " << m_numberOfRecords
       << "\nrecord_length:     " << m_recordLength
       << std::endl;

   return out;
}

ossim_uint32 ossimRpfReplaceUpdateSectionSubheader::getOffset() const
{
   return m_tableOffset;
}

ossim_uint16 ossimRpfReplaceUpdateSectionSubheader::getNumberOfRecords() const
{
   return m_numberOfRecords;
}

ossim_uint16 ossimRpfReplaceUpdateSectionSubheader::getRecordLength() const
{
   return m_recordLength;
}

void ossimRpfReplaceUpdateSectionSubheader::setNumberOfRecords(ossim_uint16 count)
{
   m_numberOfRecords = count;
}

void ossimRpfReplaceUpdateSectionSubheader::setRecordLength(ossim_uint16 length)
{
   m_recordLength = length;
}

void ossimRpfReplaceUpdateSectionSubheader::clearFields()
{
   m_tableOffset     = 0;
   m_numberOfRecords = 0;
   m_recordLength    = 0;
}


