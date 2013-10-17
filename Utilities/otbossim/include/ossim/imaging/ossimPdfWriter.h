//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file
//
// Author:  David Burken
//
// Description: OSSIM Portable Document Format (PDF) writer.
//
//----------------------------------------------------------------------------
// $Id$

#ifndef ossimPdfWriter_HEADER
#define ossimPdfWriter_HEADER 1

#include <ossim/imaging/ossimImageFileWriter.h>
#include <ossim/base/ossimConnectableObject.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimRtti.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/imaging/ossimImageSource.h>
#include <OpenThreads/Mutex>
#include <iosfwd>

// Forward class declarations:
class ossimImageData;
class ossimIpt;
class ossimIrect;

/**
 * @class ossimPdfWriter
 *
 * References:
 * 
 * Adobe PDF Reference sixth edition, Version 1.7 November 2006
 * pdf_reference_1-7.pdf
 *
 * PDF Georegistration Encoding Best Practice Version (2.2 OGC 08-139r3)
 * 08-139r3_PDF_Geo-registration_Encoding_Best_Practice_Version_2.2.pdf
 *
 * @note Spaces and newlines for formatting have been left in the code for
 * sanity purposes only.  Code is heavily traced.  You can enable trace on any
 * ossim application with "-T ossimPdfWriter".
 */
class OSSIM_DLL ossimPdfWriter : public ossimImageFileWriter
{
public:

   enum ossimPdfImageType
   {
      UNKNOWN = 0,
      JPEG    = 1, // JPEG
      RAW     = 2  // general raster, band interleaved by pixel(BIP)
   };

   /** @brief default constructor */
   ossimPdfWriter();

   /** @brief virtual destructor */
   virtual ~ossimPdfWriter();

   /** @return "pdf writer" */
   virtual ossimString getShortName() const;

   /** @return "ossim pdf writer" */
   virtual ossimString getLongName()  const;

   /** @return "ossimPdfWriter" */
   virtual ossimString getClassName()    const;

   /**
    * @brief Returns a 3-letter extension from the image type descriptor
    * (theOutputImageType) that can be used for image file extensions.
    *
    * @param imageType string representing image type.
    *
    * @return the 3-letter string extension.
    */
   virtual ossimString getExtension() const;

   /**
    * @brief void getImageTypeList(std::vector<ossimString>& imageTypeList)const
    *
    * Appends this writer image types to list "imageTypeList".
    *
    * This writer only has one type "pdf".
    *
    * @param imageTypeList stl::vector<ossimString> list to append to.
    */
   virtual void getImageTypeList(std::vector<ossimString>& imageTypeList)const;

   /** @return true if open, false if not. */
   virtual bool isOpen() const;

   /**
    * @brief Open method.
    * @return True on success, false on error.
    */
   virtual bool open();
   
   /** @brief Closes file. */
   virtual void close();
   
   /** @brief saves the state of the object. */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
   
   /**
    * @brief Method to the load (recreate) the state of an object from a keyword
    * list.
    * @return True on success, false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   /**
    * @brief Will set the property whose name matches the argument
    * "property->getName()".
    *
    * @param property Object containing property to set.
    */
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   
   /**
    * @brief Gets property for name.
    * 
    * @param name Name of property to return.
    * 
    * @returns A pointer to a property object which matches "name".
    */
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;

   /**
    * @brief Pushes this's names onto the list of property names.
    *
    * @param propertyNames array to add this's property names to.
    */
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

   virtual bool hasImageType(const ossimString& imageType) const;

   /**
    * @brief Method to write the image to a stream.
    *
    * @return true on success, false on error.
    */
   virtual bool writeStream();

   /**
    * @brief Sets the output stream to write to.
    *
    * The stream will not be closed/deleted by this object.
    *
    * @param output The stream to write to.
    */
   virtual bool setOutputStream( std::ostream& stream );

private:

   /**
    * @brief Writes the file to disk or a stream.
    * @return true on success, false on error.
    */
   virtual bool writeFile();

   /**
    * @brief Writes header.
    * @param str Stream to write to.
    */
   void writeHeader( std::ostream* str );

   /**
    * @brief Writes Catalog object.
    * @param str Stream to write to.
    * @param xref To capture object positions.
    */
   void writeCatalog( std::ostream* str,
                      std::vector<std::streamoff>& xref );
   
   /**
    * @brief Writes Outlines object.
    * @param str Stream to write to.
    * @param xref To capture object positions.
    */
#if 0
   void writeOutlines( std::ostream* str,
                       std::vector<std::streamoff>& xref );
#endif

   /**
    * @brief Writes Pages object.
    * @param str Stream to write to.
    * @param xref To capture object positions.
    */
   void writePages( std::ostream* str,
                    std::vector<std::streamoff>& xref );

   /**
    * @brief Writes document infomation dictionary.
    * @param str Stream to write to.
    * @param xrefOffset Offset to cross reference(xref) table. 
    */
   void writeInfo( std::ostream* str,
                   std::vector<std::streamoff>& xref );
   
   /**
    * @brief Writes the image dictionary and associated objects.
    * @param str Stream to write to.
    * @return true on success, false on error.
    */
   bool writeImage( std::ostream* str );

   /**
    * @brief Write a JPEG-compresses the raster tile buffer to the output stream
    * provided.
    * @param str Stream to write to.
    * @param xref To capture object positions.
    * @param id Pointer to tile data.
    * @param aoi Area of interest.
    * @param outputTileRect Initialized by this with output tile rect clamped
    * to aoi.
    */
   void writeJpegTile( std::ostream* str,
                       std::vector<std::streamoff>& xref,
                       ossimImageData* id,
                       const ossimIrect& aoi,
                       ossimIrect& outputTileRect );
   
   /**
    * @brief Write raw raster tile buffer to the output stream
    * provided.
    * @param str Stream to write to.
    * @param xref To capture object positions.
    * @param id Pointer to tile data.
    * @param aoi Area of interest.
    * @param outputTileRect Initialized by this with output tile rect clamped
    * to aoi.
    */
   void writeRawTile( std::ostream* str,
                      std::vector<std::streamoff>& xref,
                      ossimImageData* id,
                      const ossimIrect& aoi,
                      ossimIrect& outputTileRect );
   
   /**
    * @brief Writes Cross Reference(xref) section.
    * @param str Stream to write to.
    * @param xref Array of object offsets.
    */
   void writeXref( std::ostream* str,
                   const std::vector<std::streamoff>& xref );

   /**
    * @brief Writes trailer.
    * @param str Stream to write to.
    * @param entrySize Number of entries, one plus object count.
    * @param xrefOffset Offset to cross reference(xref) table. 
    */
   void writeTrailer( std::ostream* str,
                      ossim_uint32 entrySize,
                      std::streamoff xrefOffset );

   /**
    * @brief Writes "dummy"  or blank object just to keep object references
    * in line.
    * @param str Stream to write to.
    * @param xref To capture object positions.
    */
#if 0
   void writeDummy( std::ostream* str,
                    std::vector<std::streamoff>& xref );
#endif
   
   /**
    * @brief Sets up input image source.
    * This sets up the chain fed to theInputConnection which is an image
    * source sequencer.  This will remap input to eight bit if not already and
    * make input one or three band depending on number of bands. Also sets
    * m_saveInput for reconnection at the end of write.
    */
   void setupInputChain();

   /**
    * @brief Gets the enumerated value of IMAGE_TYPE_KW lookup.
    *
    * This is the image type of the stream embedded in the pdf.
    * Default "raw" if not found.
    * 
    * @return ossimPdfImageType, e.g. PNG, RAW...
    */
   ossimPdfImageType getImageType() const;   

   /**
    * @brief Initializes image type from IMAGE_TYPE_KW lookup.
    *
    * This is the image type of the stream embedded in the pdf.
    * Default "raw" if not found.
    *
    * @param type Initialized by this.
    */
   void getImageType( std::string& type ) const;

   /**
    * @brief Adds option to m_kwl with mutex lock.
    * @param key
    * @param value
    */
   void addOption( const std::string& key, const std::string& value );

   /**
    * @brief Gets the page size.
    *
    * Takes into account image width, height, and annotations.
    * 
    * @param width Width of image.
    * @param height Height of image.
    * @param size Initialized by this.
    */
#if 0
   void getMediaBoxSize( ossim_uint32 imageWidth,
                         ossim_uint32 imageHeight,
                         ossimIpt& size ) const;
#endif

   /**
    * @brief Gets page offset for image from lower left corner of media box.
    * @param aoi Area of interest.
    * @param offset Initialized by this.
    */
   void getLowerLeftPageOffset( const ossimIrect& aoi,
                                ossimIpt& offset ) const;

   /**
    * @brief Get the LGIDict string.
    * @param geom Image geometry.
    * @param aoi Area of interest.
    * @param s Initialized by this.
    */
   void getLgiDict( ossimImageGeometry* geom,
                    const ossimIrect& aoi,
                    std::string& s ) const;

   /**
    * @brief Gets projection part of LGIDict string.
    * @param geom Image geometry.
    * @param s Initialized by this. 
    */ 
   void getLgiDictProjection( ossimImageGeometry* geom,
                              std::string& s ) const;

   
   /**
    * @brief Gets Coordinate Transformation Matrix(CTM) part of
    * LGIDict string from geom. 
    * @param geom Image geometry.
    * @param aoi Area of interest.
    * @param s Initialized by this or cleared on error.
    */
   void getLgiDictCtm( ossimImageGeometry* geom,
                       const ossimIrect& aoi,
                       std::string& s ) const;

   /**
    * @brief Gets datum part of LGIDict string from geom. 
    * @param geom Image geometry.
    * @param s Initialized by this or cleared on error.
    */   
   void getLgiDictDatum( ossimImageGeometry* geom,
                         std::string& s ) const;

   /**
    * @brief Gets description part of LGIDict string from geom. 
    * @param geom Image geometry.
    * @param s Initialized by this or cleared on error.
    */   
   void getLgiDictDescription( ossimImageGeometry* geom,
                               std::string& s ) const;
   
   /**
    * @brief Gets neatline part of LGIDict string from geom. 
    * @param geom Image geometry.
    * @param s Initialized by this or cleared on error.
    */
#if 0
   void getLgiDictNeatline( ossimImageGeometry* geom,
                            std::string& s ) const;
#endif

   /**
    * @brief Gets projection type part of LGIDict string from geom. 
    * @param geom Image geometry.
    * @param s Initialized by this or cleared on error.
    */
   void getLgiDictProjectionType( ossimImageGeometry* geom,
                                  std::string& s ) const;
   
   /**
    * @brief Gets the EPSG/PCS code from geometry.
    * @param geom Image geometry.
    * @return EPSG code or 32767 on error.
    */
   ossim_uint32 getEpsgCode( const ossimImageGeometry* geom ) const;

#if 0
   void getNumberOfTiles( ossim_uint32 imageWidth,
                          ossim_uint32 imageHeight,
                          ossimIpt& size ) const;
#endif

   /**
    * @brief Gets the Title.
    *
    * Looks in m_kwl for options keyword: "Title"
    *
    * Defaults to basename of output file if not found.
    *
    * @param s Initialed by this.
    */
   void getTitle( std::string& s ) const;
   
   /**
    * @brief Gets the Author.
    *
    * Looks in m_kwl for options keyword: "Author"
    *
    * Defaults to "ossim" if not found.
    *
    * @param s Initialed by this.
    */
   void getAuthor( std::string& s ) const;

   /**
    * @brief Gets the Subject.
    *
    * Looks in m_kwl for options keyword: "Subject"
    *
    * Default: No default, empty string if not found.
    *
    * @param s Initialed by this.
    */
   void getSubject( std::string& s ) const;

   /**
    * @brief Gets the Keywords.
    *
    * Looks in m_kwl for options keyword: "Keywords"
    *
    * Default: No default, empty string if not found.
    *
    * @param s Initialed by this.
    */
   void getKeywords( std::string& s ) const;

   /**
    * @brief Gets the Creator.
    *
    * Looks in m_kwl for options keyword: "Creator"
    *
    * Defaults to "ossim" if not found.
    *
    * @param s Initialed by this.
    */
   void getCreator( std::string& s ) const;
   
   /**
    * @brief Gets the Producer.
    *
    * Looks in m_kwl for options keyword: "Producer"
    *
    * Defaults to "ossim" if not found.
    *
    * @param s Initialed by this.
    */
   void getProducer( std::string& s ) const;

   /**
    * @brief Gets the Producer.
    *
    * Looks in m_kwl for options keyword: "CreationDate"
    *
    * Defaults to current time if not found.
    *
    * @param s Initialed by this.
    *
    * NOTE: This is a formatted string in the form of:
    * D:YYYYMMDDHHmmSSOHH'mm'
    */   
   void getCreationDate( std::string& s ) const;

   /**
    * @brief Gets the Producer.
    *
    * Looks in m_kwl for options keyword: "ModDate"
    *
    * Defaults to current time if not found.
    *
    * @param s Initialed by this.
    *
    * NOTE: This is a formatted string in the form of:
    * D:YYYYMMDDHHmmSSOHH'mm'
    */
   void getModDate( std::string& s ) const;

   /**
    * @brief Get's the date in format of:
    * (D:YYYYMMDDHHmmSSZ00'00')
    *
    * @param s Initialed by this.
    *
    * NOTE: Date code is NOT thread safe.
    */
   void getGmtDate( std::string& s ) const;

   /**
    * @brief Gets tiles size as string.
    *
    * Looks in m_kwl for options keyword: "output_tile_size"
    *
    * Defaults to 1024 if not found.
    *
    * @param s Initialized by this.
    */
   void getTileSize( std::string& s ) const;

   /**
    * @brief Gets Tile size as integer point.
    *
    * Defaults to 1024x1024 if not found.
    * @param tileSize Initialized by this.
    */
   void getTileSize( ossimIpt& tileSize ) const;
   
   /**
    * @return string for x and y in the form of (x=1, y=3): "/Tile_0001_0003"
    */
   std::string getTileString( ossim_uint32 x, ossim_uint32 y ) const;

   std::ostream* m_str;
   bool m_ownsStream;

   /**
    * Holds the origin end of the chain connected to theInputConnection.
    */
   ossimRefPtr<ossimConnectableObject> m_savedInput;

   /** Holds all options in key, value pair map. */
   ossimRefPtr<ossimKeywordlist> m_kwl;

   OpenThreads::Mutex m_mutex;

TYPE_DATA
};

#endif /* #ifndef ossimPdfWriter_HEADER */
