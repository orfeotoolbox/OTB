//----------------------------------------------------------------------------
// Copyright (c) 2005, David Burken, all rights reserved.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
// 
// Class for reading and writing an ENVI (The Environment for Visualizing
// Images) header file.
//
//----------------------------------------------------------------------------
// $Id: ossimEnviHeader.cpp,v 1.4 2005/06/21 19:05:04 dburken Exp $

#include <fstream>
#include <string>
using namespace std;

#include <support_data/envi/ossimEnviHeader.h>
#include <base/common/ossimCommon.h>
#include <base/common/ossimKeywordNames.h>
#include <base/common/ossimTrace.h>
#include <base/context/ossimNotifyContext.h>
#include <base/data_types/ossimFilename.h>
#include <base/data_types/ossimKeywordlist.h>

// Define keywords not already found in ossimKeywordNames.
static const char HEADER_OFFSET_KW[]    = "header_offset";
static const char FILE_TYPE_KW[]        = "file_type";
static const char DATA_TYPE_KW[]        = "data_type";
static const char SENSOR_TYPE_KW[]      = "sensor_type";
static const char X_START_KW[]          = "x_start";
static const char MAP_INFO_KW[]         = "map_info";
static const char WAVELENGTH_UNITS_KW[] = "wavelength_units";
static const char BAND_NAME_KW[]        = "band_name";
static const char WAVELENGTH_KW[]       = "wavelength";

static ossimTrace traceDebug(ossimString("ossimEnviHeader:debug"));

ossimEnviHeader::ossimEnviHeader()
   :
   ossimErrorStatusInterface(),
   theDescription(),
   theSamples(0),
   theLines(0),
   theBands(0),
   theHeaderOffset(0),
   theFileType("ENVI Standard"),
   theDataType(0),
   theInterleave("Unknown"),
   theSensorType(),
   theByteOrder(OSSIM_LITTLE_ENDIAN),
   theXStart(0),
   theMapInfo(),
   theWavelengthUnits("Unknown"),
   theBandName(),
   theWavelength()   
{
   // Start the byte order of with the machine byte order.
   theByteOrder = ossimGetByteOrder();
}

ossimEnviHeader::~ossimEnviHeader()
{
}

bool ossimEnviHeader::open(const ossimFilename& file)
{
   ifstream is(file.c_str(), ios_base::in);
   if (!is)
   {
      setErrorStatus();
      return false;
   }

   string s;
   is >> s;

   if (s != "ENVI")
   {
      setErrorStatus();
      return false;
   }

   // Parse the file.
   while (is)
   {
      // read something.
      is >> s;
      
      if (s == "description")
      {
         parseDescription(is);
      }
      else if (s == "samples")
      {
         is >> s; // eat the =
         is >> theSamples;
      }
      else if (s == "lines")
      {
         is >> s; // eat the =
         is >> theLines;
      }
      else if (s == "bands")
      {
         is >> s; // eat the =
         is >> theBands;
      }
      else if (s == "header")
      {
         is >> s;
         if (s == "offset")
         {
            is >> s; // eat the =
            is >> theHeaderOffset;
         }
      }
      else if (s == "file")
      {
         is >> s;
         if (s == "type")
         {
            is >> s; // eat the =
            getline(is, s);
            theFileType = s;
         }
      }
      else if (s == "data")
      {
         is >> s;
         if (s == "type")
         {
            is >> s; // eat the =
            is >> theDataType;
         }
      }
      else if (s == "interleave")
      {
         is >> s; // eat the =
         is >> theInterleave;
      }
      else if (s == "sensor")
      {
         is >> s;
         if (s == "type")
         {
            is >> s; // eat the =
            is >> theSensorType;
         }
      }
      else if (s == "byte")
      {
         is >> s;
         if (s == "order")
         {
            is >> s; // eat the =
            int i;
            is >> i;
            theByteOrder = (i == 1 ? OSSIM_BIG_ENDIAN : OSSIM_LITTLE_ENDIAN);
         }
      }
      else if (s == "x")
      {
         is >> s;
         if (s == "start")
         {
            is >> s; // eat the =
            is >> theXStart;
         }
      }
      else if (s == "map")
      {
         is >> s;
         if (s == "info")
         {
            is >> s; // eat the =
            getline(is, theMapInfo); // one big string
         }
      }
      else if (s == "wavelength")
      {
         is >> s;
         if (s == "units")
         {
            is >> s; // eat the =
            is >> theWavelengthUnits;
         }
         else if (s == "=")
         {
            parseWavelength(is);
         }
      }
      else if (s == "band")
      {
         is >> s;
         if (s == "names")
         {
            parseBandNames(is);
         }
      }
   }

   // Must have these; else, error.
   if (!theSamples || !theLines || !theBands)
   {
      return false;
   }
   
   return true;
}

bool ossimEnviHeader::writeFile(const ossimFilename& file)
{
   if (theDescription.empty())
   {
      theDescription = file;
   }
   
   ofstream out(file.c_str(), ios_base::out);

   if (!out)
   {
      return false;
   }
   
   print(out);
   out.close();
   return true;
}

std::ostream& ossimEnviHeader::print(std::ostream& out) const
{
   out << "ENVI"
       << "\ndescription = {\n" << theDescription << "}"
       << "\nsamples = " << theSamples
       << "\nlines   = " << theLines
       << "\nbands   = " << theBands
       << "\nheader offset = " << theHeaderOffset
       << "\nfile type = " << theFileType
       << "\ndata type = " << theDataType
       << "\ninterleave = " << theInterleave;
   if (theSensorType.size())
   {
      out << "\nsensor type = " << theSensorType;
   }

   out << "\nbyte order = " << theByteOrder;

   if (theXStart)
   {
      out << "\nx start = " << theXStart;
   }

   if (theMapInfo.size())
   {
      out << "\nmap info = " << theMapInfo;
   }

   if (theWavelength.size())
   {
      out << "\nwavelength units = " << theWavelengthUnits;
   }

   if (theBandName.size())
   {
      out << "\nband names = {";
      ossim_uint32 i;
      ossim_uint32 size = theBandName.size();
      for (i = 0; i < size; ++i)
      {
         out << "\n " << theBandName[i];
         if (i < size-1)
         {
            out << ",";
         }
         else
         {
            out << "}";
         }
      }
   }
   else
   {
      out << "\nband names = {";
      ossim_uint32 i;
      for (i = 0; i < theBands; ++i)
      {
         out << "\nBand " << (i+1);
         if (i < theBands-1)
         {
            out << ",";
         }
         else
         {
            out << "}";
         }
      }
   }
   
   if (theWavelength.size())
   {
      out << "\nwavelength = {\n";
      ossim_uint32 i;
      ossim_uint32 size = theWavelength.size();
      for (i = 0; i < size; ++i)
      {
         out << theWavelength[i];
         if (i < size-1)
         {
            out << ", ";
         }
         else
         {
            out << "}";
         }
      }
   }

   out << endl;
   
   return out;
}

ossimString ossimEnviHeader::getDescription() const
{
   return theDescription;
}

void ossimEnviHeader::setDescription(const ossimString& description)
{
   theDescription = description;
}

ossim_uint32 ossimEnviHeader::getSamples() const
{
   return theSamples;
}

void ossimEnviHeader::setSamples(ossim_uint32 samples)
{
   theSamples = samples;
}

ossim_uint32 ossimEnviHeader::getLines() const
{
   return theLines;
}

void ossimEnviHeader::setLines(ossim_uint32 lines)
{
   theLines = lines;
}

ossim_uint32 ossimEnviHeader::getBands() const
{
   return theBands;
}

void ossimEnviHeader::setBands(ossim_uint32 bands)
{
   theBands = bands;
}

ossim_uint32 ossimEnviHeader::getHeaderOffset() const
{
   return theHeaderOffset;
}

void ossimEnviHeader::setHeaderOffset(ossim_uint32 headerOffset)
{
   theHeaderOffset = headerOffset;
}

ossimString ossimEnviHeader::getFileType() const
{
   return theFileType;
}

void ossimEnviHeader::setFileType(const ossimString& fileType)
{
   theFileType = fileType;
}

ossim_uint32 ossimEnviHeader::getDataType() const
{
   return theDataType;
}

ossimScalarType ossimEnviHeader::getOssimScalarType() const
{
   switch( theDataType )
   {
      case 1:
         return OSSIM_UINT8;

      case 2:
         return OSSIM_SINT16;

      case 3:
         return OSSIM_SINT32;

      case 4:
         return OSSIM_FLOAT32;

      case 5:
         return OSSIM_FLOAT64;

      case 6:
         return OSSIM_CFLOAT32;

      case 9:
         return OSSIM_CFLOAT64;

      case 12:
         return OSSIM_UINT16;

      case 13:
         return OSSIM_UINT32;

      default:
         return OSSIM_SCALAR_UNKNOWN;
   }

   return OSSIM_SCALAR_UNKNOWN;
}

void ossimEnviHeader::setDataType(ossimScalarType scalar)
{
   switch( scalar )
   {
      case OSSIM_UINT8:
         theDataType = 1;
         break;

      case OSSIM_SINT16:
         theDataType = 2;
         break;

      case OSSIM_SINT32:
         theDataType = 3;
         break;

      case OSSIM_FLOAT32:
         theDataType = 4;
         break;

      case OSSIM_FLOAT64:
         theDataType = 5;
         break;

      case OSSIM_CFLOAT32:
         theDataType = 6;
         break;

      case OSSIM_CFLOAT64:
         theDataType = 9;
         break;

      case OSSIM_UINT16:
         theDataType = 12;
         break;

      case OSSIM_UINT32:
         theDataType = 13;
         break;

      default:
         break;
   }
}
ossimString ossimEnviHeader::getInterleaveType() const
{
   return theInterleave;
}
ossimInterleaveType ossimEnviHeader::getOssimInterleaveType() const
{
   if (theInterleave == "bsq")
   {
      return OSSIM_BSQ;
   }
   else if  (theInterleave == "bil")
   {
      return OSSIM_BIL;
   }
   else if  (theInterleave == "bip")
   {
      return OSSIM_BIP;
   }
   else
   {
      return OSSIM_INTERLEAVE_UNKNOWN;
   }
}

void ossimEnviHeader::setInterleaveType(ossimInterleaveType interleave)
{
   switch (interleave)
   {
      case OSSIM_BIL:
         theInterleave = "bil";
         break;
         
      case OSSIM_BSQ:
         theInterleave = "bsq";
         break;
         
      case OSSIM_BIP:
         theInterleave = "bip";
         break;
      default:
         theInterleave = "Unknown";
         break;
   }
}

ossimString ossimEnviHeader::getSensorType() const
{
   return theSensorType;
}

void ossimEnviHeader::setSensorType(const ossimString& sensorType)
{
   theSensorType = sensorType;
}

ossimByteOrder ossimEnviHeader::getByteOrder() const
{
   // 0 = little
   // 1 = big
   return theByteOrder;
}

void ossimEnviHeader::setByteorder(ossimByteOrder byteOrder)
{
   theByteOrder = byteOrder;
}

ossim_int32 ossimEnviHeader::getXStart() const
{
   return theXStart;
}

void ossimEnviHeader::setXStart(ossim_int32 xstart)
{
   theXStart = xstart;
}

ossimString ossimEnviHeader::getMapInfo() const
{
   return theMapInfo;
}

void ossimEnviHeader::setMapInfo(const ossimString& mapInfo)
{
   theMapInfo = mapInfo;
}

void ossimEnviHeader::setMapInfo(const ossimKeywordlist& kwl,
                                 const char* prefix)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimEnviHeader::setMapInfo DEBUG:"
         << "\nkwl:\n"
         << kwl
         << endl;
   }

   const char* lookup;
   
   // Get the projection type.
   ossimString projection;
   lookup = kwl.find(prefix, ossimKeywordNames::TYPE_KW);
   if (lookup)
   {
      projection = lookup;
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimEnviHeader::setMapInfo WARNING:"
         << "\nNo projection type found!\nReturning..."
         << endl;
         
      return; // Have to have the projection type!
   }

   // Get the datum.
   ossimString datum = "WGS-84";
   lookup = kwl.find(prefix, ossimKeywordNames::DATUM_KW);
   if (lookup)
   {
      ossimString os = lookup;
      if (os = "WGE")
      {
         datum = "WGS-84";
      }
      else
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimEnviHeader::setMapInfo WARNING:"
            << "\nUnhandled ossim -> envi datum:  " << datum
            << "\nAssuming WGS-84!"
            << endl;
      }
   }

   if ( (projection == "ossimEquDistCylProjection") ||
        (projection == "ossimLlxyProjection") )
   {
      const char* tieLat = NULL;
      const char* tieLon = NULL;
      const char* degLat = NULL;
      const char* degLon = NULL;
      tieLat = kwl.find(prefix, ossimKeywordNames::TIE_POINT_LAT_KW);
      tieLon = kwl.find(prefix, ossimKeywordNames::TIE_POINT_LON_KW);
      degLat = kwl.find(prefix,
                        ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LAT);
      degLon = kwl.find(prefix,
                       ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LON);
      
      if (!tieLat || !tieLon || !degLat || !degLon)
      {
         return;
      }
                 
      theMapInfo = "{Geographic Lat/Lon, 1.0000, 1.0000, ";
      theMapInfo += tieLon;
      theMapInfo += ", ";
      theMapInfo += tieLat;
      theMapInfo += ", ";
      theMapInfo += degLon;
      theMapInfo += ", ";
      theMapInfo += degLat;
      theMapInfo += ", ";
      theMapInfo += datum;
      theMapInfo += ", units=degrees}";
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimEnviHeader::setMapInfo DEBUG:"
         << "\ntheMapInfo:  " << theMapInfo
         << endl;
   }
}

ossimString ossimEnviHeader::getWavelengthUnits() const
{
   return theWavelengthUnits;
}

void ossimEnviHeader::setWavelengthUnits(const ossimString& waveLengthUnits)
{
   theWavelengthUnits = waveLengthUnits;
}

void ossimEnviHeader::getBandNames(vector<ossimString>& bandNames) const
{
   bandNames = theBandName;
}

void ossimEnviHeader::setBandNames(const vector<ossimString>& bandNames)
{
   theBandName = bandNames;
}

void ossimEnviHeader::getWavelengths(vector<ossimString>& waveLengths) const
{
   waveLengths = theWavelength;
}

void ossimEnviHeader::setWavelengths(const vector<ossimString>& wavelengths)
{
   theWavelength = wavelengths;
}

void ossimEnviHeader::parseDescription(std::ifstream& is)
{
   bool record = false;
   theDescription.clear();
   char c = '\0';

   while (c != '}')
   {
      is.get(c);
      if (c == '{') // start record.
      {
         record = true;
         continue;
      }
      else if (c == '}') // finished recording
      {
         break;
      }
      else if (!is) // something bad happened.
      {
         setErrorStatus();
         return;
      }

      if (record)
      {
         theDescription.push_back(c);
      }
   }
}

void ossimEnviHeader::parseWavelength(std::ifstream& is)
{
   bool record = false;
   theWavelength.clear();
   char c = '\0';

   string bandWavelength;
   
   while (c != '}')
   {
      is.get(c);
      if (c == '{') // start record.
      {
         record = true;
         continue;
      }
      else if (c == '\n')
      {
         continue; // skip newlines
      }
      else if (c == '}') // finished recording
      {
         theWavelength.push_back(bandWavelength);
         break;
      }
      else if (c == ',')
      {
         theWavelength.push_back(bandWavelength);
         bandWavelength.clear();
         continue;
      }

      if (!is) // something bad happened.
      {
         setErrorStatus();
         return;
      }

      if (record)
      {
         bandWavelength.push_back(c);
      }
   }
}

void ossimEnviHeader::parseBandNames(std::ifstream& is)
{
   bool record = false;
   theBandName.clear();
   char c = '\0';

   string bandName;
   
   while (c != '}')
   {
      is.get(c);
      if (c == '{') // start record.
      {
         record = true;
         continue;
      }
      else if (c == '\n')
      {
         continue; // skip newlines
      }
      else if (c == '}') // finished recording
      {
         ossimString os = bandName;
         os.trim();
         theBandName.push_back(os);
         break;
      }
      else if (c == ',')
      {
         ossimString os = bandName;
         os.trim();
         theBandName.push_back(os);
         bandName.clear();
         continue;
      }

      if (!is) // something bad happened.
      {
         setErrorStatus();
         return;
      }

      if (record)
      {
         bandName.push_back(c);
      }
   }
}

bool ossimEnviHeader::loadState(const ossimKeywordlist& kwl,
                                const char* prefix)
{
   const char* lookup;
   ossimString s;

   lookup = kwl.find(prefix, ossimKeywordNames::DESCRIPTION_KW);
   if (lookup)
   {
      theDescription = lookup;
   }

   lookup = kwl.find(prefix, ossimKeywordNames::NUMBER_SAMPLES_KW);
   if (lookup)
   {
      s = lookup;
      theSamples = s.toUInt32();
   }
   
   lookup = kwl.find(prefix, ossimKeywordNames::NUMBER_LINES_KW);
   if (lookup)
   {
      s = lookup;
      theLines = s.toUInt32();
   }

   lookup = kwl.find(prefix, ossimKeywordNames::NUMBER_BANDS_KW);
   if (lookup)
   {
      s = lookup;
      theBands = s.toUInt32();
   }

   lookup = kwl.find(prefix, HEADER_OFFSET_KW);
   if (lookup)
   {
      s = lookup;
      theHeaderOffset = s.toUInt32();
   }

   lookup = kwl.find(prefix, FILE_TYPE_KW);
   if (lookup)
   {
      theFileType = lookup;
   }

   lookup = kwl.find(prefix, DATA_TYPE_KW);
   if (lookup)
   {
      s = lookup;
      theDataType = s.toUInt32();
   }

   lookup = kwl.find(prefix, ossimKeywordNames::INTERLEAVE_TYPE_KW);
   if (lookup)
   {
      theInterleave = lookup;
   }

   lookup = kwl.find(prefix, SENSOR_TYPE_KW);
   if (lookup)
   {
      theSensorType = lookup;
   }

   lookup = kwl.find(prefix, ossimKeywordNames::BYTE_ORDER_KW);
   if (lookup)
   {
      s = lookup;
      s.downcase();
      if (s == "little_endian")
      {
         theByteOrder = OSSIM_LITTLE_ENDIAN;
      }
      else if (s == "big_endian")
      {
         theByteOrder= OSSIM_BIG_ENDIAN;
      }
      else
      {
         ossim_uint32 i = s.toUInt32();
         if (i == 0)
         {
            theByteOrder = OSSIM_LITTLE_ENDIAN;
         }
         else if (i == 1)
         {
            theByteOrder= OSSIM_BIG_ENDIAN;
         }
      }
   }
   
   lookup = kwl.find(prefix, X_START_KW);
   if (lookup)
   {
      s = lookup;
      theXStart = (ossimByteOrder)s.toInt32();
   }

   lookup = kwl.find(prefix, MAP_INFO_KW);
   if (lookup)
   {
      theMapInfo = lookup;
   }

   lookup = kwl.find(prefix, WAVELENGTH_UNITS_KW);
   if (lookup)
   {
      theWavelengthUnits = lookup;
   }

   ossim_uint32 n;
   ossim_uint32 count;
   const ossim_uint32 MAX_TRIES = 1024; // Avoid infinate loop.
   
   // Get the band names.
   n = kwl.numberOf(prefix, BAND_NAME_KW);
   if (n)
   {
      theBandName.clear();
      count = 0;
      while ( (count < n) || (count > MAX_TRIES) )
      {
         s = BAND_NAME_KW;
         s += ossimString::toString(count);
         lookup = kwl.find(prefix, s);
         if (lookup)
         {
            theBandName.push_back(ossimString(lookup));
         }
         ++count;
      }
   }
            
   // Get the band names.
   n = kwl.numberOf(prefix, WAVELENGTH_KW);
   if (n)
   {
      theWavelength.clear();
      count = 0;
      while ( (count < n) || (count > MAX_TRIES) )
      {
         s = WAVELENGTH_KW;
         s += ossimString::toString(count);
         lookup = kwl.find(prefix, s);
         if (lookup)
         {
            theWavelength.push_back(ossimString(lookup));
         }
         ++count;
      }
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimEnviHeader::loadState DEUG\n"
         << *this
         << endl;
   }
   

   
   
   return true;
}
