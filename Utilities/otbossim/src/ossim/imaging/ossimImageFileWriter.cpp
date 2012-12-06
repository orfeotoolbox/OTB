//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Frank Warmerdam (warmerdam@pobox.com)
//
// Description:
//
// Contains class declaration for ossimImageFileWriter.
//*******************************************************************
//  $Id: ossimImageFileWriter.cpp 21963 2012-12-04 16:28:12Z dburken $


#include <tiff.h> /* for tiff compression defines */

#include <ossim/ossimConfig.h>
#include <ossim/imaging/ossimImageFileWriter.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimMultiResLevelHistogram.h>
#include <ossim/base/ossimMultiBandHistogram.h>
#include <ossim/base/ossimHistogram.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimErrorContext.h>
#include <ossim/base/ossimImageTypeLut.h>
#include <ossim/base/ossimIoStream.h>
#include <ossim/base/ossimUnitTypeLut.h>
#include <ossim/imaging/ossimTiffOverviewBuilder.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimHistogramWriter.h>
#include <ossim/imaging/ossimImageHistogramSource.h>
#include <ossim/imaging/ossimMetadataFileWriter.h>
#include <ossim/imaging/ossimGeomFileWriter.h>
#include <ossim/imaging/ossimEnviHeaderFileWriter.h>
#include <ossim/imaging/ossimERSFileWriter.h>
#include <ossim/imaging/ossimFgdcFileWriter.h>
#include <ossim/imaging/ossimReadmeFileWriter.h>
#include <ossim/imaging/ossimScalarRemapper.h>
#include <ossim/imaging/ossimWorldFileWriter.h>
#include <ossim/base/ossimStdOutProgress.h>
#include <ossim/base/ossimFilenameProperty.h>
#include <ossim/base/ossimBooleanProperty.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimMapProjectionInfo.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>

static ossimTrace traceDebug("ossimImageFileWriter:debug");
static const ossimString AUTO_CREATE_DIRECTORY_KW("auto_create_directory");

#if OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimImageFileWriter.cpp 21963 2012-12-04 16:28:12Z dburken $";
#endif

RTTI_DEF3(ossimImageFileWriter,
          "ossimImageFileWriter",
          ossimImageWriter,
          ossimProcessInterface,
          ossimConnectableObjectListener)

static const char SCALE_TO_EIGHT_BIT_KW[] = "scale_to_eight_bit";

ossimImageFileWriter::ossimImageFileWriter(const ossimFilename& file,
                                           ossimImageSource* inputSource,
                                           ossimObject* owner)
   : ossimImageWriter(owner,
                      1,
                      0,
                      true,
                      false),
     ossimProcessInterface(),
     ossimConnectableObjectListener(),
     theViewController(NULL),
     theProgressListener(NULL),
     theFilename(file),
     theOutputImageType(ossimImageTypeLut().getEntryString(OSSIM_IMAGE_TYPE_UNKNOWN)),
     theWriteImageFlag(true),
     theWriteOverviewFlag(false),
     theWriteHistogramFlag(false),
     theScaleToEightBitFlag(false),
     theOverviewCompressType(COMPRESSION_NONE),
     theOverviewJpegCompressQuality(75),
     theWriteEnviHeaderFlag(false),
     theWriteErsHeaderFlag(false),     
     theWriteExternalGeometryFlag(false),
     theWriteFgdcFlag(false),
     theWriteJpegWorldFileFlag(false),
     theWriteReadmeFlag(false),
     theWriteTiffWorldFileFlag(false),
     theWriteWorldFileFlag(false),
     theAutoCreateDirectoryFlag(true),
     theLinearUnits(OSSIM_UNIT_UNKNOWN),
     thePixelType(OSSIM_PIXEL_IS_POINT)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimImageFileWriter::ossimImageFileWriter entered..."
         << std::endl;
#ifdef OSSIM_ID_ENABLED
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "OSSIM_ID:  " << OSSIM_ID << std::endl;
#endif
   }

   theInputConnection = new ossimImageSourceSequencer(inputSource);

   // now map the sequencer to the same input
   connectMyInputTo(0, inputSource);
   initialize();

   // make sure we listen to our input changes so we can change
   // it for the sequencer as well.
   ossimImageWriter::addListener((ossimConnectableObjectListener*)this);

   theInputConnection->connectMyInputTo(0, inputSource, false);
   theAreaOfInterest.makeNan();
}

ossimImageFileWriter::~ossimImageFileWriter()
{
   theInputConnection = 0;
   theProgressListener = NULL;
   removeListener((ossimConnectableObjectListener*)this);
}

void ossimImageFileWriter::initialize()
{
   if(theInputConnection.valid())
   {
      theInputConnection->initialize();

      //---
      // Area Of Interest(AOI):
      // There are two AOIs to keep in sync.
      // One owned by this base: ossimImageWriter::theAreaOfInterest
      // One owned by the theInputConntion: ossimImageSourceSequencer::theAreaOfInterest
      //---
      if ( theAreaOfInterest.hasNans() == false )
      {
         //---
         // ossimImageSourceSequencer::setAreaOfInterest does more than just set the
         // variable, so only call if out of sync to avoid wasted cycles.
         //--- 
         if ( theAreaOfInterest != theInputConnection->getBoundingRect() )
         {
            theInputConnection->setAreaOfInterest( theAreaOfInterest );
         }
      }
      else
      {
         // Set this AOI to bounding rect of input.
         theAreaOfInterest = theInputConnection->getBoundingRect();
      }
   }
}

void ossimImageFileWriter::changeSequencer(ossimImageSourceSequencer* sequencer)
{
   if( sequencer )
   {
      theInputConnection = sequencer;
      theInputConnection->connectMyInputTo(0, getInput(0));
   }
}

bool ossimImageFileWriter::saveState(ossimKeywordlist& kwl,
                                 const char* prefix)const
{
   kwl.add(prefix,
           ossimKeywordNames::TYPE_KW,
           TYPE_NAME(this),
           true);

   kwl.add(prefix,
           ossimKeywordNames::FILENAME_KW,
           theFilename.c_str(),
           true);

   kwl.add(prefix,
           "create_envi_hdr",
           (ossim_uint32)theWriteEnviHeaderFlag,
           true);

   kwl.add(prefix,
           "create_ers_hdr",
           (ossim_uint32)theWriteErsHeaderFlag,
           true);

   kwl.add(prefix,
           ossimKeywordNames::CREATE_EXTERNAL_GEOMETRY_KW,
           (ossim_uint32)theWriteExternalGeometryFlag,
           true);

   kwl.add(prefix,
           "create_fgdc",
           (ossim_uint32)theWriteFgdcFlag,
           true);
   
   kwl.add(prefix,
           "create_jpeg_world_file",
           (ossim_uint32)theWriteJpegWorldFileFlag,
           true);

   kwl.add(prefix,
           "create_readme",
           (ossim_uint32)theWriteReadmeFlag,
           true);
   
   kwl.add(prefix,
           "create_tiff_world_file",
           (ossim_uint32)theWriteTiffWorldFileFlag,
           true);

   kwl.add(prefix,
           "create_world_file",
           (ossim_uint32)theWriteWorldFileFlag,
           true);

   kwl.add(prefix,
           ossimKeywordNames::CREATE_IMAGE_KW,
           (ossim_uint32)theWriteImageFlag,
           true);

   kwl.add(prefix,
           ossimKeywordNames::CREATE_OVERVIEW_KW,
           (ossim_uint32)theWriteOverviewFlag,
           true);

   kwl.add(prefix,
           ossimKeywordNames::CREATE_HISTOGRAM_KW,
           (ossim_uint32)theWriteHistogramFlag,
           true);

   kwl.add(prefix,
           SCALE_TO_EIGHT_BIT_KW,
           (ossim_uint32)theScaleToEightBitFlag,
           true);
   kwl.add(prefix,
           AUTO_CREATE_DIRECTORY_KW,
           theAutoCreateDirectoryFlag,
           true);
   kwl.add(prefix,
           ossimKeywordNames::OVERVIEW_COMPRESSION_TYPE_KW,
           theOverviewCompressType,
           true);

   kwl.add(prefix,
           ossimKeywordNames::OVERVIEW_COMPRESSION_QUALITY_KW,
           theOverviewJpegCompressQuality,
           true);

   ossimImageTypeLut lut;
   kwl.add(prefix,
           ossimKeywordNames::IMAGE_TYPE_KW,
           theOutputImageType,
           true);

   //---
   // When "theLinearUnits" == "unknown" that is a key to the writer to just
   // use whatever units makes sense for the projection.  Since "unknown"
   // doesn't look good to the average user in a spec file, change to
   // "default" if "unknown".  When read back in by the lookup table since
   // "default" is not a valid "string" theLinearUnits will remain "unknown".
   // Kind of a hack but it works:)  (drb)
   //---
   ossimString os = ossimUnitTypeLut::instance()->
      getEntryString(theLinearUnits);
   if (os == "unknown")
   {
      os = "default";  // Avoid saving as unknown for user readability.
   }
   kwl.add(prefix, "linear_units", os, true);

   // Set the pixel type.
   ossimString pixelType;
   getPixelTypeString(pixelType);
   kwl.add(prefix,
           ossimKeywordNames::PIXEL_TYPE_KW,
           pixelType.c_str(),
           true);

   return ossimSource::saveState(kwl, prefix);
}

bool ossimImageFileWriter::loadState(const ossimKeywordlist& kwl,
                                     const char* prefix)
{
   // Load the state of the base class.
   bool result = ossimSource::loadState(kwl, prefix);

   // Get the filename...
   const char* lookup = kwl.find(prefix, ossimKeywordNames::FILENAME_KW);
   if(lookup)
   {
      ossimString filename = lookup;

      lookup =  kwl.find(prefix, ossimKeywordNames::FILEPATH_KW);
      if(lookup)
      {
         filename = ossimString(lookup) + filename;
      }

      setFilename(filename);
   }

   lookup = kwl.find(prefix, ossimKeywordNames::PIXEL_TYPE_KW);
   if(lookup)
   {
      if(ossimString(lookup).downcase().contains("area"))
      {
         thePixelType = OSSIM_PIXEL_IS_AREA;
      }
      else
      {
         thePixelType = OSSIM_PIXEL_IS_POINT;
      }
   }

   lookup = kwl.find(prefix, "linear_units");
   if(lookup)
   {
      ossimUnitType units = (ossimUnitType)ossimUnitTypeLut::instance()->
         getEntryNumber(lookup, true);
      if ( (units == OSSIM_METERS) ||
           (units == OSSIM_FEET)   ||
           (units == OSSIM_US_SURVEY_FEET) )
      {
         theLinearUnits = units;
      }
   }

   lookup = kwl.find(prefix, ossimKeywordNames::CREATE_EXTERNAL_GEOMETRY_KW);
   if(lookup)
   {
      ossimString s = lookup;
      theWriteExternalGeometryFlag = s.toBool();
   }
   
   lookup = kwl.find(prefix, "create_envi_hdr");
   if(lookup)
   {
      ossimString s = lookup;
      theWriteEnviHeaderFlag = s.toBool();
   }
   
   lookup = kwl.find(prefix, "create_ers_hdr");
   if(lookup)
   {
      ossimString s = lookup;
      theWriteErsHeaderFlag = s.toBool();
   }

   lookup = kwl.find(prefix, "create_fgdc");
   if(lookup)
   {
      ossimString s = lookup;
      theWriteFgdcFlag = s.toBool();
   }
   
   lookup = kwl.find(prefix, "create_jpeg_world_file");
   if(lookup)
   {
      ossimString s = lookup;
      theWriteJpegWorldFileFlag = s.toBool();
   }
   
   lookup = kwl.find(prefix, "create_readme");
   if(lookup)
   {
      ossimString s = lookup;
      theWriteReadmeFlag = s.toBool();
   }
   
   lookup = kwl.find(prefix, "create_tiff_world_file");
   if(lookup)
   {
      ossimString s = lookup;
      theWriteTiffWorldFileFlag = s.toBool();
   }

   lookup = kwl.find(prefix, "create_world_file");
   if(lookup)
   {
      ossimString s = lookup;
      theWriteWorldFileFlag = s.toBool();
   }

   lookup = kwl.find(prefix, ossimKeywordNames::CREATE_IMAGE_KW);
   if(lookup)
   {
      ossimString s = lookup;
      theWriteImageFlag = s.toBool();
   }

   lookup = kwl.find(prefix, ossimKeywordNames::CREATE_OVERVIEW_KW);
   if(lookup)
   {
      ossimString s = lookup;
      theWriteOverviewFlag = s.toBool();
   }

   lookup = kwl.find(prefix, ossimKeywordNames::CREATE_HISTOGRAM_KW);
   if(lookup)
   {
      ossimString s = lookup;
      theWriteHistogramFlag = s.toBool();
   }

   lookup = kwl.find(prefix, SCALE_TO_EIGHT_BIT_KW);
   if(lookup)
   {
      ossimString s = lookup;
      theScaleToEightBitFlag = s.toBool();
   }

   lookup = kwl.find(prefix, AUTO_CREATE_DIRECTORY_KW);
   if(lookup)
   {
      theAutoCreateDirectoryFlag = ossimString(lookup).toBool();
   }
   lookup = kwl.find(prefix, ossimKeywordNames::OVERVIEW_COMPRESSION_TYPE_KW);
   if(lookup)
   {
      ossimString s = lookup;
      theOverviewCompressType = static_cast<ossim_uint16>(s.toUInt32());
   }

   lookup = kwl.find(prefix, ossimKeywordNames::OVERVIEW_COMPRESSION_QUALITY_KW);
   if(lookup)
   {
      ossimString s = lookup;
      theOverviewJpegCompressQuality = s.toInt32();
   }

   const char* outputImageType = kwl.find(prefix, ossimKeywordNames::IMAGE_TYPE_KW);
   if(outputImageType)
   {
      theOutputImageType = outputImageType;
   }

   if(theInputObjectList.size() != 2)
   {
      theInputObjectList.resize(2);
      theInputListIsFixedFlag = true;
   }

   theOutputListIsFixedFlag = false;

   return result;
}

bool ossimImageFileWriter::writeOverviewFile(ossim_uint16 tiff_compress_type,
                                             ossim_int32 jpeg_compress_quality, 
                                             bool includeR0) 
{
   if(theFilename == "") return false;

   // Get an image handler for the source file.
   ossimRefPtr<ossimImageHandler> ih
      = ossimImageHandlerRegistry::instance()->open(theFilename);

   if (ih.valid() == false)
   {
      // Set the error...
      return false;
   }

   ossimRefPtr<ossimTiffOverviewBuilder> ob = new ossimTiffOverviewBuilder();
   
   if ( ob->setInputSource(ih.get()) )
   {
      // Give the listener to the overview builder if set.
      if (theProgressListener)
      {
         ob->addListener(theProgressListener);
      }
      
      // Build a new file name for the overview.
      ossimFilename overview_file = theFilename;
      overview_file.setExtension(ossimString("ovr"));
      ob->setCompressionType(tiff_compress_type);
      ob->setJpegCompressionQuality(jpeg_compress_quality);
      ob->setOutputFile(overview_file);
      ob->setCopyAllFlag(includeR0);
      ob->execute();

      // Remove the listener from the overview builder.
      if (theProgressListener)
      {
         ob->removeListener(theProgressListener);
      }
   }

   return true;
}

bool ossimImageFileWriter::writeEnviHeaderFile() 
{
   if( (theFilename == "") || !theInputConnection ||
       theAreaOfInterest.hasNans())
   {
      return false;
   }

   // Make the file name.
   ossimFilename outputFile = theFilename;
   outputFile.setExtension(ossimString("hdr"));

   // Make the writer.
   ossimRefPtr<ossimMetadataFileWriter> writer =
      new ossimEnviHeaderFileWriter();
   
   // Set things up.
   writer->connectMyInputTo(0, theInputConnection.get());
   writer->initialize();
   writer->setFilename(outputFile);
   writer->setPixelType(thePixelType);
   writer->setAreaOfInterest(theAreaOfInterest);
   
   // Write it to disk.
   return writer->execute();
}

bool ossimImageFileWriter::writeErsHeaderFile() 
{
   if( (theFilename == "") || !theInputConnection ||
       theAreaOfInterest.hasNans())
   {
      return false;
   }
   
   // Make the file name.
   ossimFilename outputFile = theFilename;
   outputFile.setExtension(ossimString("ers"));

   // Make the writer.
   ossimRefPtr<ossimMetadataFileWriter> writer =
	   new ossimERSFileWriter();
   
   // Set things up.
   writer->connectMyInputTo(0, theInputConnection.get());
   writer->initialize();
   writer->setFilename(outputFile);
   writer->setPixelType(thePixelType);
   writer->setAreaOfInterest(theAreaOfInterest);
   
   // Write it to disk.
   return writer->execute();
}

bool ossimImageFileWriter::writeExternalGeometryFile() 
{
   if( (theFilename == "") || !theInputConnection ||
       theAreaOfInterest.hasNans())
   {
      return false;
   }

   // Make the file name.
   ossimFilename geomFile = theFilename;
   geomFile.setExtension(ossimString("geom"));

   // Make the writer.
   ossimRefPtr<ossimMetadataFileWriter> writer = new ossimGeomFileWriter();

   // Set things up.
//   writer->connectMyInputTo(0, theInputConnection.get());
   writer->connectMyInputTo(0, theInputConnection.get());
   writer->setFilename(geomFile);
   writer->initialize();
   writer->setPixelType(thePixelType);
   writer->setAreaOfInterest(theAreaOfInterest);

   // Write it to disk.
   return writer->execute();
}

bool ossimImageFileWriter::writeFgdcFile() 
{
   if( (theFilename == "") || !theInputConnection ||
       theAreaOfInterest.hasNans())
   {
      return false;
   }

   // Make the file name.
   ossimFilename outputFile = theFilename;
   outputFile.setExtension(ossimString("xml"));

   // Make the writer.
   ossimRefPtr<ossimMetadataFileWriter> writer = new ossimFgdcFileWriter();
   
   // Set things up.
   writer->connectMyInputTo(0, theInputConnection.get());
   writer->initialize();
   writer->setFilename(outputFile);
   writer->setPixelType(thePixelType);
   writer->setAreaOfInterest(theAreaOfInterest);
   
   // Write it to disk.
   return writer->execute();
}

bool ossimImageFileWriter::writeJpegWorldFile() 
{
   if(theFilename == "")
   {
      return false;
   }
   
   // Make the file name.
   ossimFilename file = theFilename;
   file.setExtension(ossimString("jpw")); // jpw???

   return writeWorldFile(file);
}

bool ossimImageFileWriter::writeReadmeFile() 
{
   if( (theFilename == "") || !theInputConnection ||
       theAreaOfInterest.hasNans())
   {
      return false;
   }

   // Make the file name.
   ossimFilename outputFile = theFilename.noExtension();
   
   outputFile += "_readme.txt";

   // Make the writer.
   ossimRefPtr<ossimMetadataFileWriter> writer =
      new ossimReadmeFileWriter();
   
   // Set things up.
   writer->connectMyInputTo(0, theInputConnection.get());
   writer->initialize();
   writer->setFilename(outputFile);
   writer->setPixelType(thePixelType);
   writer->setAreaOfInterest(theAreaOfInterest);
   
   // Write it to disk.
   return writer->execute();
}

bool ossimImageFileWriter::writeTiffWorldFile() 
{
   if(theFilename == "")
   {
      return false;
   }
   
   // Make the file name.
   ossimFilename file = theFilename;
   file.setExtension(ossimString("tfw"));

   return writeWorldFile(file);
}

bool ossimImageFileWriter::writeMetaDataFiles() 
{
   bool status = true;
   
   if(theWriteEnviHeaderFlag&&!needsAborting())
   {
      if(writeEnviHeaderFile() == false)
      {
         status = false;

         ossimNotify(ossimNotifyLevel_WARN)
            << "Write of envi header file failed!" << endl;
      }
   }

   if(theWriteErsHeaderFlag&&!needsAborting())
   {
      if(writeErsHeaderFile() == false)
      {
         status = false;

         ossimNotify(ossimNotifyLevel_WARN)
            << "Write of ers header file failed!" << endl;
      }
   }

   if (theWriteExternalGeometryFlag&&!needsAborting())
   {
      if( writeExternalGeometryFile() == false)
      {
         status = false;
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
            << "Write of geometry file failed!" << endl;
         }
      }
   }
   if(theWriteFgdcFlag&&!needsAborting())
   {
      if(writeFgdcFile() == false)
      {
         status = false;
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
            << "Write of fgdc file failed!" << endl;
         }
      }
   }
   if(theWriteJpegWorldFileFlag&&!needsAborting())
   {
      if(writeJpegWorldFile() == false)
      {
         status = false;
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
            << "Write of jpeg world file failed!" << endl;
         }
      }
   }
   if(theWriteReadmeFlag&&!needsAborting())
   {
      if(writeReadmeFile() == false)
      {
         status = false;
         
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
            << "Write of readme file failed!" << endl;
         }
      }
   }
   if(theWriteTiffWorldFileFlag&&!needsAborting())
   {
      if(writeTiffWorldFile() == false)
      {
         status = false;
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
            << "Write of tiff world file failed!" << endl;
         }
      }
   }

   if(theWriteWorldFileFlag&&!needsAborting())
   {
      if(writeWorldFile() == false)
      {
         status = false;
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
            << "Write of world file failed!" << endl;
         }
      }
   }
   
   if (theWriteHistogramFlag&&!needsAborting())
   {
      if (!writeHistogramFile())
      {
         status = false;
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
            << "Write of histogram file failed!" << endl;
         }
      }
   }

   return status;
}

bool ossimImageFileWriter::writeHistogramFile() 
{
   if(needsAborting()) return false;
   if(theFilename == "") return false;

   ossimFilename histogram_file = theFilename;
   histogram_file.setExtension(ossimString("his"));

   ossimRefPtr<ossimImageHandler> handler = ossimImageHandlerRegistry::instance()->
      open(theFilename);

   if (!handler)
   {
      return false;
   }

   ossimRefPtr<ossimImageHistogramSource> histoSource = new ossimImageHistogramSource;

   ossimRefPtr<ossimHistogramWriter> writer = new ossimHistogramWriter;
   histoSource->setMaxNumberOfRLevels(1); // Only compute for r0.
   histoSource->connectMyInputTo(0, handler.get());
   histoSource->enableSource();
   writer->connectMyInputTo(0, histoSource.get());
   writer->setFilename(histogram_file);
   writer->addListener(&theStdOutProgress);
   writer->execute();
   writer->disconnect();
   histoSource->disconnect();
   handler->disconnect();
   writer = 0; histoSource = 0; handler = 0;
   return true;
}

void ossimImageFileWriter::disconnectInputEvent(ossimConnectionEvent& /* event */)
{
   if(!getInput(0))
   {
      theInputConnection->disconnectMyInput((ossim_int32)0, false);
   }
   theViewController = PTR_CAST(ossimViewController, getInput(1));

   initialize();
}

void ossimImageFileWriter::connectInputEvent(ossimConnectionEvent& /* event */)
{
   theInputConnection->connectMyInputTo((ossim_int32)0,
                                        getInput(0),
                                        false);
   theViewController = PTR_CAST(ossimViewController, getInput(1));
   initialize();
}

void ossimImageFileWriter::propertyEvent(ossimPropertyEvent& /* event */)
{
   initialize();
}

void ossimImageFileWriter::setOutputImageType(const ossimString& type)
{
   theOutputImageType = type;
}

void ossimImageFileWriter::setOutputImageType(ossim_int32 type)
{
   ossimImageTypeLut lut;
   theOutputImageType = lut.getEntryString(type);
}

ossimString ossimImageFileWriter::getOutputImageTypeString() const
{
   return theOutputImageType;

}

ossim_int32 ossimImageFileWriter::getOutputImageType() const
{
   ossimImageTypeLut lut;
   return lut.getEntryNumber(theOutputImageType);
}


bool ossimImageFileWriter::getWriteImageFlag() const
{
   return theWriteImageFlag;
}

bool ossimImageFileWriter::getWriteHistogramFlag() const
{
   return theWriteHistogramFlag;
}

bool ossimImageFileWriter::getWriteOverviewFlag() const
{
   return theWriteOverviewFlag;
}

bool ossimImageFileWriter::getScaleToEightBitFlag() const
{
   return theScaleToEightBitFlag;
}

bool ossimImageFileWriter::getWriteEnviHeaderFlag() const
{
   return theWriteEnviHeaderFlag;
}

bool ossimImageFileWriter::getWriteErsHeaderFlag() const
{
   return theWriteErsHeaderFlag;
}

bool ossimImageFileWriter::getWriteExternalGeometryFlag() const
{
   return theWriteExternalGeometryFlag;
}

bool ossimImageFileWriter::getWriteFgdcFlag() const
{
   return theWriteFgdcFlag;
}

bool ossimImageFileWriter::getWriteJpegWorldFileFlag() const
{
   return theWriteJpegWorldFileFlag;
}

bool ossimImageFileWriter::getWriteReadmeFlag() const
{
   return theWriteReadmeFlag;
}

bool ossimImageFileWriter::getWriteTiffWorldFileFlag() const
{
   return theWriteTiffWorldFileFlag;
}

bool ossimImageFileWriter::getWriteWorldFileFlag() const
{
   return theWriteWorldFileFlag;
}

void ossimImageFileWriter::setWriteOverviewFlag(bool flag)
{
   theWriteOverviewFlag = flag;
}

void ossimImageFileWriter::setWriteImageFlag(bool flag)
{
   theWriteImageFlag = flag;
}

void ossimImageFileWriter::setWriteHistogramFlag(bool flag)
{
   theWriteHistogramFlag = flag;
}

void ossimImageFileWriter::setScaleToEightBitFlag(bool flag)
{
   theScaleToEightBitFlag = flag;
}

void ossimImageFileWriter::setWriteEnviHeaderFlag(bool flag)
{
   theWriteEnviHeaderFlag = flag;
}

void ossimImageFileWriter::setWriteErsHeaderFlag(bool flag)
{
   theWriteErsHeaderFlag = flag;
}

void ossimImageFileWriter::setWriteExternalGeometryFlag(bool flag)
{
   theWriteExternalGeometryFlag = flag;
}

void ossimImageFileWriter::setWriteFgdcFlag(bool flag)
{
   theWriteFgdcFlag = flag;
}

void ossimImageFileWriter::setWriteJpegWorldFile(bool flag)
{
   theWriteJpegWorldFileFlag  = flag;
}

void ossimImageFileWriter::setWriteReadme(bool flag)
{
   theWriteReadmeFlag = flag;
}

void ossimImageFileWriter::setWriteTiffWorldFile(bool flag)
{
   theWriteTiffWorldFileFlag  = flag;
}

void ossimImageFileWriter::setWriteWorldFile(bool flag)
{
   theWriteWorldFileFlag  = flag;
}

ossimObject* ossimImageFileWriter::getObject()
{
   return this;
}

const ossimObject* ossimImageFileWriter::getObject() const
{
   return this;
}

void ossimImageFileWriter::setAreaOfInterest(const ossimIrect& inputRect)
{
   // This sets "theAreaOfInterest".
   ossimImageWriter::setAreaOfInterest(inputRect);
   
   if( theInputConnection.valid() )
   {
      //---
      // ossimImageSourceSequencer::setAreaOfInterest does more than just set the
      // variable, so only call if out of sync to avoid wasted cycles.
      //--- 
      if ( theAreaOfInterest != theInputConnection->getBoundingRect() )
      {
         // Set the sequencer area of interest.
         theInputConnection->setAreaOfInterest(inputRect);
      }
   }
}

ossimImageSourceSequencer* ossimImageFileWriter::getSequencer()
{
   return theInputConnection.get();
}

bool ossimImageFileWriter::execute()
{
   static const char MODULE[] = "ossimImageFileWriter::execute";
   
   if (!theInputConnection) // Must have a sequencer...
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " DEBUG:\ntheInputConnection is NULL!" << endl;
      }
      return false;
   }
   
   if (theFilename == "") // Must have a filename...
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " DEBUG:\ntheFilename is empty!" << endl;
      }
      return false;
   }
   
   
   // make sure we have a region of interest
   if(theAreaOfInterest.hasNans())
   {
      theInputConnection->initialize();
      theAreaOfInterest = theInputConnection->getAreaOfInterest();
   }
   else
   {
      theInputConnection->setAreaOfInterest(theAreaOfInterest);
   }

   if(theAreaOfInterest.hasNans()) // Must have an area of interest...
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " ERROR:  Area of interest has nans!"
            << "Area of interest:  "
            << theAreaOfInterest
            << "\nReturning..." << endl;
      }

      return false;
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " DEBUG:\n"
         << "Area of interest:  " << theAreaOfInterest << endl;
   }
   
   ossimRefPtr<ossimImageSource> savedInput;
   if (theScaleToEightBitFlag)
   {
      if(theInputConnection->getOutputScalarType() != OSSIM_UINT8)
      {
         savedInput = new ossimScalarRemapper;
         
         savedInput->connectMyInputTo(0, theInputConnection->getInput(0));
         theInputConnection->connectMyInputTo(0, savedInput.get());
         theInputConnection->initialize();
      }
   }

   ossimFilename path = theFilename.path();
   if(!path.exists())
   {
      path.createDirectory(true);
   }
   setProcessStatus(ossimProcessInterface::PROCESS_STATUS_EXECUTING);
   //setPercentComplete(0.0); let writeFile set the precent complete
   bool wroteFile = true;
   bool result    = true;
   if (theWriteImageFlag)
   {
      wroteFile = writeFile();
   }
  
   /*
   if(needsAborting())
   {
      setProcessStatus(ossimProcessInterface::PROCESS_STATUS_ABORTED);
   }
   else
   {
      setProcessStatus(ossimProcessInterface::PROCESS_STATUS_NOT_EXECUTING);
   }
   */
   if (theWriteOverviewFlag&&!needsAborting())
   {
      // Writing overviews has been mpi'd so do on all processes.
      if (!writeOverviewFile(theOverviewCompressType,
                             theOverviewJpegCompressQuality))
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
            << "Write of overview file failed!" << endl;
         }
      }
   }

   if(!needsAborting())
   {
      // Do these only on the master process. Note left to right precedence!
      if (getSequencer() && getSequencer()->isMaster())
      {
         if (wroteFile)
         {
            if (writeMetaDataFiles() == false)
            {
               if(traceDebug())
               {
                  ossimNotify(ossimNotifyLevel_WARN)
                  << "Write of metadata file failed!" << endl;
               }
               result = false;
            }
         }
         
      } // End of "if (getSequencer() && getSequencer()->isMaster()))
   }

   if(savedInput.valid())
   {
      ossimConnectableObject* obj = theInputConnection->getInput(0);
      if(obj)
      {
         theInputConnection->connectMyInputTo(0, obj->getInput(0));
      }
   }
   if(!needsAborting())
   {
      setProcessStatus(ossimProcessInterface::PROCESS_STATUS_NOT_EXECUTING);
   }
   else 
   {
      setProcessStatus(ossimProcessInterface::PROCESS_STATUS_ABORTED);   
   }

   savedInput = 0;
   return result;
}

bool ossimImageFileWriter::writeStream()
{
   //---
   // Not implemented in this class. Derived classed should implement if
   // they can write to a stream.
   //--- 
   return false;
}

ossimRefPtr<ossimOStream> ossimImageFileWriter::getOutputStream() const
{
   //---
   // Not implemented in this class. Derived classed should implement if
   // they can write to a stream.
   //--- 
   return ossimRefPtr<ossimOStream>();  
}

bool ossimImageFileWriter::setOutputStream(
   ossimRefPtr<ossimOStream> /* stream */ )
{
   //---
   // Not implemented in this class. Derived classed should implement if
   // they can write to a stream.
   //---
   return false;
}

bool ossimImageFileWriter::setOutputStream(std::ostream& /* str */)
{
   //---
   // Not implemented in this class. Derived classed should implement if
   // they can write to a stream.
   //---
   return false;
}


void ossimImageFileWriter::setPercentComplete(double percentComplete)
{
   ossimProcessInterface::setPercentComplete(percentComplete);
   ossimProcessProgressEvent event(this, percentComplete);
   fireEvent(event);
}

void ossimImageFileWriter::setOutputName(const ossimString& outputName)
{
   ossimImageWriter::setOutputName(outputName);
   setFilename(ossimFilename(outputName));
}

void ossimImageFileWriter::setFilename(const ossimFilename& file)
{
   theFilename = file;
}

const ossimFilename& ossimImageFileWriter::getFilename()const
{
   return theFilename;
}

bool ossimImageFileWriter::canConnectMyInputTo(ossim_int32 inputIndex,
                                               const ossimConnectableObject* object)const
{
   return (object&&
           ( (PTR_CAST(ossimImageSource, object)&&inputIndex == 0)||
             (PTR_CAST(ossimViewController, object)&&inputIndex == 1)) );
}

ossim_uint16 ossimImageFileWriter::getOverviewCompressType() const
{
   return theOverviewCompressType;
}

ossim_int32  ossimImageFileWriter::getOverviewJpegCompressQuality() const
{
   return theOverviewJpegCompressQuality;
}

void ossimImageFileWriter::setOverviewCompressType(ossim_uint16 type)
{
   theOverviewCompressType = type;
}

void ossimImageFileWriter::setOverviewJpegCompressQuality(ossim_int32 quality)
{
   theOverviewJpegCompressQuality = quality;
}

void ossimImageFileWriter::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(!property) return;
   
   if(property->getName() == ossimKeywordNames::FILENAME_KW)
   {
      theFilename = property->valueToString();
   }
   else if (property->getName() == "create_envi_hdr")
   {
      theWriteEnviHeaderFlag = property->valueToString().toBool();
   }
   else if (property->getName() == "create_ers_hdr")
   {
      theWriteErsHeaderFlag = property->valueToString().toBool();
   }
   else if (property->getName() ==
            ossimKeywordNames::CREATE_EXTERNAL_GEOMETRY_KW)
   {
      theWriteExternalGeometryFlag = property->valueToString().toBool();
   }
   else if (property->getName() == "create_fgdc")
   {
      theWriteFgdcFlag = property->valueToString().toBool();
   }
   else if (property->getName() == "create_jpeg_world_file")
   {
      theWriteJpegWorldFileFlag = property->valueToString().toBool();
   }
   else if (property->getName() == "create_readme")
   {
      theWriteReadmeFlag = property->valueToString().toBool();
   }
   else if (property->getName() == "create_tiff_world_file")
   {
      theWriteTiffWorldFileFlag = property->valueToString().toBool();
   }
   else if (property->getName() == "create_world_file")
   {
      theWriteWorldFileFlag = property->valueToString().toBool();
   }
   else if (property->getName() == ossimKeywordNames::CREATE_IMAGE_KW)
   {
      theWriteImageFlag = property->valueToString().toBool();
   }
   else if (property->getName() == ossimKeywordNames::CREATE_OVERVIEW_KW)
   {
      theWriteOverviewFlag = property->valueToString().toBool();
   }
   else if (property->getName() == ossimKeywordNames::CREATE_HISTOGRAM_KW)
   {
      theWriteHistogramFlag = property->valueToString().toBool();
   }
   else if (property->getName() == SCALE_TO_EIGHT_BIT_KW)
   {
      theScaleToEightBitFlag = property->valueToString().toBool();
   }
   else if(property->getName() == AUTO_CREATE_DIRECTORY_KW)
   {
      theAutoCreateDirectoryFlag = property->valueToString().toBool();
   }
   else if (property->getName() == "linear_units")
   {
      theLinearUnits = (ossimUnitType)
         (ossimUnitTypeLut::instance()->
          getEntryNumber(property->valueToString()));
   }
   else if(property->getName() == ossimKeywordNames::IMAGE_TYPE_KW)
   {
      setOutputImageType(property->valueToString());
   }
   else if(property->getName() == ossimKeywordNames::PIXEL_TYPE_KW)
   {
      ossimStringProperty* stringProperty = PTR_CAST(ossimStringProperty,
                                                     property.get());
      if (stringProperty)
      {
         ossimString s;
         stringProperty->valueToString(s);
         setPixelType(s);
      }
   }
   else
   {
      ossimImageWriter::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimImageFileWriter::getProperty(const ossimString& name)const
{
   if(name == ossimKeywordNames::FILENAME_KW)
   {
      ossimFilenameProperty* filenameProp =
         new ossimFilenameProperty(name, theFilename);

      filenameProp->setIoType(ossimFilenameProperty::ossimFilenamePropertyIoType_OUTPUT);

      return filenameProp;
   }
   else if(name == ossimKeywordNames::IMAGE_TYPE_KW)
   {
      ossimStringProperty* prop =  new ossimStringProperty(name, theOutputImageType, false);
      std::vector<ossimString> typeList;
      getImageTypeList(typeList);
      if(typeList.size() > 1)
      {
         prop->setReadOnlyFlag(false);
         prop->setConstraints(typeList);
         prop->setChangeType(ossimProperty::ossimPropertyChangeType_AFFECTS_OTHERS);
      }
      else
      {
         prop->setReadOnlyFlag(true);
      }
      return prop;
   }
   else if(name == AUTO_CREATE_DIRECTORY_KW)
   {
      return new ossimBooleanProperty(name, theAutoCreateDirectoryFlag);
   }
   else if (name == "create_envi_hdr")
   {
      return new ossimBooleanProperty(name, theWriteEnviHeaderFlag);
   }
   else if (name == "create_ers_hdr")
   {
      return new ossimBooleanProperty(name, theWriteErsHeaderFlag);
   }
   else if (name == ossimKeywordNames::CREATE_EXTERNAL_GEOMETRY_KW)
   {
      return new ossimBooleanProperty(name, theWriteExternalGeometryFlag);
   }
   else if (name == "create_fgdc")
   {
      return new ossimBooleanProperty(name, theWriteFgdcFlag);
   }
   else if (name == "create_jpeg_world_file")
   {
      return new ossimBooleanProperty(name, theWriteJpegWorldFileFlag);
   }
   else if (name == "create_readme")
   {
      return new ossimBooleanProperty(name, theWriteReadmeFlag);
   }
   else if (name == "create_tiff_world_file")
   {
      return new ossimBooleanProperty(name, theWriteTiffWorldFileFlag);
   }
   else if (name == "create_world_file")
   {
      return new ossimBooleanProperty(name, theWriteWorldFileFlag);
   }
   else if (name == ossimKeywordNames::CREATE_IMAGE_KW)
   {
      return new ossimBooleanProperty(name, theWriteImageFlag);
   }
   else if (name == ossimKeywordNames::CREATE_OVERVIEW_KW)
   {
      return new ossimBooleanProperty(name, theWriteOverviewFlag);
   }
   else if (name == ossimKeywordNames::CREATE_HISTOGRAM_KW)
   {
      return new ossimBooleanProperty(name, theWriteHistogramFlag);
   }
   else if (name == SCALE_TO_EIGHT_BIT_KW)
   {
      return new ossimBooleanProperty(name, theScaleToEightBitFlag);
   }
   else if (name == "linear_units")
   {
      // property value
      ossimString value = ossimUnitTypeLut::instance()->
         getEntryString(theLinearUnits);
      
      //---
      // theLinearUnits is set to unknown as a default.  This keys the
      // writer to use whatever makes sense for the given output.
      // Since "unknown" is a bad name to present to users, we will
      // use "default" if theLinearUnits is == "unknown".
      //---
      if (value == "unknown")
      {
         value = "default";
      }
      
      // constraint list
      vector<ossimString> constraintList;
      constraintList.push_back(ossimString("default"));
      constraintList.push_back(ossimString("meters"));
      constraintList.push_back(ossimString("feet"));
      constraintList.push_back(ossimString("us_survey_feet"));
      
      return new ossimStringProperty(name,
                                     value,
                                     false, // not editable
                                     constraintList);
   }
   else if (name == ossimKeywordNames::PIXEL_TYPE_KW)
   {
      ossimString value;
      getPixelTypeString(value);
      ossimStringProperty* stringProp =
         new ossimStringProperty(name,
                                 value,
                                 false); // editable flag
      stringProp->addConstraint(ossimString("point"));
      stringProp->addConstraint(ossimString("area"));
      return stringProp;
   }

   return ossimImageWriter::getProperty(name);
}

void ossimImageFileWriter::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimImageWriter::getPropertyNames(propertyNames);

   propertyNames.push_back(ossimString(ossimKeywordNames::FILENAME_KW));
   propertyNames.push_back(ossimString("create_envi_hdr"));  
   propertyNames.push_back(ossimString(ossimKeywordNames::CREATE_EXTERNAL_GEOMETRY_KW));
   propertyNames.push_back(ossimString("create_fgdc"));
   propertyNames.push_back(ossimString("create_jpeg_world_file"));
   propertyNames.push_back(ossimString("create_readme"));
   propertyNames.push_back(ossimString("create_tiff_world_file"));
   propertyNames.push_back(ossimString("create_world_file"));   
   propertyNames.push_back(ossimString(ossimKeywordNames::CREATE_IMAGE_KW));
   propertyNames.push_back(ossimString(ossimKeywordNames::CREATE_OVERVIEW_KW));
   propertyNames.push_back(ossimString(ossimKeywordNames::CREATE_HISTOGRAM_KW));
   propertyNames.push_back(ossimString(AUTO_CREATE_DIRECTORY_KW));
   propertyNames.push_back(ossimString(SCALE_TO_EIGHT_BIT_KW));
   propertyNames.push_back(ossimString("linear_units"));
   propertyNames.push_back(ossimString(ossimKeywordNames::PIXEL_TYPE_KW));
   propertyNames.push_back(ossimString(ossimKeywordNames::IMAGE_TYPE_KW));

}

ossimString ossimImageFileWriter::getExtension() const
{
   // default unknown file extension
   return ossimString("ext");
}

bool ossimImageFileWriter::getOutputHasInternalOverviews( void ) const
{
   // default value: don't assume internal overviews will
   // be generated by image writers.
   return false;
}

bool ossimImageFileWriter::hasImageType(const ossimString& imageType) const
{
   std::vector<ossimString> imageTypeList;
   getImageTypeList(imageTypeList);

   std::vector<ossimString>::const_iterator i = imageTypeList.begin();

   while (i != imageTypeList.end())
   {
      if ( (*i) == imageType )
      {
         return true;
      }
      ++i;
   }

   return false; // not found...
}

bool ossimImageFileWriter::addListener(ossimListener* listener)
{
   theProgressListener = listener;
   return ossimImageWriter::addListener(listener);
}

bool ossimImageFileWriter::removeListener(ossimListener* listener)
{
   if (listener == theProgressListener)
   {
      theProgressListener = NULL;
   }
   return ossimImageWriter::removeListener(listener);
}

void ossimImageFileWriter::setPixelType(ossimPixelType type)
{
   thePixelType = type;
}

void ossimImageFileWriter::setPixelType(const ossimString& type)
{
   ossimString s = type;
   s.downcase();
   if (s == "point")
   {
      setPixelType(OSSIM_PIXEL_IS_POINT);
   }
   else if (s == "area")
   {
      setPixelType(OSSIM_PIXEL_IS_AREA);
   }
}

ossimPixelType ossimImageFileWriter::getPixelType() const
{
   return thePixelType;
}

void ossimImageFileWriter::getPixelTypeString(ossimString& type) const
{
   if (thePixelType == OSSIM_PIXEL_IS_POINT)
   {
      type = "point";
   }
   else
   {
      type = "area";
   }
}

void ossimImageFileWriter::setTileSize(const ossimIpt& tileSize)
{
   if (theInputConnection.valid())
   {
      theInputConnection->setTileSize(tileSize);
   }
}

bool ossimImageFileWriter::writeWorldFile() 
{
   bool result = false;
   
   if( theFilename.size() )
   {
      // Make the file name.
      ossimFilename file = theFilename;
      ossimString ext = file.ext().downcase();
      
      ossimString wfExt = ""; // World file extension.
      
      if ( ( ext == "tif" ) || ( ext == "tiff" ) )
      {
         wfExt = "tfw";
      }
      else if ( ( ext == "jpg" ) || ( ext == "jpeg" ) )
      {
         wfExt = "jpw";
      }
      else if ( ext == "png" )
      {
         wfExt = "pgw";
      }
      else if ( ext == "sid" )
      {
         wfExt = "sdw";
      }
      
      if ( ext.size() )
      {
         file.setExtension(wfExt);
         
         result = writeWorldFile(file);
      }
      else
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "Could not derive world file extension for: " << theFilename << "\n"; 
      }
   }
   
   if ( !result )
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "Write of world file failed!\n";
   }   
   
   return result;
}

bool ossimImageFileWriter::writeWorldFile(const ossimFilename& file) 
{
   if( !theInputConnection || theAreaOfInterest.hasNans())
   {
      return false;
   }

   // Make the writer.
   ossimRefPtr<ossimWorldFileWriter> writer = new ossimWorldFileWriter();
   
   // Set things up.
   writer->connectMyInputTo(0, theInputConnection.get());
   writer->initialize();
   writer->setFilename(file);
   writer->setAreaOfInterest(theAreaOfInterest);
   writer->setLinearUnits(theLinearUnits);
   writer->setPixelType(thePixelType);
   
   // Write it to disk.
   return writer->execute(); 
}
