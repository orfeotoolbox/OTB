//----------------------------------------------------------------------------
// File: ossimImageUtil.cpp
// 
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: ossimImageUtil class definition
//
// Utility class for processing image recursively.  This is for doing things like:
// building overview, histograms, compute min/max, extract vertices.
// 
//----------------------------------------------------------------------------
// $Id$

#include <ossim/util/ossimImageUtil.h>
#include <ossim/base/ossimArgumentParser.h>
#include <ossim/base/ossimApplicationUsage.h>
#include <ossim/base/ossimCallback1.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimContainerProperty.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimDatumFactoryRegistry.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimObjectFactoryRegistry.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimGeoidManager.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/init/ossimInit.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimPreferences.h>
#include <ossim/base/ossimProperty.h>
#include <ossim/base/ossimPropertyInterface.h>
#include <ossim/base/ossimStdOutProgress.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimXmlDocument.h>
#include <ossim/elevation/ossimElevManager.h>
#include <ossim/imaging/ossimFilterResampler.h>
#include <ossim/imaging/ossimHistogramWriter.h>
#include <ossim/imaging/ossimImageGeometry.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimImageHistogramSource.h>
#include <ossim/imaging/ossimImageWriterFactoryRegistry.h>
#include <ossim/imaging/ossimOverviewBuilderFactoryRegistry.h>
#include <ossim/init/ossimInit.h>
#include <ossim/plugin/ossimSharedPluginRegistry.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/support_data/ossimSupportFilesList.h>
#include <ossim/util/ossimFileWalker.h>

#include <iomanip>
#include <string>
#include <vector>

static std::string COPY_ALL_FLAG_KW           = "copy_all_flag";
static std::string CREATE_HISTOGRAM_KW        = "create_histogram";
static std::string CREATE_HISTOGRAM_FAST_KW   = "create_histogram_fast";
static std::string CREATE_HISTOGRAM_R0_KW     = "create_histogram_r0";
static std::string CREATE_OVERVIEWS_KW        = "create_overviews";
static std::string FALSE_KW                   = "false";
static std::string FILE_KW                    = "file";
static std::string INTERNAL_OVERVIEWS_FLAG_KW = "internal_overviews_flag";
static std::string OUTPUT_DIRECTORY_KW        = "output_directory";
static std::string OUTPUT_FILENAMES_KW        = "output_filenames";
static std::string OVERVIEW_STOP_DIM_KW       = "overview_stop_dimension";
static std::string OVERVIEW_TYPE_KW           = "overview_type";
static std::string READER_PROP_KW             = "reader_prop";
static std::string REBUILD_HISTOGRAM_KW       = "rebuild_histogram";
static std::string REBUILD_OVERVIEWS_KW       = "rebuild_overviews";
static std::string SCAN_MIN_MAX_KW            = "scan_for_min_max";
static std::string SCAN_MIN_MAX_NULL_KW       = "scan_for_min_max_null";
static std::string THREADS_KW                 = "threads";
static std::string TILE_SIZE_KW               = "tile_size";
static std::string TRUE_KW                    = "true";
static std::string WRITER_PROP_KW             = "writer_prop";

// Static trace for debugging.  Use -T ossimImageUtil to turn on.
static ossimTrace traceDebug = ossimTrace("ossimImageUtil:debug");

ossimImageUtil::ossimImageUtil()
   :
   ossimReferenced(),
   ossimFileProcessorInterface(),
   m_kwl( new ossimKeywordlist() ),
   m_fileWalker(0),
   m_mutex(),
   m_errorStatus(0)
{
}

ossimImageUtil::~ossimImageUtil()
{
   if ( m_fileWalker )
   {
      delete m_fileWalker;
      m_fileWalker = 0;
   }
}

void ossimImageUtil::addArguments(ossimArgumentParser& ap)
{
   // Set the general usage:
   ossimApplicationUsage* au = ap.getApplicationUsage();
   ossimString usageString = ap.getApplicationName();
   usageString += " [options] <file-or-directory-to-walk>";
   au->setCommandLineUsage(usageString);

   // Set the command line options:
   au->addCommandLineOption("-a or --include-fullres", "Copy full res dataset to overview file as well as building reduced res sets. Option only valid with tiff overview builder. Requires -o option.");

   au->addCommandLineOption("--compression-quality", "Compression quality for TIFF JPEG takes values from 0 to 100, where 100 is best.  For J2K plugin, numerically_lossless, visually_lossless, lossy");
   
   au->addCommandLineOption("--compression-type", "Compression type can be: deflate, jpeg, lzw, none or packbits");

   au->addCommandLineOption("--ch or --create-histogram", "Computes full histogram alongside overview.");
   
   au->addCommandLineOption("--chf or --create-histogram-fast", "Computes a histogram in fast mode which samples partial tiles.");

   au->addCommandLineOption("--create-histogram-r0", "Forces create-histogram code to compute a histogram using r0 instead of the starting resolution for the overview builder. Can require a separate pass of R0 layer if the base image has built in overviews.");

   au->addCommandLineOption("-d", "<output_directory> Write overview to output directory specified.");

   au->addCommandLineOption("--of or --output-files", "Output image files we can open, exluding overviews.");
   
   au->addCommandLineOption("-h", "Display this information");

   au->addCommandLineOption("-i or --internal-overviews", "Builds internal overviews. Requires -o option. Option only valid with tiff input image and tiff overview builder. WARNING: Modifies source image and cannot be undone!");
   
   au->addCommandLineOption("--list-entries", "Lists the entries within the image");

   au->addCommandLineOption("-o", "Creates overviews. (default=ossim_tiff_box)");

   au->addCommandLineOption("--ot", "<overview_type> Overview type. see list at bottom for valid types. (default=ossim_tiff_box)");

   au->addCommandLineOption("-r or --rebuild-overviews", "Rebuild overviews even if they are already present.");
   
   au->addCommandLineOption("--rebuild-histogram", "Rebuild histogram even if they are already present.");

   au->addCommandLineOption("--reader-prop", "Adds a property to send to the reader. format is name=value");

   au->addCommandLineOption("-s",  "Stop dimension for overviews.  This controls how \nmany layers will be built. If set to 64 then the builder will stop when height and width for current level are less than or equal to 64.  Note a default can be set in the ossim preferences file by setting the keyword \"overview_stop_dimension\".");

   au->addCommandLineOption("--tile-size", "<size> Defines the tile size for overview builder.  Tiff option only. Must be a multiple of 16. Size will be used in both x and y directions. Note a default can be set in your ossim preferences file by setting the key \"tile_size\".");

   au->addCommandLineOption("--threads", "<threads> The number of threads to use. (default=1) Note a default can be set in your ossim preferences file by setting the key \"ossim_threads\".");

   au->addCommandLineOption("--scanForMinMax", "Turns on min, max scanning when reading tiles. This option assumes the null is known.");

   au->addCommandLineOption("--scanForMinMaxNull", "Turns on min, max, null scanning when reading tiles.  This option tries to find a null value which is useful for float data.");

   au->addCommandLineOption("--writer-prop", "Adds a property to send to the writer. format is name=value");
   
} // void ossimImageUtil::addArguments(ossimArgumentParser& ap)

bool ossimImageUtil::initialize(ossimArgumentParser& ap)
{
   static const char M[] = "ossimImageUtil::initialize(ossimArgumentParser&)";
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << M << " entered...\n";
   }

   bool result = true;

   if ( (ap.argc() == 1) || ap.read("-h") || ap.read("--help") )
   {
      usage(ap);

      // continue_after_init to false
      result = false;
   }
   else
   {
      //---
      // Start with clean options keyword list.
      //---
      m_kwl->clear();

      while ( 1 ) //  While forever loop...
      {
         // Used throughout below:
         std::string ts1;
         ossimArgumentParser::ossimParameter sp1(ts1);
         std::string ts2;
         ossimArgumentParser::ossimParameter sp2(ts2);

         if( ap.read("-a") || ap.read("--include-fullres") )
         {
            setCopyAllFlag( true );
            if ( ap.argc() < 2 )
            {
               break;
            }
         }

         if( ap.read("--compression-quality", sp1) )
         {
            if ( ts1.size() )
            {
               setCompressionQuality( ts1 );
            }
            if ( ap.argc() < 2 )
            {
               break;
            }
         }

         if( ap.read("--compression-type", sp1) )
         {
            if ( ts1.size() )
            {
               setCompressionType( ts1 );
            }
            if ( ap.argc() < 2 )
            {
               break;
            }
         }
         
         if( ap.read("--ch") || ap.read("--create-histogram") )
         {
            setCreateHistogramFlag( true );
            if ( ap.argc() < 2 )
            {
               break;
            }
         }
         
         if( ap.read("--chf") || ap.read("--create-histogram-fast") )
         {
            setCreateHistogramFastFlag( true );
            if ( ap.argc() < 2 )
            {
               break;
            }
         }
         
         if( ap.read("--create-histogram-r0") )
         {
            setCreateHistogramR0Flag( true );
            if ( ap.argc() < 2 )
            {
               break;
            }
         }
         
         if( ap.read("-d", sp1) )
         {
            setOutputDirectory( ts1 );
            if ( ap.argc() < 2 )
            {
               break;
            }
         }

         if( ap.read("-i") || ap.read("--internal-overviews") )
         {
            setInternalOverviewsFlag( true );
            if ( ap.argc() < 2 )
            {
               break;
            }
         }

         if( ap.read("--of") || ap.read("--output-files") )
         {
            setOutputFileNamesFlag( true );
            if ( ap.argc() < 2 )
            {
               break;
            }
         }
         
         if( ap.read("-o") )
         {
            setCreateOverviewsFlag( true );
            if ( ap.argc() < 2 )
            {
               break;
            }
         }        
         
         if( ap.read("--ot", sp1) )
         {
            setOverviewType( ts1 );
            if ( ap.argc() < 2 )
            {
               break;
            }
         }
         
         if( ap.read("-r") || ap.read("--rebuild-overviews") )
         {
            setRebuildOverviewsFlag( true );
            if ( ap.argc() < 2 )
            {
               break;
            }
         }

         if( ap.read("--rebuild-histogram") )
         {
            setRebuildHistogramFlag( true );
            if ( ap.argc() < 2 )
            {
               break;
            }
         }

         while(ap.read("--reader-prop", sp1))
         {
            if (ts1.size())
            {
               std::string key = READER_PROP_KW;
               key += ossimString::toString( getNextReaderPropIndex() ).string();
               addOption( key, ts1 );
            }
         }
         if ( ap.argc() < 2 )
         {
            break;
         }
         
         if( ap.read("--scanForMinMax" ) )
         {
            setScanForMinMax( true );
            if ( ap.argc() < 2 )
            {
               break;
            }
         }

         if( ap.read("--scanForMinMaxNull" ) )
         {
            setScanForMinMaxNull( true );
            if ( ap.argc() < 2 )
            {
               break;
            }
         }
         
         if( ap.read("-s", sp1) )
         {
            setOverviewStopDimension( ts1 );
            if ( ap.argc() < 2 )
            {
               break;
            }
         }

         if ( ap.read("-tile-size", sp1))
         {
            setTileSize( ossimString(ts1).toInt32() );
            if ( ap.argc() < 2 )
            {
               break;
            }
         }

         if( ap.read("--threads", sp1) )
         {
            m_kwl->addPair( THREADS_KW, ts1 );
            if ( ap.argc() < 2 )
            {
               break;
            }
         }

         while(ap.read("--writer-prop", sp1))
         {
            if (ts1.size())
            {
               std::string key = WRITER_PROP_KW;
               key += ossimString::toString( getNextWriterPropIndex() ).string();
               addOption( key, ts1 );
            }
         }
         if ( ap.argc() < 2 )
         {
            break;
         }

         // End of arg parsing.
         ap.reportRemainingOptionsAsUnrecognized();
         if ( ap.errors() )
         {
            ap.writeErrorMessages(ossimNotify(ossimNotifyLevel_NOTICE));
            std::string errMsg = "Unknown option...";
            throw ossimException(errMsg);
         }

         break; // Break from while forever.
         
      } // End while (forever) loop.

      if(ap.argc() > 1)
      {
         for (ossim_int32 i = 0; i < (ap.argc()-1); ++i)
         {
            ossimString kw = "file";
            kw += ossimString::toString(i);
            std::string value = ap[i+1];
            m_kwl->addPair(kw.string(), value, true);
         }
      }
      else
      {
         usage(ap);
         result = false;
      }

   } // not usage

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "m_kwl:\n" << *(m_kwl.get()) << "\n"
         << M << " exit result = " << (result?"true":"false")
         << "\n";
   }
   
   return result;
}

ossim_int32 ossimImageUtil::execute()
{
   static const char M[] = "ossimImageUtil::execute()";
   
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << M << " entered...\n";
   }

   if ( !m_fileWalker )
   {
      m_fileWalker = new ossimFileWalker();
   }
   
   // Get the number of "file*" keywords.
   ossim_uint32 fileCount = m_kwl->numberOf("file");

   if ( fileCount )
   {
      m_fileWalker->initializeDefaultFilterList();
      
      m_fileWalker->setNumberOfThreads( getNumberOfThreads() );

      // Must set this so we can stop recursion on directory based images.
      m_fileWalker->setWaitOnDirFlag( true );

      // This links the file walker back to our "processFile" method.
      m_fileWalker->setFileProcessor( this );
      
      // Wrap in try catch block as excptions can be thrown under the hood.
      try
      {
         // Get the list of files passed to us:
         std::vector<ossimFilename> files;
         ossim_uint32 processedFiles = 0;;
         ossim_uint32 i = 0;
         while ( processedFiles < fileCount )
         {
            ossimString kw = FILE_KW;
            kw += ossimString::toString(i);
            std::string lookup = m_kwl->findKey( kw.string() );
            if ( lookup.size() )
            {
               files.push_back( ossimFilename(lookup) );
               ++processedFiles;
            }
            
            ++i;
            if ( i > (fileCount + 100) ) break;
         }

         // Process the files:
         m_fileWalker->walk( files ); 
      }
      catch (const ossimException& e)
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "Caught exception: " << e.what() << endl;
         setErrorStatus( ossimErrorCodes::OSSIM_ERROR );
      }
      
   } // if ( fileCount )

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << M << " exit status: " << m_errorStatus << std::endl;
   }
   
   // Zero is good, non zero is bad.
   return m_errorStatus; 
}

//---
// This method is called back by the ossimFileWalker::walk method for each file it finds that it
// deems can be processed.
//---
void ossimImageUtil::processFile(const ossimFilename& file)
{
   static const char M[] = "ossimImageUtil::processFile";
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << M << " entered...\n" << "file: " << file << "\n";
   }

   ossimNotify(ossimNotifyLevel_NOTICE) << "Processing file: " << file << "\n";

   m_mutex.lock();
   ossimRefPtr<ossimImageHandler> ih =
      ossimImageHandlerRegistry::instance()->open(file, true, true);


   m_mutex.unlock();

   if ( ih.valid() && !ih->hasError() )
   {
      if ( isDirectoryBasedImage( ih.get() ) )
      {
         // Tell the walker not to recurse this directory.
         m_mutex.lock();
         m_fileWalker->setRecurseFlag(false);
         m_mutex.unlock();
      }

 
      // Set any reader props:
      ossimPropertyInterface* pi = dynamic_cast<ossimPropertyInterface*>(ih.get());
      if ( pi ) setProps(pi);

      bool consumedHistogramOptions  = false;
      bool consumedCmmOptionsOptions = false;
 
      if ( getOutputFileNamesFlag() )
      {
         // Simply output the file name of any images we can open:
         ossimNotify(ossimNotifyLevel_NOTICE) << ih->getFilename().expand(); 
      }
     
      if ( createOverviews() )
      {
         // Skip shape files...
         if ( ih->getClassName() != "ossimOgrGdalTileSource" )
         {
            createOverview(ih, consumedHistogramOptions, consumedCmmOptionsOptions);
         }
      }

      // Build stand alone histogram.  Note the overview sequencer may have computed for us.
      if ( hasHistogramOption() && !consumedHistogramOptions)
      {
         createHistogram( ih );
      }
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN) << M << "\nCould not open: " << file << std::endl;
   }
   
   if(traceDebug())
   {
      // Since ossimFileWalker is threaded output the file so we know which job exited.
      ossimNotify(ossimNotifyLevel_DEBUG) << M << "\nfile: " << file << "\nexited...\n";
   }
}

// Create overview for image:
void ossimImageUtil::createOverview(ossimRefPtr<ossimImageHandler>& ih,
                                    bool& consumedHistogramOptions,
                                    bool& /* consumedCmmOptions */)
{
   static const char M[] = "ossimImageUtil::createOverview #1";
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << M << " entered...\n";
   }
    
   if ( ih.valid() )
   {
      // Get the entry list:
      std::vector<ossim_uint32> entryList;
      ih->getEntryList(entryList);
      
      bool useEntryIndex = false;
      if ( entryList.size() )
      {
         if ( (entryList.size() > 1) || (entryList[0] != 0) ) useEntryIndex = true;
      }

      // Create the overview builder:
      ossimString overviewType;
      getOverviewType( overviewType.string() );
      ossimRefPtr<ossimOverviewBuilderBase> ob =
         ossimOverviewBuilderFactoryRegistry::instance()->createBuilder(overviewType);
      if ( ob.valid() )
      {
         // Set up any overview builder options that don't involve histograms.
         ossimPropertyInterface* pi = dynamic_cast<ossimPropertyInterface*>( ob.get() );
         if ( pi ) setProps(pi);
         
         ossim_uint32 stopDimension = getOverviewStopDimension();
         if ( stopDimension ) ob->setOverviewStopDimension(stopDimension);

         ob->setScanForMinMax( scanForMinMax() );
         
         ob->setScanForMinMaxNull( scanForMinMaxNull() );
         
         for(ossim_uint32 idx = 0; idx < entryList.size(); ++idx)
         {
            createOverview(ih, ob, entryList[idx], useEntryIndex, consumedHistogramOptions);
         }
      }
      else
      {
        ossimNotify(ossimNotifyLevel_WARN)
           << "ERROR:\nCould not create builder for:  "<< overviewType << std::endl;
        outputOverviewWriterTypes();
      }
   }

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << M << " exited...\n";
   }
}

// Create overview for entry:
void ossimImageUtil::createOverview(ossimRefPtr<ossimImageHandler>& ih,
                                    ossimRefPtr<ossimOverviewBuilderBase>& ob,
                                    ossim_uint32 entry,
                                    bool useEntryIndex,
                                    bool& consumedHistogramOptions)
{
   static const char M[] = "ossimImageUtil::createOverview #2";
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << M << " entered...\n";
   }
   
   if ( ih.valid() && ob.valid() )
   {
      if (useEntryIndex)
      {
         // Set entry before deriving file name.
         ih->setCurrentEntry(entry);
         ossimNotify(ossimNotifyLevel_NOTICE) << "entry number: "<< entry << std::endl;
      }
      
      ossimFilename outputFile =
         ih->getFilenameWithThisExtension(ossimString(".ovr"), useEntryIndex);
      
      if ( rebuildOverviews() )
      {
         ih->closeOverview(); 
         if ( outputFile.exists() )
         {
            outputFile.remove();
         }
      }

      if ( getInternalOverviewsFlag() )
      {
         if ( ih->getClassName() == "ossimTiffTileSource")
         {
            //---
            // INTERNAL_OVERVIEWS_FLAG_KW is set to true:
            // Tiff reader can handle internal overviews.  Set the output file to
            // input file.  Do it after the above remove so that if there were
            // external overviews they will get removed.
            //---
            outputFile = ih->getFilename();
         }
         else 
         {
            ossimNotify(ossimNotifyLevel_NOTICE)
               << "Internal overviews not supported for reader type: "
               <<ih->getClassName()
               << "\nIgnoring option..."
               << endl;
         }
      }

      if ( hasRequiredOverview( ih, ob ) == false )
      {
         //---
         // Set create histogram code...
         //
         // Notes:
         // 1) Must put this logic after any removal of external overview file.
         // 
         // 2) Base file could have built in overviews, e.g. jp2 files.  So the sequensor could
         //    start at R6 even if there is no external overview file.
         //
         // 3) If user want the histogram from R0 the overview builder can do as long as
         //    ossimImageHandler::getNumberOfDecimationLevels returns 1.  If we are starting
         //    overview building at R6 then we must do the create histogram in a separate path.
         //---
         ossimHistogramMode histoMode = OSSIM_HISTO_MODE_UNKNOWN;
         if ( createHistogram() ||
              ( createHistogramR0() && ( ih->getNumberOfDecimationLevels() == 1 ) ) )
         {
            histoMode = OSSIM_HISTO_MODE_NORMAL;
         }
         else if ( createHistogramFast() )
         {
            histoMode = OSSIM_HISTO_MODE_FAST;
         }
         
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG) << "Histogram mode: " << histoMode << "\n";
         }
         
         if ( histoMode != OSSIM_HISTO_MODE_UNKNOWN )
         {
            consumedHistogramOptions = true;
            ob->setHistogramMode(histoMode);
            
            ossimNotify(ossimNotifyLevel_NOTICE)
               << "Creating overviews with histogram for file: " << ih->getFilename() << std::endl;
         }
         else
         {
            if ( histoMode != OSSIM_HISTO_MODE_UNKNOWN )
            {
               consumedHistogramOptions = false;  
               ossimNotify(ossimNotifyLevel_NOTICE)
                  << "Creating overviews for file: " << ih->getFilename() << std::endl;
            }
         }
         
         ob->setOutputFile(outputFile);
         ob->setInputSource(ih.get());
         
         // Create the overview for this entry in this file:
         if ( ob->execute() == false )
         {
            setErrorStatus( ossimErrorCodes::OSSIM_ERROR );
            ossimNotify(ossimNotifyLevel_WARN)
               << "Error returned creating overviews for file: " << ih->getFilename() << std::endl;
         }
      }
      else
      {
         consumedHistogramOptions = false;
         ossimNotify(ossimNotifyLevel_NOTICE)
            << "Image has required reduced resolution data sets." << std::endl;
      }
   }

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << M << " exited...\n";
   }
}

bool ossimImageUtil::hasRequiredOverview( ossimRefPtr<ossimImageHandler>& ih,
                                          ossimRefPtr<ossimOverviewBuilderBase>& ob )
{
   bool result = false;
   if ( ih.valid() && ob.valid() && ( getCopyAllFlag() == false ) )
   {
      if ( ih->getClassName() == "ossimCcfTileSource" )
      {
         // CCF reader does not use external overviews.
         result = true;
      }
      else
      {
         // Note we always have one rset
         ossim_uint32 required = 1;
         
         ossim_uint32 startingResLevel      = ih->getNumberOfDecimationLevels();
         ossim_uint32 overviewStopDimension = ob->getOverviewStopDimension();
         
         ossim_uint32 largestImageDimension =
            ih->getNumberOfSamples(0) >
            ih->getNumberOfLines(0) ?
            ih->getNumberOfSamples(0) :
            ih->getNumberOfLines(0);
         
         while(largestImageDimension > overviewStopDimension)
         {
            largestImageDimension /= 2;
            ++required;
         }
         
         if ( startingResLevel >= required )
         {
            result = true;
         }
      }
   }
   return result;
}

void ossimImageUtil::createHistogram(ossimRefPtr<ossimImageHandler>& ih)
{
   static const char M[] = "ossimImageUtil::createHistogram #1";
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << M << " entered...\n";
   }
   
   if ( ih.valid() )
   {
      // Get the entry list:
      std::vector<ossim_uint32> entryList;
      ih->getEntryList(entryList);
      
      bool useEntryIndex = false;
      if ( entryList.size() )
      {
         if ( (entryList.size() > 1) || (entryList[0] != 0) ) useEntryIndex = true;
      }

      for(ossim_uint32 idx = 0; idx < entryList.size(); ++idx)
      {
         createHistogram(ih, entryList[idx], useEntryIndex);
      }
   }

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << M << " exited...\n";
   }
}

// Create histogram for entry:
void ossimImageUtil::createHistogram(ossimRefPtr<ossimImageHandler>& ih,
                                     ossim_uint32 entry,
                                     bool useEntryIndex)
{
   static const char M[] = "ossimImageUtil::createHistogram #2";
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << M << " entered...\n";
   }
   
   if ( ih.valid() )
   {
      if (useEntryIndex)
      {
         // Set entry before deriving file name.
         ih->setCurrentEntry(entry);
         ossimNotify(ossimNotifyLevel_NOTICE) << "entry number: "<< entry << std::endl;
      }

      ossimFilename outputFile =
         ih->getFilenameWithThisExtension(ossimString(".his"), useEntryIndex);

      // Only build if needed:
      if ( (outputFile.exists() == false) || rebuildHistogram() )
      {
         ossimNotify(ossimNotifyLevel_NOTICE)
            << "Computing histogram for file: " << ih->getFilename() << std::endl;

         // Check handler to see if it's filtering bands.
         std::vector<ossim_uint32> originalBandList(0);
         if ( ih->isBandSelector() )
         { 
            // Capture for finalize method.
            ih->getOutputBandList( originalBandList );
            
            // Set output list to input.
            ih->setOutputToInputBandList();
         }

         ossimRefPtr<ossimImageHistogramSource> histoSource = new ossimImageHistogramSource;
         ossimRefPtr<ossimHistogramWriter> writer = new ossimHistogramWriter;
         
         histoSource->setMaxNumberOfRLevels(1); // Currently hard coded...
         
#if 0 /* TODO tmp drb */
         if( !ossim::isnan(histoMin) )
         {
            histoSource->setMinValueOverride(histoMin);
         }
         
         if( !ossim::isnan(histoMax) )
         {
            histoSource->setMaxValueOverride(histoMax);
         }
         
         if(histoBins > 0)
         {
            histoSource->setNumberOfBinsOverride(histoBins);
         }
#endif
         
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "Histogram mode: " << getHistogramMode() << "\n";
         }
         
         // Connect histogram source to image handler.
         histoSource->setComputationMode( getHistogramMode() );
         histoSource->connectMyInputTo(0, ih.get() );
         histoSource->enableSource();
         
         // Connect writer to histogram source.
         writer->connectMyInputTo(0, histoSource.get());
         writer->setFilename(outputFile);
         theStdOutProgress.setFlushStreamFlag(true);
         writer->addListener(&theStdOutProgress);
         
         // Compute...
         writer->execute();
         
         writer=0;

         // Reset the band list.
         if ( ih->isBandSelector() && originalBandList.size() )
         {
            ih->setOutputBandList( originalBandList );
         }

      } // Matches: if ( (outputFile.exists() == false) || rebuildHistogram() )
      
   } // Matches: if ( ih.valid() )

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << M << " exited...\n";
   }

} // End: ossimImageUtil::createHistogram #2

void ossimImageUtil::usage(ossimArgumentParser& ap)
{
   // Add global usage options.
   ossimInit::instance()->addOptions(ap);
   
   // Set app name.
   ap.getApplicationUsage()->setApplicationName(ap.getApplicationName());

   // Add options.
   addArguments(ap);
   
   // Write usage.
   ap.getApplicationUsage()->write(ossimNotify(ossimNotifyLevel_INFO));

   outputOverviewWriterTypes();
   
   ossimNotify(ossimNotifyLevel_INFO)
      << "\nExample commands:\n"
      << "\n// A single image standard tiff overviews, histogram:\n"
      << "ossim-preproc -o --ch <file>\n"
      << "\n// A single image with j2k overviews(requires kakadu plugin), histogram:\n"
      << "ossim-preproc --ot ossim_kakadu_nitf_j2k --ch <file>\n"
      << "\n// j2k, histogram, 4 threads\n"
      << "\n// standard tiff overviews, full histogram, 4 threads:\n"
      << "ossim-preproc -r -o --ch --threads 4 <directory_to_walk>\n"
      << "\n// j2k, histogram (fast mode), 4 threads\n"
      << "ossim-preproc -r --ot ossim_kakadu_nitf_j2k --chf --threads 4 "
      << "<directory_to_walk>\n"
      << "\n// tiff, jpeg compression, histogram, 4 threads\n"
      << "ossim-preproc -r --ch --compression-quality 75 --compression-type "
      << "jpeg --threads 4 <directory_to_walk>\n"
      << "\nNOTES:\n"
      << "\n  --ch  equals --create-histogram"
      << "\n  --chf equals --create-histogram-fast"
      << std::endl;
}

// Private method:
void ossimImageUtil::outputOverviewWriterTypes() const
{
   ossimNotify(ossimNotifyLevel_NOTICE)
      << "\nValid overview types: " << std::endl;
   
   std::vector<ossimString> outputType;
   
   ossimOverviewBuilderFactoryRegistry::instance()->getTypeNameList(outputType);
   std::copy(outputType.begin(),
             outputType.end(),
             std::ostream_iterator<ossimString>(ossimNotify(ossimNotifyLevel_NOTICE), "\t\n"));
}

// Private method:
bool ossimImageUtil::isDirectoryBasedImage(const ossimImageHandler* ih) const
{
   bool result = false;
   if ( ih )
   {
      // Get the image handler name.
      ossimString imageHandlerName = ih->getClassName();
      if ( (imageHandlerName == "ossimAdrgTileSource") ||
           (imageHandlerName == "ossimCibCadrgTileSource") )  
      {
         result = true;
      }
   }
   return result;
}

void ossimImageUtil::setCreateOverviewsFlag( bool flag )
{
   addOption( CREATE_OVERVIEWS_KW, ( flag ? TRUE_KW : FALSE_KW ) );
}

bool ossimImageUtil::createOverviews() const
{
   bool result = false;
   std::string lookup = m_kwl->findKey( CREATE_OVERVIEWS_KW );
   if ( lookup.size() )
   {
      result = ossimString(lookup).toBool();
   }
   return result;
}

void ossimImageUtil::setRebuildOverviewsFlag( bool flag )
{
   addOption( REBUILD_OVERVIEWS_KW, ( flag ? TRUE_KW : FALSE_KW ) );
   if ( flag )
   {
      setCreateOverviewsFlag( true ); // Turn on overview building.
   }
}

bool ossimImageUtil::rebuildOverviews() const
{
   bool result = false;
   std::string lookup = m_kwl->findKey( REBUILD_OVERVIEWS_KW );
   if ( lookup.size() )
   {
      result = ossimString(lookup).toBool();
   }
   return result;
}

void ossimImageUtil::setRebuildHistogramFlag( bool flag )
{
   addOption( REBUILD_HISTOGRAM_KW, ( flag ? TRUE_KW : FALSE_KW ) );
   if ( flag )
   {
      setCreateHistogramFlag( true ); // Turn on histogram building.
   }
}

bool ossimImageUtil::rebuildHistogram() const
{
   bool result = false;
   std::string lookup = m_kwl->findKey( REBUILD_HISTOGRAM_KW );
   if ( lookup.size() )
   {
      result = ossimString(lookup).toBool();
   }
   return result;
}

void ossimImageUtil::setScanForMinMax( bool flag )
{
   addOption( SCAN_MIN_MAX_KW, ( flag ? TRUE_KW : FALSE_KW ) ); 
}

bool ossimImageUtil::scanForMinMax() const
{
   bool result = false;
   std::string lookup = m_kwl->findKey( SCAN_MIN_MAX_KW );
   if ( lookup.size() )
   {
      result = ossimString(lookup).toBool();
   }
   return result;
}

void ossimImageUtil::setScanForMinMaxNull( bool flag )
{
   addOption( SCAN_MIN_MAX_NULL_KW, ( flag ? TRUE_KW : FALSE_KW ) ); 
}

bool ossimImageUtil::scanForMinMaxNull() const
{
   bool result = false;
   std::string lookup = m_kwl->findKey( SCAN_MIN_MAX_NULL_KW );
   if ( lookup.size() )
   {
      result = ossimString(lookup).toBool();
   }
   return result;
}

void ossimImageUtil::setCompressionQuality( const std::string& quality )
{
   if ( quality.size() )
   {
      std::string key = WRITER_PROP_KW;
      key += ossimString::toString( getNextWriterPropIndex() ).string();
      std::string value = ossimKeywordNames::COMPRESSION_QUALITY_KW;
      value += "=";
      value += quality;
      addOption( key, value );
   }
}

void ossimImageUtil::setCompressionType(const std::string& type)
{
   if ( type.size() )
   {
      std::string key = WRITER_PROP_KW;
      key += ossimString::toString( getNextWriterPropIndex() ).string();
      std::string value = ossimKeywordNames::COMPRESSION_TYPE_KW;
      value += "=";
      value += type;
      addOption( key, value );
   }   
}

void ossimImageUtil::setCopyAllFlag( bool flag )
{
   // Add this for hasRequiredOverview method.
   std::string key   = COPY_ALL_FLAG_KW;
   std::string value = ( flag ? TRUE_KW : FALSE_KW );
   addOption( key, value );

   // Add as a writer prop:
   key = WRITER_PROP_KW;
   key += ossimString::toString( getNextWriterPropIndex() ).string();
   value = COPY_ALL_FLAG_KW;
   value += "=";
   value += ( flag ? TRUE_KW : FALSE_KW );
   addOption( key, value );
}

bool ossimImageUtil::getCopyAllFlag() const
{
   bool result = false;
   std::string lookup = m_kwl->findKey( COPY_ALL_FLAG_KW );
   if ( lookup.size() )
   {
      ossimString os(lookup);
      result = os.toBool();
   }
   return result;
}

void ossimImageUtil::setInternalOverviewsFlag( bool flag )
{
   // Add this for hasRequiredOverview method.
   std::string key   = INTERNAL_OVERVIEWS_FLAG_KW;
   std::string value = ( flag ? TRUE_KW : FALSE_KW );
   addOption( key, value );

   // Add as a writer prop:
   key = WRITER_PROP_KW;
   key += ossimString::toString( getNextWriterPropIndex() ).string();
   value = INTERNAL_OVERVIEWS_FLAG_KW;
   value += "=";
   value += ( flag ? TRUE_KW : FALSE_KW );
   addOption( key, value );
}

bool ossimImageUtil::getInternalOverviewsFlag() const
{
   bool result = false;
   std::string lookup = m_kwl->findKey( INTERNAL_OVERVIEWS_FLAG_KW );
   if ( lookup.size() )
   {
      ossimString os(lookup);
      result = os.toBool();
   }
   return result;
}

void ossimImageUtil::setOutputFileNamesFlag( bool flag )
{
   std::string key   = OUTPUT_FILENAMES_KW;
   std::string value = ( flag ? TRUE_KW : FALSE_KW );
   addOption( key, value );
}

bool ossimImageUtil::getOutputFileNamesFlag() const
{
   bool result = false;
   std::string lookup = m_kwl->findKey( OUTPUT_FILENAMES_KW );
   if ( lookup.size() )
   {
      ossimString os(lookup);
      result = os.toBool();
   }
   return result;
}

void ossimImageUtil::setOutputDirectory( const std::string& directory )
{
   std::string key = OUTPUT_DIRECTORY_KW;
   addOption( key, directory );
}
   
void ossimImageUtil::setOverviewType( const std::string& type )
{
   std::string key = OVERVIEW_TYPE_KW;
   addOption( key, type );
   setCreateOverviewsFlag( true ); // Assume caller wants overviews.
}

void ossimImageUtil::getOverviewType(std::string& type) const
{
   std::string lookup = m_kwl->findKey(OVERVIEW_TYPE_KW);
   if ( lookup.size() )
   {
      type = lookup;
   }
   else
   {
      type = "ossim_tiff_box"; // default
   }
}

void ossimImageUtil::setProps(ossimPropertyInterface* pi) const
{
   if ( pi )
   {
      ossimString baseKey;
      if ( dynamic_cast<ossimImageHandler*>(pi) )
      {
         baseKey = READER_PROP_KW;
      }
      else
      {
         baseKey = WRITER_PROP_KW;
      }
      
      ossim_uint32 propCount = m_kwl->numberOf( baseKey.c_str() );
      if ( propCount )
      {
         ossim_uint32 foundProps = 0;
         ossim_uint32 index = 0;
         
         // (propCount+100) is to allow for holes like reader_prop0, reader_prop2...
         while ( (foundProps < propCount) && (index < (propCount+100) ) ) 
         {
            ossimString key = baseKey;
            key += ossimString::toString(index);
            std::string lookup = m_kwl->findKey( key.string() );
            if ( lookup.size() )
            {
               ossimString value = lookup;
               std::vector<ossimString> v = value.split("=");
               if (v.size() == 2)
               {
                  ossimString propertyName  = v[0];
                  ossimString propertyValue = v[1];
                  ossimRefPtr<ossimProperty> p =
                     new ossimStringProperty(propertyName, propertyValue);
                  pi->setProperty( p );
               }
               ++foundProps;
            }
            ++index;
         }
      }
   }
}

void ossimImageUtil::setOverviewStopDimension( ossim_uint32 dimension )
{
   addOption( OVERVIEW_STOP_DIM_KW, dimension );
}

void ossimImageUtil::setOverviewStopDimension( const std::string& dimension )
{
   addOption( OVERVIEW_STOP_DIM_KW, dimension );
}

void ossimImageUtil::setTileSize( ossim_uint32 tileSize )
{
   if ((tileSize % 16) == 0)
   {
      addOption( TILE_SIZE_KW, tileSize );
   }
   else
   {
      ossimNotify(ossimNotifyLevel_NOTICE)
         << "ossimImageUtil::setTileSize NOTICE:"
         << "\nTile width must be a multiple of 16!"
         << std::endl;
   }
}


ossim_uint32 ossimImageUtil::getOverviewStopDimension() const
{
   ossim_uint32 result = 0;
   std::string lookup = m_kwl->findKey( OVERVIEW_STOP_DIM_KW );
   if ( lookup.size() )
   {
      result = ossimString(lookup).toUInt32();
   }
   return result;
}

void ossimImageUtil::setCreateHistogramFlag( bool flag )
{
   addOption( CREATE_HISTOGRAM_KW, ( flag ? TRUE_KW : FALSE_KW ) );
}

bool ossimImageUtil::createHistogram() const
{
   bool result = false;
   std::string lookup = m_kwl->findKey( CREATE_HISTOGRAM_KW );
   if ( lookup.size() )
   {
      result = ossimString(lookup).toBool();
   }
   return result;
}

void ossimImageUtil::setCreateHistogramFastFlag( bool flag )
{
   addOption( CREATE_HISTOGRAM_FAST_KW, ( flag ? TRUE_KW : FALSE_KW ) );
}

bool ossimImageUtil::createHistogramFast() const
{
   bool result = false;
   std::string lookup = m_kwl->findKey( CREATE_HISTOGRAM_FAST_KW );
   if ( lookup.size() )
   {
      result = ossimString(lookup).toBool();
   }
   return result;
}

void ossimImageUtil::setCreateHistogramR0Flag( bool flag )
{
   addOption( CREATE_HISTOGRAM_R0_KW, ( flag ? TRUE_KW : FALSE_KW ) );
}

bool ossimImageUtil::createHistogramR0() const
{   bool result = false;
   std::string lookup = m_kwl->findKey( CREATE_HISTOGRAM_R0_KW );
   if ( lookup.size() )
   {
      result = ossimString(lookup).toBool();
   }
   return result;
}

bool ossimImageUtil::hasHistogramOption() const
{
   return ( createHistogram() || createHistogramFast() || createHistogramR0() );
}

ossimHistogramMode ossimImageUtil::getHistogramMode() const
{
   ossimHistogramMode result = OSSIM_HISTO_MODE_UNKNOWN;
   if ( createHistogram() || createHistogramR0() )
   {
      result = OSSIM_HISTO_MODE_NORMAL;
   }
   else if ( createHistogramFast() )
   {
      result = OSSIM_HISTO_MODE_FAST;
   }
   return result;
}

void ossimImageUtil::setNumberOfThreads( ossim_uint32 threads )
{
   addOption( THREADS_KW, threads );
}

void ossimImageUtil::setNumberOfThreads( const std::string& threads )
{
   addOption( THREADS_KW, threads );
}

ossim_uint32 ossimImageUtil::getNumberOfThreads() const
{
   ossim_uint32 result;
   std::string lookup = m_kwl->findKey( THREADS_KW );
   if ( lookup.size() )
   {
      result = ossimString(lookup).toUInt32();
   }
   else
   {
      result = ossim::getNumberOfThreads();
   }
   return result;
}

ossim_uint32 ossimImageUtil::getNextWriterPropIndex() const
{
   ossim_uint32 result = m_kwl->numberOf( WRITER_PROP_KW.c_str() );
   if ( result )
   {
      ossim_uint32 foundProps = 0;
      ossim_uint32 index = 0;

      //---
      // Loop until we find the last index used for WRITER_PROP_KW.
      // (result+100) is to allow for holes like writer_prop0, writer_prop2...
      //---
      while ( (foundProps < result) && (index < (result+100) ) ) 
      {
         ossimString key = WRITER_PROP_KW;
         key += ossimString::toString(index);
         std::string lookup = m_kwl->findKey( key.string() );
         if ( lookup.size() )
         {
            ++foundProps;
         }
         ++index;
      }
      result = index;
   }
   return result;
}

ossim_uint32 ossimImageUtil::getNextReaderPropIndex() const
{
   ossim_uint32 result = m_kwl->numberOf( READER_PROP_KW.c_str() );
   if ( result )
   {
      ossim_uint32 foundProps = 0;
      ossim_uint32 index = 0;
      
      //---
      // Loop until we find the last index used for WRITER_PROP_KW.
      // (result+100) is to allow for holes like reader_prop0, reader_prop2...
      //---
      while ( (foundProps < result) && (index < (result+100) ) ) 
      {
         ossimString key = READER_PROP_KW;
         key += ossimString::toString(index);
         std::string lookup = m_kwl->findKey( key.string() );
         if ( lookup.size() )
         {
            ++foundProps;
         }
         ++index;
      }
      result = index;
   }
   return result;
}

void ossimImageUtil::addOption( const std::string& key, ossim_uint32 value )
{
   addOption( key, ossimString::toString( value ).string() );
}

void ossimImageUtil::addOption(  const std::string& key, const std::string& value )
{
   m_mutex.lock();
   if ( m_kwl.valid() )
   {
      if ( key.size() && value.size() )
      {
         m_kwl->addPair( key, value );
      }
   }
   m_mutex.unlock();
}

void ossimImageUtil::setErrorStatus( ossim_int32 status )
{
   m_mutex.lock();
   m_errorStatus = status;
   m_mutex.unlock();
}
