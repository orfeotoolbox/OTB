//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
// Description: Rpf support class
//
//********************************************************************
// $Id: ossimRpfLocationSection.cpp 20324 2011-12-06 22:25:23Z dburken $

#include <ossim/support_data/ossimRpfLocationSection.h>
#include <ossim/base/ossimCommon.h> /* ossim::byteOrder() */
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimErrorCodes.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/support_data/ossimRpfComponentIdLut.h>
#include <istream>
#include <ostream>
#include <iterator>

static const ossimTrace traceDebug("ossimRpfLocationSection:debug");

std::ostream& operator<<(std::ostream& out, const ossimRpfComponentLocationRecord& data)
{
   return data.print(out);
}

ossimRpfComponentLocationRecord::ossimRpfComponentLocationRecord()
   : m_componentId(0),
     m_componentLength(0),
     m_componentLocation(0)
{
}

ossimRpfComponentLocationRecord::ossimRpfComponentLocationRecord(
   const ossimRpfComponentLocationRecord& record)
   : m_componentId(record.m_componentId),
     m_componentLength(record.m_componentLength),
     m_componentLocation(record.m_componentLocation)
{
}

const ossimRpfComponentLocationRecord& ossimRpfComponentLocationRecord::operator=(
   const ossimRpfComponentLocationRecord& rhs)
{
   if (this != &rhs)
   {
      m_componentId       = rhs.m_componentId;
      m_componentLength   = rhs.m_componentLength;
      m_componentLocation = rhs.m_componentLocation;
   }
   return *this;
}

std::ostream& ossimRpfComponentLocationRecord::print(
   std::ostream& out, const std::string& prefix) const
{
   out << prefix << "ComponentId:             "
       << m_componentId << "\n"
       << prefix << "ComponentIdString:       "
       << ossimRpfComponentIdLut::instance()->getEntryString(m_componentId) << "\n"
       << prefix << "ComponentLength:         "
       << m_componentLength   << "\n"
       << prefix << "ComponentLocation:       "
       << m_componentLocation << "\n";
   return out;
}

std::ostream& operator <<(std::ostream& out, const ossimRpfLocationSection &data)
{
   return data.print(out);
}

ossimErrorCode ossimRpfComponentLocationRecord::parseStream(
   std::istream& in, ossimByteOrder byteOrder)
{
   if(in)
   {
      in.read((char*)&m_componentId, 2);
      in.read((char*)&m_componentLength, 4);
      in.read((char*)&m_componentLocation, 4);

      if( ossim::byteOrder() != byteOrder)
      {
         // swap to native
         ossimEndian anEndian;
         anEndian.swap(m_componentId);
         anEndian.swap(m_componentLength);
         anEndian.swap(m_componentLocation);
      }
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }

   return  ossimErrorCodes::OSSIM_OK;
}

void ossimRpfComponentLocationRecord::writeStream(std::ostream& out)
{
   if( ossim::byteOrder() != OSSIM_BIG_ENDIAN)
   {
      ossimEndian endian;
      endian.swap(m_componentId);
      endian.swap(m_componentLength);
      endian.swap(m_componentLocation);
   }

   out.write((char*)&m_componentId, 2);
   out.write((char*)&m_componentLength, 4);
   out.write((char*)&m_componentLocation, 4);

   if( ossim::byteOrder() != OSSIM_BIG_ENDIAN)
   {
      // Swap back to native byte order.
      ossimEndian endian;
      endian.swap(m_componentId);
      endian.swap(m_componentLength);
      endian.swap(m_componentLocation);
   }
}

ossimRpfLocationSection::ossimRpfLocationSection()
{
   clearFields();
}

ossimErrorCode ossimRpfLocationSection::parseStream(std::istream& in,
                                                    ossimByteOrder byteOrder)
{
   ossimErrorCode result = ossimErrorCodes::OSSIM_OK;
   
   if(in)
   {
      clearFields();
      
      in.read((char*)&m_locationSectionLength, 2);
      in.read((char*)&m_locationTableOffset, 4);
      in.read((char*)&m_numberOfComponentLocationRecords, 2);
      in.read((char*)&m_locationRecordLength, 2);
      in.read((char*)&m_componentAggregateLength, 4);

      if( ossim::byteOrder() != byteOrder )
      {
         ossimEndian anEndian;
         anEndian.swap(m_locationSectionLength);
         anEndian.swap(m_locationTableOffset);
         anEndian.swap(m_numberOfComponentLocationRecords);
         anEndian.swap(m_locationRecordLength);
         anEndian.swap(m_componentAggregateLength);
      }
      
      if(traceDebug())
      {
         print(ossimNotify(ossimNotifyLevel_DEBUG));
         ossimNotify(ossimNotifyLevel_DEBUG) << std::endl;
      }

      m_componentLocationList.resize(m_numberOfComponentLocationRecords);
      for(ossim_uint32 index = 0;
          (index < m_componentLocationList.size())&&
             (result == ossimErrorCodes::OSSIM_OK);
          ++index)
      {        
         result = m_componentLocationList[index].parseStream(in, byteOrder);
      }
   }
   else
   {
      result = ossimErrorCodes::OSSIM_ERROR;
   }

   return result;
}


void ossimRpfLocationSection::writeStream(std::ostream& out)
{
   if( ossim::byteOrder() != OSSIM_BIG_ENDIAN)
   {
      // Always write in big endian.
      ossimEndian endian;
      endian.swap(m_locationSectionLength);
      endian.swap(m_locationTableOffset);
      endian.swap(m_numberOfComponentLocationRecords);
      endian.swap(m_locationRecordLength);
      endian.swap(m_componentAggregateLength);
   }
   
   out.write((char*)&m_locationSectionLength, 2);
   out.write((char*)&m_locationTableOffset, 4);
   out.write((char*)&m_numberOfComponentLocationRecords, 2);
   out.write((char*)&m_locationRecordLength, 2);
   out.write((char*)&m_componentAggregateLength, 4);

   if( ossim::byteOrder() != OSSIM_BIG_ENDIAN)
   {
      // Swap back to native byte order.
      ossimEndian endian;
      endian.swap(m_locationSectionLength);
      endian.swap(m_locationTableOffset);
      endian.swap(m_numberOfComponentLocationRecords);
      endian.swap(m_locationRecordLength);
      endian.swap(m_componentAggregateLength);
   }
   
   for(ossim_uint32 i = 0; i < m_componentLocationList.size(); ++i)
   {
      m_componentLocationList[i].writeStream(out);
   }
}

std::ostream& ossimRpfLocationSection::print(
   std::ostream& out, const std::string& prefix) const
{
   out << prefix << "LocationSectionLength:            "
       << m_locationSectionLength << "\n"
       << prefix << "LocationTableOffset:              "
       << m_locationTableOffset << "\n"
       << prefix << "NumberOfComponentLocationRecords: "
       << m_numberOfComponentLocationRecords << "\n"
       << prefix << "LocationRecordLength:             "
       << m_locationRecordLength << "\n"
       << prefix << "ComponentAggregateLength:         "
       << m_componentAggregateLength << "\n";
   
   if(m_numberOfComponentLocationRecords > 0)
   {
      std::vector<ossimRpfComponentLocationRecord>::const_iterator i =
         m_componentLocationList.begin();
      while (i != m_componentLocationList.end())
      {
         (*i).print(out, prefix);
         ++i;
      }
   }
   return out;
}

bool ossimRpfLocationSection::hasComponent(ossimRpfComponentId componentId)const
{
   ossimRpfComponentLocationRecord result;
   
   return getComponent(componentId, result);
}

bool ossimRpfLocationSection::getComponent(ossimRpfComponentId componentId,
                                           ossimRpfComponentLocationRecord &result)const
{
   std::vector<ossimRpfComponentLocationRecord>::const_iterator component =
      m_componentLocationList.begin();
   
   while(component != m_componentLocationList.end())
   {
      if((*component).m_componentId == static_cast<unsigned short>(componentId))
      {
         result = *component;

         return true;
      }      
      ++component;
   }
   
   return false;
}

void ossimRpfLocationSection::addComponentRecord(const ossimRpfComponentLocationRecord& record)
{
   m_componentLocationList.push_back(record);
}

void ossimRpfLocationSection::setLocationSectionLength(ossim_uint16 length)
{
   m_locationSectionLength = length;
}

void ossimRpfLocationSection::setLocationTableOffset(ossim_uint32 offset)
{
   m_locationTableOffset = offset;
}

void ossimRpfLocationSection::setNumberOfComponentLocationRecords(ossim_uint16 count)
{
   m_numberOfComponentLocationRecords = count;
}

void ossimRpfLocationSection::setLocationRecordLength(ossim_uint16 length)
{
   m_locationRecordLength = length;
}

void ossimRpfLocationSection::setComponentAggregateLength(ossim_uint32 length)
{
   m_componentAggregateLength = length;
}

void ossimRpfLocationSection::clearFields()
{
   m_locationSectionLength            = 0;
   m_locationTableOffset              = 0;
   m_numberOfComponentLocationRecords = 0;
   m_locationRecordLength             = 0;
   m_componentAggregateLength         = 0;

   m_componentLocationList.clear();
}

std::vector<ossimRpfComponentLocationRecord>& ossimRpfLocationSection::getLocationRecordList()
{
   return m_componentLocationList;
}
