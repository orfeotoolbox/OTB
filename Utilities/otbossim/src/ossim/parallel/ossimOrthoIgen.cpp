//----------------------------------------------------------------------------
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id: ossimOrthoIgen.cpp 17590 2010-06-17 18:09:02Z dburken $

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
#include <ossim/imaging/ossimOrthoImageMosaic.h>
#include <ossim/imaging/ossimImageWriterFactoryRegistry.h>
#include <ossim/imaging/ossimTiffWriter.h>
#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/projection/ossimEquDistCylProjection.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimPcsCodeProjectionFactory.h>
#include <ossim/imaging/ossimGeoPolyCutter.h>
#include <ossim/imaging/ossimEastingNorthingCutter.h>
#include <ossim/imaging/ossimHistogramEqualization.h>

static ossimTrace traceDebug("ossimOrthoIgen:debug");
static ossimTrace traceLog("ossimOrthoIgen:log");

using namespace ossim;

ossimOrthoIgen::ossimOrthoIgenFilename::ossimOrthoIgenFilename(const ossimFilename& file, bool decodeEntry)
{
   if(decodeEntry)
   {
      setFilenameWithDecoding(file);
   }
   else
   {
      setFilenameAndEntry(file, -1);
   }
}

void ossimOrthoIgen::ossimOrthoIgenFilename::setFilenameAndEntry(const ossimFilename& file,
                                                                 ossim_int32 entry)
{
   theEntry = entry;
   ossimFilename::size_type idx = file.rfind("|", file.length());
   ossimString actualFile = file;
   ossimString bands(""); 
   ossimString supDir("");
   bool hasOvrFile = false;

   ossimFilename tmpOvrFile = ossimString(actualFile.split("|")[actualFile.split("|").size()-1]).trim();
   if (tmpOvrFile.contains(".ovr") || tmpOvrFile.isDir())
   {
    supDir = tmpOvrFile;
    hasOvrFile = true;
   }

   if(idx != ossimFilename::npos)
   {
    if (hasOvrFile)
    {
      actualFile = ossimString(file.begin(), 
        file.begin()+idx);
    }
   }

   if (actualFile.contains("|"))
   {
    std::vector<ossimString> fileInfos = actualFile.split("|");
    if (fileInfos.size() > 1)
    {
      actualFile = fileInfos[0].trim();
      bands = fileInfos[1].trim();
    }
   }

   if (bands != "")
   {
    std::vector<ossimString> bandsStr = bands.split(",");
    for (unsigned int i = 0; i < bandsStr.size(); i++)
    {
      theBands.push_back(bandsStr[i].toUInt32()-1);
    }
   }

   if (!supDir.empty())
   {
      ossimFilename tempFullPath( supDir );

      ossimFilename drivePart;
      ossimFilename pathPart;
      ossimString filePart;
      ossimString extPart;

      tempFullPath.split( drivePart, pathPart, filePart, extPart );
      theSupplementaryDir = drivePart.dirCat(pathPart);
   }

   theFilename = actualFile;
}

void ossimOrthoIgen::ossimOrthoIgenFilename::setFilenameWithDecoding(const ossimFilename& file)
{
   ossimFilename::size_type idx = file.rfind("|", file.length());
   ossimString actualFile = file;
   ossimString entry("-1"); 
   ossimString bands(""); 
   ossimString supDir("");
   bool hasOvrFile = false;

   ossimFilename tmpOvrFile = ossimString(actualFile.split("|")[actualFile.split("|").size()-1]).trim();
   if (tmpOvrFile.contains(".ovr") || tmpOvrFile.isDir())
   {
     supDir = tmpOvrFile;
     hasOvrFile = true;
   }

   if(idx != ossimFilename::npos)
   {
     if (hasOvrFile)
     {
       actualFile = ossimString(file.begin(), 
         file.begin()+idx);
     }
   }

   std::vector<ossimString> fileInfos = actualFile.split("|");
   actualFile = fileInfos[0].trim();

   if (fileInfos.size() > 2)
   {
     entry = fileInfos[1].trim();
     bands = fileInfos[2].trim();
   }

   if (fileInfos.size() == 2)
   {
     entry = fileInfos[1].trim();
     if (entry.contains(","))//means bands
     {
       bands = entry;
       entry = "-1";
     }
   } 

   theFilename = ossimFilename(actualFile);
   theEntry    = entry.toInt32();

   if (!bands.empty())
   {
     std::vector<ossimString> bandsStr = bands.split(",");
     for (unsigned int i = 0; i < bandsStr.size(); i++)
     {
       theBands.push_back(bandsStr[i].toUInt32()-1);
     }
   }
   
   if (!supDir.empty())
   {
      ossimFilename tempFullPath( supDir );

      ossimFilename drivePart;
      ossimFilename pathPart;
      ossimString filePart;
      ossimString extPart;

      tempFullPath.split( drivePart, pathPart, filePart, extPart );
      theSupplementaryDir = drivePart.dirCat(pathPart);
   }
}

ossimOrthoIgen::ossimOrthoIgen()
   :
   theThumbnailRes(""),
   theThumbnailFlag(false),
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
   theStdoutFlag(false),
   theWriterProperties(),
   theCutRectSpecIsConsolidated(false),
   theTargetHistoFileName(),
   theContainer(new ossimConnectableContainer),
   theProductProjection(0),
   theProductChain(0),
   theKwl(),
   theFilenames(0)
{
   // setDefaultValues();
}

void ossimOrthoIgen::addArguments(ossimArgumentParser& argumentParser)
{
   argumentParser.getApplicationUsage()->addCommandLineOption(
      "--annotate", "annotation keyword list");
   argumentParser.getApplicationUsage()->addCommandLineOption(
      "-t or --thumbnail", "thumbnail resolution");
   argumentParser.getApplicationUsage()->addCommandLineOption(
   "--meters","Specifies an override for the meters per pixel");
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
      "--supplementary-directory","Specify the supplementary directory path where overviews are "
      "located");
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
      theThumbnailRes  = tempString;
      theThumbnailFlag = true;
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

   if(argumentParser.read("--meters", doubleParam))
   {
      theDeltaPerPixelUnit = OSSIM_METERS;
      theDeltaPerPixelOverride.x = tempDouble;
      theDeltaPerPixelOverride.y = tempDouble;
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
   
   if(argumentParser.read("--writer-template",
                          stringParam))
   {
      theWriterTemplate = tempString;
   }
   if(argumentParser.read("--tiling-template",
                          stringParam))
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
      theSrsString=tempString;
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
   if(argumentParser.read("--supplementary-directory", stringParam))
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

void ossimOrthoIgen::addFiles(ossimArgumentParser& argumentParser,
                              bool withDecoding,
                              ossim_uint32 startIdx)
{
   ossim_uint32 idx = startIdx;
   while(argumentParser.argv()[idx])
   {
     ossimString fileName = argumentParser.argv()[idx];
     if (fileName.contains(".src"))
     {
       addFile(fileName, withDecoding);
     }
     else
     {
       addFile(ossimFilename(fileName), withDecoding);
     }
     ++idx;
   }
}

bool ossimOrthoIgen::execute()
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimOrthoIgen::execute DEBUG: Entered ..."
         << std::endl;
   }
//   double start=0, stop=0;

   if(theFilenames.size() < 1)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimOrthoIgen::execute WARNING: No filenames to process"
         << std::endl;
      return false;
   }

//    if(ossimMpi::instance()->getRank() == 0)
//    {
//       start = ossimMpi::instance()->getTime();
//    }
   theKwl.clear();
   //ossimKeywordlist inputGeom;
   if(ossimMpi::instance()->getRank() == 0)
   {
      try
      {
         setupIgenKwl();
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
         if (theFilenames.size())
         {
            ossimFilename logFile = theFilenames[theFilenames.size()-1].theFilename;
            logFile.setExtension("log");
            theKwl.write(logFile.c_str());
         }
      }
   }

   ossimIgen *igen = new ossimIgen;
   igen->initialize(theKwl);

   try
   {
      igen->outputProduct();
   }
   catch(const ossimException& e)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << e.what() << std::endl;
      }
      delete igen;
      igen = 0;
      throw; // re-throw
   }
   
   delete igen;
   igen = 0;

   return true;
}

void ossimOrthoIgen::clearFilenameList()
{
   theFilenames.clear();
}

void ossimOrthoIgen::addFile(const ossimFilename& file,
                             bool withEncodedEntry)
{
   ossimOrthoIgenFilename filename;
   if(withEncodedEntry)
   {
      filename.setFilenameWithDecoding(file);
   }
   else
   {
      filename.setFilenameAndEntry(file, -1);
   }
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimOrthoIgen::addFile DEBUG:"
         << "\nAdded file: " << filename.theFilename
         << std::endl;
   }   

   theFilenames.push_back(filename);
}

void ossimOrthoIgen::addFile(const ossimString& fileName,
                             bool withEncodedEntry)
{
  ossimFilename f = fileName;
  std::ifstream in((f).c_str() );
  
  std::string line;
 
  // Iterate through the lines of file.
  std::vector<ossimString> fileInfos;
  while(in.good())
  {
    // Read in a line.
    std::getline(in, line);
    ossimString tmpStr = ossimString(line);
    if (tmpStr.contains(".file:")  ||
        tmpStr.contains(".entry:") ||
        tmpStr.contains(".rgb:") || 
        tmpStr.contains(".ovr:") || 
        tmpStr.contains(".file :")  ||
        tmpStr.contains(".entry :") ||
        tmpStr.contains(".rgb :") || 
        tmpStr.contains(".ovr :"))
    {
      fileInfos.push_back(tmpStr);
      continue;
    }
    else // go to next file or blank line found
    {
      if (!tmpStr.empty())
      {
        ossimNotify(ossimNotifyLevel_DEBUG)
          << "ossimOrthoIgen::addFile DEBUG:"
          << "\n" << tmpStr << " is not proper format. It will be ignored. "
          << std::endl;
        tmpStr = "";
        continue;
      }
      addFiles(tmpStr, fileInfos, withEncodedEntry);
      fileInfos.clear();//after parsing the vector, clear it and get it ready for next set of input
    }
  }
  if (fileInfos.size() > 0) // end of the file, process last set of input
  {
    addFiles("", fileInfos, withEncodedEntry);
    fileInfos.clear();
  }
  in.close();
}

void ossimOrthoIgen::addFiles(ossimString fileInfoStr, 
                              std::vector<ossimString> fileInfos,
                              bool withEncodedEntry)
{
  if (fileInfos.size() > 0)
  {
    if (fileInfos[0].contains(".file:"))
    {
      fileInfoStr = fileInfos[0].after(".file:").trim();
    }
    else if (fileInfos[0].contains(".file :"))
    {
      fileInfoStr = fileInfos[0].after(".file :").trim();
    }
  }

  bool hasEntry = false;
  for (unsigned int i = 1; i < fileInfos.size(); i++)
  {
    ossimString temp = fileInfos[i];
    if (temp.contains(".entry:"))
    {
      temp = temp.after(".entry:").trim();
      if (!temp.empty())
      {
        hasEntry = true;
      }
      else
      {
        hasEntry = false;
      }
    }
    else if (temp.contains(".entry :"))
    {
      temp = temp.after(".entry :").trim();
      if (!temp.empty())
      {
        hasEntry = true;
      }
      else
      {
        hasEntry = false;
      }
    }

    if (temp.contains(".rgb:"))
    {
      temp = temp.after(".rgb:").trim();
    }
    else if (temp.contains(".rgb :"))
    {
      temp = temp.after(".rgb :").trim();
    }
    if (temp.contains(".ovr:"))
    {
      temp = temp.after(".ovr:").trim();
    }
    else if (temp.contains(".ovr :"))
    {
      temp = temp.after(".ovr :").trim();
    }
    fileInfoStr = fileInfoStr + "|" + temp;
  }

  if (fileInfos.size() > 0)
  {
    withEncodedEntry = hasEntry;
  }
  else
  {
    withEncodedEntry = true; //will be handled by setFilenameWithDecoding() function
  }

  if (!fileInfoStr.empty())
  {
    ossimFilename file = fileInfoStr;
    ossimOrthoIgenFilename filename;
    if(withEncodedEntry)
    {
      filename.setFilenameWithDecoding(file);
    }
    else
    {
      filename.setFilenameAndEntry(file, -1);
    }

    if (traceDebug())
    {
      ossimNotify(ossimNotifyLevel_DEBUG)
        << "ossimOrthoIgen::addFile DEBUG:"
        << "\nAdded file: " << filename.theFilename
        << std::endl;
    }   

    theFilenames.push_back(filename);
  }
}

void ossimOrthoIgen::setDefaultValues()
{
   theThumbnailRes = "";
   theThumbnailFlag = false;
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

void ossimOrthoIgen::setupIgenKwl()
{
   if (traceDebug())
      ossimNotify(ossimNotifyLevel_DEBUG)<< "ossimOrthoIgen::setupIgenKwl DEBUG: Entered ..."<< std::endl;

   setupTiling();
   theKwl.add("igen.thumbnail", theThumbnailFlag, true);
   if((theThumbnailRes != "")&& theThumbnailFlag)
      theKwl.add("igen.thumbnail_res", theThumbnailRes.c_str(), true);

   // Pass the write to standard out flag to ossimIgen.
   theKwl.add("igen.write_to_stdout", theStdoutFlag, true);

   if(theSlaveBuffers == "")
      theKwl.add("igen.slave_tile_buffers", 2, true);
   else
      theKwl.add("igen.slave_tile_buffers", theSlaveBuffers.c_str(), true);

   ossim_uint32 inputFileIdx = 0;
      
   // Set up the output product's projection:
   setupView();

   if(theFilenames.size()< 1)
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
      if(theProjectionType != OSSIM_UNKNOWN_PROJECTION)
      {
         if(!orthoMosaic)
         {
            ossimImageRenderer* renderer   = new ossimImageRenderer;
            if (renderer->getResampler())
               renderer->getResampler()->setFilterType(theResamplerType);
            default_single_image_chain->addChild(renderer);
         }
      }
   }

   ossim_uint32 fileSize = (ossim_uint32)theFilenames.size()-1;
   ossim_uint32 idx;
   ossimString prefix ("object1.object");

   // Loop over each input image file to establish a single image chain that will be added to the
   // output mosaic:
   ossimImageSource* current_source = 0;
   for(idx = inputFileIdx;idx < fileSize; ++idx)
   {
      // first lets add an input handler to the chain:
      ossimFilename input  = theFilenames[idx].theFilename;
      ossimRefPtr<ossimImageHandler> handler = ossimImageHandlerRegistry::instance()->open(input);
      if(!handler.valid())
      {
         ossimNotify(ossimNotifyLevel_WARN) << "Could not open input file <" << input << ">. "
            << "Skipping this entry." << std::endl;

         // WHAT'S THIS? (OLK 02Apr10)
         //   ossimImageChain* tempChain = (ossimImageChain*)chain->dup();
         //   tempChain->makeUniqueIds();
         //   tempChain->saveState(theKwl, (ossimString("object1.object")+ossimString::toString(chainIdx)+".").c_str());
         //   rootChain->add(tempChain);
         //   ++chainIdx;
         //   mosaicObject->connectMyInputTo(tempChain);

         continue;
      }

      std::vector<ossim_uint32> entryList;
      if(theFilenames[idx].theEntry > -1 )
         entryList.push_back(theFilenames[idx].theEntry);
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

         // Establish the image handler for this particular frame. This may be just a copy of
         // the handler already opened in the case of single image per file:
         ossimImageHandler* img_handler = (ossimImageHandler*)handler->dup();
         if ( theSupplementaryDirectory.empty() == false )
            img_handler->setSupplementaryDirectory( theSupplementaryDirectory );
         else if (theFilenames[idx].theSupplementaryDir.empty() == false)
            img_handler->setSupplementaryDirectory(theFilenames[idx].theSupplementaryDir);
         img_handler->setCurrentEntry(entryList[entryIdx]);
         if ( img_handler->getOverview() == 0 )
         {
            ossimFilename ovrFile = img_handler->getFilenameWithThisExtension(ossimString(".ovr"));
            img_handler->openOverview( ovrFile );
         }
         singleImageChain->addLast(img_handler);
         current_source = img_handler;

         // Install a band selector if needed:
         if (theFilenames[idx].theBands.size() && (img_handler->getNumberOfOutputBands() > 1))
         {
            ossim_uint32 bands = img_handler->getNumberOfOutputBands();
            bool validBand = true;
            for (ossim_uint32 i = 0; i < theFilenames[idx].theBands.size(); ++i)
            {
               if (theFilenames[idx].theBands[i] >= bands)
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
               bs->setOutputBandList(theFilenames[idx].theBands);
               current_source = bs.get();
            }
         }

         // Install a histogram object if needed:
         setupHistogram(singleImageChain, img_handler);

         // Add a cache to the left of the resampler.
         addChainCache(singleImageChain);

         // Add the single image chain to the mosaic and save it to the product spec file:
         singleImageChain->makeUniqueIds();
         mosaicObject->connectMyInputTo(singleImageChain);
         //ossimString newPrefix = (prefix + ossimString::toString(source_index++) + ".");
         //singleImageChain->saveState(theKwl, newPrefix);
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
   // mosaic for reprojectiong to output projection:
   if(orthoMosaic)
   {
      ossimImageRenderer* renderer   = new ossimImageRenderer;
      renderer->getResampler()->setFilterType(theResamplerType);
      theProductChain->addFirst(current_source);
   }

   // Annotation setup...
   setupAnnotation();

   // Output rect cutter:
   setupCutter();

   // After all the connections hae been established, save the state of the complete product chain
   //theProductChain->saveState(theKwl, "object1.");
   theContainer->addChild(theProductChain.get());
   theContainer->saveState(theKwl, "object1.");

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

   ossimGpt originLatLon(theCutOrigin.lat, theCutOrigin.lon, 0.0, theProductProjection->getDatum());

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

   ossimFilename outputFilename = ossimFilename::NIL;
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
      outputFilename = outputFilename.path();
   }

   try
   {
      //---
      // Set the output file name if not already set.
      // NOTE: Could be outputing to stdout in which case outputFilename does not
      // make sense.  Leaving here though to not break code downstream. (drb)
      //---
      if ( outputFilename == ossimFilename::NIL )
      {
         if (theFilenames.size())
            outputFilename = theFilenames[theFilenames.size()-1].theFilename;
         else
            throw(ossimException(std::string("Writer output filename not set.")));
      }

      //---
      // Final check for writer.
      //---
      if ( !writer.valid() )
      {
         // Derive writer from the extension.
         ossimFilename ext = outputFilename.ext();
         if ( ext.size() )
            writer = ossimImageWriterFactoryRegistry::instance()->createWriterFromExtension(ext);

         //---
         // Lastly default to tiff.  Perhaps throw exception here instead. (drb)
         //---
         if( !writer.valid() )
         {
            writer = new ossimTiffWriter;
            outputFilename.setExtension("tif");
         }
      }

      //---
      // Set writer filename, connect and add to writer to keyword list.
      //---
      if ( writer.valid() )
      {
         writer->setFilename(outputFilename);
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
         writer->saveState(theKwl, "object2.");
         //theContainer->addChild(writer.get());
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
void ossimOrthoIgen::setupView()
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimOrthoIgen::setupView DEBUG:"
         << "Entered...."
         << std::endl;
   }

   theProductProjection = 0;

   // The input projection may be referenced, so establish it here corresponding to the first
   // input image in the list:
   ossimRefPtr<ossimProjection> inputProj = 0;
   ossimFilename input  = theFilenames[0].theFilename;
   ossimRefPtr<ossimImageHandler> handler = ossimImageHandlerRegistry::instance()->open(input);
   if(!handler.valid())
   {
      std::string errMsg = "ossimOrthoIgen::setupView() -- Could not open file: ";
      errMsg += input;
      throw(ossimException(errMsg));
   }

   // Consider distributed image data directories (I guess this is sometimes needed for establishing
   // an image geometry???)
   if ( theSupplementaryDirectory.empty() == false )
      handler->setSupplementaryDirectory( theSupplementaryDirectory );
   else if (theFilenames[0].theSupplementaryDir.empty() == false )
      handler->setSupplementaryDirectory( theFilenames[0].theSupplementaryDir );
   if(theFilenames[0].theEntry > -1)
      handler->setCurrentEntry(theFilenames[0].theEntry);
   else
   {
      std::vector<ossim_uint32> entryList;
      handler->getEntryList(entryList);
      if ( entryList.size() > 0 )
         handler->setCurrentEntry(entryList[0]);
   }

   // Fetch this first input image's geometry and corresponding projection:
   ossimRefPtr<ossimImageGeometry> inputGeom = handler->getImageGeometry();
   if (inputGeom.valid())
      inputProj = inputGeom->getProjection();

   // Throw exception if no valid input image projection could be established:
   if(!inputProj.valid())
   {
      std::string errMsg = "ossimOrthoIgen::setupView() -- Could not establish input image's "
         "projection. Cannot setup output view.";
      throw(ossimException(errMsg));
   }

   // The input projection was successfully established. Define some quantities that may be needed
   // for the output projection:
   ossimDpt metersPerPixel = inputProj->getMetersPerPixel();
   double meanGSD = (metersPerPixel.x+metersPerPixel.y)*.5;
   ossimDpt gsd(meanGSD, meanGSD);
   ossimUnitType gsdUnits = OSSIM_METERS;
   if(!theDeltaPerPixelOverride.hasNans())
   {
      gsd = theDeltaPerPixelOverride;
      gsdUnits = theDeltaPerPixelUnit;
   }
   ossimIrect rect = handler->getBoundingRect();

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

   // SRS code specified on the command line
   else if (!theSrsString.empty())
   {
      // Inconsistent specification of PCS, GCS, SRS codes. Need to check with full name then just code:
      ossimProjection* base_proj = 
         ossimProjectionFactoryRegistry::instance()->createProjection(theSrsString);
      if (!base_proj && (theSrsString.contains(":")))
      {
         ossimString srs_code = theSrsString.split(":")[1].trim();
         base_proj = ossimProjectionFactoryRegistry::instance()->createProjection(srs_code);
      }

      theProductProjection = PTR_CAST(ossimMapProjection, base_proj);
      if(theProductProjection.valid())
      {
         // Was not assigned before. Now we know
         if (theProductProjection->isGeographic())
         {
            theProjectionType = OSSIM_GEO_PROJECTION;
            ossimGpt gpt(theGeographicOriginOfLatitude, 0.0);
            theProductProjection->setOrigin(gpt);
         }
         else
            theProjectionType = OSSIM_UTM_PROJECTION;
      }
      else
      {
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
      ossimGpt midGpt;

      inputProj->lineSampleToWorld(ossimDpt(rect.midPoint()), midGpt);

      utm->setZone(midGpt);
      utm->setHemisphere(midGpt);
      int pcsCode = ossimPcsCodeProjectionFactory::instance()->getPcsCodeFromProjection(utm);
      utm->setPcsCode(pcsCode);
      theProductProjection = utm;
   }

   // None of the above?
   else
   {
      // Either OSSIM_INPUT_PROJECTION or OSSIM_UNKNOWN_PROJECTION. In both cases
      // just use the first image's input projection for the output. Need to make 
      // sure the input_proj is a map projection though:
      ossimMapProjection* map_proj = PTR_CAST(ossimMapProjection, inputProj.get());
      if (!map_proj)
      {
         std::string errMsg = "ossimOrthoIgen::setupView() -- First input image's projection must "
            "be a map projection when specifying \"input-proj\".";
         throw(ossimException(errMsg));
      }
      theProductProjection = PTR_CAST(ossimMapProjection, map_proj->dup());
      theProjectionType = OSSIM_INPUT_PROJECTION; // just in case it was unknown before
   }

   // At this point there should be a valid output projection defined:
   if (!theProductProjection.valid())
   {
      std::string errMsg = "ossimOrthoIgen::setupView() -- Could not establish valid output "
         "projection";
      throw(ossimException(errMsg));
   }

   //Perform some tasks common to all projection types:
   if (gsdUnits == OSSIM_DEGREES)
      theProductProjection->setDecimalDegreesPerPixel(gsd);
   else
      theProductProjection->setMetersPerPixel(gsd);
   theProjectionName = theProductProjection->getProjectionName();

   // Fix the tiepoint misalignment between input and output image projections:
   consolidateCutRectSpec();
   snapTiePointToInputProj(PTR_CAST(ossimMapProjection, inputProj.get()));

   // Finally, save this output projection spec to the Igen KWL:
   theProductProjection->saveState(theKwl, "product.projection.");

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimOrthoIgen::setupView DEBUG:"
         << "Leaving...." << __LINE__
         << std::endl;
   }
}

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

bool ossimOrthoIgen::setupTiling()
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimOrthoIgen::setupTiling: Entered......" << std::endl;
   }
   ossimKeywordlist templateKwl;
   ossimFilename outputFilename = theFilenames[theFilenames.size()-1].theFilename;

   if((theTilingTemplate == "")||
      (!templateKwl.addFile(theTilingTemplate)))
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimOrthoIgen::setupTiling: Leaving......" << __LINE__ << std::endl;
         
      }
      return false;
   }

   if(outputFilename.isDir())
   {
      if(templateKwl.find("igen.tiling.type"))
      {
         theKwl.add(templateKwl);
         theTilingFilename = templateKwl.find("igen.tiling.tile_name_mask");
      }
      else if(templateKwl.find("type"))
      {
         theKwl.add("igen.tiling.", templateKwl);
         theTilingFilename = templateKwl.find("tile_name_mask");
      }
   }
   else
   {
      bool tilingEnabled = false;
      theTilingFilename = outputFilename.file();
      if(templateKwl.find("igen.tiling.type"))
      {
         templateKwl.add("igen.tiling.tile_name_mask",
                     theTilingFilename.c_str(),
                     true);
         theKwl.add(templateKwl);
         tilingEnabled = true;
      }
      else if(templateKwl.find("type"))
      {
         templateKwl.add("tile_name_mask",
                     theTilingFilename.c_str(),
                     true);
         theKwl.add("igen.tiling.", templateKwl);
         tilingEnabled = true;
      }
      if(tilingEnabled) // check for tile mask override
      {
         theFilenames[theFilenames.size()-1].theFilename = theFilenames[theFilenames.size()-1].theFilename.path();
      }
   }

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
         // POTENTIAL BUG: converion from degrees longitude to meters should be a function 
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
void ossimOrthoIgen::snapTiePointToInputProj(ossimMapProjection* in_proj)
{
   if (!in_proj || !theProductProjection.valid())
      return;

   // Make sure the AOI is specified in terms of UL corner and distance towards LR:
   if (!theCutRectSpecIsConsolidated)
      consolidateCutRectSpec();

   ossimDpt uli (0,0); // upper left corner of input image in pixels

   // When no cutting is requested, then it's the trivial case of simply copying the input
   // projection's tiepoint to the output:
   if (theCutOrigin.hasNans())
   {
      ossimGpt ulg; // input image UL geographic origin.
      in_proj->lineSampleToWorld(uli, ulg);
      theProductProjection->setUlTiePoints(ulg);
      return;
   }

   // We will be modifying the progection's tiepoint (a.k.a. UL corner point):
   ossimGpt originLatLon(theCutOrigin.lat, theCutOrigin.lon, 0.0, theProductProjection->getDatum());
   if(in_proj->isGeographic())  // geographic projection, units = decimal degrees.
   {
      // Need to use the degrees-per-pixel quantity for accurate snap:
      ossimGpt ulg; // input image UL geographic origin.
      ossimDpt degPerPixel (in_proj->getDecimalDegreesPerPixel());
      in_proj->lineSampleToWorld(uli, ulg);

      // Establish offset from cut-rect UL to input image UL in integral pixels, and
      // correct the cut-rect's origin for the misalignment:
      double dLat = originLatLon.lat - ulg.lat;
      dLat = ossim::round<int>(dLat/degPerPixel.y) * degPerPixel.y;
      originLatLon.lat = ulg.lat + dLat - 0.5*degPerPixel.y;

      double dLon = originLatLon.lon - ulg.lon;
      dLon = ossim::round<int>(dLon/degPerPixel.x) * degPerPixel.x;
      originLatLon.lon = ulg.lon + dLon - 0.5*degPerPixel.x;

      theProductProjection->setUlTiePoints(originLatLon);
   }

   else    // projection in meters...
   {
      ossimDpt originEN = in_proj->forward(originLatLon);

      // Directly use the meters offset from input image UL in computing snap shift:
      ossimDpt ulEN; // input image UL E/N origin.
      ossimDpt mtrsPerPixel (theProductProjection->getMetersPerPixel());
      in_proj->lineSampleToEastingNorthing(uli, ulEN);

      // Establish offset from cut-rect UL to input image UL in integral pixels, and
      // correct the cut-rect's origin for the misalignment:
      double dE = originEN.x - ulEN.x;
      dE = ossim::round<int>(dE/mtrsPerPixel.x) * mtrsPerPixel.x;
      originEN.x = ulEN.x + dE; // - 0.5*mtrsPerPixel.x;

      double dN = originEN.y - ulEN.y;
      dN = ossim::round<int>(dN/mtrsPerPixel.y) * mtrsPerPixel.y;
      originEN.y = ulEN.y + dN; // - 0.5*mtrsPerPixel.y;

      theProductProjection->setUlTiePoints(originEN);
   }
}

//*************************************************************************************************
//! Sets up the histogram operation requested for the image chain passed in.
//*************************************************************************************************
void ossimOrthoIgen::setupHistogram(ossimImageChain* input_chain,  ossimImageHandler* handler)
{
   // Check if any histo operation was requested:
   if(((ossim::isnan(theHighPercentClip) || ossim::isnan(theLowPercentClip)) &&
      !theUseAutoMinMaxFlag && (theStdDevClip < 0) && theTargetHistoFileName.empty()))
   {
      return; // no histo op requested
   }

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

   // Remaining operations require a histogram on the input image source:
   ossimFilename inputHistoFilename (handler->createDefaultHistogramFilename());
   if (!inputHistoFilename.exists())
   {
      // Try "foo_e0.his" if image is "foo.tif" where "e0" is entry index.
      inputHistoFilename = handler->getFilenameWithThisExtension(ossimString(".his"), true);
      if ( !inputHistoFilename.exists() )
      {
         // No histogram availablefor this image, need to create one (TODO):
         ossimNotify(ossimNotifyLevel_WARN)
            <<"Histogram file <" << inputHistoFilename 
            << "> not found. No histogram operations will be performed on this image." 
            << std::endl;
         return;
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
      forwardEq->setHistogram(inputHistoFilename);
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
   ossimHistogramRemapper* remapper = new ossimHistogramRemapper;
   if (renderer)
      input_chain->insertLeft(remapper, renderer);
   else
      input_chain->addFirst(remapper);

   // Fetch the input histogram:
   bool histo_read_ok = remapper->openHistogram(inputHistoFilename);
   if (!histo_read_ok)
   {
      // No histogram available for this image, need to create one (TODO):
      ossimNotify(ossimNotifyLevel_WARN)<<"Error encountered loading histogram file <" 
         << inputHistoFilename << ">. No histogram operations will be performed on this image." 
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
   else if (theUseAutoMinMaxFlag)
   {
      remapper->setStretchMode(ossimHistogramRemapper::LINEAR_AUTO_MIN_MAX, true);
   }
   else if (theStdDevClip > 0)
   {
      remapper->setStretchMode((ossimHistogramRemapper::StretchMode) theStdDevClip, true);
   }
   
   return;
}

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

#if 0
         //---
         // See if the underlying image is tiled.  If so set the cache tile size to that.
         //
         // NOTE: This actually slowed things down; hence, commented out (drb).
         //---
         ossimImageHandler* ih =
            PTR_CAST(ossimImageHandler,
                     chain->findFirstObjectOfType(ossimString("ossimImageHandler")));
         if ( ih )
         {
            if ( ih->isImageTiled() )
            {
               ossimIpt size(static_cast<ossim_int32>( ih->getImageTileWidth() ),
                             static_cast<ossim_int32>( ih->getImageTileHeight() ) );
               cache->setTileSize(size);
            }
         }
#endif
         
         chain->insertLeft(cache, renderer);
      }
   }
}
