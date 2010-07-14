//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
// 
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfBoundaryRectTable.cpp 16997 2010-04-12 18:53:48Z dburken $


#include <istream>
#include <iterator>
#include <ostream>

#include <ossim/support_data/ossimRpfBoundaryRectTable.h>
#include <ossim/base/ossimEndian.h>

std::ostream& operator <<(std::ostream& out, const ossimRpfBoundaryRectTable& data)
{
   return data.print(out);
}

ossimRpfBoundaryRectTable::ossimRpfBoundaryRectTable(ossim_uint32 numberOfEntries)
   :
   m_table(numberOfEntries)
{
}

ossimRpfBoundaryRectTable::ossimRpfBoundaryRectTable(const ossimRpfBoundaryRectTable& obj)
   :
   m_table(obj.m_table)
{
}

const ossimRpfBoundaryRectTable& ossimRpfBoundaryRectTable::operator=(
   const ossimRpfBoundaryRectTable& rhs)
{
   if ( this != &rhs )
   {
      m_table = rhs.m_table;
   }
   return *this;
}

ossimRpfBoundaryRectTable::~ossimRpfBoundaryRectTable()
{}

ossimErrorCode ossimRpfBoundaryRectTable::parseStream(std::istream& in,
                                                      ossimByteOrder byteOrder)
{
   if(in)
   {
      std::vector<ossimRpfBoundaryRectRecord>::iterator entry = m_table.begin();
      
      while(entry != m_table.end())
      {
         (*entry).parseStream(in, byteOrder);
         ++entry;
      }
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }

   return ossimErrorCodes::OSSIM_OK;
}

void ossimRpfBoundaryRectTable::writeStream(std::ostream& out)
{
   std::vector<ossimRpfBoundaryRectRecord>::iterator entry = m_table.begin();
   while ( entry != m_table.end() )
   {
      (*entry).writeStream(out);
      ++entry;
   }
}

void ossimRpfBoundaryRectTable::setNumberOfEntries(ossim_uint32 numberOfEntries)
{
   m_table.resize(numberOfEntries);
}

ossim_uint32 ossimRpfBoundaryRectTable::getNumberOfEntries() const
{
   return static_cast<ossim_uint32>(m_table.size());
}

bool ossimRpfBoundaryRectTable::getEntry(
   ossim_uint32 entry, ossimRpfBoundaryRectRecord& record) const
{
   bool result = true;
   if ( entry < m_table.size() )
   {
      record = m_table[entry];
   }
   else
   {
      result = false;
   }
   return result;
}

std::ostream& ossimRpfBoundaryRectTable::print(std::ostream& out)const
{
   std::copy(m_table.begin(), m_table.end(),
             std::ostream_iterator<ossimRpfBoundaryRectRecord>(out, "\n"));
   return out;
}
