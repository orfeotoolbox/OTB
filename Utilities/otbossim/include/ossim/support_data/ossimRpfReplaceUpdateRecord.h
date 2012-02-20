//----------------------------------------------------------------------------
//
// File:     ossimRpfReplaceUpdateRecord.h
// 
// License:  See top level LICENSE.txt file.
// 
// Author:   David Burken
//
// Description: See class description.
// 
//----------------------------------------------------------------------------
// $Id: ossimRpfReplaceUpdateRecord.h 20324 2011-12-06 22:25:23Z dburken $

#ifndef ossimRpfReplaceUpdateRecord_HEADER
#define ossimRpfReplaceUpdateRecord_HEADER 1

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorCodes.h>
#include <iosfwd>

/**
 * @class ossimRpfReplaceUpdateRecord
 * @brief RPF replace/update section subheader record.
 * See MIL-STD-2411 for detailed information.
 */
class ossimRpfReplaceUpdateRecord
{
public:

   /** @brief Convenience output operator. */
   friend std::ostream& operator <<(std::ostream& out,
                                    const ossimRpfReplaceUpdateRecord& data);

   /** @brief default constructor */
   ossimRpfReplaceUpdateRecord();

   /** @brief copy constructor */
   ossimRpfReplaceUpdateRecord(const ossimRpfReplaceUpdateRecord& obj);

   /** @brief assignment operator */
   const ossimRpfReplaceUpdateRecord& operator=(const ossimRpfReplaceUpdateRecord& rhs);

   /**
    * @brief Method to parse the record.
    * Note no byte swapping required for this record.
    * @param in Stream sitting at record.
    */
   ossimErrorCode parseStream(std::istream& in);

   /**
    * @brief Write method.
    * Note no byte swapping required for this record.
    * @param out Stream to write to.
    */
   void writeStream(std::ostream& out);

   /** @brief Clears fields. */
   void clearFields();

  /**
    * @brief print method that outputs a key/value type format adding prefix
    * to keys.
    * @param out Stream to output to.
    * @param prefix This will be prepended to key.
    * @param recordNumber This is added to key.  See below.
    * e.g. Where prefix = "nitf.rpf.",  recordNumber = 0 and key is "new_file"
    * key becomes: "nitf.rpf.replace_update_record0.new_file:"
    * @return output stream.
    */
 
   std::ostream& print( std::ostream& out,
                        const std::string& prefix=std::string(),
                        ossim_uint32 recordNumber=0) const;

   /**
    * @brief Gets new file field.
    * @param file Set by this.
    */
   void getNewFilename(std::string& file) const;

   /**
    * @brief Gets old file field.
    * @param file Set by this.
    */
   void getOldFilename(std::string& file) const;

   /** @return The update status field. */
   ossim_uint8 getUpdateStatus() const;

   /**
    * @brief Sets old file name 12 byte field.
    * @param file
    */
   void setNewFilename(const std::string& file);

   /**
    * @brief Sets new file name 12 byte field.
    * @param file
    */
   void setOldFilename(const std::string& file);

   /** @return Sets the update status field. */
   void setUpdateStatus(ossim_uint8 status);
   
private:
   
   /** This is a 12 byte asci field. */
   char m_newFile[13];

   /** This is a 12 byte asci field. */
   char m_oldFile[13];

   /** This is a 1 byte unsigned integer. */
   ossim_uint8 m_updateStatus;
};

#endif /* #ifndef ossimRpfReplaceUpdateRecord_HEADER */
