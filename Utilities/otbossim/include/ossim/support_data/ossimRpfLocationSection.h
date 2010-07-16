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
// $Id: ossimRpfLocationSection.h 16997 2010-04-12 18:53:48Z dburken $
#ifndef ossimRpfLocationSection_HEADER
#define ossimRpfLocationSection_HEADER

#include <iosfwd>
#include <vector>

#include <ossim/base/ossimConstants.h>
#include <ossim/support_data/ossimRpfConstants.h>
#include <ossim/base/ossimErrorCodes.h>

//  10 byte record
class ossimRpfComponentLocationRecord
{
public:
   friend std::ostream& operator<<(
      std::ostream& out, const ossimRpfComponentLocationRecord& data);

   /** default constructor */
   ossimRpfComponentLocationRecord();

   /** copy constructor */
   ossimRpfComponentLocationRecord(const ossimRpfComponentLocationRecord& record);

   /** assignment operator */
   const ossimRpfComponentLocationRecord& operator=(const ossimRpfComponentLocationRecord& rhs);
   
   /**
    * @brief print method that outputs a key/value type format adding prefix
    * to keys.
    * @param out String to output to.
    * @param prefix This will be prepended to key.
    * e.g. Where prefix = "nitf." and key is "file_name" key becomes:
    * "nitf.file_name:"
    * @return output stream.
    */
   std::ostream& print(std::ostream& out,
                       const std::string& prefix=std::string()) const;

   ossimErrorCode parseStream(std::istream& in, ossimByteOrder endianOrder);

   /**
    * @brief Write method.
    *
    * @param out Stream to write to.
    */
   void writeStream(std::ostream& out);

   /*!
    * The component is a 2-Byte unsigned value which
    * identitfies a level-2 component.  The table can be
    * found in MIL-STD-2411-1 in section 5.1.1 and in ossimRpfConstants.h.
    */
   ossim_uint16 m_componentId;

   /*!
    * Is a 4 byte unsigned integer indicating the length in
    * bytes of the component.
    */
   ossim_uint32  m_componentLength;

   /*!
    * Is a 4 byte value defining the absolute address (byte number)
    * of the first byte of the component measured
    * from the beginning of the RPF Frame file.
    */ 
   ossim_uint32  m_componentLocation;
};

class ossimRpfLocationSection
{
public:
   friend std::ostream& operator <<(std::ostream& out,
                                    const ossimRpfLocationSection &data);
   ossimRpfLocationSection();
   virtual ~ossimRpfLocationSection(){}

   virtual ossimErrorCode parseStream(std::istream& in,
                                      ossimByteOrder endianOrder);

   /**
    * @brief Write method.
    *
    * @param out Stream to write to.
    */
   void writeStream(std::ostream& out);

   /**
    * @brief print method that outputs a key/value type format adding prefix
    * to keys.
    * @param out String to output to.
    * @param prefix This will be prepended to key.
    * e.g. Where prefix = "nitf." and key is "file_name" key becomes:
    * "nitf.file_name:"
    * @return output stream.
    */
   std::ostream& print(std::ostream& out,
                       const std::string& prefix=std::string()) const;

   /*!
    * Will let you know if a component exists within the location section.
    * these component Ids' are defined in the MIL-STD-2411-1 standards doc.
    * the ossimRpfComponentId is in ossimRpfConstants.h
    */
   bool hasComponent(ossimRpfComponentId componentId)const;

   bool getComponent(ossimRpfComponentId componentId,
                     ossimRpfComponentLocationRecord& result)const;

   /**
    * @brief Method to add a component location record.
    *
    * This will push the record onto the back of theComponentLocationList array.
    *
    * @param record Record to add to array.
    */
   void addComponentRecord(const ossimRpfComponentLocationRecord& record);

   void setLocationSectionLength(ossim_uint16 length);
   void setLocationTableOffset(ossim_uint32 offset);
   void setNumberOfComponentLocationRecords(ossim_uint16 count);
   void setLocationRecordLength(ossim_uint16 length);
   void setComponentAggregateLength(ossim_uint32 length);

   /** @brief Clears records and fields. Public interface to clearFields. */
   void clearFields();

   /** Brief Direct access to the list of records. */
   std::vector<ossimRpfComponentLocationRecord>& getLocationRecordList();
   
private:

   /*!
    * This is a 2 byte unsigned short >= 34 indicatin
    * the length in bytes of the entire location section
    */
   ossim_uint16 m_locationSectionLength;

   ossim_uint32 m_locationTableOffset;

   ossim_uint16 m_numberOfComponentLocationRecords;

   ossim_uint16 m_locationRecordLength;

   ossim_uint32 m_componentAggregateLength;

   /*!
    * Since there are not very many components within an Rpf file
    * it would be overkill to sort these components into an stl
    * map.  We will just use an array (stl vector) and do linear searches
    * for components when we need to.
    */
   std::vector<ossimRpfComponentLocationRecord> m_componentLocationList;
};

#endif
