//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfHeader.h 9967 2006-11-29 02:01:23Z gpotts $
#ifndef ossimRpfHeader_HEADER
#define ossimRpfHeader_HEADER
#include <iostream>
#include <fstream>
using namespace std;
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
   friend ostream &operator<<(ostream& out, const ossimRpfHeader &data);
   
   ossimRpfHeader();
   virtual ~ossimRpfHeader();
   ossim_int32 parseStream(istream& in);
   
   virtual void print(ostream &out)const;
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
   ossimRpfCoverageSection* getNewCoverageSection(istream& in)const;

   ossimRpfColorGrayscaleSubheader*     getNewColorGrayscaleSubheader(istream& in)const;
   ossimRpfCompressionSection*          getNewCompressionSection(istream& in)const;
   ossimRpfCompressionSectionSubheader* getNewCompressionSectionSubheader(istream& in)const;
   ossimRpfImageDisplayParameterSubheader* getNewImageDisplayParameterSubheader(istream& in)const;
   ossimRpfAttributeSectionSubheader* getNewAttributeSectionSubheader(istream& in)const;
   ossimRpfImageDescriptionSubheader* getNewImageDescriptionSubheader(istream& in)const;
   ossimRpfMaskSubsection* getNewMaskSubsection(istream& in)const;
   ossimRpfBoundaryRectSectionSubheader* getNewBoundaryRectSectSubheader(istream& in)const;
   ossimRpfBoundaryRectTable* getNewBoundaryRectTable(istream& in)const;
   ossimRpfFrameFileIndexSectionSubheader* getNewFrameFileIndexSectionSubheader(istream& in)const;
   ossimRpfFrameFileIndexSubsection* getNewFileIndexSubsection(istream& in)const;
   ossimRpfColorTableIndexSectionSubheader* getNewColorTableIndexSectionSubheader(istream& in)const;

   ossimRpfCompressionSection*          getNewCompressionSection(const ossimFilename& file)const;
   ossimRpfBoundaryRectSectionSubheader* getNewBoundaryRectSectSubheader(const ossimFilename& file)const;
   ossimRpfBoundaryRectTable* getNewBoundaryRectTable(const ossimFilename& file)const;
   ossimRpfFrameFileIndexSectionSubheader* getNewFrameFileIndexSectionSubheader(const ossimFilename& file)const;
   ossimRpfFrameFileIndexSubsection* getNewFileIndexSubsection(const ossimFilename& file)const;
   ossimRpfColorTableIndexSectionSubheader* getNewColorTableIndexSectionSubheader(const ossimFilename& file)const;

   ossimString getDate()const;
private:
   ossim_uint8    theLittleBigEndianIndicator; // 1 byte bool
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
 
