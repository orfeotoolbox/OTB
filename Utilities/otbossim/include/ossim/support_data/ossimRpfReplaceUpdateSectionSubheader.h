//----------------------------------------------------------------------------
//
// File:     ossimRpfReplaceUpdateSectionSubheader.h
// 
// License:  See top level LICENSE.txt file.
// 
// Author:   David Burken
//
// Description: See class description.
// 
//----------------------------------------------------------------------------
// $Id: ossimRpfReplaceUpdateSectionSubheader.h 20324 2011-12-06 22:25:23Z dburken $

#ifndef ossimRpfReplaceUpdateSubheader_Header
#define ossimRpfReplaceUpdateSubheader_Header 1

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorCodes.h>
#include <ossim/base/ossimReferenced.h>
#include <iosfwd>

/**
 * @class ossimRpfReplaceUpdateSectionSubheader
 * @brief RPF replace/update section subheader record.
 * See MIL-STD-2411 for detailed information.
 */
class ossimRpfReplaceUpdateSectionSubheader : public ossimReferenced
{
public:

   /** @brief Convenience output operator. */
   friend std::ostream& operator<<(std::ostream& out,
                                   const ossimRpfReplaceUpdateSectionSubheader& data);

   /** @brief default constructor */
   ossimRpfReplaceUpdateSectionSubheader();

   /** @brief copy constructor */
   ossimRpfReplaceUpdateSectionSubheader(const ossimRpfReplaceUpdateSectionSubheader& obj);

   /** @brief assignment operator */
   const ossimRpfReplaceUpdateSectionSubheader& operator=(
      const ossimRpfReplaceUpdateSectionSubheader& rhs);

   /** @brief virtual destructor */
   virtual ~ossimRpfReplaceUpdateSectionSubheader();

   /**
    * @brief Method to parse the record.
    * @param in Stream sitting at record.
    * @param byteOrder Byte order of system.
    */   
   ossimErrorCode parseStream(std::istream& in, ossimByteOrder byteOrder);

   /**
    * @brief Write method.
    *
    * Note always writes out in big endian at this point.
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
   std::ostream& print(std::ostream& out) const;

   /** @return Offset to first record. */
   ossim_uint32 getOffset() const;

   /** @return Number of records. */
   ossim_uint16 getNumberOfRecords() const;

   /** @return Length of one record. */
   ossim_uint16 getRecordLength() const;

   /**
    * @brief Sets number of records.
    * @param count
    */
   void setNumberOfRecords(ossim_uint16 count);

   void setRecordLength(ossim_uint16 length);
   void clearFields();
   
private:
   ossim_uint32  m_tableOffset;
   ossim_uint16  m_numberOfRecords;
   ossim_uint16  m_recordLength;
};



#endif /* #ifndef ossimRpfReplaceUpdateSubheader_Header */
