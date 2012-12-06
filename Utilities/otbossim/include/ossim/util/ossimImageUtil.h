//----------------------------------------------------------------------------
// File: ossimImageUtil.h
// 
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: ossimImageUtil
//
// See class descriptions below for more.
// 
//----------------------------------------------------------------------------
// $Id$

#ifndef ossimImageUtil_HEADER
#define ossimImageUtil_HEADER 1

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimPropertyInterface.h>
#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimOverviewBuilderBase.h>
#include <OpenThreads/Mutex>
#include <ostream>

class ossimArgumentParser;
class ossimFileWalker;
class ossimGpt;

/**
 * @brief ossimImageUtil class.
 *
 * Utility class for processing image recursively.  This is for doing things like:
 * 
 * building overview, histograms, compute min/max, extract vertices.
 */
class OSSIM_DLL ossimImageUtil : public ossimReferenced
{
public:

   /** default constructor */
   ossimImageUtil();

   /** virtual destructor */
   virtual ~ossimImageUtil();

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
    * @brief Execute method.
    *
    * This launches file walking mechanism.
    * 
    * @note Throws ossimException on error.
    */
   void execute();

   /**
    * @brief ProcessFile method.
    *
    * This method is linked to the ossimFileWalker::walk method via a callback
    * mechanism.  It is called by the ossimFileWalk (caller).  In turn this
    * class (callee) calls ossimFileWalker::setRecurseFlag and
    * ossimFileWalker::setAbortFlag to control the waking process.
    * 
    * @param file to process.
    */
   void processFile(const ossimFilename& file);

   /**
    * @brief Sets create overviews flag keyword CREATE_OVERVIEWS_KW used by
    * processFile method.
    *
    * @param flag If true overview will be created if image does not already
    * have the required or if the REBUILD_OVERVIEWS_KW is set.
    *
    * @note Number of required overviews is controlled by the ossim preferences
    * keyword overview_stop_dimension.
    */
   void setCreateOverviewsFlag( bool flag );

   /** @return true if CREATE_OVERVIEWS_KW is found and set to true. */
   bool createOverviews() const;
   
   /**
    * @brief Sets the rebuild overview flag keyword REBUILD_OVERVIEWS_KW used by
    * processFile method.
    *
    * @param flag If true forces a rebuild of overviews even if image has
    * required number of reduced resolution data sets.
    *
    * @note Number of required overviews is controlled by the ossim preferences
    * keyword overview_stop_dimension.
    */
   void setRebuildOverviewsFlag( bool flag );

   /** @return true if REBUILD_OVERVIEWS_KW is found and set to true. */
   bool rebuildOverviews() const;

   /**
    * @brief Sets the rebuild histogram flag keyword REBUILD_HISTOGRAM_KW used by
    * processFile method.
    *
    * @param flag If true forces a rebuild of histogram even if image has one already.
    */
   void setRebuildHistogramFlag( bool flag );

   /** @return true if REBUILD_HISTOGRAM_KW is found and set to true. */
   bool rebuildHistogram() const;

   /**
    * @brief Sets key OVERVIEW_TYPE_KW.
    *
    * Available types depens on plugins.  Know types:
    * ossim_tiff_box ( defualt )
    * ossim_tiff_nearest
    * ossim_kakadu_nitf_j2k ( kakadu plugin )
    * gdal_tiff_nearest	    ( gdal plugin )
    * gdal_tiff_average	    ( gdal plugin )
    * gdal_hfa_nearest      ( gdal plugin )	
    * gdal_hfa_average      ( gdal plugin )	
    * 
    * @param type One of the above.
    */
   void setOverviewType( const std::string& type );
   
   /**
    * @brief sets the overview stop dimension.
    *
    * The overview builder will decimate the image until both dimensions are
    * at or below this dimension.
    *
    * @param dimension
    *
    * @note Recommend a power of 2 value, i.e. 8, 16, 32 and so on.
    */
   void setOverviewStopDimension( ossim_uint32 dimension );
   void setOverviewStopDimension( const std::string& dimension );

   /**
    * @brief Sets the tile size.
    *
    * @param tileSize
    *
    * @note Must be a multiple of 16, i.e. 64, 128, 256 and so on.
    */
   void setTileSize( ossim_uint32 tileSize );

   /**
    * @return Overview stop dimension or 0 if OVERVIEW_STOP_DIM_KW is not
    * found.
    */
   ossim_uint32 getOverviewStopDimension() const;

   /**
    * @brief Sets create histogram flag keyword CREATE_HISTOGRAM_KW used by
    * processFile method.
    *
    * @param flag If true a full histogram will be created.
    */
   void setCreateHistogramFlag( bool flag );
   
   /** @return true if CREATE_HISTOGRAM_KW is found and set to true. */
   bool createHistogram() const;

   /**
    * @brief Sets create histogram flag keyword CREATE_HISTOGRAM_FAST_KW used by
    * processFile method.
    *
    * @param flag If true a histogram will be created in fast mode.
    */
   void setCreateHistogramFastFlag( bool flag );

   /** @return true if CREATE_HISTOGRAM_FAST_KW is found and set to true. */
   bool createHistogramFast() const;

   /**
    * @brief Sets create histogram "R0" flag keyword CREATE_HISTOGRAM_R0_KW used by
    * processFile method.
    *
    * @param flag If true a histogram will be created from R0.
    */
   void setCreateHistogramR0Flag( bool flag );

   /** @return true if CREATE_HISTOGRAM_R0_KW is found and set to true. */
   bool createHistogramR0() const;

   /** @return true if any of the histogram options are set. */
   bool hasHistogramOption() const;

   /** @return Histogram mode or OSSIM_HISTO_MODE_UNKNOWN if not set. */
   ossimHistogramMode getHistogramMode() const;

   /**
    * @brief Sets scan for min/max flag keyword SCAN_MIN_MAX_KW used by
    * processFile method.
    *
    * @param flag If true a file will be scanned for min/max and a file.omd
    * will be written out.
    */
   void setScanForMinMax( bool flag );
   
   /** @return true if SCAN_MIN_MAX_KW is found and set to true. */
   bool scanForMinMax() const;

   /**
    * @brief Sets scan for min/max/null flag keyword SCAN_MIN_MAX_KW used by
    * processFile method.
    *
    * @param flag If true a file will be scanned for min/max/null and a file.omd
    * will be written out.
    */
   void setScanForMinMaxNull( bool flag );

   /** @return true if SCAN_MIN_MAX_NULL_KW is found and set to true. */
   bool scanForMinMaxNull() const;

   /**
    * @brief Sets the writer property for compression quality.
    *
    * @param quality For TIFF JPEG takes values from 1
    * to 100, where 100 is best.  For J2K plugin (if available),
    * numerically_lossless, visually_lossless, lossy.
    */
   void setCompressionQuality( const std::string& quality );

   /**
    * @brief Sets the compression type to use when building overviews.
    *  
    * @param compression_type Current supported types:
    * - deflate 
    * - jpeg
    * - lzw
    * - none
    * - packbits
    */
   void setCompressionType( const std::string& type );

   /**
    * @brief Sets the overview builder copy all flag.
    * @param flag
    */
   void setCopyAllFlag( bool flag );

   /**
    * @return true if COPY_ALL_FLAG_KW key is found and value is true; else,
    * false.
    */
   bool getCopyAllFlag() const;
   
   /**
    * @brief Sets the output directory.  Typically overviews and histograms
    * are placed parallel to image file.  This overrides.
    *  
    * @param directory
    */
   void setOutputDirectory( const std::string& directory );
 
   /**
    * @brief Sets the output file name flag OUTPUT_FILENAMES_KW.
    *
    * If set to true all files that we can successfully open will be output.
    *  
    * @param flag
    */
   void setOutputFileNamesFlag( bool flag );

   /**
    * @return true if OUTPUT_FILENAMES_KW key is found and value is true; else,
    * false.
    */
   bool getOutputFileNamesFlag() const;
 
   /**
    * @brief Set number of threads to use.
    *
    * This is only used in execute method if a directory is given to
    * application to walk.
    *
    * @param threads Defaults to 1 if THREADS_KW is not found.
    */
   void setNumberOfThreads( ossim_uint32 threads );
   void setNumberOfThreads( const std::string& threads );

private:

   void createOverview(ossimRefPtr<ossimImageHandler>& ih,
                       bool& consumedHistogramOptions,
                       bool& consumedCmmOptions);

   void createOverview(ossimRefPtr<ossimImageHandler>& ih,
                       ossimRefPtr<ossimOverviewBuilderBase>& ob,
                       ossim_uint32 entry,
                       bool useEntryIndex,
                       bool& consumedHistogramOptions);

   /** @return true if entry has required overviews. */
   bool hasRequiredOverview( ossimRefPtr<ossimImageHandler>& ih,
                             ossimRefPtr<ossimOverviewBuilderBase>& ob );

   void createHistogram(ossimRefPtr<ossimImageHandler>& ih);

   void createHistogram(ossimRefPtr<ossimImageHandler>& ih,
                       ossim_uint32 entry,
                       bool useEntryIndex);

   
   /** @brief Initializes arg parser and outputs usage. */
   void usage(ossimArgumentParser& ap);

   void outputOverviewWriterTypes() const;

   /**
    * @return true if file is a directory based image and the stager should go
    * on to next directory; false if stager should continue with directory.
    */
   bool isDirectoryBasedImage(const ossimImageHandler* ih) const;


   /**
    * @brief Initializes type from OVERVIEW_TYPE_KW or sets to default
    * ossim_tiff_box if not found.
    */
   void getOverviewType(std::string& type) const;

   /** @brief set reader or writer properties based on cast of pi. */
   void setProps(ossimPropertyInterface* pi) const;
   
   /**
    * @return Threads to use.  Defaults to 1 if THREADS_KW is not found.
    */
   ossim_uint32 getNumberOfThreads() const;

   /** @return the next writer prop index. */
   ossim_uint32 getNextWriterPropIndex() const;

   /** @return the next reader prop index. */   
   ossim_uint32 getNextReaderPropIndex() const;

   /**
    * @brief Adds option to m_kwl with mutex lock.
    * @param key
    * @param value
    */
   void addOption( const std::string& key, ossim_uint32 value );
   void addOption( const std::string& key, const std::string& value );
   
   /** Holds all options passed into intialize except writer props. */
   ossimRefPtr<ossimKeywordlist> m_kwl;

   ossimFileWalker*   m_fileWalker;
   OpenThreads::Mutex m_mutex;
};

#endif /* #ifndef ossimImageUtil_HEADER */
