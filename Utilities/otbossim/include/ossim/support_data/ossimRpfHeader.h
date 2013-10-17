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
// $Id: ossimRpfHeader.h 22013 2012-12-19 17:37:20Z dburken $
#ifndef ossimRpfHeader_HEADER
#define ossimRpfHeader_HEADER 1

#include <iosfwd>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/support_data/ossimNitfRegisteredTag.h>
#include <ossim/support_data/ossimRpfConstants.h>

class ossimRpfLocationSection;
class ossimRpfCoverageSection;
class ossimRpfBoundaryRectTable;
class ossimRpfBoundaryRectSectionSubheader;
class ossimRpfFrameFileIndexSectionSubheader;
class ossimRpfFrameFileIndexSubsection;
class ossimRpfColorTableIndexSectionSubheader;
class ossimRpfImageDescriptionSubheader;
class ossimRpfMaskSubsection;
class ossimRpfAttributeSectionSubheader;
class ossimRpfImageDisplayParameterSubheader;
class ossimRpfCompressionSection;
class ossimRpfCompressionSectionSubheader;
class ossimRpfColorGrayscaleSubheader;

class ossimRpfHeader : public ossimNitfRegisteredTag
{
public:

   /** default constructor */
   ossimRpfHeader();

   /** copy constructor */
   ossimRpfHeader(const ossimRpfHeader& obj);

   /** assignment operator */
   const ossimRpfHeader& operator=(const ossimRpfHeader& rhs);

   virtual ~ossimRpfHeader();

   /**
    * @brief Parse method.
    *
    * @param in Stream to parse.
    */
   virtual void parseStream(std::istream& in);

   /**
    * @brief Write method.
    *
    * @param out Stream to write to.
    */
   virtual void writeStream(std::ostream& out);
   
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

   virtual ossimByteOrder getByteOrder()const;
   
   const ossimRpfLocationSection* getLocationSection() const;

   ossimRpfLocationSection* getLocationSection();

   bool hasComponent(ossimRpfComponentId componentId)const;
   ossimString getSecurityClassification()const;
   
   /*!
    * This will return a new coverage section.  It will return NULL if
    * one does not exist.
    */
   ossimRpfCoverageSection*
      getNewCoverageSection(const ossimFilename& file)const;
   ossimRpfCoverageSection*
      getNewCoverageSection(std::istream& in)const;

   ossimRpfColorGrayscaleSubheader*
      getNewColorGrayscaleSubheader(std::istream& in)const;
   ossimRpfCompressionSection*
      getNewCompressionSection(std::istream& in)const;
   ossimRpfCompressionSectionSubheader*
      getNewCompressionSectionSubheader(std::istream& in)const;
   ossimRpfImageDisplayParameterSubheader*
      getNewImageDisplayParameterSubheader(std::istream& in)const;
   ossimRpfAttributeSectionSubheader*
      getNewAttributeSectionSubheader(std::istream& in)const;
   ossimRpfImageDescriptionSubheader*
      getNewImageDescriptionSubheader(std::istream& in)const;
   ossimRpfMaskSubsection*
      getNewMaskSubsection(std::istream& in)const;
   ossimRpfBoundaryRectSectionSubheader*
      getNewBoundaryRectSectSubheader(std::istream& in)const;
   ossimRpfBoundaryRectTable*
      getNewBoundaryRectTable(std::istream& in)const;
   ossimRpfFrameFileIndexSectionSubheader*
      getNewFrameFileIndexSectionSubheader(std::istream& in)const;
   ossimRpfFrameFileIndexSubsection*
      getNewFileIndexSubsection(std::istream& in)const;
   ossimRpfColorTableIndexSectionSubheader*
      getNewColorTableIndexSectionSubheader(std::istream& in)const;

   ossimRpfCompressionSection*
      getNewCompressionSection(const ossimFilename& file)const;
   ossimRpfBoundaryRectSectionSubheader*
      getNewBoundaryRectSectSubheader(const ossimFilename& file)const;
   ossimRpfBoundaryRectTable*
      getNewBoundaryRectTable(const ossimFilename& file)const;
   ossimRpfFrameFileIndexSectionSubheader*
      getNewFrameFileIndexSectionSubheader(const ossimFilename& file)const;
   ossimRpfFrameFileIndexSubsection*
      getNewFileIndexSubsection(const ossimFilename& file)const;
   ossimRpfColorTableIndexSectionSubheader*
      getNewColorTableIndexSectionSubheader(const ossimFilename& file)const;

   ossimString getDate()const;

   /** @brief returns the byte position of the location section. */
   ossim_uint32 getLocationSectionLocation() const;
   
   void setFilename(const ossimString& file);
   void setNewRepUpIndicator(const ossimString& s);
   void setGovSpecNumber(const ossimString& s);
   void setGovSpecDate(const ossimString& s);
   void setSecurityClassification(const ossimString& s);
   void setCountryCode(const ossimString& s);
   void setSecurityReleaseMarking(const ossimString& s);
   void setLocationSectionPos(std::streamoff off);

   /*!
    * Method to the load (recreate) the state of the object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);

private:

   /** 0x00 = big, 0xff = little */
   ossim_uint8  m_littleBigEndianIndicator; // 1 byte bool
   ossim_uint16 m_headerSectionLength;      // 2 bytes uint
   char         m_fileName[13];             // 12 byte ascii
   ossim_uint8  m_newRepUpIndicator;        // 1 byte unsigned int
   char         m_govSpecNumber[16];        // 15 byte ascii
   char         m_govSpecDate[9];           // 8 byte ascii
   /**
    * 1 byte field that can have the values
    *
    * U   Unclassified
    * R   Restricted
    * C   Confidential
    * S   Secret
    * T   Top Secret
    */ 
   char         m_securityClassification[2]; // 1 byte ascii
   char         m_countryCode[3];            // 2 byte ascii
   char         m_securityReleaseMarking[3]; // 2 byte ascii
   ossim_uint32 m_locSectionLoc;             // 4 byte unsigned int
   
   ossimRpfLocationSection* m_locationSection;
};

#endif
 
