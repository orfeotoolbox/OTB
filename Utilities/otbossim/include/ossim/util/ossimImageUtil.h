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
    * @return true, indicating process should continue with execute.
    * @note A throw with an error message of "usage" is used to get out when
    * a usage is printed.
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

private:

   void createOverview(ossimRefPtr<ossimImageHandler>& ih,
                       bool& consumedHistogramOptions,
                       bool& consumedCmmOptions);

   void createOverview(ossimRefPtr<ossimImageHandler>& ih,
                       ossimRefPtr<ossimOverviewBuilderBase>& ob,
                       ossim_uint32 entry,
                       bool useEntryIndex,
                       bool& consumedHistogramOptions);

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

   /** @return true if CREATE_OVERVIEWS_KW is found and set to true. */
   bool createOverviews() const;

   /** @return true if REBUILD_OVERVIEWS_KW is found and set to true. */
   bool rebuildOverviews() const;

   /** @return true if SCAN_MIN_MAX_KW is found and set to true. */
   bool scanForMinMax() const;

   /** @return true if SCAN_MIN_MAX_NULL_KW is found and set to true. */
   bool scanForMinMaxNull() const;

   /**
    * @brief Initializes type from OVERVIEW_TYPE_KW or sets to default
    * ossim_tiff_box if not found.
    */
   void getOverviewType(std::string& type) const;

   /** @brief set reader or writer properties based on cast of pi. */
   void setProps(ossimPropertyInterface* pi) const;

   /**
    * @return Overview stop dimension or 0 if OVERVIEW_STOP_DIM_KW is not
    * found.
    */
   ossim_uint32 getOverviewStopDimension() const;

   /** @return true if CREATE_HISTOGRAM_KW is found and set to true. */
   bool createHistogram() const;

   /** @return true if CREATE_HISTOGRAM_FAST_KW is found and set to true. */
   bool createHistogramFast() const;

   /** @return true if CREATE_HISTOGRAM_R0_KW is found and set to true. */
   bool createHistogramR0() const;

   /** @return true if any of the histogram options are set. */
   bool hasHistogramOption() const;

   /** @return Histogram mode or OSSIM_HISTO_MODE_UNKNOWN if not set. */
   ossimHistogramMode getHistogramMode() const;

   /**
    * @return Threads to use.  Defaults to 1 if THREADS_KW is not found.
    */
   ossim_uint32 getNumberOfThreads() const;
   
   /** Holds all options passed into intialize except writer props. */
   ossimRefPtr<ossimKeywordlist> m_kwl;

   ossimFileWalker*   m_fileWalker;
   OpenThreads::Mutex m_mutex;
};

#endif /* #ifndef ossimImageUtil_HEADER */
