//*******************************************************************
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
// Description: Rpf support class.
//
//********************************************************************
// $Id: ossimRpfPathnameRecord.cpp 16997 2010-04-12 18:53:48Z dburken $

#include <istream>
#include <ostream>
#include <ossim/support_data/ossimRpfPathnameRecord.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimErrorCodes.h>

std::ostream& operator <<(std::ostream& out, const ossimRpfPathnameRecord& data)
{
   return data.print(out);
}

ossimRpfPathnameRecord::ossimRpfPathnameRecord()
   :
   m_length(0),
   m_pathname("")
{   
}

ossimRpfPathnameRecord::ossimRpfPathnameRecord(const ossimRpfPathnameRecord& obj)
   :
   m_length(obj.m_length),
   m_pathname(obj.m_pathname)
{
}

const ossimRpfPathnameRecord& ossimRpfPathnameRecord::operator=(const ossimRpfPathnameRecord& rhs)
{
   if ( this != &rhs )
   {
      m_length   = rhs.m_length;
      m_pathname = rhs.m_pathname;
   }
   return *this;
}

void ossimRpfPathnameRecord::clearFields()
{
   m_length   = 0;
   m_pathname = "";
}

ossimErrorCode ossimRpfPathnameRecord::parseStream(std::istream& in, ossimByteOrder byteOrder)
{
   if(in)
   {
      ossimEndian anEndian;
      
      clearFields();
      in.read((char*)&m_length, 2);

      if(anEndian.getSystemEndianType() != byteOrder)
      {
         anEndian.swap(m_length);
      }

      char *temp = new char[m_length+1];
      in.read((char*)temp, m_length);
      temp[m_length] = '\0';
      m_pathname = temp;
      
      delete [] temp;
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }

   return ossimErrorCodes::OSSIM_OK;
}

void ossimRpfPathnameRecord::writeStream(std::ostream& out)
{
   ossimEndian anEndian;
   if( anEndian.getSystemEndianType() != OSSIM_BIG_ENDIAN )
   {
      // Always write big endian.
      anEndian.swap(m_length);
   }
   
   out.write((char*)&m_length, 2);

   if( anEndian.getSystemEndianType() != OSSIM_BIG_ENDIAN )
   {
      // Swap back to native.
      anEndian.swap(m_length);
   }   

   if(m_pathname.size() >= m_length)
   {
      out.write(m_pathname.c_str(), m_length);
   }
}

std::ostream& ossimRpfPathnameRecord::print(std::ostream& out)const
{
   out << "length:        " << m_length
       << "\npathname:      " << m_pathname << std::endl;
   return out;
}

ossimString ossimRpfPathnameRecord::getPathname() const
{
   return m_pathname;
}

void ossimRpfPathnameRecord::setPathName(const ossimString& path)
{
   m_pathname = path;
   m_length = static_cast<ossim_uint16>( path.size() );
}
