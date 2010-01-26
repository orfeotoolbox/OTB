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
// $Id: ossimRpfHeader.h 14241 2009-04-07 19:59:23Z dburken $
#ifndef ossimRpfHeader_HEADER
#define ossimRpfHeader_HEADER

#include <iosfwd>
#include <ossim/base/ossimConstants.h>
#include <ossim/support_data/ossimRpfConstants.h>
#include <ossim/base/ossimFilename.h>

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
class ossimRpfMaskSubsection;
class ossimRpfImageDisplayParameterSubheader;
class ossimRpfColorGrayscaleSubheader;
class ossimRpfHeader
{
public:
   friend std::ostream& operator<<(std::ostream& out,
                                   const ossimRpfHeader &data);
   
   ossimRpfHeader();
   virtual ~ossimRpfHeader();
   ossim_int32 parseStream(std::istream& in);
   
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
   
   virtual ossimByteOrder getByteOrder()const
   {
      return ((theLittleBigEndianIndicator==0xFF)?OSSIM_LITTLE_ENDIAN:OSSIM_BIG_ENDIAN);
   }
   const ossimRpfLocationSection& getLocationSection()const{return *theLocationSection;}

   bool hasComponent(ossimRpfComponentId componentId)const;
   ossimString getSecurityClassification()const;
   
   /*!
    * This will return a new coverage section.  It will return NULL if
    * one does not exist.
    */
   ossimRpfCoverageSection* getNewCoverageSection(const ossimFilename& file)const;
   ossimRpfCoverageSection* getNewCoverageSection(std::istream& in)const;

   ossimRpfColorGrayscaleSubheader*     getNewColorGrayscaleSubheader(std::istream& in)const;
   ossimRpfCompressionSection*          getNewCompressionSection(std::istream& in)const;
   ossimRpfCompressionSectionSubheader* getNewCompressionSectionSubheader(std::istream& in)const;
   ossimRpfImageDisplayParameterSubheader* getNewImageDisplayParameterSubheader(std::istream& in)const;
   ossimRpfAttributeSectionSubheader* getNewAttributeSectionSubheader(std::istream& in)const;
   ossimRpfImageDescriptionSubheader* getNewImageDescriptionSubheader(std::istream& in)const;
   ossimRpfMaskSubsection* getNewMaskSubsection(std::istream& in)const;
   ossimRpfBoundaryRectSectionSubheader* getNewBoundaryRectSectSubheader(std::istream& in)const;
   ossimRpfBoundaryRectTable* getNewBoundaryRectTable(std::istream& in)const;
   ossimRpfFrameFileIndexSectionSubheader* getNewFrameFileIndexSectionSubheader(std::istream& in)const;
   ossimRpfFrameFileIndexSubsection* getNewFileIndexSubsection(std::istream& in)const;
   ossimRpfColorTableIndexSectionSubheader* getNewColorTableIndexSectionSubheader(std::istream& in)const;

   ossimRpfCompressionSection*          getNewCompressionSection(const ossimFilename& file)const;
   ossimRpfBoundaryRectSectionSubheader* getNewBoundaryRectSectSubheader(const ossimFilename& file)const;
   ossimRpfBoundaryRectTable* getNewBoundaryRectTable(const ossimFilename& file)const;
   ossimRpfFrameFileIndexSectionSubheader* getNewFrameFileIndexSectionSubheader(const ossimFilename& file)const;
   ossimRpfFrameFileIndexSubsection* getNewFileIndexSubsection(const ossimFilename& file)const;
   ossimRpfColorTableIndexSectionSubheader* getNewColorTableIndexSectionSubheader(const ossimFilename& file)const;

   ossimString getDate()const;
private:
   ossim_uint8  theLittleBigEndianIndicator; // 1 byte bool
   ossim_uint16 theHeaderSectionLength;     // 2 bytes uint
   char         theFileName[13];            // 12 byte ascii
   unsigned char theNewRepUpIndicator;       // 1 byte unsigned int
   char         theGovSpecNumber[16];       // 15 byte ascii
   char         theGovSpecDate[9];          // 8 byte ascii
   /**
    * 1 byte field that can have the values
    *
    * U   Unclassified
    * R   Restricted
    * C   Confidential
    * S   Secret
    * T   Top Secret
    */ 
   char         theSecurityClassification[2];// 1 byte ascii
   char         theCountryCode[3];          // 2 byte ascii
   char         theSecurityReleaseMarking[3]; // 2 byte ascii
   ossim_uint32 theLocSectionLoc;           // 4 byte unsigned int
   
   ossimRpfLocationSection *theLocationSection;
};

#endif
 
