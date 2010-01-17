//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  David Burken
//
// Description:
//
// Contains class declaration for CcfHead.
//
// Notes:
// - This is not a full blown CCF header class as it only captures the
//   data needed for loading.
// - Currently only version 6 is supported.
//
//*******************************************************************
//  $Id: ossimCcfHead.h 10265 2007-01-14 19:18:43Z dburken $
#ifndef ossimCcfHead_HEADER
#define ossimCcfHead_HEADER

#include <iosfwd>
#include <vector>

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorStatusInterface.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimIpt.h>

class ossimIrect;

//***************************************************************************
// CLASS:  ossimCcfHead
//***************************************************************************
class OSSIM_DLL ossimCcfHead : public ossimErrorStatusInterface
{
public:

   ossimCcfHead();
   
   ossimCcfHead(const char* ccf_file);

   ~ossimCcfHead ();

   /*!
    *  Reads the ccf header and initializes data members.  Returns true on
    *  successful read, false on error.
    */
   bool parseCcfHeader(const char* ccf_file);

   virtual std::ostream& print(std::ostream& out) const;

   ossim_uint32 numberOfReducedResSets() const { return theNumberOfRLevels; }

   ossim_uint32 numberOfLines(ossim_uint32 reduced_res_level) const;

   ossim_uint32 numberOfSamples(ossim_uint32 reduced_res_level) const;

   ossim_uint32 chunksInLineDir(ossim_uint32 reduced_res_level) const;
   
   ossim_uint32 chunksInSampleDir(ossim_uint32 reduced_res_level) const;

   std::streampos startOfData(ossim_uint32 reduced_res_level) const;

   /*!
    *  Returns the highest available reduced resolution level.
    *  Zero based.
    */
   ossim_uint32 highestReducedResSet() const
      { return !theNumberOfRLevels ? 0 : (theNumberOfRLevels - 1); }

   /*!
    *  Returns the zero based image rectangle for the reduced resolution
    *  level passed in.  Returns a rectangle of (0,0,0,0) if reduced_res_level
    *  is out of range.
    */
   ossimIrect imageRect(ossim_uint32 reduced_res_level) const;

   /*!
    *  Returns the ccf file name as a String.
    */
   ossimString imageFile() const { return theCcfFile; }

   /*!
    *  Returns the radiometry string.
    */
   ossimString radiometry() const { return theRadiometryString; }

   /*!
    *  Returns the number of bands.
    */
   ossim_uint32 numberOfBands() const { return theNumberOfBands; }

   /*!
    *  Returns the enumerated pixel type.
    */
   ossimScalarType pixelType() const { return thePixelType; }

   /*!
    *  Returns the bytes per chunk.
    */
   ossim_uint32 bytesPerChunk() const { return theBytesPerChunk; }

   /*!
    *  Returns the bytes per chip.
    */
   ossim_uint32 bytesPerChip() const { return theBytesPerChip; }

   /*!
    *  Returns the number of chips in sample direction of a chunk.
    */
   ossim_uint32 sampleChipsPerChunk() const { return theSampleChipsPerChunk; }

   /*!
    *  Returns the number of chips in line direction of a chunk.
    */
   ossim_uint32 lineChipsPerChunk() const { return theLineChipsPerChunk; }

   /*!
    *  Returns the samples per chip.
    */
   ossim_uint32 samplesPerChip() const { return theSamplesPerChip; }

   /*!
    *  Returns the lines per chip.
    */
   ossim_uint32 linesPerChip() const { return theLinesPerChip; }

   /*!
    *  Returns the number of pixels in a single band of a chip.
    *  Note:  This is not in bytes but in pixels.
    */
   ossim_uint32 chipSizePerBand() const
      { return theLinesPerChip * theSamplesPerChip; }

   const std::vector<ossimIpt>& getValidImageVertices()const
      {
         return theValidImageVertices;
      }
private:

   /*!
    *  Parses a version five header.
    */
   bool parseV5CcfHeader(std::istream& is);

   /*!
    *  Parses a version six header.
    */
   bool parseV6CcfHeader(std::istream& is);

   /*!
    *  Parses the radiometry string and initializes "theNumberOfBands" and
    *  "thePixelType".
    */
   void parseRadString();

   ossimString            theCcfFile;
   ossim_uint32           theNumberOfBands;
   ossimScalarType        thePixelType;
   ossimString            theFileType;
   ossim_uint32           theVersionNumber;
   ossim_uint32           theLinesPerChip;
   ossim_uint32           theSamplesPerChip;
   ossim_uint32           theLineChipsPerChunk;
   ossim_uint32           theSampleChipsPerChunk;
   ossim_uint32           theRectInFullImageStartLine;
   ossim_uint32           theRectInFuleImageStartSample;
   ossim_uint32           theRectInFullImageStopLine;
   ossim_uint32           theRectInFuleImageStopSample;
   ossim_uint32           theNumberOfValidImageVertices;
   std::vector<ossimIpt>  theValidImageVertices;
   ossim_uint32           theFirstBandHeaderPointer;
   ossimString            theRadiometryString;
   ossim_uint32           theBytesPerPixel;
   ossim_uint32           theBytesPerChip;
   ossim_uint32           theBytesPerChunk;
   ossimString            theCompressionType;
   ossim_uint32           theNumberOfRLevels;
   std::vector<bool>           theOccupiedFlag;
   std::vector<std::streampos> theStartOfData;
   std::vector<ossim_uint32>   theNumberOfLines;
   std::vector<ossim_uint32>   theNumberOfSamples;
   std::vector<ossim_uint32>   theChunksInLineDir;
   std::vector<ossim_uint32>   theChunksInSampleDir;
};


#endif // #ifndef CcfHead_HEADER
