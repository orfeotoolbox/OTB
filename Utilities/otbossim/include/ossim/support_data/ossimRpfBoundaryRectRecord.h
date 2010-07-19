//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
// 
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfBoundaryRectRecord.h 16997 2010-04-12 18:53:48Z dburken $
#ifndef ossimRpfBoundaryRectRecord_HEADER
#define ossimRpfBoundaryRectRecord_HEADER

#include <iosfwd>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorCodes.h>
#include <ossim/base/ossimString.h>
#include <ossim/support_data/ossimRpfCoverageSection.h>

class ossimRpfBoundaryRectRecord
{
public:
   friend std::ostream& operator <<(std::ostream& out, const ossimRpfBoundaryRectRecord& data);

   /** default constructor */
   ossimRpfBoundaryRectRecord();

   /** copy constructor */
   ossimRpfBoundaryRectRecord(const ossimRpfBoundaryRectRecord& ojb);

   /** assignment operator */
   const ossimRpfBoundaryRectRecord& operator=(const ossimRpfBoundaryRectRecord& rhs);

   /** destructor */
   ~ossimRpfBoundaryRectRecord();
   
   ossimErrorCode parseStream(std::istream& in, ossimByteOrder byteOrder);

   /**
    * @brief Write method.
    *
    * @param out Stream to write to.
    */
   void writeStream(std::ostream& out);   
   
   void clearFields();

   ossimString getProductType()const{return ossimString(m_productDataType);}
   ossimString getScale()const{return ossimString(m_scale);}
   ossimString getCompressionRatio()const{return ossimString(m_compressionRatio);}
   
   ossim_uint32 getNumberOfFramesVertical()const{ return m_numberOfFramesNorthSouth;}
   ossim_uint32 getNumberOfFramesHorizontal()const{ return m_numberOfFramesEastWest;}

   const ossimRpfCoverageSection& getCoverage()const{return m_coverage;}

   void setCoverage(const ossimRpfCoverageSection& coverage);
   
   char getZone()const{return m_zone;}

   /**
    * @brief prints to out.
    * @param out Stream to print to.
    * @param prefix If not empty will be prepended onto the keyword.
    */
   std::ostream& print(std::ostream& out, ossimString prefix) const;

private:
   /*!
    * a five byte asci field
    */
   char m_productDataType[6];

   /*!
    * five byte asci field
    */
   char m_compressionRatio[6];

   /*!
    * 12 byte asci field.
    */
   char m_scale[13];

   /*!
    *
    */
   char m_zone;

   /*!
    * is a 5 byte asci field.
    */
   char m_producer[6];

   ossimRpfCoverageSection m_coverage;

   /*!
    * Four byte fields.
    */
   ossim_uint32 m_numberOfFramesNorthSouth;

   /*!
    * Four byte fields.
    */
   ossim_uint32 m_numberOfFramesEastWest;   
};

#endif
