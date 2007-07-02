//*******************************************************************
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
// Description: This class extends the stl's string class.
//
//********************************************************************
// $Id: ossimRpfLocationSection.cpp 9963 2006-11-28 21:11:01Z gpotts $
#include <ossim/support_data/ossimRpfLocationSection.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimErrorCodes.h>
#include <ossim/base/ossimTrace.h>

static const ossimTrace traceDebug("ossimRpfLocationSection:debug");

ostream& operator<<(ostream& out,
                    const ossimRpfComponentLocationRecord& data)
{
   data.print(out);

   return out;
}

void ossimRpfComponentLocationRecord::print(ostream& out)const
{
   out << "theComponentId:             " << theComponentId       << endl
       << "theComponentLength:         " << theComponentLength   << endl
       << "theComponentLocation:       " << theComponentLocation;
}

ostream& operator <<(ostream& out,
                     const ossimRpfLocationSection &data)
{
   data.print(out);

   return out;
}

ossimErrorCode ossimRpfComponentLocationRecord::parseStream(istream& in,
                                                            ossimByteOrder byteOrder)
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

ossimErrorCode ossimRpfLocationSection::parseStream(istream& in,
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

void ossimRpfLocationSection::print(ostream& out) const
{
   out << "theLocationSectionLength:            " << theLocationSectionLength << endl
       << "theLocationTableOffset:              " << theLocationTableOffset << endl
       << "theNumberOfComponentLocationRecords: " << theNumberOfComponentLocationRecords << endl
       << "theLocationRecordLength:             " << theLocationRecordLength << endl
       << "theComponentAggregateLength:         " << theComponentAggregateLength;
   
   if(theNumberOfComponentLocationRecords > 0)
   {
      out << endl;
      copy(theComponentLocationList.begin(),
           theComponentLocationList.end(),
           ostream_iterator<ossimRpfComponentLocationRecord>(out, "\n"));
   }
}

bool ossimRpfLocationSection::hasComponent(ossimRpfComponentId componentId)const
{
   ossimRpfComponentLocationRecord result;
   
   return getComponent(componentId, result);
}

bool ossimRpfLocationSection::getComponent(ossimRpfComponentId componentId,
                                           ossimRpfComponentLocationRecord &result)const
{
   vector<ossimRpfComponentLocationRecord>::const_iterator component = theComponentLocationList.begin();
   
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
