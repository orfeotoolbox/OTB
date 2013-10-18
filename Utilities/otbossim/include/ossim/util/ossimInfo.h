//----------------------------------------------------------------------------
// File: ossimInfo.h
// 
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: ossimInfo class declaration
//
// See class doxygen descriptions below for more.
// 
//----------------------------------------------------------------------------
// $Id: ossimInfo.h 21773 2012-09-26 00:08:19Z dlucas $

#ifndef ossimInfo_HEADER
#define ossimInfo_HEADER 1

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/imaging/ossimImageHandler.h>

#include <ostream>

class ossimArgumentParser;
class ossimGpt;

/**
 * @brief ossimInfo class.
 *
 * This is a utility class for getting information from the ossim library.
 * This includes information from an image, general library queries, like
 * loaded plugins, and just general stuff like height for point, conversions
 * and so on that are easily obtained through the library.
 */
class OSSIM_DLL ossimInfo : public ossimReferenced
{
public:

   /** default constructor */
   ossimInfo();

   /** virtual destructor */
   virtual ~ossimInfo();

   /**
    * @brief Adds application arguments to the argument parser.
    * @param ap Parser to add to.
    */
   void addArguments(ossimArgumentParser& ap);

   /**
    * @brief Initial method.
    *
    * Typically called from application prior to execute.  This parses
    * all options and put in keyword list m_kwl.
    * 
    * @param ap Arg parser to initialize from.
    *
    * @return true, indicating process should continue with execute.
    */
   bool initialize(ossimArgumentParser& ap);

   /**
    * @brief execute method.
    *
    * Performs the actual dump of information.  This executes any options
    * set including image operations, i.e. -i -p --dno and so on.
    * 
    * @note Throws ossimException on error.
    */
   void execute();

   /**
    * @brief handles image options.
    *
    * Handles image type info opions,  i.e. -i -p --dno and so on.
    *
    * @return Number of consumed options.
    */
   ossim_uint32 executeImageOptions(const ossimFilename& file);

   /**
    * @brief getImageInfo Method to open image "file" and get image info
    * in the form of a ossimKeywordlist.
    *
    * Flags turn on various pieces of info.  These equate to options in
    * ossim-info for image information.
    *
    * This method opens and closes "file" under the hood so if an image is
    * open and it's not the same as "file" it will be closed by this call.
    * On exite the image will be closed.
    *
    * @param file Image file to get information for.
    * @param dumpFlag      ossim-info -d
    * @param dnoFlag       ossim-info --dno
    * @param imageGeomFlag ossim-info -p
    * @param imageInfoFlag ossim-info -i 
    * @param metaDataFlag  ossim-info -m 
    * @param paletteFlag   ossim-info --palette
    * @param kwl Initialized by this method.
    */
   void getImageInfo(const ossimFilename& file,
                     bool dumpFlag,
                     bool dnoFlag,
                     bool imageGeomFlag,
                     bool imageInfoFlag,
                     bool metaDataFlag,
                     bool paletteFlag,
                     ossimKeywordlist& kwl);
   
   /**
    * @brief Opens image handler and stores in m_img data member.
    * @param Image to open.
    * @note Throws ossimException if image cannot be opened.
    */
   void openImage(const ossimFilename& file);

   /** @brief Closes image if open. */
   void closeImage();

   /**
    * @return Ref pointer to the image handler.  Can be null if an image is
    * not open.
    */
   ossimRefPtr<ossimImageHandler> getImageHandler();

   /** @brief Dumps the image information from ossimInfoFactoryRegistry */
   void dumpImage(const ossimFilename& file, bool dnoFlag) const;

   /**
    * @brief Dumps the image information from ossimInfoFactoryRegistry to
    * keyword list.
    * @param file Image to dump.
    * @param dnoFlag Dump no overviews. If true do not dump overviews.
    * @param kwl Initialized by this method.
    */
   void dumpImage(const ossimFilename& file,
                  bool dnoFlag,
                  ossimKeywordlist& kwl) const;

   /**
    * @brief Prints factories.
    * @param keywordListFlag If true the result of a saveState will be output
    * for each factory.
    * */
   void printFactories(bool keywordListFlag) const;

   /**
    * @brief Populates keyword list with metadata.
    * This requires open image.
    *
    * @param kwl Keyword list to populate.
    */
   void getImageMetadata(ossimKeywordlist& kwl) const;

   /**
    * @brief Populates keyword list with palette data.
    * This requires open image.
    *
    * @param kwl Keyword list to populate.
    */
   void getImagePalette(ossimKeywordlist& kwl);

   /**
    * @brief Populates keyword list with general image information.
    *
    * This requires open image.
    *
    * @param kwl Keyword list to populate.
    * @param dnoFlag If true no entries flaged as overviews will be output.
    */
   void getImageInfo(ossimKeywordlist& kwl, bool dnoFlag);

   /**
    * @brief Populates keyword list with general image information.
    *
    * @param entry Entry number to select.  Note this is the entry number from
    * the getEntryList call not a simple zero based entry index.
    * 
    * @param kwl Keyword list to populate.
    *
    * @param dnoFlag If true no entries flaged as overviews will be output.
    *
    * This requires open image.
    *
    * @return true if entry info was saved to keyword list false if not.
    */
   bool getImageInfo(ossim_uint32 entry, ossimKeywordlist& kwl, bool dnoFlag);

   /**
    * @brief Populates keyword list with image geometry/projection information.
    *
    * This requires open image.
    *
    * @param kwl Keyword list to populate.
    * @param dnoFlag If true no entries flaged as overviews will be output.
    */
   void getImageGeometryInfo(ossimKeywordlist& kwl, bool dnoFlag);

   /**
    * @brief Populates keyword list with image geometry/projection information.
    *
    * @param entry Entry number to select.  Note this is the entry number
    * from the getEntryList call not a simple zero based entry index.
    * 
    * @param kwl Keyword list to populate.
    *
    * @param dnoFlag If true no entries flaged as overviews will be output.
    *
    * This requires open image.
    *
    * @return true if entry info was saved to keyword list false if not.
    */
   bool getImageGeometryInfo( ossim_uint32 entry, 
                              ossimKeywordlist& kwl, 
                              bool dnoFlag );

   void getCenterImage(ossimKeywordlist& kwl);
   bool getCenterImage(ossim_uint32 entry, ossimKeywordlist& kwl);
   void getCenterGround(ossimKeywordlist& kwl);
   bool getCenterGround(ossim_uint32 entry, ossimKeywordlist& kwl);

   /**
    * @brief Populates keyword list with image rectangle.
    *
    * This requires open image.
    *
    * @param kwl Keyword list to populate.
    */
   void getImageRect(ossimKeywordlist& kwl);

   /**
    * @brief Populates keyword list with image rectangle.
    *
    * @param entry Entry number to select.  Note this is the entry number from
    * the getEntryList call not a simple zero based entry index.
    * 
    * @param kwl Keyword list to populate.
    *
    * This requires open image.
    *
    * @return true if entry info was saved to keyword list false if not.
    */
   bool getImageRect(ossim_uint32 entry, ossimKeywordlist& kwl);

   /**
    * @return true if current open image entry is an overview.
    */
   bool isImageEntryOverview() const;

   /** @brief Dumps ossim preferences/configuration data. */
   void printConfiguration() const;

   /**
    * @brief Dumps ossim preferences/configuration data.
    * @param out Output to write to.
    * @return stream
    */
   std::ostream& printConfiguration(std::ostream& out) const;

   /** @brief Dumps datum list to stdout. */
   void printDatums() const;

   /** @brief Dumps datum list to stream. */
   std::ostream& printDatums(std::ostream& out) const;

   /** @brief Converts degrees to radians and outputs to stdout. */
   void deg2rad(const ossim_float64& degrees) const;

   /**
    * @brief Converts degrees to radians and outputs to stream.
    * @param out Output to write to.
    * @return stream
    */
   std::ostream& deg2rad(const ossim_float64& degrees, std::ostream& out) const;

   /** @brief Converts radians to degrees and outputs to stdout. */
   void rad2deg(const ossim_float64& radians) const;

   /**
    * @brief Converts radians to degrees and outputs to stream. 
    * @param out Output to write to.
    * @return stream
    */
   std::ostream& rad2deg(const ossim_float64& radians, std::ostream& out) const;

   /** @brief Converts feet to meters and outputs to stdout. */
   void ft2mtrs(const ossim_float64& feet, bool us_survey) const;

   /**
    * @brief Converts feet to meters and outputs to stream.
    * @param out Output to write to out.
    * @return stream
    */
   std::ostream& ft2mtrs(const ossim_float64& feet, bool us_survey, std::ostream& out) const;

   /** @brief Converts meters to feet and outputs to stdout. */
   void mtrs2ft(const ossim_float64& meters, bool us_survey) const;

   /**
    * @brief Converts meters to feet and outputs to stream.
    * @param out Output to write to out.
    * @return stream
    */   
   std::ostream& mtrs2ft(const ossim_float64& meters, bool us_survey, std::ostream& out) const;

   /**
    * @brief Get meters per degree for a given latitude and outputs to stdout.
    * @param latitude
    */
   void mtrsPerDeg(const ossim_float64& latitude) const;

   /**
    * @brief Get meters per degree for a given latitude and outputs to stream.
    * @param latitude
    * @param out Output to write to out.
    * @return stream
    */   
   std::ostream& mtrsPerDeg(const ossim_float64& latitude, std::ostream& out) const;

   /**
    * @brief Gets the height for ground point (latitude, longitude). Outputs
    * to stdout.
    * @param gpt Ground point.
    */
   void outputHeight(const ossimGpt& gpt) const;

   /**
    * @brief Gets the height for ground point (latitude, longitude). Outputs
    * to out.
    * @param out Output to write to out.
    * @return stream
    */
   std::ostream& outputHeight(const ossimGpt& gpt, std::ostream& out) const;

   /** @brief Prints loaded plugins to stdout. */
   void printPlugins() const;

   /**
    * @brief Prints loaded plugins to stream.
    * @param out Output to write to out.
    * @return stream
    */
   std::ostream& printPlugins(std::ostream& out) const;

   /**
    * @brief Test a plugin load  and outputs to stdout.
    * 
    * @param plugin Plugin to test.
    */
   void testPlugin(const ossimFilename& plugin) const;

   /**
    * @brief Test a plugin load outputs to stream.
    * 
    * @param plugin Plugin to test.
    * @param out Stream to write to.
    * @param stream
    */
   std::ostream& testPlugin(const ossimFilename& plugin, std::ostream& out) const;

   /** @brief Prints overview types to stdout. */
   void printOverviewTypes() const;

   /** @brief Prints overview types to stream. */
   std::ostream& printOverviewTypes(std::ostream& out) const;

   /** @breif Prints projections to stdout. */
   void printProjections() const;

   /** @breif Prints projections to stream. */
   std::ostream& printProjections(std::ostream& out) const;

   /** @brief Prints reader properties to stdout. */
   void printReaderProps() const;

   /** @brief Prints reader properties to stream. */
   std::ostream& printReaderProps(std::ostream& out) const;

   /** @brief Prints resampler filters to stdout. */
   void printResamplerFilters() const;

   /** @brief Prints resampler filters to stream. */
   std::ostream& printResamplerFilters(std::ostream& out) const;

   /** @brief Prints list of available writers to stdout. */
   void printWriters() const;

   /** @brief Prints list of available writers to stream. */
   std::ostream& printWriters(std::ostream& out) const;

   /** @brief Prints writer properties to stdout. */
   void printWriterProps() const;

   /** @brief Prints writer properties to stream. */
   std::ostream& printWriterProps(std::ostream& out) const;

   /**
    * @brief Gets the radiometry string, i.e. "8-bit" and so on, from scalar.
    * @param scalar Scalar type.
    * @param s String to initialize.
    */
   void getRadiometry(ossimScalarType scalar, std::string& s) const;
   
private:
 
   /**
    * @brief Populates keyword list with metadata.
    * @param ih Pointer to an image handler.
    * @param kwl Keyword list to populate.
    */
   void getImageMetadata( const ossimImageHandler* ih, 
                          ossimKeywordlist& kwl ) const;
 
  /**
   * @brief Populates keyword list with palette data.
   * @param ih Pointer to an image handler.
   * @param kwl Keyword list to populate.
   */
   void getImagePalette( ossimImageHandler* ih, 
                         ossimKeywordlist& kwl ) const;
  
   /**
    * @brief Populates keyword list with general image information.
    * @param ih Pointer to an image handler.
    * @param kwl Keyword list to populate.
    * @param dnoFlag If true no entries flaged as overviews will be output.
    */
   void getImageInfo( ossimImageHandler* ih, 
                      ossimKeywordlist& kwl, 
                      bool dnoFlag ) const;

   /**
    * @brief Populates keyword list with general image information.
    * @param ih Pointer to an image handler.
    * @param entry Entry number to select.  Note this is the entry number from
    * the getEntryList call not a simple zero based entry index.
    * @param kwl Keyword list to populate.
    * @param dnoFlag If true no entries flaged as overviews will be output.
    * @return true if entry info was saved to keyword list false if not.
    */
   bool getImageInfo( ossimImageHandler* ih, 
                      ossim_uint32 entry, 
                      ossimKeywordlist& kwl, 
                      bool dnoFlag ) const;
   
   /**
    * @brief Populates keyword list with image geometry/projection information.
    * @param ih Pointer to an image handler.
    * @param kwl Keyword list to populate.
    * @param dnoFlag If true no entries flaged as overviews will be output.
    */
   void getImageGeometryInfo( ossimImageHandler* ih,
                              ossimKeywordlist& kwl, 
                              bool dnoFlag ) const;

   /**
    * @brief Populates keyword list with image geometry/projection information.
    * @param ih Pointer to an image handler.
    * @param entry Entry number to select.  Note this is the entry number
    * from the getEntryList call not a simple zero based entry index.
    * @param kwl Keyword list to populate.
    * @param dnoFlag If true no entries flaged as overviews will be output.
    * @return true if entry info was saved to keyword list false if not.
    */
   bool getImageGeometryInfo( ossimImageHandler* ih,
                              ossim_uint32 entry, 
                              ossimKeywordlist& kwl, 
                              bool dnoFlag ) const;

   void getCenterImage( ossimImageHandler* ih,
                      ossimKeywordlist& kwl ) const;
   bool getCenterImage( ossimImageHandler* ih,
                      ossim_uint32 entry, 
                      ossimKeywordlist& kwl ) const;
   void getCenterGround( ossimImageHandler* ih,
                      ossimKeywordlist& kwl ) const;
   bool getCenterGround( ossimImageHandler* ih,
                      ossim_uint32 entry, 
                      ossimKeywordlist& kwl ) const;

   /**
    * @brief Populates keyword list with image rectangle.
    *
    * @param kwl Keyword list to populate.
    */
   void getImageRect( ossimImageHandler* ih,
                      ossimKeywordlist& kwl ) const;

   /**
    * @brief Populates keyword list with image rectangle.
    * @param entry Entry number to select.  Note this is the entry number from
    * the getEntryList call not a simple zero based entry index.
    * @param kwl Keyword list to populate.
    * @return true if entry info was saved to keyword list false if not.
    */
   bool getImageRect( ossimImageHandler* ih,
                      ossim_uint32 entry, 
                      ossimKeywordlist& kwl ) const;

   /** @return true if current open image entry is an overview. */
   bool isImageEntryOverview( const ossimImageHandler* ih ) const;

  /**
   * @brief Opens image.
   * @param Image to open.
   * @return ossimRefPtr with image handler.
   * @note Throws ossimException if image cannot be opened.
   */
   ossimRefPtr<ossimImageHandler> openImageHandler(const ossimFilename& file) const;
   
   /** @brief Initializes arg parser and outputs usage. */
   void usage(ossimArgumentParser& ap);

   /** Hold all options passed into intialize except writer props. */
   ossimRefPtr<ossimKeywordlist> m_kwl;

   /** Holds the open image. */
   ossimRefPtr<ossimImageHandler> m_img;
};

#endif /* #ifndef ossimInfo_HEADER */
