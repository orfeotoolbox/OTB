//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfLocationSection.h 9967 2006-11-29 02:01:23Z gpotts $
#ifndef ossimRpfLocationSection_HEADER
#define ossimRpfLocationSection_HEADER
#include <iostream>
#include <vector>
#include <iterator>
using namespace std;

#include <ossim/base/ossimConstants.h>
#include <ossim/support_data/ossimRpfConstants.h>
#include <ossim/base/ossimErrorContext.h>

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
