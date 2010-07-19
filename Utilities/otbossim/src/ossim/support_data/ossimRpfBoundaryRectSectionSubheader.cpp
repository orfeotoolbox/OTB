//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfBoundaryRectSectionSubheader.cpp 16997 2010-04-12 18:53:48Z dburken $

#include <istream>
#include <ostream>

#include <ossim/support_data/ossimRpfBoundaryRectSectionSubheader.h>
#include <ossim/base/ossimCommon.h> /* ossim::byteOrder() */
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimErrorCodes.h>

std::ostream& operator <<(std::ostream& out,
                          const ossimRpfBoundaryRectSectionSubheader &data)
{
   return data.print(out);
}

ossimRpfBoundaryRectSectionSubheader::ossimRpfBoundaryRectSectionSubheader()
   :
   m_rectangleTableOffset(0),
   m_numberOfEntries(0),
   m_lengthOfEachEntry(0)
{
}

ossimRpfBoundaryRectSectionSubheader::ossimRpfBoundaryRectSectionSubheader(
   const ossimRpfBoundaryRectSectionSubheader& obj)
   :
   m_rectangleTableOffset(obj.m_rectangleTableOffset),
   m_numberOfEntries(obj.m_numberOfEntries),
   m_lengthOfEachEntry(obj.m_lengthOfEachEntry)
{
}
const ossimRpfBoundaryRectSectionSubheader& ossimRpfBoundaryRectSectionSubheader::operator=(
   const ossimRpfBoundaryRectSectionSubheader& rhs)
{
   if ( this != &rhs )
   {
      m_rectangleTableOffset = rhs.m_rectangleTableOffset;
      m_numberOfEntries = rhs.m_numberOfEntries;
      m_lengthOfEachEntry = rhs.m_lengthOfEachEntry;
   }
   return *this;
}

ossimRpfBoundaryRectSectionSubheader::~ossimRpfBoundaryRectSectionSubheader()
{
}

ossimErrorCode ossimRpfBoundaryRectSectionSubheader::parseStream(std::istream& in,
                                                                 ossimByteOrder byteOrder)
{
   if(in)
   {
      clearFields();
      
      in.read((char*)&m_rectangleTableOffset, 4);
      in.read((char*)&m_numberOfEntries, 2);
      in.read((char*)&m_lengthOfEachEntry, 2);

      if( ossim::byteOrder() != byteOrder )
      {
         ossimEndian anEndian;
         anEndian.swap(m_rectangleTableOffset);
         anEndian.swap(m_numberOfEntries);
         anEndian.swap(m_lengthOfEachEntry);
      }
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }
   
   return ossimErrorCodes::OSSIM_OK;
}

void ossimRpfBoundaryRectSectionSubheader::writeStream(std::ostream& out)
{
   ossimEndian anEndian;
   
   if( anEndian.getSystemEndianType() != OSSIM_BIG_ENDIAN )
   {
      // Always write out big endian.
      anEndian.swap(m_rectangleTableOffset);
      anEndian.swap(m_numberOfEntries);
      anEndian.swap(m_lengthOfEachEntry);
   }

   out.write((char*)&m_rectangleTableOffset, 4);
   out.write((char*)&m_numberOfEntries, 2);
   out.write((char*)&m_lengthOfEachEntry, 2);

   if( anEndian.getSystemEndianType() != OSSIM_BIG_ENDIAN )
   {
      // Swap back to native.
      anEndian.swap(m_rectangleTableOffset);
      anEndian.swap(m_numberOfEntries);
      anEndian.swap(m_lengthOfEachEntry);
   }
}
   
std::ostream& ossimRpfBoundaryRectSectionSubheader::print(std::ostream& out) const
{
   out << "RectangleTableOffset:      " << m_rectangleTableOffset
       << "\nNumberOfEntries:           " << m_numberOfEntries
       << "\nLengthOfEachEntry:         " << m_lengthOfEachEntry << std::endl;
   return out;
}

ossim_uint16 ossimRpfBoundaryRectSectionSubheader::getNumberOfEntries() const
{
   return m_numberOfEntries;
}

ossim_uint32 ossimRpfBoundaryRectSectionSubheader::getTableOffset() const
{
   return m_rectangleTableOffset;
}

ossim_uint16 ossimRpfBoundaryRectSectionSubheader::getLengthOfEachEntry() const
{
   return m_lengthOfEachEntry;
}

void ossimRpfBoundaryRectSectionSubheader::setNumberOfEntries(ossim_uint16 entries)
{
   m_numberOfEntries = entries;
}

void ossimRpfBoundaryRectSectionSubheader::setTableOffset(ossim_uint32 offset)
{
   m_rectangleTableOffset = offset;
}

void ossimRpfBoundaryRectSectionSubheader::setLengthOfEachEntry(ossim_uint16 length)
{
   m_lengthOfEachEntry = length;
}

void ossimRpfBoundaryRectSectionSubheader::clearFields()
{
   m_rectangleTableOffset = 0;
   m_numberOfEntries      = 0;
   m_lengthOfEachEntry    = 0;
}
