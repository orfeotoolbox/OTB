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
// $Id: ossimRpfLocationSection.h,v 1.3 2003/06/15 19:01:34 dburken Exp $
#ifndef ossimRpfLocationSection_HEADER
#define ossimRpfLocationSection_HEADER
#include <iostream>
#include <vector>
#include <iterator>
using namespace std;

#include "base/common/ossimConstants.h" // ossiByteOrder
#include "ossimRpfConstants.h"
#include "base/context/ossimErrorContext.h" // for ossimErrorCode

struct ossimRpfComponentLocationRecord
{
public:
   friend ostream& operator<<(ostream& out,
                              const ossimRpfComponentLocationRecord& data);
   void print(ostream& out)const;
   ossimErrorCode parseStream(istream& in, ossimByteOrder endianOrder);

   /*!
    * The component is a 2-Byte unsigned value which
    * identitfies a level-2 component.  The table can be
    * found in MIL-STD-2411-1 in section 5.1.1
    */
   ossim_uint16 theComponentId;

   /*!
    * Is a 4 byte unsigned integer indicating the length in
    * bytes of the component.
    */
   ossim_uint32  theComponentLength;

   /*!
    * Is a 4 byte value defining the absolute address (byte number)
    * of the first byte of the component measured
    * from the beginning of the RPF Frame file.
    */ 
   ossim_uint32  theComponentLocation;
};

class ossimRpfLocationSection
{
public:
   friend ostream& operator <<(ostream& out,
                               const ossimRpfLocationSection &data);
   ossimRpfLocationSection();
   virtual ~ossimRpfLocationSection(){}

   virtual ossimErrorCode parseStream(istream& in,
                            ossimByteOrder endianOrder);
   virtual void print(ostream& out)const;

   /*!
    * Will let you know if a component exists within the location section.
    * these component Ids' are defined in the MIL-STD-2411-1 standards doc.
    * the ossimRpfComponentId is in ossimConstants.h
    */
   bool hasComponent(ossimRpfComponentId componentId)const;

   bool getComponent(ossimRpfComponentId componentId,
                     ossimRpfComponentLocationRecord& result)const;

   
   ossim_uint32 getOffset()const;
   
private:
   void clearFields();

   /*!
    * This is a 2 byte unsigned short >= 34 indicatin
    * the length in bytes of the entire location section
    */
   ossim_uint16 theLocationSectionLength;

   ossim_uint32  theLocationTableOffset;

   ossim_uint16 theNumberOfComponentLocationRecords;

   ossim_uint16 theLocationRecordLength;

   ossim_uint32  theComponentAggregateLength;

   /*!
    * Since there are not very many components within an Rpf file
    * it would be overkill to sort these components into an stl
    * map.  We will just use an array (stl vector) and do linear searches
    * for components when we need to.
    */
   vector<ossimRpfComponentLocationRecord> theComponentLocationList;
};

#endif
