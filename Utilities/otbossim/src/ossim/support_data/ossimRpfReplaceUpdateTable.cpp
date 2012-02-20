//----------------------------------------------------------------------------
//
// File:     ossimRpfReplaceUpdateTable.cpp
// 
// License:  See top level LICENSE.txt file.
// 
// Author:   David Burken
//
// Description:
//
// Holds a table of RPF replace/update section subheader records.
//
// See MIL-STD-2411 for detailed information.
// 
//----------------------------------------------------------------------------
// $Id: ossimRpfReplaceUpdateTable.cpp 20324 2011-12-06 22:25:23Z dburken $

#include <ossim/support_data/ossimRpfReplaceUpdateTable.h>
#include <iostream>

ossimRpfReplaceUpdateTable::ossimRpfReplaceUpdateTable()
   :
   m_table(0)
{
}

ossimRpfReplaceUpdateTable::ossimRpfReplaceUpdateTable(const ossimRpfReplaceUpdateTable& obj)
   :
   m_table(obj.m_table)
{
}

const ossimRpfReplaceUpdateTable& ossimRpfReplaceUpdateTable::operator=(
   const ossimRpfReplaceUpdateTable& rhs)
{
   if ( this != &rhs )
   {
      m_table = rhs.m_table;
   }
   return *this;
}

void ossimRpfReplaceUpdateTable::addRecord(const ossimRpfReplaceUpdateRecord& record)
{
   m_table.push_back(record);
}

void ossimRpfReplaceUpdateTable::clear()
{
   m_table.clear();
}

std::ostream& ossimRpfReplaceUpdateTable::print( std::ostream& out,
                                                 const std::string& prefix ) const
{
   ossim_uint32 size = static_cast<ossim_uint32>( m_table.size() );
   for (ossim_uint32 i = 0; i < size; ++i)
   {
      m_table[i].print(out, prefix, i);
   }
   return out;
}
