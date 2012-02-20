//----------------------------------------------------------------------------
//
// File:     ossimRpfReplaceUpdateRecord.cpp
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
// $Id: ossimRpfReplaceUpdateRecord.cpp 20324 2011-12-06 22:25:23Z dburken $

#include <ossim/support_data/ossimRpfReplaceUpdateRecord.h>
#include <ossim/support_data/ossimNitfCommon.h>
#include <cstring> /* for memset, memcpy */
#include <istream>
#include <ostream>

std::ostream& operator <<(std::ostream& out, const ossimRpfReplaceUpdateRecord& data)
{
   return data.print(out);
}

ossimRpfReplaceUpdateRecord::ossimRpfReplaceUpdateRecord()
   :
   m_updateStatus(0)
{
   clearFields();
}

ossimRpfReplaceUpdateRecord::ossimRpfReplaceUpdateRecord(const ossimRpfReplaceUpdateRecord& obj)
   :
   m_updateStatus(obj.m_updateStatus)
{
   memcpy(m_newFile, obj.m_newFile, 13);
   memcpy(m_oldFile, obj.m_oldFile, 13);
}

const ossimRpfReplaceUpdateRecord& ossimRpfReplaceUpdateRecord::operator=(
   const ossimRpfReplaceUpdateRecord& rhs)
{
   if ( this != &rhs )
   {
      memcpy(m_newFile, rhs.m_newFile, 13);
      memcpy(m_oldFile, rhs.m_oldFile, 13);
      m_updateStatus = rhs.m_updateStatus;      
   }
   return *this;
}

ossimErrorCode ossimRpfReplaceUpdateRecord::parseStream( std::istream& in )
{
   ossimErrorCode status = ossimErrorCodes::OSSIM_OK;
   if( in.good() )
   {
      clearFields();
      in.read((char*)&m_newFile, 12);
      in.read((char*)&m_oldFile, 12);
      in.read((char*)&m_updateStatus, 1);
   }

   if ( in.fail() )
   {
      status = ossimErrorCodes::OSSIM_ERROR;
   }
   
   return status;
}

void ossimRpfReplaceUpdateRecord::writeStream(std::ostream& out)
{
   out.write((char*)&m_newFile, 12);
   out.write((char*)&m_oldFile, 12);
   out.write((char*)&m_updateStatus, 1);
}

std::ostream& ossimRpfReplaceUpdateRecord::print(std::ostream& out,
                                                 const std::string& prefix,
                                                 ossim_uint32 recordNumber)const
{
   std::string pfx = prefix;
   pfx += "replace_update_record";
   pfx += ossimString::toString(recordNumber).string();
   pfx += ".";
   out << pfx << "new_file: "   << m_newFile << "\n"
       << pfx << "old_file: " << m_oldFile << "\n"
       << pfx << "update_status: " << int(m_updateStatus)
       << std::endl;
   return out;
}

void ossimRpfReplaceUpdateRecord::getNewFilename(std::string& file) const
{
   file = m_newFile;
}

void ossimRpfReplaceUpdateRecord::getOldFilename(std::string& file) const
{
   file = m_oldFile;
}

ossim_uint8 ossimRpfReplaceUpdateRecord::getUpdateStatus() const
{
   return m_updateStatus;
}

void ossimRpfReplaceUpdateRecord::setNewFilename(const std::string& file)
{
   ossimNitfCommon::setField( m_newFile, ossimString(file), 12 );
}

void ossimRpfReplaceUpdateRecord::setOldFilename(const std::string& file)
{
   ossimNitfCommon::setField( m_oldFile, ossimString(file), 12 );
}

void ossimRpfReplaceUpdateRecord::setUpdateStatus(ossim_uint8 status)
{
   m_updateStatus = status;
}

void ossimRpfReplaceUpdateRecord::clearFields()
{
   memset(m_newFile, ' ', 12);
   memset(m_oldFile, ' ', 12);   
   m_updateStatus = 0;

   m_newFile[12] = '\0';
   m_oldFile[12] = '\0';
}
