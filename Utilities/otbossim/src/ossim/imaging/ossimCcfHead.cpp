//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  David Burken
//
// Description:
//
// Contains class definition for CcfHead.
// 
//*******************************************************************
//  $Id: ossimCcfHead.cpp 13842 2008-11-04 19:40:10Z gpotts $

#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;

#include <ossim/imaging/ossimCcfHead.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimScalarTypeLut.h>
#include <ossim/base/ossimErrorCodes.h>
#include <ossim/base/ossimErrorContext.h>

static const bool TRACE = false; // Temp until trace code implemented.

ossimCcfHead::ossimCcfHead()
   :
      theCcfFile(),
      theNumberOfBands(1),
      thePixelType(OSSIM_UCHAR),
      theFileType(),
      theVersionNumber(0),
      theLinesPerChip(0),
      theSamplesPerChip(0),
      theLineChipsPerChunk(0),
      theSampleChipsPerChunk(0),
      theRectInFullImageStartLine(0),
      theRectInFuleImageStartSample(0),
      theRectInFullImageStopLine(0),
      theRectInFuleImageStopSample(0),
      theNumberOfValidImageVertices(0),
      theValidImageVertices(1),
      theFirstBandHeaderPointer(0),
      theRadiometryString(),
      theBytesPerPixel(0),
      theBytesPerChip(0),
      theBytesPerChunk(0),
      theCompressionType(),
      theNumberOfRLevels(0),
      theOccupiedFlag(1),
      theStartOfData(1),
      theNumberOfLines(1),
      theNumberOfSamples(1),
      theChunksInLineDir(1),
      theChunksInSampleDir(1)
{}

//***************************************************************************
// Public Constructor:
//***************************************************************************
ossimCcfHead::ossimCcfHead(const char* ccf_file)
   :
      theCcfFile(ccf_file),
      theNumberOfBands(1),
      thePixelType(OSSIM_UCHAR),
      theFileType(),
      theVersionNumber(0),
      theLinesPerChip(0),
      theSamplesPerChip(0),
      theLineChipsPerChunk(0),
      theSampleChipsPerChunk(0),
      theRectInFullImageStartLine(0),
      theRectInFuleImageStartSample(0),
      theRectInFullImageStopLine(0),
      theRectInFuleImageStopSample(0),
      theNumberOfValidImageVertices(0),
      theValidImageVertices(1),
      theFirstBandHeaderPointer(0),
      theRadiometryString(""),
      theBytesPerPixel(0),
      theBytesPerChip(0),
      theBytesPerChunk(0),
      theCompressionType(),
      theNumberOfRLevels(0),
      theOccupiedFlag(1),
      theStartOfData(1),
      theNumberOfLines(1),
      theNumberOfSamples(1),
      theChunksInLineDir(1),
      theChunksInSampleDir(1)
{
   static const char MODULE[] = "ossimCcfHead::ossimCcfHead";
   
   if (!parseCcfHeader(ccf_file))
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      
      if (TRACE)
      {
         cerr << MODULE << " ERROR!"
              << "\nError initializing from ccf_file:  " << ccf_file
              << "\nReturning..." << std::endl;
      }
   }
}

//***************************************************************************
// Destructor:
//***************************************************************************
ossimCcfHead::~ossimCcfHead()
{}

//***************************************************************************
// Public Method:
//***************************************************************************
bool ossimCcfHead::parseCcfHeader(const char* ccf_file)
{
   static const char MODULE[] = "ossimCcfHead::parseossimCcfHeader";

   theCcfFile = ccf_file;
   
   std::ifstream *is = new std::ifstream(ccf_file);

   if (!(*is))
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      
      if (TRACE)
      {
         cerr << MODULE << " ERROR!"
              << "\nCannot open file:  " << ccf_file << std::endl;
      }
      
      is->close();
      delete is;
      
      return false;
   }

   const ossim_uint32 MAX_LEN = 256;
   char tmp[MAX_LEN];

   // Check the first string should be "CCF" else get out...
   // this might hang so I changed it to not use the
   // >> operator unless it is a ccf file.  If it is another
   // file we might not be guranteed a whitespace or \n character
   // will exist and therefore the entrie file could have
   // been read in.
   //
   char type[4];
   is->read(type, 3);
   type[3] = '\0';
   theFileType = type;
   if (theFileType != "CCF")
   {
      if (TRACE)
      {
         cerr << MODULE << " ERROR!"
              << "\nNot a ccf file.  Returning..." << std::endl;
      }

      is->close();
      delete is;

      return false;
   }

   
   // Check the version number.
   (*is) >> tmp 
         >> theVersionNumber;

   //***
   // Call the appropriate method for the version.  Currently only version 6
   // supported as that was all I had in-house.  Feel free to add your own
   // version reader!
   //***
   bool status = false;
   
   switch(theVersionNumber)
   {
   case 5:
      status = parseV5CcfHeader(*is);
      break;
   case 6:
      status = parseV6CcfHeader(*is);
      break;
   default:
      // Version type not supported...
      if (TRACE)
      {
         cerr << MODULE << " ERROR!"
              << "\nUsupported version:  " << theVersionNumber
              << "  Returning..." << std::endl;
      }
      is->close();
      delete is;
       return false;

      break;
   }
   
   // Close the stream.
   is->close();
   delete is;
   //***
   // Parse the radiometry string.  This will initialize "theNumberOfBands"
   // and the pixel type.
   //***
   parseRadString();

   return status;
}

//***************************************************************************
// Private Method:
//***************************************************************************
bool ossimCcfHead::parseV5CcfHeader(std::istream& is)
{
   static const char MODULE[] = "CcfHead::parseV5CcfHeader";

   // Check the stream.
   if (!is)
   {
      if (TRACE)
      {
         cerr << MODULE << " Bad Stream passed to method!"
              << "\nReturning..."
              << std::endl;
      }

      return false;
   }

   const ossim_uint32 MAX_LEN = 256;
   char tmp[MAX_LEN];
   
   //***
   // These are all fixed/not used so just skip...
   //***
   is.read(tmp, 1);                   // eat the '\n'
   is.getline(tmp, MAX_LEN-1, '\n');  // skip "ccf_maker"

   is >> tmp // "LinesPerChip"
      >> theLinesPerChip
      >> tmp // "SamplesPerChip"
      >> theSamplesPerChip
      >> tmp // "LineChipsPerChunk"
      >> theLineChipsPerChunk
      >> tmp // "SampleChipsPerChunk"   
      >> theSampleChipsPerChunk;

   is.read(tmp, 1);                   // eat the '\n'
   is.getline(tmp, MAX_LEN-1, '\n');  // skip "NumberOfBands"
   is.getline(tmp, MAX_LEN-1, '\n');  // skip "RectInFullImageSpace"

   is >> tmp // skip "StartLine"
      >> theRectInFullImageStartLine
      >> tmp // skip "StartSample"
      >> theRectInFuleImageStartSample
      >> tmp // skip "StopLine"
      >> theRectInFullImageStopLine
      >> tmp // skip "StopSample"
      >> theRectInFuleImageStopSample;

   // No valid image vertices in this version.
   theNumberOfValidImageVertices = 0;
   theValidImageVertices[0] = ossimIpt(0,0);
   
   is.read(tmp, 1);                   // eat the '\n'
   is.getline(tmp, MAX_LEN-1, '\n');  // skip blank line

   is >> tmp                         // skip "FirstBandHeaderPointer"
      >> theFirstBandHeaderPointer;  // Offset to band header.

   // Seek to the band header record.
   is.seekg(theFirstBandHeaderPointer, ios::beg);

   is.getline(tmp, MAX_LEN-1, '\n');  // skip "BAND" line
   is.getline(tmp, MAX_LEN-1, '\n');  // skip "NextBandHeaderPointer" line
   is.getline(tmp, MAX_LEN-1, '\n');  // skip "ChunkMapPointer" line

   is >> tmp;  // skip "Radiometry"

   is.getline(tmp, MAX_LEN-1, '\n'); // Get the radiometry string.
   const char* rad = tmp;
   while ( *rad && (*rad == ' ')) rad++;  // Eat the whitespaces...
   theRadiometryString = rad;

   is >> tmp // skip "BytesPerPixel"
      >> theBytesPerPixel
      >> tmp // skip "BytesPerChip"
      >> theBytesPerChip
      >> tmp // skip "BytesPerChunk"
      >> theBytesPerChunk
      >> tmp // skip "CompressionType"
      >> theCompressionType
      >> tmp // skip "NumberOfRLevels"
      >> theNumberOfRLevels;

   if ((ossim_uint32)theNumberOfRLevels > theOccupiedFlag.size())
   {
      // Resize all the vectors.
      theOccupiedFlag.resize(theNumberOfRLevels);
      theStartOfData.resize(theNumberOfRLevels);
      theNumberOfLines.resize(theNumberOfRLevels);
      theNumberOfSamples.resize(theNumberOfRLevels);
      theChunksInLineDir.resize(theNumberOfRLevels);
      theChunksInSampleDir.resize(theNumberOfRLevels);
   }

   is.read(tmp, 1);                  // eat the '\n'
   is.getline(tmp, MAX_LEN-1, '\n'); // skip blank line
   
   ossim_uint32 i;
   for (i=0; i<theNumberOfRLevels; i++)
   {
      is.getline(tmp, MAX_LEN-1, '\n'); // skip the Rlevel line

      ossim_uint32 tmp_long;
      
      is >> tmp // skip "Occupied"
         >> tmp_long;

      theOccupiedFlag[i] = tmp_long;

      is >> tmp // skip "StartOfData"
         >> tmp_long;

      theStartOfData[i] = tmp_long;
      
      is >> tmp // skip "NumberOfLines"
         >> tmp_long;

      theNumberOfLines[i] = tmp_long;

      is >> tmp // skip "NumberOfSamples"
         >> tmp_long;

      theNumberOfSamples[i] = tmp_long;

      is >> tmp // skip "ChunksInLineDir"
         >> tmp_long;

      theChunksInLineDir[i] = tmp_long;

      is >> tmp // skip "ChunksInSampleDir"
         >> tmp_long;

      theChunksInSampleDir[i] = tmp_long;

      is.getline(tmp, MAX_LEN-1, '\n'); // skip "LineOffset" line
      is.getline(tmp, MAX_LEN-1, '\n'); // skip "LineDecimation" line
      is.getline(tmp, MAX_LEN-1, '\n'); // skip "SampleOffset" line
      is.getline(tmp, MAX_LEN-1, '\n'); // skip "SampleDecimation" line
      is.read(tmp, 1);                  // Eat the '\n'
      is.getline(tmp, MAX_LEN-1, '\n'); // skip blank line
      is.read(tmp, 1);                  // Eat the '\n'
   }

   // Note:  The caller will close the stream.
   
   return true;
}

//***************************************************************************
// Private Method:
//***************************************************************************
bool ossimCcfHead::parseV6CcfHeader(std::istream& is)
{
   static const char MODULE[] = "CcfHead::parseV6CcfHeader";

   // Check the stream.
   if (!is)
   {
      if (TRACE)
      {
         cerr << MODULE << " Bad Stream passed to method!"
              << "\nReturning..."
              << std::endl;
      }

      return false;
   }

   const ossim_uint32 MAX_LEN = 256;
   char tmp[MAX_LEN];
   
   //***
   // These are all fixed/not used so just skip...
   //***
   is.read(tmp, 1);                   // eat the '\n'
   is.getline(tmp, MAX_LEN-1, '\n');  // skip "ccf_maker"

   is >> tmp // "LinesPerChip"
      >> theLinesPerChip
      >> tmp // "SamplesPerChip"
      >> theSamplesPerChip
      >> tmp // "LineChipsPerChunk"
      >> theLineChipsPerChunk
      >> tmp // "SampleChipsPerChunk"
      >> theSampleChipsPerChunk;

   is.read(tmp, 1);                   // eat the '\n'
   is.getline(tmp, MAX_LEN-1, '\n');  // skip "NumberOfBands"
   is.getline(tmp, MAX_LEN-1, '\n');  // skip "RectInFullImageSpace"

   is >> tmp // skip "StartLine"
      >> theRectInFullImageStartLine
      >> tmp // skip "StartSample"
      >> theRectInFuleImageStartSample
      >> tmp // skip "StopLine"
      >> theRectInFullImageStopLine
      >> tmp // skip "StopSample"
      >> theRectInFuleImageStopSample
      >> tmp // skip "ValidImageVertices"
      >> theNumberOfValidImageVertices;

   // Get the valid image vertices.
   ossim_uint32 i;

   if (theNumberOfValidImageVertices != theValidImageVertices.size())
   {
      theValidImageVertices.resize(theNumberOfValidImageVertices);
   }

   for (i=0; i<theNumberOfValidImageVertices; i++)
   {
      ossimString tmp_dbl;  // Can be doubles in header.
      
      is >> tmp      // skip "Vertex"
         >> tmp      // skip Vertex number
         >> tmp_dbl; // line

      theValidImageVertices[i].y = static_cast<int>(tmp_dbl.toDouble());
      tmp_dbl = "";
      is >> tmp_dbl; // sample

      theValidImageVertices[i].x = static_cast<int>(tmp_dbl.toDouble());
   }

   is.read(tmp, 1);                   // eat the '\n'
   is.getline(tmp, MAX_LEN-1, '\n');  // skip blank line

   is >> tmp                         // skip "FirstBandHeaderPointer"
      >> theFirstBandHeaderPointer;  // Offset to band header.

   // Seek to the band header record.
   is.seekg(theFirstBandHeaderPointer, ios::beg);

   is.getline(tmp, MAX_LEN-1, '\n');  // skip "BAND" line
   is.getline(tmp, MAX_LEN-1, '\n');  // skip "NextBandHeaderPointer" line
   is.getline(tmp, MAX_LEN-1, '\n');  // skip "ChunkMapPointer" line

   is >> tmp;  // skip "Radiometry"

   is.getline(tmp, MAX_LEN-1, '\n'); // Get the radiometry string.
   
   const char* rad = tmp;
   while ( *rad && (*rad == ' ')) rad++;  // Eat the whitespaces...
   theRadiometryString = rad;

   is >> tmp // skip "BytesPerPixel"
      >> theBytesPerPixel
      >> tmp // skip "BytesPerChip"
      >> theBytesPerChip
      >> tmp // skip "BytesPerChunk"
      >> theBytesPerChunk
      >> tmp // skip "CompressionType"
      >> theCompressionType
      >> tmp // skip "NumberOfRLevels"
      >> theNumberOfRLevels;

   if (theNumberOfRLevels > theOccupiedFlag.size())
   {
      // Resize all the vectors.
      theOccupiedFlag.resize(theNumberOfRLevels);
      theStartOfData.resize(theNumberOfRLevels);
      theNumberOfLines.resize(theNumberOfRLevels);
      theNumberOfSamples.resize(theNumberOfRLevels);
      theChunksInLineDir.resize(theNumberOfRLevels);
      theChunksInSampleDir.resize(theNumberOfRLevels);
   }

   is.read(tmp, 1);                  // eat the '\n'
   is.getline(tmp, MAX_LEN-1, '\n'); // skip blank line
   
   for (i=0; i<theNumberOfRLevels; i++)
   {
      is.getline(tmp, MAX_LEN-1, '\n'); // skip the Rlevel line

      ossim_uint32 tmp_long;
      
      is >> tmp // skip "Occupied"
         >> tmp_long;

      theOccupiedFlag[i] = tmp_long;

      is >> tmp // skip "StartOfData"
         >> tmp_long;

      theStartOfData[i] = tmp_long;
      
      is >> tmp // skip "NumberOfLines"
         >> tmp_long;

      theNumberOfLines[i] = tmp_long;

      is >> tmp // skip "NumberOfSamples"
         >> tmp_long;

      theNumberOfSamples[i] = tmp_long;

      is >> tmp // skip "ChunksInLineDir"
         >> tmp_long;

      theChunksInLineDir[i] = tmp_long;

      is >> tmp // skip "ChunksInSampleDir"
         >> tmp_long;

      theChunksInSampleDir[i] = tmp_long;      

      is.getline(tmp, MAX_LEN-1, '\n'); // skip "LineOffset" line
      is.getline(tmp, MAX_LEN-1, '\n'); // skip "LineDecimation" line
      is.getline(tmp, MAX_LEN-1, '\n'); // skip "SampleOffset" line
      is.getline(tmp, MAX_LEN-1, '\n'); // skip "SampleDecimation" line
      is.read(tmp, 1);                  // Eat the '\n'
      is.getline(tmp, MAX_LEN-1, '\n'); // skip blank line
      is.read(tmp, 1);                  // Eat the '\n'
   }

   // Note:  The caller will close the stream.
   
   return true;
}

//***************************************************************************
// Public Method:
//***************************************************************************
std::ostream& ossimCcfHead::print(std::ostream& out) const
{
   static const char MODULE[] = "ossimCcfHead::print";

   out << MODULE
       << "\ntheErrorStatus:                 " << theErrorStatus
       << "\ntheCcfFile:                     " << theCcfFile
       << "\ntheNumberOfBands:               " << theNumberOfBands
       << "\nthePixelType:                   "
       << (ossimScalarTypeLut::instance()->getEntryString(thePixelType))
       << "\ntheFileType:                    " << theFileType
       << "\ntheVersionNumber:               " << theVersionNumber
       << "\ntheLinesPerChip:                " << theLinesPerChip
       << "\ntheSamplesPerChip:              " << theSamplesPerChip
       << "\ntheLineChipsPerChunk:           " << theLineChipsPerChunk
       << "\ntheSampleChipsPerChunk:         " << theSampleChipsPerChunk
       << "\ntheRectInFullImageStartLine:    " << theRectInFullImageStartLine
       << "\ntheRectInFuleImageStartSample:  " << theRectInFuleImageStartSample
       << "\ntheRectInFullImageStopLine:     " << theRectInFullImageStopLine
       << "\ntheRectInFuleImageStopSample:   " << theRectInFuleImageStopSample
       << "\ntheNumberOfValidImageVertices:  "
       << theNumberOfValidImageVertices;
   
   ossim_uint32 i;
   
   for (i=0; i<theNumberOfValidImageVertices; i++)
   {
      out << "\ntheValidImageVertices[" << i << "]:  "
          << theValidImageVertices[i];
   }
   
   out << "\ntheFirstBandHeaderPointer:  " << theFirstBandHeaderPointer
       << "\ntheRadiometryString:        " << theRadiometryString
       << "\ntheBytesPerPixel:           " << theBytesPerPixel
       << "\ntheBytestPerChip:           " << theBytesPerChip
       << "\ntheBytesPerChunk:           " << theBytesPerChunk
       << "\ntheCompressionType:         " << theCompressionType
       << "\ntheNumberOfRLevels:         " << theNumberOfRLevels;
   
   for (i=0; i<theNumberOfRLevels; i++)
   {
      out << "\ntheOccupiedFlag[" << i << "]:       " << theOccupiedFlag[i]
          << "\ntheStartOfData[" << i << "]:        " << theStartOfData[i]
          << "\ntheNumberOfLines[" << i << "]:      " << theNumberOfLines[i]
          << "\ntheNumberOfSamples[" << i << "]:    " << theNumberOfSamples[i]
          << "\ntheChunksInLineDir[" << i << "]:    " << theChunksInLineDir[i]
          << "\ntheChunksInSampleDir[" << i << "]:  "
          << theChunksInSampleDir[i];
   }
   
   out << std::endl;

   return ossimErrorStatusInterface::print(out);
}

//***************************************************************************
// Public Method:
//***************************************************************************
ossim_uint32 ossimCcfHead::numberOfLines(ossim_uint32 reduced_res_level) const
{
   static const char MODULE[] = "ossimCcfHead::numberOfLines";

   if (reduced_res_level > highestReducedResSet() )
   {
      cerr << MODULE << " ERROR!"
           << "\nInvalid reduced res level:  " << reduced_res_level
           << "\nHighest reduced res level available:  "
           << highestReducedResSet() << std::endl;   
      return 0;
   }

   return theNumberOfLines[reduced_res_level];
}

//***************************************************************************
// Public Method:
//***************************************************************************
ossim_uint32 ossimCcfHead::numberOfSamples(ossim_uint32 reduced_res_level) const
{
   static const char MODULE[] = "ossimCcfHead::numberOfSamples";

   if (reduced_res_level > highestReducedResSet() )
   {
      cerr << MODULE << " ERROR!"
           << "\nInvalid reduced res level:  " << reduced_res_level
           << "\nHighest reduced res level available:  "
           << highestReducedResSet() << std::endl;   
      return 0;
   }
   
   return theNumberOfSamples[reduced_res_level];
}

//***************************************************************************
// Public Method:
//***************************************************************************
ossim_uint32 ossimCcfHead::chunksInLineDir(ossim_uint32 reduced_res_level) const
{
   static const char MODULE[] = "ossimCcfHead::chunksInLineDir";

   if (reduced_res_level > highestReducedResSet() )
   {
      cerr << MODULE << " ERROR!"
           << "\nInvalid reduced res level:  " << reduced_res_level
           << "\nHighest reduced res level available:  "
           << highestReducedResSet() << std::endl;   
      return 0;
   }
   
   return theChunksInLineDir[reduced_res_level];
}

//***************************************************************************
// Public Method:
//***************************************************************************
ossim_uint32 ossimCcfHead::chunksInSampleDir(ossim_uint32 reduced_res_level) const
{
   static const char MODULE[] = "ossimCcfHead::chunksInSampleDir";

   if (reduced_res_level > highestReducedResSet() )
   {
      cerr << MODULE << " ERROR!"
           << "\nInvalid reduced res level:  " << reduced_res_level
           << "\nHighest reduced res level available:  "
           << highestReducedResSet() << std::endl;   
      return 0;
   }
   
   return theChunksInSampleDir[reduced_res_level];
}


//***************************************************************************
// Public Method:
//***************************************************************************
std::streampos ossimCcfHead::startOfData(ossim_uint32 reduced_res_level) const
{
   static const char MODULE[] = "ossimCcfHead::startOfData";

   if (reduced_res_level > highestReducedResSet() )
   {
      cerr << MODULE << " ERROR!"
           << "\nInvalid reduced res level:  " << reduced_res_level
           << "\nHighest reduced res level available:  "
           << highestReducedResSet() << std::endl;   
      return 0;
   }
   
   return theStartOfData[reduced_res_level];
}

//***************************************************************************
// Public Method:
//***************************************************************************
ossimIrect ossimCcfHead::imageRect(ossim_uint32 reduced_res_level) const
{
   static const char MODULE[] = "ossimCcfHead::imageRect";
   
   if (reduced_res_level > highestReducedResSet() )
   {
      cerr << MODULE << " ERROR!"
           << "\nInvalid reduced res level:  " << reduced_res_level
           << "\nHighest reduced res level available:  "
           << highestReducedResSet() << std::endl;
      return ossimIrect(0,0,0,0);
   }

   return ossimIrect(0,
                     0,
                     numberOfSamples(reduced_res_level) - 1,
                     numberOfLines(reduced_res_level)   - 1);
}

//***************************************************************************
// Public Method:
//***************************************************************************
void ossimCcfHead::parseRadString()
{
   static const char MODULE[] = "ossimCcfHead::parseRadString";
   
   if (theRadiometryString.contains("RadiomNBand"))
   {
      //***
      // Complex string, must parse. Should look like:
      // "RadiomNBand (3 3 Linear8 )"
      //***
      char tmp[80];
      std::istringstream is(theRadiometryString);

      is >> tmp;               // Skip the "RadiomNBand" string.
      is.get(tmp, 3);          // Eat the space and the '('.
      is >> theNumberOfBands;  // Get the number of bands.
   }

   if (theRadiometryString.contains("8"))
   {
      thePixelType = OSSIM_UCHAR;
   }
   else if(theRadiometryString.contains("16"))
   {
      thePixelType = OSSIM_USHORT16;
   }
   else if (theRadiometryString.contains("11"))
   {
      thePixelType = OSSIM_USHORT11;
   }
   else
   {
      cerr << MODULE << " Unknown radiometry!"
           << "\ntheRadiometryString:  " << theRadiometryString << std::endl;
   }

   if (TRACE)
   {
      cout << MODULE
           << "\ntheRadiometryString:  " << theRadiometryString
           << "\ntheNumberOfBands:     " << theNumberOfBands
           << "\nthePixelType:         "
           << (ossimScalarTypeLut::instance()->getEntryString(thePixelType))
           << std::endl;
   }
}
