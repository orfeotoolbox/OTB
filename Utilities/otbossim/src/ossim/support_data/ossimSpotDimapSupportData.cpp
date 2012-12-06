//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Oscar Kramer (ossim port by D. Burken)
//
// Description:
//
// Contains definition of class ossimSpotDimapSupportData.
//
//*****************************************************************************
// $Id: ossimSpotDimapSupportData.cpp 20609 2012-02-27 12:05:13Z gpotts $

#include <ossim/support_data/ossimSpotDimapSupportData.h>
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
static ossimTrace traceDebug ("ossimSpotDimapSupportData:debug");

static const ossim_uint32  LAGRANGE_FILTER_SIZE = 8; // num samples considered

ossimSpotDimapSupportData::ossimSpotDimapSupportData ()
   :
   ossimErrorStatusInterface(),
   theMetadataVersion(OSSIM_SPOT_METADATA_VERSION_UNKNOWN),
   theImageID(),
   theMetadataFile(),
   theProductionDate(),
   theInstrument(),
   theInstrumentIndex(0),
   theSunAzimuth(0.0),
   theSunElevation(0.0),
   theIncidenceAngle(0.0),
   theViewingAngle(0.0),
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
   theSwirDataFlag(false),
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
ossimSpotDimapSupportData::ossimSpotDimapSupportData(const ossimSpotDimapSupportData& rhs)
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
    theSwirDataFlag (rhs.theSwirDataFlag),
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

ossimSpotDimapSupportData::ossimSpotDimapSupportData (const ossimFilename& dimapFile, bool  processSwir)
   :
   ossimErrorStatusInterface(),
   theMetadataVersion(OSSIM_SPOT_METADATA_VERSION_UNKNOWN),
   theImageID(),
   theMetadataFile (dimapFile),
   theProductionDate(),
   theInstrument(),
   theInstrumentIndex(0),
   theSunAzimuth(0.0),
   theSunElevation(0.0),
   theIncidenceAngle(0.0),
   theViewingAngle(0.0),
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
   theSwirDataFlag (processSwir),
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
         << "ossimSpotDimapSupportData::ossimSpotDimapSupportData: entering..."
         << std::endl;
   }

   loadXmlFile(dimapFile, processSwir);

   // Finished successful parse:
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimSpotDimapSupportData::ossimSpotDimapSupportData: leaving..."
         << std::endl;
   }
}

ossimSpotDimapSupportData::~ossimSpotDimapSupportData ()
{
}

ossimObject* ossimSpotDimapSupportData::dup()const
{
   return new ossimSpotDimapSupportData(*this);
}

void ossimSpotDimapSupportData::clearFields()
{
   clearErrorStatus();
   //theSensorID="Spot 5";
   theSensorID="";
   theMetadataVersion = OSSIM_SPOT_METADATA_VERSION_UNKNOWN;
   theImageID = "";
   theMetadataFile = "";
   theProductionDate = "";
   theInstrument = "";
   theInstrumentIndex = 0;
   theSunAzimuth = 0.0;
   theSunElevation = 0.0;
   theIncidenceAngle = 0.0;
   theViewingAngle = 0.0;
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
   theSwirDataFlag = false;
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

bool ossimSpotDimapSupportData::loadXmlFile(const ossimFilename& file,
                                            bool processSwir)
{
   static const char MODULE[] = "ossimSpotDimapSupportData::loadXmlFile";

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " DEBUG:"
         << "\nFile: " << file << std::endl;
   }
   clearFields();
   theSwirDataFlag = processSwir;
   theMetadataFile = file;

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
            << "ossimSpotDimapSupportData::loadXmlFile:"
            << "\nUnable to parse xml file" << std::endl;
      }
      setErrorStatus();
      return false;
   }

   //---
   // Check that it is a SPOT DIMAP file format
   //---
   vector<ossimRefPtr<ossimXmlNode> > xml_nodes;
   xml_nodes.clear();
   ossimString xpath = "/Dimap_Document/Dataset_Sources/Source_Information/Scene_Source/MISSION";
   xmlDocument->findNodes(xpath, xml_nodes);
   if (xml_nodes.size() == 0)
   {
      setErrorStatus();
      if(traceDebug())
      {
	 ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG:\n Not a SPOT DIMAP file format."<< std::endl;
      }
      return false;
   }
   if ( xml_nodes[0]->getText() != "SPOT" && xml_nodes[0]->getText() != "Spot" && xml_nodes[0]->getText() != "spot" )
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG:\n Not a SPOT DIMAP file format."<< std::endl;
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
            << "ossimSpotDimapSupportData::loadXmlFile:"
            << "\nMetadata initialization failed.  Returning false"
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
            << "ossimSpotDimapSupportData::loadXmlFile:"
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
         << "ossimSpotDimapSupportData::loadXmlFile:"
         << "\nScene source initialization failed.  Returning false"
         << std::endl;

      return false;
   }

   if (initFramePoints(xmlDocument)  == false)
   {
      ossimNotify(ossimNotifyLevel_FATAL)
         << MODULE << " DEBUG:"
         << "ossimSpotDimapSupportData::loadXmlFile:"
         << "\nFrame point initialization failed.  Returning false"
         << std::endl;
      return false;
   }

   if (parsePart1(xmlDocument) == false)
   {
      ossimNotify(ossimNotifyLevel_FATAL)
         << MODULE << " DEBUG:"
         << "ossimSpotDimapSupportData::loadXmlFile:"
         << "\nPart 1 initialization failed.  Returning false"
         << std::endl;
      return false;
   }

   if (parsePart2(xmlDocument) == false)
   {
      ossimNotify(ossimNotifyLevel_FATAL)
         << MODULE << " DEBUG:"
         << "ossimSpotDimapSupportData::loadXmlFile:"
         << "\nPart 2 initialization failed.  Returning false"
         << std::endl;
      return false;
   }

   if (parsePart3(xmlDocument) == false)
   {
      ossimNotify(ossimNotifyLevel_FATAL)
         << MODULE << " DEBUG:"
         << "ossimSpotDimapSupportData::loadXmlFile:"
         << "\nPart 3 initialization failed.  Returning false"
         << std::endl;
      return false;
   }

   if (parsePart4(xmlDocument) == false)
   {
      ossimNotify(ossimNotifyLevel_FATAL)
         << MODULE << " DEBUG:"
         << "ossimSpotDimapSupportData::loadXmlFile:"
         << "\nPart 4 initialization failed.  Returning false"
         << std::endl;
      return false;
   }

   if (traceDebug())
   {
      printInfo(ossimNotify(ossimNotifyLevel_DEBUG));

      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " DEBUG: exited..."
         << std::endl;
   }

   return true;
}

void ossimSpotDimapSupportData::getPositionEcf(ossim_uint32 sample,
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

void ossimSpotDimapSupportData::getPositionEcf(const ossim_float64& time,
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

void ossimSpotDimapSupportData::getVelocityEcf(ossim_uint32 sample, ossimEcefPoint& ve)  const
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

void ossimSpotDimapSupportData::getVelocityEcf(const ossim_float64& time,
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

void ossimSpotDimapSupportData::getEphSampTime(ossim_uint32 sample,
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

void ossimSpotDimapSupportData::getAttitude(ossim_uint32 sample,
                                            ossimDpt3d& at)  const
{
   if (sample >= theAttitudeSamples.size())
   {
      at.makeNan();
      return;
   }

   at = theAttitudeSamples[sample];
}

void ossimSpotDimapSupportData::getAttitude(const ossim_float64& time,
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

void ossimSpotDimapSupportData::extrapolateAttitude(const ossim_float64& time, ossimDpt3d& at) const
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

void ossimSpotDimapSupportData::getAttSampTime(ossim_uint32 sample, ossim_float64& at)  const
{
   if (sample >= theAttSampTimes.size())
   {
      at = ossim::nan();
      return;
   }

   at = theAttSampTimes[sample];
}

void ossimSpotDimapSupportData::getPixelLookAngleX(ossim_uint32 sample,
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

void ossimSpotDimapSupportData::getPixelLookAngleX(const ossim_float64& sample,
                                                   ossim_float64& pa) const
{
   ossim_uint32 s = static_cast<ossim_uint32>(sample);
   getInterpolatedLookAngle(s, thePixelLookAngleX, pa);
}

void ossimSpotDimapSupportData::getPixelLookAngleY(ossim_uint32 sample,
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

void ossimSpotDimapSupportData::getPixelLookAngleY(const ossim_float64& sample,
                                                   ossim_float64& pa) const
{
   ossim_uint32 s = static_cast<ossim_uint32>(sample);
   getInterpolatedLookAngle(s, thePixelLookAngleY, pa);
}

void ossimSpotDimapSupportData::getInterpolatedLookAngle(
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

void ossimSpotDimapSupportData::getBilinearInterpolation(
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

void ossimSpotDimapSupportData::getLagrangeInterpolation(
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

ossim_float64 ossimSpotDimapSupportData::convertTimeStamp(const ossimString& time_stamp) const
{
   double ti;
   convertTimeStamp(time_stamp, ti);
   return ti;
}

void ossimSpotDimapSupportData::convertTimeStamp(const ossimString& time_stamp,
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

void ossimSpotDimapSupportData::getGeoPosPoint (ossim_uint32 point,
                                                ossimDpt& ip,
                                                ossimGpt& gp) const
{
   if (point < theGeoPosImagePoints.size())
   {
      ip = theGeoPosImagePoints [point];
      gp = theGeoPosGroundPoints[point];
   }
}

void ossimSpotDimapSupportData::printInfo(ostream& os) const
{
   ossimString corr_att = "NO";
   if (theStarTrackerUsed)
      corr_att = "YES";

   os << "\n----------------- Info on SPOT5 Image -------------------"
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
      << "\n  Incidence Angle:      " << theIncidenceAngle
      << "\n  Viewing Angle:        " << theViewingAngle      
      << "\n  Scene Orientation:    " << theSceneOrientation 
      << "\n  Corrected Attitude:   " << corr_att
      << "\n  Sun Azimuth:          " << theSunAzimuth
      << "\n  Sun Elevation:        " << theSunElevation
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

ossimString ossimSpotDimapSupportData::getSensorID() const
{
  return theSensorID;
}

ossimString   ossimSpotDimapSupportData::getMetadataVersionString() const
{
   if (theMetadataVersion == OSSIM_SPOT_METADATA_VERSION_1_1)
   {
      return ossimString("1.1");
   }
   else if (theMetadataVersion == OSSIM_SPOT_METADATA_VERSION_1_0)
   {
      return ossimString("1.0");
   }
   return ossimString("unknown");
}

ossimString ossimSpotDimapSupportData::getAcquisitionDate() const
{
   return theAcquisitionDate;
}

ossimString ossimSpotDimapSupportData::getProductionDate() const
{
   return theProductionDate;
}

ossimString ossimSpotDimapSupportData::getImageID() const
{
   return theImageID;
}

ossimFilename ossimSpotDimapSupportData::getMetadataFile() const
{
   return theMetadataFile;
}

ossimString ossimSpotDimapSupportData::getInstrument() const
{
   return theInstrument;
}

ossim_uint32 ossimSpotDimapSupportData::getInstrumentIndex() const
{
   return theInstrumentIndex;
}

void ossimSpotDimapSupportData::getSunAzimuth(ossim_float64& az) const
{
   az = theSunAzimuth;
}

void ossimSpotDimapSupportData::getSunElevation(ossim_float64& el) const
{
   el = theSunElevation;
}

void ossimSpotDimapSupportData::getImageSize(ossimDpt& sz) const
{
   sz = theImageSize;
}

void ossimSpotDimapSupportData::getLineSamplingPeriod(ossim_float64& pe) const
{
   pe = theLineSamplingPeriod;
}

bool ossimSpotDimapSupportData::isStarTrackerUsed() const
{
   return theStarTrackerUsed;
}

bool ossimSpotDimapSupportData::isSwirDataUsed() const
{
   return theSwirDataFlag;
}

ossim_uint32 ossimSpotDimapSupportData::getNumberOfBands() const
{
   return theNumBands;
}

ossim_uint32 ossimSpotDimapSupportData::getStepCount() const
{
   return theStepCount;
}

void ossimSpotDimapSupportData::getIncidenceAngle(ossim_float64& ia) const
{
   ia = theIncidenceAngle;
}

void ossimSpotDimapSupportData::getViewingAngle(ossim_float64& va) const
{
   va = theViewingAngle;
}

void ossimSpotDimapSupportData::getSceneOrientation(ossim_float64& so) const
{
   so = theSceneOrientation;
}

void ossimSpotDimapSupportData::getRefGroundPoint(ossimGpt& gp) const
{
   gp = theRefGroundPoint;
}

void ossimSpotDimapSupportData::getRefImagePoint(ossimDpt& rp) const
{
   rp = theRefImagePoint;
}

void ossimSpotDimapSupportData::getRefLineTime(ossim_float64& rt) const
{
   rt = theRefLineTime;
}

void ossimSpotDimapSupportData::getRefLineTimeLine(ossim_float64& rtl) const
{
   rtl = theRefLineTimeLine;
}

ossim_uint32 ossimSpotDimapSupportData::getNumEphSamples() const
{
   return (ossim_uint32)theEphSampTimes.size();
}

ossim_uint32 ossimSpotDimapSupportData::getNumAttSamples() const
{
   return (ossim_uint32)theAttSampTimes.size();
}

ossim_uint32 ossimSpotDimapSupportData::getNumGeoPosPoints() const
{
   return (ossim_uint32)theGeoPosImagePoints.size();
}

void ossimSpotDimapSupportData::getUlCorner(ossimGpt& pt) const
{
   pt = theUlCorner;
}

void ossimSpotDimapSupportData::getUrCorner(ossimGpt& pt) const
{
   pt = theUrCorner;
}

void ossimSpotDimapSupportData::getLrCorner(ossimGpt& pt) const
{
   pt = theLrCorner;
}

void ossimSpotDimapSupportData::getLlCorner(ossimGpt& pt) const
{
   pt = theLlCorner;
}

void ossimSpotDimapSupportData::getImageRect(ossimDrect& rect)const
{
   rect = ossimDrect(0.0, 0.0, theImageSize.x-1.0, theImageSize.y-1.0);
}

void ossimSpotDimapSupportData::getSubImageOffset(ossimDpt& offset) const
{
   offset = theSubImageOffset;
}

bool ossimSpotDimapSupportData::saveState(ossimKeywordlist& kwl,
                                          const char* prefix)const
{
   kwl.add(prefix,
           ossimKeywordNames::TYPE_KW,
           "ossimSpotDimapSupportData",
           true);

   kwl.add(prefix,
           "metadata_file",
           theMetadataFile,
           true);

   kwl.add(prefix,
           ossimKeywordNames::AZIMUTH_ANGLE_KW,
           theSunAzimuth,
           true);

   kwl.add(prefix,
           ossimKeywordNames::ELEVATION_ANGLE_KW,
           theSunElevation,
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

   ossimString tempString;
   ossim_uint32 idx = 0;

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
           "swir_data_flag",
           static_cast<ossim_uint32>(theSwirDataFlag),
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

   kwl.add(prefix,
           "incident_angle",
           theIncidenceAngle,
           true);

   kwl.add(prefix,
           "viewing_angle",
           theViewingAngle,
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

bool ossimSpotDimapSupportData::loadState(const ossimKeywordlist& kwl,
                                          const char* prefix)
{
   clearFields();

   ossimString type = kwl.find(prefix, ossimKeywordNames::TYPE_KW);

   if(type != "ossimSpotDimapSupportData")
   {
      return false;
   }
   theMetadataFile = kwl.find(prefix, "metadata_file");

   theSunAzimuth   = ossimString(kwl.find(prefix, ossimKeywordNames::AZIMUTH_ANGLE_KW)).toDouble();
   theSunElevation = ossimString(kwl.find(prefix, ossimKeywordNames::ELEVATION_ANGLE_KW)).toDouble();

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


   ossim_uint32 idx = 0;
   ossim_uint32 total =  ossimString(kwl.find(prefix,"number_of_pixel_lookat_angle_x")).toUInt32();
   ossimString tempString;

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
   theSwirDataFlag    = ossimString(kwl.find(prefix, "swir_data_flag")).toBool();
   theNumBands        = ossimString(kwl.find(prefix, ossimKeywordNames::NUMBER_BANDS_KW)).toUInt32();
   theAcquisitionDate = kwl.find(prefix, ossimKeywordNames::IMAGE_DATE_KW);
   theProductionDate  = kwl.find(prefix, "production_date");
   theImageID         = kwl.find(prefix, "image_id");
   theInstrument      = kwl.find(prefix, "instrument");
   theInstrumentIndex = ossimString(kwl.find(prefix, "instrument_index")).toUInt32();
   theStepCount       = ossimString(kwl.find(prefix, "step_count")).toInt32();
   
   theIncidenceAngle  = ossimString(kwl.find(prefix, "incident_angle")).toDouble();
   theViewingAngle    = ossimString(kwl.find(prefix, "viewing_angle")).toDouble();
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

ossimGpt ossimSpotDimapSupportData::createGround(const ossimString& s)const
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

ossimDpt ossimSpotDimapSupportData::createDpt(const ossimString& s)const
{
   std::istringstream in(s.string());
   ossimString x, y;
   ossimString code;

   in >> x.string() >> y.string();

   return ossimDpt(x.toDouble(), y.toDouble());

}

bool ossimSpotDimapSupportData::parsePart1(
   ossimRefPtr<ossimXmlDocument> xmlDocument)
{
   static const char MODULE[] = "ossimSpotDimapSupportData::parsePart1";

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

   if (theSwirDataFlag)
   {
      theImageSize.line /= 2.0;
      theImageSize.samp /= 2.0;
   }

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


   if (theSwirDataFlag)
   {
      theRefImagePoint.line /= 2.0;
      theRefImagePoint.samp /= 2.0;
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

bool ossimSpotDimapSupportData::parsePart2(
   ossimRefPtr<ossimXmlDocument> xmlDocument)
{
   static const char MODULE[] = "ossimSpotDimapSupportData::parsePart2";

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

   if (theSwirDataFlag)
   {
      theLineSamplingPeriod *= 2.0;
   }

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
      if (theSwirDataFlag)
      {
         setErrorStatus();
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << MODULE << " DEBUG:"
               << "\nSWIR band error..."
               << std::endl;
         }
         return false;
      }
      band_index = 0;
   }
   else if (theNumBands == 3)
   {
      band_index = 0; // using green band for PSI angles
   }
   else if (theNumBands == 4)
   {
      if (theSwirDataFlag)
      {
         band_index = 3;
      }
      else
      {
         band_index = 1; // using green band for PSI angles
      }
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

   if (theMetadataVersion == OSSIM_SPOT_METADATA_VERSION_1_1)
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

   if (theMetadataVersion == OSSIM_SPOT_METADATA_VERSION_1_1)
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

bool ossimSpotDimapSupportData::parsePart3(
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

bool ossimSpotDimapSupportData::parsePart4(ossimRefPtr<ossimXmlDocument> xmlDocument)
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
  xpath = "/Dimap_Document/Image_Interpretation/Spectral_Band_Info";
  xmlDocument->findNodes(xpath, xml_nodes);
  node = xml_nodes.begin();
  while (node != xml_nodes.end())
  {
    sub_nodes.clear();
    xpath = "BAND_INDEX";
    (*node)->findChildNodes(xpath, sub_nodes);
    if (sub_nodes.size() == 0)
    {
      setErrorStatus();
      return false;
    }

    ossim_int32 bandIndex = sub_nodes[0]->getText().toInt32() - 1;

    if( (bandIndex >= static_cast<int>(theNumBands) ) || (bandIndex<0) )
    {
       ossimNotify(ossimNotifyLevel_WARN) << "ossimSpotDimapSupportData::parsePart4 ERROR: Band index outside of range\n";
       return false;
    }

    sub_nodes.clear();
    xpath = "PHYSICAL_BIAS";
    (*node)->findChildNodes(xpath, sub_nodes);
    if (sub_nodes.size() == 0)
    {
      setErrorStatus();
      return false;
    }
    thePhysicalBias[bandIndex] = sub_nodes[0]->getText().toDouble();

    sub_nodes.clear();
    xpath = "PHYSICAL_GAIN";
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
  xpath = "/Dimap_Document/Data_Strip/Sensor_Calibration/Solar_Irradiance/Band_Solar_Irradiance";
  xmlDocument->findNodes(xpath, xml_nodes);
  node = xml_nodes.begin();
  while (node != xml_nodes.end())
  {
    sub_nodes.clear();
    xpath = "BAND_INDEX";
    (*node)->findChildNodes(xpath, sub_nodes);
    if (sub_nodes.size() == 0)
    {
      setErrorStatus();
      return false;
    }

    ossim_int32 bandIndex = sub_nodes[0]->getText().toInt32() - 1;
    
    if((bandIndex >= static_cast<ossim_int32>(theNumBands) ) || (bandIndex<0))
    {
       ossimNotify(ossimNotifyLevel_WARN) << "ossimSpotDimapSupportData::parsePart4 ERROR: Band index outside of range\n";
       return false;
    }
    
    sub_nodes.clear();
    xpath = "SOLAR_IRRADIANCE_VALUE";
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

bool ossimSpotDimapSupportData::initMetadataVersion(ossimRefPtr<ossimXmlDocument> xmlDocument)
{
   ossimString xpath;
   std::vector<ossimRefPtr<ossimXmlNode> > xml_nodes;

   //---
   // Get the version string which can be used as a key for parsing.
   //---
   xpath = "/Dimap_Document/Metadata_Id/METADATA_FORMAT";
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
   if (value == "1.0")
   {
      theMetadataVersion = OSSIM_SPOT_METADATA_VERSION_1_0;
   }
   else if (value == "1.1")
   {
      theMetadataVersion = OSSIM_SPOT_METADATA_VERSION_1_1;
   }

   if (theMetadataVersion == OSSIM_SPOT_METADATA_VERSION_UNKNOWN)
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

bool ossimSpotDimapSupportData::initImageId(
   ossimRefPtr<ossimXmlDocument> xmlDocument)
{
   ossimString xpath;
   vector<ossimRefPtr<ossimXmlNode> > xml_nodes;

   //---
   // Fetch the Image ID:
   //---
   xpath = "/Dimap_Document/Production/JOB_ID";
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

bool ossimSpotDimapSupportData::initSceneSource(
   ossimRefPtr<ossimXmlDocument> xmlDocument)
{
   ossimString xpath;
   vector<ossimRefPtr<ossimXmlNode> > xml_nodes;

  //---
  // Fetch the mission index (Spot 1, 4 or 5):
  // and generate theSensorID
  //---
   xml_nodes.clear();
   xpath = "/Dimap_Document/Dataset_Sources/Source_Information/Scene_Source/MISSION_INDEX";
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
   if (xml_nodes[0]->getText() == "1")
     theSensorID = "Spot 1";
   if (xml_nodes[0]->getText() == "2")
     theSensorID = "Spot 2";
   if (xml_nodes[0]->getText() == "4")
     theSensorID = "Spot 4";
   if (xml_nodes[0]->getText() == "5")
     theSensorID = "Spot 5";

   //---
   // Fetch the Sun Azimuth:
   //---
   xml_nodes.clear();
   xpath = "/Dimap_Document/Dataset_Sources/Source_Information/Scene_Source/SUN_AZIMUTH";
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
   theSunAzimuth = xml_nodes[0]->getText().toDouble();

   //---
   // Fetch the Sun Elevation:
   //---
   xml_nodes.clear();
   xpath = "/Dimap_Document/Dataset_Sources/Source_Information/Scene_Source/SUN_ELEVATION";
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
   theSunElevation = xml_nodes[0]->getText().toDouble();

   //---
   // Fetch incidence angle:
   //---
   xml_nodes.clear();
   xpath = "/Dimap_Document/Dataset_Sources/Source_Information/Scene_Source/INCIDENCE_ANGLE";
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
   theIncidenceAngle = xml_nodes[0]->getText().toDouble();

   //---
   // Fetch viewing angle:
   //
   // From the SPOT Dimap documentation (Dimap Generic 1.0), VIEWING_ANGLE
   // (the scene instrumental viewing angle) is ONLY available for SPOT5 data.
   // FROM SPOT: You can use use incidence angle to calculate viewing angle
   // (called look direction as well).
   // FIX (see below): need theSatelliteAltitude and theIncidenceAngle. The
   // equation is shown below where RT is the mean value of WGS84 ellipsoid 
   // semi-axis.
   //---
   if(this->theSensorID == "Spot 5")
   {
      xml_nodes.clear();
      xpath = "/Dimap_Document/Dataset_Sources/Source_Information/Scene_Source/VIEWING_ANGLE";
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
      theViewingAngle = xml_nodes[0]->getText().toDouble();
   }
   else
   {
      xml_nodes.clear();
      xpath = "/Dimap_Document/Data_Strip/Ephemeris/SATELLITE_ALTITUDE";
      
      theViewingAngle = -1.0;
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
      //compute VIEWING_ANGLE
      double theSatelliteAltitude =  xml_nodes[0]->getText().toDouble();
      double RT = 63710087714.0;
      theViewingAngle = asin((RT/(RT+theSatelliteAltitude))*sin(theIncidenceAngle));
   }

   //---
   // Fetch Step Count:
   //---
   xml_nodes.clear();
   xpath = "/Dimap_Document/Data_Strip/Sensor_Configuration/Mirror_Position/STEP_COUNT";
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
   theStepCount = xml_nodes[0]->getText().toInt();
   
   return true;
}

bool ossimSpotDimapSupportData::initFramePoints(
   ossimRefPtr<ossimXmlDocument> xmlDocument)
{
   ossimString xpath;
   vector<ossimRefPtr<ossimXmlNode> > xml_nodes;

   //---
   // Corner points:
   //---
   xml_nodes.clear();
   xpath = "/Dimap_Document/Dataset_Frame/Vertex";
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
      xpath = "FRAME_LAT";
      (*node)->findChildNodes(xpath, sub_nodes);
      if (sub_nodes.size() == 0)
      {
         setErrorStatus();
         return false;
      }
      gpt.lat = sub_nodes[0]->getText().toDouble();

      sub_nodes.clear();
      xpath = "FRAME_LON";
      (*node)->findChildNodes(xpath, sub_nodes);
      if (sub_nodes.size() == 0)
      {
         setErrorStatus();
         return false;
      }
      gpt.lon = sub_nodes[0]->getText().toDouble();
      gpt.hgt = 0.0; // assumed

      sub_nodes.clear();
      xpath = "FRAME_ROW";
      (*node)->findChildNodes(xpath, sub_nodes);
      if (sub_nodes.size() == 0)
      {
         setErrorStatus();
         return false;
      }
      ipt.line = sub_nodes[0]->getText().toDouble() - 1.0;

      sub_nodes.clear();
      xpath = "FRAME_COL";
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
   xpath = "/Dimap_Document/Dataset_Frame/Scene_Center/FRAME_LON";
   xmlDocument->findNodes(xpath, xml_nodes);
   if (xml_nodes.size() != 1)
   {
      setErrorStatus();
      return false;
   }
   theRefGroundPoint.lon = xml_nodes[0]->getText().toDouble();

   xml_nodes.clear();
   xpath = "/Dimap_Document/Dataset_Frame/Scene_Center/FRAME_LAT";
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
   xml_nodes.clear();
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
   theSceneOrientation = xml_nodes[0]->getText().toDouble();  

   return true;
}

