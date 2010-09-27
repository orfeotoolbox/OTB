//----------------------------------------------------------------------------
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id: ossimOrthoIgen.cpp 18011 2010-08-31 12:48:56Z dburken $

// In Windows, standard output is ASCII by default. 
// Let's include the following in case we have
// to change it over to binary mode.
#if defined(_WIN32)
#include <io.h>
#include <fcntl.h>
#endif

#include <ossim/parallel/ossimOrthoIgen.h>
#include <sstream>
#include <ossim/base/ossimCommon.h>
#include <ossim/parallel/ossimIgen.h>
#include <ossim/parallel/ossimMpi.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimObjectFactoryRegistry.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/imaging/ossimBandSelector.h>
#include <ossim/imaging/ossimCacheTileSource.h>
#include <ossim/imaging/ossimGeoAnnotationSource.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageRenderer.h>
#include <ossim/imaging/ossimHistogramRemapper.h>
#include <ossim/imaging/ossimImageMosaic.h>
#include <ossim/imaging/ossimFilterResampler.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimMaskedImageHandler.h>
#include <ossim/imaging/ossimOrthoImageMosaic.h>
#include <ossim/imaging/ossimImageWriterFactoryRegistry.h>
#include <ossim/imaging/ossimTiffWriter.h>
#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/projection/ossimEquDistCylProjection.h>
#include <ossim/projection/ossimEpsgProjectionFactory.h>
#include <ossim/imaging/ossimGeoPolyCutter.h>
#include <ossim/imaging/ossimEastingNorthingCutter.h>
#include <ossim/imaging/ossimHistogramEqualization.h>
#include <ossim/imaging/ossimImageHistogramSource.h>
#include <ossim/imaging/ossimHistogramWriter.h>
#include <ossim/base/ossimStdOutProgress.h>
#include <ossim/base/ossimPreferences.h>

static ossimTrace traceDebug("ossimOrthoIgen:debug");
static ossimTrace traceLog("ossimOrthoIgen:log");

static const char* AUTOGENERATE_HISTOGRAM_KW = "autogenerate_histogram";

using namespace ossim;

//*************************************************************************************************
// Parses the .src file specified in the command line. These contain an alternate specification
// of input file and associated attributes as a KWL.
//*************************************************************************************************
bool ossimOrthoIgen::parseFilename(const ossimString& file_spec, bool decodeEntry)
{
   ossimSrcRecord src_record;

   std::vector<ossimString> fileInfos = file_spec.split("|");
   unsigned int num_fields = (unsigned int) fileInfos.size();
   unsigned int field_idx = 0;

   if (num_fields == 0)
      return false;

   // First field is the actual filename:
   src_record.setFilename(fileInfos[field_idx]);
   ++field_idx;

   // Next field depends on whether an entry is being decoded:
   if ((field_idx < num_fields) && decodeEntry)
   {
      src_record.setEntryIndex(fileInfos[field_idx].trim().toInt32());
      ++field_idx;
   }

   // The rest of the fields can appear in any order:
   while (field_idx < num_fields)
   {
      ossimString active_field (fileInfos[field_idx].trim());
      ossimString downcased_field (active_field);
      downcased_field.downcase();
      ++field_idx;

      // Check for overview file spec:
      ossimFilename filename (active_field);
      if (filename.contains(".ovr") || filename.isDir())
      {
         src_record.setSupportDir(filename.path());
      }
      else if (filename.contains(".mask") || filename.isDir())
      {
         src_record.setSupportDir(filename.path());
      }

      // else check for auto-minmax histogram stretch:
      else if ((downcased_field == "auto-minmax") || downcased_field.contains("std-stretch"))
      {
         src_record.setHistogramOp(downcased_field);
      }

      // Otherwise, this must be a band specification. Band numbers begin with 1:
      else
      {
         // multiple bands delimited by comma:
         std::vector<ossimString> bandsStr = active_field.split(",");
         std::vector<ossim_uint32> bands;
         for (unsigned int i = 0; i < bandsStr.size(); i++)
         {
            int band = bandsStr[i].toInt32() - 1;
            if (band >= 0)
               bands.push_back((ossim_uint32)band);
         }
         src_record.setBands(bands);
      }

   } // end of while loop parsing fileInfos spec

   theSrcRecords.push_back(src_record);
   return true;
}

//*************************************************************************************************
// Constructor
//*************************************************************************************************
ossimOrthoIgen::ossimOrthoIgen()
   :
   ossimIgen(),
   theDeltaPerPixelUnit(OSSIM_UNIT_UNKNOWN),
   theDeltaPerPixelOverride(ossim::nan(), ossim::nan()),
   theProjectionType(OSSIM_UNKNOWN_PROJECTION),
   theProjectionName(""),
   theGeographicOriginOfLatitude(0.0),
   theCombinerType("ossimImageMosaic"),
   theResamplerType("nearest neighbor"),
   theWriterType(""),
   theTemplateView(""),
   theTilingTemplate(""),
   theTilingFilename(""),
   theChainTemplate(""),
   theCombinerTemplate(""),
   theAnnotationTemplate(""),
   theWriterTemplate(""),
   theSupplementaryDirectory(""),
   theSlaveBuffers("2"),
   theCutOriginType(ossimOrthoIgen::OSSIM_CENTER_ORIGIN),
   theCutOrigin(ossim::nan(), ossim::nan()),
   theCutDxDy(ossim::nan(), ossim::nan()),
   theCutOriginUnit(OSSIM_UNIT_UNKNOWN),
   theCutDxDyUnit(OSSIM_UNIT_UNKNOWN),
   theLowPercentClip(ossim::nan()),
   theHighPercentClip(ossim::nan()),
   theStdDevClip(-1),
   theUseAutoMinMaxFlag(false),
   theScaleToEightBitFlag(false),
   theWriterProperties(),
   theCutRectSpecIsConsolidated(false),
   theTargetHistoFileName(),
   theReferenceProj(0)
{
   // setDefaultValues();
}

//*************************************************************************************************
// Initializes the argument parser
//*************************************************************************************************
void ossimOrthoIgen::addArguments(ossimArgumentParser& argumentParser)
{
   argumentParser.getApplicationUsage()->addCommandLineOption(
      "--annotate", "annotation keyword list");
   argumentParser.getApplicationUsage()->addCommandLineOption(
      "-t or --thumbnail", "thumbnail size");
   argumentParser.getApplicationUsage()->addCommandLineOption(
      "--meters","Specifies an override for the meters per pixel. Takes a single value applied "
      "equally to x and y directions.");
   argumentParser.getApplicationUsage()->addCommandLineOption(
      "--meters-xy","Specifies an override for the meters-per-pixel. Takes two values <x> <y>");
   argumentParser.getApplicationUsage()->addCommandLineOption(
      "--slave-buffers","number of slave tile buffers for mpi processing (default = 2)");
   argumentParser.getApplicationUsage()->addCommandLineOption(
      "--view-template","Specify an external file that contains view information");
   argumentParser.getApplicationUsage()->addCommandLineOption(
      "--chain-template","Specify an external file that contains chain information");
   argumentParser.getApplicationUsage()->addCommandLineOption(
      "--combiner-template","Specify an external file that contains combiner information");
   argumentParser.getApplicationUsage()->addCommandLineOption(
      "--tiling-template","Specify an external file that contains tiling information");
   argumentParser.getApplicationUsage()->addCommandLineOption(
      "--writer-template","Specify an external file that contains tiling information");
   argumentParser.getApplicationUsage()->addCommandLineOption(
      "--utm","Defaults to a utm image chain with GSD = to the input");
   argumentParser.getApplicationUsage()->addCommandLineOption(
      "--geo","Defaults to a geographic image chain with GSD = to the input.  Origin of latitude is"
      "on the equator.");
   argumentParser.getApplicationUsage()->addCommandLineOption(
      "--input-proj","Makes the view equal to the input.  If more than one file then the first is "
      "taken");
   argumentParser.getApplicationUsage()->addCommandLineOption(
      "--srs","specify an output reference frame/projection. Example: --srs EPSG:4326");
   argumentParser.getApplicationUsage()->addCommandLineOption(
      "--wkt","specify an output reference frame/projection that is in a wkt format.  Must have the"
      " ossimgdal_plugin compiled");
   argumentParser.getApplicationUsage()->addCommandLineOption(
      "--geo-scaled","Takes a latitude as an argument for purpose of scaling.  Specifies that no "
      "spec file was defined.  Defaults to a scaled geographic image chain with GSD = to the input.");
   argumentParser.getApplicationUsage()->addCommandLineOption(
      "--combiner-type","Specify what mosaic to use, ossimImageMosiac or ossimFeatherMosaic or "
      "osimBlendMosaic ... etc");
   argumentParser.getApplicationUsage()->addCommandLineOption(
      "--resample-type","Specify what resampler to use, nearest neighbor, bilinear, cubic");
   argumentParser.getApplicationUsage()->addCommandLineOption(
      "--cut-center-ll","Specify the center cut in lat lon space.  Takes two argument <lat> <lon>");
   argumentParser.getApplicationUsage()->addCommandLineOption(
      "--cut-radius-meters","Specify the cut distance in meters.  A bounding box for the cut will "
      "be produced");
   argumentParser.getApplicationUsage()->addCommandLineOption(
      "--cut-bbox-ll","Specify the min lat and min lon and max lat and maxlon <minLat> <minLon> "
      "<maxLat> <maxLon>");
   argumentParser.getApplicationUsage()->addCommandLineOption(
      "--cut-pixel-width-height","Specify cut box's width and height in pixels");
   argumentParser.getApplicationUsage()->addCommandLineOption(
      "--hist-match","Takes one image filename argument for target histogram to match."
      " Incompatible with other histogram options.");
   argumentParser.getApplicationUsage()->addCommandLineOption(
      "--hist-stretch","Specify in normalized percent the low clip and then the high clip value"
      " as <low.dd> <hi.dd>."
      " Incompatible with other histogram options.");
   argumentParser.getApplicationUsage()->addCommandLineOption(
      "--hist-std-stretch","Specify histogram stretch as a standard deviation from the mean as"
      " <int>, where <int> is 1, 2, or 3."
      " Incompatible with other histogram options.");
   argumentParser.getApplicationUsage()->addCommandLineOption(
      "--hist-auto-minmax","uses the automatic search for the best min and max clip values."
      " Incompatible with other histogram options.");
   argumentParser.getApplicationUsage()->addCommandLineOption(
      "--supplementary-directory or --support","Specify the supplementary directory path where "
      "overviews, histograms and external geometries are located");
   argumentParser.getApplicationUsage()->addCommandLineOption(
      "--scale-to-8-bit","Scales output to eight bits if not already.");
   argumentParser.getApplicationUsage()->addCommandLineOption(
      "-w or --writer","Specifies the output writer.  Default uses output file extension to "
      "determine writer.");
   argumentParser.getApplicationUsage()->addCommandLineOption(
      "--writer-prop","Passes a name=value pair to the writer for setting it's property.  Any "
      "number of these can appear on the line.");
   argumentParser.getApplicationUsage()->addCommandLineOption(
      "--stdout","Output the image to standard out.  This will return an error if writer does not "
      "support writing to standard out.  Callers should combine this with the --ossim-logfile "
      "option to ensure output image stream does not get corrupted.  You must still pass an output "
      "file so the writer type can be determined like \"dummy.png\".");
}

//*************************************************************************************************
// Initializes this objects data members given the command line args
//*************************************************************************************************
void ossimOrthoIgen::initialize(ossimArgumentParser& argumentParser)
{
   if(traceDebug())
   {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimOrthoIgen::initialize DEBUG:"
            << "Entered..... " 
            << std::endl;
   }
   double tempDouble;
   double tempDouble2;
   double tempDouble3;
   double tempDouble4;
   ossimString tempString;
   ossimArgumentParser::ossimParameter stringParam(tempString);
   ossimArgumentParser::ossimParameter doubleParam(tempDouble);
   ossimArgumentParser::ossimParameter doubleParam2(tempDouble2);
   ossimArgumentParser::ossimParameter doubleParam3(tempDouble3);
   ossimArgumentParser::ossimParameter doubleParam4(tempDouble4);
   theCutOriginType = ossimOrthoIgen::OSSIM_CENTER_ORIGIN;
   theCutOrigin.makeNan();
   theCutDxDy.makeNan();
   theCutOriginUnit   = OSSIM_UNIT_UNKNOWN;
   theCutDxDyUnit     = OSSIM_UNIT_UNKNOWN;
   theLowPercentClip  = ossim::nan();
   theHighPercentClip = ossim::nan();
   double minLat=ossim::nan(), minLon=ossim::nan(), maxLat=ossim::nan(), maxLon=ossim::nan();
   theUseAutoMinMaxFlag = false;
   theDeltaPerPixelOverride.makeNan();
   theDeltaPerPixelUnit = OSSIM_UNIT_UNKNOWN;
   if(argumentParser.read("--annotate", stringParam))
   {
      theAnnotationTemplate = ossimFilename(tempString);
   }
   if(argumentParser.read("-t", stringParam)   ||
      argumentParser.read("--thumbnail", stringParam))
   {
      ossimString comma (",");
      if (tempString.contains(comma))
      {
         theThumbnailSize.x = tempString.before(comma).toInt();
         theThumbnailSize.y = tempString.after(comma).toInt();
      }
      else
      {
         theThumbnailSize.x = tempString.toInt();
         theThumbnailSize.y = theThumbnailSize.x ;
      }
      theBuildThumbnailFlag = true;
   }

   if(argumentParser.read("-w", stringParam)   ||
      argumentParser.read("--writer", stringParam))
   {
      theWriterType = tempString;
   }

   theWriterProperties.clear();
   
   while(argumentParser.read("--writer-prop", stringParam))
   {
      std::vector<ossimString> splitArray;
      tempString.split(splitArray, "=");
      if(splitArray.size() == 2)
      {
         theWriterProperties.insert(std::make_pair(splitArray[0], splitArray[1]));
      }
   }
         
   if(argumentParser.read("--slave-buffers", stringParam))
   {
      theSlaveBuffers = tempString;
   }
   if(argumentParser.read("--cut-center-ll", doubleParam, doubleParam2))
   {
      theCutOrigin.lat = tempDouble;
      theCutOrigin.lon = tempDouble2;
      theCutOriginUnit = OSSIM_DEGREES;
      theCutOriginType = ossimOrthoIgen::OSSIM_CENTER_ORIGIN;
   }
   if(argumentParser.read("--cut-radius-meters", doubleParam))
   {
      theCutDxDy.x = tempDouble;
      theCutDxDy.y = tempDouble;
      theCutDxDyUnit = OSSIM_METERS;
   }
   if(argumentParser.read("--cut-bbox-ll", doubleParam, doubleParam2, doubleParam3, doubleParam4))
   {
      minLat = tempDouble;
      minLon = tempDouble2;
      maxLat = tempDouble3;
      maxLon = tempDouble4;
      theCutOriginUnit = OSSIM_DEGREES;
      theCutOriginType = ossimOrthoIgen::OSSIM_UPPER_LEFT_ORIGIN;
      theCutOrigin.lat = maxLat;
      theCutOrigin.lon = minLon;
      theCutDxDy.lat   = (maxLat-minLat);
      theCutDxDy.lon   = (maxLon-minLon);
      theCutDxDyUnit   = theCutOriginUnit;
   }
   if(argumentParser.read("--cut-pixel-width-height", doubleParam, doubleParam2))
   {
      if((ossim::isnan(minLat) == false)&&
         (ossim::isnan(minLon) == false)&&
         (ossim::isnan(maxLat) == false)&&
         (ossim::isnan(maxLon) == false))
      {
         theDeltaPerPixelOverride = ossimDpt(theCutDxDy.x/(tempDouble-1),
                                             theCutDxDy.y/(tempDouble2-1));
         theDeltaPerPixelUnit     = theCutDxDyUnit;
      }
      else
      {
         theCutOrigin.makeNan();
         ossimNotify(ossimNotifyLevel_WARN) << "Can't have option --cut-pixel-width-height without --cut-bbox-ll" << std::endl;
      }
   }
   
   // The three histogram options are mutually exclusive:
   bool histo_op_selected = false;
   if(argumentParser.read("--hist-match", stringParam))
   {
      ossimFilename target_image (tempString);
      histo_op_selected = true;
      
      // Check for histogram matching request and initialize for that:
      if (target_image.isReadable())
      {
         // Establish target histogram file:
         theTargetHistoFileName = target_image;
         theTargetHistoFileName.setExtension("his");
         if (!theTargetHistoFileName.isReadable())
         {
            ossimNotify(ossimNotifyLevel_NOTICE)<<"Target histogram file <" << theTargetHistoFileName 
               << "> not found. Cannot perform histogram matching." << std::endl;
            theTargetHistoFileName.clear();
         }
      }
   }
   if(argumentParser.read("--hist-stretch", doubleParam, doubleParam2))
   {
      if (histo_op_selected)
      {
         ossimNotify(ossimNotifyLevel_WARN) << "Cannot specify nore than one histogram operation. "
            " Ignoring --hist-stretch option." << std::endl;
      }
      else
      {
         theLowPercentClip = tempDouble;
         theHighPercentClip = tempDouble2;
         histo_op_selected = true;
      }
   }
   if(argumentParser.read("--hist-std-stretch", stringParam))
   {
      if (histo_op_selected)
      {
         ossimNotify(ossimNotifyLevel_WARN) << "Cannot specify nore than one histogram operation. "
            " Ignoring --hist-stretch option." << std::endl;
      }
      else
      {
         theStdDevClip = tempString.toInt32();
         histo_op_selected = true;
         if ((theStdDevClip < 1) || (theStdDevClip > 3))
         {
            ossimNotify(ossimNotifyLevel_WARN) << "Invalid standard deviation value provided with"
               " --hist-std-stretch option. Only 1,2, or 3 allowed. Ignoring option."<< std::endl;
         }
      }
   }
   if(argumentParser.read("--hist-auto-minmax"))
   {
      if (histo_op_selected)
      {
         ossimNotify(ossimNotifyLevel_WARN) << "Cannot specify nore than one histogram operation. "
            " Ignoring --hist-auto-minmax option." << std::endl;
      }
      else
         theUseAutoMinMaxFlag = true;
   }

   int num_params = argumentParser.numberOfParams("--meters", doubleParam);
   if (num_params == 1)
   {
      argumentParser.read("--meters", doubleParam);
      theDeltaPerPixelUnit = OSSIM_METERS;
      theDeltaPerPixelOverride.x = tempDouble;
      theDeltaPerPixelOverride.y = tempDouble;
   }
   else if (num_params == 2)
   {
      argumentParser.read("--meters", doubleParam, doubleParam2);
      theDeltaPerPixelUnit = OSSIM_METERS;
      theDeltaPerPixelOverride.x = tempDouble;
      theDeltaPerPixelOverride.y = tempDouble2;
   }

   if(argumentParser.read("--scale-to-8-bit"))
   {
      theScaleToEightBitFlag = true;
   }

   if (argumentParser.read("--stdout"))
   {
#if defined(_WIN32)
      // In Windows, cout is ASCII by default. 
      // Let's change it over to binary mode.
      int result = _setmode( _fileno(stdout), _O_BINARY );
      if( result == -1 )
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimOrthoIgen::initialize WARNING:"
            << "\nCannot set standard output mode to binary."
            << std::endl;
         return;
      }
#endif

      theStdoutFlag = true;
   }
   
   if(argumentParser.read("--writer-template", stringParam))
   {
      theWriterTemplate = tempString;
   }
   if(argumentParser.read("--tiling-template", stringParam))
   {
      theTilingTemplate = ossimFilename(tempString);
   }
   if(argumentParser.read("--chain-template", stringParam))
   {
      theChainTemplate = ossimFilename(tempString);
   }
   if(argumentParser.read("--combiner-template", stringParam))
   {
      theCombinerTemplate = ossimFilename(tempString);
   }
   if (argumentParser.read("--utm"))
   {
      theProjectionType = OSSIM_UTM_PROJECTION;
      theProjectionName = "ossimUtmProjection";
   }
   else if(argumentParser.read("--geo"))
   {
      theProjectionType = OSSIM_GEO_PROJECTION;
      theProjectionName = "ossimEquDistCylProjection";
   }
   else if(argumentParser.read("--input-proj"))
   {
      theProjectionType = OSSIM_INPUT_PROJECTION;
   }
   else if (argumentParser.read("--srs", stringParam))
   {
      theCrsString=tempString;
      theProjectionType = OSSIM_SRS_PROJECTION;
   }

   if(argumentParser.read("--view-template", stringParam))
   {
      theTemplateView = ossimFilename(tempString);
      theProjectionType = OSSIM_EXTERNAL_PROJECTION;
   }
   if(argumentParser.read("--geo-scaled", doubleParam))
   {
      theProjectionType = OSSIM_GEO_PROJECTION;
      theProjectionName = "ossimEquDistCylProjection";
      if ( (tempDouble < 90.0) && (tempDouble > -90.0) )
      {
         theGeographicOriginOfLatitude = tempDouble;
      }
      else
      {
         theGeographicOriginOfLatitude = 0.0;
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimOrthoIgen::initialize WARNING:"
            << "\nLatitude out  of range!  Must be between -90 and 90."
            << "\nRemains at zero."
            << std::endl;
      }
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimOrthoIgen::initialize DEBUG:"
            << "\ngeographicOriginOfLatitude:  " << theGeographicOriginOfLatitude
            << std::endl;
      }
   }
   if(argumentParser.read("--combiner-type", stringParam))
      theCombinerType = tempString;

   if(argumentParser.read("--resample-type", stringParam))
   {
      theResamplerType = tempString;
   }
   if(argumentParser.read("--supplementary-directory", stringParam) ||
      argumentParser.read("--support", stringParam))
   {
      theSupplementaryDirectory = ossimFilename(tempString);
   }

   if(traceDebug())
   {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimOrthoIgen::initialize DEBUG:"
            << "Leaving..... " 
            << std::endl;
   }
}

//*************************************************************************************************
// Adds any file specifications to the files list
//*************************************************************************************************
void ossimOrthoIgen::addFiles(ossimArgumentParser& argumentParser,
                              bool withDecoding,
                              ossim_uint32 startIdx)
{
   ossim_uint32 idx = startIdx;
   ossim_uint32 last_idx = argumentParser.argc()-1;
   while(argumentParser.argv()[idx] && (idx < last_idx))
   {
     ossimString file_spec = argumentParser.argv()[idx];
     if (file_spec.contains(".src"))
     {
        // input file spec provided via src file. Need to parse it:
        addSrcFile(ossimFilename(file_spec));
     }
     else
     {
        // Filename with optional switches explicitly provided on command line:
        parseFilename(file_spec, withDecoding);
     }
     ++idx;
   }

   // The last filename left on the command line should be the product filename:
   theProductFilename = argumentParser.argv()[last_idx];
}

//*************************************************************************************************
// Performs the top-level management of image generation
//*************************************************************************************************
bool ossimOrthoIgen::execute()
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimOrthoIgen::execute DEBUG: Entered ..."
         << std::endl;
   }
//   double start=0, stop=0;

   if(theSrcRecords.size() < 1)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimOrthoIgen::execute WARNING: No filenames to process"
         << std::endl;
      return false;
   }

   if (!theCrsString.empty() && !theProductFilename.empty())
   {
     if ((theProductFilename.ext().upcase() == "KMZ" || theProductFilename.ext().upcase() == "KML") 
       && theCrsString.upcase() != "EPSG:4326")
     {
       ossimNotify(ossimNotifyLevel_FATAL)
         << "ossimOrthoIgen::execute ERROR: Unsupported projection for kmz or kml"
         << std::endl;
       return false;
     }
   }

//    if(ossimMpi::instance()->getRank() == 0)
//    {
//       start = ossimMpi::instance()->getTime();
//    }
   if(ossimMpi::instance()->getRank() == 0)
   {
      try
      {
         setupIgenChain();
      }
      catch (const ossimException& e)
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG) << e.what() << std::endl;
         }
         throw; // re-throw exception
      }

      if (traceLog())
      {
         generateLog();
      }
   }

   try
   {
     outputProduct();
   }
   catch(const ossimException& e)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << e.what() << std::endl;
      }
      throw; // re-throw
   }
   
   return true;
}

//*************************************************************************************************
// METHOD
//*************************************************************************************************
void ossimOrthoIgen::clearFilenameList()
{
   theSrcRecords.clear();
}

//*************************************************************************************************
// Parses the .src file specified in the command line. These contain an alternate specification
// of input file and associated attributes as a KWL.
//*************************************************************************************************
void ossimOrthoIgen::addSrcFile(const ossimFilename& src_file)
{
   if (!src_file.isReadable())
      return;

   ossimKeywordlist src_kwl (src_file);
   unsigned int image_idx = 0;

   // Loop to read all image file entries:
   while (true)
   {
      ossimSrcRecord src_record(src_kwl, image_idx++);
      if (!src_record.valid()) break;
      theSrcRecords.push_back(src_record);
   }
}

//*************************************************************************************************
// METHOD
//*************************************************************************************************
void ossimOrthoIgen::setDefaultValues()
{
   theBuildThumbnailFlag = false;
   theDeltaPerPixelUnit = OSSIM_UNIT_UNKNOWN;
   theDeltaPerPixelOverride.makeNan();
   theTemplateView = "";
   theProjectionType = OSSIM_UNKNOWN_PROJECTION;
   theGeographicOriginOfLatitude = 0.0;
   theCombinerType = "ossimImageMosaic";
   theResamplerType = "nearest neighbor";
   theTilingTemplate = "";
   theTilingFilename = "";
   theSupplementaryDirectory = "";
   theSlaveBuffers = "2";
   clearFilenameList();
   theLowPercentClip = ossim::nan();
   theHighPercentClip = ossim::nan();
   theCutOrigin.makeNan();
   theCutDxDy.makeNan();
   theCutOriginUnit   = OSSIM_UNIT_UNKNOWN;
   theCutDxDyUnit     = OSSIM_UNIT_UNKNOWN;
}

//*************************************************************************************************
// Initializes the processing chain from the information on the command line
//*************************************************************************************************
void ossimOrthoIgen::setupIgenChain()
{
   if (traceDebug())
      ossimNotify(ossimNotifyLevel_DEBUG)<< "ossimOrthoIgen::setupIgenChain DEBUG: Entered ..."<< std::endl;

   setupTiling();

   if (theSlaveBuffers == "")
      theNumberOfTilesToBuffer = 2;
   else
      theNumberOfTilesToBuffer = theSlaveBuffers.toLong();

   if(theProductFilename.empty())
      throw(ossimException(std::string("Must supply an output file.")));

   // Create the output mosaic object, to be connected to its inputs later:
   ossimKeywordlist templateKwl;
   templateKwl.clear();
   ossimRefPtr<ossimImageCombiner> mosaicObject = 0;
   if(theCombinerTemplate.exists())
   {
      templateKwl.addFile(theCombinerTemplate);
      mosaicObject = PTR_CAST(ossimImageCombiner, 
                              ossimObjectFactoryRegistry::instance()->createObject(templateKwl));
   }
   if (!mosaicObject.valid())
   {
      mosaicObject = PTR_CAST(ossimImageCombiner, 
         ossimObjectFactoryRegistry::instance()->createObject(theCombinerType));
      if(!mosaicObject.valid())
      {
         mosaicObject = PTR_CAST(ossimImageMosaic, ossimObjectFactoryRegistry::instance()->
            createObject(ossimString("ossimImageMosaic")));
      }
   }

   // An orthomosaic implies that all input images are already orthorectified to a common projection
   // so the input chains do not require a renderer:
   bool orthoMosaic = (PTR_CAST(ossimOrthoImageMosaic, mosaicObject.get()) != 0);

   // Establish default individual input chain from template, if any:
   templateKwl.clear();
   ossimRefPtr<ossimImageChain> default_single_image_chain = 0;
   if(theChainTemplate.exists())
   {
      templateKwl.addFile(theChainTemplate);
      ossimObject* obj = 0;
      if(templateKwl.find("type"))
         obj = ossimObjectFactoryRegistry::instance()->createObject(templateKwl);
      else if(templateKwl.find("object1.type"))
         obj = ossimObjectFactoryRegistry::instance()->createObject(templateKwl, "object1.");  
      default_single_image_chain = PTR_CAST(ossimImageChain, obj);
   }
   if(!default_single_image_chain.valid())  // then create a default rendering chain
   {
      default_single_image_chain = new ossimImageChain;
      //if ((theProjectionType != OSSIM_UNKNOWN_PROJECTION) ||
      //    (!theDeltaPerPixelOverride.hasNans()) || theBuildThumbnailFlag)
      {
         // Only need a renderer if an output projection or an explicit GSD was specified.
         if(!orthoMosaic)
         {
            ossimImageRenderer* renderer   = new ossimImageRenderer;
            if (renderer->getResampler())
               renderer->getResampler()->setFilterType(theResamplerType);
            default_single_image_chain->addChild(renderer);
         }
      }
   }

   ossim_uint32 num_inputs = (ossim_uint32)theSrcRecords.size();
   ossim_uint32 idx;
   ossimString prefix ("object1.object");
   theReferenceProj = 0;

   // Loop over each input image file to establish a single image chain that will be added to the
   // output mosaic:
   ossimImageSource* current_source = 0;
   for(idx = 0; idx < num_inputs; ++idx)
   {
      // first lets add an input handler to the chain:
      ossimFilename input  = theSrcRecords[idx].getFilename();
      ossimRefPtr<ossimImageHandler> handler = ossimImageHandlerRegistry::instance()->open(input);
      if(!handler.valid())
      {
         ossimNotify(ossimNotifyLevel_WARN) << "Could not open input file <" << input << ">. "
            << "Skipping this entry." << std::endl;
         continue;
      }

      std::vector<ossim_uint32> entryList;
      if(theSrcRecords[idx].getEntryIndex() > -1 )
         entryList.push_back(theSrcRecords[idx].getEntryIndex());
      else
         handler->getEntryList(entryList);

      // Input image file may have multiple entries. Loop over each and establish single image
      // chains for each:
      ossim_uint32 entryIdx = 0;
      for(entryIdx = 0; entryIdx < entryList.size(); ++entryIdx)
      {
         // Instantiate the chain for one input image source. Copy existing default chain
         // which may already possess a renderer (so don't do any addFirst()!):
         ossimImageChain* singleImageChain = (ossimImageChain*) default_single_image_chain->dup();

         // Establish the image handler for this particular frame. This may be just
         // the handler already opened in the case of single image per file:
          ossimImageHandler* img_handler = 0;
         if (entryList.size() == 1)
            img_handler = handler.get();
         else
            img_handler = (ossimImageHandler*)handler->dup();

         // The user can specify an external "support" (a.k.a. supplementary directory) several ways
         if ( theSupplementaryDirectory.empty() == false )
         {
            img_handler->setSupplementaryDirectory( theSupplementaryDirectory );
         }
         else if (theSrcRecords[idx].getSupportDir().empty() == false)
         {
            img_handler->setSupplementaryDirectory(theSrcRecords[idx].getSupportDir());
         }
         else if (theSrcRecords[idx].getOverviewPath().empty() == false)
         {
            if (theSrcRecords[idx].getOverviewPath().isDir())
               img_handler->setSupplementaryDirectory(theSrcRecords[idx].getOverviewPath());
            else
               img_handler->setSupplementaryDirectory(theSrcRecords[idx].getOverviewPath().path());
         }
         img_handler->setCurrentEntry(entryList[entryIdx]);
         if ( img_handler->hasOverviews() )
         {
            img_handler->openOverview();
         }
         ossimRefPtr<ossimMaskedImageHandler> mask_img_handler = 0;
         if (theSrcRecords[idx].getMaskPath().exists())
         {
           mask_img_handler = new ossimMaskedImageHandler;
           mask_img_handler->setInputImageSource(img_handler);
           if (mask_img_handler->open(theSrcRecords[idx].getMaskPath()))
           {
             singleImageChain->addLast(mask_img_handler.get());
             current_source = mask_img_handler.get();
           }
           else
           {
             singleImageChain->addLast(img_handler);
             current_source = img_handler;
           }
         }
         else
         {
           singleImageChain->addLast(img_handler);
           current_source = img_handler;
         }

         // If this is the first input chain, use it as the reference projection to help with
         // the instantiation of the product projection (the view):
         if (!theReferenceProj.valid())
         {
            ossimRefPtr<ossimImageGeometry> geom = img_handler->getImageGeometry();
            if ( geom.valid() ) theReferenceProj = geom->getProjection();
         }

         // Install a band selector if needed:
         if (theSrcRecords[idx].getBands().size() && (img_handler->getNumberOfOutputBands() > 1))
         {
            ossim_uint32 bands = img_handler->getNumberOfOutputBands();
            bool validBand = true;
            for (ossim_uint32 i = 0; i < theSrcRecords[idx].getBands().size(); ++i)
            {
               if (theSrcRecords[idx].getBands()[i] >= bands)
               {
                  validBand = false;
                  ossimNotify(ossimNotifyLevel_FATAL) << " ERROR:" << "\nBand list range error!"
                     << "\nHighest available band:  " << bands << std::endl;
               }
            }
            if (validBand)
            {
               ossimRefPtr<ossimBandSelector> bs = new ossimBandSelector();
               singleImageChain->insertRight(bs.get(), current_source);
               bs->setOutputBandList(theSrcRecords[idx].getBands());
               current_source = bs.get();
            }
         }

         // Install a histogram object if needed:
         setupHistogram(singleImageChain, theSrcRecords[idx]);

         // Add a cache to the left of the resampler.
         addChainCache(singleImageChain);

         // Add the single image chain to the mosaic and save it to the product spec file:
         singleImageChain->makeUniqueIds();
         mosaicObject->connectMyInputTo(singleImageChain);
         theContainer->addChild(singleImageChain);
      }
   }

   // Finished initializing the inputs to the mosaic. Add the mosaic to the product chain.
   theProductChain = new ossimImageChain;
   theProductChain->addFirst(mosaicObject.get());

   // Now need to pass the product chain through the histogram setup for possible remapper given 
   // target histogram (used when histo-matching selected):
   setupHistogram();

   // When mosaicking common input projections without rendering each, need to add a renderer to the
   // mosaic for reprojecting to output projection:
   if(orthoMosaic)
   {
      ossimImageRenderer* renderer   = new ossimImageRenderer;
      renderer->getResampler()->setFilterType(theResamplerType);
      theProductChain->addFirst(current_source);
   }

   // Set up the output product's projection:
   setupProjection();

   // Annotation setup...
   setupAnnotation();

   // Output rect cutter:
   setupCutter();

   // Base class makes sure the product view projection is properly wired into the chain:
   setView();

   // After all the connections have been established, add the product chain to the overall 
   // product container. This container will also hold the writer object.
   theContainer->addChild(theProductChain.get());

   // Lastly, set up the write object (object2):
   setupWriter();

}

//*************************************************************************************************
void ossimOrthoIgen::setupCutter()
{
   ossimImageSource* input_source = theProductChain->getFirstSource();
   if(theCutDxDy.hasNans() || !theProductProjection.valid() || !input_source)
      return;

   // Before anything, make sure the cut rect is relative to UL:
   if (!theCutRectSpecIsConsolidated)
      consolidateCutRectSpec();

   // Assumed WGS-84 coordinates specified on command line:
   ossimGpt originLatLon(theCutOrigin.lat, theCutOrigin.lon, 0.0);

   ossimDpt uli (0,0); // input image UL pixel origin (we want to be an integral distance (in pixels from here)
   if(!theProductProjection->isGeographic())  // projection in meters...
   {
      ossimEastingNorthingCutter* cutter = new ossimEastingNorthingCutter;
      ossimDpt ul (theProductProjection->forward(originLatLon));
      ossimDpt lr (ul.x + theCutDxDy.x, ul.y - theCutDxDy.y);
      cutter->setEastingNorthingRectangle(ul, lr);
      theProductChain->addFirst(cutter);
   }

   else // geographic projection, units = decimal degrees.
   {
      ossimGeoPolyCutter* cutter = new ossimGeoPolyCutter;
      std::vector<ossimGpt> polygon;

      ossimGpt ul(originLatLon.lat,                originLatLon.lon               );
      ossimGpt ur(originLatLon.lat,                originLatLon.lon + theCutDxDy.x);
      ossimGpt lr(originLatLon.lat - theCutDxDy.y, originLatLon.lon + theCutDxDy.x);
      ossimGpt ll(originLatLon.lat - theCutDxDy.y, originLatLon.lon               );

      polygon.push_back(ul);
      polygon.push_back(ur);
      polygon.push_back(lr);
      polygon.push_back(ll);

      cutter->setView(theProductProjection.get());
      cutter->setNumberOfPolygons(1);
      cutter->setPolygon(polygon);
      theProductChain->addFirst(cutter);
   }

   return;
}

void ossimOrthoIgen::setupWriter()
{
   if (!theProductChain.valid())
      return;

   ossimRefPtr<ossimImageFileWriter> writer = 0;
   
   if (theWriterType.size())
   {
       // User selected writer with -w or --writer option.
      writer = ossimImageWriterFactoryRegistry::instance()->createWriter(theWriterType);
   }
   else if ( theWriterTemplate.size() && theWriterTemplate.exists() )
   {
      // User sent us a writer template.
      ossimKeywordlist kwlTemplate;
      kwlTemplate.addFile(theWriterTemplate);

      // Try first with no prefix.
      writer = ossimImageWriterFactoryRegistry::instance()->createWriter(kwlTemplate);
      if ( !writer.valid() )
         writer = ossimImageWriterFactoryRegistry::instance()->createWriter(kwlTemplate, "object2.");
   }
   else if ( theTilingFilename == "%SRTM%")
   {
      ossimKeywordlist kwlWriter;
      kwlWriter.add("type", "ossimGeneralRasterWriter", true);
      kwlWriter.add("byte_order", "big_endian");
      writer = ossimImageWriterFactoryRegistry::instance()->createWriter(kwlWriter);
      theProductFilename = theProductFilename.path();
   }

   try
   {
      //---
      // Set the output file name if not already set.
      // NOTE: Could be outputing to stdout in which case outputFilename does not
      // make sense.  Leaving here though to not break code downstream. (drb)
      //---
      if ( theProductFilename == ossimFilename::NIL )
      {
         throw(ossimException(std::string("Writer output filename not set.")));
      }

      //---
      // Final check for writer.
      //---
      if ( !writer.valid() )
      {
         // Derive writer from the extension.
         ossimFilename ext = theProductFilename.ext();
         if ( ext.size() )
            writer = ossimImageWriterFactoryRegistry::instance()->createWriterFromExtension(ext);

         //---
         // Lastly default to tiff.  Perhaps throw exception here instead. (drb)
         //---
         if( !writer.valid() )
         {
            writer = new ossimTiffWriter;
            theProductFilename.setExtension("tif");
         }
      }

      //---
      // Set writer filename, connect and add to writer to keyword list.
      //---
      if ( writer.valid() )
      {
         writer->setFilename(theProductFilename);
         if(theScaleToEightBitFlag)
            writer->setScaleToEightBitFlag(theScaleToEightBitFlag);

         writer->connectMyInputTo(0, theProductChain.get());

         ossimPropertyInterface* propInterface = (ossimPropertyInterface*)writer.get();
         PropertyMap::iterator iter = theWriterProperties.begin();
         while(iter != theWriterProperties.end())
         {
            propInterface->setProperty(iter->first, iter->second);
            ++iter;
         }
         theContainer->addChild(writer.get());
      }
      else
      {
         throw(ossimException(std::string("Unable to create writer.")));
      }
   }
   catch (const ossimException& e)
   {
      if (traceDebug())
         ossimNotify(ossimNotifyLevel_DEBUG) << e.what() << std::endl;
      throw; // re-throw exception
   }
}


//*************************************************************************************************
// This method establishes the output (view) projection of the product.
// NOTE: Completely rewritten to simplify and reduce redundancy. OLK 3/10
//*************************************************************************************************
void ossimOrthoIgen::setupProjection()
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<<"Entering ossimOrthoIgen::setupProjection():"<<std::endl;
   }

   theProductProjection = 0;

   // Throw exception if no valid input image projection was established:
   if(!theReferenceProj.valid())
   {
      std::string errMsg = "ossimOrthoIgen::setupView() -- Could not establish input image's "
         "projection. Cannot setup output view.";
      throw(ossimException(errMsg));
   }

   // Define some quantities that may be needed for the output projection:
   ossimDpt metersPerPixel = theReferenceProj->getMetersPerPixel();
   double meanGSD = (metersPerPixel.x+metersPerPixel.y)*.5;
   ossimDpt gsd(meanGSD, meanGSD);
   ossimUnitType gsdUnits = OSSIM_METERS;
   if(!theDeltaPerPixelOverride.hasNans())
   {
      gsd = theDeltaPerPixelOverride;
      gsdUnits = theDeltaPerPixelUnit;
   }

   // Now focus on establishing the output product projection.
   // Consider externally specified geometry first:
   if (theProjectionType == OSSIM_EXTERNAL_PROJECTION)
   {
      if (!theTemplateView.isReadable())
      {
         std::string errMsg = "ossimOrthoIgen::setupView() -- Could not read the product "
            "projection template file at <";
         errMsg += theTemplateView;
         errMsg += ">. Cannot establish output projection.";
         throw(ossimException(errMsg));
      }

      // Default template format is no prefix, but consider alternate with prefix if first attempt 
      // fails:
      ossimKeywordlist templateKwl (theTemplateView);
      ossimObjectFactoryRegistry* ofr = ossimObjectFactoryRegistry::instance();
      ossimRefPtr<ossimObject> productObj = ofr->createObject(templateKwl, "product.projection.");
      if(!productObj.valid())
         productObj = ofr->createObject(templateKwl);
      theProductProjection = PTR_CAST(ossimMapProjection, productObj.get());
   }

   // Geographic? (Assuming WGS 84 for now.)
   else if (theProjectionType == OSSIM_GEO_PROJECTION)
   {
      theProductProjection = new ossimEquDistCylProjection();
      ossimGpt gpt(theGeographicOriginOfLatitude, 0.0);
      theProductProjection->setOrigin(gpt);
   }

   // CRS code specified on the command line
   else if (theProjectionType == OSSIM_SRS_PROJECTION)
   {
      ossimProjection* base_proj = 
        ossimEpsgProjectionFactory::instance()->createProjection(theCrsString);

      theProductProjection = PTR_CAST(ossimMapProjection, base_proj);
      if(theProductProjection.valid())
      {
         // Reassign the type for geographic. Now we know
         if (theProductProjection->isGeographic())
         {
            theProjectionType = OSSIM_GEO_PROJECTION;
            ossimGpt gpt(theGeographicOriginOfLatitude, 0.0);
            theProductProjection->setOrigin(gpt);
         }
      }
      else
      {
         theProjectionType = OSSIM_UNKNOWN_PROJECTION;
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimOrthoIgen::srs WARNING:" << " Unsupported spatial reference system."
            << " Will default to the projection from the input image."
            << std::endl;
      }
   }

   // UTM?
   else if (theProjectionType == OSSIM_UTM_PROJECTION)
   {
      ossimUtmProjection* utm = new ossimUtmProjection;
      ossimGpt refGpt;
      theReferenceProj->lineSampleToWorld(ossimDpt(0,0), refGpt);

      utm->setZone(refGpt);
      utm->setHemisphere(refGpt);
      theProductProjection = utm;
   }

   // None of the above?
   else
   {
      // Either OSSIM_INPUT_PROJECTION or OSSIM_UNKNOWN_PROJECTION. In both cases
      // just use the first image's input projection for the output. Need to make 
      // sure the input_proj is a map projection though:
      ossimMapProjection* map_proj = PTR_CAST(ossimMapProjection, theReferenceProj.get());
      if (map_proj)
      {
         theProductProjection = PTR_CAST(ossimMapProjection, map_proj->dup());
         theProjectionType = OSSIM_INPUT_PROJECTION; // just in case it was unknown before
      }
      else
      {  
         theProjectionType = OSSIM_GEO_PROJECTION;
         theProductProjection = new ossimEquDistCylProjection();
         ossimGpt gpt(theGeographicOriginOfLatitude, 0.0);
         theProductProjection->setOrigin(gpt);

         // std::string errMsg = "ossimOrthoIgen::setupView() -- First input image's projection must "
         //    "be a map projection when specifying \"input-proj\".";
         // throw(ossimException(errMsg));
      }  
   }

   // At this point there should be a valid output projection defined:
   if (!theProductProjection.valid())
   {
      std::string errMsg = "ossimOrthoIgen::setupView() -- Could not establish valid output "
         "projection";
      throw(ossimException(errMsg));
   }

   // HACK: The projection may not have had the PCS code initialized even though it
   // is an EPSG projection, so take this opportunity to identify a PCS for output:
   theProductProjection->getPcsCode();

   //Perform some tasks common to all projection types:
   if (gsdUnits == OSSIM_DEGREES)
      theProductProjection->setDecimalDegreesPerPixel(gsd);
   else
      theProductProjection->setMetersPerPixel(gsd);
   theProjectionName = theProductProjection->getProjectionName();

   // Fix the tiepoint misalignment between input and output image projections:
   consolidateCutRectSpec();
   snapTiePointToRefProj();

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimOrthoIgen::setupView DEBUG:"
         << "Leaving...." << __LINE__
         << std::endl;
   }
}

//*************************************************************************************************
// METHOD
//*************************************************************************************************
void ossimOrthoIgen::setupAnnotation()
{
   ossimImageSource* input_source = theProductChain->getFirstSource();
   if (!input_source)
      return;

   if(theAnnotationTemplate.exists() == false)
      return;
   
   ossimKeywordlist templateKwl;
   if (templateKwl.addFile(theAnnotationTemplate) == false)
      return;
   
   ossimRefPtr<ossimObject> obj = ossimObjectFactoryRegistry::instance()->
      createObject(templateKwl, "object1.");
   if (obj.valid())
   {
      ossimGeoAnnotationSource* oga = PTR_CAST(ossimGeoAnnotationSource, obj.get());
      if (oga)
      {
         if (theProductProjection.valid())
            oga->setGeometry(new ossimImageGeometry(0, theProductProjection.get())); 
         theProductChain->addFirst(oga);
      }
   }
   return;
}

//*************************************************************************************************
// Set up multi-file tiling if indicated on the command line.
//*************************************************************************************************
bool ossimOrthoIgen::setupTiling()
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimOrthoIgen::setupTiling: Entered......" << std::endl;
   }
   ossimKeywordlist templateKwl;
   ossimFilename outputFilename = theSrcRecords[theSrcRecords.size()-1].getFilename();
   theTilingEnabled = false;

   if ((theTilingTemplate == "")||(!templateKwl.addFile(theTilingTemplate)))
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimOrthoIgen::setupTiling: Leaving......" << __LINE__ << std::endl;
      }
      return false;
   }

   ossimString prefix ("igen.tiling.");
   while (1)
   {
      if(outputFilename.isDir())
      {
         if(templateKwl.find(prefix.chars(), "type"))
         {
            theTilingFilename = templateKwl.find(prefix.chars(),"tile_name_mask");
            theTilingEnabled = true;
            break;
         }
      }
      else
      {
         theTilingFilename = outputFilename.file();
         if(templateKwl.find(prefix.chars(), "type"))
         {
            templateKwl.add(prefix.chars(), "tile_name_mask", theTilingFilename.c_str(), true);
            ossimFilename path (outputFilename.path());
            theSrcRecords[theSrcRecords.size()-1].setFilename(path);
            theTilingEnabled = true;
            break;
         }
      }

      // If we got here, then no matches were found in the template. Try again but without a prefix:
      if (prefix.empty())
         break;
      prefix.clear();
   }

   // Initialize the tiling object if enabled:
   if (theTilingEnabled && !theTiling.loadState(templateKwl, prefix))
      theTilingEnabled = false;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimOrthoIgen::setupTiling: templateKwl = \n" << templateKwl << std::endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimOrthoIgen::setupTiling: Leaving......" << __LINE__ << std::endl;
   }
   
   return true;
}

//*************************************************************************************************
// Consolidates specification of bounding rect given various ways of specifying on the command
// line. This avoids multiple, redundant checks scattered throughout the code. On exit:
// 
//   1. theCutOriginType is converted to OSSIM_UPPER_LEFT_ORIGIN
//   2. theCutDxDy reflects the full size of the rect, in the units corresponding to the projection
//   3. theCutDxDyUnit is METERS for UTM, DEGREES for geographic
//   4. A boolean data member (theCutRectSpecIsConsolidated) is set. This is checked throughout to
//      make sure the consolidation was done.
//
//*************************************************************************************************
void ossimOrthoIgen::consolidateCutRectSpec()
{
   theCutRectSpecIsConsolidated = true;
   if (!theProductProjection.valid() || theCutDxDy.hasNans())
      return; 

   if ((theCutDxDyUnit != OSSIM_METERS) && 
       (theCutDxDyUnit != OSSIM_DEGREES) &&
       (theCutDxDyUnit != OSSIM_UNIT_UNKNOWN))
   {
      ossimNotify(ossimNotifyLevel_WARN) << "ossimOrthoIgen::consolidateCutRectSpec: An unhandled"
         " type of units was encountered. The cut rect needs to be specified in either meters or"
         " degrees. The resulting cut rect and origin may be incorrect." << std::endl;
      return;
   }

   // The lat lon of the origin is assumed WGS_84:
   ossimGpt originPLH (theCutOrigin.lat, theCutOrigin.lon);
   if(theProductProjection->isGeographic()) 
   {
      // geographic projection; units need to be decimal degrees:
      if (theCutDxDyUnit == OSSIM_METERS)
      {
         // POTENTIAL BUG: converion from meters to degrees longitude should be a function 
         //                of latitude here. Implemented here but needs testing:
         ossimDpt mtrs_per_deg (originPLH.metersPerDegree());
         theCutDxDy.x = theCutDxDy.x/mtrs_per_deg.x;
         theCutDxDy.y = theCutDxDy.y/mtrs_per_deg.y;
         theCutDxDyUnit = OSSIM_DEGREES;
      }
      else // unknown, assume native units
         theCutDxDyUnit = OSSIM_DEGREES;

      std::vector<ossimGpt> polygon;

      if (theCutOriginType == OSSIM_CENTER_ORIGIN)
      {
         theCutOrigin.lat += theCutDxDy.y;
         theCutOrigin.lon += theCutDxDy.x;
      }
   }

   else 
   {
       // projection in meters; units need to be in meters:
      if (theCutDxDyUnit == OSSIM_DEGREES)
      {
         // POTENTIAL BUG: conversion from degrees longitude to meters should be a function 
         //                of latitude here. Implemented here but needs testing:
         ossimDpt mtrs_per_deg (originPLH.metersPerDegree());
         theCutDxDy.x = theCutDxDy.x * mtrs_per_deg.x;
         theCutDxDy.y = theCutDxDy.y * mtrs_per_deg.y;
         theCutDxDyUnit = OSSIM_METERS;
      }
      else // unknown, assume native units
         theCutDxDyUnit = OSSIM_METERS;

      if (theCutOriginType == OSSIM_CENTER_ORIGIN)
      {
         ossimDpt originEN = theProductProjection->forward(originPLH);
         originEN = originEN + theCutDxDy;
         originPLH = theProductProjection->inverse(originEN);
         theCutOrigin.lat = originPLH.lat;
         theCutOrigin.lon = originPLH.lon;
      }
   }

   // One more check then everything should be relative to UL:
   if (theCutOriginType == OSSIM_CENTER_ORIGIN)
   {
      // theCutDxDy in this case represented a radius. This needs to be converted to 
      // OSSIM_UPPER_LEFT_ORIGIN form:
      theCutDxDy.x *= 2.0;
      theCutDxDy.y *= 2.0;
      theCutOriginType = OSSIM_UPPER_LEFT_ORIGIN;
   }
}

//*************************************************************************************************
// Implemented to fix the misalignment between the input projection and the product. This was
// due to an UL corner in the product that was not an integral distance (in pixels) from the UL
// corner of the input image (assuming single input). OLK 3/10
//*************************************************************************************************
void ossimOrthoIgen::snapTiePointToRefProj()
{
   if (!theProductProjection.valid())
      return;

   // Make sure the AOI is specified in terms of UL corner and distance towards LR:
   if (!theCutRectSpecIsConsolidated)
      consolidateCutRectSpec();

   // When no cutting is requested, then it's the trivial case of simply copying the input
   // projection's tiepoint to the output:
   if (theCutOrigin.hasNans())
   {
      establishMosaicTiePoint();
      return;
   }

   // The notion of snapping to a tiepoint is only valid for a map projected input:
   ossimMapProjection* in_proj = PTR_CAST(ossimMapProjection, theReferenceProj.get());
   if (!in_proj)
      return;

   ossimDpt uli (0,0); // upper left corner of input image in pixels
   ossimGpt ulg; // input image UL geographic origin.
   in_proj->lineSampleToWorld(uli, ulg);

   // We will be modifying the projection's tiepoint (a.k.a. UL corner point):
   ossimGpt cutUL_gpt(theCutOrigin.lat, theCutOrigin.lon, 0.0, theProductProjection->getDatum());
   if(in_proj->isGeographic())  // geographic projection, units = decimal degrees.
   {
      // Need to use the degrees-per-pixel quantity for accurate snap:
      ossimDpt degPerPixel (in_proj->getDecimalDegreesPerPixel());

      // Establish offset from cut-rect UL to input image UL in integral pixels, and
      // correct the cut-rect's origin for the misalignment:
      double dLat = cutUL_gpt.lat - ulg.lat;
      dLat = ossim::round<int>(dLat/degPerPixel.y) * degPerPixel.y;
      cutUL_gpt.lat = ulg.lat + dLat - 0.5*degPerPixel.y;

      double dLon = cutUL_gpt.lon - ulg.lon;
      dLon = ossim::round<int>(dLon/degPerPixel.x) * degPerPixel.x;
      cutUL_gpt.lon = ulg.lon + dLon - 0.5*degPerPixel.x;

      theProductProjection->setUlTiePoints(cutUL_gpt);
   }

   else    // projection in meters...
   {
      // Establish the easting northing (in input projection space) of the UL cut rect:
      ossimDpt cut_ul_EN_in = in_proj->forward(cutUL_gpt);

      // Directly use the meters offset from input image UL in computing snap shift:
      ossimDpt ul_EN_in; // input image UL E/N origin.
      ossimDpt mtrsPerPixel (theProductProjection->getMetersPerPixel());
      in_proj->lineSampleToEastingNorthing(uli, ul_EN_in);

      // Establish offset from cut-rect UL to input image UL in integral pixels, and
      // correct the cut-rect's origin for the misalignment:
      double dE = cut_ul_EN_in.x - ul_EN_in.x;
      dE = ((int)(dE/mtrsPerPixel.x)) * mtrsPerPixel.x;
      cut_ul_EN_in.x = ul_EN_in.x + dE; // + 0.5*mtrsPerPixel.x;

      double dN = cut_ul_EN_in.y - ul_EN_in.y;
      dN = ((int)(dN/mtrsPerPixel.y)) * mtrsPerPixel.y;
      cut_ul_EN_in.y = ul_EN_in.y + dN; // - 0.5*mtrsPerPixel.y;

      // Convert input projection easting northing to geographic tiepoint:
      cutUL_gpt = in_proj->inverse(cut_ul_EN_in);
      theProductProjection->setUlTiePoints(cutUL_gpt);

      // Update the cut rect with new UL coordinates:
      theCutOrigin.lat = cutUL_gpt.lat;
      theCutOrigin.lon = cutUL_gpt.lon;
   }
}

//*************************************************************************************************
//! Sets up the histogram operation requested for the image chain passed in.
//*************************************************************************************************
void ossimOrthoIgen::setupHistogram(ossimImageChain* input_chain, const ossimSrcRecord& src_record)
{
   // Check if the source passed in is the output mosaic object, because the target
   // histogram remapper needs to be connected to it (only valid when histo matching is requested):
   if (input_chain == NULL)
   {
      if (!theTargetHistoFileName.isReadable())
         return;

      ossimHistogramRemapper* remapper = new ossimHistogramRemapper;
      remapper->openHistogram(theTargetHistoFileName);
      theProductChain->addFirst(remapper);
      return;
   }

   // Check if any histo operation was requested on individual image:
   if ((ossim::isnan(theHighPercentClip) || ossim::isnan(theLowPercentClip)) &&
      !theUseAutoMinMaxFlag && (theStdDevClip < 0) && src_record.getHistogramOp().empty() &&
      theTargetHistoFileName.empty())
   {
      return; // no histo op requested
   }

   // Remaining operations require a histogram on the input image source:
   ossimImageHandler* handler = PTR_CAST(ossimImageHandler, input_chain->getLastSource());
   if (handler == NULL)
   {
      ossimNotify(ossimNotifyLevel_FATAL)<<"Could not locate an image handler object in the image"
         << "chain provided. This should not happen. Ignoring histogram request." << std::endl;
      return;
   }

   // Establish the ideal filename for this histogram. The following do-block is all for testing
   // different histogram file naming schemes since alternate directory and entry-indexing might be
   // used:
   ossimFilename histoFilename (src_record.getHistogramPath());
   ossimFilename candidateHistoFilename;
   ossimFilename defaultHistoFilename (handler->createDefaultHistogramFilename());
   ossimFilename entryName (handler->getFilenameWithThisExtension(ossimString(".his"), true));
   do
   {
      if (!histoFilename.empty())
      {
         // Try histogram filename based on specified name in the .src file:
         if (histoFilename.isDir())
            histoFilename = histoFilename.dirCat(defaultHistoFilename.file());
         if (histoFilename.exists()) break;

         // Try specified name with entry index:
         if (src_record.getEntryIndex() >= 0)
         {
            histoFilename = histoFilename.path().dirCat(entryName.file());
            if (histoFilename.exists()) break;
         }

         // Not found so set the candidate filename in case we need to generate it:
         candidateHistoFilename = histoFilename;
      }

      // Next try looking for a histogram based on the default name:
      histoFilename = defaultHistoFilename;
      if (histoFilename.exists())  break;

      // Last possibility is the default name with entry index:
      if (src_record.getEntryIndex() >= 0)
      {
         histoFilename = entryName;
         if (histoFilename.exists())  break;
      }

      // If not already set, set the candidate filename in case we need to generate it:
      if (candidateHistoFilename.empty())
         candidateHistoFilename = histoFilename;
   }
   while (false); // only pass through once


   // If the histogram was still not located, look into creating one:
   if (!histoFilename.exists())
   {
      // Check the preferences for histogram autogeneration:
      ossimString lookup = ossimPreferences::instance()->findPreference(AUTOGENERATE_HISTOGRAM_KW);
      if (lookup.toBool())
      {
         // No histogram available for this image, need to create one:
         histoFilename = candidateHistoFilename;
         ossimNotify(ossimNotifyLevel_WARN) <<"Histogram file <" <<  histoFilename
            << "> not found. Creating one now..."  << std::endl;
         bool success = createHistogram(input_chain, histoFilename);
         if (!success)
         {
            ossimNotify(ossimNotifyLevel_WARN) <<"Error encountered creating histogram file <" 
               << histoFilename << ">. Ignoring histogram request."  << std::endl;
            return;
         }
      }
   }

   // Need to insert any histogram object to the left of the renderer in the chain. Search for a 
   // renderer and save for later:
   ossimConnectableObject* renderer = PTR_CAST(ossimConnectableObject,
      input_chain->findFirstObjectOfType(ossimString("ossimImageRenderer")));

   // Histo Match?
   if (theTargetHistoFileName.isReadable())
   {
      // A histogram match was requested. This involves applying a histo equalization to the input
      // chain and then applying an inverted equalization using the target histogram:
      ossimHistogramEqualization* forwardEq = new ossimHistogramEqualization;
      ossimHistogramEqualization* inverseEq = new ossimHistogramEqualization;
      
      // Init equalizers with the source and target histogram files:
      forwardEq->setInverseFlag(false);
      forwardEq->setHistogram(histoFilename);
      inverseEq->setInverseFlag(true);
      inverseEq->setHistogram(theTargetHistoFileName);

      // Insert to the left of renderer if one exists:
      if (renderer)
         input_chain->insertLeft(forwardEq, renderer);
      else
         input_chain->addFirst(forwardEq);
      input_chain->insertRight(inverseEq, forwardEq);
      
      return;
   }

   // Remaining possibilities (clip or stretch) require a remapper.
   // Insert to the left of renderer if one exists:
   ossimRefPtr<ossimHistogramRemapper> remapper = new ossimHistogramRemapper;
   if (renderer)
      input_chain->insertLeft(remapper.get(), renderer);
   else
      input_chain->addFirst(remapper.get());

   // Fetch the input histogram:
   bool histo_read_ok = remapper->openHistogram(histoFilename);
   if (!histo_read_ok)
   {
      // No histogram available for this image, need to create one (TODO):
      ossimNotify(ossimNotifyLevel_WARN)<<"Error encountered loading histogram file <" 
         << histoFilename << ">. No histogram operations will be performed on this image." 
         << std::endl;
      return;
   }

   // Set values to construct remap table:
   if (!ossim::isnan(theHighPercentClip) && !ossim::isnan(theLowPercentClip))
   {
      // Hi/Lo clip requested
      remapper->setHighNormalizedClipPoint(1.0-theHighPercentClip);
      remapper->setLowNormalizedClipPoint(theLowPercentClip);
   }

   else
   {
      // Consider histogram stretch operations. These can be on a per-image basis or global for all
      // input images. Give priority to the img_histo_op (per-image spec) over general flags below:
      ossimHistogramRemapper::StretchMode mode = ossimHistogramRemapper::STRETCH_UNKNOWN;
      ossimString img_histo_op (src_record.getHistogramOp());
      if (img_histo_op=="auto-minmax")
         mode = ossimHistogramRemapper::LINEAR_AUTO_MIN_MAX;
      else if (img_histo_op.contains("std-stretch"))
      {
         if (img_histo_op.contains("1"))
            mode = ossimHistogramRemapper::LINEAR_1STD_FROM_MEAN;
         else if (img_histo_op.contains("2"))
            mode = ossimHistogramRemapper::LINEAR_2STD_FROM_MEAN;
         else if (img_histo_op.contains("3"))
            mode = ossimHistogramRemapper::LINEAR_3STD_FROM_MEAN;
      }
      else if (theUseAutoMinMaxFlag)
         mode = ossimHistogramRemapper::LINEAR_AUTO_MIN_MAX;
      else if (theStdDevClip > 0)
         mode = (ossimHistogramRemapper::StretchMode) theStdDevClip;

      // Finally init the remapper with proper stretch mode:
      if (mode != ossimHistogramRemapper::STRETCH_UNKNOWN)
         remapper->setStretchMode(mode, true);
   }
   
   return;
}


//*************************************************************************************************
//! Utility method for creating a histogram for an input image. Returns TRUE if successful.
//*************************************************************************************************
bool ossimOrthoIgen::createHistogram(ossimImageChain* chain, const ossimFilename& histo_filename)
{
   ossimRefPtr<ossimImageHistogramSource> histoSource = new ossimImageHistogramSource;
   ossimRefPtr<ossimHistogramWriter> writer = new ossimHistogramWriter;

   histoSource->connectMyInputTo(chain);
   histoSource->enableSource();
   histoSource->setComputationMode(OSSIM_HISTO_MODE_FAST);

   writer->connectMyInputTo(histoSource.get());
   writer->setFilename(histo_filename);
   writer->addListener(&theStdOutProgress);
   bool success = writer->execute();

   writer=0;
   histoSource=0;

   if (success)
   {
      ossimNotify(ossimNotifyLevel_NOTICE)<<std::endl;
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)<<"Error encountered creating Histogram file <" 
         << histo_filename << ">. No histogram operations will be performed on this image." 
         << std::endl;
   }

   return success;
}

//*************************************************************************************************
// METHOD
//*************************************************************************************************
void ossimOrthoIgen::addChainCache(ossimImageChain* chain) const
{
   if (chain)
   {  
      ossimConnectableObject* renderer =
         PTR_CAST(ossimConnectableObject,
                  chain->findFirstObjectOfType(ossimString("ossimImageRenderer")));

      if (renderer)
      {
         ossimCacheTileSource* cache = new ossimCacheTileSource();
         chain->insertLeft(cache, renderer);
      }
   }
}

//*************************************************************************************************
// Generates a log KWL file that could be fed directly to Igen. Used for verifying chain.
//*************************************************************************************************
void ossimOrthoIgen::generateLog()
{
   if (!theSrcRecords.size() || !theProductChain.valid() || theProductFilename.empty())
      return;

   // Establish output filename:
   ossimFilename logFile = theProductFilename;
   logFile.setExtension("log");

   // Fill a KWL with all info:
   ossimKeywordlist kwl; 
   theContainer->saveState(kwl);

   if (theProductProjection.valid())
      theProductProjection->saveState(kwl, "product.projection.");

   kwl.write(logFile.chars());
}

//*************************************************************************************************
//! Determines the UL corner tiepoint of the product projection as the overall UL corner of the
//! mosaic.
//*************************************************************************************************
void ossimOrthoIgen::establishMosaicTiePoint()
{
   if (!theProductChain.valid())
      return;

   // Need to find all image handlers to query for their UL ground point:
   ossimConnectableObject::ConnectableObjectList clientList;
   theProductChain->findAllInputsOfType(clientList, STATIC_TYPE_INFO(ossimImageHandler), true, true);

   if (clientList.size() == 0)
   {
      ossimNotify(ossimNotifyLevel_WARN)<<"ossimOrthoIgen::establishMosaicTiePoint() WARNING -- "
         "Expected to find image handler in the chain but none was identified."<<std::endl;
      return;
   }

   // Loop over all input handlers and latch the most NW tiepoint as the mosaic TP:
   ossimConnectableObject::ConnectableObjectList::iterator iter = clientList.begin();
   ossimGpt tie_pt_i, tie_pt;
   tie_pt.makeNan();
   while (iter != clientList.end())
   {
      ossimImageHandler* handler = PTR_CAST(ossimImageHandler, (*iter).get());
      iter++;

      if (!handler) break;

      ossimRefPtr<ossimImageGeometry> geom = handler->getImageGeometry();
      if (!geom) break;

      ossimIrect boundingRect = handler->getBoundingRect();
      ossimDpt ulPt = boundingRect.ul();
      geom->localToWorld(ulPt, tie_pt_i);

      if (tie_pt.hasNans())
         tie_pt = tie_pt_i;
      else
      {
         if (tie_pt_i.lat > tie_pt.lat) tie_pt.lat = tie_pt_i.lat;
         if (tie_pt_i.lon < tie_pt.lon) tie_pt.lon = tie_pt_i.lon;
      }
   }

   if (!tie_pt.hasNans())
      theProductProjection->setUlTiePoints(tie_pt);
}
