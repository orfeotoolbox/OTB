//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
// 
// Description: Rpf support class.
// 
//********************************************************************
// $Id: ossimRpfFrameFileIndexSubsection.h 16997 2010-04-12 18:53:48Z dburken $
#ifndef ossimRpfFrameFileIndexSubsection_HEADER
#define ossimRpfFrameFileIndexSubsection_HEADER

#include <iosfwd>
#include <vector>

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimReferenced.h>
#include <ossim/support_data/ossimRpfFrameFileIndexRecord.h>
#include <ossim/support_data/ossimRpfPathnameRecord.h>

class ossimFilename;

class ossimRpfFrameFileIndexSubsection : public ossimReferenced
{
public:
   friend std::ostream& operator <<(std::ostream& out,
                                    const ossimRpfFrameFileIndexSubsection& data);

   /** default constructor */
   ossimRpfFrameFileIndexSubsection();

   /** copy constructor */
   ossimRpfFrameFileIndexSubsection(const ossimRpfFrameFileIndexSubsection& obj);

   /** assignment operator */
   const ossimRpfFrameFileIndexSubsection& operator=(const ossimRpfFrameFileIndexSubsection& rhs);

   virtual ~ossimRpfFrameFileIndexSubsection();

   ossimErrorCode parseStream(std::istream &in, ossimByteOrder byteOrder);

   /**
    * @brief Write method.
    *
    * @param out Stream to write to.
    */
   void writeStream(std::ostream& out);
   
   void clearFields();

   std::ostream& print(std::ostream& out) const;
   
   void setNumberOfFileIndexRecords(ossim_uint32 numberOfIndexRecords);
   void setNumberOfPathnames(ossim_uint32 numberOfPathnames);
   
   const std::vector<ossimRpfFrameFileIndexRecord>& getIndexTable() const;
   const std::vector<ossimRpfPathnameRecord>& getPathnameTable() const;

   /**
    * @brief Gets the record matching file.
    *
    * @param file In the form of: 003H1U1B.I21
    *
    * @param record The record to initialize.
    *
    * @return true if matching record found, false if not.
    */
   bool getFrameFileIndexRecordFromFile(const ossimFilename& file,
                                        ossimRpfFrameFileIndexRecord& record) const;
   
private:
   std::vector<ossimRpfFrameFileIndexRecord> m_indexTable;

   std::vector<ossimRpfPathnameRecord> m_pathnameTable;
};

#endif
