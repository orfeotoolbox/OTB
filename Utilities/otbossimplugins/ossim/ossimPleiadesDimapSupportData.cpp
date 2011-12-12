//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Mickael Savinaud
//
// Description:
//
// Contains definition of class ossimPleiadesDimapSupportData.
//
//*****************************************************************************
// FIXME $Id: ossimPleiadesDimapSupportData 19682 2011-05-31 14:21:20Z dburken $

#include <ossimPleiadesDimapSupportData.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimXmlDocument.h>
#include <ossim/base/ossimXmlAttribute.h>
#include <ossim/base/ossimXmlNode.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotifyContext.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <sstream>


// Define Trace flags for use within this file:
static ossimTrace traceDebug ("ossimPleiadesDimapSupportData:debug");

static const ossim_uint32  LAGRANGE_FILTER_SIZE = 8; // num samples considered

namespace ossimplugins
{
ossimPleiadesDimapSupportData::ossimPleiadesDimapSupportData ()
   :
   ossimErrorStatusInterface(),
   theMetadataVersion(OSSIM_PLEIADES_METADATA_VERSION_UNKNOWN),
   theImageID(),
   theMetadataFile(),
   theProductionDate(),
   theInstrument(),
   theInstrumentIndex(0),

   theSunAzimuth(),
   theSunElevation(),
   theIncidenceAngle(),
   theViewingAngle(),

   theSceneOrientation(0.0),
   theImageSize(0.0, 0.0),
   theRefGroundPoint(0.0, 0.0, 0.0),
   theRefImagePoint(0.0, 0.0),
   theSubImageOffset(0.0, 0.0),
   theRefLineTime(0.0),
   theRefLineTimeLine(0.0),
   theLineSamplingPeriod(0.0),
   theDetectorCount(0),
   thePixelLookAngleX(),
   thePixelLookAngleY(),
   theAttitudeSamples(),
   theAttSampTimes(),
   thePosEcfSamples(),
   theVelEcfSamples(),
   theEphSampTimes(),
   theStarTrackerUsed(false),
   theMultiDataFile(),
   theNumBands(0),
   theAcquisitionDate(),
   theStepCount(0),
   theUlCorner(),
   theUrCorner(),
   theLrCorner(),
   theLlCorner(),
   theGeoPosImagePoints(),
   theGeoPosGroundPoints()
{
}
ossimPleiadesDimapSupportData::ossimPleiadesDimapSupportData(const ossimPleiadesDimapSupportData& rhs)
   :ossimErrorStatusInterface(rhs),
    theMetadataVersion(rhs.theMetadataVersion),
    theImageID(rhs.theImageID),
    theMetadataFile (rhs.theMetadataFile),
    theProductionDate(rhs.theProductionDate),
    theInstrument(rhs.theInstrument),
    theInstrumentIndex(rhs.theInstrumentIndex),
    theSunAzimuth(rhs.theSunAzimuth),
    theSunElevation(rhs.theSunElevation),
    theIncidenceAngle(rhs.theIncidenceAngle),
    theViewingAngle(rhs.theViewingAngle),
    theSceneOrientation(rhs.theSceneOrientation),
    theImageSize(rhs.theImageSize),
    theRefGroundPoint(rhs.theRefGroundPoint),
    theRefImagePoint(rhs.theRefImagePoint),
    theSubImageOffset(rhs.theSubImageOffset),
    theRefLineTime(rhs.theRefLineTime),
    theRefLineTimeLine(rhs.theRefLineTimeLine),
    theLineSamplingPeriod(rhs.theLineSamplingPeriod),
    theDetectorCount(rhs.theDetectorCount),
    thePixelLookAngleX(rhs.thePixelLookAngleX),
    thePixelLookAngleY(rhs.thePixelLookAngleY),
    theAttitudeSamples(rhs.theAttitudeSamples),
    theAttSampTimes(rhs.theAttSampTimes),
    thePosEcfSamples(rhs.thePosEcfSamples),
    theVelEcfSamples(rhs.theVelEcfSamples),
    theEphSampTimes(rhs.theEphSampTimes),
    theStarTrackerUsed(rhs.theStarTrackerUsed),
    theMultiDataFile(rhs.theMultiDataFile),
    theNumBands(rhs.theNumBands),
    theAcquisitionDate(rhs.theAcquisitionDate),
    theStepCount(rhs.theStepCount),
    theUlCorner(rhs.theUlCorner),
    theUrCorner(rhs.theUrCorner),
    theLrCorner(rhs.theLrCorner),
    theLlCorner(rhs.theLlCorner),
    theGeoPosImagePoints(rhs.theGeoPosImagePoints),
    theGeoPosGroundPoints(rhs.theGeoPosGroundPoints)
{
}

ossimPleiadesDimapSupportData::ossimPleiadesDimapSupportData (const ossimFilename& dimapFile)
   :
   ossimErrorStatusInterface(),
   theMetadataVersion(OSSIM_PLEIADES_METADATA_VERSION_UNKNOWN),
   theImageID(),
   theMetadataFile (dimapFile),
   theProductionDate(),
   theInstrument(),
   theInstrumentIndex(0),
   theSunAzimuth(),
   theSunElevation(),
   theIncidenceAngle(),
   theViewingAngle(),
   theSceneOrientation(0.0),
   theImageSize(0.0, 0.0),
   theRefGroundPoint(0.0, 0.0, 0.0),
   theRefImagePoint(0.0, 0.0),
   theSubImageOffset(0.0, 0.0),
   theRefLineTime(0.0),
   theRefLineTimeLine(0.0),
   theLineSamplingPeriod(0.0),
   theDetectorCount(0),
   thePixelLookAngleX(),
   thePixelLookAngleY(),
   theAttitudeSamples(),
   theAttSampTimes(),
   thePosEcfSamples(),
   theVelEcfSamples(),
   theEphSampTimes(),
   theStarTrackerUsed(false),
   theMultiDataFile(),
   theNumBands(0),
   theAcquisitionDate(),
   theStepCount(0),
   theUlCorner(),
   theUrCorner(),
   theLrCorner(),
   theLlCorner(),
   theGeoPosImagePoints(),
   theGeoPosGroundPoints()
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimPleiadesDimapSupportData::ossimPleiadesDimapSupportData: entering..."
         << std::endl;
   }

   loadXmlFile(dimapFile);

   // Finished successful parse:
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimPleiadesDimapSupportData::ossimPleiadesDimapSupportData: leaving..."
         << std::endl;
   }
}

ossimPleiadesDimapSupportData::~ossimPleiadesDimapSupportData ()
{
}

ossimObject* ossimPleiadesDimapSupportData::dup()const
{
   return new ossimPleiadesDimapSupportData(*this);
}

void ossimPleiadesDimapSupportData::clearFields()
{
   clearErrorStatus();
   theSensorID="";
   theMetadataVersion = OSSIM_PLEIADES_METADATA_VERSION_UNKNOWN;
   theImageID = "";
   theMetadataFile = "";
   theProductionDate = "";
   theInstrument = "";
   theInstrumentIndex = 0;
   theSunAzimuth.clear();
   theSunElevation.clear();
   theIncidenceAngle.clear();
   theViewingAngle.clear();
   theSceneOrientation = 0.0;
   theImageSize.makeNan();
   theRefGroundPoint.makeNan();
   theRefImagePoint.makeNan();
   theSubImageOffset.makeNan();
   theRefLineTime = ossim::nan();
   theRefLineTimeLine = ossim::nan();
   theLineSamplingPeriod = ossim::nan();
   theDetectorCount = 0;
   thePixelLookAngleX.clear();
   thePixelLookAngleY.clear();
   theAttitudeSamples.clear(); // x=pitch, y=roll, z=yaw
   theAttSampTimes.clear();
   thePosEcfSamples.clear();
   theVelEcfSamples.clear();
   theEphSampTimes.clear();
   theStarTrackerUsed = false;
   ossimString msg = "";
   theMultiDataFile.setBooleanValue(false,msg);
   theNumBands = 0;
   theAcquisitionDate = "";
   theStepCount = 0;

   //---
   // Corner points:
   //---
   theUlCorner.makeNan();
   theUrCorner.makeNan();
   theLrCorner.makeNan();
   theLlCorner.makeNan();

   //---
   // Geoposition Points:
   //---
   theGeoPosImagePoints.clear();
   theGeoPosGroundPoints.clear();

}

bool ossimPleiadesDimapSupportData::loadXmlFile(const ossimFilename& file)
{
   static const char MODULE[] = "ossimPleiadesDimapSupportData::loadXmlFile";
   traceDebug.setTraceFlag(true);

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " DEBUG:"
         << "\nFile: " << file << std::endl;
   }
   clearFields();
   theMetadataFile = file;

  /* ossim_int64 fileSize = file.fileSize();
   std::ifstream in(file.c_str(), std::ios::binary|std::ios::in);
   std::vector<char> fullBuffer;
   ossimString bufferedIo;
   if(in.good()&&(fileSize > 0))
   {
      char buf[100];
      fullBuffer.resize(fileSize);
      in.read(buf, ossim::min((ossim_int64)100, fileSize));
      if(!in.fail())
      {
         ossimString testString = ossimString(buf,
                                              buf + in.gcount());
         if(testString.contains("xml"))
         {
            in.seekg(0);
            in.read(&fullBuffer.front(), (std::streamsize)fullBuffer.size());
            if(!in.fail())
            {
               bufferedIo = ossimString(fullBuffer.begin(),
                                        fullBuffer.begin()+in.gcount());
            }
         }
      }
   }
   else
   {
      return false;
   }
   //---
   // Instantiate the XML parser:
   //---
   ossimRefPtr<ossimXmlDocument> xmlDocument;

   if(bufferedIo.empty())
   {
      xmlDocument = new ossimXmlDocument(file);
   }
   else
   {
      xmlDocument = new ossimXmlDocument;
      std::istringstream inStringStream(bufferedIo.string());
      if(!xmlDocument->read(inStringStream))
      {
         return false;
      }
   }
   if (xmlDocument->getErrorStatus())
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_FATAL)
            << MODULE << " DEBUG:"
            << "ossimPleiadesDimapSupportData::loadXmlFile:"
            << "\nUnable to parse xml file" << std::endl;
      }
      setErrorStatus();
      return false;
   }

   //---
   // Check that it is a PLEIADES DIMAP file format
   //---
   vector<ossimRefPtr<ossimXmlNode> > xml_nodes;
   xml_nodes.clear();
   ossimString xpath = "/DIMAP_Document/Dataset_Sources/Source_Identification/Strip_Source/MISSION";
   xmlDocument->findNodes(xpath, xml_nodes);
   if (xml_nodes.size() == 0)
   {
      setErrorStatus();
      if(traceDebug())
      {
	 ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG:\n Not a PLEIADES DIMAP file format -> Key not found!"<< std::endl;
      }
      return false;
   }
   if ( xml_nodes[0]->getText() != "PHR" )
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG:\n Not a PLEIADES DIMAP file format -> Key value incorrect!"<< std::endl;
      }
      return false;
   }


   //---
   // Get the version string.  This must be performed first as it is used
   // as a key for parsing different versions.
   //---
   if (initMetadataVersion(xmlDocument) == false)
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_FATAL)
            << MODULE << " DEBUG:"
            << "ossimPleiadesDimapSupportData::loadXmlFile:"
            << "\nMetadata initialization failed.  Returning false"
            << std::endl;
      }
      return false;
   }*/

   ossimRefPtr<ossimXmlDocument> xmlDocument = InitXmlDocumentParser(file, OSSIM_PLEIADES_METADATA_TYPE_PRODUCT);
   if (!xmlDocument)
     {
     if(traceDebug())
     {
        ossimNotify(ossimNotifyLevel_FATAL)
           << MODULE << " DEBUG:"
           << "ossimPleiadesDimapSupportData::loadXmlFile:"
           << "\nInitXmlDocumentParser failed.  Returning false"
           << std::endl;
     }
     return false;

     }


   // Get the image id.
   if (initImageId(xmlDocument) == false)
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_FATAL)
            << MODULE << " DEBUG:"
            << "ossimPleiadesDimapSupportData::loadXmlFile:"
            << "\nImageId initialization failed.  Returning false"
            << std::endl;
      }
      return false;
   }

   // Get data from "Scene_Source" section.
   if (initSceneSource(xmlDocument)  == false)
   {
      ossimNotify(ossimNotifyLevel_FATAL)
         << MODULE << " DEBUG:"
         << "ossimPleiadesDimapSupportData::loadXmlFile:"
         << "\nScene source initialization failed.  Returning false"
         << std::endl;

      return false;
   }

   if (initFramePoints(xmlDocument)  == false)
   {
      ossimNotify(ossimNotifyLevel_FATAL)
         << MODULE << " DEBUG:"
         << "ossimPleiadesDimapSupportData::loadXmlFile:"
         << "\nFrame point initialization failed.  Returning false"
         << std::endl;
      return false;
   }

   if (parseCommonMetadata(xmlDocument) == false)
   {
      ossimNotify(ossimNotifyLevel_FATAL)
         << MODULE << " DEBUG:"
         << "ossimPleiadesDimapSupportData::loadXmlFile:"
         << "\nparseCommonMetadata initialization failed.  Returning false"
         << std::endl;
      return false;
   }

   if (parseRadiometricMetadata(xmlDocument) == false)
   {
      ossimNotify(ossimNotifyLevel_FATAL)
         << MODULE << " DEBUG:"
         << "ossimPleiadesDimapSupportData::loadXmlFile:"
         << "\nparseRadiometricMetadata initialization failed.  Returning false"
         << std::endl;
      return false;
   }

   if (theProcessingLevelString == "SENSOR")
     {
     if (parseSensorMetadata(xmlDocument) == false)
       {
       ossimNotify(ossimNotifyLevel_FATAL)
          << MODULE << " DEBUG:"
          << "ossimPleiadesDimapSupportData::loadXmlFile:"
          << "\nparseSensorMetadata initialization failed.  Returning false"
          << std::endl;
       return false;
       }
     }

   this->printInfo(std::cout);





   if (traceDebug())
   {
      printInfo(ossimNotify(ossimNotifyLevel_DEBUG));

      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " DEBUG: exited..."
         << std::endl;
   }

   return true;
}

void ossimPleiadesDimapSupportData::getPositionEcf(ossim_uint32 sample,
                                               ossimEcefPoint& pe)  const
{
   pe.makeNan();

   if (thePosEcfSamples.size() < theDetectorCount)
   {
      if(theImageSize.samp > 0)
      {
         double t = 0.0;
         double tempIdx = 0.0;
         double tempIdxFraction = 0.0;
         t = static_cast<double>(sample)/
            static_cast<double>(theDetectorCount-1);
         tempIdx = (thePosEcfSamples.size()-1)*t;
         tempIdxFraction = tempIdx - (ossim_int32)tempIdx;
         ossim_uint32 idxStart = (ossim_uint32)tempIdx;
         ossim_uint32 idxEnd = (ossim_uint32)ceil(tempIdx);
         if(idxEnd >= thePosEcfSamples.size())
         {
            idxEnd = (ossim_uint32)thePosEcfSamples.size()-1;
         }
         if(idxStart > idxEnd)
         {
            idxStart = idxEnd;
         }
         pe = ossimEcefPoint(thePosEcfSamples[idxStart].x +tempIdxFraction*( thePosEcfSamples[idxEnd].x - thePosEcfSamples[idxStart].x),
                             thePosEcfSamples[idxStart].y +tempIdxFraction*( thePosEcfSamples[idxEnd].y - thePosEcfSamples[idxStart].y),
                             thePosEcfSamples[idxStart].z +tempIdxFraction*( thePosEcfSamples[idxEnd].z - thePosEcfSamples[idxStart].z));

      }
   }
   else if(thePosEcfSamples.size() == theDetectorCount)
   {
      pe = ossimEcefPoint(thePosEcfSamples[sample].x,
                          thePosEcfSamples[sample].y,
                          thePosEcfSamples[sample].z);
   }
}

void ossimPleiadesDimapSupportData::getPositionEcf(const ossim_float64& time,
                                               ossimEcefPoint& pe)  const
{
   ossimDpt3d tempPt;

   if((thePosEcfSamples.size() < 8)||
      (theEphSampTimes.size() < 8))
   {
      getBilinearInterpolation(time, thePosEcfSamples, theEphSampTimes, tempPt);
   }
   else
   {
      getLagrangeInterpolation(time, thePosEcfSamples, theEphSampTimes, tempPt);
   }

   pe = ossimEcefPoint(tempPt.x,
                       tempPt.y,
                       tempPt.z);
}

void ossimPleiadesDimapSupportData::getVelocityEcf(ossim_uint32 sample, ossimEcefPoint& ve)  const
{
   ve.makeNan();

   if (theVelEcfSamples.size() < theDetectorCount)
   {
      if(theImageSize.samp > 0)
      {
         double t = 0.0;
         double tempIdx = 0.0;
         double tempIdxFraction = 0.0;
         t = static_cast<double>(sample)/
            static_cast<double>(theDetectorCount-1);
         tempIdx = (theVelEcfSamples.size()-1)*t;
         tempIdxFraction = tempIdx - (ossim_int32)tempIdx;
          ossim_uint32 idxStart = (ossim_uint32)tempIdx;
         ossim_uint32 idxEnd = (ossim_uint32)ceil(tempIdx);
         if(idxEnd >= theVelEcfSamples.size())
         {
            idxEnd = (ossim_uint32)theVelEcfSamples.size()-1;
         }
         if(idxStart > idxEnd)
         {
            idxStart = idxEnd;
         }
         ve = ossimEcefPoint(theVelEcfSamples[idxStart].x +tempIdxFraction*( theVelEcfSamples[idxEnd].x - theVelEcfSamples[idxStart].x),
                             theVelEcfSamples[idxStart].y +tempIdxFraction*( theVelEcfSamples[idxEnd].y - theVelEcfSamples[idxStart].y),
                             theVelEcfSamples[idxStart].z +tempIdxFraction*( theVelEcfSamples[idxEnd].z - theVelEcfSamples[idxStart].z));

      }

   }
   else if(theVelEcfSamples.size() == theDetectorCount)
   {
      ve = ossimEcefPoint(theVelEcfSamples[sample].x,
                          theVelEcfSamples[sample].y,
                          theVelEcfSamples[sample].z);
   }
}

void ossimPleiadesDimapSupportData::getVelocityEcf(const ossim_float64& time,
                                               ossimEcefPoint& ve)  const
{
   ossimDpt3d tempPt;

   if((theVelEcfSamples.size() < 8) ||
      (theEphSampTimes.size() < 8))
   {
      getBilinearInterpolation (time, theVelEcfSamples, theEphSampTimes, tempPt);
   }
   else
   {
      getLagrangeInterpolation (time, theVelEcfSamples, theEphSampTimes, tempPt);
   }

   ve = ossimEcefPoint(tempPt.x,
                       tempPt.y,
                       tempPt.z);
}

void ossimPleiadesDimapSupportData::getEphSampTime(ossim_uint32 sample,
                                               ossim_float64& et)  const
{
   et = ossim::nan();
   if(theEphSampTimes.size() < theImageSize.samp)
   {
      if(theImageSize.samp > 0)
      {
         double t = 0.0;
         double tempIdx = 0.0;
         double tempIdxFraction = 0.0;
         t = (double)sample/(double)(theImageSize.samp-1);
         tempIdx = (theEphSampTimes.size()-1)*t;
         tempIdxFraction = tempIdx - (ossim_int32)tempIdx;
         ossim_uint32 idxStart = (ossim_uint32)tempIdx;
         ossim_uint32 idxEnd = (ossim_uint32)ceil(tempIdx);
         if(idxEnd >= theEphSampTimes.size())
         {
            idxEnd = (ossim_uint32)theEphSampTimes.size()-1;
         }
         if(idxStart > idxEnd)
         {
            idxStart = idxEnd;
         }
         et = (theEphSampTimes[idxStart] +tempIdxFraction*(theEphSampTimes[idxEnd] -
                                                           theEphSampTimes[idxStart]));
      }
   }
   else if(theEphSampTimes.size() == theImageSize.samp)
   {
      et = theEphSampTimes[sample];
   }
}

void ossimPleiadesDimapSupportData::getAttitude(ossim_uint32 sample,
                                            ossimDpt3d& at)  const
{
   if (sample >= theAttitudeSamples.size())
   {
      at.makeNan();
      return;
   }

   at = theAttitudeSamples[sample];
}

void ossimPleiadesDimapSupportData::getAttitude(const ossim_float64& time,
                                            ossimDpt3d& at)  const
{
   if (theAttSampTimes.empty())
   {
     at.makeNan();
     return;
   }

   if ((time <  theAttSampTimes.front()) ||
       (time >= theAttSampTimes.back() ))
   {
      extrapolateAttitude(time, at);
      return;
   }

   //***
   // Search the attitude sampling time array for surrounding samples:
   //***
   int i=0;
   while ((i < (int)theAttSampTimes.size()) &&
          (theAttSampTimes[i] < time)) ++i;
   --i;

   //***
   // Linearly interpolate attitudes angles:
   //***
   ossim_float64 dt1   = time - theAttSampTimes[i];
   ossim_float64 dt0   = theAttSampTimes[i+1] - time;
   ossim_float64 dt    = theAttSampTimes[i+1] - theAttSampTimes[i];

   at = (theAttitudeSamples[i+1]*dt1 + theAttitudeSamples[i]*dt0)/dt;
}

void ossimPleiadesDimapSupportData::extrapolateAttitude(const ossim_float64& time, ossimDpt3d& at) const
{
   at.makeNan();
   int last_samp = (int) theAttSampTimes.size() - 1;
   if (last_samp < 1)
      return;

   ossimDpt3d dAtt, dAtt_dt;
   double dt, delta_t;

   // Determine whether extrapolating at the front or the back of the range:
   if (time < theAttSampTimes.front())
   {
      dt = theAttSampTimes[1] - theAttSampTimes[0];
      dAtt = theAttitudeSamples[1] - theAttitudeSamples[0];
      dAtt_dt = dAtt/dt;
      delta_t = time - theAttSampTimes[0];
      at = theAttitudeSamples[0] + (dAtt_dt*delta_t);
   }
   else if (time >= theAttSampTimes.back())
   {
      dt = theAttSampTimes[last_samp] - theAttSampTimes[last_samp-1];
      dAtt = theAttitudeSamples[last_samp] - theAttitudeSamples[last_samp-1];
      dAtt_dt = dAtt/dt;
      delta_t = time - theAttSampTimes[last_samp];
      at = theAttitudeSamples[last_samp] + (dAtt_dt*delta_t);
   }

   return;
}

void ossimPleiadesDimapSupportData::getAttSampTime(ossim_uint32 sample, ossim_float64& at)  const
{
   if (sample >= theAttSampTimes.size())
   {
      at = ossim::nan();
      return;
   }

   at = theAttSampTimes[sample];
}

void ossimPleiadesDimapSupportData::getPixelLookAngleX(ossim_uint32 sample,
                                                   ossim_float64& pa) const
{
   if (sample >= thePixelLookAngleX.size())
   {
      setErrorStatus();
      pa = ossim::nan();

      return;
   }

   pa = thePixelLookAngleX[sample];
}

void ossimPleiadesDimapSupportData::getPixelLookAngleX(const ossim_float64& sample,
                                                   ossim_float64& pa) const
{
   ossim_uint32 s = static_cast<ossim_uint32>(sample);
   getInterpolatedLookAngle(s, thePixelLookAngleX, pa);
}

void ossimPleiadesDimapSupportData::getPixelLookAngleY(ossim_uint32 sample,
                                                   ossim_float64& pa) const
{
   if (sample >= thePixelLookAngleY.size())
   {
      setErrorStatus();
      pa = ossim::nan();
      return;
   }

   pa = thePixelLookAngleY[sample];
}

void ossimPleiadesDimapSupportData::getPixelLookAngleY(const ossim_float64& sample,
                                                   ossim_float64& pa) const
{
   ossim_uint32 s = static_cast<ossim_uint32>(sample);
   getInterpolatedLookAngle(s, thePixelLookAngleY, pa);
}

void ossimPleiadesDimapSupportData::getInterpolatedLookAngle(
   const ossim_float64& p,
   const std::vector<ossim_float64>& angles,
   ossim_float64& la) const
{
   if ((p < 0.0) || (p >= (ossim_float64) angles.size()))
   {
      setErrorStatus();
      la = ossim::nan();
      return;
   }

   ossim_float64 p0 = floor(p);
   ossim_float64 p1 = ceil (p);

   if (p0 == p1)
   {
      la = angles[(int) p0];
   }
   else
   {
      ossim_float64 angle_0 = angles[(int) p0];
      ossim_float64 angle_1 = angles[(int) p1];

      la = (angle_0*(p1-p) + angle_1*(p-p0))/(p1-p0);
   }
}

void ossimPleiadesDimapSupportData::getBilinearInterpolation(
   const ossim_float64& time,
   const std::vector<ossimDpt3d>& V,
   const std::vector<ossim_float64>& T,
   ossimDpt3d& li) const
{
   ossim_uint32 samp0 = 0;
   while ((samp0 < T.size()) && (T[samp0] < time)) ++samp0;

   if(samp0==0)
   {
      li = V[0];
   }
   else if(samp0 == T.size())
   {
      li = V[1];
   }
   else
   {
      double t = (T[samp0-1]-time)/(T[samp0-1] - T[samp0]);

      li = V[samp0-1] + (V[samp0]-V[samp0-1])*t;
   }
}

void ossimPleiadesDimapSupportData::getLagrangeInterpolation(
   const ossim_float64& time,
   const std::vector<ossimDpt3d>& V,
   const std::vector<ossim_float64>& T,
   ossimDpt3d& li) const

{
//    std::cout << "V size = " << V.size() << std::endl
//              << "T size = " << T.size() << std::endl;

   ossim_uint32 filter_size = 8;
   //
   // Verify that t is within allowable range:
   //
   ossim_uint32 lagrange_half_filter = 4;

   if(T.size() <= filter_size)
   {
      filter_size = (ossim_uint32)T.size()/2;
      lagrange_half_filter = filter_size/2;
   }
   if ((time <  T[lagrange_half_filter]) ||
       (time >= T[T.size()-lagrange_half_filter] ))
   {
      setErrorStatus();
      li.makeNan();

      return;
   }

   //***
   // Search the sampling time array for surrounding samples:
   //***
   ossim_uint32 samp0 = lagrange_half_filter;
   while ((samp0 < T.size()) && (T[samp0] < time)) ++samp0;

   //***
   // Do not use sample if it falls in neighborhood of desired time:
   //***
   ossim_uint32 bump = 0;
   if (fabs(T[samp0] - time) < theLineSamplingPeriod/2.0)
      bump = 1;

   samp0 -= lagrange_half_filter; // adjust to first sample in window

   //***
   // Outer summation loop:
   //***
   ossimDpt3d S (0, 0, 0);
   for (ossim_uint32 j=samp0; j<(samp0+filter_size+bump); ++j)
   {
      ossim_float64 numerator   = 1.0;
      ossim_float64 denominator = 1.0;

      //***
      // Skip this sample if too close to desired time:
      //***
      if (bump && (j == (samp0+lagrange_half_filter) ))
         ++j;

      //***
      // Inner loop for product series:
      //***
      for (ossim_uint32 i=samp0; i<(samp0+filter_size+bump); ++i)
      {
         //***
         // Skip this sample if too close to desired time:
         //***
         if (bump && (i == (samp0+lagrange_half_filter) ))
            ++i;

         if (i != j)
         {
            numerator   *= time - T[i];
            denominator *= T[j] - T[i];
         }
      }

      ossimDpt3d p = V[j];
      p = p * numerator;
      p = p / denominator;
      S += p;
   }

   li = S;
}

ossim_float64 ossimPleiadesDimapSupportData::convertTimeStamp(const ossimString& time_stamp) const
{
   double ti;
   convertTimeStamp(time_stamp, ti);
   return ti;
}

void ossimPleiadesDimapSupportData::convertTimeStamp(const ossimString& time_stamp,
                                                 ossim_float64& ti) const
{
   int    year, month, day, hour, minute;
   double second;

   //***
   // Time stamps are in the format: "yyyy-mm-ddThh:mm:ss.ssssss"
   //***
   int converted = sscanf(time_stamp,
                          "%4d-%2d-%2dT%2d:%2d:%9lf",
                          &year, &month, &day,
                          &hour, &minute, &second);

   if (converted != 6)
   {
      setErrorStatus();
      ti = ossim::nan();
   }
   else
   {
      ti = (((((year-2002.0)*12.0 + month - 1.0)*365.0 + day - 1.0)*24.0
             + hour)*60.0 + minute)*60.0 + second;
   }
}

void ossimPleiadesDimapSupportData::getGeoPosPoint (ossim_uint32 point,
                                                ossimDpt& ip,
                                                ossimGpt& gp) const
{
   if ((point >= 0) && point < theGeoPosImagePoints.size())
   {
      ip = theGeoPosImagePoints [point];
      gp = theGeoPosGroundPoints[point];
   }
}

void ossimPleiadesDimapSupportData::printInfo(ostream& os) const
{
   ossimString corr_att = "NO";
   if (theStarTrackerUsed)
      corr_att = "YES";

   os << "\n----------------- Info on Pleiades Image -------------------"
      << "\n  "
      << "\n  Job Number (ID):      " << theImageID
      << "\n  Acquisition Date:     " << theAcquisitionDate
      << "\n  Instrument:           " << theInstrument
      << "\n  Instrument Index:     " << theInstrumentIndex
      << "\n  Production Date:      " << theProductionDate
      << "\n  Number of Bands:      " << theNumBands
      << "\n  Geo Center Point:     " << theRefGroundPoint
      << "\n  Detector count:       " << theDetectorCount
      << "\n  Image Size:           " << theImageSize
      << "\n  Incidence Angle (TopCenter):   " << theIncidenceAngle[0]
      << "\n  Incidence Angle (Center):      " << theIncidenceAngle[1]
      << "\n  Incidence Angle (BottomCenter):" << theIncidenceAngle[2]

      << "\n  Viewing Angle (TopCenter):     " << theViewingAngle[0]
      << "\n  Viewing Angle (Center):        " << theViewingAngle[1]
      << "\n  Viewing Angle (BottomCenter):  " << theViewingAngle[2]

      << "\n  Scene Orientation:    " << theSceneOrientation 
      << "\n  Corrected Attitude:   " << corr_att
      << "\n  Sun Azimuth (TopCenter):       " << theSunAzimuth[0]
      << "\n  Sun Azimuth (Center):          " << theSunAzimuth[1]
      << "\n  Sun Azimuth (BottomCenter):    " << theSunAzimuth[2]

      << "\n  Sun Elevation (TopCenter):     " << theSunElevation[0]
      << "\n  Sun Elevation (Center):        " << theSunElevation[1]
      << "\n  Sun Elevation (BottomCenter):  " << theSunElevation[2]

      << "\n  Sub image offset:     " << theSubImageOffset
      << "\n  Step Count:           " << theStepCount
      << "\n  PixelLookAngleX size: " << thePixelLookAngleX.size()
      << "\n  thePosEcfSamples size:" << thePosEcfSamples.size()
      << "\n  Corner Points:"
      << "\n     UL: " << theUlCorner
      << "\n     UR: " << theUrCorner
      << "\n     LR: " << theLrCorner
      << "\n     LL: " << theLlCorner
      << "\n"
      << "\n---------------------------------------------------------"
      << "\n  " << std::endl;
}

ossimString ossimPleiadesDimapSupportData::getSensorID() const
{
  return theSensorID;
}

ossimString   ossimPleiadesDimapSupportData::getMetadataVersionString() const
{
   if (theMetadataVersion == OSSIM_PLEIADES_METADATA_VERSION_2_0)
   {
      return ossimString("2.0");
   }
   return ossimString("unknown");
}

ossimString ossimPleiadesDimapSupportData::getAcquisitionDate() const
{
   return theAcquisitionDate;
}

ossimString ossimPleiadesDimapSupportData::getProductionDate() const
{
   return theProductionDate;
}

ossimString ossimPleiadesDimapSupportData::getImageID() const
{
   return theImageID;
}

ossimFilename ossimPleiadesDimapSupportData::getMetadataFile() const
{
   return theMetadataFile;
}

ossimString ossimPleiadesDimapSupportData::getInstrument() const
{
   return theInstrument;
}

ossim_uint32 ossimPleiadesDimapSupportData::getInstrumentIndex() const
{
   return theInstrumentIndex;
}

void ossimPleiadesDimapSupportData::getSunAzimuth(std::vector<ossim_float64>& az) const
{
   az = theSunAzimuth;
}

void ossimPleiadesDimapSupportData::getSunElevation(std::vector<ossim_float64>& el) const
{
   el = theSunElevation;
}

void ossimPleiadesDimapSupportData::getImageSize(ossimDpt& sz) const
{
   sz = theImageSize;
}

void ossimPleiadesDimapSupportData::getLineSamplingPeriod(ossim_float64& pe) const
{
   pe = theLineSamplingPeriod;
}

bool ossimPleiadesDimapSupportData::isStarTrackerUsed() const
{
   return theStarTrackerUsed;
}

ossim_uint32 ossimPleiadesDimapSupportData::getNumberOfBands() const
{
   return theNumBands;
}

ossim_uint32 ossimPleiadesDimapSupportData::getStepCount() const
{
   return theStepCount;
}

void ossimPleiadesDimapSupportData::getIncidenceAngle(std::vector<ossim_float64>& ia) const
{
   ia = theIncidenceAngle;
}

void ossimPleiadesDimapSupportData::getViewingAngle(std::vector<ossim_float64>& va) const
{
   va = theViewingAngle;
}

void ossimPleiadesDimapSupportData::getSceneOrientation(ossim_float64& so) const
{
   so = theSceneOrientation;
}

void ossimPleiadesDimapSupportData::getRefGroundPoint(ossimGpt& gp) const
{
   gp = theRefGroundPoint;
}

void ossimPleiadesDimapSupportData::getRefImagePoint(ossimDpt& rp) const
{
   rp = theRefImagePoint;
}

void ossimPleiadesDimapSupportData::getRefLineTime(ossim_float64& rt) const
{
   rt = theRefLineTime;
}

void ossimPleiadesDimapSupportData::getRefLineTimeLine(ossim_float64& rtl) const
{
   rtl = theRefLineTimeLine;
}

ossim_uint32 ossimPleiadesDimapSupportData::getNumEphSamples() const
{
   return (ossim_uint32)theEphSampTimes.size();
}

ossim_uint32 ossimPleiadesDimapSupportData::getNumAttSamples() const
{
   return (ossim_uint32)theAttSampTimes.size();
}

ossim_uint32 ossimPleiadesDimapSupportData::getNumGeoPosPoints() const
{
   return (ossim_uint32)theGeoPosImagePoints.size();
}

void ossimPleiadesDimapSupportData::getUlCorner(ossimGpt& pt) const
{
   pt = theUlCorner;
}

void ossimPleiadesDimapSupportData::getUrCorner(ossimGpt& pt) const
{
   pt = theUrCorner;
}

void ossimPleiadesDimapSupportData::getLrCorner(ossimGpt& pt) const
{
   pt = theLrCorner;
}

void ossimPleiadesDimapSupportData::getLlCorner(ossimGpt& pt) const
{
   pt = theLlCorner;
}

void ossimPleiadesDimapSupportData::getImageRect(ossimDrect& rect)const
{
   rect = ossimDrect(0.0, 0.0, theImageSize.x-1.0, theImageSize.y-1.0);
}

void ossimPleiadesDimapSupportData::getSubImageOffset(ossimDpt& offset) const
{
   offset = theSubImageOffset;
}

bool ossimPleiadesDimapSupportData::saveState(ossimKeywordlist& kwl,
                                          const char* prefix)const
{
  ossimString tempString;
  ossim_uint32 idx = 0;

   kwl.add(prefix,
           ossimKeywordNames::TYPE_KW,
           "ossimPleiadesDimapSupportData",
           true);

   kwl.add(prefix,
           "metadata_file",
           theMetadataFile,
           true);

   tempString = "";
   for(idx = 0; idx < theSunAzimuth.size(); ++idx)
   {
      tempString += (ossimString::toString(theSunAzimuth[idx]) + " ");
   }

   kwl.add(prefix,
           ossimKeywordNames::AZIMUTH_ANGLE_KW,
           tempString,
           true);

   kwl.add(prefix,
           "number_of_azimuth_angles",
           static_cast<ossim_uint32>(theSunAzimuth.size()),
           true);

   tempString = "";
   for(idx = 0; idx <  theSunElevation.size(); ++idx)
   {
      tempString += (ossimString::toString(theSunElevation[idx]) + " ");
   }

   kwl.add(prefix,
           ossimKeywordNames::ELEVATION_ANGLE_KW,
           tempString,
           true);

   kwl.add(prefix,
           "number_of_elevation_angles",
           static_cast<ossim_uint32>(theSunElevation.size()),
           true);

   //---
   // Note: since this is a new keyword, use the point.toString as there is
   // no backwards compatibility issues.
   //---
   kwl.add(prefix,
           "detector_count",
           theDetectorCount,
           true);

   kwl.add(prefix,
           "image_size",
           ossimString::toString(theImageSize.x) + " " +
           ossimString::toString(theImageSize.y),
           true);

   kwl.add(prefix,
           "reference_ground_point",
           ossimString::toString(theRefGroundPoint.latd()) + " " +
           ossimString::toString(theRefGroundPoint.lond()) + " " +
           ossimString::toString(theRefGroundPoint.height()) + " " +
           theRefGroundPoint.datum()->code(),
           true);

   kwl.add(prefix,
           "reference_image_point",
           ossimString::toString(theRefImagePoint.x) + " " +
           ossimString::toString(theRefImagePoint.y),
           true);

   kwl.add(prefix,
           "sub_image_offset",
           ossimString::toString(theSubImageOffset.x) + " " +
           ossimString::toString(theSubImageOffset.y),
           true);

   kwl.add(prefix,
           "reference_line_time",
           theRefLineTime,
           true);

   kwl.add(prefix,
           "reference_line_time_line",
           theRefLineTimeLine,
           true);

   kwl.add(prefix,
           "line_sampling_period",
           theLineSamplingPeriod,
           true);



   tempString = "";
   for(idx = 0; idx < thePixelLookAngleX.size(); ++idx)
   {
      tempString += (ossimString::toString(thePixelLookAngleX[idx]) + " ");
   }

   kwl.add(prefix,
           "pixel_lookat_angle_x",
           tempString,
           true);

   kwl.add(prefix,
           "number_of_pixel_lookat_angle_x",
           static_cast<ossim_uint32>(thePixelLookAngleX.size()),
           true);

   tempString = "";
   for(idx = 0; idx < thePixelLookAngleY.size(); ++idx)
   {
      tempString += (ossimString::toString(thePixelLookAngleY[idx]) + " ");
   }
   kwl.add(prefix,
           "pixel_lookat_angle_y",
           tempString,
           true);
   kwl.add(prefix,
           "number_of_pixel_lookat_angle_y",
           static_cast<ossim_uint32>(thePixelLookAngleY.size()),
           true);


   tempString = "";
   for(idx = 0; idx < theAttitudeSamples.size(); ++idx)
   {
      tempString += (ossimString::toString(theAttitudeSamples[idx].x) + " " +
                     ossimString::toString(theAttitudeSamples[idx].y) + " " +
                     ossimString::toString(theAttitudeSamples[idx].z) + " ");
   }
   kwl.add(prefix,
           "attitude_samples",
           tempString,
           true);
   kwl.add(prefix,
           "number_of_attitude_samples",
           static_cast<ossim_uint32>(theAttitudeSamples.size()),
           true);

   tempString = "";
   for(idx = 0; idx < theAttSampTimes.size(); ++idx)
   {
      tempString += (ossimString::toString(theAttSampTimes[idx]) + " ");
   }
   kwl.add(prefix,
           "attitude_sample_times",
           tempString,
           true);
   kwl.add(prefix,
           "number_of_attitude_sample_times",
           static_cast<ossim_uint32>(theAttSampTimes.size()),
           true);

   tempString = "";
   for(idx = 0; idx < thePosEcfSamples.size(); ++idx)
   {
      tempString += (ossimString::toString(thePosEcfSamples[idx].x) + " " +
                     ossimString::toString(thePosEcfSamples[idx].y) + " " +
                     ossimString::toString(thePosEcfSamples[idx].z) + " ");
   }
   kwl.add(prefix,
           "position_ecf_samples",
           tempString,
           true);
   kwl.add(prefix,
           "number_of_position_ecf_samples",
           static_cast<ossim_uint32>(thePosEcfSamples.size()),
           true);

   tempString = "";
   for(idx = 0; idx < theVelEcfSamples.size(); ++idx)
   {
      tempString += (ossimString::toString(theVelEcfSamples[idx].x) + " " +
                     ossimString::toString(theVelEcfSamples[idx].y) + " " +
                     ossimString::toString(theVelEcfSamples[idx].z) + " ");
   }
   kwl.add(prefix,
           "velocity_ecf_samples",
           tempString,
           true);
   kwl.add(prefix,
           "number_of_velocity_ecf_samples",
           static_cast<ossim_uint32>(thePosEcfSamples.size()),
           true);

   tempString = "";
   for(idx = 0; idx < theEphSampTimes.size(); ++idx)
   {
      tempString += (ossimString::toString(theEphSampTimes[idx]) + " ");
   }

   kwl.add(prefix,
           "ephemeris_sample_times",
           tempString,
           true);
   kwl.add(prefix,
           "number_of_ephemeris_sample_times",
           static_cast<ossim_uint32>(theEphSampTimes.size()),
           true);

   kwl.add(prefix,
           "star_tracker_used_flag",
           static_cast<ossim_uint32>(theStarTrackerUsed),
           true);

   kwl.add(prefix,
           ossimKeywordNames::NUMBER_BANDS_KW,
           theNumBands,
           true);

   kwl.add(prefix,
           "image_id",
           theImageID,
           true);

   kwl.add(prefix,
           "instrument",
           theInstrument,
           true);

   kwl.add(prefix,
           "instrument_index",
           theInstrumentIndex,
           true);

   kwl.add(prefix,
           ossimKeywordNames::IMAGE_DATE_KW,
           theAcquisitionDate,
           true);

   kwl.add(prefix,
           "production_date",
           theProductionDate,
           true);

   tempString = "";
   for(idx = 0; idx <  theIncidenceAngle.size(); ++idx)
   {
      tempString += (ossimString::toString(theIncidenceAngle[idx]) + " ");
   }

   kwl.add(prefix,
           "incident_angle",
           tempString,
           true);

   kwl.add(prefix,
           "number_of_incident_angles",
           static_cast<ossim_uint32>(theIncidenceAngle.size()),
           true);

   tempString = "";
   for(idx = 0; idx <  theViewingAngle.size(); ++idx)
   {
      tempString += (ossimString::toString(theViewingAngle[idx]) + " ");
   }

   kwl.add(prefix,
           "viewing_angle",
           tempString,
           true);

   kwl.add(prefix,
           "number_of_viewing_angle",
           static_cast<ossim_uint32>(theViewingAngle.size()),
           true);

   kwl.add(prefix,
           "scene_orientation",
           theSceneOrientation,
           true);
                      
   kwl.add(prefix,
           "step_count",
           theStepCount,
           true);
           
   kwl.add(prefix,
           "ul_ground_point",
           ossimString::toString(theUlCorner.latd()) + " " +
           ossimString::toString(theUlCorner.lond()) + " " +
           ossimString::toString(theUlCorner.height()) + " " +
           theUlCorner.datum()->code(),
           true);

   kwl.add(prefix,
           "ur_ground_point",
           ossimString::toString(theUrCorner.latd()) + " " +
           ossimString::toString(theUrCorner.lond()) + " " +
           ossimString::toString(theUrCorner.height()) + " " +
           theUrCorner.datum()->code(),
           true);

   kwl.add(prefix,
           "lr_ground_point",
           ossimString::toString(theLrCorner.latd()) + " " +
           ossimString::toString(theLrCorner.lond()) + " " +
           ossimString::toString(theLrCorner.height()) + " " +
           theLrCorner.datum()->code(),
           true);

   kwl.add(prefix,
           "ll_ground_point",
           ossimString::toString(theLlCorner.latd()) + " " +
           ossimString::toString(theLlCorner.lond()) + " " +
           ossimString::toString(theLlCorner.height()) + " " +
           theLlCorner.datum()->code(),
           true);

   kwl.add(prefix,
           "sensorID",
           theSensorID,
           true);


   tempString = "";
   for(idx = 0; idx < thePhysicalBias.size(); ++idx)
   {
     tempString += (ossimString::toString(thePhysicalBias[idx]) + " ");
   }
   kwl.add(prefix,
           "physical_bias",
           tempString,
           true);

   tempString = "";
   for(idx = 0; idx < thePhysicalGain.size(); ++idx)
   {
     tempString += (ossimString::toString(thePhysicalGain[idx]) + " ");
   }
   kwl.add(prefix,
           "physical_gain",
           tempString,
           true);

   tempString = "";
   for(idx = 0; idx < theSolarIrradiance.size(); ++idx)
   {
     tempString += (ossimString::toString(theSolarIrradiance[idx]) + " ");
   }

   kwl.add(prefix,
           "solar_irradiance",
           tempString,
           true);

   return true;
}

bool ossimPleiadesDimapSupportData::loadState(const ossimKeywordlist& kwl,
                                          const char* prefix)
{
   ossim_uint32 idx = 0;
   ossim_uint32 total;
   ossimString tempString;

   clearFields();


   ossimString type = kwl.find(prefix, ossimKeywordNames::TYPE_KW);

   if(type != "ossimPleiadesDimapSupportData")
   {
      return false;
   }
   theMetadataFile = kwl.find(prefix, "metadata_file");

   total =  ossimString(kwl.find(prefix,"number_of_azimuth_angle")).toUInt32();
   theSunAzimuth.resize(total);
   tempString = kwl.find(prefix,ossimKeywordNames::AZIMUTH_ANGLE_KW);
   if(tempString != "")
   {
      std::istringstream in(tempString.string());
      ossimString tempValue;
      for(idx = 0; idx < theSunAzimuth.size();++idx)
      {
         in >> tempValue.string();
         theSunAzimuth[idx] = tempValue.toDouble();
      }
   }

   total =  ossimString(kwl.find(prefix,"number_of_elevation_angle")).toUInt32();
   theSunElevation.resize(total);
   tempString = kwl.find(prefix,ossimKeywordNames::ELEVATION_ANGLE_KW);
   if(tempString != "")
   {
      std::istringstream in(tempString.string());
      ossimString tempValue;
      for(idx = 0; idx < theSunElevation.size();++idx)
      {
         in >> tempValue.string();
         theSunElevation[idx] = tempValue.toDouble();
      }
   }

   const char* lookup = kwl.find(prefix, "detector_count");
   if (lookup)
   {
      theDetectorCount = ossimString(lookup).toUInt32();
   }

   theImageSize      = createDpt(kwl.find(prefix, "image_size"));
   theRefGroundPoint = createGround(kwl.find(prefix, "reference_ground_point"));
   theRefImagePoint  = createDpt(kwl.find(prefix, "reference_image_point"));
   theSubImageOffset = createDpt(kwl.find(prefix, "sub_image_offset"));

   theRefLineTime    = ossimString(kwl.find(prefix, "reference_line_time")).toDouble();

   lookup = kwl.find(prefix, "reference_line_time_line");
   if (lookup)
   {
      theRefLineTimeLine = ossimString(lookup).toDouble();
   }

   theLineSamplingPeriod = ossimString(kwl.find(prefix, "line_sampling_period")).toDouble();


   total =  ossimString(kwl.find(prefix,"number_of_pixel_lookat_angle_x")).toUInt32();
   thePixelLookAngleX.resize(total);
   tempString = kwl.find(prefix,"pixel_lookat_angle_x");
   if(tempString != "")
   {
      std::istringstream in(tempString.string());
      ossimString tempValue;
      for(idx = 0; idx < thePixelLookAngleX.size();++idx)
      {
         in >> tempValue.string();
         thePixelLookAngleX[idx] = tempValue.toDouble();
      }
   }

   total =  ossimString(kwl.find(prefix,"number_of_pixel_lookat_angle_y")).toUInt32();
   thePixelLookAngleY.resize(total);
   tempString = kwl.find(prefix,"pixel_lookat_angle_y");
   if(tempString != "")
   {
      std::istringstream in(tempString.string());
      ossimString tempValue;
      for(idx = 0; idx < thePixelLookAngleY.size();++idx)
      {
         in >> tempValue.string();
         thePixelLookAngleY[idx] = tempValue.toDouble();
      }
   }
   

   total =  ossimString(kwl.find(prefix,"number_of_attitude_samples")).toUInt32();
   theAttitudeSamples.resize(total);
   tempString = kwl.find(prefix,"attitude_samples");
   if(tempString != "")
   {
      std::istringstream in(tempString.string());
      ossimString x, y, z;
      for(idx = 0; idx < theAttitudeSamples.size();++idx)
      {
         in >> x.string() >> y.string() >> z.string();
         theAttitudeSamples[idx] =ossimDpt3d(x.toDouble(), y.toDouble(), z.toDouble());
      }
   }

   total =  ossimString(kwl.find(prefix,"number_of_attitude_sample_times")).toUInt32();
   theAttSampTimes.resize(total);
   tempString = kwl.find(prefix,"attitude_sample_times");
   if(tempString != "")
   {
      std::istringstream in(tempString.string());
      ossimString tempValue;
      for(idx = 0; idx < theAttSampTimes.size();++idx)
      {
         in >> tempValue.string();
         theAttSampTimes[idx] = tempValue.toDouble();
      }
   }

   total =  ossimString(kwl.find(prefix,"number_of_position_ecf_samples")).toUInt32();
   thePosEcfSamples.resize(total);
   tempString = kwl.find(prefix,"position_ecf_samples");
   if(tempString != "")
   {
      std::istringstream in(tempString.string());
      ossimString x, y, z;
      for(idx = 0; idx < thePosEcfSamples.size();++idx)
      {
         in >> x.string() >> y.string() >> z.string();
         thePosEcfSamples[idx] = ossimDpt3d(x.toDouble(), y.toDouble(), z.toDouble());
      }
   }

   total =  ossimString(kwl.find(prefix,"number_of_velocity_ecf_samples")).toUInt32();
   theVelEcfSamples.resize(total);
   tempString = kwl.find(prefix,"velocity_ecf_samples");
   if(tempString != "")
   {
      std::istringstream in(tempString.string());
      ossimString x, y, z;
      for(idx = 0; idx < theVelEcfSamples.size();++idx)
      {
         in >> x.string() >> y.string() >> z.string();
         theVelEcfSamples[idx] = ossimDpt3d(x.toDouble(), y.toDouble(), z.toDouble());
      }
   }

   total =  ossimString(kwl.find(prefix,"number_of_ephemeris_sample_times")).toUInt32();
   theEphSampTimes.resize(total);
   tempString = kwl.find(prefix,"ephemeris_sample_times");
   if(tempString != "")
   {
      std::istringstream in(tempString.string());
      ossimString tempValue;
      for(idx = 0; idx < theEphSampTimes.size();++idx)
      {
         in >> tempValue.string();
         theEphSampTimes[idx] = tempValue.toDouble();
      }
   }

   tempString = "";
   for(idx = 0; idx < theEphSampTimes.size(); ++idx)
   {
      tempString += (ossimString::toString(theEphSampTimes[idx]) + " ");
   }

   theStarTrackerUsed = ossimString(kwl.find(prefix, "star_tracker_used_flag")).toBool();
   theNumBands        = ossimString(kwl.find(prefix, ossimKeywordNames::NUMBER_BANDS_KW)).toUInt32();
   theAcquisitionDate = kwl.find(prefix, ossimKeywordNames::IMAGE_DATE_KW);
   theProductionDate  = kwl.find(prefix, "production_date");
   theImageID         = kwl.find(prefix, "image_id");
   theInstrument      = kwl.find(prefix, "instrument");
   theInstrumentIndex = ossimString(kwl.find(prefix, "instrument_index")).toUInt32();
   theStepCount       = ossimString(kwl.find(prefix, "step_count")).toInt32();
   
   total =  ossimString(kwl.find(prefix,"number_of_incident_angle")).toUInt32();
   theIncidenceAngle.resize(total);
   tempString = kwl.find(prefix,"incident_angle");
   if(tempString != "")
   {
      std::istringstream in(tempString.string());
      ossimString tempValue;
      for(idx = 0; idx < theIncidenceAngle.size();++idx)
      {
         in >> tempValue.string();
         theIncidenceAngle[idx] = tempValue.toDouble();
      }
   }

   total =  ossimString(kwl.find(prefix,"number_of_viewing_angle")).toUInt32();
   theViewingAngle.resize(total);
   tempString = kwl.find(prefix,"viewing_angle");
   if(tempString != "")
   {
      std::istringstream in(tempString.string());
      ossimString tempValue;
      for(idx = 0; idx < theViewingAngle.size();++idx)
      {
         in >> tempValue.string();
         theViewingAngle[idx] = tempValue.toDouble();
      }
   }

   theSceneOrientation= ossimString(kwl.find(prefix, "scene_orientation")).toDouble();
   
   theUlCorner =createGround( kwl.find(prefix, "ul_ground_point"));
   theUrCorner =createGround( kwl.find(prefix, "ur_ground_point"));
   theLrCorner =createGround( kwl.find(prefix, "lr_ground_point"));
   theLlCorner =createGround( kwl.find(prefix, "ll_ground_point"));

   theSensorID = ossimString(kwl.find(prefix, "sensorID"));

   thePhysicalBias.resize(theNumBands);
   tempString = kwl.find(prefix,"physical_bias");
   if(tempString != "")
   {
      std::istringstream in(tempString.string());
      ossimString tempValue;
      for(idx = 0; idx < thePhysicalBias.size();++idx)
      {
         in >> tempValue.string();
         thePhysicalBias[idx] = tempValue.toDouble();
      }
   }

   thePhysicalGain.resize(theNumBands);
   tempString = kwl.find(prefix,"physical_gain");
   if(tempString != "")
   {
      std::istringstream in(tempString.string());
      ossimString tempValue;
      for(idx = 0; idx < thePhysicalGain.size();++idx)
      {
         in >> tempValue.string();
         thePhysicalGain[idx] = tempValue.toDouble();
      }
   }

   theSolarIrradiance.resize(theNumBands);
   tempString = kwl.find(prefix,"solar_irradiance");
   if(tempString != "")
   {
      std::istringstream in(tempString.string());
      ossimString tempValue;
      for(idx = 0; idx < theSolarIrradiance.size();++idx)
      {
         in >> tempValue.string();
         theSolarIrradiance[idx] = tempValue.toDouble();
      }
   }

   return true;
}

ossimGpt ossimPleiadesDimapSupportData::createGround(const ossimString& s)const
{
   std::istringstream in(s.string());
   ossimString lat, lon, height;
   ossimString code;

   in >> lat.string() >> lon.string() >> height.string() >> code.string();

   return ossimGpt(lat.toDouble(),
                   lon.toDouble(),
                   height.toDouble(),
                   ossimDatumFactory::instance()->create(code));

}

ossimDpt ossimPleiadesDimapSupportData::createDpt(const ossimString& s)const
{
   std::istringstream in(s.string());
   ossimString x, y;
   ossimString code;

   in >> x.string() >> y.string();

   return ossimDpt(x.toDouble(), y.toDouble());

}

bool ossimPleiadesDimapSupportData::parsePart1(
   ossimRefPtr<ossimXmlDocument> xmlDocument)
{
   static const char MODULE[] = "ossimPleiadesDimapSupportData::parsePart1";

   ossimString xpath;
   vector<ossimRefPtr<ossimXmlNode> > xml_nodes;


   //---
   // Fetch the ImageSize:
   //---
   xml_nodes.clear();
   xpath = "/Dimap_Document/Raster_Dimensions/NCOLS";
   xmlDocument->findNodes(xpath, xml_nodes);
   if (xml_nodes.size() == 0)
   {
      setErrorStatus();
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " DEBUG:"
            << "\nCould not find: " << xpath
            << std::endl;
      }
      return false;
   }
   theImageSize.samp = xml_nodes[0]->getText().toDouble();

   xml_nodes.clear();
   xpath = "/Dimap_Document/Raster_Dimensions/NROWS";
   xmlDocument->findNodes(xpath, xml_nodes);
   if (xml_nodes.size() == 0)
   {
      setErrorStatus();
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " DEBUG:"
            << "\nCould not find: " << xpath
            << std::endl;
      }
      return false;
   }
   theImageSize.line = xml_nodes[0]->getText().toDouble();

   //---
   // We will make the RefImagePoint the zero base center of the image.  This
   // is used by the ossimSensorModel::worldToLineSample iterative loop as
   // the starting point.  Since the ossimSensorModel does not know of the
   // sub image we make it zero base.
   //---
   theRefImagePoint.line = theImageSize.line / 2.0;
   theRefImagePoint.samp = theImageSize.samp / 2.0;

   xml_nodes.clear();
   xpath = "/Dimap_Document/Data_Strip/Sensor_Configuration/Time_Stamp/SCENE_CENTER_LINE";
   xmlDocument->findNodes(xpath, xml_nodes);
   if (xml_nodes.size() == 0)
   {
      setErrorStatus();
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " DEBUG:"
            << "\nCould not find: " << xpath
            << std::endl;
      }
      return false;
   }

   // Relative to full image frame.
   theRefLineTimeLine = xml_nodes[0]->getText().toDouble() - 1.0;

   // See if there's a sub image offset...
   xml_nodes.clear();
   xpath = "/Dimap_Document/Data_Processing/Regions_Of_Interest/Region_Of_Interest/COL_MIN";
   xmlDocument->findNodes(xpath, xml_nodes);
   if (xml_nodes.size() == 0)
   {
      theSubImageOffset.samp = 0.0;
   }
   else
   {
      theSubImageOffset.samp = xml_nodes[0]->getText().toDouble() - 1.0;
   }

   xml_nodes.clear();
   xpath = "/Dimap_Document/Data_Processing/Regions_Of_Interest/Region_Of_Interest/ROW_MIN";
   xmlDocument->findNodes(xpath, xml_nodes);
   if (xml_nodes.size() == 0)
   {
      theSubImageOffset.line = 0.0;
   }
   else
   {
      theSubImageOffset.line = xml_nodes[0]->getText().toDouble() - 1.0;
   }


   //---
   // Fetch the RefLineTime:
   //---
   xml_nodes.clear();
   xpath = "/Dimap_Document/Data_Strip/Sensor_Configuration/Time_Stamp/SCENE_CENTER_TIME";
   xmlDocument->findNodes(xpath, xml_nodes);
   if (xml_nodes.size() == 0)
   {
      setErrorStatus();
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " DEBUG:"
            << "\nCould not find: " << xpath
            << std::endl;
      }
      return false;
   }
   theAcquisitionDate = xml_nodes[0]->getText();
   convertTimeStamp(theAcquisitionDate, theRefLineTime);

   //---
   // Fetch the ProductionDate:
   //---
   xml_nodes.clear();
   xpath = "/Dimap_Document/Production/DATASET_PRODUCTION_DATE";
   xmlDocument->findNodes(xpath, xml_nodes);
   if (xml_nodes.size() == 0)
   {
      setErrorStatus();
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " DEBUG:"
            << "\nCould not find: " << xpath
            << std::endl;
      }
      return false;
   }
   theProductionDate = xml_nodes[0]->getText();
   
   //---
   // Fetch the Instrument:
   //---
   xml_nodes.clear();
   xpath = "/Dimap_Document/Dataset_Sources/Source_Information/Scene_Source/INSTRUMENT";
   xmlDocument->findNodes(xpath, xml_nodes);
   if (xml_nodes.size() == 0)
   {
      setErrorStatus();
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " DEBUG:"
            << "\nCould not find: " << xpath
            << std::endl;
      }
      return false;
   }
   theInstrument = xml_nodes[0]->getText();

   //---
   // Fetch the Instrument Index:
   //---
   xml_nodes.clear();
   xpath = "/Dimap_Document/Dataset_Sources/Source_Information/Scene_Source/INSTRUMENT_INDEX";
   xmlDocument->findNodes(xpath, xml_nodes);
   if (xml_nodes.size() == 0)
   {
      setErrorStatus();
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " DEBUG:"
            << "\nCould not find: " << xpath
            << std::endl;
      }
      return false;
   }
   theInstrumentIndex = xml_nodes[0]->getText().toUInt32();

   return true;
}

bool ossimPleiadesDimapSupportData::parsePart2(
   ossimRefPtr<ossimXmlDocument> xmlDocument)
{
   static const char MODULE[] = "ossimPleiadesDimapSupportData::parsePart2";

   ossimString xpath;
   std::vector<ossimRefPtr<ossimXmlNode> > xml_nodes;
   std::vector<ossimRefPtr<ossimXmlNode> > sub_nodes;
   std::vector<ossimRefPtr<ossimXmlNode> >::iterator node;
   unsigned int band_index;

   //---
   // Fetch the LineSamplingPeriod:
   //---
   xml_nodes.clear();
   xpath = "/Dimap_Document/Data_Strip/Sensor_Configuration/Time_Stamp/LINE_PERIOD";

   xmlDocument->findNodes(xpath, xml_nodes);
   if (xml_nodes.size() == 0)
   {
      setErrorStatus();
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " DEBUG:"
            << "\nCould not find: " << xpath
            << std::endl;
      }
      return false;
   }
   theLineSamplingPeriod = xml_nodes[0]->getText().toDouble();


   //---
   // Fetch number of bands
   //---
   xml_nodes.clear();
   xpath = "/Dimap_Document/Raster_Dimensions/NBANDS";
   xmlDocument->findNodes(xpath, xml_nodes);
   if (xml_nodes.size() == 0)
   {
      setErrorStatus();
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " DEBUG:"
            << "\nCould not find: " << xpath
            << std::endl;
      }
      return false;
   }
   theNumBands = atoi(xml_nodes[0]->getText());

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " DEBUG:"
         << "\nNumber of bands: " << theNumBands
         << std::endl;

   }

   if (theNumBands == 1)
   {

      band_index = 0;
   }
   else if (theNumBands == 3)
   {
      band_index = 0; // using green band for PSI angles
   }
   else if (theNumBands == 4)
   {
         band_index = 1; // using green band for PSI angles
   }
   else
   {
      setErrorStatus();
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " DEBUG:"
            << "\nBand ERROR!"
            << std::endl;
      }
      return false;
   }

   //---
   // Fetch the PixelLookAngleX and PixelLookAngleY arrays. If MS, then the
   // green band PSI angles are used unless SWIR requested:
   //---
   thePixelLookAngleX.clear();
   xml_nodes.clear();
   xpath = "/Dimap_Document/Data_Strip/Sensor_Configuration/"
      "Instrument_Look_Angles_List/Instrument_Look_Angles/";
   xmlDocument->findNodes(xpath, xml_nodes);
//   if (xml_nodes.size() != theNumBands)
//   {
      if(xml_nodes.size() == 0)
      {
         setErrorStatus();
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << MODULE << " DEBUG:"
               << "\nCould not find: " << xpath
               << std::endl;
         }
         return false;
      }
//       else
//       {
//       }

//   }

   xpath = "Look_Angles_List/Look_Angles/PSI_X";
   sub_nodes.clear();
   xml_nodes[band_index]->findChildNodes(xpath, sub_nodes);

   theDetectorCount = (ossim_uint32)sub_nodes.size();

   if (theMetadataVersion == OSSIM_PLEIADES_METADATA_VERSION_2_0)
   {
      for (ossim_uint32 i=0; i<theDetectorCount; ++i)
      {
         thePixelLookAngleX.push_back(sub_nodes[i]->getText().toDouble());
      }
   }
   else if (sub_nodes.size() != theImageSize.samp)
   {
      // theFullImageSize.samp = sub_nodes.size();
//       if ((theSubImageOffset.samp + theImageSize.samp - 1)<= sub_nodes.size())
//       {
//          ossim_uint32 i = theSubImageOffset.samp;
//          for (ossim_uint32 idx 0; idx<theImageSize.samp; ++idx)
//          {
//             thePixelLookAngleX.push_back(sub_nodes[i]->getText().toDouble());
//             ++i;
//          }
//       }
//       else
//       {
      std::vector<double> tempV(sub_nodes.size());
      double t = 0.0;
      double tempIdx = 0.0;
      double tempIdxFraction = 0.0;
      ossim_int32 idxStart = 0;
      ossim_int32 idxEnd = 0;
      ossim_uint32 idx = 0;
      for(idx = 0; idx < sub_nodes.size();++idx)
      {
         tempV[idx] = sub_nodes[idx]->getText().toDouble();
      }
      for(idx = 0; idx < theImageSize.samp; ++idx)
      {
         t = (double)idx/(double)(theImageSize.samp);
         tempIdx = (sub_nodes.size()-1)*t;
         tempIdxFraction = tempIdx - (ossim_int32)tempIdx;
         idxStart = (ossim_int32)tempIdx;
         idxEnd = (ossim_int32)ceil(tempIdx);
         if(idxEnd >= (ossim_int32)sub_nodes.size())
         {
            idxEnd = (ossim_int32)sub_nodes.size()-1;
         }

         thePixelLookAngleX.push_back(tempV[idxStart] + tempIdxFraction*(tempV[idxEnd] - tempV[idxStart]));
      }
   }
   else
   {
      for (ossim_uint32 i=0; i<theImageSize.samp; ++i)
      {
         thePixelLookAngleX.push_back(sub_nodes[i]->getText().toDouble());
      }
   }

   thePixelLookAngleY.clear();
   xpath = "Look_Angles_List/Look_Angles/PSI_Y";
   sub_nodes.clear();
   xml_nodes[band_index]->findChildNodes(xpath, sub_nodes);

   if (theMetadataVersion == OSSIM_PLEIADES_METADATA_VERSION_2_0)
   {
      for (ossim_uint32 i=0; i<theDetectorCount; ++i)
      {
         thePixelLookAngleY.push_back(sub_nodes[i]->getText().toDouble());
      }
   }
   else if (sub_nodes.size() != theImageSize.samp)
   {
//       if ((theSubImageOffset.samp + theImageSize.samp - 1)<= sub_nodes.size())
//       {
//          ossim_uint32 i = theSubImageOffset.samp;
//          for (ossim_uint32 idx 0; idx<theImageSize.samp; ++idx)
//          {
//             thePixelLookAngleX.push_back(sub_nodes[i]->getText().toDouble());
//             ++i;
//          }
//       }
      std::vector<double> tempV(sub_nodes.size());
      double t = 0.0;
      double tempIdx = 0.0;
      double tempIdxFraction = 0.0;
      ossim_int32 idxStart = 0;
      ossim_int32 idxEnd = 0;
      ossim_uint32 idx = 0;
      for(idx = 0; idx < sub_nodes.size();++idx)
      {
         tempV[idx] = sub_nodes[idx]->getText().toDouble();
      }
      for(idx = 0; idx < theImageSize.samp; ++idx)
      {
         t = (double)idx/(double)(theImageSize.samp-1);
         tempIdx = (sub_nodes.size()-1)*t;
         tempIdxFraction = tempIdx - (ossim_int32)tempIdx;
         idxStart = (ossim_int32)tempIdx;
         idxEnd = (ossim_int32)ceil(tempIdx);
         if(idxEnd >= (ossim_int32)sub_nodes.size())
         {
            idxEnd = (ossim_int32)sub_nodes.size()-1;
         }
         if(idxStart > idxEnd)
         {
            idxStart = idxEnd;
         }
         thePixelLookAngleY.push_back(tempV[idxStart] + tempIdxFraction*(tempV[idxEnd] - tempV[idxStart]));
      }
   }
   else
   {
      for (ossim_uint32 i=0; i<theImageSize.samp; ++i)
      {
         thePixelLookAngleY.push_back(sub_nodes[i]->getText().toDouble());
      }
   }

   //---
   // Fetch the Attitude Samples:
   //---
   theAttitudeSamples.clear();
   theAttSampTimes.clear();
   xml_nodes.clear();
   xpath = "/Dimap_Document/Data_Strip/Satellite_Attitudes/Corrected_Attitudes/"
      "Corrected_Attitude/Angles";
   xmlDocument->findNodes(xpath, xml_nodes);
   if (xml_nodes.size() == 0)
   {
      xpath = "/Dimap_Document/Data_Strip/Satellite_Attitudes/Raw_Attitudes/Aocs_Attitude/Angles_List/Angles";

      xmlDocument->findNodes(xpath, xml_nodes);
      if (xml_nodes.size() == 0)
      {
         setErrorStatus();
         return false;
      }
   }
   node = xml_nodes.begin();
   while (node != xml_nodes.end())
   {
      ossimDpt3d V;
      sub_nodes.clear();
      xpath = "OUT_OF_RANGE";
      (*node)->findChildNodes(xpath, sub_nodes);
      if (sub_nodes.size() == 0)
      {
         setErrorStatus();
         return false;
      }
      if (sub_nodes[0]->getText() == "N")
      {
         sub_nodes.clear();
         xpath = "PITCH";
         (*node)->findChildNodes(xpath, sub_nodes);
         if (sub_nodes.size() == 0)
         {
            setErrorStatus();
            return false;
         }
         V.x = sub_nodes[0]->getText().toDouble();

         sub_nodes.clear();
         xpath = "ROLL";
         (*node)->findChildNodes(xpath, sub_nodes);
         if (sub_nodes.size() == 0)
         {
            setErrorStatus();
            return false;
         }
         V.y = sub_nodes[0]->getText().toDouble();

         sub_nodes.clear();
         xpath = "YAW";
         (*node)->findChildNodes(xpath, sub_nodes);
         if (sub_nodes.size() == 0)
         {
            setErrorStatus();
            return false;
         }
         V.z = sub_nodes[0]->getText().toDouble();

         theAttitudeSamples.push_back(V);

         sub_nodes.clear();
         xpath = "TIME";
         (*node)->findChildNodes(xpath, sub_nodes);
         if (sub_nodes.size() == 0)
         {
            setErrorStatus();
            return false;
         }
         theAttSampTimes.push_back(convertTimeStamp(sub_nodes[0]->getText()));
      }
      ++node;
   }

   return true;
}

bool ossimPleiadesDimapSupportData::parsePart3(
   ossimRefPtr<ossimXmlDocument> xmlDocument)
{
   ossimString xpath;
   std::vector<ossimRefPtr<ossimXmlNode> > xml_nodes;
   std::vector<ossimRefPtr<ossimXmlNode> > sub_nodes;
   std::vector<ossimRefPtr<ossimXmlNode> >::iterator node;

   //---
   // Fetch the ephemeris samples:
   //---
   thePosEcfSamples.clear();
   theVelEcfSamples.clear();
   theEphSampTimes.clear();
   xml_nodes.clear();
   xpath = "/Dimap_Document/Data_Strip/Ephemeris/Points/Point";
   xmlDocument->findNodes(xpath, xml_nodes);
   if (xml_nodes.size() == 0)
   {
      setErrorStatus();
      return false;
   }
   node = xml_nodes.begin();

   while (node != xml_nodes.end())
   {
      ossimDpt3d VP;
      sub_nodes.clear();
      xpath  = "Location/X";
      (*node)->findChildNodes(xpath, sub_nodes);
      if (sub_nodes.size() == 0)
      {
         setErrorStatus();
         return false;
      }
      VP.x = sub_nodes[0]->getText().toDouble();

      sub_nodes.clear();
      xpath  = "Location/Y";
      (*node)->findChildNodes(xpath, sub_nodes);
      if (sub_nodes.size() == 0)
      {
         setErrorStatus();
         return false;
      }
      VP.y = sub_nodes[0]->getText().toDouble();

      sub_nodes.clear();
      xpath  = "Location/Z";
      (*node)->findChildNodes(xpath, sub_nodes);
      if (sub_nodes.size() == 0)
      {
         setErrorStatus();
         return false;
      }
      VP.z = sub_nodes[0]->getText().toDouble();

      thePosEcfSamples.push_back(VP);

      ossimDpt3d VV;
      sub_nodes.clear();
      xpath = "Velocity/X";
      (*node)->findChildNodes(xpath, sub_nodes);
      if (sub_nodes.size() == 0)
      {
         setErrorStatus();
         return false;
      }
      VV.x = sub_nodes[0]->getText().toDouble();

      sub_nodes.clear();
      xpath = "Velocity/Y";
      (*node)->findChildNodes(xpath, sub_nodes);
      if (sub_nodes.size() == 0)
      {
         setErrorStatus();
         return false;
      }
      VV.y = sub_nodes[0]->getText().toDouble();

      sub_nodes.clear();
      xpath = "Velocity/Z";
      (*node)->findChildNodes(xpath, sub_nodes);
      if (sub_nodes.size() == 0)
      {
         setErrorStatus();
         return false;
      }
      VV.z = sub_nodes[0]->getText().toDouble();

      theVelEcfSamples.push_back(VV);

      sub_nodes.clear();
      xpath  = "TIME";
      (*node)->findChildNodes(xpath, sub_nodes);
      if (sub_nodes.size() == 0)
      {
         setErrorStatus();
         return false;
      }
      theEphSampTimes.push_back(convertTimeStamp(sub_nodes[0]->getText()));

      ++node;
   }

   //---
   // Fetch the star tracker flag:
   //---
   xml_nodes.clear();
   xpath = "/Dimap_Document/Data_Strip/Satellite_Attitudes/Corrected_Attitudes/"
      "STAR_TRACKER_USED";
   xmlDocument->findNodes(xpath, xml_nodes);
   if (xml_nodes.size() == 0)
   {
         theStarTrackerUsed = false;
//       setErrorStatus();
//       return false;
   }
   else
   {
      if (xml_nodes[0]->getText() == "Y")
         theStarTrackerUsed = true;
      else
         theStarTrackerUsed = false;
   }

   //---
   // Geoposition points:
   //---
   xml_nodes.clear();
   xpath = "/Dimap_Document/Geoposition/Geoposition_Points/Tie_Point";
   xmlDocument->findNodes(xpath, xml_nodes);
   node = xml_nodes.begin();
   while (node != xml_nodes.end())
   {
      ossimGpt gpt;
      ossimDpt  ipt;

      sub_nodes.clear();
      xpath = "TIE_POINT_DATA_Y";
      (*node)->findChildNodes(xpath, sub_nodes);
      if (sub_nodes.size() == 0)
      {
         setErrorStatus();
         return false;
      }
      ipt.line = sub_nodes[0]->getText().toDouble() - 1.0;

      sub_nodes.clear();
      xpath = "TIE_POINT_DATA_X";
      (*node)->findChildNodes(xpath, sub_nodes);
      if (sub_nodes.size() == 0)
      {
         setErrorStatus();
         return false;
      }
      ipt.samp = sub_nodes[0]->getText().toDouble() - 1.0;

      sub_nodes.clear();
      xpath = "TIE_POINT_CRS_Y";
      (*node)->findChildNodes(xpath, sub_nodes);
      if (sub_nodes.size() == 0)
      {
         setErrorStatus();
         return false;
      }
      gpt.lat = sub_nodes[0]->getText().toDouble();

      sub_nodes.clear();
      xpath = "TIE_POINT_CRS_X";
      (*node)->findChildNodes(xpath, sub_nodes);
      if (sub_nodes.size() == 0)
      {
         setErrorStatus();
         return false;
      }
      gpt.lon = sub_nodes[0]->getText().toDouble();

      sub_nodes.clear();
      xpath = "TIE_POINT_CRS_Z";
      (*node)->findChildNodes(xpath, sub_nodes);
      if (sub_nodes.size() == 0)
      {
         setErrorStatus();
         return false;
      }
      gpt.hgt = sub_nodes[0]->getText().toDouble();

      theGeoPosImagePoints.push_back(ipt);
      theGeoPosGroundPoints.push_back(gpt);

      ++node;
   }

   return true;
}


bool ossimPleiadesDimapSupportData::initMetadataVersion(ossimRefPtr<ossimXmlDocument> xmlDocument)
{
   ossimString xpath;
   std::vector<ossimRefPtr<ossimXmlNode> > xml_nodes;

   //---
   // Get the version string which can be used as a key for parsing.
   //---
   xpath = "/DIMAP_Document/Metadata_Identification/METADATA_FORMAT";
   xmlDocument->findNodes(xpath, xml_nodes);
   if (xml_nodes.size() == 0)
   {
      setErrorStatus();
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG:\nCould not find: " << xpath
            << endl;
      }
      return false;
   }

   ossimString attribute = "version";
   ossimString value;
   xml_nodes[0]->getAttributeValue(value, attribute);
   if (value == "2.0")
   {
      theMetadataVersion = OSSIM_PLEIADES_METADATA_VERSION_2_0;
   }

   if (theMetadataVersion == OSSIM_PLEIADES_METADATA_VERSION_UNKNOWN)
   {
      setErrorStatus();
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "WARNING:  metadata version not found!"
            << std::endl;
      }
      return false;
   }
   return true;
}

bool ossimPleiadesDimapSupportData::initImageId(
   ossimRefPtr<ossimXmlDocument> xmlDocument)
{
   ossimString xpath;
   vector<ossimRefPtr<ossimXmlNode> > xml_nodes;

   //---
   // Fetch the Image ID:
   //---
   xpath = "/DIMAP_Document/Product_Information/Delivery_Identification/JOB_ID";
   xmlDocument->findNodes(xpath, xml_nodes);
   if (xml_nodes.size() == 0)
   {
      setErrorStatus();
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG:\nCould not find: " << xpath
            << endl;
      }
      return false;
   }
   theImageID = xml_nodes[0]->getText();
   return true;
}

bool ossimPleiadesDimapSupportData::initSceneSource(
   ossimRefPtr<ossimXmlDocument> xmlDocument)
{
   ossimString xpath;
   vector<ossimRefPtr<ossimXmlNode> > xml_nodes;

  //---
  // Fetch the mission index (1A ou 1B) ?
  // and generate theSensorID
  //---
   xml_nodes.clear();
   xpath = "/DIMAP_Document/Dataset_Sources/Source_Identification/Strip_Source/MISSION_INDEX";
   xmlDocument->findNodes(xpath, xml_nodes);
   if (xml_nodes.size() == 0)
   {
     setErrorStatus();
     if(traceDebug())
       {
	 ossimNotify(ossimNotifyLevel_DEBUG)
	   << "DEBUG:\nCould not find: " << xpath
	   << std::endl;
       }
     return false;
   }
   if (xml_nodes[0]->getText() == "1A")
     theSensorID = "PHR 1A";
   if (xml_nodes[0]->getText() == "1B")
     theSensorID = "PHR 1B";


   xml_nodes.clear();
   xpath = "/DIMAP_Document/Geometric_Data/Use_Area/Located_Geometric_Values";
   xmlDocument->findNodes(xpath, xml_nodes);
   if (xml_nodes.size() != 3 )
      {
         setErrorStatus();
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "DEBUG:\nCould not find: " << xpath
               << std::endl;
         }
         return false;
      }

   std::vector<ossimRefPtr<ossimXmlNode> >::iterator node = xml_nodes.begin();
   while (node != xml_nodes.end())
     {
     std::vector<ossimRefPtr<ossimXmlNode> > sub_nodes;

     //---
     // Fetch the Sun Azimuth:
     //---
     xpath = "Solar_Incidences/SUN_AZIMUTH";
     (*node)->findChildNodes(xpath, sub_nodes);
     if (sub_nodes.size() == 0)
     {
        setErrorStatus();
        if(traceDebug())
        {
           ossimNotify(ossimNotifyLevel_DEBUG)
              << "DEBUG:\nCould not find: " << xpath
              << std::endl;
        }
        return false;
     }
     theSunAzimuth.push_back(sub_nodes[0]->getText().toDouble());

     //---
     // Fetch the Sun Elevation:
     //---
     sub_nodes.clear();
     xpath = "Solar_Incidences/SUN_ELEVATION";
     (*node)->findChildNodes(xpath, sub_nodes);
     if (sub_nodes.size() == 0)
     {
        setErrorStatus();
        if(traceDebug())
        {
           ossimNotify(ossimNotifyLevel_DEBUG)
              << "DEBUG:\nCould not find: " << xpath
              << std::endl;
        }
        return false;
     }
     theSunElevation.push_back(sub_nodes[0]->getText().toDouble());

     //---
     // Fetch incidence angle:
     //---
     sub_nodes.clear();
     xpath = "Acquisition_Angles/INCIDENCE_ANGLE";
     (*node)->findChildNodes(xpath, sub_nodes);
     if (sub_nodes.size() == 0)
       {
       setErrorStatus();
       if(traceDebug())
         {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG:\nCould not find: " << xpath
            << std::endl;
         }
       return false;
       }
     theIncidenceAngle.push_back(sub_nodes[0]->getText().toDouble());

     sub_nodes.clear();
     xpath = "Acquisition_Angles/VIEWING_ANGLE";
     (*node)->findChildNodes(xpath, sub_nodes);
     if (sub_nodes.size() == 0)
       {
       setErrorStatus();
       if(traceDebug())
         {
         ossimNotify(ossimNotifyLevel_DEBUG)
              << "DEBUG:\nCould not find: " << xpath
              << std::endl;
         }
         return false;
       }
     theViewingAngle.push_back(sub_nodes[0]->getText().toDouble());

     ++node;
     }
   
   return true;
}

bool ossimPleiadesDimapSupportData::initFramePoints(
   ossimRefPtr<ossimXmlDocument> xmlDocument)
{
   ossimString xpath;
   vector<ossimRefPtr<ossimXmlNode> > xml_nodes;

   //---
   // Corner points:
   //---
   xml_nodes.clear();
   xpath = "/DIMAP_Document/Dataset_Content/Dataset_Extent/Vertex";
   xmlDocument->findNodes(xpath, xml_nodes);
   if (xml_nodes.size() != 4)
   {
      setErrorStatus();
      return false;
   }
   std::vector<ossimRefPtr<ossimXmlNode> >::iterator node = xml_nodes.begin();
   while (node != xml_nodes.end())
   {
      ossimGpt gpt;
      ossimDpt ipt;

      std::vector<ossimRefPtr<ossimXmlNode> > sub_nodes;
      xpath = "LAT";
      (*node)->findChildNodes(xpath, sub_nodes);
      if (sub_nodes.size() == 0)
      {
         setErrorStatus();
         return false;
      }
      gpt.lat = sub_nodes[0]->getText().toDouble();

      sub_nodes.clear();
      xpath = "LON";
      (*node)->findChildNodes(xpath, sub_nodes);
      if (sub_nodes.size() == 0)
      {
         setErrorStatus();
         return false;
      }
      gpt.lon = sub_nodes[0]->getText().toDouble();
      gpt.hgt = 0.0; // assumed

      sub_nodes.clear();
      xpath = "ROW";
      (*node)->findChildNodes(xpath, sub_nodes);
      if (sub_nodes.size() == 0)
      {
         setErrorStatus();
         return false;
      }
      ipt.line = sub_nodes[0]->getText().toDouble() - 1.0;

      sub_nodes.clear();
      xpath = "COL";
      (*node)->findChildNodes(xpath, sub_nodes);
      if (sub_nodes.size() == 0)
      {
         setErrorStatus();
         return false;
      }
      ipt.samp = sub_nodes[0]->getText().toDouble() - 1.0;

      if (ipt.line < 1.0)
         if (ipt.samp < 1.0)
            theUlCorner = gpt;
         else
            theUrCorner = gpt;
      else
         if (ipt.samp < 1.0)
            theLlCorner = gpt;
         else
            theLrCorner = gpt;

      ++node;
   }

   //---
   // Center of frame.
   //---
   theRefGroundPoint.hgt = 0.0; // needs to be looked up

   xml_nodes.clear();
   xpath = "/DIMAP_Document/Dataset_Content/Dataset_Extent/Center/LON";
   xmlDocument->findNodes(xpath, xml_nodes);
   if (xml_nodes.size() != 1)
   {
      setErrorStatus();
      return false;
   }
   theRefGroundPoint.lon = xml_nodes[0]->getText().toDouble();

   xml_nodes.clear();
   xpath = "/DIMAP_Document/Dataset_Content/Dataset_Extent/Center/LAT";
   xmlDocument->findNodes(xpath, xml_nodes);
   if (xml_nodes.size() != 1)
   {
      setErrorStatus();
      return false;
   }
   theRefGroundPoint.lat = xml_nodes[0]->getText().toDouble();

  
   //---
   // Fetch scene orientation:
   //---
/*   xml_nodes.clear();
   xpath = "/Dimap_Document/Dataset_Frame/SCENE_ORIENTATION";
   xmlDocument->findNodes(xpath, xml_nodes);
   if (xml_nodes.size() == 0)
   {
      setErrorStatus();
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG:\nCould not find: " << xpath
            << std::endl;
      }
      return false;
   }
   theSceneOrientation = xml_nodes[0]->getText().toDouble();  */


   return true;
}


bool ossimPleiadesDimapSupportData::parseCommonMetadata(
   ossimRefPtr<ossimXmlDocument> xmlDocument)
{
   static const char MODULE[] = "ossimPleiadesDimapSupportData::parseXMLDocument";

   ossimString xpath;
   vector<ossimRefPtr<ossimXmlNode> > xml_nodes;

   //---
   // Fetch if the DIMAP is linked to one or many JP2 files:
   //---
   xml_nodes.clear();
   xpath = "/DIMAP_Document/Raster_Data/Data_Access/DATA_FILE_TILES";
   xmlDocument->findNodes(xpath, xml_nodes);
   if (xml_nodes.size() == 0)
   {
      setErrorStatus();
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " DEBUG:"
            << "\nCould not find: " << xpath
            << std::endl;
      }
      return false;
   }
   theMultiDataFile.setValue(xml_nodes[0]->getText());


   //---
   // Fetch the MegaImageSize:
   //---
   xml_nodes.clear();
   xpath = "/DIMAP_Document/Raster_Data/Raster_Dimensions/NCOLS";
   xmlDocument->findNodes(xpath, xml_nodes);
   if (xml_nodes.size() == 0)
   {
      setErrorStatus();
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " DEBUG:"
            << "\nCould not find: " << xpath
            << std::endl;
      }
      return false;
   }
   theImageSize.samp = xml_nodes[0]->getText().toDouble();

   xml_nodes.clear();
   xpath = "/DIMAP_Document/Raster_Data/Raster_Dimensions/NROWS";
   xmlDocument->findNodes(xpath, xml_nodes);
   if (xml_nodes.size() == 0)
   {
      setErrorStatus();
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " DEBUG:"
            << "\nCould not find: " << xpath
            << std::endl;
      }
      return false;
   }
   theImageSize.line = xml_nodes[0]->getText().toDouble();


   if (theMultiDataFile.getBoolean())
    {
    //---
    // Fetch the Number of MegaTiles:
    //---
    xml_nodes.clear();
    xpath = "/DIMAP_Document/Raster_Data/Raster_Dimensions/Tile_Set/NTILES";
    xmlDocument->findNodes(xpath, xml_nodes);
    if (xml_nodes.size() == 0)
      {
      setErrorStatus();
      if (traceDebug())
        {
        ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " DEBUG:" << "\nCould not find: " << xpath << std::endl;
        }
      return false;
      }
    theNumberOfMegaTiles = xml_nodes[0]->getText().toUInt32();

    //---
    // Fetch the Number of MegaTiles in X and Y:
    //---
    xml_nodes.clear();
    xpath = "/DIMAP_Document/Raster_Data/Raster_Dimensions/Tile_Set/Regular_Tiling/NTILES_COUNT";
    xmlDocument->findNodes(xpath, xml_nodes);
    if (xml_nodes.size() == 0)
      {
      setErrorStatus();
      if (traceDebug())
        {
        ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " DEBUG:" << "\nCould not find: " << xpath << std::endl;
        }
      return false;
      }

    ossimString attribute = "ntiles_x";
    ossimString value;
    xml_nodes[0]->getAttributeValue(value, attribute);
    theNumberOfMegaTilesInRow = value.toUInt32();

    attribute = "ntiles_y";
    xml_nodes[0]->getAttributeValue(value, attribute);
    theNumberOfMegaTilesInCol = value.toUInt32();

    if (theNumberOfMegaTilesInRow * theNumberOfMegaTilesInCol != theNumberOfMegaTiles)
      {
      setErrorStatus();
      if (traceDebug())
        {
        ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " DEBUG:" << "\nCould not find: " << xpath << std::endl;
        }
      return false;
      }

    //---
    // Fetch the size of MegaTiles:
    //---
    xml_nodes.clear();
    xpath = "/DIMAP_Document/Raster_Data/Raster_Dimensions/Tile_Set/Regular_Tiling/NTILES_SIZE";
    xmlDocument->findNodes(xpath, xml_nodes);
    if (xml_nodes.size() == 0)
      {
      setErrorStatus();
      if (traceDebug())
        {
        ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " DEBUG:" << "\nCould not find: " << xpath << std::endl;
        }
      return false;
      }

    attribute = "nrows";
    xml_nodes[0]->getAttributeValue(value, attribute);
    theTileSize.line = value.toUInt32();

    attribute = "ncols";
    xml_nodes[0]->getAttributeValue(value, attribute);
    theTileSize.samp = value.toUInt32();

    }

   //--- TODO_MSD
   // We will make the RefImagePoint the zero base center of the image.  This
   // is used by the ossimSensorModel::worldToLineSample iterative loop as
   // the starting point.  Since the ossimSensorModel does not know of the
   // sub image we make it zero base.
   //---
   theRefImagePoint.line = theImageSize.line / 2.0;
   theRefImagePoint.samp = theImageSize.samp / 2.0;



   //---
   // Fetch the ProductionDate:
   //---
   xml_nodes.clear();
   xpath = "/DIMAP_Document/Product_Information/Delivery_Identification/PRODUCTION_DATE";
   xmlDocument->findNodes(xpath, xml_nodes);
   if (xml_nodes.size() == 0)
   {
      setErrorStatus();
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " DEBUG:"
            << "\nCould not find: " << xpath
            << std::endl;
      }
      return false;
   }
   theProductionDate = xml_nodes[0]->getText();

   //---
   // Fetch the Instrument:
   //---
   xml_nodes.clear();
   xpath = "/DIMAP_Document/Dataset_Sources/Source_Identification/Strip_Source/INSTRUMENT";
   xmlDocument->findNodes(xpath, xml_nodes);
   if (xml_nodes.size() == 0)
   {
      setErrorStatus();
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " DEBUG:"
            << "\nCould not find: " << xpath
            << std::endl;
      }
      return false;
   }
   theInstrument = xml_nodes[0]->getText();

   //---
   // Fetch the Instrument Index:
   //---
   xml_nodes.clear();
   xpath = "/DIMAP_Document/Dataset_Sources/Source_Identification/Strip_Source/INSTRUMENT_INDEX";
   xmlDocument->findNodes(xpath, xml_nodes);
   if (xml_nodes.size() == 0)
   {
      setErrorStatus();
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " DEBUG:"
            << "\nCould not find: " << xpath
            << std::endl;
      }
      return false;
   }
   theInstrumentIndex = xml_nodes[0]->getText().toUInt32();

  //---
  // Fetch number of bands
  //---
  xml_nodes.clear();
  xpath = "/DIMAP_Document/Raster_Data/Raster_Dimensions/NBANDS";
  xmlDocument->findNodes(xpath, xml_nodes);
  if (xml_nodes.size() == 0)
    {
    setErrorStatus();
    if (traceDebug())
      {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " DEBUG:" << "\nCould not find: " << xpath << std::endl;
      }
    return false;
    }
  theNumBands = atoi(xml_nodes[0]->getText());

  if (traceDebug())
    {
    ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " DEBUG:" << "\nNumber of bands: " << theNumBands << std::endl;
    }

  if (traceDebug())
    {
    ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " DEBUG:" << "\nNumber of bands: " << theNumBands << std::endl;
    }

  //---
  // Fetch Band Dispaly Order
  //---
  xml_nodes.clear();
  xpath = "/DIMAP_Document/Raster_Data/Raster_Display/Band_Display_Order/RED_CHANNEL";
  xmlDocument->findNodes(xpath, xml_nodes);
  if (xml_nodes.size() == 0)
    {
    setErrorStatus();
    if (traceDebug())
      {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " DEBUG:" << "\nCould not find: " << xpath << std::endl;
      }
    return false;
    }
  theBandOrder.push_back(xml_nodes[0]->getText());

  if (theNumBands > 1)
    {
  xml_nodes.clear();
  xpath = "/DIMAP_Document/Raster_Data/Raster_Display/Band_Display_Order/GREEN_CHANNEL";
  xmlDocument->findNodes(xpath, xml_nodes);
  if (xml_nodes.size() == 0)
    {
    setErrorStatus();
    if (traceDebug())
      {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " DEBUG:" << "\nCould not find: " << xpath << std::endl;
      }
    return false;
    }
  theBandOrder.push_back(xml_nodes[0]->getText());

  xml_nodes.clear();
  xpath = "/DIMAP_Document/Raster_Data/Raster_Display/Band_Display_Order/BLUE_CHANNEL";
  xmlDocument->findNodes(xpath, xml_nodes);
  if (xml_nodes.size() == 0)
    {
    setErrorStatus();
    if (traceDebug())
      {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " DEBUG:" << "\nCould not find: " << xpath << std::endl;
      }
    return false;
    }
  theBandOrder.push_back(xml_nodes[0]->getText());

  if (theNumBands > 3)
    {
    xml_nodes.clear();
    xpath = "/DIMAP_Document/Raster_Data/Raster_Display/Band_Display_Order/ALPHA_CHANNEL";
    xmlDocument->findNodes(xpath, xml_nodes);
    if (xml_nodes.size() == 0)
      {
      setErrorStatus();
      if (traceDebug())
        {
        ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " DEBUG:" << "\nCould not find: " << xpath << std::endl;
        }
      return false;
      }
    theBandOrder.push_back(xml_nodes[0]->getText());
    }

    }

  //---
  // Fetch the Processing Level:
  //---
  xml_nodes.clear();
  xpath = "/DIMAP_Document/Processing_Information/Product_Settings/PROCESSING_LEVEL";
  xmlDocument->findNodes(xpath, xml_nodes);
  if (xml_nodes.size() == 0)
  {
     setErrorStatus();
     if(traceDebug())
     {
        ossimNotify(ossimNotifyLevel_DEBUG)
           << MODULE << " DEBUG:"
           << "\nCould not find: " << xpath
           << std::endl;
     }
     return false;
  }
  theProcessingLevelString = xml_nodes[0]->getText();

  //---
  // Fetch the Spectral Processing:
  //---
  xml_nodes.clear();
  xpath = "/DIMAP_Document/Processing_Information/Product_Settings/SPECTRAL_PROCESSING";
  xmlDocument->findNodes(xpath, xml_nodes);
  if (xml_nodes.size() == 0)
  {
     setErrorStatus();
     if(traceDebug())
     {
        ossimNotify(ossimNotifyLevel_DEBUG)
           << MODULE << " DEBUG:"
           << "\nCould not find: " << xpath
           << std::endl;
     }
     return false;
  }
  theSpectralProcessingString = xml_nodes[0]->getText();

  //---
  // Fetch the Imaging Date:
  //---
  xml_nodes.clear();
  xpath = "/DIMAP_Document/Dataset_Sources/Source_Identification/Strip_Source/IMAGING_DATE";
  xmlDocument->findNodes(xpath, xml_nodes);
  if (xml_nodes.size() == 0)
  {
     setErrorStatus();
     if(traceDebug())
     {
        ossimNotify(ossimNotifyLevel_DEBUG)
           << MODULE << " DEBUG:"
           << "\nCould not find: " << xpath
           << std::endl;
     }
     return false;
  }
   theFirstLineImagingDate= xml_nodes[0]->getText();

   //---
   // Fetch the Imaging Time:
   //---
   xml_nodes.clear();
   xpath = "/DIMAP_Document/Dataset_Sources/Source_Identification/Strip_Source/IMAGING_TIME";
   xmlDocument->findNodes(xpath, xml_nodes);
   if (xml_nodes.size() == 0)
   {
      setErrorStatus();
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " DEBUG:"
            << "\nCould not find: " << xpath
            << std::endl;
      }
      return false;
   }
   theFirstLineImagingTime= xml_nodes[0]->getText();

   theAcquisitionDate = theFirstLineImagingDate + "T" + theFirstLineImagingTime;

   return true;
}

bool ossimPleiadesDimapSupportData::parseRadiometricMetadata(
   ossimRefPtr<ossimXmlDocument> xmlDocument)
{
  ossimString xpath;
  std::vector<ossimRefPtr<ossimXmlNode> > xml_nodes;
  std::vector<ossimRefPtr<ossimXmlNode> > sub_nodes;
  std::vector<ossimRefPtr<ossimXmlNode> >::iterator node;

  //---
  // Fetch the gain and bias for each spectral band:
  //---
  thePhysicalGain.assign(theNumBands, 1.000);
  thePhysicalBias.assign(theNumBands, 0.000);

  xml_nodes.clear();
  xpath = "/DIMAP_Document/Radiometric_Data/Radiometric_Calibration/Instrument_Calibration/Band_Measurement_List/Band_Radiance";
  xmlDocument->findNodes(xpath, xml_nodes);
  node = xml_nodes.begin();
  while (node != xml_nodes.end())
    {
    sub_nodes.clear();
    xpath = "BAND_ID";
    (*node)->findChildNodes(xpath, sub_nodes);
    if (sub_nodes.size() == 0)
      {
      setErrorStatus();
      return false;
      }

    ossimString bandName = sub_nodes[0]->getText();
    ossim_uint32 bandIndex;
    if (bandName == "B0")
      bandIndex = 0;
    else
      if (bandName == "B1")
        bandIndex = 1;
      else
        if (bandName == "B2")
          bandIndex = 2;
        else
          if (bandName == "B3")
            bandIndex = 3;
          else
            if (bandName == "P") bandIndex = 0;
            else
              {
              ossimNotify(ossimNotifyLevel_WARN)
                        << "ossimPleiadesDimapSupportData::parseRadiometricMetadata ERROR: Band ID is incorrect\n";
              }

    if ((bandIndex >= theNumBands ) )
      {
      ossimNotify(ossimNotifyLevel_WARN)
          << "ossimPleiadesDimapSupportData::parseRadiometricMetadata ERROR: Band index outside of range\n";
      return false;
      }

    sub_nodes.clear();
    xpath = "BIAS";
    (*node)->findChildNodes(xpath, sub_nodes);
    if (sub_nodes.size() == 0)
      {
      setErrorStatus();
      return false;
      }
    thePhysicalBias[bandIndex] = sub_nodes[0]->getText().toDouble();

    sub_nodes.clear();
    xpath = "GAIN";
    (*node)->findChildNodes(xpath, sub_nodes);
    if (sub_nodes.size() == 0)
      {
      setErrorStatus();
      return false;
      }
    thePhysicalGain[bandIndex] = sub_nodes[0]->getText().toDouble();

    ++node;
    }

  theSolarIrradiance.assign(theNumBands, 0.000);
  xml_nodes.clear();
  xpath = "/DIMAP_Document/Radiometric_Data/Radiometric_Calibration/Instrument_Calibration/Band_Measurement_List/Band_Solar_Irradiance";
  xmlDocument->findNodes(xpath, xml_nodes);
  node = xml_nodes.begin();
  while (node != xml_nodes.end())
    {
    sub_nodes.clear();
    xpath = "BAND_ID";
    (*node)->findChildNodes(xpath, sub_nodes);
    if (sub_nodes.size() == 0)
      {
      setErrorStatus();
      return false;
      }

    ossimString bandName = sub_nodes[0]->getText();
    ossim_uint32 bandIndex;
    if (bandName == "B0")
      bandIndex = 0;
    else
      if (bandName == "B1")
        bandIndex = 1;
      else
        if (bandName == "B2")
          bandIndex = 2;
        else
          if (bandName == "B3")
            bandIndex = 3;
          else
            if (bandName == "P")
              bandIndex = 0;
            else
              {
              ossimNotify(ossimNotifyLevel_WARN)
                  << "ossimPleiadesDimapSupportData::parseRadiometricMetadata ERROR: Band ID is incorrect\n";
              }

    if ((bandIndex >= theNumBands))
      {
      ossimNotify(ossimNotifyLevel_WARN)
          << "ossimPleiadesDimapSupportData::parseRadiometricMetadata ERROR: Band index outside of range\n";
      return false;
      }

    sub_nodes.clear();
    xpath = "VALUE";
    (*node)->findChildNodes(xpath, sub_nodes);
    if (sub_nodes.size() == 0)
      {
      setErrorStatus();
      return false;
      }
    theSolarIrradiance[bandIndex] = sub_nodes[0]->getText().toDouble();

    ++node;
    }

  return true;
}


bool ossimPleiadesDimapSupportData::parseSensorMetadata(
   ossimRefPtr<ossimXmlDocument> xmlDocument)
{
  static const char MODULE[] = "ossimPleiadesDimapSupportData::parseSensorMetadata";

  ossimString xpath;
  vector<ossimRefPtr<ossimXmlNode> > xml_nodes;

  //---
  // Fetch the RPC file:
  //---
  xml_nodes.clear();
  xpath = "/DIMAP_Document/Geoposition/Geoposition_Models/Rational_Function_Model/Component/COMPONENT_PATH";
  xmlDocument->findNodes(xpath, xml_nodes);
  if (xml_nodes.size() == 0)
  {
     setErrorStatus();
     if(traceDebug())
     {
        ossimNotify(ossimNotifyLevel_DEBUG)
           << MODULE << " DEBUG:"
           << "\nCould not find: " << xpath
           << std::endl;
     }
     return false;
  }

  ossimString attribute = "href";
  xml_nodes[0]->getAttributeValue(theRPCModelFile, attribute);

  ossimRefPtr<ossimXmlDocument> xmlDocumentRPC = InitXmlDocumentParser(theRPCModelFile, OSSIM_PLEIADES_METADATA_TYPE_RPC);
  if (!xmlDocumentRPC)
    {
    if(traceDebug())
    {
       ossimNotify(ossimNotifyLevel_FATAL)
          << MODULE << " DEBUG:"
          << "ossimPleiadesDimapSupportData::parseSensorMetadata:"
          << "\nInitXmlDocumentParser failed.  Returning false"
          << std::endl;
    }
    return false;

    }

  //---
  // Fetch the Global RFM - Direct Model:
  //---
  xml_nodes.clear();
  xpath = "/DIMAP_Document/Rational_Function_Model/Global_RFM/Direct_Model";
  xmlDocumentRPC->findNodes(xpath, xml_nodes);
  if (xml_nodes.size() == 0)
  {
     setErrorStatus();
     return false;
  }





  return true;
}

ossimRefPtr<ossimXmlDocument> ossimPleiadesDimapSupportData::InitXmlDocumentParser(const ossimFilename& file,
                                                                                   ossimPleiadesMetadataFileType type)
{
  static const char MODULE[] = "ossimPleiadesDimapSupportData::InitXmlDocumentParser";
  traceDebug.setTraceFlag(true);

  //---
  // Instantiate the file reading:
  //---
  ossim_int64 fileSize = file.fileSize();
  std::ifstream in(file.c_str(), std::ios::binary|std::ios::in);
  std::vector<char> fullBuffer;
  ossimString bufferedIo;
  if(in.good()&&(fileSize > 0))
  {
     char buf[100];
     fullBuffer.resize(fileSize);
     in.read(buf, ossim::min((ossim_int64)100, fileSize));
     if(!in.fail())
     {
        ossimString testString = ossimString(buf,
                                             buf + in.gcount());
        if(testString.contains("xml"))
        {
           in.seekg(0);
           in.read(&fullBuffer.front(), (std::streamsize)fullBuffer.size());
           if(!in.fail())
           {
              bufferedIo = ossimString(fullBuffer.begin(),
                                       fullBuffer.begin()+in.gcount());
           }
        }
     }
  }
  else
  {
     return NULL;
  }

  //---
  // Instantiate the XML parser:
  //---
  ossimRefPtr<ossimXmlDocument> xmlDocument;

  if(bufferedIo.empty())
  {
     xmlDocument = new ossimXmlDocument(file);
  }
  else
  {
     xmlDocument = new ossimXmlDocument;
     std::istringstream inStringStream(bufferedIo.string());
     if(!xmlDocument->read(inStringStream))
     {
        return NULL;
     }
  }
  if (xmlDocument->getErrorStatus())
  {
     if(traceDebug())
     {
        ossimNotify(ossimNotifyLevel_FATAL)
           << MODULE << " DEBUG:"
           << "ossimPleiadesDimapSupportData::InitXmlDocumentParser:"
           << "\nUnable to parse xml file" << std::endl;
     }
     setErrorStatus();
     return NULL;
  }

  vector<ossimRefPtr<ossimXmlNode> > xml_nodes;
  ossimString xpath;

   //---
   // Get the version string which can be used as a key for parsing.
   //---
   xml_nodes.clear();
   xpath = "/DIMAP_Document/Metadata_Identification/METADATA_FORMAT";
   xmlDocument->findNodes(xpath, xml_nodes);
   if (xml_nodes.size() == 0)
   {
      setErrorStatus();
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG:\nCould not find: " << xpath
            << endl;
      }
      return NULL;
   }

   ossimString attribute = "version";
   ossimString value;
   ossimPleiadesMetadataVersion metadataVersion;
   xml_nodes[0]->getAttributeValue(value, attribute);
   if (value == "2.0")
   {
     metadataVersion = OSSIM_PLEIADES_METADATA_VERSION_2_0;
   }

   if (metadataVersion == OSSIM_PLEIADES_METADATA_VERSION_UNKNOWN)
   {
      setErrorStatus();
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "WARNING:  DIMAP metadata version not found!"
            << std::endl;
      }
      return NULL;
   }

   if (type == OSSIM_PLEIADES_METADATA_TYPE_PRODUCT)
     theMetadataVersion = metadataVersion;

  //---
  // Check that it is a valid PHR DIMAP file format
  //---
  xml_nodes.clear();
  xpath = "/DIMAP_Document/Metadata_Identification/METADATA_PROFILE";
  xmlDocument->findNodes(xpath, xml_nodes);
  if (xml_nodes.size() == 0)
  {
     setErrorStatus();
     if(traceDebug())
     {
  ossimNotify(ossimNotifyLevel_DEBUG)
           << "DEBUG:\n Not a PLEIADES DIMAPv2 file -> Key not found!"<< std::endl;
     }
     return NULL;
  }
  std::cout << xml_nodes[0]->getText() << std::endl;
  if ( (xml_nodes[0]->getText() != "PHR_SENSOR")
     && (xml_nodes[0]->getText() != "PHR_ORTHO")
     && (xml_nodes[0]->getText() != "PHR_MOSAIC")
     )
  {
     if(traceDebug())
     {
        ossimNotify(ossimNotifyLevel_DEBUG)
           << "DEBUG:\n Not a PLEIADES DIMAPv2 file -> Key value incorrect!"<< std::endl;
     }
     return NULL;
  }
  if ( (type==OSSIM_PLEIADES_METADATA_TYPE_RPC) && (xml_nodes[0]->getText() != "PHR_SENSOR") )
    {
    if (traceDebug())
      {
      ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG:\n Not a RPC PLEIADES DIMAPv2 file -> Key value incorrect!"
          << std::endl;
      }
    return NULL;
    }

  //---
  // Check if the type is correct ?
  //---
  xml_nodes.clear();
  xpath = "/DIMAP_Document/Metadata_Identification/METADATA_SUBPROFILE";;
  xmlDocument->findNodes(xpath, xml_nodes);
  if (xml_nodes.size() == 0)
  {
     setErrorStatus();
     if(traceDebug())
     {
  ossimNotify(ossimNotifyLevel_DEBUG)
           << "DEBUG:\n Not a PLEIADES DIMAP file -> Key not found!"<< std::endl;
     }
     return NULL;
  }
  if (  ((type!=OSSIM_PLEIADES_METADATA_TYPE_RPC) || (xml_nodes[0]->getText() != "RPC"))
     && ((type!=OSSIM_PLEIADES_METADATA_TYPE_PRODUCT) || (xml_nodes[0]->getText() != "PRODUCT"))
     )
  {
     if(traceDebug())
     {
        ossimNotify(ossimNotifyLevel_DEBUG)
           << "DEBUG:\n Not a PLEIADES DIMAP file format -> Key value not coherent with input type !"<< std::endl;
     }
     return NULL;
  }

  return xmlDocument;
}


}
