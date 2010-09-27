//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Utility class definition processing digital elevation
// models(dems).
// 
//----------------------------------------------------------------------------
// $Id$

#include <string>

#include <ossim/util/ossimDemUtil.h>

#include <ossim/base/ossimArgumentParser.h>
#include <ossim/base/ossimApplicationUsage.h>
#include <ossim/base/ossimConnectableObject.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimProperty.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimStdOutProgress.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/base/ossimTrace.h>

#include <ossim/imaging/ossimBumpShadeTileSource.h>
#include <ossim/imaging/ossimFilterResampler.h>
#include <ossim/imaging/ossimImageFileWriter.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageMosaic.h>
#include <ossim/imaging/ossimImageRenderer.h>
#include <ossim/imaging/ossimImageSource.h>
#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/imaging/ossimImageToPlaneNormalFilter.h>
#include <ossim/imaging/ossimImageWriterFactoryRegistry.h>
#include <ossim/imaging/ossimIndexToRgbLutFilter.h>
#include <ossim/imaging/ossimScalarRemapper.h>

#include <ossim/init/ossimInit.h>

#include <ossim/projection/ossimEquDistCylProjection.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimUtmProjection.h>

#include <ossim/support_data/ossimSrcRecord.h>

static ossimTrace traceDebug   = ossimTrace("ossimDemUtil:debug");
static ossimTrace traceLog     = ossimTrace("ossimDemUtil:log");
static ossimTrace traceOptions = ossimTrace("ossimDemUtil:options");


static const char COLOR_BLUE_KW[]           = "color_blue";
static const char COLOR_GREEN_KW[]          = "color_green";
static const char COLOR_RED_KW[]            = "color_red";
static const char CUT_CENTER_LAT_KW[]       = "cut_center_latitude";
static const char CUT_CENTER_LON_KW[]       = "cut_center_longitude";
static const char CUT_CENTER_HEIGHT_KW[]    = "cut_center_height"; // pixels
static const char CUT_CENTER_WIDTH_KW[]     = "cut_center_width";  // pixels
static const char CUT_MAX_LAT_KW[]          = "cut_maximum_latitude";
static const char CUT_MAX_LON_KW[]          = "cut_maximum_longitude";
static const char CUT_MIN_LAT_KW[]          = "cut_minimum_latitude";
static const char CUT_MIN_LON_KW[]          = "cut_minimum_longitude";
static const char DEM_KW[]                  = "dem";
static const char EXAGGERATION_KW[]         = "exaggeration";
static const char FILE_KW[]                 = "file";
static const char HISTO_OP_KW[]             = "hist-op";
static const char IMG_KW[]                  = "image";
static const char LUT_FILE_KW[]             = "lut_file";
static const char METERS_KW[]               = "meters";
static const char OP_KW[]                   = "operation";
static const char RESAMPLER_FILTER_KW[]     = "resampler_filter";
static const char SCALE_2_8_BIT_KW[]        = "scale_2_8_bit";
static const char SMOOTHNESS_FACTOR_KW[]    = "smoothness_factor";
static const char SRC_FILE_KW[]             = "src_file";
static const char SRS_KW[]                  = "srs";
static const char THUMBNAIL_RESOLUTION_KW[] = "thumbnail_resolution"; // pixels
static const char WRITER_KW[]               = "writer";
static const char WRITER_PROPERTY_KW[]      = "writer_property";

ossimDemUtil::ossimDemUtil()
   : ossimReferenced(),
     m_operation(OSSIM_DEM_OP_UNKNOWN),
     m_kwl(new ossimKeywordlist()),
     m_srcKwl(0),
     m_outputProjection(0),
     m_demLayer(0),
     m_imgLayer(0)
{
}

ossimDemUtil::~ossimDemUtil()
{
}

void ossimDemUtil::addArguments(ossimArgumentParser& ap)
{
   ossimString usageString = ap.getApplicationName();
   usageString += " [option]... [input-option]... <input-file(s)> <output-file>\nNote at least one input is required either from one of the input options, e.g. --input-dem <my-dem.hgt> or adding to command line in front of the output file in which case the code will try to ascertain what type of input it is.\n\nAvailable traces:\n-T \"ossimDemUtil:debug\"   - General debug trace to standard out.\n-T \"ossimDemUtil:log\"     - Writes a log file to output-file.log.\n-T \"ossimDemUtil:options\" - Writes the options to output-file-options.kwl.";

   ossimApplicationUsage* appuse = ap.getApplicationUsage();
   
   appuse->setCommandLineUsage(usageString);
   
   appuse->setDescription(ap.getApplicationName()+" Utility application for generating elevation products from dem data.");
   
   appuse->addCommandLineOption("--azimuth", "<azimuth>\nhillshade option - Light source azimuth angle for bump shade.\nRange: 0 to 360, Default = 180.0");

   appuse->addCommandLineOption("--central-meridian","<central_meridian_in_decimal_degrees>\nNote if set this will be used for the central meridian of the projection.  This can be used to lock the utm zone.");

   appuse->addCommandLineOption("--color","<r> <g> <b>\nhillshade option - Set the red, green and blue color values to be used with hillshade.\nThis option can be used with or without an image source for color.\nRange 0 to 255, Defualt r=255, g=255, b=255");

   appuse->addCommandLineOption("--color-table","<color-table.kwl>\nhillshade or color-relief option - Keyword list containing color table for color-relief option.");

   appuse->addCommandLineOption("--cut-bbox-ll", "<min_lat> <min_lon> <max_lat> <max_lon>\nSpecify a bounding box with the minimum latitude/longitude and max latitude/longitude in decimal degrees.");
   
   appuse->addCommandLineOption("--cut-center-llwh","<latitude> <longitude> <width> <height>\nSpecify the center cut in latitude longitude space with width and height in pixels.");

   appuse->addCommandLineOption("--elevation", "<elevation>\nhillshade option - Light source elevation angle for bumb shade.\nRange: 0 to 90, Default = 45.0");
   
   appuse->addCommandLineOption("--exaggeration", "<exageration>\nExaggeration for image to plane noraml filter.\nRange: .0001 to 50000, Default = 1.0");

   appuse->addCommandLineOption("-h or --help", "Display this help and exit.");

   appuse->addCommandLineOption("--histogram-op", "<operation>\nHistogram operation to perform. Valid operations are \"auto-minmax\", \"std-stretch-1\", \"std-stretch-2\" and \"std-stretch-3\".");

   appuse->addCommandLineOption("--input-dem", "<dem> Input dem to process.");

   appuse->addCommandLineOption("--input-img", "<image> Input image to process.");
   
   appuse->addCommandLineOption("--input-src","<file.src> Input source file list keyword list with list of dems or images or both to process.");
   
   appuse->addCommandLineOption("--meters", "<meters>\nSpecifies an override for the meters per pixel");
      
   appuse->addCommandLineOption("--op","<operation>\nOperation to perform. Valid operations are \"color-relief\", \"hillshade\" and \"ortho\".");

   appuse->addCommandLineOption("--options-keyword-list","<options.kwl>  This can be all or part of the application options.  To get a template you can turn on trace to the ossimDemUtil class by adding \"-T ossimDemUtil\" to your command.");

   appuse->addCommandLineOption("--origin-latitude","<latidude_in_decimal_degrees>\nNote if set this will be used for the origin latitude of the projection.  Setting this to something other than 0.0 with a geographic projection creates a scaled geographic projection.");

   appuse->addCommandLineOption("--projection", "<output_projection> Can be input, geo, geo-scaled, or utm.\nIf input and multiple sources the projection of the first image will be used.\nIf geo-scaled the origin of latitude will be set to scene center.\nIf utm the zone will be set from the scene center.\nIf --srs is used it takes precident over this option.");
   
   appuse->addCommandLineOption(
      "--resample-filter","<type>\nSpecify what resampler filter to use, e.g. nearest neighbor, bilinear, cubic.\nSee ossim-info ----resampler-filters"); 

   appuse->addCommandLineOption("--scale-to-8-bit","Scales output to eight bits if not already.");
   
   appuse->addCommandLineOption("--smoothness-factor","<factor>\nhillshade operation - Smoothness factor for image to plane noraml filter.\nRange: .0001 to 40, Default = 1.0");

   appuse->addCommandLineOption("--srs","<src_code>\nSpecify an output reference frame/projection. Example: --srs EPSG:4326");

   appuse->addCommandLineOption("-t or --thumbnail", "<max_dimension>\nSpecify a thumbnail resolution.\nScale will be adjusted so the maximum dimension = argument given.");
   
   appuse->addCommandLineOption(
      "-w or --writer","<writer>\nSpecifies the output writer.  Default uses output file extension to "
      "determine writer.");
   
   appuse->addCommandLineOption("--writer-prop", "<writer-property>\nPasses a name=value pair to the writer for setting it's property. Any number of these can appear on the line.");
}

bool ossimDemUtil::initialize(ossimArgumentParser& ap)
{
   static const char MODULE[] = "ossimDemUtil::initialize(ossimArgumentParser&)";

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }
   
   //---
   // Start with clean options keyword list.
   //---
   m_kwl->clear();
   
   ossimString tempString1;
   ossimArgumentParser::ossimParameter stringParam1(tempString1);
   ossimString tempString2;
   ossimArgumentParser::ossimParameter stringParam2(tempString2);
   ossimString tempString3;
   ossimArgumentParser::ossimParameter stringParam3(tempString3);
   ossimString tempString4;
   ossimArgumentParser::ossimParameter stringParam4(tempString4);

   if( ap.read("-h") || ap.read("--help") || (ap.argc() == 1) )
   {
      // Print the usage from the arg parser.
      ap.getApplicationUsage()->write( ossimNotify(ossimNotifyLevel_NOTICE) );

      // Print the valid writer types.
      ossimNotify(ossimNotifyLevel_NOTICE)
         << "\nValid output writer types for \"-w\" or \"--writer\" option:\n\n";
      ossimImageWriterFactoryRegistry::instance()->
         printImageTypeList( ossimNotify(ossimNotifyLevel_NOTICE) );
      
      std::string errMsg = "usage";
      return false; // indicates process should be terminated
   }   

   //---
   // Extract optional arguments.
   //---
   ossim_uint32 demIdx  = 0;
   ossim_uint32 imgIdx  = 0;
   ossim_uint32 propIdx = 0;
   ossimString  key     = "";
   
   //---
   // Extract optional arguments and stuff them in a keyword list.
   //---
   if( ap.read("--azimuth", stringParam1) )
   {
      m_kwl->add( ossimKeywordNames::AZIMUTH_ANGLE_KW, tempString1.c_str() );
   }
   
   if( ap.read("--central-meridian") )
   {
      m_kwl->add( ossimKeywordNames::CENTRAL_MERIDIAN_KW, tempString1.c_str() );
   }

   if( ap.read("--color", stringParam1, stringParam2, stringParam3) )
   {
      m_kwl->add( COLOR_RED_KW,   tempString1.c_str() );
      m_kwl->add( COLOR_GREEN_KW, tempString2.c_str() );
      m_kwl->add( COLOR_BLUE_KW,  tempString3.c_str() );
   }

   if( ap.read("--color-table", stringParam1) )
   {
      m_kwl->add( LUT_FILE_KW, tempString1.c_str() );
   }

   if( ap.read("--cut-bbox-ll", stringParam1, stringParam2, stringParam3, stringParam4) )
   {
      m_kwl->add( CUT_MIN_LAT_KW, tempString1.c_str() );
      m_kwl->add( CUT_MIN_LON_KW, tempString2.c_str() );
      m_kwl->add( CUT_MAX_LAT_KW, tempString3.c_str() );
      m_kwl->add( CUT_MAX_LON_KW, tempString4.c_str() );
   }
   
   if( ap.read("--cut-center-llwh", stringParam1, stringParam2, stringParam3, stringParam4) )
   {
      m_kwl->add( CUT_CENTER_LAT_KW,    tempString1.c_str() );
      m_kwl->add( CUT_CENTER_LON_KW,    tempString2.c_str() );
      m_kwl->add( CUT_CENTER_WIDTH_KW,  tempString3.c_str() );
      m_kwl->add( CUT_CENTER_HEIGHT_KW, tempString4.c_str() );
   }

   if ( ap.read("--histogram-op", stringParam1) )
   {
      m_kwl->add( HISTO_OP_KW, tempString1.c_str() );
   }
   
   while( ap.read("--input-dem", stringParam1) )
   {
      key = DEM_KW;
      key += ossimString::toString(demIdx);
      key += ".";
      key += FILE_KW;
      m_kwl->add( key, tempString1.c_str() );
      ++demIdx;
   }
   
   while( ap.read("--input-img", stringParam1) )
   {
      key = IMG_KW;
      key += ossimString::toString(imgIdx);
      key += ".";
      key += FILE_KW;
      m_kwl->add(key, tempString1.c_str() );
      ++imgIdx;
   }

   if( ap.read("--input-src", stringParam1) )
   {
      m_kwl->add( SRC_FILE_KW, tempString1.c_str() );
   }
   
   if( ap.read("--elevation", stringParam1) )
   {
      m_kwl->add( ossimKeywordNames::ELEVATION_ANGLE_KW, tempString1.c_str() );
   }
   
   if( ap.read("--exaggeration", stringParam1) )
   {
      m_kwl->add( EXAGGERATION_KW, tempString1.c_str() );
   }

   if( ap.read("--meters", stringParam1) )
   {
      m_kwl->add( METERS_KW, tempString1.c_str() );
   }

   if( ap.read("--op", stringParam1) )
   {
      m_kwl->add( OP_KW, tempString1.c_str() );
   }

   if( ap.read("--options-keyword-list", stringParam1) )
   {
      ossimFilename optionsKwl = tempString1;
      if ( optionsKwl.exists() )
      {
         if ( m_kwl->addFile(optionsKwl) == false )
         {
            std::string errMsg = "ERROR could not open options keyword list file: ";
            errMsg += optionsKwl;
            throw ossimException(errMsg);
         }
      }
      else
      {
         std::string errMsg = "ERROR options keyword list file does not exists: ";
         errMsg += optionsKwl;
         throw ossimException(errMsg); 
      }
   }
   
   if( ap.read("--origin-latitude", stringParam1) )
   {
      m_kwl->add( ossimKeywordNames::ORIGIN_LATITUDE_KW, tempString1.c_str() );
   }

   if( ap.read("--projection", stringParam1) )
   {
      m_kwl->add( ossimKeywordNames::PROJECTION_KW, tempString1.c_str() );
   }

   if( ap.read("--origin-latitude", stringParam1) )
   {
      m_kwl->add( ossimKeywordNames::ORIGIN_LATITUDE_KW, tempString1.c_str() );
   }

   if( ap.read("--resample-filter", stringParam1) )
   {
      m_kwl->add( RESAMPLER_FILTER_KW, tempString1.c_str() );
   }

   if ( ap.read("--scale-to-8-bit") )
   {
      m_kwl->add( SCALE_2_8_BIT_KW, "true");
   }

   if( ap.read("--smoothness-factor", stringParam1) )
   {
      m_kwl->add( SMOOTHNESS_FACTOR_KW, tempString1.c_str() );
   }
   
   if( ap.read("--srs", stringParam1) )
   {
      if ( tempString1.contains("EPSG:") )
      {
         tempString1.gsub( ossimString("EPSG:"), ossimString("") );
      }
      m_kwl->add( SRS_KW, tempString1.c_str() );
   }

   if( ap.read("-t", stringParam1) || ap.read("--thumbnail", stringParam1) )
   {
      m_kwl->add( THUMBNAIL_RESOLUTION_KW, tempString1.c_str() );
   }   

   if( ap.read("-w", stringParam1) || ap.read("--writer", stringParam1) )
   {
      m_kwl->add( WRITER_KW, tempString1); 
   }

   while (ap.read("--writer-prop", stringParam1))
   {
      key = WRITER_PROPERTY_KW;
      key += ossimString::toString(propIdx);
      m_kwl->add(key, tempString1.c_str() );
      ++propIdx;
   }

   // End of arg parsing.
   ap.reportRemainingOptionsAsUnrecognized();
   if ( ap.errors() )
   {
      ap.writeErrorMessages(ossimNotify(ossimNotifyLevel_NOTICE));
      std::string errMsg = "Unknown option...";
      throw ossimException(errMsg);
   }

   if ( ap.argc() >= 2 )
   {
      // Output file is last arg:
      m_kwl->add( ossimKeywordNames::OUTPUT_FILE_KW, ap[ap.argc()-1]);
   }
   else
   {
      if (  !m_kwl->find(ossimKeywordNames::OUTPUT_FILE_KW) )
      {
         ap.writeErrorMessages(ossimNotify(ossimNotifyLevel_NOTICE));
         std::string errMsg = "Must supply an output file.";
         throw ossimException(errMsg);
      }
   }

   if ( ap.argc() > 2 ) // User passed inputs in front of output file.
   {
      int pos = 1; // ap.argv[0] is application name. 
      while ( pos < (ap.argc()-1) )
      {
         ossimFilename file = ap[pos];
         if ( traceDebug() )
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "argv[" << pos << "]: " << file << "\n";
         }
         
         if ( isDemFile(file) )
         {
            key = DEM_KW;
            key += ossimString::toString(demIdx);
            key += ".";
            key += FILE_KW;
            m_kwl->add( key, file.c_str() );
            ++demIdx;
         }
         else if ( isSrcFile(file) ) 
         {
            if ( m_kwl->find( SRC_FILE_KW ) ) // --input-src used also
            {
               std::string errMsg = MODULE;
               errMsg += "ERROR Multiple src files passed in.  Please combine into one.";
               throw ossimException(errMsg);
            }
            
            m_kwl->add( SRC_FILE_KW, file.c_str() );
         }
         else // Add as an input image.
         {
            key = IMG_KW;
            key += ossimString::toString(imgIdx);
            key += ".";
            key += FILE_KW;
            m_kwl->add(key, file.c_str() );
            ++imgIdx;
         }
         
         ++pos; // Go to next arg...
         
      } // End: while ( pos < (ap.argc()-1) )
      
   } // End: if ( ap.argc() > 2 )

   initialize();
   
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " exited..." << std::endl;
   }  
   return true;
   
} // End: void ossimDemUtil::initialize(ossimArgumentParser& ap)

void ossimDemUtil::initialize()
{
   static const char MODULE[] = "ossimDemUtil::initialize()";

   if ( traceOptions() )
   {
      ossimFilename optionsFile;
      getOutputFilename(optionsFile);
      optionsFile = optionsFile.noExtension();
      optionsFile += "-options.kwl";
      ossimString comment = " Can be use for --options-keyword-list argument.";
      m_kwl->write( optionsFile.c_str(), comment.c_str() );
   } 

   // Determine the operation to do.
   const char* lookup = m_kwl->find(OP_KW);
   if ( lookup )
   {
      ossimString s = lookup;
      s.downcase();
      
      if ( s == "hillshade" )
      {
         m_operation = OSSIM_DEM_OP_HILL_SHADE;
      }
      else if ( s == "color-relief" )
      {
         m_operation = OSSIM_DEM_OP_COLOR_RELIEF;
      }
      else if ( s == "ortho" )
      {
         m_operation = OSSIM_DEM_OP_ORTHO;
      }
      else
      {
         std::string errMsg = "unknown operation: ";
         errMsg += s;
         throw ossimException(errMsg);
      }
   }
   else
   {
      std::string errMsg = "keyword not found: ";
      errMsg += OP_KW;
      errMsg += "\nUse --op option to specify operation.\n";
      throw ossimException(errMsg);  
   }

   //---
   // Populate the m_srcKwl if --src option was set.
   // Note do this before creating chains.
   //---
   initializeSrcKwl();

   // Create chains for any dem sources.
   addDemSources();

   // Create chains for any image sources.
   addImgSources();

   // Create the output projection.
   createOutputProjection();

   // Setup the view in all the chains.
   propagateOutputProjectionToChains();
   
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " exited...\n";
   }

} // End: void ossimDemUtil::initialize()

void ossimDemUtil::execute()
{
   static const char MODULE[] = "ossimDemUtil::execute";

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }

   const char* lookup = 0;  // used throughout...

   ossimRefPtr<ossimImageSource> source = 0;

   if ( hasBumpShadeArg() )
   {
      // Combine the dems.
      ossimRefPtr<ossimImageSource> demSource = combineLayers( m_demLayer );

      // Set up the normal source.
      ossimRefPtr<ossimImageToPlaneNormalFilter> normSource = new ossimImageToPlaneNormalFilter;

      // Set the exaggeration.
      ossim_float64 exaggeration = 1.0;
      lookup = m_kwl->find( EXAGGERATION_KW );
      if ( lookup )
      {
         exaggeration = ossimString::toFloat64(lookup);
      }
      normSource->setXScale(exaggeration);
      normSource->setXScale(exaggeration);
      
      // Set the smoothness factor.
      ossim_float64 smoothnessFactor = 1.0;
      lookup = m_kwl->find( SMOOTHNESS_FACTOR_KW );
      if ( lookup )
      {
         smoothnessFactor = ossimString::toFloat64(lookup);
      }
      normSource->setSmoothnessFactor(smoothnessFactor);
      
      // Connect to dems.
      normSource->connectMyInputTo( demSource.get() );
      
      ossimRefPtr<ossimImageSource> colorSource = 0;
      if ( hasLutFile() )
      {
         if ( m_imgLayer.size() )
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE << " WARNING:"
               << "\nBoth a color table and image(s) have been provided for a color source.\n"
               << "Choosing color table of image(s).\n";
         }

         colorSource = addIndexToRgbLutFilter( demSource );
      }
      else
      {
         // Combine the images and set as color source for bump shade.
         colorSource = combineLayers( m_imgLayer );
      }

      // Create the bump shade.
      ossimRefPtr<ossimBumpShadeTileSource> bumpShade = new ossimBumpShadeTileSource;

      // Set the azimuth angle.
      ossim_float64 azimuthAngle = 180;
      lookup = m_kwl->find( ossimKeywordNames::AZIMUTH_ANGLE_KW );
      if ( lookup )
      {
         ossim_float64 f = ossimString::toFloat64(lookup);
         if ( (f >= 0) && (f <= 360) )
         {
            azimuthAngle = f;
         }
      }
      bumpShade->setAzimuthAngle(azimuthAngle);

      // Set the elevation angle.
      ossim_float64 elevationAngle = 45.0;
      lookup = m_kwl->find( ossimKeywordNames::ELEVATION_ANGLE_KW );
      if ( lookup )
      {
         ossim_float64 f = ossimString::toFloat64(lookup);
         if ( (f >= 0.0) && (f <= 90) )
         {
            elevationAngle = f;
         }
      }
      bumpShade->setElevationAngle(elevationAngle);

      if ( !hasLutFile() )
      {
         // Set the color.
         ossim_uint8 r = 0xff;
         ossim_uint8 g = 0xff;
         ossim_uint8 b = 0xff;
         lookup = m_kwl->find( COLOR_RED_KW );
         if ( lookup )
         {
            r = ossimString(lookup).toUInt8();
         }
         lookup = m_kwl->find( COLOR_GREEN_KW );
         if ( lookup )
         {
            g = ossimString(lookup).toUInt8();
         }
         lookup = m_kwl->find( COLOR_BLUE_KW );
         if ( lookup )
         {
            b = ossimString(lookup).toUInt8();
         }
         bumpShade->setRgbColorSource(r, g, b);
      }

      // Connect the two sources.
      bumpShade->connectMyInputTo(0, normSource.get());
      bumpShade->connectMyInputTo(1, colorSource.get());

      if ( traceDebug() )
      {
         ossim_uint8 r = 0xff;
         ossim_uint8 g = 0xff;
         ossim_uint8 b = 0xff;
         bumpShade->getRgbColorSource(r, g, b);
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "exaggeration:      " << exaggeration
            << "\nazimuthAngle:      " << azimuthAngle
            << "\nelevation angle:   " << elevationAngle
            << "\nsmoothness factor: " << smoothnessFactor
            << "\nr:                 " << int(r)
            << "\ng:                 " << int(g)
            << "\nb:                 " << int(b)
            << "\n";
      }

      // Capture the pointer to give to the writer.
      source = bumpShade.get();
   }
   else if ( m_operation == OSSIM_DEM_OP_COLOR_RELIEF )
   {
      source = combineLayers();
      if ( hasLutFile() )
      {
         source = addIndexToRgbLutFilter( source );
      }
      else
      {
         // No LUT file provided, so doing the default 8-bit linear stretch:
         source = addScalarRemapper( source );
      }
   }
   else if ( m_operation == OSSIM_DEM_OP_ORTHO )
   {
      source = combineLayers();
   }

   if ( source.valid() )
   {
      // This is conditional.  May be set at the ossimSingleImageChain level.
      if ( scaleToEightBit() && ( source->getOutputScalarType() != OSSIM_UINT8 ) )
      {
         source = addScalarRemapper( source );
      }
      
      //---
      // Get the area of interest. This will be the scene bounding rect if not
      // explicitly set by user with one of the --cut options.
      // a thumbnail is desired.
      //---
      ossimIrect aoi;
      getAreaOfInterest(source.get(), aoi);

      if ( hasThumbnailResolution() )
      {
         // Adjust the projection scale and get the new rect.
         initializeThumbnailProjection( aoi, aoi );
      }

      // Set up the writer.
      ossimRefPtr<ossimImageFileWriter> writer = createNewWriter();

      // Connect the writer.
      writer->connectMyInputTo(0, source.get());

      //---
      // Set the area of interest.
      // 
      // NOTE: This must be called after the writer->connectMyInputTo as
      // ossimImageFileWriter::initialize incorrectly resets theAreaOfInterest back to the
      // bounding rect.
      //---
      if ( !aoi.hasNans() )
      {
         writer->setAreaOfInterest(aoi);
      }

      // Add a listener to get percent complete.
      ossimStdOutProgress prog(0, true);
      writer->addListener(&prog);

      if ( traceLog() )
      {
         ossimKeywordlist logKwl;
         writer->saveStateOfAllInputs(logKwl);
         
         ossimFilename logFile;
         getOutputFilename(logFile);
         logFile.setExtension("log");
         ossimKeywordlist kwl;
         writer->saveStateOfAllInputs(kwl);
         kwl.write( logFile.c_str() );
      }
      
      if (writer->getErrorStatus() == ossimErrorCodes::OSSIM_OK)
      {
         // Write the file:
         writer->execute();
      }
   }
   
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " exited...\n";
   }   
}

void ossimDemUtil::addDemSources()
{
   static const char MODULE[] = "ossimDemUtil::addDemSources";
   
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }

   // Add the images from the options keyword list.
   ossim_uint32 demCount = m_kwl->numberOf(DEM_KW);
   for (ossim_uint32 i = 0; i < demCount; ++i)
   {
      ossimString key = DEM_KW;
      key += ossimString::toString(i);
      key += ".";
      key += FILE_KW;
      const char* lookup = m_kwl->find( key.c_str() );
      if ( lookup )
      {
         ossimFilename f = lookup;
         addDemSource(f);
      }
   }

   if ( m_srcKwl.valid() )
   {
      // Add stuff from src keyword list.
      demCount = m_srcKwl->numberOf(DEM_KW);
      for (ossim_uint32 i = 0; i < demCount; ++i)
      {
         ossimString prefix = DEM_KW;
         prefix += ossimString::toString(i);
         prefix += ".";
         ossimSrcRecord src;
         if ( src.loadState( *(m_srcKwl.get()), prefix ) )
         {
            addDemSource(src);
         }
      }
   }
   
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " exited...\n";
   } 
}

void ossimDemUtil::addDemSource(const ossimFilename& file)
{
   static const char MODULE[] = "ossimDemUtil::addDemSource(const ossimFilename&)";

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }

   ossimRefPtr<ossimSingleImageChain> ic = createChain(file);
   if ( ic.valid() )
   {
      m_demLayer.push_back(ic);
   }

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " exiting...\n";
   }
}

void ossimDemUtil::addDemSource(const ossimSrcRecord& rec)
{
   static const char MODULE[] = "ossimDemUtil::addDemSource(const ossimSrcRecord&)";

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }

   ossimRefPtr<ossimSingleImageChain> ic = createChain(rec);
   if ( ic.valid() )
   {
      m_demLayer.push_back(ic);
   }

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " exiting...\n";
   }
}

void ossimDemUtil::addImgSources()
{
   static const char MODULE[] = "ossimDemUtil::addImgSources";
   
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }
   
   ossim_uint32 imgCount = m_kwl->numberOf(IMG_KW);
   for (ossim_uint32 i = 0; i < imgCount; ++i)
   {
      ossimString key = IMG_KW;
      key += ossimString::toString(i);
      key += ".";
      key += FILE_KW;
      const char* lookup = m_kwl->find( key.c_str() );
      if ( lookup )
      {
         ossimFilename f = lookup;
         addImgSource(f);
      }               
   }

   if ( m_srcKwl.valid() )
   {
      // Add stuff from src keyword list.
      imgCount = m_srcKwl->numberOf(IMG_KW);
      for (ossim_uint32 i = 0; i < imgCount; ++i)
      {
         ossimString prefix = IMG_KW;
         prefix += ossimString::toString(i);
         prefix += ".";
         ossimSrcRecord src;
         if ( src.loadState( *(m_srcKwl.get()), prefix ) )
         {
            addImgSource(src);
         }
      }
   }
   
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " exited...\n";
   }
}

void ossimDemUtil::addImgSource(const ossimFilename& file)
{
   static const char MODULE[] = "ossimDemUtil::addImgSource";
   
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }

   ossimRefPtr<ossimSingleImageChain> ic = createChain(file);
   if ( ic.valid() )
   {
      m_imgLayer.push_back(ic);
   }

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " exiting...\n";
   }
}

void ossimDemUtil::addImgSource(const ossimSrcRecord& rec)
{
   static const char MODULE[] = "ossimDemUtil::addImgSource(const ossimSrcRecord&)";

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }

   ossimRefPtr<ossimSingleImageChain> ic = createChain(rec);
   if ( ic.valid() )
   {
      m_imgLayer.push_back(ic);
   }

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " exiting...\n";
   }
}

ossimRefPtr<ossimSingleImageChain> ossimDemUtil::createChain(const ossimFilename& file) const
{
   static const char MODULE[] = "ossimDemUtil::createChain(const ossimFilename&";
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " entered..."
         << "\nfile: " << file << "\n";
   }   
   
   ossimRefPtr<ossimSingleImageChain> ic = 0;
   
   if ( file.size() )
   {
      if ( file.exists() )
      {
         ic = new ossimSingleImageChain;
         if ( ic->open(file) )
         {
            //---
            // If multiple inputs and scaleToEightBit do it at the end of the processing
            // chain to alleviate un-even stretches between inputs.
            //---
            bool scaleFlag = ( scaleToEightBit() && ( getNumberOfInputs() == 1) );
            ic->setRemapToEightBitFlag( scaleFlag );
            
            // Always have resampler cache.
            ic->setAddResamplerCacheFlag(true);

            //---
            // Don't need a chain cache as we're doing a sequential write.  So the same tile
            // should never be visited more than once.
            //---
            ic->setAddChainCacheFlag(false);

            // Histogram.
            ic->setAddHistogramFlag( hasHistogramOperation() );

            // Create the chain.
            ic->createRenderedChain();

            // Set the filter type if needed.
            const char* lookup = m_kwl->find( RESAMPLER_FILTER_KW );
            if ( lookup )
            {
               ic->getImageRenderer()->getResampler()->setFilterType(ossimString(lookup));
            }

            // Histogram setup.
            if ( hasHistogramOperation() )
            {
               setupChainHistogram( ic );
            }
         }
      }
   }

   if ( !ic.valid() )
   {
      std::string errMsg = "Could not open: ";
      errMsg += file;
      throw ossimException(errMsg); 
   }

   if ( traceDebug() )
   {
      ossimKeywordlist kwl;
      ic->saveState(kwl, 0);

      ossimNotify(ossimNotifyLevel_DEBUG)
         << "chain:\n" << kwl << "\n"
         << MODULE << " exiting...\n";
   }   

   return ic;
}

ossimRefPtr<ossimSingleImageChain> ossimDemUtil::createChain(const ossimSrcRecord& rec) const
{
   static const char MODULE[] = "ossimDemUtil::createChain(const ossimSrcRecord&)";
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }   
   
   ossimRefPtr<ossimSingleImageChain> ic = new ossimSingleImageChain;
   if ( ic->open(rec) )
   {
      //---
      // If multiple inputs and scaleToEightBit do it at the end of the processing
      // chain to alleviate un-even strectes between inputs.
      //---
      bool scaleFlag = ( scaleToEightBit() && ( getNumberOfInputs() == 1) );
      ic->setRemapToEightBitFlag( scaleFlag );
      
      // Always have resampler cache.
      ic->setAddResamplerCacheFlag(true);
      
      //---
      // Don't need a chain cache as we're doing a sequential write.  So the same tile
      // should never be visited more than once.
      //---
      ic->setAddChainCacheFlag(false);

      // Histogram.
      ic->setAddHistogramFlag( hasHistogramOperation() );

      //---
      // NOTE: Histogram and band selector can be set in ic->createRenderedChain(rec)
      // if the right keywords are there.
      //---
      ic->createRenderedChain(rec);

      // Set the filter type if needed.
      const char* lookup = m_kwl->find( RESAMPLER_FILTER_KW );
      if ( lookup )
      {
         ic->getImageRenderer()->getResampler()->setFilterType(ossimString(lookup));
      }
   }
   else // Open failed.
   {
      std::string errMsg = "Could not open from src record!";
      throw ossimException(errMsg); 
   }

   if ( traceDebug() )
   {
      ossimKeywordlist kwl;
      ic->saveState(kwl, 0);

      ossimNotify(ossimNotifyLevel_DEBUG)
         << "chain:\n" << kwl << "\n"
         << MODULE << " exiting...\n";
   }   

   return ic;
}
   
void ossimDemUtil::createOutputProjection()
{
   static const char MODULE[] = "ossimDemUtil::createOutputProjection";
   
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }

   // Start with null projection.
   m_outputProjection  = 0;

   const char* op  = m_kwl->find(ossimKeywordNames::PROJECTION_KW);
   const char* srs = m_kwl->find(SRS_KW);
   
   if ( op && srs )
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " WARNING:"
         << "\nBoth " << SRS_KW << " and " << ossimKeywordNames::PROJECTION_KW
         << " keywords are set!"
         << "\nsrs:               " << srs
         << "\noutput_projection: " << op
         << "\nTaking " << srs << " over " << op << "\n";
   }
   
   bool usingInput = false;
   
   // If an srs code use that first.
   if (srs)
   {
      m_outputProjection = getNewProjectionFromSrsCode( ossimString(srs) );
   }
   else if (op)
   {
      ossimString projString = op;
      ossimString os = op;
      os.downcase();
      
      if ( (os == "utm") || (projString == "ossimUtmProjection") )
      {
         m_outputProjection = getNewUtmProjection();
      }
      else if ( os == "input" )
      {
         m_outputProjection = getFirstInputProjection();
         usingInput = true;
      }
      else if (os == "geo")
      {
         m_outputProjection = getNewGeoProjection();
      }
      else if (os == "geo-scaled")
      {
         m_outputProjection = getNewGeoScaledProjection();
      }
   }
   
   if ( !m_outputProjection.valid() )
   {
      // Try first input. If map projected use that.
      m_outputProjection = getFirstInputProjection();
      if ( m_outputProjection.valid() )
      {
         usingInput = true;
         if ( traceDebug() )
         {
            
            ossimNotify(ossimNotifyLevel_WARN)
               << "WARNING: No projection set!"
               << "\nDefaulting to first input's projection.\n";
         }
      }
      else
      {
         m_outputProjection = getNewGeoScaledProjection();
         if ( traceDebug() )
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "WARNING: No projection set!"
               << "\nDefaulting to scaled geographic at scene center.\n";
         }
      }
   }

   //---
   // If the input is the same as output projection do not modify; else, set
   // the gsd to user selected "METERS_KW" or the best resolution of the inputs,
   // set the tie and then snap it to the projection origin.
   //---
   if ( !usingInput || m_kwl->find(METERS_KW) )
   {
      // Set the scale.
      initializeProjectionGsd();
      
      // Set the tie.
      intiailizeProjectionTiePoint();

      // Adjust the projection tie and origin.
      m_outputProjection->snapTiePointToOrigin();
   }
   
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "output projection:\n";

      m_outputProjection->print(ossimNotify(ossimNotifyLevel_DEBUG));

      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " exited...\n";
   }
}

void ossimDemUtil::intiailizeProjectionTiePoint()
{
   static const char MODULE[] = "ossimDemUtil::initializeProjectionTiePoint()";
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }
   
   ossimGpt tiePoint;
   tiePoint.makeNan();

   if ( m_outputProjection.valid() )
   {
      getTiePoint(tiePoint);
      
      if ( !tiePoint.hasNans() )
      {
         m_outputProjection->setUlTiePoints(tiePoint);
      }
      else
      {
         std::string errMsg = MODULE;
         errMsg += " tie point has nans!";
         throw( ossimException(errMsg) );
      }
   }
   else
   {
      std::string errMsg = MODULE;
      errMsg += "m_projection is null!";
      throw( ossimException(errMsg) ); 
   }
   
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "projection tie point: " << tiePoint << "\n" << MODULE << " exited...\n";
   }
}

void ossimDemUtil::initializeProjectionGsd()
{
   static const char MODULE[] = "ossimDemUtil::initializeProjectionGsd()";
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }

   ossimDpt gsd;
   gsd.makeNan();

   if ( m_outputProjection.valid() )
   {
      const char* lookup = m_kwl->find(METERS_KW);
      if ( lookup )
      {
         gsd.x = ossimString::toFloat64(lookup);
         gsd.y = gsd.x;
      }

      if ( gsd.hasNans() )
      {
         getMetersPerPixel(gsd);
      }
      
      if ( !gsd.hasNans() )
      {
         m_outputProjection->setMetersPerPixel(gsd);
      }
      else
      {
         std::string errMsg = MODULE;
         errMsg += "gsd has nans!";
         throw( ossimException(errMsg) );
      }
   }
   else
   {
      std::string errMsg = MODULE;
      errMsg += "m_projection is null!";
      throw( ossimException(errMsg) ); 
   }
   
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "projection gsd: " << gsd << "\n" << MODULE << " exited...\n";
   }
}

void ossimDemUtil::getTiePoint(ossimGpt& tie)
{
   static const char MODULE[] = "ossimDemUtil::getTiePoint(ossimGpt&)";
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }

   std::vector< ossimRefPtr<ossimSingleImageChain> >::iterator chainIdx;

   tie.lat = ossim::nan();
   tie.lon = ossim::nan();
   tie.hgt = 0.0;
   
   // Loop through dem layers.
   ossimGpt chainTiePoint;
   chainIdx = m_demLayer.begin();
   while ( chainIdx != m_demLayer.end() )
   {
      getTiePoint( (*chainIdx).get(), chainTiePoint );
      if ( tie.hasNans() )
      {
         tie = chainTiePoint;
      }
      else
      {
         if ( chainTiePoint.lat > tie.lat )
         {
            tie.lat = chainTiePoint.lat;
         }
         if ( chainTiePoint.lon < tie.lon )
         {
            tie.lon = chainTiePoint.lon;
         }
      }
      ++chainIdx;
   }

   // Loop through image layers.
   chainIdx = m_imgLayer.begin();
   while ( chainIdx != m_imgLayer.end() )
   {
      getTiePoint( (*chainIdx).get(), chainTiePoint );
      if ( tie.hasNans() )
      {
         tie = chainTiePoint;
      }
      else
      {
         if ( chainTiePoint.lat > tie.lat )
         {
            tie.lat = chainTiePoint.lat;
         }
         if ( chainTiePoint.lon < tie.lon )
         {
            tie.lon = chainTiePoint.lon;
         }
      }
      ++chainIdx;
   }

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "tie point: " << tie << "\n" << MODULE << " exited...\n";
   }
}

void ossimDemUtil::getTiePoint(ossimSingleImageChain* chain, ossimGpt& tie)
{
   static const char MODULE[] = "ossimDemUtil::getTiePoint(ossimSingleImageChain*,ossimGpt&)";
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }   
   
   if (chain)
   {
      ossimRefPtr<ossimImageGeometry> geom = chain->getImageGeometry();
      if ( geom.valid() )
      {
         ossimIrect boundingRect = chain->getBoundingRect();
         ossimDpt ulPt = boundingRect.ul();
         geom->localToWorld(ulPt, tie);

         tie.hgt = 0.0;
         
         if ( tie.hasNans() )
         {
            std::string errMsg = MODULE;
            errMsg += "\ngeom->localToWorld returned nan for chain.";
            errMsg += "\nChain: ";
            errMsg += chain->getFilename();
            throw ossimException(errMsg);
         }
      }
      else
      {
         std::string errMsg = MODULE;
         errMsg += "\nNo geometry for chain: ";
         errMsg += chain->getFilename();
         throw ossimException(errMsg);
      }
   }
   else
   {
      std::string errMsg = MODULE;
      errMsg += " ERROR: Null chain passed to method!";
      throw ossimException(errMsg);
   }

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "chain name: " << chain->getFilename()
         << "\ntie point:  " << tie << "\n"
         << MODULE << " exited...\n";
   }
}

void ossimDemUtil::getMetersPerPixel(ossimDpt& gsd)
{
   static const char MODULE[] = "ossimDemUtil::getMetersPerPixel(ossimDpt&)";
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }

   gsd.makeNan();
   
   ossimDpt chainGsd;
   std::vector< ossimRefPtr<ossimSingleImageChain> >::iterator chainIdx;

   // Loop through dem layers.
   chainIdx = m_demLayer.begin();
   while ( chainIdx != m_demLayer.end() )
   {
      getMetersPerPixel( (*chainIdx).get(), chainGsd);
      if ( gsd.hasNans() || ( chainGsd.x < gsd.x ) ) 
      {
         gsd = chainGsd;
      }
      ++chainIdx;
   }

   // Loop through image layers.
   chainIdx = m_imgLayer.begin();
   while ( chainIdx != m_imgLayer.end() )
   {
      getMetersPerPixel( (*chainIdx).get(), chainGsd);
      if ( gsd.hasNans() || ( chainGsd.x < gsd.x ) )
      {
         gsd = chainGsd;
      }
      ++chainIdx;
   }

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "gsd: " << gsd << "\n" << MODULE << " exited...\n";
   }
}

void ossimDemUtil::getMetersPerPixel(ossimSingleImageChain* chain, ossimDpt& gsd)
{
   static const char MODULE[] = "ossimDemUtil::getMetersPerPixel(ossimSingleImageChain*,ossimDpt&)";
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }

   if (chain)
   {
      ossimRefPtr<ossimImageGeometry> geom = chain->getImageGeometry();
      if ( geom.valid() )
      {
         gsd = geom->getMetersPerPixel();
         
         if ( gsd.hasNans() )
         {
            std::string errMsg = MODULE;
            errMsg += "\ngeom->getMetersPerPixel returned nan for chain.";
            errMsg += "\nChain: ";
            errMsg += chain->getFilename();
            throw ossimException(errMsg);
         }
      }
      else
      {
         std::string errMsg = MODULE;
         errMsg += "\nNo geometry for chain: ";
         errMsg += chain->getFilename();
         throw ossimException(errMsg);
      }
   }
   else
   {
      std::string errMsg = MODULE;
      errMsg += " ERROR: Null chain passed to method!";
      throw ossimException(errMsg);
   }
   
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "chain name: " << chain->getFilename()
         << "\nmeters per pixel: " << gsd << "\n" << MODULE << " exited...\n";
   }
}

void ossimDemUtil::getOrigin(ossimGpt& gpt)
{
   static const char MODULE[] = "ossimDemUtil::getOrigin(ossimGpt&)";
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }     

   gpt.lat = ossim::nan();
   gpt.lon = ossim::nan();
   gpt.hgt = 0.0;

   // Latitude:
   const char* lookup = m_kwl->find(ossimKeywordNames::ORIGIN_LATITUDE_KW);
   if ( lookup )
   {
      ossim_float64 latitude = ossimString::toFloat64(lookup);
      if ( (latitude >= -90) && (latitude <= 90.0) )
      {
         gpt.lat = latitude;
      }
      else
      {
         std::string errMsg = "origin latitude range error!";
         errMsg += " Valid range: -90 to 90";
         throw ossimException(errMsg);
      }
   }

   // Longitude:
   lookup = m_kwl->find(ossimKeywordNames::CENTRAL_MERIDIAN_KW);
   if ( lookup )
   {
      ossim_float64 longitude = ossimString::toFloat64(lookup);
      if ( (longitude >= -180.0) && (longitude <= -180.0) )
      {
         gpt.lon = longitude;
      }
      else
      {
         std::string errMsg = "central meridian range error!";
         errMsg += " Valid range: -180 to 180";
         throw ossimException(errMsg);
      }
   }

   if ( gpt.hasNans() )
   {
      ossimGpt sceneCenter;
      getSceneCenter(sceneCenter);
      if ( ossim::isnan(gpt.lat) )
      {
         gpt.lat = sceneCenter.lat;
      }
      if ( ossim::isnan(gpt.lon) )
      {
         gpt.lon = sceneCenter.lon;
      }    
   }
      
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "origin: " << gpt << "\n" << MODULE << " exited...\n";
   }
}

void ossimDemUtil::getSceneCenter(ossimGpt& gpt)
{
   static const char MODULE[] = "ossimDemUtil::getSceneCenter(ossimGpt&)";
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }

   std::vector<ossimGpt> centerGptArray;
   ossimGpt centerGpt;
   
   std::vector< ossimRefPtr<ossimSingleImageChain> >::iterator chainIdx;

   // Loop through dem layers.
   chainIdx = m_demLayer.begin();
   while ( chainIdx != m_demLayer.end() )
   {
      getSceneCenter( (*chainIdx).get(), centerGpt);
      if ( !centerGpt.hasNans() )
      {
         centerGptArray.push_back( centerGpt );
      }
      ++chainIdx;
   }

   // Loop through image layers.
   chainIdx = m_imgLayer.begin();
   while ( chainIdx != m_imgLayer.end() )
   {
      getSceneCenter( (*chainIdx).get(), centerGpt);
      if ( !centerGpt.hasNans() )
      {
         centerGptArray.push_back( centerGpt );
      }
      ++chainIdx;
   }

   ossim_float64 lat = 0.0;
   ossim_float64 lon = 0.0;
   
   std::vector<ossimGpt>::const_iterator pointIdx = centerGptArray.begin();
   while ( pointIdx != centerGptArray.end() )
   {
      lat += (*pointIdx).lat;
      lon += (*pointIdx).lon;
      ++pointIdx;
   }

   lat /= centerGptArray.size();
   lon /= centerGptArray.size();

   if ( (lat >= -90.0) && (lat <= 90.0) && (lon >= -180.0) && (lon <= 180.0) )
   {
      gpt.lat = lat;
      gpt.lon = lon;
   }
   else
   {
      std::string errMsg = MODULE;
      errMsg += " range error!";
      errMsg += "\nlatitude = ";
      errMsg += ossimString::toString(lat);
      errMsg += "\nlongitude = ";
      errMsg += ossimString::toString(lon);
      throw ossimException(errMsg);
   }

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "scene center: " << gpt << "\n" << MODULE << " exited...\n";
   }
}

void ossimDemUtil::getSceneCenter(ossimSingleImageChain* chain, ossimGpt& gpt)
{
   static const char MODULE[] =
      "ossimDemUtil::getSceneCenter(const ossimSingleImageChain*,ossimGpt&)";

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }   
   
   if (chain)
   {
      ossimRefPtr<ossimImageGeometry> geom = chain->getImageGeometry();
      if ( geom.valid() )
      {
         ossimIrect boundingRect = chain->getBoundingRect();
         ossimDpt midPoint = boundingRect.midPoint();
         geom->localToWorld(midPoint, gpt);
         gpt.hgt = 0.0;
         
         if ( gpt.hasNans() )
         {
            std::string errMsg = MODULE;
            errMsg += "\ngeom->localToWorld returned nan for chain.";
            errMsg += "\nChain: ";
            errMsg += chain->getFilename();
            throw ossimException(errMsg);
         }
      }
      else
      {
         std::string errMsg = MODULE;
         errMsg += "\nNo geometry for chain: ";
         errMsg += chain->getFilename();
         throw ossimException(errMsg);
      }
   }
   else
   {
      std::string errMsg = MODULE;
      errMsg += " ERROR: Null chain passed to method!";
      throw ossimException(errMsg);
   }

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "chain name: " << chain->getFilename()
         << "\nscene center: " << gpt << "\n"
         << MODULE << " exited...\n";
   }
}

ossimRefPtr<ossimMapProjection> ossimDemUtil::getFirstInputProjection()
{
   static const char MODULE[] = "ossimDemUtil::getFirstInputProjection";
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }

   ossimRefPtr<ossimImageHandler>  ih     = 0;
   ossimRefPtr<ossimMapProjection> result = 0;

   // Get the first image handler.
   if ( m_demLayer.size() )
   {
      ih = m_demLayer[0]->getImageHandler();
   }
   else if ( m_imgLayer.size() )
   {
      ih = m_imgLayer[0]->getImageHandler();
   }
   
   if ( ih.valid() )
   {
      // Get the geometry from the first image handler.      
      ossimRefPtr<ossimImageGeometry> geom = ih->getImageGeometry();
      if ( geom.valid() )
      {
         // Get the image projection.
         ossimRefPtr<ossimProjection> proj = geom->getProjection();
         if ( proj.valid() )
         {
            // Cast and assign to result.
            ossimMapProjection* mapProj = PTR_CAST( ossimMapProjection, proj.get() );
            if (mapProj)
            {
               // Must duplicate in case the output projection gets modified.
               result = (ossimMapProjection*) mapProj->dup();
            }
            if ( !result.valid() && traceDebug() )
            {
               ossimNotify(ossimNotifyLevel_WARN) << "Could not cast to map projection.\n";
            }
         }
         else if ( traceDebug() )
         {
            ossimNotify(ossimNotifyLevel_WARN) << "No projection in first chain...\n";
         }
      }
   }
   else if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_WARN) << "No image handler in first chain...\n";
   }
   
   if ( traceDebug() )
   {
      if ( result.valid() )
      {
         result->print(ossimNotify(ossimNotifyLevel_DEBUG));
      }
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " exited...\n";
   }
   
   return result;
}

ossimRefPtr<ossimMapProjection> ossimDemUtil::getNewGeoProjection()
{
   return ossimRefPtr<ossimMapProjection>(new ossimEquDistCylProjection());
}

ossimRefPtr<ossimMapProjection> ossimDemUtil::getNewGeoScaledProjection()
{
   ossimRefPtr<ossimMapProjection> result = getNewGeoProjection();
   ossimGpt origin;
   getOrigin(origin);
   result->setOrigin(origin);
   return result;
}

ossimRefPtr<ossimMapProjection> ossimDemUtil::getNewProjectionFromSrsCode(const ossimString& code)
{
   ossimRefPtr<ossimMapProjection> result = 0;
   ossimRefPtr<ossimProjection> proj = ossimProjectionFactoryRegistry::instance()->
      createProjection(code);
   if ( proj.valid() )
   {
      result = PTR_CAST( ossimMapProjection, proj.get() );
   }
   return result;
}

ossimRefPtr<ossimMapProjection> ossimDemUtil::getNewUtmProjection()
{
   ossimRefPtr<ossimUtmProjection> utm = new ossimUtmProjection;
   ossimGpt origin;
   getOrigin(origin);
   utm->setZone(origin);
   utm->setHemisphere(origin);
   return ossimRefPtr<ossimMapProjection>(utm.get());
}

ossimRefPtr<ossimImageFileWriter> ossimDemUtil::createNewWriter() const
{
   static const char MODULE[] = "ossimDemUtil::createNewWriter()";
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }

   ossimFilename outputFile;
   getOutputFilename(outputFile);

   if ( outputFile == ossimFilename::NIL)
   {
      std::string errMsg = MODULE;
      errMsg += " ERROR no output file name!";
      throw ossimException(errMsg);
   }

   ossimRefPtr<ossimImageFileWriter> writer = 0;
   
   const char* lookup = m_kwl->find( WRITER_KW );
   if ( lookup ) // User defined.
   {
      writer = ossimImageWriterFactoryRegistry::instance()->createWriter(ossimString(lookup));

      if ( !writer.valid() )
      {
         std::string errMsg = MODULE;
         errMsg += " ERROR creating writer: ";
         errMsg += lookup;
         throw ossimException(errMsg);
      }
   }
   else // Create from output file extension.
   {
      writer = ossimImageWriterFactoryRegistry::instance()->
         createWriterFromExtension( outputFile.ext() );

      if ( !writer.valid() )
      {
         std::string errMsg = MODULE;
         errMsg += " ERROR creating writer from extension: ";
         errMsg += outputFile.ext();
         throw ossimException(errMsg);
      }
   }

   // Set the output name.
   writer->setFilename( outputFile );

   // Add any writer props.
   ossim_uint32 count = m_kwl->numberOf(WRITER_PROPERTY_KW);
   for (ossim_uint32 i = 0; i < count; ++i)
   {
      ossimString key = WRITER_PROPERTY_KW;
      key += ossimString::toString(i);
      lookup = m_kwl->find( key.c_str() );
      if ( lookup )
      {
         ossimString s = lookup;
         std::vector<ossimString> splitArray;
         s.split(splitArray, "=");
         if(splitArray.size() == 2)
         {
            ossimRefPtr<ossimProperty> prop =
               new ossimStringProperty(splitArray[0], splitArray[1]);

            if ( traceDebug() )
            {
               ossimNotify(ossimNotifyLevel_DEBUG)
                  << "Setting writer prop: " << splitArray[0] << "=" << splitArray[1] << "\n";
            }
            
            writer->setProperty( prop );
         }
      }
   }
   
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "writer name: " << writer->getClassName() << "\n"
         << MODULE << " exited...\n";
   }

   return writer;
}

void ossimDemUtil::propagateOutputProjectionToChains()
{
   static const char MODULE[] = "ossimDemUtil::propagateOutputProjectionToChains()";
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }

   std::vector< ossimRefPtr<ossimSingleImageChain> >::iterator chainIdx;

   // Loop through dem layers.
   chainIdx = m_demLayer.begin();
   while ( chainIdx != m_demLayer.end() )
   {
      ossimRefPtr<ossimImageRenderer> resampler = (*chainIdx)->getImageRenderer();
      if ( resampler.valid() )
      {
         resampler->setView( m_outputProjection.get() );
      }
      else
      {
         std::string errMsg = MODULE;
         errMsg += " chain has no resampler!";
         throw( ossimException(errMsg) );
      }
      ++chainIdx;
   }

   // Loop through image layers.
   chainIdx = m_imgLayer.begin();
   while ( chainIdx != m_imgLayer.end() )
   {
      ossimRefPtr<ossimImageRenderer> resampler = (*chainIdx)->getImageRenderer();
      if ( resampler.valid() )
      {
         resampler->setView( m_outputProjection.get() );
      }
      else
      {
         std::string errMsg = MODULE;
         errMsg += " chain has no resampler!";
         throw( ossimException(errMsg) );
      }
      ++chainIdx;
   }
   
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " exited...\n";
   }
}

ossimRefPtr<ossimImageSource> ossimDemUtil::combineLayers(
   std::vector< ossimRefPtr<ossimSingleImageChain> >& layers) const
{
   static const char MODULE[] = "ossimDemUtil::combineLayers(layers)";
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }

   ossimRefPtr<ossimImageSource> result = 0;

   if ( layers.size() == 1 )
   {
      result = layers[0].get();
   }
   else if ( layers.size() > 1 )
   {
      result = new ossimImageMosaic;

      std::vector< ossimRefPtr<ossimSingleImageChain> >::iterator chainIdx = layers.begin();
      while ( chainIdx != layers.end() )
      {
         result->connectMyInputTo( (*chainIdx).get() );
         ++chainIdx;
      }
   }

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " exited...\n";
   }

   return result;
}

ossimRefPtr<ossimImageSource> ossimDemUtil::combineLayers()
{
   static const char MODULE[] = "ossimDemUtil::combineLayers()";
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }

   ossimRefPtr<ossimImageSource> result = 0;

   ossim_uint32 layerCount = (ossim_uint32) m_demLayer.size() + m_imgLayer.size();

   if ( layerCount )
   {
      if ( layerCount == 1 )
      {
         if ( m_imgLayer.size() )
         {
            result = m_imgLayer[0].get();
         }
         else
         {
            result = m_demLayer[0].get();
         }
      }
      else
      {
         result = new ossimImageMosaic;
         
         // Combine the images.  Note we'll put the images on top of the dems.
         if ( m_imgLayer.size() )
         {
            std::vector< ossimRefPtr<ossimSingleImageChain> >::iterator chainIdx =
               m_imgLayer.begin();
            while ( chainIdx !=  m_imgLayer.end() )
            {
               result->connectMyInputTo( (*chainIdx).get() );
               ++chainIdx;
            }
         }
         if ( m_demLayer.size() ) // Combine any dem layers.
         {
            std::vector< ossimRefPtr<ossimSingleImageChain> >::iterator chainIdx =
               m_demLayer.begin();
            while ( chainIdx != m_demLayer.end() )
            {
               result->connectMyInputTo( (*chainIdx).get() );
               ++chainIdx;
            }
         }
      }
   }
   
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " exited...\n";
   }
   
   return result;
}

ossimRefPtr<ossimImageSource> ossimDemUtil::addIndexToRgbLutFilter(
   ossimRefPtr<ossimImageSource> &source) const
{
   static const char MODULE[] = "ossimDemUtil::addIndexToRgbLutFilter(source)";
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }

   ossimRefPtr<ossimImageSource> result = 0;

   if ( source.valid() )
   {
      ossimRefPtr<ossimIndexToRgbLutFilter> lut = new ossimIndexToRgbLutFilter();
      ossimFilename lutFile = m_kwl->find( LUT_FILE_KW );
      if ( lutFile.exists() )
      {
         lut->setLut(lutFile);
         
         // Connect to dems:
         lut->connectMyInputTo( source.get() );

         // Note sure about this.  Make option maybe? (drb)
         lut->setInterpolationType(ossimIndexToRgbLutFilter::ossimIndexToRgbLutFilter_LINEAR);
         
         // Set as color source for bump shade.
         result = lut.get();
      }
      else
      {
         std::string errMsg = MODULE;
         errMsg += " color table does not exists: ";
         errMsg += lutFile;
         throw ossimException(errMsg);
      }
   }
   else
   {
      std::string errMsg = MODULE;
      errMsg += " ERROR: Null source passed to method!";
      throw ossimException(errMsg);
   }

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " exited...\n";
   }

   return result;
}

ossimRefPtr<ossimImageSource> ossimDemUtil::addScalarRemapper(
   ossimRefPtr<ossimImageSource> &source) const
{
   static const char MODULE[] = "ossimDemUtil::addScalarRemapper(source)";
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }
   
   ossimRefPtr<ossimImageSource> result = 0;
   
   if ( source.valid() )
   {
      if ( source->getOutputScalarType() != OSSIM_UINT8 )
      {
         ossimRefPtr<ossimScalarRemapper> remapper = new ossimScalarRemapper();
         remapper->setOutputScalarType(OSSIM_UINT8);
         remapper->connectMyInputTo( source.get() );
         result = remapper.get();
      }
      else
      {
         result = source;
      }
   }
   else
   {
      std::string errMsg = MODULE;
      errMsg += " ERROR: Null source passed to method!";
      throw ossimException(errMsg);
   }

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " exited...\n";
   }

   return result;
}

bool ossimDemUtil::setupChainHistogram( ossimRefPtr<ossimSingleImageChain> &chain) const
{
   static const char MODULE[] = "ossimDemUtil::setupChainHistogram(chain)";
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   } 
   
   bool result = false;

   if ( chain.valid() )
   {
      ossimRefPtr<ossimHistogramRemapper> remapper = chain->getHistogramRemapper();

      if ( remapper.valid() )
      {
         if ( remapper->getHistogramFile() == ossimFilename::NIL )
         {
            ossimRefPtr<ossimImageHandler> ih = chain->getImageHandler();
            if ( ih.valid() )
            {
               ossimFilename f = ih->getFilenameWithThisExtension( ossimString("his") );

               if ( f.empty() || (f.exists() == false) )
               {
                  // For backward compatibility check if single entry and _e0.his
                  f = ih->getFilenameWithThisExtension( ossimString("his"), true );
               }

               if ( remapper->openHistogram( f ) == false )
               {
                  ossimNotify(ossimNotifyLevel_WARN)
                     << MODULE << " WARNING:"
                     << "\nCould not open:  " << f << "\n";
               }
            }
         }

         if ( remapper->getHistogramFile() != ossimFilename::NIL )
         {
            const char* lookup = m_kwl->find(HISTO_OP_KW);
            if (lookup)
            {
               result = true;
               
               // Enable.
               remapper->setEnableFlag(true);
               
               // Set the histo mode:
               ossimString op = lookup;
               op.downcase();
               if ( op == "auto-minmax" )
               {
                  remapper->setStretchMode( ossimHistogramRemapper::LINEAR_AUTO_MIN_MAX );
               }
               else if ( (op == "std-stretch-1") || (op == "std-stretch 1") )
               {
                  remapper->setStretchMode( ossimHistogramRemapper::LINEAR_1STD_FROM_MEAN );
               } 
               else if ( (op == "std-stretch-2") || (op == "std-stretch 2") )
               {
                  remapper->setStretchMode( ossimHistogramRemapper::LINEAR_2STD_FROM_MEAN );
               } 
               else if ( (op == "std-stretch-3") || (op == "std-stretch 3") )
               {
                  remapper->setStretchMode( ossimHistogramRemapper::LINEAR_3STD_FROM_MEAN );
               }
               else
               {
                  result = false;
                  remapper->setEnableFlag(false);
                  ossimNotify(ossimNotifyLevel_WARN)
                     << MODULE << "\nUnhandled operation: " << op << "\n";
               }
            }
         }
      }
   }

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " exited...\n";
   }
   
   return result;
}

void ossimDemUtil::getOutputFilename(ossimFilename& f) const
{
   static const char MODULE[] = "ossimDemUtil::getOutputFilename()";
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   } 

   const char* lookup =  m_kwl->find( ossimKeywordNames::OUTPUT_FILE_KW );
   if ( lookup )
   {
      f = lookup;
   }
   else
   {
      f = ossimFilename::NIL;
   }

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " exited...\n";
   }   
}

void ossimDemUtil::getAreaOfInterest(const ossimImageSource* source, ossimIrect& rect) const
{
   static const char MODULE[] = "ossimDemUtil::getAreaOfInterest()";
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }

   // Nan rect for starters.
   rect.makeNan();
   
   if ( source && m_outputProjection.valid() )
   {
      if ( m_kwl->find( CUT_CENTER_LAT_KW ) ) // --cut-center-llwh
      {
         const char* latStr    = m_kwl->find( CUT_CENTER_LAT_KW );
         const char* lonStr    = m_kwl->find( CUT_CENTER_LON_KW );   
         const char* widthStr  = m_kwl->find( CUT_CENTER_WIDTH_KW );
         const char* heightStr = m_kwl->find( CUT_CENTER_HEIGHT_KW );
         
         if ( latStr && lonStr && widthStr && heightStr )
         {
            ossimGpt centerGpt(0,0,0);
            ossimIpt widthHeight(0,0);
            centerGpt.lat      = ossimString(latStr).toFloat64();
            centerGpt.lon      = ossimString(lonStr).toFloat64();
            ossim_int32 width  = ossimString(widthStr).toInt32();
            ossim_int32 height = ossimString(heightStr).toInt32();
            
            if ( !centerGpt.hasNans() )
            {
               ossimDpt centerDpt;
               m_outputProjection->worldToLineSample(centerGpt, centerDpt);
               if ( !centerDpt.hasNans() )
               {
                  ossimIpt ul( ossim::round<int>(centerDpt.x - (width/2)),
                               ossim::round<int>(centerDpt.y - (height/2)) );
                  ossimIpt lr( (ul.x + width - 1), ul.y + height - 1);
                  rect = ossimIrect(ul, lr);
               }
            }
         }
      }
      else if ( m_kwl->find( CUT_MAX_LAT_KW ) ) // --cut-bbox-ll
      {
         const char* maxLat = m_kwl->find( CUT_MAX_LAT_KW );
         const char* maxLon = m_kwl->find( CUT_MAX_LON_KW );
         const char* minLat = m_kwl->find( CUT_MIN_LAT_KW );
         const char* minLon = m_kwl->find( CUT_MIN_LON_KW );
         
         if ( maxLat && maxLon && minLat && minLon )
         {
            ossimGpt gpt(0.0, 0.0, 0.0);
            ossimDpt dpt;
            
            // Upper left:
            gpt.lat = ossimString(maxLat).toFloat64();
            gpt.lon = ossimString(minLon).toFloat64();
            m_outputProjection->worldToLineSample(gpt, dpt);
            ossimIpt ul(dpt);
            
            // Lower right:
            gpt.lat = ossimString(minLat).toFloat64();
            gpt.lon = ossimString(maxLon).toFloat64();
            m_outputProjection->worldToLineSample(gpt, dpt);
            ossimIpt lr(dpt);
            
            rect = ossimIrect(ul, lr);
         }
      }

      // If no user defined rect set to scene bounding rect.
      if ( rect.hasNans() ) 
      {
         rect = source->getBoundingRect(0);
      }
      
   } // if ( source && m_outputProjection.valid() )
   else
   {
      // Should never happer...
      std::string errMsg = MODULE;
      if ( !source )
      {
         errMsg += " image source null!";
      }
      else
      {
         errMsg += " output projection null!";
      }
      throw( ossimException(errMsg) );
   }

   
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "aoi: " << rect << "\n"
         << MODULE << " exited...\n";
   }
}

void ossimDemUtil::initializeThumbnailProjection(const ossimIrect& originalRect,
                                                ossimIrect& adjustedRect)
{
   static const char MODULE[] = "ossimDemUtil::initializeThumbnailProjection";
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " entered...\n"
         << "origial rect:  " << originalRect << "\n";

      if (m_outputProjection.valid())
      {
         m_outputProjection->print(ossimNotify(ossimNotifyLevel_DEBUG));
      }
   }

   if ( !originalRect.hasNans() && m_outputProjection.valid() )
   {
      //---
      // Thumbnail setup:
      //---
      const char* thumbRes = m_kwl->find(THUMBNAIL_RESOLUTION_KW);
      if ( thumbRes )
      {
         ossim_float64 thumbSize = ossimString(thumbRes).toFloat64();
         ossim_float64 maxRectDimension =
            ossim::max( originalRect.width(), originalRect.height() );
         
         if ( maxRectDimension > thumbSize )
         {
            // Need to adjust scale:
            
            // Get the corners:
            ossimGpt ulGpt;
            ossimGpt lrGpt;
            ossimDpt dpt;
            
            m_outputProjection->lineSampleToWorld(ossimDpt(originalRect.ul()), ulGpt);
            m_outputProjection->lineSampleToWorld(ossimDpt(originalRect.lr()), lrGpt);         
            
            ossim_float64 scale = maxRectDimension / thumbSize;

            //---
            // Adjust the projection scale.  Note the "true" is to recenter
            // the tie point so it falls relative to the projection origin.
            //---
            m_outputProjection->applyScale(ossimDpt(scale, scale), true);
            
            // Get the new upper left in view space.
            m_outputProjection->worldToLineSample(ulGpt, dpt);
            ossimIpt ul(dpt);
            
            // Get the new lower right in view space.
            m_outputProjection->worldToLineSample(lrGpt, dpt);
            ossimIpt lr(dpt);

            // Clamp to thumbnail bounds.
            ossim_int32 ts = thumbSize;

            if ( (lr.x - ul.x + 1) > ts )
            {
               lr.x = ul.x + ts - 1;
            }
            if ( (lr.y - ul.y + 1) > ts )
            {
               lr.y = ul.y + ts - 1;
            }
            
            adjustedRect = ossimIrect(ul, lr);
         }
      }
      
   } // if ( !originalRect.hasNans() && m_outputProjection.valid() )
   else
   {
      // Should never happer...
      std::string errMsg = MODULE;
      if ( originalRect.hasNans() )
      {
         errMsg += " passed in rect has nans!";
      }
      else
      {
         errMsg += " output projection null!";
      }
      throw( ossimException(errMsg) );
   }
   
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "\nadjusted rect: " << adjustedRect << "\n";
      if (m_outputProjection.valid())
      {
         m_outputProjection->print(ossimNotify(ossimNotifyLevel_DEBUG));
      }
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " exited...\n";
   }
}

bool ossimDemUtil::hasLutFile() const
{
   bool result = false;
   if ( m_kwl.valid() )
   {
      result = ( m_kwl->find( LUT_FILE_KW ) != 0 );
   }
   return result;
}

bool ossimDemUtil::hasBumpShadeArg() const
{
   bool result = ( m_operation == OSSIM_DEM_OP_HILL_SHADE );
   if ( !result && m_kwl.valid() )
   {
      result = ( m_kwl->find( ossimKeywordNames::AZIMUTH_ANGLE_KW ) ||
                 m_kwl->find( COLOR_RED_KW ) ||
                 m_kwl->find( COLOR_GREEN_KW ) ||
                 m_kwl->find( COLOR_BLUE_KW ) ||
                 m_kwl->find( ossimKeywordNames::ELEVATION_ANGLE_KW ) ||
                 m_kwl->find( EXAGGERATION_KW ) ||
                 m_kwl->find( SMOOTHNESS_FACTOR_KW ) );
   }
   return result;
}

bool ossimDemUtil::hasThumbnailResolution() const
{
   bool result = false;
   if ( m_kwl.valid() )
   {
      result = ( m_kwl->find(THUMBNAIL_RESOLUTION_KW) != 0 );
   }
   return result;
}

bool ossimDemUtil::hasHistogramOperation() const
{
   bool result = false;
   
   if ( m_kwl.valid() )
   {
      result = ( m_kwl->find(HISTO_OP_KW) != 0 );
   }
   // No option for this right now.  Only through src file.
   return result;
}

bool ossimDemUtil::isDemFile(const ossimFilename& file) const
{
   bool result = false;
   ossimString ext = file.ext();
   if ( ext.size() >= 2 )
   {
      ext.downcase();
      if ( ( ext == "hgt" ) ||
           ( ext == "dem" ) ||
          ( ( (*ext.begin()) == 'd' ) && ( (*(ext.begin()+1)) == 't' ) ) )
      {
         result = true;
      }
   }
   return result;
}

bool ossimDemUtil::isSrcFile(const ossimFilename& file) const
{
   bool result = false;
   ossimString ext = file.ext();
   ext.downcase();
   if ( ext == "src" )
   {
      result = true;
   }
   return result;
}

bool ossimDemUtil::scaleToEightBit() const
{
   bool result = false;
   if ( m_operation == OSSIM_DEM_OP_COLOR_RELIEF ) // Always 8 bit...
   {
      result = true;
   }
   else if ( m_kwl.valid() )
   {
      const char* lookup = m_kwl->find(SCALE_2_8_BIT_KW);
      if ( lookup )
      {
         result = ossimString(lookup).toBool();
      }
   }
   return result;
}

void  ossimDemUtil::initializeSrcKwl()
{
   static const char MODULE[] = "ossimDemUtil::initializeSrcKwl";
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " entered...\n";
   }

   const char* lookup = m_kwl->find(SRC_FILE_KW);
   if ( lookup )
   {
      m_srcKwl = new ossimKeywordlist();
      if ( m_srcKwl->addFile(lookup) == false )
      {
         m_srcKwl = 0;
      }
   }
   else
   {
      m_srcKwl = 0; 
   }

   if ( traceDebug() )
   {
      if ( m_srcKwl.valid() )
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "src keyword list:\n" << *(m_srcKwl.get()) << "\n";
      }
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " exited...\n";
   }
}

ossim_uint32 ossimDemUtil::getNumberOfInputs() const
{
   ossim_uint32 result = 0;
   if ( m_kwl.valid() )
   {
      result += m_kwl->numberOf(DEM_KW);
      result += m_kwl->numberOf(IMG_KW);
      
   }
   if ( m_srcKwl.valid() )
   {
      result += m_srcKwl->numberOf(DEM_KW);
      result += m_srcKwl->numberOf(IMG_KW);
   }
   return result;
}
