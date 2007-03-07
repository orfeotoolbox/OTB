//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfLocationSection.cpp,v 1.2 2005/08/24 12:02:47 gpotts Exp $
#include "ossimRpfLocationSection.h"
#include <base/misc/ossimEndian.h>
#include <base/common/ossimErrorCodes.h>
#include <base/common/ossimTrace.h>

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
