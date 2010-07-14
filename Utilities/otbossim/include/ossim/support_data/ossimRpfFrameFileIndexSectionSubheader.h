//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfFrameFileIndexSectionSubheader.h 16997 2010-04-12 18:53:48Z dburken $
#ifndef ossimRpfFrameFileIndexSectionSubheader_HEADER
#define ossimRpfFrameFileIndexSectionSubheader_HEADER

#include <iosfwd>

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorCodes.h>
#include <ossim/base/ossimReferenced.h>

class ossimRpfFrameFileIndexSectionSubheader : public ossimReferenced
{
public:
   friend std::ostream& operator<<(std::ostream& out,
                                   const ossimRpfFrameFileIndexSectionSubheader& data);

   /** default constructor */
   ossimRpfFrameFileIndexSectionSubheader();

   /** copy constructor */
   ossimRpfFrameFileIndexSectionSubheader(const ossimRpfFrameFileIndexSectionSubheader& obj);

   /** assignment operator */
   const ossimRpfFrameFileIndexSectionSubheader& operator=(
      const ossimRpfFrameFileIndexSectionSubheader& rhs);

   virtual ~ossimRpfFrameFileIndexSectionSubheader();

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

   ossim_uint32 getOffset() const;
   ossim_uint32 getNumberOfIndexRecords() const;
   ossim_uint16 getNumberOfPathnameRecords() const;
   ossim_uint16 getIndexRecordLength() const;

   void setNumberOfIndexRecords(ossim_uint32 count);
   void setNumberOfPathnameRecords(ossim_uint16 count);
   void setIndexRecordLength(ossim_uint16 length);

   void clearFields();
private:
   char          m_highestSecurityClassification;
   ossim_uint32  m_indexTableOffset;
   ossim_uint32  m_numberOfIndexRecords;
   ossim_uint16  m_numberOfPathnameRecords;
   ossim_uint16  m_indexRecordLength;
};

#endif
