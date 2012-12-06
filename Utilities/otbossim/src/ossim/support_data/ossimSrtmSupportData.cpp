//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:
// 
// Support data class for a Shuttle Radar Topography Mission (SRTM) file.
//
//----------------------------------------------------------------------------
// $Id: ossimSrtmSupportData.cpp 21527 2012-08-26 16:50:49Z dburken $

#include <cmath>
#include <fstream>
#include <iostream>
#include <iomanip>

#include <ossim/support_data/ossimSrtmSupportData.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimDatumFactory.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimRegExp.h>
#include <ossim/base/ossimScalarTypeLut.h>
#include <ossim/base/ossimStreamFactoryRegistry.h>
#include <ossim/projection/ossimEquDistCylProjection.h>
#include <ossim/projection/ossimMapProjection.h>

// Static trace for debugging
static ossimTrace traceDebug("ossimSrtmSupportData:debug");

//---
// Start with the min and max at some default setting.
// Mt. Everest highest elev point on Earth 8850 meters.
//---
static const ossim_float64 DEFAULT_MIN = -8850.0;
static const ossim_float64 DEFAULT_MAX =  8850.0;


ossimSrtmSupportData::ossimSrtmSupportData()
   :
   theFile(),
   theNumberOfLines(0),
   theNumberOfSamples(0),
   theSouthwestLatitude(ossim::nan()),
   theSouthwestLongitude(ossim::nan()),
   theLatSpacing(ossim::nan()),
   theLonSpacing(ossim::nan()),
   theMinPixelValue(DEFAULT_MIN),
   theMaxPixelValue(DEFAULT_MAX),
   theScalarType(OSSIM_SCALAR_UNKNOWN)
{
}

ossimSrtmSupportData::~ossimSrtmSupportData()
{
}

bool ossimSrtmSupportData::setFilename(const ossimFilename& srtmFile,
                                       bool scanForMinMax)
{
   theFile = srtmFile;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimSrtmSupportData::setFilename: entered:"
         << "\nsrtmFile:  " << srtmFile
         << "\nscanForMinMax flag:  " << scanForMinMax
         << std::endl;
   }
   
   theFileStream =  ossimStreamFactoryRegistry::instance()->
      createNewIFStream(theFile,
                        std::ios_base::in | std::ios_base::binary);
   if (theFileStream.valid())
   {
      if(theFileStream->fail())
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << theFile << " does not exist: leaving ..." << std::endl;
         }
         clear();
         return false;
      }
   }
   else
   {
      return false;
   }

   // Start with default.
   theMinPixelValue = DEFAULT_MIN;
   theMaxPixelValue = DEFAULT_MAX;
   
   // See if we have an ossim metadata file to initialize from.
   bool outputOmd     = false;
   bool loadedFromOmd = false;
   
   ossimFilename omdFile = theFile;
   omdFile.setExtension(ossimString("omd"));
   if (omdFile.exists())
   {
      //---
      // The loadOmd is called instead of loadState so theFile is not
      // overwrote.
      //---
      ossimKeywordlist kwl(omdFile);
      loadedFromOmd = loadOmd(kwl);
   }

   if (!loadedFromOmd)
   {
      if (!setCornerPoints())
      {
         clear();
         return false;
      }
      if (!setSize())
      {
         clear();
         return false;
      }
      outputOmd = true;
   }

   if (scanForMinMax)
   {
      // These could have been picked up in the loadOmd.
      if ( (theMinPixelValue == DEFAULT_MIN) ||
           (theMaxPixelValue == DEFAULT_MAX) )
      {
         if ( computeMinMax() )
         {
            outputOmd = true;
         }
         else
         {
            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG)
                  << "Unable to compute min max: leaving ..." << std::endl;
            }
            clear();
            return false;
         }
      }
   }

   //---
   // NOTE:  The outputOmd flag should probably be set if !loadedFromOmd.
   // Leaving as is for now (only set if scanForMinMax).
   //---
   if (outputOmd)
   {
      ossimKeywordlist kwl;
      saveState(kwl);
      kwl.write(omdFile);
   }

   if(theFileStream->is_open())
   {
      theFileStream->close();
   }
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << *this << std::endl;
   }
   
   return true;
}

ossimFilename ossimSrtmSupportData::getFilename() const
{
   return theFile;
}

ossim_uint32 ossimSrtmSupportData::getNumberOfLines() const
{
   return theNumberOfLines;
}

ossim_uint32 ossimSrtmSupportData::getNumberOfSamples() const
{
   return theNumberOfSamples;
}

bool ossimSrtmSupportData::getImageGeometry(ossimKeywordlist& kwl,
                                            const char* prefix)
{
   if (theFile == ossimFilename::NIL)
   {
      return false;
   }
   
   kwl.add(prefix,
           ossimKeywordNames::TYPE_KW,
           "ossimEquDistCylProjection",
           true);

   kwl.add(prefix,
           ossimKeywordNames::ORIGIN_LATITUDE_KW,
           0.0,
           true);
   
   kwl.add(prefix,
           ossimKeywordNames::CENTRAL_MERIDIAN_KW,
           theSouthwestLongitude,
           true);
   
   kwl.add(prefix,
           ossimKeywordNames::TIE_POINT_LAT_KW,
           (theSouthwestLatitude+1.0),
           true);

   kwl.add(prefix,
           ossimKeywordNames::TIE_POINT_LON_KW,
           theSouthwestLongitude,
           true);

   // Add the pixel scale.
   kwl.add(prefix,
           ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LAT,
           theLatSpacing,
           true);
   
   kwl.add(prefix,
           ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LON,
           theLonSpacing,
           true);

   // Add the datum.  (always WGS-84 per spec)
   kwl.add(prefix,
           ossimKeywordNames::DATUM_KW,
           ossimDatumFactory::instance()->wgs84()->code(),
           true);

   // Add the number of lines and samples.
   kwl.add(prefix,
           ossimKeywordNames::NUMBER_LINES_KW,
           theNumberOfLines,
           true);

   kwl.add(prefix,
           ossimKeywordNames::NUMBER_SAMPLES_KW,
           theNumberOfSamples,
           true);
   
   return true;
}

ossimRefPtr<ossimProjection> ossimSrtmSupportData::getProjection() const
{
   //---
   // Make an Equidistant Cylindrical projection with a origin at the equator
   // since the DTED post spacing is considered to be square.
   //---
   const ossimDatum* datum = ossimDatumFactory::instance()->wgs84();
   ossimRefPtr<ossimEquDistCylProjection> eq = new ossimEquDistCylProjection(*(datum->ellipsoid()));
   
   //---
   // Set the tie point.
   // NOTE: Latitude southwest corner we need northwest; hence, the +1.
   //---
   eq->setUlTiePoints( ossimGpt(theSouthwestLatitude+1.0, theSouthwestLongitude, 0.0, datum) );
   
   // Set the scale:
   eq->setDecimalDegreesPerPixel( ossimDpt(theLonSpacing, theLatSpacing) );

   ossimRefPtr<ossimProjection> proj = eq.get();

   return proj;
}

bool ossimSrtmSupportData::saveState(ossimKeywordlist& kwl,
                                     const char* prefix) const
{
   if (theFile == ossimFilename::NIL)
   {
      return false;
   }
   
   ossimString bandPrefix;
   if (prefix)
   {
      bandPrefix = prefix;
   }
   bandPrefix += "band1.";
   
   kwl.add(prefix,
           ossimKeywordNames::FILENAME_KW,
           theFile.c_str(),
           true);

   kwl.add(prefix,
           ossimKeywordNames::NUMBER_BANDS_KW,
           1,
           true);

   kwl.add(prefix,
           ossimKeywordNames::NUMBER_LINES_KW,
           theNumberOfLines,
           true);
   
   kwl.add(prefix,
           ossimKeywordNames::NUMBER_SAMPLES_KW,
           theNumberOfSamples,
           true);

   //---
   // Special case, store the tie point as the upper left lat so add one.
   //---
   kwl.add(prefix,
           ossimKeywordNames::TIE_POINT_LAT_KW,
           (theSouthwestLatitude + 1.0),
           true);

   kwl.add(prefix,
           ossimKeywordNames::TIE_POINT_LON_KW,
           theSouthwestLongitude,
           true);

   kwl.add(prefix,
           ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LAT,
           theLatSpacing,
           true);
   
   kwl.add(prefix,
           ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LON,
           theLonSpacing,
           true);

   // User can opt out of scanning for this so don't save if it is still nan.
   if (theMinPixelValue != DEFAULT_MIN)
   {
      kwl.add(bandPrefix,
              ossimKeywordNames::MIN_VALUE_KW,
              theMinPixelValue,
              true);
   }

   // User can opt out of scanning for this so don't save if it is still nan.
   if (theMaxPixelValue != DEFAULT_MAX)
   {
      kwl.add(bandPrefix.c_str(),
              ossimKeywordNames::MAX_VALUE_KW,
              theMaxPixelValue,
              true);
   }

   // constant
   kwl.add(bandPrefix,
           ossimKeywordNames::NULL_VALUE_KW,
           "-32768",
           true);

   // constant
   kwl.add(prefix,
           ossimKeywordNames::BYTE_ORDER_KW,
           "big_endian",
           true);

   // constant
   kwl.add(prefix,
           ossimKeywordNames::SCALAR_TYPE_KW,
           ossimScalarTypeLut::instance()->getEntryString(theScalarType),
           true);

   return true;
}
   
bool ossimSrtmSupportData::loadState(const ossimKeywordlist& kwl,
                                     const char* prefix)
{
   ossimString bandPrefix;
   if (prefix)
   {
      bandPrefix = prefix;
   }
   bandPrefix += "band1.";
   
   ossimString s; // For numeric conversions.
   
   const char* lookup;

   // Look for "filename" then look for deprecated "image_file" next.
   lookup = kwl.find(prefix, ossimKeywordNames::FILENAME_KW);
   if (lookup)
   {
      theFile = lookup;
   }
   else
   {
      // Deprecated...
      lookup = kwl.find(prefix, ossimKeywordNames::IMAGE_FILE_KW);
      if (lookup)
      {
         theFile = lookup;
      }
      else
      {
         return false;
      }
   }

   return loadOmd(kwl, prefix);
}

bool ossimSrtmSupportData::loadOmd(const ossimKeywordlist& kwl,
                                   const char* prefix)
{
   std::string pfx;
   std::string bandPrefix;
   
   if (prefix) // Cannot give null to std::string.
   {
      pfx        = prefix;
      bandPrefix = prefix;
   }
   bandPrefix += "band1.";
   
   ossimString value;

   //---
   // Look for the min and max first since they could have been populated by
   // "cmm" (compute min max).  Min and Max are not required by this method
   // as they are handled elsewhere if they are not found here.
   //---
   
   // Not an error if not present.
   value.string() = kwl.findKey(bandPrefix, std::string(ossimKeywordNames::MIN_VALUE_KW));
   if ( value.size() )
   {
      theMinPixelValue = value.toFloat64();
   }
   
   // Not an error if not present.
   value.string() = kwl.findKey(bandPrefix.c_str(), std::string(ossimKeywordNames::MAX_VALUE_KW));
   if ( value.size() )
   {
      theMaxPixelValue = value.toFloat64();
   }
   
   value.string() = kwl.findKey(pfx, std::string(ossimKeywordNames::NUMBER_LINES_KW));
   if ( value.size() )
   {
      theNumberOfLines = value.toUInt32();
   }
   else
   {
      return false;
   }
   
   value.string() = kwl.findKey(pfx, std::string(ossimKeywordNames::NUMBER_SAMPLES_KW));
   if ( value.size() )
   {
      theNumberOfSamples = value.toUInt32();
   }
   else
   {
      return false;
   }

   //---
   // Special case the tie point was stored as the upper left so we must
   // subtract one.
   //---
   value.string() = kwl.findKey(pfx, std::string(ossimKeywordNames::TIE_POINT_LAT_KW));
   if ( value.size() )
   {
      theSouthwestLatitude = value.toFloat64() - 1.0;
   }
   else
   {
      return false;
   }

   value.string() = kwl.findKey(pfx, std::string(ossimKeywordNames::TIE_POINT_LON_KW));
   if ( value.size() )
   {
      theSouthwestLongitude = value.toFloat64();
   }
   else
   {
      return false;
   }
   
   int scalar = ossimScalarTypeLut::instance()->getEntryNumber(kwl, prefix);
   
   if (scalar != ossimLookUpTable::NOT_FOUND)
   {
      theScalarType = (ossimScalarType)scalar;
      if((theScalarType != OSSIM_FLOAT32) && (theScalarType != OSSIM_SINT16))
      {
         return false;
      }
   }
   else
   {
      return false;
   }
   
   theLatSpacing = 1.0 / (theNumberOfLines   - 1);
   theLonSpacing = 1.0 / (theNumberOfSamples - 1);
   
   return true;
}

ossim_float64 ossimSrtmSupportData::getSouthwestLatitude() const
{
   return theSouthwestLatitude;
}

ossim_float64 ossimSrtmSupportData::getSouthwestLongitude() const
{
   return theSouthwestLongitude;
}
ossim_float64 ossimSrtmSupportData::getLatitudeSpacing() const
{
   return theLatSpacing;
}

ossim_float64 ossimSrtmSupportData::getLongitudeSpacing() const
{
   return theLonSpacing;
}

void ossimSrtmSupportData::clear()
{
   theFile               = ossimFilename::NIL;
   theNumberOfLines      = 0;
   theNumberOfSamples    = 0;
   theSouthwestLatitude  = ossim::nan();
   theSouthwestLongitude = ossim::nan();
   theLatSpacing         = ossim::nan();
   theLonSpacing         = ossim::nan();
   theMinPixelValue      = DEFAULT_MIN;
   theMaxPixelValue      = DEFAULT_MAX;
}

bool ossimSrtmSupportData::setCornerPoints()
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimSrtmSupportData::setCornerPoints(): entered..." << std::endl;
   }
   
   ossimFilename f = theFile.fileNoExtension();
   ossimString regularExp1 = "[N|S][0-9][0-9][E|W][0-9][0-9][0-9]";
   ossimString regularExp2 = "[E|W][0-9][0-9][0-9][N|S][0-9][0-9]";
   ossimRegExp regEx;
   bool latLonOrderFlag = true;
   bool foundFlag = false;
   f = f.upcase();

   regEx.compile(regularExp1.c_str());
   foundFlag = regEx.find(f.c_str());
   if(!foundFlag)
   {
      regEx.compile(regularExp2.c_str());
      foundFlag = regEx.find(f.c_str());
      if(foundFlag)
      {
         latLonOrderFlag = false;
         f = ossimFilename(ossimString(f.begin()+regEx.start(),
                                       f.begin()+regEx.end()));
      }
   }
   if(foundFlag)
   {
      f = ossimFilename(ossimString(f.begin()+regEx.start(),
                                    f.begin()+regEx.end()));
   }
   if (f.size() != 7)
   {
      return false;
   }
//    ossimString s;

   if(latLonOrderFlag)
   {
      
//       s.push_back(f[1]);
//       s.push_back(f[2]);
      theSouthwestLatitude = ossimString(f.begin()+1,
                                         f.begin()+3).toDouble();//s.toDouble();
      // Get the latitude.
      if (f[static_cast<std::string::size_type>(0)] == 'S')
      {
         theSouthwestLatitude *= -1;
      }
      else if (f[static_cast<std::string::size_type>(0)] != 'N')
      {
         return false; // Must be either 's' or 'n'.
      }
      // Get the longitude.
//       s.clear();
//       s.push_back(f[4]);
//       s.push_back(f[5]);
//       s.push_back(f[6]);
      theSouthwestLongitude = ossimString(f.begin()+4,
                                          f.begin()+7).toDouble();//s.toDouble();
      if (f[static_cast<std::string::size_type>(3)] == 'W')
      {
      theSouthwestLongitude *= -1;
      }
      else if (f[static_cast<std::string::size_type>(3)] != 'E')
      {
         return false; // Must be either 'e' or 'w'.
      }
   }
   else
   {
      // Get the longitude.
//       s.clear();
//       s.push_back(f[1]);
//       s.push_back(f[2]);
//       s.push_back(f[3]);
      theSouthwestLongitude =  ossimString(f.begin()+1,
                                           f.begin()+4).toDouble();//s.toDouble();
      if (f[static_cast<std::string::size_type>(0)] == 'W')
      {
      theSouthwestLongitude *= -1;
      }
      else if (f[static_cast<std::string::size_type>(0)] != 'E')
      {
         return false; // Must be either 'e' or 'w'.
      }
//       s.clear();
      
//       s.push_back(f[5]);
//       s.push_back(f[6]);
      theSouthwestLatitude = ossimString(f.begin()+5,
                                         f.begin()+7).toDouble();//s.toDouble();
      // Get the latitude.
      if (f[static_cast<std::string::size_type>(4)] == 'S')
      {
         theSouthwestLatitude *= -1;
      }
      else if (f[static_cast<std::string::size_type>(4)] != 'N')
      {
         return false; // Must be either 's' or 'n'.
      }
   }
   

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimSrtmSupportData::setCornerPoints(): leaving with true..."
         << std::endl;
   }
   return true;
}

bool ossimSrtmSupportData::setSize()
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimSrtmSupportData::setSize(): entered..." << std::endl;
   }

   if(theFileStream->is_open() == false)
   {
      theFileStream =
         ossimStreamFactoryRegistry::instance()->createNewIFStream(
            theFile,
            std::ios_base::in | std::ios_base::binary);
   }
   
   if (!theFileStream.valid())
   {
      return false;
   }

   if(theFileStream->fail())
   {
      return false;
   }
   
   ossim_float64 size = 0.0;
   theFileStream->seekg(0, std::ios::beg);
   if(theFileStream->isCompressed())
   {
      ossimFilename tmp = theFile;
      tmp.setExtension("hgt");
      if(!tmp.exists())
      {
         ossimOFStream out(tmp.c_str(),
                           std::ios::out|std::ios::binary);

         if(!out.fail())
         {
            bool done = false;
            char buf[1024];
            while(!done&&!theFileStream->fail())
            {
               theFileStream->read(buf, 1024);
               if(theFileStream->gcount() < 1024)
               {
                  done = true;
               }
               if(theFileStream->gcount() > 0)
               {
                  out.write(buf, theFileStream->gcount());
               }
            }
            out.close();
            size = tmp.fileSize();
            tmp.remove();
         }
      }
   }
   else
   {
      size = theFile.fileSize();
   }
   if (!size)
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimSrtmSupportData::setSize(): leaving with false at line "
            << __LINE__ << std::endl;
      }
      return false;
   }

   theScalarType = OSSIM_SCALAR_UNKNOWN;
   
   //---
   // Assuming square for now.  Have to check the spec for this.
   //---
   if (size == 25934402) // 2 * 3601 * 3601 three arc second
   {
      theNumberOfLines     = 3601;
      theNumberOfSamples   = 3601;
      theScalarType = OSSIM_SINT16;
   }
   else if(size == 51868804) // 4*3601*3601
   {
      theNumberOfLines   = 3601;
      theNumberOfSamples = 3601;
      theScalarType = OSSIM_FLOAT32;
   }
   else if (size == 2884802) // 2 * 1201 * 1201 one arc second
   {
      theNumberOfLines   = 1201;
      theNumberOfSamples = 1201;
      theScalarType = OSSIM_SINT16;
   }
   else if (size == 5769604)
   {
      theNumberOfLines   = 1201;
      theNumberOfSamples = 1201;
      theScalarType = OSSIM_FLOAT32;
   }
   else // try to get a square width and height
   {
      ossim_uint64 lines   = (ossim_uint64)sqrt((ossim_float64)(size / 2));
      ossim_uint64 samples = (ossim_uint64)sqrt((ossim_float64)(size / 2));
      // check square
      if(lines*samples*2 == size)
      {
         theNumberOfLines   = lines;
         theNumberOfSamples = samples;
         theScalarType = OSSIM_SINT16;
         
      }
      else
      {
         ossim_uint64 lines   = (ossim_uint64)sqrt((ossim_float64)(size / 4));
         ossim_uint64 samples = (ossim_uint64)sqrt((ossim_float64)(size / 4));
         // check square
         if(lines*samples*4 == size)
         {
            theNumberOfLines   = lines;
            theNumberOfSamples = samples;
            theScalarType = OSSIM_FLOAT32;
         }
         else
         {
            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG)
                  << "ossimSrtmSupportData::setSize(): leaving with false at line " << __LINE__ << std::endl;
            }
            return false;
         }
      }
   }
      
   theLatSpacing      = 1.0 / (theNumberOfLines   - 1);
   theLonSpacing      = 1.0 / (theNumberOfSamples - 1);
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimSrtmSupportData::setSize(): leaving with true..."
         << std::endl;
   }

   theFileStream->close();
   
   return true;
}

bool ossimSrtmSupportData::computeMinMax()
{
   if(theScalarType == OSSIM_FLOAT32)
   {
      return computeMinMaxTemplate((ossim_float32)0,
                                   -32768.0);
   }
   return computeMinMaxTemplate((ossim_sint16)0,
                                -32768.0);
}

template <class T>
bool ossimSrtmSupportData::computeMinMaxTemplate(T /* dummy */,
                                                 double defaultNull)
{
   if(theFileStream->is_open() == false)
   {
      theFileStream =
         ossimStreamFactoryRegistry::instance()->createNewIFStream(
            theFile,
            std::ios_base::in | std::ios_base::binary);
   }
   
   if (!theFileStream.valid())
   {
      return false;
   }

   if(theFileStream->fail())
   {
      return false;
   }
   
   const size_t BYTES_IN_LINE = theNumberOfSamples * 2;
   const T NULL_PIX = (T)defaultNull;

   double minValue = 1.0/FLT_EPSILON;
   double maxValue = -1.0/FLT_EPSILON;
   T* line_buf = new T[theNumberOfSamples];
   char* char_buf = (char*)line_buf;
   ossimEndian swapper;

   ossimByteOrder endianType = ossim::byteOrder();
   for (ossim_uint32 line = 0; line < theNumberOfLines; ++line)
   {
      theFileStream->read(char_buf, (std::streamsize)BYTES_IN_LINE);
      if(endianType == OSSIM_LITTLE_ENDIAN)
      {
         swapper.swap(line_buf, theNumberOfSamples);
      }
      for (ossim_uint32 sample = 0; sample < theNumberOfSamples; ++sample)
      {
         if (line_buf[sample] == NULL_PIX) continue;
         if (line_buf[sample] > maxValue) maxValue = line_buf[sample];
         if (line_buf[sample] < minValue) minValue = line_buf[sample];
      }
   }
   delete [] line_buf;
   theMinPixelValue = minValue;
   theMaxPixelValue = maxValue;

   theFileStream->close();

   return true;
}

ossim_float64 ossimSrtmSupportData::getMinPixelValue() const
{
   return theMinPixelValue;
}

ossim_float64 ossimSrtmSupportData::getMaxPixelValue() const
{
   return theMaxPixelValue;
}

ossimScalarType ossimSrtmSupportData::getScalarType()const
{
   return theScalarType;
}

std::ostream& ossimSrtmSupportData::print(std::ostream& out) const
{
   out << std::setprecision(15) << "ossimSrtmSupportData data members:"
       << "\nFile:                  " << theFile
       << "\nLines:                 " << theNumberOfLines
       << "\nSamples:               " << theNumberOfSamples
       << "\nSouth West Latitude:   " << theSouthwestLatitude
       << "\nSouth West Longitude:  " << theSouthwestLongitude
       << "\nLatitude spacing:      " << theLatSpacing
       << "\nLongitude spacing:     " << theLonSpacing
       << "\nMin post value:        " << theMinPixelValue
       << "\nMax post value:        " << theMaxPixelValue
       << std::endl;
   return out;
}


