//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:
// 
// Class for reading and writing an ENVI (The Environment for Visualizing
// Images) header file.  This parses envi header and places in a keyword
// list.
//
//----------------------------------------------------------------------------
// $Id: ossimEnviHeader.h 22349 2013-08-01 21:38:29Z dburken $

#ifndef ossimEnviHeader_HEADER
#define ossimEnviHeader_HEADER 1

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimReferenced.h>
#include <iosfwd>

/**
 * Class for reading and writing an ENVI (The Environment for Visualizing
 * Images) header file.
 */
class OSSIM_DLL ossimEnviHeader : public ossimReferenced
{
public:
   /** default construtor */
   ossimEnviHeader();

   /** virtual destructor */
   virtual ~ossimEnviHeader();

   virtual void reset();

   /** @return Const reference to map. */
   const ossimKeywordlist& getMap() const;
   
   /** @return Reference to map. */
   ossimKeywordlist& getMap();

   /**
    * @brief Gets value for key.
    * @param key To search for.
    * @param value Initialized by this.
    * @return true on success, false on error. This will return true if key is
    * found, even if value is empty.
    */
   bool getValue( const ossimString& key, ossimString& value ) const;

   /**
    * @brief Gets value for key.
    * @param key
    * @param value
    * @return true if key is in map even if value is empty; false, if not.
    */
   bool findCaseInsensitive( const ossimString& key,
                             ossimString& value ) const;

   /**
    * @brief Gets value for key.
    * @param key
    * @param value
    * @return true if key is in map even if value is empty; false, if not.
    */
   bool findSubStringCaseInsensitive( const ossimString& key,
                                      ossimString& value) const;

   /**
    * Opens an envi header.
    * 
    * @return true on success, false on error.
    */
   bool open(const ossimFilename& file);

   /**
    * Writes header to file in a standard envi format.
    *
    * @param file File to write to.
    *
    * @return true on success, false on error.
    */
   bool writeFile(const ossimFilename& file);
   
   /**
    * Prints header to out in a standard envi format.
    *
    * @param out Stream to write to.
    *
    * @return Reference to the stream passed.
    */
   std::ostream& print(std::ostream& out) const;

   /** @brief friend operator<< */
   friend OSSIM_DLL std::ostream& operator<<(std::ostream& out,
                                             const ossimEnviHeader& obj);

   /**
    * @return The description of the file.
    */
   ossimString getDescription() const;

   /**
    * @param description The description of the file.
    */
   void setDescription(const ossimString& description);

   /**
    * @return The number of samples.
    */
   ossim_uint32 getSamples() const;

   /**
    * Sets the number of samples.
    * 
    * @param samples The number of samples.
    */
   void setSamples(ossim_uint32 samples);

   /**
    * @return The number of lines.
    */
   ossim_uint32 getLines() const;

   /**
    * Sets the number of lines.
    * 
    * @param lines The number of lines.
    */
   void setLines(ossim_uint32 lines);

   /**
    * @return The number of bands.
    */
   ossim_uint32 getBands() const;

   /**
    * Sets the number of bands.
    * 
    * @param bands The number of bands.
    */
   void setBands(ossim_uint32 bands);

   /**
    * @brief Gets default bands if "default bands" keyword is present.
    * @param bands Initialized by this.
    * @return true on success, false if keyword not found. "bands" will be
    * zeroed out on failure. 
    */
   bool getDefaultBands( std::vector<ossim_uint32>& bands ) const;

   /**
    * @brief Gets rgb bands if "wavelength" keyword is present.
    * @param bands Initialized by this.
    * @return true on success, false if keyword not found.  "bands" will be
    * zeroed out on failure.
    */   
   bool getRgbBandsFromWaveLength( std::vector<ossim_uint32>& bands ) const;

   /**
    * @return The number of header offset in bytes.
    */
   ossim_uint32 getHeaderOffset() const;

   /**
    * Sets the header offset in bytes.
    * 
    * @param headerOffset The number of header offset in bytes.
    */
   void setHeaderOffset(ossim_uint32 headerOffset);

   /**
    * @return The file type
    */
   ossimString getFileType() const;

   /**
    * Sets the file type.
    * 
    * @param fileType The sensor type as a string.
    */
   void setFileType(const ossimString& fileType);

   /**
    * @return The envi data type..
    */
   ossim_uint32 getDataType() const;

   /**
    * @return The ossimScalarType from the envi data type..
    */
   ossimScalarType getOssimScalarType() const;

   /**
    * Sets the envi data type based on the ossimScalarType.
    * 
    * @param scalar The ossimScalarType of the image.
    */
   void setDataType(ossimScalarType scalar);

   /**
    * @return The envi interleave type.
    */
   ossimString getInterleaveType() const;

   /**
    * @return The ossimInterleaveType from the envi interleave..
    */
   ossimInterleaveType getOssimInterleaveType() const;

   /**
    * Sets the envi interleave type string based on the ossimInterleaveType.
    * 
    * @param interleave The ossimInterleaveType of the image.
    */
   void setInterleaveType(ossimInterleaveType interleave);

   /**
    * @return The sensor type..
    */
   ossimString getSensorType() const;

   /**
    * Sets the envi sensor type string.
    * 
    * @param sensorType The sensor type as a string.
    */
   void setSensorType(const ossimString& sensorType);

   /**
    * @return If key "byte order" found returns the envi byte order; else,
    * system byte order.  
    *
    * @note (Same as the ossimByteOrder enumeration):
    * 0 = LITTLE_ENDIAN,
    * 1 = BIG_ENDIAN
    */
   ossimByteOrder getByteOrder() const;
   
   /**
    * Sets the envi byte order from the ossimByteOrder.
    * 
    * @param byteorder The ossimByteOrder of the image.
    */
   void setByteorder(ossimByteOrder byteOrder);
   
   /**
    * @return The x start.
    */
   ossim_int32 getXStart() const;
   
   /**
    * Sets the x start.
    * 
    * @param xStart
    */
   void setXStart(ossim_int32 xstart);
   
   /**
    * @return The x start.
    */
   ossim_int32 getYStart() const;
   
   /**
    * Sets the y start.
    * 
    * @param ystart
    */
   void setYStart(ossim_int32 ystart);

   /**
    * @return The envi map info string.
    */
   ossimString getMapInfo() const;

   /**
    * Sets the envi map info string.
    * 
    * @param mapInfo envi map info string.
    */
   void setMapInfo(const ossimString& mapInfo);

   /**
    * Sets the envi map info string from a keyword list containing geometry
    * information.
    * 
    * @param kwl Keyword list containing geometry information.
    */
   void setMapInfo(const ossimKeywordlist& kwl,  const char* prefix=0);

   /**
    * @return The env wavelength units..
    */
   ossimString getWavelengthUnits() const;

   /**
    * Sets the envi wavelength units string.
    * 
    * @param wavelengthUnits envi wavelength units string.
    */
   void setWavelengthUnits(const ossimString& wavelenghtUnits);

   /**
    * @param bandNames Vector of strings to initialize with band names.
    */
   void getBandNames(std::vector<ossimString>& bandNames) const;

   /**
    * Sets the band name string vector.
    * 
    * @param bandNames Vector of band name strings.
    */
   void setBandNames(const std::vector<ossimString>& bandNames);

   /**
    * @param wavelengths Vector of strings to initialize with wave lengths.
    */
   void getWavelengths(std::vector<ossimString>& wavelengths) const;

   /**
    * Sets the envi band name string.
    * 
    * @param wavelengths Vector of band name strings.
    */
   void setWavelengths(const std::vector<ossimString>& wavelengths);

   /**
    * Method to the load (recreate) the state of the object from a keyword
    * list.
    *
    * @param kwl Keyword list to initialize from.
    *
    * @param prefix Usually something like: "object1."
    *
    * @return This method will alway return true as it is intended to be
    * used in conjuction with the set methods.
    *
    * Keywords picked up by loadState:
    * 
    * description: My_file
    *
    * number_samples: 1024
    *
    * number_lines: 512
    *
    * number_bands: 3
    *
    * header_offset: 0
    *
    * file_type: ENVI Standard
    *
    * data_type: 1
    *
    * interleave_type: bil
    *
    * sensor_type:  Unknown
    *
    * (little_endian or big_endian)
    * byte_order: little_endian
    * 
    * map_info: Unkown
    *
    * wavelength_units: 
    *
    * band_name0: Red band
    * band_name1: Green band
    * band_name2: Blue band
    *
    * wavelength0:  620.25
    * wavelength1:  514.5
    * wavelength2:  470.0
    */
   bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);

   /**
    * @brief Global method to test first line of file for "ENVI".
    * @return true on success, false on error.
    */
   static bool isEnviHeader( const ossimFilename& file );

   /**
    * @brief Global method to test first line of stream for "ENVI".
    * @return true on success, false on error.
    */
   static bool isEnviHeader( std::istream& in );

   /** @return Path to envi header file. */
   const ossimFilename& getFile() const;
   
private:

   /**
    * @brief Check band list to see if any are outside of range of bands.
    * Bands should be zero base.
    * @return true if all bands are less than number of bands; false, if
    * outside range or if "bands" key is not found.
    */
   bool rangeCheckBands( const std::vector<ossim_uint32>& bands ) const;

   /**
    * @brief Parses stream.
    * @return true on success, false on error.
    */
   bool readStream(std::istream& in);
   
   ossimFilename       m_file; // Name of header file.
   ossimKeywordlist    m_keywords;
};
#endif /* #ifndef ossimEnviHeader_HEADER */

