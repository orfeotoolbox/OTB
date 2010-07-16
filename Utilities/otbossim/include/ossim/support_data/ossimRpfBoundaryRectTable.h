//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
// 
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfBoundaryRectTable.h 16997 2010-04-12 18:53:48Z dburken $
#ifndef ossimRpfBoundaryRectTable_HEADER
#define ossimRpfBoundaryRectTable_HEADER

#include <iosfwd>
#include <vector>

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorCodes.h>
#include <ossim/base/ossimReferenced.h>
#include <ossim/support_data/ossimRpfBoundaryRectRecord.h>

class ossimRpfBoundaryRectTable : public ossimReferenced
{
public:
   
   friend std::ostream& operator <<(std::ostream& out, const ossimRpfBoundaryRectTable& data);

   /** default constructor */
   ossimRpfBoundaryRectTable(ossim_uint32 numberOfEntries=0);

   /** copy constructor */
   ossimRpfBoundaryRectTable(const ossimRpfBoundaryRectTable& obj);

   /** assignment operator */
   const ossimRpfBoundaryRectTable& operator=(const ossimRpfBoundaryRectTable& rhs);
   
   virtual ~ossimRpfBoundaryRectTable();
   
   ossimErrorCode parseStream(std::istream& in, ossimByteOrder byteOrder);

   /**
    * @brief Write method.
    *
    * @param out Stream to write to.
    */
   void writeStream(std::ostream& out);   

   void setNumberOfEntries(ossim_uint32 numberOfEntries);

   ossim_uint32 getNumberOfEntries() const;

   /**
    * @brief Gets record for entry.
    * @param entry Zero base entry to get.
    * @param record Record to initialize.
    * @return true on success, false if entry doesn't exist.
    */
   bool getEntry(ossim_uint32 entry, ossimRpfBoundaryRectRecord& record) const;

   std::ostream& print(std::ostream& out)const;
   
private:
   std::vector<ossimRpfBoundaryRectRecord> m_table;
};
#endif
