//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
// 
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfFrameFileIndexRecord.h 16997 2010-04-12 18:53:48Z dburken $
#ifndef ossimRpfFrameFileIndexRecord_HEADER
#define ossimRpfFrameFileIndexRecord_HEADER

#include <iosfwd>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorCodes.h>
#include <ossim/base/ossimFilename.h>

class ossimRpfFrameFileIndexRecord
{
public:
   friend std::ostream& operator <<(std::ostream& out,
                                    const ossimRpfFrameFileIndexRecord& data);

   /** default constructor */
   ossimRpfFrameFileIndexRecord();

   /** copy constructor */
   ossimRpfFrameFileIndexRecord(const ossimRpfFrameFileIndexRecord& obj);

   /** assignment operator */
   const ossimRpfFrameFileIndexRecord& operator=(const ossimRpfFrameFileIndexRecord& rhs);
   
   ossimErrorCode parseStream(std::istream& in, ossimByteOrder byteOrder);

   /**
    * @brief Write method.
    *
    * @param out Stream to write to.
    */
   void writeStream(std::ostream& out);
   
   void clearFields();
   
   std::ostream& print(std::ostream& out)const;
   
   ossim_uint16  getBoundaryRecNumber()const;
   ossim_uint16  getLocationRowNumber()const;
   ossim_uint16  getLocationColNumber()const;
   ossim_uint32  getPathnameRecordOffset()const;
   ossimFilename getFilename()const;

   /** @brief Sets the zero based entry number. */
   void setBoundaryRecNumber(ossim_uint16 entry);
   
   void setPathnameRecordOffset(ossim_uint32 offset);
   
private:
   ossim_uint16 m_boundaryRectRecordNumber;
   ossim_uint16 m_locationRowNumber;
   ossim_uint16 m_locationColumnNumber;
   ossim_uint32 m_pathnameRecordOffset;

   /*!
    * This is a 12 byte asci field.
    */
   char           m_filename[13];

   /*!
    * this is a 6 byte asci field.
    */
   char           m_geographicLocation[7];

   char           m_securityClassification;

   /*!
    * is a 2 byte field.
    */
   char           m_fileSecurityCountryCode[3];

   /*!
    * This is a 2 byte field.
    */
   char           m_fileSecurityReleaseMarking[3];
};

#endif
