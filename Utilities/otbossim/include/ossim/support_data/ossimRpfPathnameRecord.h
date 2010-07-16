//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
// 
// Description: Rpf support class.
// 
//********************************************************************
// $Id: ossimRpfPathnameRecord.h 16997 2010-04-12 18:53:48Z dburken $
#ifndef ossimRpfPathnameRecord_HEADER
#define ossimRpfPathnameRecord_HEADER

#include <iosfwd>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorCodes.h>
#include <ossim/base/ossimString.h>

class ossimRpfPathnameRecord
{
public:

   friend std::ostream& operator <<(std::ostream& out,
                                    const ossimRpfPathnameRecord& data);
   /** default constructor */
   ossimRpfPathnameRecord();

   /** copy constructor */
   ossimRpfPathnameRecord(const ossimRpfPathnameRecord& obj);

   /** assignment operator */
   const ossimRpfPathnameRecord& operator=(const ossimRpfPathnameRecord& rhs);

   void clearFields();

   ossimErrorCode parseStream(std::istream& in, ossimByteOrder byteOrder);

   /**
    * @brief Write method.
    *
    * @param out Stream to write to.
    */
   void writeStream(std::ostream& out);

   std::ostream& print(std::ostream& out) const;

   ossimString getPathname()const;

   /**
    * @brief Set the pathname and length from string size.
    * @param path Path like: ./N03E030
    */
   void setPathName(const ossimString& path);
   
private:   
   ossim_uint16 m_length;
   ossimString  m_pathname;
};

#endif
