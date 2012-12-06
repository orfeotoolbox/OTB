//----------------------------------------------------------------------------
// File: ossimInfo.cpp
// 
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: ossimInfo class definition
//
// Utility class for getting information from the ossim library.
// 
//----------------------------------------------------------------------------
// $Id: ossimInfo.cpp 21773 2012-09-26 00:08:19Z dlucas $

#include <ossim/util/ossimInfo.h>
#include <ossim/base/ossimArgumentParser.h>
#include <ossim/base/ossimApplicationUsage.h>
#include <ossim/base/ossimContainerProperty.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimDatumFactoryRegistry.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimObjectFactoryRegistry.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/imaging/ossimFilterResampler.h>
#include <ossim/base/ossimGeoidManager.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/init/ossimInit.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimPreferences.h>
#include <ossim/base/ossimProperty.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimXmlDocument.h>
#include <ossim/elevation/ossimElevManager.h>
#include <ossim/imaging/ossimFilterResampler.h>
#include <ossim/imaging/ossimImageGeometry.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimImageWriterFactoryRegistry.h>
#include <ossim/imaging/ossimOverviewBuilderFactoryRegistry.h>
#include <ossim/init/ossimInit.h>
#include <ossim/plugin/ossimSharedPluginRegistry.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/support_data/ossimInfoBase.h>
#include <ossim/support_data/ossimInfoFactoryRegistry.h>
#include <ossim/support_data/ossimSupportFilesList.h>

#include <iomanip>
#include <vector>

static const char CENTER_GROUND_KW[]        = "center_ground";
static const char CENTER_IMAGE_KW[]         = "center_image";
static const char CONFIGURATION_KW[]        = "configuration";
static const char DATUMS_KW[]               = "datums";
static const char DEG2RAD_KW[]              = "deg2rad";
static const char DUMP_KW[]                 = "dump";
static const char DUMP_NO_OVERVIEWS_KW[]    = "dump_no_overviews";
static const char FACTORIES_KW[]            = "factories";
static const char FACTORY_KEYWORD_LIST_KW[] = "factory_keyword_list";
static const char FACTORY_OBJECT_KW[]       = "factory_object";
static const char FACTORY_TYPE_KW[]         = "factory_type";
static const char FORMAT_KW[]               = "format"; 
static const char FT2MTRS_KW[]              = "ft2mtrs";
static const char FT2MTRS_US_SURVEY_KW[]    = "ft2mtrs_us_survey";
static const char GEOM_INFO_KW[]            = "geometry_info";
static const char HEIGHT_KW[]               = "height";
static const char IMAGE_CENTER_KW[]         = "image_center";
static const char IMAGE_FILE_KW[]           = "image_file";
static const char IMAGE_INFO_KW[]           = "image_info";
static const char IMAGE_RECT_KW[]           = "image_rect";
static const char METADATA_KW[]             = "metadata";
static const char MTRS2FT_KW[]              = "mtrs2ft";
static const char MTRS2FT_US_SURVEY_KW[]    = "mtrs2ft_us_survey";
static const char MTRSPERDEG_KW[]           = "mtrs_per_deg";
static const char OUTPUT_FILE_KW[]          = "output_file";
static const char OSSIM_LOGFILE_KW[]        = "ossim_logfile";
static const char OVERVIEW_TYPES_KW[]       = "overview_types";
static const char OVERWRITE_KW[]            = "overwrite";
static const char PALETTE_KW[]              = "palette";
static const char PLUGINS_KW[]              = "plugins";
static const char PLUGIN_TEST_KW[]          = "plugin_test";
static const char PROJECTIONS_KW[]          = "projections";
static const char RAD2DEG_KW[]              = "rad2deg";
static const char READER_PROPS_KW[]         = "reader_props";
static const char RESAMPLER_FILTERS_KW[]    = "resampler_filters";
static const char WRITERS_KW[]              = "writers_kw";
static const char WRITER_PROPS_KW[]         = "writer_props";

// Static trace for debugging.  Use -T ossimInfo to turn on.
static ossimTrace traceDebug = ossimTrace("ossimInfo:debug");

ossimInfo::ossimInfo() :
   m_kwl(new ossimKeywordlist()),
   m_img(0)
{
}

ossimInfo::~ossimInfo()
{
}

void ossimInfo::addArguments(ossimArgumentParser& ap)
{
   // Set the general usage:
   ossimApplicationUsage* au = ap.getApplicationUsage();
   ossimString usageString = ap.getApplicationName();
   usageString += " [options] <optional-image>";
   au->setCommandLineUsage(usageString);

   // Set the command line options:
   au->addCommandLineOption("-c", "Will print ground and image center.");

   au->addCommandLineOption("--cg", "Will print out ground center.");

   au->addCommandLineOption("--ci", "Will print out image center.");

   au->addCommandLineOption("--config", "Displays configuration info.");

   au->addCommandLineOption("-d", "A generic dump if one is available.");

   au->addCommandLineOption("--datums", "Prints datum list.");   

   au->addCommandLineOption("--deg2rad", "<degrees> Gives radians from degrees.");
   
   au->addCommandLineOption("--dno", "A generic dump if one is available.  This option ignores overviews.");
   
   au->addCommandLineOption("-f", "<format> Will output the information specified format [KWL | XML].  Default is KWL.");   

   au->addCommandLineOption("--factories", "<keyword_list_flag> Prints factory list.  If keyword_list_flag is true, the result of a saveState will be output for each object.");
   
   au->addCommandLineOption("--ft2mtrs", "<feet> Gives meters from feet (0.3048 meters per foot).");
   
   au->addCommandLineOption("--ft2mtrs-us-survey", "<feet> Gives meters from feet (0.3048006096 meters per foot).");
   
   au->addCommandLineOption("-h", "Display this information");

   au->addCommandLineOption("--height", "<latitude-in-degrees> <longitude-in-degrees> Returns the MSL and ellipoid height given a latitude longitude position.");

   au->addCommandLineOption("-i", "Will print out the general image information.");
   
   au->addCommandLineOption("-m", "Will print out meta data image information.");

   au->addCommandLineOption("--mtrsPerDeg", "<latitude> Gives meters per degree and meters per minute for a given latitude.");
   
   au->addCommandLineOption("--mtrs2ft", "<meters> Gives feet from meters (0.3048 meters per foot).");

   au->addCommandLineOption("--mtrs2ft-us-survey", "<meters> Gives feet from meters (0.3048006096 meters per foot).");   
   
   au->addCommandLineOption("-o", "<output-file> Will output the information to the file specified.  Default is to standard out.");

   au->addCommandLineOption("--overview-types", "Prints overview builder types.");
   
   au->addCommandLineOption("-p", "Will print out the image projection information.");
   
   au->addCommandLineOption("--palette", "Will print out the color palette if one exists.");

   au->addCommandLineOption("--plugins", "Prints plugin list.");
   
   au->addCommandLineOption("--plugin-test", "Test plugin passed to option.");
   
   au->addCommandLineOption("--projections", "Prints projections.");
   
   au->addCommandLineOption("-r", "Will print image rectangle.");

   au->addCommandLineOption("--rad2deg", "<radians> Gives degrees from radians.");

   au->addCommandLineOption("--reader-props", "Prints readers and properties.");

   au->addCommandLineOption("--resampler-filters", "Prints resampler filter list.");
   
   au->addCommandLineOption("-s", "Force the ground rect to be the specified datum");
   
   au->addCommandLineOption("-v", "Overwrite existing geometry.");
   
   au->addCommandLineOption("--writer-props", "Prints writers and properties.");

   au->addCommandLineOption("--writers", "Prints list of available writers.");
   
} // void ossimInfo::addArguments(ossimArgumentParser& ap)

bool ossimInfo::initialize(ossimArgumentParser& ap)
{
   static const char M[] = "ossimInfo::initialize(ossimArgumentParser&)";
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

      bool requiresInputImage = false;
      
      while ( 1 ) //  While forever loop...
      {
         // Used throughout below:
         std::string ts1;
         ossimArgumentParser::ossimParameter sp1(ts1);
         std::string ts2;
         ossimArgumentParser::ossimParameter sp2(ts2);
         const char TRUE_KW[] = "true";

         if( ap.read("-c") )
         {
            m_kwl->add( IMAGE_CENTER_KW, TRUE_KW );
            if ( ap.argc() < 2 )
            {
               break;
            }
         }

         if( ap.read("--cg") )
         {
            m_kwl->add( CENTER_GROUND_KW, TRUE_KW );
            if ( ap.argc() < 2 )
            {
               break;
            }
         }

         if( ap.read("--ci") )
         {
            m_kwl->add( CENTER_IMAGE_KW, TRUE_KW );
            if ( ap.argc() < 2 )
            {
               break;
            }
         }

         if( ap.read("--config") || ap.read("--configuration") )
         {
            m_kwl->add( CONFIGURATION_KW, TRUE_KW );
            if ( ap.argc() < 2 )
            {
               break;
            }
         }

         if( ap.read("--datums") )
         {
            m_kwl->add( DATUMS_KW, TRUE_KW );
            if ( ap.argc() < 2 )
            {
               break;
            }
         }

         if( ap.read("--deg2rad", sp1) )
         {
            m_kwl->add( DEG2RAD_KW, ts1.c_str() );
            if ( ap.argc() < 2 )
            {
               break;
            }
         }
         if( ap.read("-d") )
         {
            m_kwl->add( DUMP_KW, TRUE_KW );
            requiresInputImage = true;
            if ( ap.argc() < 2 )
            {
               break;
            }
         }

         if( ap.read("--dno") )
         {
            m_kwl->add( DUMP_KW, TRUE_KW );
            m_kwl->add( DUMP_NO_OVERVIEWS_KW, TRUE_KW );
            requiresInputImage = true;
            if ( ap.argc() < 2 )
            {
               break;
            }
         }

         if( ap.read("-f", sp1) )
         {
            m_kwl->add( FORMAT_KW, ts1.c_str());
            if ( ap.argc() < 2 )
            {
               break;
            }
         }

         if( ap.read("--factories", sp1) )
         {
            m_kwl->add( FACTORIES_KW, TRUE_KW);
            m_kwl->add( FACTORY_KEYWORD_LIST_KW, ts1.c_str());
            if ( ap.argc() < 2 )
            {
               break;
            }
         }

         if( ap.read("--ft2mtrs", sp1) )
         {
            m_kwl->add( FT2MTRS_KW, ts1.c_str());
            if ( ap.argc() < 2 )
            {
               break;
            }
         }

         if( ap.read("--ft2mtrs-us-survey", sp1) )
         {
            m_kwl->add( FT2MTRS_KW, ts1.c_str());
            m_kwl->add( FT2MTRS_US_SURVEY_KW, TRUE_KW);
            if ( ap.argc() < 2 )
            {
               break;
            }
         }

         if( ap.read("--height", sp1, sp2) )
         {
            ossimString lat = ts1;
            ossimString lon = ts2;
            ossimGpt gpt;
            gpt.lat = lat.toFloat64();
            gpt.lon = lon.toFloat64();
            m_kwl->add( HEIGHT_KW, gpt.toString().c_str() );
            if ( ap.argc() < 2 )
            {
               break;
            }
         }
         
         if( ap.read("-i") )
         {
            m_kwl->add( IMAGE_INFO_KW, TRUE_KW );
            requiresInputImage = true;
            if ( ap.argc() < 2 )
            {
               break;
            }
         }

         if( ap.read("-m") )
         {
            m_kwl->add( METADATA_KW, TRUE_KW );
            requiresInputImage = true;
            if ( ap.argc() < 2 )
            {
               break;
            }
         }

         if( ap.read("--mtrs2ft", sp1) )
         {
            m_kwl->add( MTRS2FT_KW, ts1.c_str());
            if ( ap.argc() < 2 )
            {
               break;
            }
         }

         if( ap.read("--mtrs2ft-us-survey", sp1) )
         {
            m_kwl->add( MTRS2FT_KW, ts1.c_str());
            m_kwl->add( MTRS2FT_US_SURVEY_KW, TRUE_KW);
            if ( ap.argc() < 2 )
            {
               break;
            }
         }
         
         if( ap.read("--mtrsPerDeg", sp1) )
         {
            m_kwl->add( MTRSPERDEG_KW, ts1.c_str());
            if ( ap.argc() < 2 )
            {
               break;
            }
         }

         if( ap.read("-o", sp1) )
         {
            m_kwl->add( OUTPUT_FILE_KW, ts1.c_str());
            if ( ap.argc() < 2 )
            {
               break;
            }
         }

         if( ap.read("--overview-types") )
         {
            m_kwl->add( OVERVIEW_TYPES_KW, TRUE_KW );
            if ( ap.argc() < 2 )
            {
               break;
            }
         }

         if( ap.read("-p") )
         {
            m_kwl->add( GEOM_INFO_KW, TRUE_KW );
            requiresInputImage = true;
            if ( ap.argc() < 2 )
            {
               break;
            }
         }
         
         if( ap.read("--palette") )
         {
            m_kwl->add( PALETTE_KW, TRUE_KW );
            requiresInputImage = true;
            if ( ap.argc() < 2 )
            {
               break;
            }
         }

         if( ap.read("--plugins") )
         {
            m_kwl->add( PLUGINS_KW, TRUE_KW );
            if ( ap.argc() < 2 )
            {
               break;
            }
         }

         if( ap.read("--plugin-test", sp1) )
         {
            m_kwl->add( PLUGIN_TEST_KW, ts1.c_str());
            if ( ap.argc() < 2 )
            {
               break;
            }
         }

         if( ap.read("--projections") )
         {
            m_kwl->add( PROJECTIONS_KW, TRUE_KW );
            if ( ap.argc() < 2 )
            {
               break;
            }
         }

         if( ap.read("-r") )
         {
            m_kwl->add( IMAGE_RECT_KW, TRUE_KW );
            requiresInputImage = true;
            if ( ap.argc() < 2 )
            {
               break;
            }
         }

         if( ap.read("--rad2deg", sp1) )
         {
            m_kwl->add( RAD2DEG_KW, ts1.c_str());
            if ( ap.argc() < 2 )
            {
               break;
            }
         }

         if( ap.read("--reader-props") )
         {
            m_kwl->add( READER_PROPS_KW, TRUE_KW );
            if ( ap.argc() < 2 )
            {
               break;
            }
         }
         
         if( ap.read("--resampler-filters") )
         {
            m_kwl->add( RESAMPLER_FILTERS_KW, TRUE_KW );
            if ( ap.argc() < 2 )
            {
               break;
            }
         }

         if( ap.read("-v") )
         {
            m_kwl->add( OVERWRITE_KW, TRUE_KW );
            if ( ap.argc() < 2 )
            {
               break;
            }
         }

         if( ap.read("--writer-props") )
         {
            m_kwl->add( WRITER_PROPS_KW, TRUE_KW );
            if ( ap.argc() < 2 )
            {
               break;
            }
         }

         if( ap.read("--writers") )
         {
            m_kwl->add( WRITERS_KW, TRUE_KW );
            if ( ap.argc() < 2 )
            {
               break;
            }
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

      if ( ap.argc() == 2 )
      {
         m_kwl->add( IMAGE_FILE_KW, ap[1]  );
      }

      if ( requiresInputImage && ( ap.argc() == 1 ) )
      {
         usage(ap);
         
         // continue_after_init to false
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

void ossimInfo::execute()
{
   static const char M[] = "ossimInfo::execute()";
   
   const ossim_uint32 KEY_COUNT = m_kwl->getSize();

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << M << " entered..."
         << "\nMap size: " << KEY_COUNT << "\n";
   }

   if ( KEY_COUNT )
   {
      ossim_uint32 consumedKeys = 0;
   
      const char* lookup;

      lookup = m_kwl->find(IMAGE_FILE_KW);
      if ( lookup )
      {
         ++consumedKeys;
         ossimFilename image = lookup;

         consumedKeys += executeImageOptions(image);
      }

      if ( consumedKeys < KEY_COUNT )
      {
         ossimString value;

         lookup = m_kwl->find(CONFIGURATION_KW);
         if ( lookup )
         {
            ++consumedKeys;
            value = lookup;
            if ( value.toBool() )
            {
               printConfiguration();
            }
         }
   
         lookup = m_kwl->find(DATUMS_KW);
         if ( lookup )
         {
            ++consumedKeys;
            value = lookup;
            if ( value.toBool() )
            {
               printDatums();
            }
         }

         lookup = m_kwl->find(DEG2RAD_KW);
         if ( lookup )
         {
            ++consumedKeys;
            value = lookup;
            deg2rad( value.toFloat64() );
         }

         lookup = m_kwl->find(FACTORIES_KW);
         if ( lookup )
         {
            ++consumedKeys;
            ossimString factories = lookup;
            bool keywordListFlag = false;
            lookup = m_kwl->find(FACTORY_KEYWORD_LIST_KW);
            if ( lookup )
            {
               ++consumedKeys;
               keywordListFlag = ossimString(lookup).toBool();
            }
            printFactories(keywordListFlag);
         }

         lookup = m_kwl->find(FT2MTRS_KW);
         if ( lookup )
         {
            ++consumedKeys;
            value = lookup;
            bool us_survey = false;
            lookup = m_kwl->find(FT2MTRS_US_SURVEY_KW);
            if ( lookup )
            {
               ++consumedKeys;
               us_survey = ossimString(lookup).toBool();
            }
            ft2mtrs( value.toFloat64(), us_survey);
         }

         lookup = m_kwl->find(HEIGHT_KW);
         if ( lookup )
         {
            ++consumedKeys;
            value = lookup;
            ossimGpt gpt;
            gpt.toPoint(value);
            outputHeight(gpt);
         }

         lookup = m_kwl->find(MTRS2FT_KW);
         if ( lookup )
         {
            ++consumedKeys;
            value = lookup;
            bool us_survey = false;
            lookup = m_kwl->find(MTRS2FT_US_SURVEY_KW);
            if ( lookup )
            {
               ++consumedKeys;
               us_survey = ossimString(lookup).toBool();
            }
            mtrs2ft( value.toFloat64(), us_survey);
         }

         lookup = m_kwl->find(MTRSPERDEG_KW);
         if ( lookup )
         {
            ++consumedKeys;
            value = lookup;
            mtrsPerDeg( value.toFloat64() );
         }

         lookup = m_kwl->find(OVERVIEW_TYPES_KW);
         if ( lookup )
         {
            ++consumedKeys;
            value = lookup;
            if ( value.toBool() )
            {
               printOverviewTypes();
            }
         }

         lookup = m_kwl->find(PLUGINS_KW);
         if ( lookup )
         {
            ++consumedKeys;
            value = lookup;
            if ( value.toBool() )
            {
               printPlugins();
            }
         }

         lookup = m_kwl->find(PLUGIN_TEST_KW);
         if ( lookup )
         {
            ++consumedKeys;
            value = lookup;
            testPlugin(value);
         }

         lookup = m_kwl->find(PROJECTIONS_KW);
         if ( lookup )
         {
            ++consumedKeys;
            value = lookup;
            if ( value.toBool() )
            {
               printProjections();
            }
         }

         lookup = m_kwl->find(RAD2DEG_KW);
         if ( lookup )
         {
            ++consumedKeys;
            value = lookup;
            rad2deg( value.toFloat64() );
         }

         lookup = m_kwl->find(READER_PROPS_KW);
         if ( lookup )
         {
            ++consumedKeys;
            value = lookup;
            if ( value.toBool() )
            {
               printReaderProps();
            }
         }
   
         lookup = m_kwl->find(RESAMPLER_FILTERS_KW);
         if ( lookup )
         {
            ++consumedKeys;
            value = lookup;
            if ( value.toBool() )
            {
               printResamplerFilters();
            }
         }

         lookup = m_kwl->find(WRITERS_KW);
         if ( lookup )
         {
            ++consumedKeys;
            value = lookup;
            if ( value.toBool() )
            {
               printWriters();
            }
         }

         lookup = m_kwl->find(WRITER_PROPS_KW);
         if ( lookup )
         {
            ++consumedKeys;
            value = lookup;
            if ( value.toBool() )
            {
               printWriterProps();
            }
         }
         
      } // if ( consumedKeys < KEY_COUNT )

      if ( traceDebug() )
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "KEY_COUNT:    " << KEY_COUNT
            << "\nconsumedKeys: " << consumedKeys << "\n";
      }
         
   } // if ( KEY_COUNT )
   
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << M << " exited...\n";
   }
}

ossim_uint32 ossimInfo::executeImageOptions(const ossimFilename& file)
{
   static const char M[] = "ossimInfo::executeImageOptions()";
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << M << " entered...\nfile: " << file << "\n";
   }
   
   ossim_uint32 consumedKeys = 0;

   const char* lookup = 0;
   ossimString value = "";
   bool dnoFlag = false;
   
   // Check for dump.  Does not require image to be opened.
   lookup = m_kwl->find( DUMP_KW );
   if ( lookup )
   {
      ++consumedKeys;
      lookup = m_kwl->find( DUMP_NO_OVERVIEWS_KW );
      if ( lookup )
      {
         ++consumedKeys;
         value = lookup;
         dnoFlag = value.toBool();
      }
      dumpImage(file, dnoFlag);
   }
   
   bool centerGroundFlag = false;
   bool centerImageFlag  = false;
   bool imageCenterFlag  = false;
   bool imageRectFlag    = false;
   bool metaDataFlag     = false;
   bool paletteFlag      = false;
   bool imageInfoFlag    = false;
   bool imageGeomFlag    = false;
   
   // Center Ground:
   lookup = m_kwl->find( CENTER_GROUND_KW );
   if ( lookup )
   {
      ++consumedKeys;
      value = lookup;
      centerGroundFlag = value.toBool();
   }

   // Center Image:
   lookup = m_kwl->find( CENTER_IMAGE_KW );
   if ( lookup )
   {
      ++consumedKeys;
      value = lookup;
      centerImageFlag = value.toBool();
   }

   // Metadata:
   lookup = m_kwl->find( METADATA_KW );
   if ( lookup )
   {
      ++consumedKeys;
      value = lookup;
      metaDataFlag = value.toBool();
   }
   
   // Palette:
   lookup = m_kwl->find( PALETTE_KW );
   if ( lookup )
   {
      ++consumedKeys;
      value = lookup;
      paletteFlag = value.toBool();
   }

   // Image center:
   lookup = m_kwl->find( IMAGE_CENTER_KW );
   if ( lookup )
   {
      ++consumedKeys;
      value = lookup;
      imageCenterFlag = value.toBool();
   }

   // Image rect:
   lookup = m_kwl->find( IMAGE_RECT_KW );
   if ( lookup )
   {
      ++consumedKeys;
      value = lookup;
      imageRectFlag = value.toBool();
   }
   
   //---
   // General image info:
   // Defaulted ON if no image options set.
   //---
   lookup = m_kwl->find( IMAGE_INFO_KW ); 
   if ( lookup )
   {
      ++consumedKeys;
      value = lookup;
      imageInfoFlag = value.toBool();
   }
   
   //---
   // Image geometry info:
   // Defaulted on if no image options set.
   //---
   lookup = m_kwl->find( GEOM_INFO_KW );
   if ( lookup )
   {
      ++consumedKeys;
      value = lookup;
      imageGeomFlag = value.toBool();
   }      
   
   // If no options consumed default is image info and geom info:
   if ( consumedKeys == 0 )
   {
      imageInfoFlag = true;
      imageGeomFlag = true;
   }

   if ( centerGroundFlag || centerImageFlag || imageCenterFlag || imageRectFlag || metaDataFlag || paletteFlag || imageInfoFlag || imageGeomFlag )
   {
      // Requires open image.
      if ( m_img.valid() == false )
      {
         openImage(file);
      }

      // Output keyword list.
      ossimKeywordlist okwl;

      if ( centerGroundFlag )
      {
         getCenterGround(okwl);
      }

      if ( centerImageFlag )
      {
         getCenterImage(okwl);
      }

      if ( imageCenterFlag )
      {
         getCenterGround(okwl);
         getCenterImage(okwl);
      }

      if ( imageRectFlag )
      {
         getImageRect(okwl);
      }
    
      if ( metaDataFlag )
      {
         getImageMetadata(okwl);
      }
      
      if ( paletteFlag )
      {
         getImagePalette(okwl);
      }
      
      if ( imageInfoFlag )
      {
         getImageInfo(okwl, dnoFlag);
      }
      
      if ( imageGeomFlag )
      {
         getImageGeometryInfo(okwl, dnoFlag);
      }

      if ( imageRectFlag )
      {
         getImageRect(okwl);
      }
      
      lookup = m_kwl->find( OUTPUT_FILE_KW );
      ossimFilename outputFile;
      if ( lookup )
      {
         ++consumedKeys;
         outputFile = lookup;
      }
      
      bool xmlOutFlag = false;
      lookup = m_kwl->find( FORMAT_KW );
      if ( lookup )
      {
         ++consumedKeys;
         ossimString format = lookup;
         if ( format.upcase() == "XML" )
         {
            xmlOutFlag = true;
         }
      }
      
      bool overwriteFlag = false;
      lookup = m_kwl->find( OVERWRITE_KW );
      if ( lookup )
      {
         ++consumedKeys;
         value = lookup;
         overwriteFlag = value.toBool();
      }
      
      if ( !xmlOutFlag )
      {
         if ( outputFile == ossimFilename::NIL )
         {
            ossimNotify(ossimNotifyLevel_INFO) << okwl << std::endl;
         }
         else
         {
            // check to make sure geometry doesn't already exist
            if( outputFile.exists() && !overwriteFlag )
            {
               ossimNotify(ossimNotifyLevel_INFO)
                  << "ERROR: Geometry already exists - "  << outputFile
                  << "\nUse -v option to overwrite."
                  << std::endl;
            }
            else
            {
               okwl.write(outputFile);
            }
         }
      }
      else
      {
         ossimXmlDocument document;
         document.fromKwl( okwl );
         if ( outputFile == ossimFilename::NIL )
         {
            ossimNotify(ossimNotifyLevel_INFO) << document << std::endl;
         }
         else
         {
            // check to make sure geometry doesn't already exist
            if( outputFile.exists() && !overwriteFlag )
            {
               ossimNotify(ossimNotifyLevel_INFO)
                  << "ERROR: Geometry already exists - "  << outputFile
                  << "\nUse -v option to overwrite."
                  << std::endl;
            }
            else
            {
               document.write( outputFile );
            }
         }
      }
      
   } // if ( metaDataFlag || paletteFlag || imageInfoFlag || imageGeomFlag )
   
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "consumedKeys: " << consumedKeys << "\n"
         << M << " exited...\n";
   }
   
   return consumedKeys;
}

void ossimInfo::getImageInfo(const ossimFilename& file,
                             bool dumpFlag,
                             bool dnoFlag,
                             bool imageGeomFlag,
                             bool imageInfoFlag,
                             bool metaDataFlag,
                             bool paletteFlag,
                             ossimKeywordlist& kwl)
{
   if ( dumpFlag || dnoFlag )
   {
      dumpImage(file, dnoFlag, kwl);
   }

   // These flags requires open image.
   if ( imageGeomFlag || imageInfoFlag || metaDataFlag || paletteFlag )
   {
      // Note: openImageHandler throws ossimException if it can't open.
      ossimRefPtr<ossimImageHandler> ih = openImageHandler( file );
      if ( ih.valid() )
      {
         if ( metaDataFlag )
         {
            getImageMetadata( ih.get(), kwl );
         }
         if ( paletteFlag )
         {
            getImagePalette( ih.get(), kwl );
         }
         if ( imageInfoFlag )
         {
            getImageInfo( ih.get(), kwl, dnoFlag );
         }
         if ( imageGeomFlag )
         {
            getImageGeometryInfo( ih.get(), kwl, dnoFlag) ;
         }
      }
   }
}

void ossimInfo::openImage(const ossimFilename& file)
{
   m_img = openImageHandler( file );
}

ossimRefPtr<ossimImageHandler> ossimInfo::openImageHandler(const ossimFilename& file) const
{
   ossimRefPtr<ossimImageHandler> result = ossimImageHandlerRegistry::instance()->open(file);
   if ( result.valid() == false )
   {
      std::string errMsg = "ossimInfo::openImage ERROR:\nCould not open: ";
      errMsg += file.string();
      throw ossimException(errMsg);
   }
   return result;
}

void ossimInfo::closeImage()
{
   m_img = 0;
}

ossimRefPtr<ossimImageHandler> ossimInfo::getImageHandler()
{
   return m_img;
}

void ossimInfo::dumpImage(const ossimFilename& file, bool dnoFlag) const
{
   ossimRefPtr<ossimInfoBase> info = ossimInfoFactoryRegistry::instance()->create(file);
   if (info.valid())
   {
      if (dnoFlag) // Default info processes overviews.
      {
         info->setProcessOverviewFlag(false);
      }
      info->print(ossimNotify(ossimNotifyLevel_INFO));
      info = 0;
   }
   else
   {
      ossimNotify(ossimNotifyLevel_INFO)
         << "No dump available for:  " << file.c_str() << std::endl;
   }
}

void ossimInfo::dumpImage(const ossimFilename& file,
                          bool dnoFlag,
                          ossimKeywordlist& kwl) const
{
   ossimRefPtr<ossimInfoBase> info = ossimInfoFactoryRegistry::instance()->create(file);
   if (info.valid())
   {
      if (dnoFlag) // Default info processes overviews.
      {
         info->setProcessOverviewFlag(false);
      }
      info->getKeywordlist(kwl);
      info = 0;
   }
   else
   {
      ossimNotify(ossimNotifyLevel_INFO)
         << "No dump available for:  " << file.c_str() << std::endl;
   }
}
void ossimInfo::getImageMetadata(ossimKeywordlist& kwl) const
{
   if ( m_img.valid() )
   {
      getImageMetadata( m_img.get(), kwl);
   }
}

void ossimInfo::getImageMetadata(const ossimImageHandler* ih, ossimKeywordlist& kwl) const
{
   if ( ih )
   {
      std::vector< ossimRefPtr< ossimProperty > > list;
      ih->getPropertyList(list);
      std::vector< ossimRefPtr< ossimProperty > >::const_iterator i = list.begin();
      while (i != list.end())
      {
         if ( (*i).valid() )
         {
            ossimString key;
            ossimString value;
            
            // Check for one level of nested container.
            if ((*i)->getClassName() == "ossimContainerProperty")
            {
               ossimContainerProperty* ptr = PTR_CAST(ossimContainerProperty, (*i).get());
               if (ptr)
               {
                  std::vector< ossimRefPtr< ossimProperty > > list2;    
                  ptr->getPropertyList(list2);
                  
                  std::vector< ossimRefPtr< ossimProperty > >::const_iterator i2 = list2.begin();
                  while (i2 != list2.end())
                  {
                     key   = (*i2)->getName();
                     value = (*i2)->valueToString();
                     kwl.add(key.c_str(), value.c_str(), true);
                     ++i2;
                  }
               }
            }
            else // Not a container.
            {
               key   = (*i)->getName();
               value = (*i)->valueToString();
               kwl.add(key.c_str(), value.c_str(), true);
            }
         }
         ++i;
      }
      
   } // if ( ih )

} // End: getImageMetadata(ossimImageHandler* ih, ossimKeywordlist& kwl)

void ossimInfo::getImagePalette(ossimKeywordlist& kwl)
{
   if ( m_img.valid() )
   {
      getImagePalette( m_img.get(), kwl );
   }
}

void ossimInfo::getImagePalette(ossimImageHandler* ih, ossimKeywordlist& kwl) const
{
   if ( ih )
   {
      if(ih->getLut().valid())
      {
         ossim_uint32 entryIdx = 0;
         std::vector<ossim_uint32> entryList;
         ih->getEntryList(entryList);
         for(entryIdx = 0; entryIdx < ih->getNumberOfEntries();++entryIdx)
         {
            ih->setCurrentEntry(entryList[entryIdx]);
            ossimString prefix = "image";
            prefix = prefix + ossimString::toString(entryList[entryIdx]) + ".lut.";
            if(ih->getLut().valid())
            {
               ih->getLut()->saveState(kwl, prefix);
            }
         }
      }
      
   } // if ( ih )
}

void ossimInfo::getImageInfo(ossimKeywordlist& kwl, bool dnoFlag)
{
   if ( m_img.valid() )
   {
      getImageInfo( m_img.get(), kwl, dnoFlag );
   }
}

void ossimInfo::getImageInfo( ossimImageHandler* ih, ossimKeywordlist& kwl, bool dnoFlag ) const
{
   if ( ih )
   {
      ossim_uint32 numEntries = 0;

      std::vector<ossim_uint32> entryList;
      ih->getEntryList(entryList);

      std::vector<ossim_uint32>::const_iterator i = entryList.begin();
      while ( i != entryList.end() )
      {
         if ( getImageInfo( ih, (*i), kwl, dnoFlag ) )
         {
            ++numEntries;
         }
         ++i;
      }
      
      kwl.add(ossimKeywordNames::NUMBER_ENTRIES_KW, numEntries, true);

   } // if ( ih )
}
   
bool ossimInfo::getImageInfo( ossim_uint32 entry, ossimKeywordlist& kwl, bool dnoFlag )
{
   bool result = false;
   if ( m_img.valid() )
   {
      result = getImageInfo( m_img.get(), entry, kwl, dnoFlag );
   }
   return result;
}
 
bool ossimInfo::getImageInfo( ossimImageHandler* ih, ossim_uint32 entry, 
                              ossimKeywordlist& kwl, bool dnoFlag ) const
{
   bool result = false;
   
   if ( ih )
   {
      if ( ih->setCurrentEntry(entry) )
      {
         bool outputEntry = true;
         if ( dnoFlag )
         {
            if ( isImageEntryOverview() )
            {
               outputEntry = false;
            }
         }

         if ( outputEntry )
         {
            result = true;

            ossimString prefix = "image";
            prefix = prefix + ossimString::toString(entry) + ".";
            kwl.add(prefix, ossimKeywordNames::ENTRY_KW, entry, true);
            kwl.add(prefix, "type", ih->getClassName().c_str(), true);
            if (ih->getOverview())
            {
               kwl.add(prefix, "overview.type",
                       ih->getOverview()->getClassName().c_str(), true);
            }
            
            ossimDrect boundingRect = ih->getBoundingRect();
            kwl.add(prefix,ossimKeywordNames::UL_X_KW, boundingRect.ul().x, true);
            kwl.add(prefix,ossimKeywordNames::UL_Y_KW, boundingRect.ul().y, true);
            kwl.add(prefix,ossimKeywordNames::LR_X_KW, boundingRect.lr().x, true);
            kwl.add(prefix,ossimKeywordNames::LR_Y_KW, boundingRect.lr().y, true);
            
            const ossim_uint32 BANDS = ih->getNumberOfInputBands();
            kwl.add(prefix,ossimKeywordNames::NUMBER_INPUT_BANDS_KW, BANDS, true);
            kwl.add(prefix,ossimKeywordNames::NUMBER_OUTPUT_BANDS_KW,
                    ih->getNumberOfOutputBands(), true);
            kwl.add(prefix,ossimKeywordNames::NUMBER_LINES_KW,
                    boundingRect.height(), true);
            kwl.add(prefix,ossimKeywordNames::NUMBER_SAMPLES_KW,
                    boundingRect.width(), true);
            
            ossimScalarType scalar = ih->getOutputScalarType();
            
            for(ossim_uint32 i = 0; i < BANDS; ++i)
            {
               ossimString band = ossimString("band") + ossimString::toString(i) + ".";
               
               kwl.add(prefix, band+"null_value", ih->getNullPixelValue(i), true);
               kwl.add(prefix, band+"min_value", ih->getMinPixelValue(i), true);
               kwl.add(prefix, band+"max_value", ih->getMaxPixelValue(i), true);
            }
            
            // Output Radiometry.
            std::string rad;
            getRadiometry(scalar, rad);
            kwl.add(prefix, "radiometry", rad.c_str(), true);
            kwl.add(prefix,"number_decimation_levels", ih->getNumberOfDecimationLevels(), true);
            
         } // if ( outputEntry )
         
      } // if ( ih->setCurrentEntry(entry) )
      
   } // if ( ih )
   
   return result;

} // End: ossimInfo::getImageInfo( ih, entry...

void ossimInfo::getImageGeometryInfo(ossimKeywordlist& kwl, bool dnoFlag)
{
   if ( m_img.valid() )
   {
      getImageGeometryInfo( m_img.get(), kwl, dnoFlag );
   }
}

void ossimInfo::getImageGeometryInfo( ossimImageHandler* ih, ossimKeywordlist& kwl, bool dnoFlag) const
{
   if ( ih )
   {      ossim_uint32 numEntries = 0;

      std::vector<ossim_uint32> entryList;
      ih->getEntryList(entryList);

      std::vector<ossim_uint32>::const_iterator i = entryList.begin();
      while ( i != entryList.end() )
      {
         if ( getImageGeometryInfo( ih, (*i), kwl, dnoFlag ) )
         {
            ++numEntries;
         }
         ++i;
      }
      
      kwl.add(ossimKeywordNames::NUMBER_ENTRIES_KW, numEntries, true);

   } // if ( ih )
}
   
bool ossimInfo::getImageGeometryInfo(ossim_uint32 entry, ossimKeywordlist& kwl, bool dnoFlag)
{
   bool result = false; 
   if ( m_img.valid() )
   {
      getImageGeometryInfo( m_img.get(), entry, kwl, dnoFlag );
   }
   return result;
}

bool ossimInfo::getImageGeometryInfo( ossimImageHandler* ih,
                                      ossim_uint32 entry, 
                                      ossimKeywordlist& kwl, 
                                      bool dnoFlag) const
{
   bool result = false;
   
   if ( ih )
   {      
      if ( ih->setCurrentEntry(entry) )
      {
         bool outputEntry = true;
         if ( dnoFlag )
         {
            if ( isImageEntryOverview() )
            {
               outputEntry = false;
            }
         }

         if ( outputEntry )
         {
            ossimRefPtr<ossimImageGeometry> geom = ih->getImageGeometry();
            if(geom.valid())
            {
               result = true;
               
               ossimString prefix = "image";
               prefix = prefix + ossimString::toString(entry) + ossimString(".geometry.");
               
               geom->saveState(kwl, prefix);
               
               // Output support files list:
               ossimSupportFilesList::instance()->save(kwl, prefix);
               
               ossimGpt ulg;
               ossimGpt llg;
               ossimGpt lrg;
               ossimGpt urg;

               ossimDrect outputRect = ih->getBoundingRect();

               geom->localToWorld(outputRect.ul(), ulg);
               geom->localToWorld(outputRect.ll(), llg);
               geom->localToWorld(outputRect.lr(), lrg);
               geom->localToWorld(outputRect.ur(), urg);
               
               //---
               // *** HACK *** 
               // Encountered CADRG RPF imagery where the left edge was longitude -180 and
               // right edge +180. The projection code above reasonably maps all -180 to +180.
               // This however breaks the image footprint since it would appear that the left
               // and right edges were coincident instead of 360 degrees apart, i.e., a line
               // segment instead of a rect. So added check here for coincident left and right
               // edges and remapping left edge to -180.
               //---
               if ((ulg.lon == 180.0) && (urg.lon == 180.0))  
               {
                  ulg.lon = -180.0;
               }
               if ((llg.lon == 180.0) && (lrg.lon == 180.0))  
               {
                  llg.lon = -180.0;
               }

               kwl.add(prefix, "ul_lat", ulg.latd(), true);
               kwl.add(prefix, "ul_lon", ulg.lond(), true);
               kwl.add(prefix, "ll_lat", llg.latd(), true);
               kwl.add(prefix, "ll_lon", llg.lond(), true);
               kwl.add(prefix, "lr_lat", lrg.latd(), true);
               kwl.add(prefix, "lr_lon", lrg.lond(), true);
               kwl.add(prefix, "ur_lat", urg.latd(), true);
               kwl.add(prefix, "ur_lon", urg.lond(), true);
               
               if(!kwl.find(ossimKeywordNames::TIE_POINT_LAT_KW))
               {
                  kwl.add(prefix, ossimKeywordNames::TIE_POINT_LAT_KW, ulg.latd(), true);
                  kwl.add(prefix, ossimKeywordNames::TIE_POINT_LON_KW, ulg.lond(), true);
                  
                  if ( outputRect.height()-1.0 > DBL_EPSILON )
                  {
                     kwl.add(prefix, ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LAT,
                             fabs(ulg.latd()-llg.latd())/(outputRect.height()-1.0), true);
                  }
                  
                  if ( outputRect.width()-1.0 > DBL_EPSILON )
                  {
                     kwl.add(prefix, ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LON,
                             fabs(ulg.lond()-urg.lond())/(outputRect.width()-1.0), true);
                  }
               }
               
               ossimDpt gsd = geom->getMetersPerPixel();
               kwl.add(prefix, ossimKeywordNames::METERS_PER_PIXEL_X_KW, gsd.x, true);
               kwl.add(prefix, ossimKeywordNames::METERS_PER_PIXEL_Y_KW, gsd.y, true);
               
            } // if(geom.valid())

         } // if ( outputEntry )

      } // if ( ih->setCurrentEntry(entry) )

      if ( !result )
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "No geometry for file " << ih->getFilename() << std::endl;
      }
      
   } // if ( ih )
   
   return result;

} // End: ossimInfo::getImageGeometryInfo( ih, entry...

void ossimInfo::getCenterImage(ossimKeywordlist& kwl)
{
   if ( m_img.valid() )
   {
      getCenterImage( m_img.get(), kwl );
   }
}

void ossimInfo::getCenterImage( ossimImageHandler* ih, ossimKeywordlist& kwl) const
{
   if ( ih )
   {  
      ossim_uint32 numEntries = 0;
      
      std::vector<ossim_uint32> entryList;
      ih->getEntryList(entryList);
      
      std::vector<ossim_uint32>::const_iterator i = entryList.begin();
      while ( i != entryList.end() )
      {
         if ( getCenterImage( ih, (*i), kwl ) )
         {
            ++numEntries;
         }
         ++i;
      }
   } 
}
   
bool ossimInfo::getCenterImage(ossim_uint32 entry, ossimKeywordlist& kwl)
{
   bool result = false;   
   if ( m_img.valid() )
   {
      result = getCenterImage( m_img.get(), entry, kwl );
   }
   return result;
}

bool ossimInfo::getCenterImage( ossimImageHandler* ih,
                              ossim_uint32 entry, 
                              ossimKeywordlist& kwl ) const
{
   bool result = false;
   
   if ( ih )
   {
      if ( ih->setCurrentEntry(entry) )
      {
         ossimString prefix = "image";
         prefix = prefix + ossimString::toString(entry) + ".";
         ossimDrect outputRect = ih->getBoundingRect();
         ossimDpt iPt = outputRect.midPoint();
         kwl.add(prefix, "center_image", iPt.toString().c_str(), true);
         result = true;

      } // if ( ih->setCurrentEntry(entry) )

      if ( !result )
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "Could not get image center for: " << ih->getFilename() << std::endl;
      }
      
   } // if ( ih )

   return result;

}

void ossimInfo::getCenterGround(ossimKeywordlist& kwl)
{
   if ( m_img.valid() )
   {
      getCenterGround( m_img.get(), kwl );
   }
}

void ossimInfo::getCenterGround( ossimImageHandler* ih, ossimKeywordlist& kwl) const
{
   if ( ih )
   {  
      ossim_uint32 numEntries = 0;
      
      std::vector<ossim_uint32> entryList;
      ih->getEntryList(entryList);
      
      std::vector<ossim_uint32>::const_iterator i = entryList.begin();
      while ( i != entryList.end() )
      {
         if ( getCenterGround( ih, (*i), kwl ) )
         {
            ++numEntries;
         }
         ++i;
      }
   } 
}
   
bool ossimInfo::getCenterGround(ossim_uint32 entry, ossimKeywordlist& kwl)
{
   bool result = false;   
   if ( m_img.valid() )
   {
      result = getCenterGround( m_img.get(), entry, kwl );
   }
   return result;
}

bool ossimInfo::getCenterGround( ossimImageHandler* ih,
                              ossim_uint32 entry, 
                              ossimKeywordlist& kwl ) const
{
   bool result = false;
   
   if ( ih )
   {
      if ( ih->setCurrentEntry(entry) )
      {
         ossimString prefix = "image";
         prefix = prefix + ossimString::toString(entry) + ".";

         ossimRefPtr<ossimImageGeometry> geom = ih->getImageGeometry();
         if(geom.valid())
         {
            ossimDpt iPt;
            ossimGpt gPt;
            geom->localToWorld(iPt, gPt);
            kwl.add(prefix, "center_ground", gPt.toString().c_str(), true);
         }

         result = true;

      } // if ( ih->setCurrentEntry(entry) )

      if ( !result )
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "Could not get ground center for: " << ih->getFilename() << std::endl;
      }
      
   } // if ( ih )

   return result;

}

void ossimInfo::getImageRect(ossimKeywordlist& kwl)
{
   if ( m_img.valid() )
   {
      getImageRect( m_img.get(), kwl );
   }
}

void ossimInfo::getImageRect( ossimImageHandler* ih, ossimKeywordlist& kwl) const
{
   if ( ih )
   {  
      ossim_uint32 numEntries = 0;
      
      std::vector<ossim_uint32> entryList;
      ih->getEntryList(entryList);
      
      std::vector<ossim_uint32>::const_iterator i = entryList.begin();
      while ( i != entryList.end() )
      {
         if ( getImageRect( ih, (*i), kwl ) )
         {
            ++numEntries;
         }
         ++i;
      }
   } // if ( ih )
}
   
bool ossimInfo::getImageRect(ossim_uint32 entry, ossimKeywordlist& kwl)
{
   bool result = false;   
   if ( m_img.valid() )
   {
      result = getImageRect( m_img.get(), entry, kwl );
   }
   return result;
}

bool ossimInfo::getImageRect( ossimImageHandler* ih,
                              ossim_uint32 entry, 
                              ossimKeywordlist& kwl ) const
{
   bool result = false;
   
   if ( ih )
   {
      if ( ih->setCurrentEntry(entry) )
      {
         ossimString prefix = "image";
         prefix = prefix + ossimString::toString(entry) + ".";
         ossimIrect outputRect = ih->getBoundingRect();
         kwl.add(prefix, "image_rectangle", outputRect.toString().c_str(), true);
         result = true;

      } // if ( ih->setCurrentEntry(entry) )

      if ( !result )
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "Could not get image rectangle for: " << ih->getFilename() << std::endl;
      }
      
   } // if ( ih )

   return result;

} // End: getImageRect( ih, entry...

// Note be sure to m_img->setCurrentEntry before calling.
bool ossimInfo::isImageEntryOverview() const
{
   bool result = false; // Have to prove it.
   if ( m_img.valid() )
   {
      result = isImageEntryOverview( m_img.get() );
   }
   return result;
}
 
bool ossimInfo::isImageEntryOverview( const ossimImageHandler* ih ) const
{
   bool result = false; // Have to prove it.
   if ( ih )
   {     
      ossimString s = "imag";
      ossimRefPtr<ossimProperty> prop = ih->getProperty(s);
      if (prop.valid())
      {
         ossimString s;
         prop->valueToString(s);
         if (s.toFloat32() < 1.0)
         {
            result = true;
         }
      }
   }
   return result;
}

void ossimInfo::printConfiguration() const
{
   printConfiguration( ossimNotify(ossimNotifyLevel_INFO) );
}

std::ostream& ossimInfo::printConfiguration(std::ostream& out) const
{
   out << "version:  OSSIM "
       << ossimInit::instance()->instance()->version().c_str()
       << "\npreferences_keyword_list:\n"
       << ossimPreferences::instance()->preferencesKWL()
       << std::endl;
   return out;
}

void ossimInfo::printFactories(bool keywordListFlag)const
{
   std::vector<ossimString> typeList;
   ossimObjectFactoryRegistry::instance()->getTypeNameList(typeList);
   for(int i = 0; i < (int)typeList.size(); ++i)
   {
      if(keywordListFlag)
      {
         ossimObject* obj = ossimObjectFactoryRegistry::instance()->createObject(typeList[i]);
         if(obj)
         {
            cout << typeList[i] << endl;
            cout << "______________________________________________________" << endl;
            ossimKeywordlist kwl;
            obj->saveState(kwl);
            cout << kwl << endl;
            cout << "______________________________________________________" << endl;
            delete obj;
         }
      }
      else
      {
         cout << typeList[i] << endl;
      }
   }  
}

void ossimInfo::printDatums() const
{
   ossimInfo::printDatums( ossimNotify(ossimNotifyLevel_INFO) );
}

std::ostream& ossimInfo::printDatums(std::ostream& out) const
{
   // Capture the original flags.
   std::ios_base::fmtflags f = out.flags();

   std::vector<ossimString> datumList;
   ossimDatumFactoryRegistry::instance()->getList(datumList);
   
   std::vector<ossimString>::const_iterator i = datumList.begin();
   
   while ( i != datumList.end() )
   {
      const ossimDatum* datum = ossimDatumFactoryRegistry::instance()->create(*i);
      if (datum)
      {
         if ( datum->ellipsoid() )
         {
            out << setiosflags(ios::left)
                << setw(7)
                << datum->code().c_str()
                << setw(48)
                << datum->name().c_str()
                << setw(10) 
                << "Ellipse:"
                << datum->ellipsoid()->name()
                << std::endl;
         }
         else
         {
            out << "No ellipsoid for code: " << (*i) << std::endl;
         }
      }
      else
      {
         out << "No datum for code: " << (*i) << std::endl;
      }
      
      ++i;
   }

   // Reset flags.
   out.setf(f);
   
   return out;
}

void ossimInfo::deg2rad(const ossim_float64& degrees) const
{
   deg2rad( degrees, ossimNotify(ossimNotifyLevel_INFO) );
}

std::ostream& ossimInfo::deg2rad(const ossim_float64& degrees, std::ostream& out) const
{
   double radians = degrees * RAD_PER_DEG;

   // Capture the original flags.
   std::ios_base::fmtflags f = out.flags();
   
   out << std::setiosflags(std::ios::fixed) << std::setprecision(15)
       << "\n" << degrees << " degrees = "
       << radians << " radians.\n" << std::endl;

   // Reset flags.
   out.setf(f);

   return out;
}

void ossimInfo::rad2deg(const ossim_float64& radians) const
{
   rad2deg(radians, ossimNotify(ossimNotifyLevel_INFO));
}

std::ostream& ossimInfo::rad2deg(const ossim_float64& radians, std::ostream& out) const
{
   double degrees = radians * DEG_PER_RAD;

   // Capture the original flags.
   std::ios_base::fmtflags f = out.flags();
   
   out << std::setiosflags(std::ios::fixed) << std::setprecision(15)
       << "\n" << radians << " radians = "
       << degrees << " degrees.\n" << std::endl;

   // Reset flags.
   out.setf(f);

   return out;
}

void ossimInfo::ft2mtrs(const ossim_float64& feet, bool us_survey) const
{
   ft2mtrs( feet, us_survey, ossimNotify(ossimNotifyLevel_INFO) );
}

std::ostream& ossimInfo::ft2mtrs(const ossim_float64& feet,
                                 bool us_survey,
                                 std::ostream& out) const
{
   ossim_float64 meters = 0.0;
   std::string conversionString;
   if (us_survey)
   {
      meters = feet * US_METERS_PER_FT;
      conversionString = "0.3048006096";
   }
   else
   {
      meters = feet * MTRS_PER_FT;
      conversionString = "0.3048";
   }

   // Capture the original flags.
   std::ios_base::fmtflags f = out.flags();
   
   out << setiosflags(ios::fixed) << setprecision(15)
       << feet << " * " << conversionString << " = "
       << meters << " meters." << std::endl;

   // Reset flags.
   out.setf(f);

   return out;
}

void ossimInfo::mtrs2ft(const ossim_float64& meters, bool us_survey) const
{
   mtrs2ft(meters, us_survey, ossimNotify(ossimNotifyLevel_INFO));
}

std::ostream& ossimInfo::mtrs2ft(const ossim_float64& meters,
                                 bool us_survey,
                                 std::ostream& out) const
{
   // Capture the original flags.
   std::ios_base::fmtflags f = out.flags();

   double feet = 0.0;
   std::string conversionString;

   if (us_survey)
   {
      feet = meters / US_METERS_PER_FT;
      conversionString = "0.3048006096";
   }
   else
   {
      feet = meters / MTRS_PER_FT;
      conversionString = "0.3048";
   }

   out << setiosflags(ios::fixed) << setprecision(15)
       << meters << " / " << conversionString << " = "
       << feet << " feet." << std::endl;

   // Reset flags.
   out.setf(f);
   
   return out;
}

void ossimInfo::mtrsPerDeg(const ossim_float64& latitude) const
{
   mtrsPerDeg(latitude, ossimNotify(ossimNotifyLevel_INFO));
}

std::ostream& ossimInfo::mtrsPerDeg(const ossim_float64& latitude, std::ostream& out) const
{
   ossimGpt gpt(latitude, 0.0);
   ossimDpt      mpd          = gpt.metersPerDegree();  
   ossim_float64 radius       = gpt.datum()->ellipsoid()->geodeticRadius(latitude);
   ossim_float64 arcLengthLat = mpd.y/60.0;
   ossim_float64 arcLengthLon = mpd.x/60.0;
   out << setiosflags(ios::fixed) << setprecision(15)
       << "Meters per degree and minute at latitude of " << latitude << ":\n"
       << "Meters per degree latitude:   "
       << setw(20) << mpd.y << "\n"
       << "Meters per degree longitude:  "
       << setw(20) << mpd.x << "\n"
       << "Meters per minute latitude:   "
       << setw(20) << arcLengthLat << "\n"
       << "Meters per minute longitude:  "
       << setw(20) << arcLengthLon << "\n"
       << "Geodetic radius:              "
       << setw(20) << radius << "\n"
       << std::endl;
   return out;
}

void ossimInfo::outputHeight(const ossimGpt& gpt) const
{
   outputHeight(gpt, ossimNotify(ossimNotifyLevel_INFO));
}

std::ostream& ossimInfo::outputHeight(const ossimGpt& gpt, std::ostream& out) const
{
   // Capture the original flags.
   std::ios_base::fmtflags f = out.flags();

   ossim_float64 hgtAboveMsl = ossimElevManager::instance()->getHeightAboveMSL(gpt);
   ossim_float64 hgtAboveEllipsoid = ossimElevManager::instance()->getHeightAboveEllipsoid(gpt);
   ossim_float64 geoidOffset = ossimGeoidManager::instance()->offsetFromEllipsoid(gpt);
   ossim_float64 mslOffset = 0.0;
   
   if(ossim::isnan(hgtAboveEllipsoid)||ossim::isnan(hgtAboveMsl))
   {
      mslOffset = ossim::nan();
   }
   else
   {
      mslOffset = hgtAboveEllipsoid - hgtAboveMsl;
   }
   
   std::vector<ossimFilename> cellList;
   ossimElevManager::instance()->getOpenCellList(cellList);
   
   if (!cellList.empty())
   {
      out << "Opened cell:            " << cellList[0] << "\n";
   }
   else
   {
      out << "Did not find cell for point: " << gpt << "\n";
   }
   
   out << "MSL to ellipsoid delta: ";
   if (!ossim::isnan(mslOffset))
   {
      out << std::setprecision(15) << mslOffset;
   }
   else
   {
      out << "nan";
   }
   out << "\nHeight above MSL:       ";
   if (!ossim::isnan(hgtAboveMsl))
   {
      out << std::setprecision(15) << hgtAboveMsl;
   }
   else
   {
      out << "nan";
   }
   out << "\nHeight above ellipsoid: ";
   if (!ossim::isnan(hgtAboveEllipsoid))
   {
      out << std::setprecision(15) << hgtAboveEllipsoid << "\n";
   }
   else
   {
      out << "nan" << "\n";
   }
   out << "Geoid value:            ";

   if (!ossim::isnan(geoidOffset))
   {
      out << std::setprecision(15) << geoidOffset << std::endl;
   }
   else
   {
      out << "nan" << std::endl;
   }

   // Reset flags.
   out.setf(f);
   
   return out;
}

void ossimInfo::printPlugins() const
{
   printPlugins(ossimNotify(ossimNotifyLevel_INFO));
}

std::ostream& ossimInfo::printPlugins(std::ostream& out) const
{
   if(ossimSharedPluginRegistry::instance()->getNumberOfPlugins() > 0)
   {
      ossimSharedPluginRegistry::instance()->printAllPluginInformation(out);
   }
   else
   {
      out << "No plugins loaded in the OSSIM core library" << std::endl;
   }
   return out;
}

void ossimInfo::testPlugin(const ossimFilename& plugin) const
{
   testPlugin(plugin, ossimNotify(ossimNotifyLevel_INFO));
}

std::ostream& ossimInfo::testPlugin(const ossimFilename& plugin, std::ostream& out) const
{
   if( ossimSharedPluginRegistry::instance()->registerPlugin(plugin.expand()) )
   {
      out << "Plugin loaded: " << plugin << std::endl;
   }
   else
   {
      out << "Unable to load plugin: " << plugin << std::endl;
   }
   return out;
}

void ossimInfo::printOverviewTypes() const
{
   printOverviewTypes(ossimNotify(ossimNotifyLevel_INFO));
}

std::ostream& ossimInfo::printOverviewTypes(std::ostream& out) const
{
   out << "\nValid overview types: " << std::endl;
   
   std::vector<ossimString> outputType;
   
   ossimOverviewBuilderFactoryRegistry::instance()->getTypeNameList(outputType);
   std::copy(outputType.begin(),
             outputType.end(),
             std::ostream_iterator<ossimString>(out, "\t\n"));
   return out;
}

void ossimInfo::printProjections() const
{
   printProjections(ossimNotify(ossimNotifyLevel_INFO));
}

std::ostream& ossimInfo::printProjections(std::ostream& out) const
{
   out << "Projections:\n";
   
   std::vector<ossimString> list;
   ossimProjectionFactoryRegistry::instance()->
      getAllTypeNamesFromRegistry(list);
   
   std::vector<ossimString>::const_iterator i = list.begin();
   while ( i != list.end() )
   {
      out << *i << "\n";
      ++i;
   }
   out << std::endl;
   
   return out;
}

void ossimInfo::printReaderProps() const
{
   printReaderProps(ossimNotify(ossimNotifyLevel_INFO));
}

std::ostream& ossimInfo::printReaderProps(std::ostream& out) const
{
   return ossimImageHandlerRegistry::instance()->printReaderProps( out );
}

void ossimInfo::printResamplerFilters() const
{
   printResamplerFilters(ossimNotify(ossimNotifyLevel_INFO));
}

std::ostream& ossimInfo::printResamplerFilters(std::ostream& out) const
{
   std::vector<ossimString> list;
   ossimFilterResampler f;
   f.getFilterTypes(list);
   std::vector<ossimString>::const_iterator i = list.begin();
   while ( i != list.end() )
   {
      out << *i << "\n";
      ++i;
   }
   out << std::endl;
   return out;
}

void ossimInfo::printWriters() const
{
   printWriters(ossimNotify(ossimNotifyLevel_INFO));
}

std::ostream& ossimInfo::printWriters(std::ostream& out) const
{
   return ossimImageWriterFactoryRegistry::instance()->printImageTypeList( out );
}


void ossimInfo::printWriterProps() const
{
   printWriterProps(ossimNotify(ossimNotifyLevel_INFO));
}

std::ostream& ossimInfo::printWriterProps(std::ostream& out) const
{
   return ossimImageWriterFactoryRegistry::instance()->printWriterProps( out );
}

void ossimInfo::getRadiometry(ossimScalarType scalar, std::string& s) const
{
   // Output Radiometry.
   switch(scalar)
   {
      case OSSIM_UINT8:
      {
         s = "8-bit";
         break;
      }
      case OSSIM_USHORT11:
      {
         s = "11-bit";
         break;
      }
      case OSSIM_UINT16:
      {
         s = "16-bit unsigned";
         break;
      }
      case OSSIM_SINT16:
      {
         s = "16-bit signed";
         break;
      }
      case OSSIM_UINT32:
      {
         s = "32-bit unsigned";
         break;
      }
      case OSSIM_SINT32:
      {
         s = "32-bit signed";
         break;
      }
      case OSSIM_FLOAT32:
      {
         s = "float";
         break;
      }
      case OSSIM_NORMALIZED_FLOAT:
      {
         s = "normalized float";
         break;
      }
      default:
      {
         s = "unknown";
         break;
      }
   }
}

void ossimInfo::usage(ossimArgumentParser& ap)
{
   // Add global usage options.
   ossimInit::instance()->addOptions(ap);
   
   // Set app name.
   ap.getApplicationUsage()->setApplicationName(ap.getApplicationName());

   // Add options.
   addArguments(ap);
   
   // Write usage.
   ap.getApplicationUsage()->write(ossimNotify(ossimNotifyLevel_INFO));
   
   ossimNotify(ossimNotifyLevel_INFO)
      << " examples:\n\n" 
      << "    ossim-info --version\n"
      << "    ossim-info -i ./myfile.tif\n"
      << "      prints out only general image information\n\n"
      << "    ossim-info -p ./myfile.tif\n"
      << "      prints out only image projection information\n\n"
      << "    ossim-info -p -s wge ./myfile.tif\n"
      << "      prints out only image projection information and shifts to wgs84\n\n"
      << "    ossim-info -p -i ./myfile.tif\n"
      << "      prints out both image and projection information\n\n"
      << "    ossim-info -p -i ./myfile.tif -o ./myfile.geom\n"
      << "      writes geometry file with both image and projection information\n\n"
      << "    ossim-info -p -i ./myfile.tif -v -o ./myfile.geom\n"
      << "      writes geometry file with both image and projection information\n"
      << "      while overwriting existing .geom file.\n\n"
      << "    ossim-info -f XML ./myfile.tif\n"
      << "      prints out image and projection information as an XML document\n\n"
      << "    ossim-info -d myfile.ntf\n"
      << "      Dumps all data available, in this case, all nitf tags, from file.\n\n"
      << "    ossim-info -d a.toc\n"
      << "      Dumps all data available, in this case, all nitf and rpf tags, from file.\n\n"
      << "    ossim-info --dno a.toc\n"
      << "      \"dno\" for \"dump no overviws\" Dumps all data available,\n"
      << "       in this case, all nitf and rpf tags, from file ignoring overviews.\n\n"
      << "    ossim-info -d -i -p myfile.ntf\n"
      << "      Typical usage case, i.e. do a dump of tags and print out image and\n"
      << "      projection information.\n\n"
      << std::endl;
}
