//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfBoundaryRectSectionSubheader.h 16997 2010-04-12 18:53:48Z dburken $
#ifndef ossimRpfBoundaryRectSectionSubheader_HEADER
#define ossimRpfBoundaryRectSectionSubheader_HEADER

#include <iosfwd>

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorCodes.h>
#include <ossim/base/ossimReferenced.h>

class ossimRpfBoundaryRectSectionSubheader : public ossimReferenced
{
public:
   friend std::ostream& operator <<(std::ostream& out,
                                    const ossimRpfBoundaryRectSectionSubheader &data);

   /** default constructor */
   ossimRpfBoundaryRectSectionSubheader();

   /** copy constructor */
   ossimRpfBoundaryRectSectionSubheader(const ossimRpfBoundaryRectSectionSubheader& obj);

   /** assignment operator */
   const ossimRpfBoundaryRectSectionSubheader& operator=(
      const ossimRpfBoundaryRectSectionSubheader& rhs);

   virtual ~ossimRpfBoundaryRectSectionSubheader();

   ossimErrorCode parseStream(std::istream& in, ossimByteOrder);

   /**
    * @brief Write method.
    *
    * @param out Stream to write to.
    */
   void writeStream(std::ostream& out);

   /**
    * @brief print method
    * @param out String to output to.
    * @return output stream.
    */
   std::ostream& print(std::ostream& out) const;

   ossim_uint16 getNumberOfEntries() const;
   ossim_uint32 getTableOffset() const;
   ossim_uint16 getLengthOfEachEntry() const;

   void setNumberOfEntries(ossim_uint16 entries);
   void setTableOffset(ossim_uint32 offset);
   void setLengthOfEachEntry(ossim_uint16 length);
   
   void clearFields();
   
private:
   
   ossim_uint32 m_rectangleTableOffset;
   ossim_uint16 m_numberOfEntries;
   ossim_uint16 m_lengthOfEachEntry;
};

#endif
