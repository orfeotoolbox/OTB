//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top level LICENSE.txt file.
//
// Author:  Frank Warmerdam (warmerdam@pobox.com)
//
// Description:
//
// Contains class declaration for ossimImageFileWriter.
//*******************************************************************
//  $Id: ossimImageFileWriter.cpp,v 1.45 2005/12/27 16:09:10 dburken Exp $

#include <ossimConfig.h>
#include <imaging/formats/ossimImageFileWriter.h>
#include <base/data_types/ossimKeywordlist.h>
#include <base/data_types/ossimFilename.h>
#include <base/data_types/ossimMultiResLevelHistogram.h>
#include <base/data_types/ossimMultiBandHistogram.h>
#include <base/data_types/ossimHistogram.h>
#include <base/common/ossimKeywordNames.h>
#include <base/context/ossimErrorContext.h>
#include <base/misc/lookup_tables/ossimImageTypeLut.h>
#include <imaging/formats/tiff/ossimTiffOverviewBuilder.h>
#include <imaging/factory/ossimImageHandlerRegistry.h>
#include <imaging/formats/ossimImageHandler.h>
#include <imaging/histogram/ossimHistogramWriter.h>
#include <imaging/histogram/ossimImageHistogramSource.h>
#include <imaging/metadata/ossimMetadataFileWriter.h>
#include <imaging/metadata/ossimGeomFileWriter.h>
#include <imaging/metadata/ossimEnviHeaderFileWriter.h>
#include <imaging/metadata/ossimFgdcFileWriter.h>
#include <imaging/metadata/ossimJpegWorldFileWriter.h>
#include <imaging/metadata/ossimReadmeFileWriter.h>
#include <imaging/metadata/ossimTiffWorldFileWriter.h>
#include <base/common/ossimStdOutProgress.h>
#include <base/property/ossimFilenameProperty.h>
#include <base/property/ossimBooleanProperty.h>
#include <base/common/ossimTrace.h>
#include <projections/map_projections/ossimMapProjection.h>
#include <projections/map_projections/ossimMapProjectionInfo.h>
#include <projections/factory/ossimProjectionFactoryRegistry.h>

static ossimTrace traceDebug("ossimImageFileWriter:debug");

#if OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimImageFileWriter.cpp,v 1.45 2005/12/27 16:09:10 dburken Exp $";
#endif

RTTI_DEF3(ossimImageFileWriter,
          "ossimImageFileWriter",
          ossimImageWriter,
          ossimProcessInterface,
          ossimConnectableObjectListener);

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
     theOverviewCompressType(COMPRESSION_NONE),
     theOverviewJpegCompressQuality(75),

     theWriteEnviHeaderFlag(false),
     theWriteExternalGeometryFlag(false),
     theWriteFgdcFlag(false),
     theWriteJpegWorldFileFlag(false),
     theWriteReadmeFlag(false),
     theWriteTiffWorldFileFlag(false)
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
   //
   ossimImageWriter::addListener((ossimConnectableObjectListener*)this);

   theInputConnection->connectMyInputTo(0, inputSource, false);
   theAreaOfInterest.makeNan();
}

ossimImageFileWriter::~ossimImageFileWriter()
{
   if(theInputConnection)
   {
      delete theInputConnection;
      theInputConnection = NULL;
   }
   theProgressListener = NULL;
   removeListener((ossimConnectableObjectListener*)this);
}

void ossimImageFileWriter::initialize()
{
   if(theInputConnection)
   {
      theInputConnection->initialize();
      setAreaOfInterest(theInputConnection->getBoundingRect());
   }
}

void ossimImageFileWriter::changeSequencer(ossimImageSourceSequencer* sequencer)
{
   if(!sequencer) return;
   if(theInputConnection)
   {
      sequencer->setAreaOfInterest(theInputConnection->getAreaOfInterest());
   }

   if(theInputConnection)
   {
      delete theInputConnection;
   }
   theInputConnection = sequencer;
   theInputConnection->connectMyInputTo(0, getInput(0));
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
                                             ossim_int32 jpeg_compress_quality) const
{
   if(theFilename == "") return false;

   // Get an image handler for the source file.
   ossimImageHandler* ih
      = ossimImageHandlerRegistry::instance()->open(theFilename);

   if (!ih)
   {
      if (ih) delete ih;

      // Set the error...
      return false;
   }

   ossimTiffOverviewBuilder ob(ih);

   if (ob.getErrorStatus() != ossimErrorCodes::OSSIM_ERROR)
   {
      // Give the listener to the overview builder if set.
      if (theProgressListener)
      {
         ob.addListener(theProgressListener);
      }
      
      // Build a new file name for the overview.
      ossimFilename overview_file = theFilename;
      overview_file.setExtension(ossimString("ovr"));
      ob.setCompressionType(tiff_compress_type);
      ob.setJpegCompressionQuality(jpeg_compress_quality);
      ob.buildOverview(overview_file, 0);

      // Remove the listener from the overview builder.
      if (theProgressListener)
      {
         ob.removeListener(theProgressListener);
      }
   }

   delete ih;

   return true;
}

bool ossimImageFileWriter::writeEnviHeaderFile() const
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
   writer->connectMyInputTo(0, theInputConnection);
   writer->initialize();
   writer->setFilename(outputFile);
   writer->setAreaOfInterest(theAreaOfInterest);
   
   // Write it to disk.
   return writer->execute();
}

bool ossimImageFileWriter::writeExternalGeometryFile() const
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
   writer->connectMyInputTo(0, theInputConnection);
   writer->initialize();
   writer->setFilename(geomFile);
   writer->setAreaOfInterest(theAreaOfInterest);

   // Write it to disk.
   return writer->execute();
}

bool ossimImageFileWriter::writeFgdcFile() const
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
   writer->connectMyInputTo(0, theInputConnection);
   writer->initialize();
   writer->setFilename(outputFile);
   writer->setAreaOfInterest(theAreaOfInterest);
   
   // Write it to disk.
   return writer->execute();
}

bool ossimImageFileWriter::writeJpegWorldFile() const
{
   if( (theFilename == "") || !theInputConnection ||
       theAreaOfInterest.hasNans())
   {
      return false;
   }

   // Make the file name.
   ossimFilename outputFile = theFilename;
   outputFile.setExtension(ossimString("jpw")); // jpw???

   // Make the writer.
   ossimRefPtr<ossimMetadataFileWriter> writer =
      new ossimJpegWorldFileWriter();
   
   // Set things up.
   writer->connectMyInputTo(0, theInputConnection);
   writer->initialize();
   writer->setFilename(outputFile);
   writer->setAreaOfInterest(theAreaOfInterest);
   
   // Write it to disk.
   return writer->execute();
}

bool ossimImageFileWriter::writeReadmeFile() const
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
   writer->connectMyInputTo(0, theInputConnection);
   writer->initialize();
   writer->setFilename(outputFile);
   writer->setAreaOfInterest(theAreaOfInterest);
   
   // Write it to disk.
   return writer->execute();
}

bool ossimImageFileWriter::writeTiffWorldFile() const
{
   if( (theFilename == "") || !theInputConnection ||
       theAreaOfInterest.hasNans())
   {
      return false;
   }

   // Make the file name.
   ossimFilename outputFile = theFilename;
   outputFile.setExtension(ossimString("tfw"));

   // Make the writer.
   ossimRefPtr<ossimMetadataFileWriter> writer =
      new ossimTiffWorldFileWriter();
   
   // Set things up.
   writer->connectMyInputTo(0, theInputConnection);
   writer->initialize();
   writer->setFilename(outputFile);
   writer->setAreaOfInterest(theAreaOfInterest);
   
   // Write it to disk.
   return writer->execute();
}

bool ossimImageFileWriter::writeMetaDataFiles() const
{
   bool status = true;
   
   if(theWriteEnviHeaderFlag)
   {
      if(writeEnviHeaderFile() == false)
      {
         status = false;

         ossimNotify(ossimNotifyLevel_WARN)
            << "Write of envi header file failed!" << endl;
      }
   }

   if (theWriteExternalGeometryFlag)
   {
      if( writeExternalGeometryFile() == false)
      {
         status = false;

         ossimNotify(ossimNotifyLevel_WARN)
            << "Write of geometry file failed!" << endl;
      }
   }
   if(theWriteFgdcFlag)
   {
      if(writeFgdcFile() == false)
      {
         status = false;

         ossimNotify(ossimNotifyLevel_WARN)
            << "Write of fgdc file failed!" << endl;
      }
   }
   if(theWriteJpegWorldFileFlag)
   {
      if(writeJpegWorldFile() == false)
      {
         status = false;
         
         ossimNotify(ossimNotifyLevel_WARN)
            << "Write of jpeg world file failed!" << endl;
      }
   }
   if(theWriteReadmeFlag)
   {
      if(writeReadmeFile() == false)
      {
         status = false;
         
         ossimNotify(ossimNotifyLevel_WARN)
            << "Write of readme file failed!" << endl;
      }
   }
   if(theWriteTiffWorldFileFlag)
   {
      if(writeTiffWorldFile() == false)
      {
         status = false;
         
         ossimNotify(ossimNotifyLevel_WARN)
            << "Write of tiff world file failed!" << endl;
      }
   }

   if (theWriteOverviewFlag)
   {
      if (!writeOverviewFile(theOverviewCompressType,
                             theOverviewJpegCompressQuality))
      {
         status = false;
         
         ossimNotify(ossimNotifyLevel_WARN)
            << "Write of overview file failed!" << endl;
      }
   }
         
   if (theWriteHistogramFlag)
   {
      if (!writeHistogramFile())
      {
         status = false;
         
         ossimNotify(ossimNotifyLevel_WARN)
            << "Write of histogram file failed!" << endl;
      }
   }

   return status;
}

bool ossimImageFileWriter::writeHistogramFile() const
{
   if(theFilename == "") return false;

   ossimFilename histogram_file = theFilename;
   histogram_file.setExtension(ossimString("his"));

   ossimImageHandler* handler = ossimImageHandlerRegistry::instance()->
      open(theFilename);

   if (!handler)
   {
      return false;
   }

   ossimImageHistogramSource* histoSource = new ossimImageHistogramSource;

   ossimHistogramWriter* writer = new ossimHistogramWriter;
   histoSource->setMaxNumberOfRLevels(1); // Only compute for r0.
   histoSource->connectMyInputTo(0, handler);
   histoSource->enableSource();
   writer->connectMyInputTo(0, histoSource);
   writer->setFilename(histogram_file);
   writer->addListener(&theStdOutProgress);
   writer->execute();

   delete handler;
   delete writer;

   return true;
}

void ossimImageFileWriter::disconnectInputEvent(ossimConnectionEvent& event)
{
   if(!getInput(0))
   {
      theInputConnection->disconnectMyInput((ossim_int32)0, false);
   }
   theViewController = PTR_CAST(ossimViewController, getInput(1));

   initialize();
}

void ossimImageFileWriter::connectInputEvent(ossimConnectionEvent& event)
{
   theInputConnection->connectMyInputTo((ossim_int32)0,
                                        getInput(0),
                                        false);
   theViewController = PTR_CAST(ossimViewController, getInput(1));
   initialize();
}

void ossimImageFileWriter::propertyEvent(ossimPropertyEvent& event)
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

bool ossimImageFileWriter::getWriteEnviHeaderFlag() const
{
   return theWriteEnviHeaderFlag;
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

void ossimImageFileWriter::setWriteEnviHeaderFlag(bool flag)
{
   theWriteEnviHeaderFlag = flag;
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
   ossimImageWriter::setAreaOfInterest(inputRect);
   if(theInputConnection)
   {
      theInputConnection->setAreaOfInterest(inputRect);
   }
}

ossimImageSourceSequencer* ossimImageFileWriter::getSequencer()
{
   return theInputConnection;
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

   setProcessStatus(ossimProcessInterface::PROCESS_STATUS_EXECUTING);
   setPercentComplete(0.0);
   bool wroteFile = true;
   if (theWriteImageFlag)
   {
      wroteFile = writeFile();
   }
   
   if(needsAborting())
   {
      setProcessStatus(ossimProcessInterface::PROCESS_STATUS_ABORTED);
   }
   else
   {
      setProcessStatus(ossimProcessInterface::PROCESS_STATUS_NOT_EXECUTING);
   }

   if(getProcessStatus() != ossimProcessInterface::PROCESS_STATUS_ABORTED)
   {
      // Do these only on the master process. Note left to right precedence!
      if (getSequencer() && getSequencer()->isMaster())
      {
         if (wroteFile)
         {
            if (writeMetaDataFiles() == false)
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << "Write of overview file failed!" << endl;
               return false;
            }
         }
         
      } // End of "if (getSequencer() && getSequencer()->isMaster()))
   }
   
   return true;
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
           ( (PTR_CAST(ossimImageSourceInterface, object)&&inputIndex == 0)||
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
   else if (name == "create_envi_hdr")
   {
      return new ossimBooleanProperty(name, theWriteEnviHeaderFlag);
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
   propertyNames.push_back(ossimString(ossimKeywordNames::CREATE_IMAGE_KW));
   propertyNames.push_back(ossimString(ossimKeywordNames::CREATE_OVERVIEW_KW));
   propertyNames.push_back(ossimString(ossimKeywordNames::CREATE_HISTOGRAM_KW));
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

void ossimImageFileWriter::setTileSize(const ossimIpt& tileSize)
{
   if (theInputConnection)
   {
      theInputConnection->setTileSize(tileSize);
   }
}
