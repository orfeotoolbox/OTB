//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
// 
// Author: Garrett Potts
// 
// Description: This class give the capability to access tiles from an
//              rpf file.
//
//********************************************************************
// $Id: ossimRpfFrame.h 20324 2011-12-06 22:25:23Z dburken $

#ifndef ossimRpfFrame_HEADER
#define ossimRpfFrame_HEADER 1

#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimErrorCodes.h>
#include <ossim/support_data/ossimNitfFile.h>
#include <ossim/support_data/ossimRpfColorGrayscaleTable.h>
#include <ossim/support_data/ossimRpfReplaceUpdateTable.h>
#include <iosfwd>
#include <vector>

class ossimRpfHeader;
class ossimRpfAttributes;
class ossimRpfCoverageSection;
class ossimRpfImageDescriptionSubheader;
class ossimRpfImageDisplayParameterSubheader;
class ossimRpfMaskSubheader;
class ossimRpfCompressionSection;
class ossimRpfColorGrayscaleSubheader;
class ossimRpfColorConverterSubsection;

class OSSIM_DLL ossimRpfFrame : public ossimReferenced
{
public:
   friend std::ostream& operator <<(std::ostream& out,
                                    const ossimRpfFrame& data);
   ossimRpfFrame();
   ~ossimRpfFrame();
   
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

   ossimErrorCode parseFile(const ossimFilename& filename,
                            bool minimalParse = false);
   
   const ossimRpfHeader* getRpfHeader()const{return theHeader;}

   bool hasSubframeMaskTable()const;

   const vector< vector<ossim_uint32> >& getSubFrameMask(
      ossim_uint32 spectralGroup)
   {
      return theSubframeMaskTable[spectralGroup];
   }
   
   bool fillSubFrameBuffer(ossim_uint8* buffer,
                           ossim_uint32 spectralGroup,
                           ossim_uint32 row,
                           ossim_uint32 col)const;
   
   const ossimRpfCompressionSection* getCompressionSection()const
   {
      return theCompressionSection;
   }
   const vector<ossimRpfColorGrayscaleTable>& getColorGrayscaleTable()const
   {
      return theColorGrayscaleTable;
   }
   const ossimRpfColorConverterSubsection* getColorConverterSubsection()const
   {
      return theColorConverterSubsection;
   }
   const ossimRpfAttributes* getAttributes()const
   {
      return theAttributes;
   }
   const ossimNitfFile* getNitfFile()const
   {
      return theNitfFile.get();
   }

   /**
    * @return The RPF replace / update table.  The ossimRefPtr can have a null
    * internal pointer if record was not found. Callers should check
    * ossimRefPtr<ossimRpfReplaceUpdateTable>::valid() before using pointer.
    */
   ossimRefPtr<ossimRpfReplaceUpdateTable> getRpfReplaceUpdateTable() const;

private:
   void clearFields();
   void deleteAll();
   ossimErrorCode populateCoverageSection(std::istream& in);
   ossimErrorCode populateCompressionSection(std::istream& in);
   ossimErrorCode populateImageSection(std::istream& in);
   ossimErrorCode populateAttributeSection(std::istream& in);
   ossimErrorCode populateColorGrayscaleSection(std::istream& in);
   ossimErrorCode populateMasks(std::istream& in);
   ossimErrorCode populateReplaceUpdateTable(std::istream& in);

   /*!
    * The header will be instantiated during the opening of the
    * frame file.
    */
   ossimRpfHeader *theHeader;

   /*!
    * The filename is set if its a successful open.
    */
   ossimFilename   theFilename;

   /*!
    * This will hold the coverage.
    */
   ossimRpfCoverageSection* theCoverage;

   /*!
    * This will hold the attributes of this frame.
    */
   ossimRpfAttributes *theAttributes;

   /*!
    * This is the subheader for the image.  It will have
    * some general information about the image.
    */
   ossimRpfImageDescriptionSubheader* theImageDescriptionSubheader;

   /*!
    * This is the mask subheader.
    */
   ossimRpfMaskSubheader* theMaskSubheader;
   
   /*!
    * This is the subheader for the display parameters
    * for this frame.
    */
   ossimRpfImageDisplayParameterSubheader* theImageDisplayParameterSubheader;

   /*!
    * Will hold a pointer to the compression information section.
    */
   ossimRpfCompressionSection*             theCompressionSection;
   
   /*!
    * If present, it will hold the color grayscale subheader.
    */
   ossimRpfColorGrayscaleSubheader*        theColorGrayscaleSubheader;

   /*!
    * 
    */
   ossimRpfColorConverterSubsection*       theColorConverterSubsection;

   /*!
    * 
    */
   vector<ossimRpfColorGrayscaleTable>     theColorGrayscaleTable;

   /*!
    * 
    */
   ossimRefPtr<ossimNitfFile>              theNitfFile;

   /*!
    * We have a 3-D array.  For the most part the numberof spectral groups
    * should be one.  The indexes are as follows:
    *
    * theSubframeMasKTable[spectralGroup][row][col]
    *
    *  It will hold RPF_NULL(0xffffffff) if the subframe does not exist
    *  and it will hold an offset value from the start of the
    * [spatial data section] to the first byte of the subframe table.
    */
   vector< vector< vector< ossim_uint32> > > theSubframeMaskTable;

   /*!
    * We have a 3-D array.  For the most part the numberof spectral groups
    * should be one.  The indexes are as follows:
    *
    * theSubframeTransparencyMaskTable[spectralGroup][row][col]
    *
    *  It will hold RPF_NULL(0xffffffff) if the subframe does not exist
    *  and it will hold an offset value from the start of the
    * [spatial data section] to the first byte of the subframe table.
    */
   vector< vector< vector< ossim_uint32> > > theSubframeTransparencyMaskTable;

   /** Holds table of "replace / update" records if present. */
   ossimRefPtr<ossimRpfReplaceUpdateTable> theReplaceUpdateTable;

};

#endif
