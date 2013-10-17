//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:
// 
// Class for reading and writing an ENVI (The Environment for Visualizing
// Images) header file.
//
//----------------------------------------------------------------------------
// $Id: ossimEnviHeader.cpp 22349 2013-08-01 21:38:29Z dburken $

#include <ossim/support_data/ossimEnviHeader.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/support_data/ossimWavelength.h>
#include <algorithm>
#include <fstream>
#include <string>

typedef  std::unary_function<std::pair<ossimString, ossimString>, bool> KwlCompareFunctionType;
typedef  std::pair<const ossimString, ossimString> KwlComparePairType;

class KwlKeyCaseInsensitiveEquals : public KwlCompareFunctionType
{
public:
   KwlKeyCaseInsensitiveEquals(const ossimString& key):m_key(key){}
   virtual bool operator()(const KwlComparePairType& rhs)const
   {
      return (m_key == rhs.first.downcase());
   }
   ossimString m_key;
};
class KwlKeySubStringCaseInsensitive : public KwlCompareFunctionType
{
public:
   KwlKeySubStringCaseInsensitive(const ossimString& key):m_key(key){}
   virtual bool operator()(const KwlComparePairType& rhs)const
   {
      return (rhs.first.downcase().contains(m_key));
   }
   ossimString m_key;
};

// Define keywords not already found in ossimKeywordNames.
static const char HEADER_OFFSET_KW[]    = "header_offset";
static const char FILE_TYPE_KW[]        = "file_type";
static const char DATA_TYPE_KW[]        = "data_type";
static const char SENSOR_TYPE_KW[]      = "sensor_type";
static const char X_START_KW[]          = "x_start";
static const char Y_START_KW[]          = "y_start";
static const char MAP_INFO_KW[]         = "map_info";
static const char WAVELENGTH_UNITS_KW[] = "wavelength_units";
static const char BAND_NAME_KW[]        = "band_name";
static const char WAVELENGTH_KW[]       = "wavelength";

static ossimTrace traceDebug(ossimString("ossimEnviHeader:debug"));

ossimEnviHeader::ossimEnviHeader()
   :
   m_file(),
   m_keywords()
{
   // Start the byte order of with the machine byte order.
   m_keywords[FILE_TYPE_KW] = "ENVI Standard";
   setByteorder(ossim::byteOrder());   
}

ossimEnviHeader::~ossimEnviHeader()
{
}

void ossimEnviHeader::reset()
{
   m_file.string().clear();
   m_keywords.clear();
   m_keywords[FILE_TYPE_KW] = "ENVI Standard";
   setByteorder(ossim::byteOrder());
}

const ossimKeywordlist& ossimEnviHeader::getMap() const
{
   return m_keywords;
}

ossimKeywordlist& ossimEnviHeader::getMap()
{
   return m_keywords;
}

bool ossimEnviHeader::getValue( const ossimString& key, ossimString& value ) const
{
   bool result = false;
   value.string() = m_keywords.findKey( key.string());
   if( value.size() )
   {
      result = true;
   }
   else
   {
      result = m_keywords.hasKey( key.string() );
   }
   return result;
}

bool ossimEnviHeader::findSubStringCaseInsensitive(const ossimString& key,
                                                   ossimString& value) const
{
   return m_keywords.findValue<KwlKeySubStringCaseInsensitive>(
      value, KwlKeySubStringCaseInsensitive(key));
}

bool ossimEnviHeader::open(const ossimFilename& file)
{
   bool result = false;

   reset(); // Clear the map, file name.

   std::ifstream in;
   in.open(file.c_str(), std::ios::in | std::ios::binary);
   if( in.is_open() )
   {
      result = readStream( in );
      if ( result )
      {
         m_file = file;
      }
   }

   return result;
}

bool ossimEnviHeader::readStream(std::istream& in)
{
   reset();
   bool result = isEnviHeader( in );
   if ( result )
   {
      m_keywords.clear();
      
      while(!in.eof()&&in.good())
      {
         // read name
         ossimString name = "";
         ossimString value = "";
         ossim::skipws(in);
         int c = static_cast<char>(in.get());
         while((c != '=')&&(in.good())&&!in.eof())
         {
            name +=static_cast<char>(c);
            c = in.get();
         }
         ossim::skipws(in);
         c = in.get();
         
         if(in.good()&&!in.eof())
         {
            if(c == '{') // continue til '}'
            {
               c = in.get();
               while((c != '}')&&(in.good())&&!in.eof())
               {
                  value +=static_cast<char>(c);
                  c = in.get();
               }
            }
            else
            {
               while(((c != '\n')&&(c!='\r'))&&(in.good())&&!in.eof())
               {
                  value +=static_cast<char>(c);
                  c = (in.get());
               }
               
            }
            m_keywords.add(name.trim(), value);
         }
      }

      // Test for minimum set of keywords needed.
      if ( m_keywords.findKey( std::string("samples") ).empty() ||
           m_keywords.findKey( std::string("lines") ).empty() || 
           m_keywords.findKey( std::string("bands") ).empty())
      {
         result =  false;
      }
   }

   return result;
}

bool ossimEnviHeader::writeFile(const ossimFilename& file)
{
   if (m_keywords.findKey( std::string("description") ).empty())
   {
      m_keywords["description"] = file.c_str();
   }
   
   std::ofstream out(file.c_str(), std::ios_base::out);

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
   out << "ENVI" << "\n" << m_keywords << std::endl;
   return out;
}

std::ostream& operator<<(std::ostream& out, const ossimEnviHeader& obj)
{
   return obj.print( out );
}

ossimString ossimEnviHeader::getDescription() const
{
   return ossimString( m_keywords.findKey( std::string("description") ) );
}

void ossimEnviHeader::setDescription(const ossimString& description)
{
   m_keywords["description"] = description.c_str();
}

ossim_uint32 ossimEnviHeader::getSamples() const
{
   ossim_uint32 result = 0;
   ossimString value = m_keywords.findKey( std::string("samples") );
   if( value.size() )
   {
      result = value.toUInt32();
   }
   return result;
}

void ossimEnviHeader::setSamples(ossim_uint32 samples)
{
   m_keywords[ std::string("samples") ] = ossimString::toString(samples).c_str();
}

ossim_uint32 ossimEnviHeader::getLines() const
{
   ossim_uint32 result = 0;
   ossimString value = m_keywords.findKey( std::string("lines") );
   if( value.size() )
   {
      result = value.toUInt32();
   }
   return result;
}

void ossimEnviHeader::setLines(ossim_uint32 lines)
{
   m_keywords["lines"] = ossimString::toString(lines).c_str();
}

ossim_uint32 ossimEnviHeader::getBands() const
{
   ossim_uint32 result = 0;
   ossimString value = m_keywords.findKey( std::string("bands") );
   if( value.size() )
   {
      result = value.toUInt32();
   }
   return result;
}

void ossimEnviHeader::setBands(ossim_uint32 bands)
{
   m_keywords["bands"] = ossimString::toString(bands).c_str();
}

bool ossimEnviHeader::getDefaultBands( std::vector<ossim_uint32>& bands ) const
{
   bands.clear();

   // Note: We could do a "m_map.downcaseKeywords()". (drb)
   ossimString value = m_keywords.findKey( std::string("default bands") );
   if ( !value.size() )
   {
      value = m_keywords.findKey( std::string("Default bands") );
      if ( !value.size() )
      {
        value = m_keywords.findKey( std::string("DEFAULT BANDS") );
      }
   }

   if ( value.size() )
   {
      std::vector<ossimString> strLst;
      value.split( strLst, ossimString(","));
      if ( strLst.size() )
      {
         std::vector<ossimString>::const_iterator i = strLst.begin();
         ossim_uint32 band = 0;
         while ( i != strLst.end() )
         {
            band = (*i).toUInt32();
            if ( band )
            {
               // Assuming "default bands" are one based.  Totally a hunch... (drb)
               --band;
               bands.push_back(band);
            }
            else
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << "ossimEnviHeader::getDefaultBands WARN!"
                  << "\nDetected zero based bands in \"default bands\" from header!"
                  << std::endl;
            }
            
            ++i;
         }
      }
   }

   if ( rangeCheckBands( bands ) == false )
   {
      bands.clear();
   }
   
   return (bands.size() ? true : false);
}

ossim_uint32 ossimEnviHeader::getHeaderOffset() const
{
   ossim_uint32 result = 0;
   ossimString value = m_keywords["header offset"];
   if( value.size() )
   {
      result = value.toUInt32();
   }
   return result;
}

void ossimEnviHeader::setHeaderOffset(ossim_uint32 headerOffset)
{
   m_keywords["header offset"] = ossimString::toString(headerOffset).c_str();
}

ossimString ossimEnviHeader::getFileType() const
{
   return m_keywords[FILE_TYPE_KW];
}

void ossimEnviHeader::setFileType(const ossimString& fileType)
{
    m_keywords[FILE_TYPE_KW] = fileType.c_str();
}

ossim_uint32 ossimEnviHeader::getDataType() const
{
   return ossimString(m_keywords["data type"]).toUInt32();
}

ossimScalarType ossimEnviHeader::getOssimScalarType() const
{
   ossim_uint32 dataTypeInt = ossimString(m_keywords["data type"]).toUInt32();
   
   ossimScalarType result = OSSIM_SCALAR_UNKNOWN;

   switch( dataTypeInt )
   {
      case 1:
         result = OSSIM_UINT8;
         break;
      case 2:
         result = OSSIM_SINT16;
         break;
      case 3:
         result = OSSIM_SINT32;
         break;
      case 4:
         result = OSSIM_FLOAT32;
         break;
      case 5:
         result = OSSIM_FLOAT64;
         break;
      case 6:
         result = OSSIM_CFLOAT32;
         break;
      case 9:
         result = OSSIM_CFLOAT64;
         break;
      case 12:
         result = OSSIM_UINT16;
         break;
      case 13:
         result = OSSIM_UINT32;
         break;
      default:
         break;
   }

   return result;
}

void ossimEnviHeader::setDataType(ossimScalarType scalar)
{
   ossimString dataTypeString = "";
   switch( scalar )
   {
      case OSSIM_UINT8:
         dataTypeString = "1";
         break;
      case OSSIM_SINT16:
         dataTypeString = "2";
         break;
      case OSSIM_SINT32:
         dataTypeString = "3";
         break;
      case OSSIM_FLOAT32:
         dataTypeString = "4";
         break;
      case OSSIM_FLOAT64:
         dataTypeString = "5";
         break;
      case OSSIM_CFLOAT32:
         dataTypeString = "6";
         break;
      case OSSIM_CFLOAT64:
         dataTypeString = "9";
         break;
      case OSSIM_UINT16:
         dataTypeString = "12";
         break;
      case OSSIM_UINT32:
         dataTypeString = "13";
         break;
      default:
         break;
   }
   
   if(!dataTypeString.empty())
   {
      m_keywords["data type"] = dataTypeString.c_str();
   }
}
ossimString ossimEnviHeader::getInterleaveType() const
{
   return m_keywords["interleave"];
}
ossimInterleaveType ossimEnviHeader::getOssimInterleaveType() const
{
   ossimString interleave = getInterleaveType();
   if (interleave == "bsq")
   {
      return OSSIM_BSQ;
   }
   else if  (interleave == "bil")
   {
      return OSSIM_BIL;
   }
   else if  (interleave == "bip")
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
   std::string interleaveString = "";
   switch (interleave)
   {
      case OSSIM_BIL:
         interleaveString = "bil";
         break;
      case OSSIM_BSQ:
         interleaveString = "bsq";
         break;
      case OSSIM_BIP:
         interleaveString = "bip";
         break;
      default:
         interleaveString = "Unknown";
         break;
   }
   m_keywords["interleave"] = interleaveString;
}

ossimString ossimEnviHeader::getSensorType() const
{
   return m_keywords["sensor type"];
}

void ossimEnviHeader::setSensorType(const ossimString& sensorType)
{
   m_keywords["sensor type"] = sensorType.c_str();
}

ossimByteOrder ossimEnviHeader::getByteOrder() const
{
   ossimByteOrder result = ossim::byteOrder(); // System byte order.
   std::string value = m_keywords["byte order"];
   if ( value.size() )
   {
      // 0 = LITTLE_ENDIAN
      // 1 = BIG_ENDIAN
      ossim_int32 order = ossimString(value).toInt32();
      result = order ? OSSIM_BIG_ENDIAN : OSSIM_LITTLE_ENDIAN;
   }
   return result;
}

void ossimEnviHeader::setByteorder(ossimByteOrder byteOrder)
{
   if(byteOrder==OSSIM_LITTLE_ENDIAN)
   {
      m_keywords["byte order"] = "0";
   }
   else
   {
      m_keywords["byte order"] = "1";
   }
}

ossim_int32 ossimEnviHeader::getXStart() const
{
   ossim_int32 result = 0;
   std::string s = m_keywords.findKey( std::string("x start") );
   if ( !s.size() )
   {
      s = m_keywords.findKey( std::string("sample start") );
   }
   if ( s.size() )
   {
      result = ossimString( s ).toInt32();
   }
   return result;
}

void ossimEnviHeader::setXStart(ossim_int32 xstart)
{
   m_keywords["x start"] = ossimString::toString(xstart).c_str();
}

ossim_int32 ossimEnviHeader::getYStart() const
{
   ossim_int32 result = 0;
   std::string s = m_keywords.findKey( std::string("y start") );
   if ( !s.size() )
   {
      s = m_keywords.findKey( std::string("line start") );
   }
   if ( s.size() )
   {
      result = ossimString( s ).toInt32();
   }
   return result;
}

void ossimEnviHeader::setYStart(ossim_int32 ystart)
{
   m_keywords["y start"] = ossimString::toString(ystart).c_str();
}

ossimString ossimEnviHeader::getMapInfo() const
{
   return m_keywords["map info"];
}

void ossimEnviHeader::setMapInfo(const ossimString& mapInfo)
{
   m_keywords["map info"] = mapInfo.c_str();
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
         << std::endl;
   }
   ossimString mapInfoString;
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
         << std::endl;
         
      return; // Have to have the projection type!
   }

   // Get the datum.
   ossimString datum = "WGS-84";
   lookup = kwl.find(prefix, ossimKeywordNames::DATUM_KW);
   if (lookup)
   {
      ossimString os = lookup;
      if (os == "WGE")
      {
         datum = "WGS-84";
      }
      else
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimEnviHeader::setMapInfo WARNING:"
            << "\nUnhandled ossim -> envi datum:  " << datum
            << "\nAssuming WGS-84!"
            << std::endl;
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
                 
      mapInfoString = "{Geographic Lat/Lon, 1.0000, 1.0000, ";
      mapInfoString += tieLon;
      mapInfoString += ", ";
      mapInfoString += tieLat;
      mapInfoString += ", ";
      mapInfoString += degLon;
      mapInfoString += ", ";
      mapInfoString += degLat;
      mapInfoString += ", ";
      mapInfoString += datum;
      mapInfoString += ", units=degrees}";
   }

   m_keywords["map info"] = mapInfoString.c_str();
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimEnviHeader::setMapInfo DEBUG:"
         << "\ntheMapInfo:  " << mapInfoString
         << std::endl;
   }
}

ossimString ossimEnviHeader::getWavelengthUnits() const
{
   return m_keywords["wavelength units"];
}

void ossimEnviHeader::setWavelengthUnits(const ossimString& waveLengthUnits)
{
   
   m_keywords["wavelength units"] = waveLengthUnits.c_str();
}

void ossimEnviHeader::getBandNames(std::vector<ossimString>& bandNames) const
{
   bandNames.clear();
   ossimString bandNamesString = m_keywords["band names"];
   bandNamesString.split(bandNames, ",");
}

void ossimEnviHeader::setBandNames(const std::vector<ossimString>& bandNames)
{
   ossimString value;
   value.join(bandNames, ",");
   m_keywords["band names"] = value.c_str();
}

void ossimEnviHeader::getWavelengths(std::vector<ossimString>& waveLengths)
   const
{
   waveLengths.clear();
   ossimString bandNamesString = m_keywords[WAVELENGTH_KW];
   bandNamesString.split(waveLengths, ",");
}

void ossimEnviHeader::setWavelengths(
   const std::vector<ossimString>& wavelengths)
{
   ossimString value;
   value.join(wavelengths, ",");
   m_keywords[WAVELENGTH_KW] = value.c_str();
}

bool ossimEnviHeader::loadState(const ossimKeywordlist& kwl,
                                const char* prefix)
{
   std::string lookup;
   std::string pfx = (prefix ? prefix: "" );
   ossimString s;

   reset();

   lookup = kwl.findKey(pfx, std::string(ossimKeywordNames::FILENAME_KW));
   if (lookup.size())
   {
      m_file.string() = lookup;
   }
  
   lookup = kwl.findKey(pfx, std::string(ossimKeywordNames::DESCRIPTION_KW));
   if (lookup.size())
   {
      setDescription(lookup);
   }

   lookup = kwl.findKey(pfx, std::string(ossimKeywordNames::NUMBER_SAMPLES_KW));
   if (lookup.size())
   {
      m_keywords["samples"] = lookup;
   }
   
   lookup = kwl.findKey(pfx, std::string(ossimKeywordNames::NUMBER_LINES_KW));
   if (lookup.size())
   {
      m_keywords["lines"] = lookup;
   }

   lookup = kwl.findKey(pfx, std::string(ossimKeywordNames::NUMBER_BANDS_KW));
   if (lookup.size())
   {
      m_keywords["bands"] = lookup;
   }

   lookup = kwl.findKey(pfx, std::string(HEADER_OFFSET_KW));
   if (lookup.size())
   {
      m_keywords["header offset"] = lookup;
   }

   lookup = kwl.findKey(pfx, std::string(FILE_TYPE_KW));
   if (lookup.size())
   {
       m_keywords["file_type"] = lookup;
   }

   lookup = kwl.findKey(pfx, DATA_TYPE_KW);
   if (lookup.size())
   {
      m_keywords["data type"] = lookup;
   }

   lookup = kwl.findKey(pfx, std::string(ossimKeywordNames::INTERLEAVE_TYPE_KW));
   if (lookup.size())
   {
      m_keywords["interleave"] = lookup;
   }

   lookup = kwl.findKey(pfx, std::string(SENSOR_TYPE_KW));
   if (lookup.size())
   {
      m_keywords["sensor type"] = lookup;
   }

   lookup = kwl.findKey(pfx, std::string(ossimKeywordNames::BYTE_ORDER_KW));
   if (lookup.size())
   {
      s = lookup;
      s.downcase();
      if (s == "little_endian")
      {
         m_keywords["byte order"] = "0";
      }
      else if (s == "big_endian")
      {
         m_keywords["byte order"] = "1";
      }
      else
      {
         m_keywords["byte order"] = lookup;
      }
   }
   
   lookup = kwl.findKey(pfx, std::string(X_START_KW));
   if (lookup.size())
   {
      m_keywords["x start"] = lookup;
   }
   lookup = kwl.findKey(pfx, std::string(Y_START_KW));
   if (lookup.size())
   {
      m_keywords["y start"] = lookup;
   }

   lookup = kwl.findKey(pfx, std::string(MAP_INFO_KW));
   if (lookup.size())
   {
      m_keywords["map info"] = lookup;
   }

   lookup = kwl.findKey(pfx, std::string(WAVELENGTH_UNITS_KW));
   if (lookup.size())
   {
      m_keywords["wavelength units"] = lookup;
   }

   ossim_uint32 n;
   ossim_uint32 count;
   const ossim_uint32 MAX_TRIES = 1024; // Avoid infinate loop.
   
   // Get the band names.
   n = kwl.numberOf(prefix, BAND_NAME_KW);
   if (n)
   {
      ossimString value = "";
      count = 0;
      while ( (count < n) || (count > MAX_TRIES) )
      {
         s = BAND_NAME_KW;
         s += ossimString::toString(count);
         lookup = kwl.findKey(pfx, s.string());
         if (lookup.size())
         {
            if(!value.empty())
            {
               value += ossimString(lookup);
            }
            else
            {
               value += (", " + ossimString(lookup));

            }
            
         }
         ++count;
      }
      m_keywords["band names"] = value.c_str();
   }
            
   // Get the band names.
   n = kwl.numberOf(prefix, WAVELENGTH_KW);
   if (n)
   {
      ossimString value;
      count = 0;
      while ( (count < n) || (count > MAX_TRIES) )
      {
         s = WAVELENGTH_KW;
         s += ossimString::toString(count);
         lookup = kwl.findKey(pfx, s.string());
         if (lookup.size())
         {
            if(!value.empty())
            {
               value += ossimString(lookup);
            }
            else
            {
               value += (", " + ossimString(lookup));
            }
         }
         ++count;
      }
      m_keywords["wavelength"] = value.c_str();
   }
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimEnviHeader::loadState DEUG\n";
      print(ossimNotify(ossimNotifyLevel_DEBUG));
   }
   
   return true;
}

bool ossimEnviHeader::isEnviHeader( const ossimFilename& file )
{
   bool result = false;
   std::ifstream in;
   in.open(file.c_str(), std::ios::in | std::ios::binary);
   if ( in.is_open() )
   {
      result = isEnviHeader( in );
      in.close();
   }
   return result;
}

bool ossimEnviHeader::isEnviHeader( std::istream& in )
{
   bool result = false;
   ossim::skipws(in);
   char eh[5];
   in.read(eh, 4);
   eh[4] = '\0';
   std::string s(eh);
   if ( s == "ENVI" )
   {
      result = true;
   }
   return result;
}

const ossimFilename& ossimEnviHeader::getFile() const
{
   return m_file;
}

bool ossimEnviHeader::rangeCheckBands( const std::vector<ossim_uint32>& bands ) const
{
   bool result = false;
   const ossim_uint32 INPUT_BANDS = getBands();
   if ( INPUT_BANDS )
   {
      result = true;
      std::vector<ossim_uint32>::const_iterator i = bands.begin();
      while ( i != bands.end() )
      {
         if ( (*i) >= INPUT_BANDS )
         {
            result = false;
            break;
         }
         ++i;
      }
   }
   return result;
}
