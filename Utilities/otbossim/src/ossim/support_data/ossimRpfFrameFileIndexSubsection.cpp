//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
// 
// Description: Rpf support class.
// 
//********************************************************************
// $Id: ossimRpfFrameFileIndexSubsection.cpp 16997 2010-04-12 18:53:48Z dburken $

#include <cstring> /* for memset */
#include <istream>
#include <iterator>
#include <ostream>
#include <ossim/support_data/ossimRpfFrameFileIndexSubsection.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimErrorCodes.h>
#include <ossim/base/ossimFilename.h>

std::ostream& operator <<(std::ostream& out, const ossimRpfFrameFileIndexSubsection& data)
{
   return data.print(out);
}

ossimRpfFrameFileIndexSubsection::ossimRpfFrameFileIndexSubsection()
   :
   m_indexTable(0),
   m_pathnameTable(0)
{
}

ossimRpfFrameFileIndexSubsection::ossimRpfFrameFileIndexSubsection(
   const ossimRpfFrameFileIndexSubsection& obj)
   :
   m_indexTable(obj.m_indexTable),
   m_pathnameTable(obj.m_pathnameTable)
{
}

const ossimRpfFrameFileIndexSubsection& ossimRpfFrameFileIndexSubsection::operator=(
   const ossimRpfFrameFileIndexSubsection& rhs)
{
   if ( this != &rhs )
   {
      m_indexTable = rhs.m_indexTable;
      m_pathnameTable = rhs.m_pathnameTable;
   }
   return *this;
}

ossimRpfFrameFileIndexSubsection::~ossimRpfFrameFileIndexSubsection()
{
}

ossimErrorCode ossimRpfFrameFileIndexSubsection::parseStream(std::istream &in,
                                                             ossimByteOrder byteOrder)
{
   ossimErrorCode result = ossimErrorCodes::OSSIM_OK;
   
   if(in && (m_indexTable.size()>0))
   {
      ossim_uint32 index;
      for(index = 0;
          (index < m_indexTable.size()) && (result == ossimErrorCodes::OSSIM_OK);
          ++index)
      {
         result = m_indexTable[index].parseStream(in, byteOrder);
      }
      for(index = 0;
          (index < m_pathnameTable.size()) && (result ==ossimErrorCodes::OSSIM_OK);
          ++index)
      {
         result = m_pathnameTable[index].parseStream(in, byteOrder);
      }
   }
   else
   {
      result = ossimErrorCodes::OSSIM_ERROR;
   }

   return result;
}

void ossimRpfFrameFileIndexSubsection::writeStream(std::ostream& out)
{
   std::vector<ossimRpfFrameFileIndexRecord>::iterator indexIter = m_indexTable.begin();
   while ( indexIter != m_indexTable.end() )
   {
      (*indexIter).writeStream(out);
      ++indexIter;
   }

   std::vector<ossimRpfPathnameRecord>::iterator pathIter = m_pathnameTable.begin();
   while ( pathIter != m_pathnameTable.end() )
   {
      (*pathIter).writeStream(out);
      ++pathIter;
   }
}

void ossimRpfFrameFileIndexSubsection::clearFields()
{
   m_indexTable.clear();
   m_pathnameTable.clear();
}

std::ostream& ossimRpfFrameFileIndexSubsection::print(std::ostream& out)const
{
   copy(m_indexTable.begin(),
        m_indexTable.end(),
        std::ostream_iterator<ossimRpfFrameFileIndexRecord>(out, "\n"));
   copy(m_pathnameTable.begin(),
        m_pathnameTable.end(),
        std::ostream_iterator<ossimRpfPathnameRecord>(out, "\n"));
   return out;
}

void ossimRpfFrameFileIndexSubsection::setNumberOfFileIndexRecords(
   ossim_uint32 numberOfIndexRecords)
{
   m_indexTable.resize(numberOfIndexRecords);
}

void ossimRpfFrameFileIndexSubsection::setNumberOfPathnames(ossim_uint32 numberOfPathnames)
{
   m_pathnameTable.resize(numberOfPathnames);
}

const std::vector<ossimRpfFrameFileIndexRecord>&
ossimRpfFrameFileIndexSubsection::getIndexTable()const
{
   return m_indexTable;
}

const std::vector<ossimRpfPathnameRecord>&
ossimRpfFrameFileIndexSubsection::getPathnameTable() const
{
   return m_pathnameTable;
}

bool ossimRpfFrameFileIndexSubsection::getFrameFileIndexRecordFromFile(
   const ossimFilename& file, ossimRpfFrameFileIndexRecord& record) const
{
   bool result = false;
   std::vector<ossimRpfFrameFileIndexRecord>::const_iterator i = m_indexTable.begin();
   while ( i != m_indexTable.end() )
   {
      if ( (*i).getFilename() == file )
      {
         record = (*i);
         result = true;
         break;
      }
      ++i;
   }
   return result;
}
