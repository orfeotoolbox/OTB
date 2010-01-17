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
// $Id: ossimRpfLocationSection.cpp 14241 2009-04-07 19:59:23Z dburken $

#include <istream>
#include <ostream>
#include <iterator>

#include <ossim/support_data/ossimRpfLocationSection.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimErrorCodes.h>
#include <ossim/base/ossimTrace.h>

static const ossimTrace traceDebug("ossimRpfLocationSection:debug");

std::ostream& operator<<(std::ostream& out,
                         const ossimRpfComponentLocationRecord& data)
{
   data.print(out);

   return out;
}

std::ostream& ossimRpfComponentLocationRecord::print(
   std::ostream& out, const std::string& prefix) const
{
   out << prefix << "ComponentId:             "
       << theComponentId << "\n"
       << prefix << "ComponentLength:         "
       << theComponentLength   << "\n"
       << prefix << "ComponentLocation:       "
       << theComponentLocation << "\n";
   return out;
}

std::ostream& operator <<(std::ostream& out,
                          const ossimRpfLocationSection &data)
{
   data.print(out);

   return out;
}

ossimErrorCode ossimRpfComponentLocationRecord::parseStream(
   std::istream& in, ossimByteOrder byteOrder)
{
   if(in)
   {
      ossimEndian anEndian;

      
      in.read((char*)&theComponentId, 2);
      in.read((char*)&theComponentLength, 4);
      in.read((char*)&theComponentLocation, 4);

      if(anEndian.getSystemEndianType() != byteOrder)
      {
         anEndian.swap(theComponentId);
         anEndian.swap(theComponentLength);
         anEndian.swap(theComponentLocation);
      }
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }

   return  ossimErrorCodes::OSSIM_OK;
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
      ossimEndian anEndian;

      in.read((char*)&theLocationSectionLength, 2);
      in.read((char*)&theLocationTableOffset, 4);
      in.read((char*)&theNumberOfComponentLocationRecords, 2);
      in.read((char*)&theLocationRecordLength, 2);
      in.read((char*)&theComponentAggregateLength, 4);
      
      if(anEndian.getSystemEndianType() != byteOrder)
      {
          anEndian.swap(theLocationSectionLength);
          anEndian.swap(theLocationTableOffset);
          anEndian.swap(theNumberOfComponentLocationRecords);
          anEndian.swap(theLocationRecordLength);
          anEndian.swap(theComponentAggregateLength);
      }
      if(traceDebug())
      {
         print(ossimNotify(ossimNotifyLevel_DEBUG));
         ossimNotify(ossimNotifyLevel_DEBUG) << std::endl;
      }
      theComponentLocationList.resize(theNumberOfComponentLocationRecords);
      for(unsigned long index = 0;
          (index < theComponentLocationList.size())&&
                      (result == ossimErrorCodes::OSSIM_OK);
          index++)
      {        
         result = theComponentLocationList[index].parseStream(in, byteOrder);
      }
   }
   else
   {
      result = ossimErrorCodes::OSSIM_ERROR;
   }

   return result;
}

std::ostream& ossimRpfLocationSection::print(
   std::ostream& out, const std::string& prefix) const
{
   out << prefix << "LocationSectionLength:            "
       << theLocationSectionLength << "\n"
       << prefix << "LocationTableOffset:              "
       << theLocationTableOffset << "\n"
       << prefix << "NumberOfComponentLocationRecords: "
       << theNumberOfComponentLocationRecords << "\n"
       << prefix << "LocationRecordLength:             "
       << theLocationRecordLength << "\n"
       << prefix << "ComponentAggregateLength:         "
       << theComponentAggregateLength << "\n";
   
   if(theNumberOfComponentLocationRecords > 0)
   {
      std::vector<ossimRpfComponentLocationRecord>::const_iterator i =
         theComponentLocationList.begin();
      while (i != theComponentLocationList.end())
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
      theComponentLocationList.begin();
   
   while(component != theComponentLocationList.end())
   {
      if((*component).theComponentId == static_cast<unsigned short>(componentId))
      {
         result = *component;

         return true;
      }      
      ++component;
   }
   
   return false;
}

void ossimRpfLocationSection::clearFields()
{
   theLocationSectionLength            = 0;
   theLocationTableOffset              = 0;
   theNumberOfComponentLocationRecords = 0;
   theLocationRecordLength             = 0;
   theComponentAggregateLength         = 0;

   theComponentLocationList.clear();
}
