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
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimTrace.h>

#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimXmlDocument.h>
#include <ossim/base/ossimXmlAttribute.h>
#include <ossim/base/ossimXmlNode.h>

#include <ossimPluginCommon.h>
#include <ossim/base/ossimCommon.h>

#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimNotifyContext.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <sstream>


// Define Trace flags for use within this file:
static ossimTrace traceExec  ("ossimPleiadesDimapSupportData:exec");
static ossimTrace traceDebug ("ossimPleiadesDimapSupportData:debug");

static std::string getVectorFloat64AsString(std::vector<ossim_float64> in)
{
  std::vector<ossim_float64>::iterator it;
  std::stringstream strTmp;

  for ( it=in.begin() ; it < in.end(); it++ )
    {
    strTmp << " " << (*it);
    }

  return strTmp.str();
}

static std::string getVectorDoubleAsString(std::vector<double> in)
{
  std::vector<double>::iterator it;
  std::stringstream strTmp;

  for ( it=in.begin() ; it < in.end(); it++ )
    {
    strTmp << " " << (*it);
    }

  return strTmp.str();
}

static std::string getVectorStringAsString(std::vector<ossimString> in)
{
  std::vector<ossimString>::iterator it;
  std::stringstream strTmp;

  for ( it=in.begin() ; it < in.end(); it++ )
    {
    strTmp << " " << (*it);
    }

  return strTmp.str();
}



namespace ossimplugins
{

static bool readOneXmlNode(ossimRefPtr<ossimXmlDocument> xmlDocument,
                           ossimString xpath,
                           ossimString& nodeValue)
{
  vector<ossimRefPtr<ossimXmlNode> > xml_nodes;

  xmlDocument->findNodes(xpath, xml_nodes);
  if (xml_nodes.size() == 0)
    {
    if(traceDebug())
      {
      ossimNotify(ossimNotifyLevel_DEBUG) << " DEBUG:"
          << "\nCould not find: " << xpath << std::endl;
      }
    return false;
    }
  if (xml_nodes.size() != 1)
    {
    if(traceDebug())
      {
      ossimNotify(ossimNotifyLevel_DEBUG) << " DEBUG:"
          << "\nMore than one node value with " << xpath << std::endl;
      }
    return false;
    }
  nodeValue = xml_nodes[0]->getText();
  return true;
}

ossimPleiadesDimapSupportData::ossimPleiadesDimapSupportData ()
   :ossimErrorStatusInterface(),
    theMetadataSubProfile(OSSIM_PLEIADES_METADATA_SUBPROFILE_UNKNOWN),
    theProductIsOk(false),
    TheRpcIsOk(false),
    theXmlDocumentRoot(),

    theSensorID(),
    theImageID(),
    theProductionDate(),
    theAcquisitionDate(),
    theFirstLineImagingTime(),
    theFirstLineImagingDate(),
    theInstrument(),
    theInstrumentIndex(),
    theProcessingLevelString(),
    theSpectralProcessingString(),

    theSunAzimuth(),
    theSunElevation(),
    theIncidenceAngle(),
    theViewingAngle(),
    theAzimuthAngle(),

    theImageSize(0, 0),
    theTileSize(0, 0),
    theNumberOfMegaTilesInRow(0),
    theNumberOfMegaTilesInCol(0),
    theNumberOfMegaTiles(0),
    theMultiDataFile(),

    theNumBands(0),
    theBandOrder(),

    thePhysicalBias(),
    thePhysicalGain(),
    theSolarIrradiance(),

    theUlCorner(),
    theUrCorner(),
    theLrCorner(),
    theLlCorner(),

    theSpecId(),
    theErrBias(0),
    theErrBiasX(0),
    theErrBiasY(0),
    theErrRand(0),
    theLineOffset(0),
    theSampOffset(0),
    theLatOffset(0),
    theLonOffset(0),
    theHeightOffset(0),
    theLineScale(0),
    theSampScale(0),
    theLatScale(0),
    theLonScale(0),
    theHeightScale(0),
    theLineNumCoeff(),
    theLineDenCoeff(),
    theSampNumCoeff(),
    theSampDenCoeff(),

    theRefGroundPoint(0.0, 0.0, 0.0),
    theRefImagePoint(0.0, 0.0)
{
}

ossimPleiadesDimapSupportData::~ossimPleiadesDimapSupportData ()
{
}


void ossimPleiadesDimapSupportData::clearFields()
{
   clearErrorStatus();
   theMetadataSubProfile = OSSIM_PLEIADES_METADATA_SUBPROFILE_UNKNOWN;
   theProductIsOk = false;
   TheRpcIsOk = false;
   theXmlDocumentRoot = "";
   theImageID = "";
   theSensorID="";
   theProductionDate = "";
   theAcquisitionDate = "";
   theFirstLineImagingTime = "";
   theFirstLineImagingDate = "";
   theInstrument = "";
   theInstrumentIndex = "";
   theProcessingLevelString = "";
   theSpectralProcessingString = "";

   theSunAzimuth.clear();
   theSunElevation.clear();
   theIncidenceAngle.clear();
   theViewingAngle.clear();
   theAzimuthAngle.clear();

   theImageSize.makeNan();
   theTileSize.makeNan();
   theNumberOfMegaTilesInRow = 0;
   theNumberOfMegaTilesInCol = 0;
   theNumberOfMegaTiles = 0;
   ossimString msg = "";
   theMultiDataFile.setBooleanValue(false, msg);

   theNumBands = 0;
   theBandOrder.clear();

   thePhysicalBias.clear();
   thePhysicalGain.clear();
   theSolarIrradiance.clear();

   //---
   // Corner points:
   //---
   theUlCorner.makeNan();
   theUrCorner.makeNan();
   theLrCorner.makeNan();
   theLlCorner.makeNan();

   theRefGroundPoint.makeNan();
   theRefImagePoint.makeNan();

   //---
   // RPC model parameters
   //---
   theErrBias = 0.0;
   theErrBiasX = 0.0;
   theErrBiasY = 0.0;
   theErrRand = 0.0;
   theLineOffset = 0;
   theSampOffset = 0;
   theLatOffset = 0.0;
   theLonOffset = 0.0;
   theHeightOffset = 0.0;
   theLineScale = 0.0;
   theSampScale = 0.0;
   theLatScale = 0.0;
   theLonScale = 0.0;
   theHeightScale = 0.0;
   theLineNumCoeff.clear();
   theLineDenCoeff.clear();
   theSampNumCoeff.clear();
   theSampDenCoeff.clear();
   theSpecId = "";
}

void ossimPleiadesDimapSupportData::printInfo(ostream& os) const
{

   os << "\n----------------- Info on Pleiades Image -------------------"
      << "\n  "
      << "\n  Job Number (ID):      " << theImageID
      << "\n  Acquisition Date:     " << theAcquisitionDate
      << "\n  Instrument:           " << theInstrument
      << "\n  Instrument Index:     " << theInstrumentIndex
      << "\n  Production Date:      " << theProductionDate
      << "\n  Processing Level:     " << theProcessingLevelString
      << "\n  Spectral Processing:  " << theSpectralProcessingString

      << "\n  Number of Bands:      " << theNumBands
      << "\n  Bands Display Order:  " << getVectorStringAsString(theBandOrder)

      << "\n  Image Size:           " << theImageSize
      << "\n  Tile Size:            " << theTileSize
      << "\n  Number Of Tiles (Row, Col): " << theNumberOfMegaTiles << " ("
                                            << theNumberOfMegaTilesInRow << ", "
                                            << theNumberOfMegaTilesInCol << ")"

      << "\n  Incidence Angle (TopCenter, Center, BottomCenter):   " << getVectorFloat64AsString(theIncidenceAngle)
      << "\n  Viewing Angle (TopCenter, Center, BottomCenter):     " << getVectorFloat64AsString(theViewingAngle)
      << "\n  Azimuth Angle (TopCenter, Center, BottomCenter):     " << getVectorFloat64AsString(theAzimuthAngle)
      << "\n  Sun Azimuth (TopCenter, Center, BottomCenter):       " << getVectorFloat64AsString(theSunAzimuth)
      << "\n  Sun Elevation (TopCenter, Center, BottomCenter):     " << getVectorFloat64AsString(theSunElevation)

      << "\n  Physical Bias (for each band):     " << getVectorFloat64AsString(thePhysicalBias)
      << "\n  Physical Gain (for each band):     " << getVectorFloat64AsString(thePhysicalGain)
      << "\n  Solar Irradiance (for each band):  " << getVectorFloat64AsString(theSolarIrradiance)

      << "\n  Geo Center Point:     " << theRefGroundPoint
      // TODO add RefImagePoint if necessary

      << "\n  Corner Points:"
      << "\n     UL: " << theUlCorner
      << "\n     UR: " << theUrCorner
      << "\n     LR: " << theLrCorner
      << "\n     LL: " << theLlCorner
      << "\n"

      << "\n  RPC model parameters:"
      << "\n     RPC ID: " << theSpecId
      << "\n     SampNumCoeff: " << getVectorDoubleAsString(theSampNumCoeff)
      << "\n     SampDenCoeff: " << getVectorDoubleAsString(theSampDenCoeff)
      << "\n     LineNumCoeff: " << getVectorDoubleAsString(theLineNumCoeff)
      << "\n     LineDenCoeff: " << getVectorDoubleAsString(theLineDenCoeff)
      << "\n     LonScale: " << theLonScale
      << "\n     LonOffset: " << theLonOffset
      << "\n     LatScale: " << theLatScale
      << "\n     LonScale: " << theLonScale
      << "\n     HeightScale: " << theHeightScale
      << "\n     HeightOffset: " << theHeightOffset
      << "\n     SampScale: " << theSampScale
      << "\n     SampOffset: " << theSampOffset
      << "\n     LineScale: " << theLineScale
      << "\n     LineOffset: " << theLineOffset
      << "\n     theErrBias: " << theErrBias
      << "\n     theErrBiasX: " << theErrBiasX
      << "\n     theErrBiasY: " << theErrBiasY
      << "\n     theErrRand: " << theErrRand
      << "\n"
      << "\n---------------------------------------------------------"
      << "\n  " << std::endl;
}

bool ossimPleiadesDimapSupportData::parseXmlFile(const ossimFilename& file)
{
   static const char MODULE[] = "ossimPleiadesDimapSupportData::parseXmlFile";
   //traceDebug.setTraceFlag(true);

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " DEBUG:" << "\nFile: " << file << std::endl;
   }

   if (allMetadataRead())
     clearFields();

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
      in.read(buf, std::min((ossim_int64)100, fileSize));
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
            << MODULE << " DEBUG:" << "ossimPleiadesDimapSupportData::parseXmlFile:"
            << "\nUnable to parse xml file" << std::endl;
      }
      setErrorStatus();
      return false;
   }

  if (!parseMetadataIdentification(xmlDocument))
    {
    if (traceDebug())
      {
      ossimNotify(ossimNotifyLevel_FATAL)
          << MODULE << " DEBUG:" << "ossimPleiadesDimapSupportData::parseXmlFile:"
          << "\nparseMetadataIdentification failed.  Returning false"
          << std::endl;
      }
    return false;
    }

  if (theMetadataSubProfile == OSSIM_PLEIADES_METADATA_SUBPROFILE_PRODUCT)
    {
    if (theProductIsOk)
      clearFields();

    if (!parseDatasetIdentification(xmlDocument))
      {
      if (traceDebug())
        {
        ossimNotify(ossimNotifyLevel_FATAL)
            << MODULE << " DEBUG:" << "ossimPleiadesDimapSupportData::parseXmlFile:"
            << "\nparseDatasetdentification failed.  Returning false" << std::endl;
        }
      return false;
      }

    if (!parseDatasetContent(xmlDocument))
     {
     ossimNotify(ossimNotifyLevel_FATAL)
         << MODULE << " DEBUG:" << "ossimPleiadesDimapSupportData::parseXmlFile:"
         << "\nparseDatasetContent failed.  Returning false" << std::endl;
     return false;
     }

    //
    if (!parseProductInformation(xmlDocument))
      {
      if (traceDebug())
        {
        ossimNotify(ossimNotifyLevel_FATAL)
            << MODULE << " DEBUG:" << "ossimPleiadesDimapSupportData::parseXmlFile:"
            << "\nparseProductInformation failed.  Returning false" << std::endl;
        }
      return false;
      }

    //
    if (!parseCoordinateReferenceSystem(xmlDocument))
      {
      if (traceDebug())
        {
        ossimNotify(ossimNotifyLevel_FATAL)
            << MODULE << " DEBUG:" << "ossimPleiadesDimapSupportData::parseXmlFile:"
            << "\nparseCoordinateReferenceSystem failed.  Returning false" << std::endl;
        }
      return false;
      }

    //
    if (!parseGeoposition(xmlDocument))
      {
      if (traceDebug())
        {
        ossimNotify(ossimNotifyLevel_FATAL)
            << MODULE << " DEBUG:" << "ossimPleiadesDimapSupportData::parseXmlFile:"
            << "\nparseGeoposition failed.  Returning false" << std::endl;
        }
      return false;
      }

    //
    if (!parseProcessingInformation(xmlDocument))
      {
      if (traceDebug())
        {
        ossimNotify(ossimNotifyLevel_FATAL)
            << MODULE << " DEBUG:" << "ossimPleiadesDimapSupportData::parseXmlFile:"
            << "\nparseProcessingInformation failed.  Returning false" << std::endl;
        }
      return false;
      }

    //
    if (!parseRasterData(xmlDocument))
      {
      if (traceDebug())
        {
        ossimNotify(ossimNotifyLevel_FATAL)
            << MODULE << " DEBUG:" << "ossimPleiadesDimapSupportData::parseXmlFile:"
            << "\nparseRasterData failed.  Returning false" << std::endl;
        }
      return false;
      }

    //
    if (!parseRadiometricData(xmlDocument))
      {
      ossimNotify(ossimNotifyLevel_FATAL)
          << MODULE << " DEBUG:" << "ossimPleiadesDimapSupportData::parseXmlFile:"
          << "\nparseRadiometricData failed.  Returning false" << std::endl;
      return false;
      }

    //
    if (!parseGeometricData(xmlDocument))
      {
      ossimNotify(ossimNotifyLevel_FATAL)
          << MODULE << " DEBUG:" << "ossimPleiadesDimapSupportData::parseXmlFile:"
          << "\nparseGeometricData failed.  Returning false" << std::endl;
      return false;
      }

    //
    if (!parseQualityAssessment(xmlDocument))
      {
      ossimNotify(ossimNotifyLevel_FATAL)
          << MODULE << " DEBUG:" << "ossimPleiadesDimapSupportData::parseXmlFile:"
          << "\nparseQualityAssessment failed.  Returning false" << std::endl;
      return false;
      }

    //
    if (!parseDatasetSources(xmlDocument))
      {
      ossimNotify(ossimNotifyLevel_FATAL)
          << MODULE << " DEBUG:" << "ossimPleiadesDimapSupportData::parseXmlFile:"
          << "\nparseDatasetSources failed.  Returning false" << std::endl;
      return false;
      }

    theProductIsOk = true;
    }
  else
    {
    // Parse RPC file
    if (theProcessingLevelString == "SENSOR")
      {
      if (parseRPCMetadata(xmlDocument) == false)
        {
        ossimNotify(ossimNotifyLevel_FATAL)
            << MODULE << " DEBUG:" << "ossimPleiadesDimapSupportData::parseXmlFile:"
            << "\nparseRPCMetadata initialization failed.  Returning false" << std::endl;
        return false;
        }
      TheRpcIsOk = true;
      }
    else
      TheRpcIsOk = true;
    }

  if (theProcessingLevelString != "SENSOR")
    TheRpcIsOk = true;

  if (traceDebug() && allMetadataRead())
    {
    printInfo(ossimNotify(ossimNotifyLevel_DEBUG));

    ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " DEBUG: exited..." << std::endl;
    }

  return true;
}


ossimString ossimPleiadesDimapSupportData::getProcessingLevel() const
{
  return theProcessingLevelString;
}

ossimString ossimPleiadesDimapSupportData::getSensorID() const
{
  return theSensorID;
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

ossimString ossimPleiadesDimapSupportData::getInstrument() const
{
   return theInstrument;
}

ossimString ossimPleiadesDimapSupportData::getInstrumentIndex() const
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

void ossimPleiadesDimapSupportData::getImageSize(ossimIpt& sz) const
{
   sz = theImageSize;
}

ossim_uint32 ossimPleiadesDimapSupportData::getNumberOfBands() const
{
   return theNumBands;
}

void ossimPleiadesDimapSupportData::getIncidenceAngle(std::vector<ossim_float64>& ia) const
{
   ia = theIncidenceAngle;
}

void ossimPleiadesDimapSupportData::getViewingAngle(std::vector<ossim_float64>& va) const
{
   va = theViewingAngle;
}

void ossimPleiadesDimapSupportData::getRefGroundPoint(ossimGpt& gp) const
{
   gp = theRefGroundPoint;
}

void ossimPleiadesDimapSupportData::getRefImagePoint(ossimDpt& rp) const
{
   rp = theRefImagePoint;
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


bool ossimPleiadesDimapSupportData::saveState(ossimKeywordlist& kwl,
                                          const char* prefix)const
{
  ossimString tempString;
  ossim_uint32 idx = 0;

   kwl.add(prefix,
           ossimKeywordNames::TYPE_KW,
           "ossimPleiadesDimapSupportData",
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
           "number_of_azimuth_angle",
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
           "number_of_elevation_angle",
           static_cast<ossim_uint32>(theSunElevation.size()),
           true);

   //---
   // Note: since this is a new keyword, use the point.toString as there is
   // no backwards compatibility issues.
   //---

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
           ossimKeywordNames::NUMBER_BANDS_KW,
           theNumBands,
           true);

   tempString = "";
   for(idx = 0; idx <  theBandOrder.size(); ++idx)
   {
      tempString += (theBandOrder[idx] + " ");
   }

   kwl.add(prefix,
           "band_name_list",
           tempString,
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
           "number_of_incident_angle",
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

   tempString = "";
   for(idx = 0; idx <  theAzimuthAngle.size(); ++idx)
   {
      tempString += (ossimString::toString(theAzimuthAngle[idx]) + " ");
   }

   kwl.add(prefix,
           "scene_orientation",
           tempString,
           true);

   kwl.add(prefix,
           "number_of_scene_orientation",
           static_cast<ossim_uint32>(theAzimuthAngle.size()),
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

   const char* lookup;

   theImageSize      = createIpt(kwl.find(prefix, "image_size"));
   theRefGroundPoint = createGround(kwl.find(prefix, "reference_ground_point"));
   theRefImagePoint  = createDpt(kwl.find(prefix, "reference_image_point"));

   theNumBands        = ossimString(kwl.find(prefix, ossimKeywordNames::NUMBER_BANDS_KW)).toUInt32();

   theBandOrder.resize(theNumBands);
   tempString = kwl.find(prefix,"band_name_list");
   if(tempString != "")
   {
      std::istringstream in(tempString.string());
      ossimString tempValue;
      for(idx = 0; idx < theBandOrder.size();++idx)
      {
         in >> tempValue.string();
         theBandOrder[idx] = tempValue;
      }
   }

   theAcquisitionDate = kwl.find(prefix, ossimKeywordNames::IMAGE_DATE_KW);
   theProductionDate  = kwl.find(prefix, "production_date");
   theImageID         = kwl.find(prefix, "image_id");
   theInstrument      = kwl.find(prefix, "instrument");
   theInstrumentIndex = kwl.find(prefix, "instrument_index");
   
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

   total =  ossimString(kwl.find(prefix,"number_of_scene_orientation")).toUInt32();
   theAzimuthAngle.resize(total);
   tempString = kwl.find(prefix,"scene_orientation");
   if(tempString != "")
   {
      std::istringstream in(tempString.string());
      ossimString tempValue;
      for(idx = 0; idx < theAzimuthAngle.size();++idx)
      {
         in >> tempValue.string();
         theAzimuthAngle[idx] = tempValue.toDouble();
      }
   }


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

ossimIpt ossimPleiadesDimapSupportData::createIpt(const ossimString& s)const
{
   std::istringstream in(s.string());
   ossimString x, y;
   ossimString code;

   in >> x.string() >> y.string();

   return ossimIpt(x.toInt(), y.toInt());

}

bool ossimPleiadesDimapSupportData::parseProductInformation(
   ossimRefPtr<ossimXmlDocument> xmlDocument)
{
   ossimString xpath;
   vector<ossimRefPtr<ossimXmlNode> > xml_nodes;

   //---
   // Fetch the Image ID:
   //---
   xpath = "/Product_Information/Delivery_Identification/JOB_ID";
   xpath = theXmlDocumentRoot + xpath;
   if (!readOneXmlNode(xmlDocument, xpath, theImageID))
     {
     return false;
     }

   //---
   // Fetch the ProductionDate:
   //---
   xpath = "/Product_Information/Delivery_Identification/PRODUCTION_DATE";
   xpath = theXmlDocumentRoot + xpath;
   if (!readOneXmlNode(xmlDocument, xpath, theProductionDate))
     {
     return false;
     }

   return true;
}

bool ossimPleiadesDimapSupportData::parseDatasetContent(
   ossimRefPtr<ossimXmlDocument> xmlDocument)
{
   ossimString xpath, nodeValue;
   vector<ossimRefPtr<ossimXmlNode> > xml_nodes;

   //---
   // Corner points:
   //---
   xml_nodes.clear();
   xpath = "/Dataset_Content/Dataset_Extent/Vertex";
   xpath = theXmlDocumentRoot + xpath;

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
   theRefGroundPoint.hgt = 0.0; // TODO needs to be looked up

   xpath = "/Dataset_Content/Dataset_Extent/Center/LON";
   xpath = theXmlDocumentRoot + xpath;
   if (!readOneXmlNode(xmlDocument, xpath, nodeValue))
     {
     return false;
     }
   theRefGroundPoint.lon = nodeValue.toDouble();

   xpath = "/Dataset_Content/Dataset_Extent/Center/LAT";
   xpath = theXmlDocumentRoot + xpath;
   if (!readOneXmlNode(xmlDocument, xpath, nodeValue))
     {
     return false;
     }
   theRefGroundPoint.lat = nodeValue.toDouble();

   return true;
}

bool ossimPleiadesDimapSupportData::parseRadiometricData(
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
  xpath = "/Radiometric_Data/Radiometric_Calibration/Instrument_Calibration/Band_Measurement_List/Band_Radiance";
  xpath = theXmlDocumentRoot + xpath;
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
  xpath = "/Radiometric_Data/Radiometric_Calibration/Instrument_Calibration/Band_Measurement_List/Band_Solar_Irradiance";
  xpath = theXmlDocumentRoot + xpath;
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
        if (theNumBands != 1)
          bandIndex = 1;
        else
          bandIndex = 0;
      else
        if (bandName == "B2")
          if (theNumBands != 1)
            bandIndex = 2;
          else
            bandIndex = 0;
        else
          if (bandName == "B3")
            if (theNumBands != 1)
              bandIndex = 3;
            else
              bandIndex = 0;
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
          << "ossimPleiadesDimapSupportData::parseRadiometricMetadata ERROR: Band index outside of range 2\n";
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


bool ossimPleiadesDimapSupportData::parseRPCMetadata(
   ossimRefPtr<ossimXmlDocument> xmlDocument)
{
  static const char MODULE[] = "ossimPleiadesDimapSupportData::parseSensorMetadata";

  ossimString xpath, nodeValue;
  vector<ossimRefPtr<ossimXmlNode> > xml_nodes;

  //---
  // Fetch the Global RFM - Direct Model - Bias:
  //---
  xpath = "/Rational_Function_Model/Resource_Reference/RESOURCE_ID";
  xpath = theXmlDocumentRoot + xpath;
  if (!readOneXmlNode(xmlDocument, xpath, nodeValue))
    {
    return false;
    }
  theSpecId = nodeValue;

  //---
  // Fetch the Global RFM - Direct Model:
  //---
  xml_nodes.clear();
  xpath = "/Rational_Function_Model/Global_RFM/Direct_Model";
  xpath = theXmlDocumentRoot + xpath;
  xmlDocument->findNodes(xpath, xml_nodes);
  if (xml_nodes.size() == 0)
  {
    setErrorStatus();
    if(traceDebug())
    {
       ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " DEBUG:" << "\nCould not find: " << xpath << std::endl;
    }
    return false;
  }

  for (ossim_uint32 it = 1; it < 21; it ++)
    {
    std::ostringstream valueStr;
    valueStr << it;

    xml_nodes.clear();
    xpath = "/Rational_Function_Model/Global_RFM/Direct_Model/LINE_NUM_COEFF_";
    xpath = theXmlDocumentRoot + xpath;
    xpath = xpath + valueStr.str();
    xmlDocument->findNodes(xpath, xml_nodes);
    if (xml_nodes.size() == 0)
      {
        setErrorStatus();
        if(traceDebug())
        {
           ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " DEBUG:" << "\nCould not find: " << xpath << std::endl;
        }
        return false;
      }
    theLineNumCoeff.push_back(xml_nodes[0]->getText().toDouble());

    xml_nodes.clear();
    xpath = "/Rational_Function_Model/Global_RFM/Direct_Model/LINE_DEN_COEFF_";
    xpath = theXmlDocumentRoot + xpath;
    xpath = xpath + valueStr.str();
    xmlDocument->findNodes(xpath, xml_nodes);
    if (xml_nodes.size() == 0)
      {
        setErrorStatus();
        if(traceDebug())
        {
           ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " DEBUG:" << "\nCould not find: " << xpath << std::endl;
        }
        return false;
      }
    theLineDenCoeff.push_back(xml_nodes[0]->getText().toDouble());

    xml_nodes.clear();
    xpath = "/Rational_Function_Model/Global_RFM/Direct_Model/SAMP_NUM_COEFF_";
    xpath = theXmlDocumentRoot + xpath;
    xpath = xpath + valueStr.str();
    xmlDocument->findNodes(xpath, xml_nodes);
    if (xml_nodes.size() == 0)
      {
        setErrorStatus();
        if(traceDebug())
        {
           ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " DEBUG:" << "\nCould not find: " << xpath << std::endl;
        }
        return false;
      }
    theSampNumCoeff.push_back(xml_nodes[0]->getText().toDouble());

    xml_nodes.clear();
    xpath = "/Rational_Function_Model/Global_RFM/Direct_Model/SAMP_DEN_COEFF_";
    xpath = theXmlDocumentRoot + xpath;
    xpath = xpath + valueStr.str();
    xmlDocument->findNodes(xpath, xml_nodes);
    if (xml_nodes.size() == 0)
      {
        setErrorStatus();
        if(traceDebug())
        {
           ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " DEBUG:" << "\nCould not find: " << xpath << std::endl;
        }
        return false;
      }
    theSampDenCoeff.push_back(xml_nodes[0]->getText().toDouble());

    }

  //---
  // Fetch the Global RFM - Direct Model - Bias:
  //---
  xpath = "/Rational_Function_Model/Global_RFM/Direct_Model/ERR_BIAS_X";
  xpath = theXmlDocumentRoot + xpath;
  if (!readOneXmlNode(xmlDocument, xpath, nodeValue))
    {
    return false;
    }
  theErrBiasX = nodeValue.toDouble();

  xpath = "/Rational_Function_Model/Global_RFM/Direct_Model/ERR_BIAS_Y";
  xpath = theXmlDocumentRoot + xpath;
  if (!readOneXmlNode(xmlDocument, xpath, nodeValue))
    {
    return false;
    }
  theErrBiasY = nodeValue.toDouble();

  //---
  // Fetch the Global RFM validity parameters :
  //---
  xpath = "/Rational_Function_Model/Global_RFM/RFM_Validity/LONG_SCALE";
  xpath = theXmlDocumentRoot + xpath;
  if (!readOneXmlNode(xmlDocument, xpath, nodeValue))
    {
    return false;
    }
  theLonScale = nodeValue.toDouble();

  xpath = "/Rational_Function_Model/Global_RFM/RFM_Validity/LONG_OFF";
  xpath = theXmlDocumentRoot + xpath;
  if (!readOneXmlNode(xmlDocument, xpath, nodeValue))
    {
    return false;
    }
  theLonOffset = nodeValue.toDouble();

  xpath = "/Rational_Function_Model/Global_RFM/RFM_Validity/LAT_SCALE";
  xpath = theXmlDocumentRoot + xpath;
  if (!readOneXmlNode(xmlDocument, xpath, nodeValue))
    {
    return false;
    }
  theLatScale = nodeValue.toDouble();

  xpath = "/Rational_Function_Model/Global_RFM/RFM_Validity/LAT_OFF";
  xpath = theXmlDocumentRoot + xpath;
  if (!readOneXmlNode(xmlDocument, xpath, nodeValue))
    {
    return false;
    }
  theLatOffset = nodeValue.toDouble();

  xpath = "/Rational_Function_Model/Global_RFM/RFM_Validity/HEIGHT_SCALE";
  xpath = theXmlDocumentRoot + xpath;
  if (!readOneXmlNode(xmlDocument, xpath, nodeValue))
    {
    return false;
    }
  theHeightScale = nodeValue.toDouble();

  xpath = "/Rational_Function_Model/Global_RFM/RFM_Validity/HEIGHT_OFF";
  xpath = theXmlDocumentRoot + xpath;
  if (!readOneXmlNode(xmlDocument, xpath, nodeValue))
    {
    return false;
    }
  theHeightOffset = nodeValue.toDouble();

  xpath = "/Rational_Function_Model/Global_RFM/RFM_Validity/SAMP_SCALE";
  xpath = theXmlDocumentRoot + xpath;
  if (!readOneXmlNode(xmlDocument, xpath, nodeValue))
    {
    return false;
    }
  theSampScale = nodeValue.toDouble();

  xpath = "/Rational_Function_Model/Global_RFM/RFM_Validity/SAMP_OFF";
  xpath = theXmlDocumentRoot + xpath;
  if (!readOneXmlNode(xmlDocument, xpath, nodeValue))
    {
    return false;
    }
  theSampOffset = nodeValue.toInt32();

  xpath = "/Rational_Function_Model/Global_RFM/RFM_Validity/LINE_SCALE";
  xpath = theXmlDocumentRoot + xpath;
  if (!readOneXmlNode(xmlDocument, xpath, nodeValue))
    {
    return false;
    }
  theLineScale = nodeValue.toDouble();

  xpath = "/Rational_Function_Model/Global_RFM/RFM_Validity/LINE_OFF";
  xpath = theXmlDocumentRoot + xpath;
  if (!readOneXmlNode(xmlDocument, xpath, nodeValue))
    {
    return false;
    }
  theLineOffset = nodeValue.toInt32();

  return true;
}

bool ossimPleiadesDimapSupportData::parseMetadataIdentification(ossimRefPtr<ossimXmlDocument> xmlDocument)
{
  static const char MODULE[] = "ossimPleiadesDimapSupportData::parseMetadataIdentification";

  vector<ossimRefPtr<ossimXmlNode> > xml_nodes;
  ossimString xpath, nodeValue;
  theXmlDocumentRoot = "/DIMAP_Document";

  //---
  // Get the version string which can be used as a key for parsing.
  //---
  xml_nodes.clear();
  xpath = "/Metadata_Identification/METADATA_FORMAT";
  xpath = theXmlDocumentRoot + xpath;
  xmlDocument->findNodes(xpath, xml_nodes);
  if (xml_nodes.size() == 0)
    {
    // FIXME MSD: used to support peiades samples from SPOT-IMAGES website which are not coherent
    // with the last specification. Should be remove when first data will be available and sample
    // replaced.
    theXmlDocumentRoot = "/PHR_DIMAP_Document";
    if (traceDebug())
    {
      ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG:\nTry to use the old root: " << theXmlDocumentRoot << endl;
    }

    xml_nodes.clear();
    xpath = "/Metadata_Identification/METADATA_FORMAT";
    xpath = theXmlDocumentRoot + xpath;
    xmlDocument->findNodes(xpath, xml_nodes);
      if (xml_nodes.size() == 0)
        {
        setErrorStatus();
        if (traceDebug())
          {
          ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG:\nCould not find: " << xpath << endl;
          }
        return false;
        }
    }

  ossimString attribute = "version";
  ossimString value;
  xml_nodes[0]->getAttributeValue(value, attribute);
  if (value != "2.0")
    {
    setErrorStatus();
    if (traceDebug())
      {
      ossimNotify(ossimNotifyLevel_WARN)
          << "WARNING: DIMAP metadata version is not correct!" << std::endl;
      }
    return false;
    }

  //---
  // Check that it is a valid PHR DIMAPv2 file
  //---
  xpath = "/Metadata_Identification/METADATA_PROFILE";
  xpath = theXmlDocumentRoot + xpath;
  if (!readOneXmlNode(xmlDocument, xpath, nodeValue))
    {
    return false;
    }

  ossimString metadataProfile;
  if (  (nodeValue != "PHR_SENSOR")
     && (nodeValue != "PHR_ORTHO")
     && (nodeValue != "PHR_MOSAIC") )
    {
    if (traceDebug())
      {
      ossimNotify(ossimNotifyLevel_DEBUG)
          << "DEBUG:\n Not a PLEIADES DIMAPv2 file: metadata profile is incorrect!" << std::endl;
      }
    return false;
    }
  else
    metadataProfile = nodeValue;

  //---
  // Get the subprofile
  //---
  xpath = "/Metadata_Identification/METADATA_SUBPROFILE";
  xpath = theXmlDocumentRoot + xpath;
  if (!readOneXmlNode(xmlDocument, xpath, nodeValue))
    {
    return false;
    }

  if ((nodeValue == "PRODUCT"))
    theMetadataSubProfile = OSSIM_PLEIADES_METADATA_SUBPROFILE_PRODUCT;
  else if ( (nodeValue == "RPC") && (metadataProfile == "PHR_SENSOR") )
    theMetadataSubProfile = OSSIM_PLEIADES_METADATA_SUBPROFILE_RPC;
  else
    {
    theMetadataSubProfile = OSSIM_PLEIADES_METADATA_SUBPROFILE_UNKNOWN;
    if (traceDebug())
      {
      ossimNotify(ossimNotifyLevel_DEBUG)
           << "DEBUG:\n Not a PLEIADES DIMAPv2 file: metadata subprofile is incorrect !" << std::endl;
      }
    return false;
    }

  return true;
}

bool ossimPleiadesDimapSupportData::parseDatasetIdentification(ossimRefPtr<ossimXmlDocument> xmlDocument)
{
  return true;
}

bool ossimPleiadesDimapSupportData::parseCoordinateReferenceSystem(ossimRefPtr<ossimXmlDocument> xmlDocument)
{
  return true;
}

bool ossimPleiadesDimapSupportData::parseGeoposition(ossimRefPtr<ossimXmlDocument> xmlDocument)
{
  return true;
}

bool ossimPleiadesDimapSupportData::parseProcessingInformation(ossimRefPtr<ossimXmlDocument> xmlDocument)
{
  static const char MODULE[] = "ossimPleiadesDimapSupportData::parseProcessingInformation";

  vector<ossimRefPtr<ossimXmlNode> > xml_nodes;
  ossimString xpath, nodeValue;

  //---
  // Fetch the Processing Level:
  //---
  xpath = "/Processing_Information/Product_Settings/PROCESSING_LEVEL";
  xpath = theXmlDocumentRoot + xpath;
  if (!readOneXmlNode(xmlDocument, xpath, theProcessingLevelString))
    {
    return false;
    }

  //---
  // Fetch the Spectral Processing:
  //---
  xpath = "/Processing_Information/Product_Settings/SPECTRAL_PROCESSING";
  xpath = theXmlDocumentRoot + xpath;
  if (!readOneXmlNode(xmlDocument, xpath, theSpectralProcessingString))
    {
    return false;
    }

  return true;
}

bool  ossimPleiadesDimapSupportData::parseRasterData(ossimRefPtr<ossimXmlDocument> xmlDocument)
{
  static const char MODULE[] = "ossimPleiadesDimapSupportData::parseRasterData";
  vector<ossimRefPtr<ossimXmlNode> > xml_nodes;
  ossimString xpath, nodeValue;

  //---
  // Fetch if the product file is linked to one or many JP2 files:
  //---
  xpath = "/Raster_Data/Data_Access/DATA_FILE_TILES";
  xpath = theXmlDocumentRoot + xpath;
  if (!readOneXmlNode(xmlDocument, xpath, nodeValue))
    {
    return false;
    }
  theMultiDataFile.setValue(nodeValue);

  //---
  // Fetch the MegaImageSize:
  //---
  xpath = "/Raster_Data/Raster_Dimensions/NCOLS";
  xpath = theXmlDocumentRoot + xpath;
  if (!readOneXmlNode(xmlDocument, xpath, nodeValue))
    {
    return false;
    }
  theImageSize.samp = nodeValue.toInt();

  xpath = "/Raster_Data/Raster_Dimensions/NROWS";
  xpath = theXmlDocumentRoot + xpath;
  if (!readOneXmlNode(xmlDocument, xpath, nodeValue))
    {
    return false;
    }
  theImageSize.line = nodeValue.toInt();

  if (theMultiDataFile.getBoolean())
   {
   //---
   // Fetch the Number of MegaTiles:
   //---
    xpath = "/Raster_Data/Raster_Dimensions/Tile_Set/NTILES";
    xpath = theXmlDocumentRoot + xpath;
    if (!readOneXmlNode(xmlDocument, xpath, nodeValue))
      {
      return false;
      }
    theNumberOfMegaTiles = nodeValue.toUInt32();

   //---
   // Fetch the Number of MegaTiles in X and Y:
   //---
   xml_nodes.clear();
   xpath = "/Raster_Data/Raster_Dimensions/Tile_Set/Regular_Tiling/NTILES_COUNT";
   xpath = theXmlDocumentRoot + xpath;
   xmlDocument->findNodes(xpath, xml_nodes);
   if (xml_nodes.size() == 0)
     {
     setErrorStatus();
     if (traceDebug())
       {
       ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " DEBUG: \nCould not find: " << xpath << std::endl;
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
       ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " DEBUG: \nIncoherent number of tiles: " << xpath << std::endl;
       }
     return false;
     }

   //---
   // Fetch the size of MegaTiles:
   //---
   xml_nodes.clear();
   xpath = "/Raster_Data/Raster_Dimensions/Tile_Set/Regular_Tiling/NTILES_SIZE";
   xpath = theXmlDocumentRoot + xpath;
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

  //--- TODO_MSD is it useful in the case of RPC model ???
  // We will make the RefImagePoint the zero base center of the image.  This
  // is used by the ossimSensorModel::worldToLineSample iterative loop as
  // the starting point.  Since the ossimSensorModel does not know of the
  // sub image we make it zero base. (comments from spot)
  //---
  theRefImagePoint.line = theImageSize.line / 2.0;
  theRefImagePoint.samp = theImageSize.samp / 2.0;

  //---
  // Fetch number of bands
  //---
  xpath = "/Raster_Data/Raster_Dimensions/NBANDS";
  xpath = theXmlDocumentRoot + xpath;
  if (!readOneXmlNode(xmlDocument, xpath, nodeValue))
    {
    return false;
    }
  theNumBands = nodeValue.toUInt32();

  if (traceDebug())
    {
    ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " DEBUG:" << "\nNumber of bands: " << theNumBands << std::endl;
    }

  //---
  // Fetch Band Display Order
  //---
  xml_nodes.clear();
  xpath = "/Raster_Data/Raster_Display/Band_Display_Order/RED_CHANNEL";
  xpath = theXmlDocumentRoot + xpath;
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
    xpath = "/Raster_Data/Raster_Display/Band_Display_Order/GREEN_CHANNEL";
    xpath = theXmlDocumentRoot + xpath;
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
    xpath = "/Raster_Data/Raster_Display/Band_Display_Order/BLUE_CHANNEL";
    xpath = theXmlDocumentRoot + xpath;
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
      xpath = "/Raster_Data/Raster_Display/Band_Display_Order/ALPHA_CHANNEL";
      xpath = theXmlDocumentRoot + xpath;
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

  return true;
}

bool  ossimPleiadesDimapSupportData::parseGeometricData(ossimRefPtr<ossimXmlDocument> xmlDocument)
{
  ossimString xpath;
  vector<ossimRefPtr<ossimXmlNode> > xml_nodes;

  xml_nodes.clear();
  xpath = "/Geometric_Data/Use_Area/Located_Geometric_Values";
  xpath = theXmlDocumentRoot + xpath;
  xmlDocument->findNodes(xpath, xml_nodes);
  if (xml_nodes.size() != 3 )
     {
        setErrorStatus();
        if(traceDebug())
        {
           ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG:\nCould not find: " << xpath << std::endl;
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
          ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG:\nCould not find: " << xpath << std::endl;
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
          ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG:\nCould not find: " << xpath << std::endl;
       }
       return false;
    }
    theSunElevation.push_back(sub_nodes[0]->getText().toDouble());

    //---
    // Fetch the Incidence Angle:
    //---
    sub_nodes.clear();
    xpath = "Acquisition_Angles/INCIDENCE_ANGLE";
    (*node)->findChildNodes(xpath, sub_nodes);
    if (sub_nodes.size() == 0)
      {
      setErrorStatus();
      if(traceDebug())
        {
        ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG:\nCould not find: " << xpath << std::endl;
        }
      return false;
      }
    theIncidenceAngle.push_back(sub_nodes[0]->getText().toDouble());

    //---
    // Fetch the Viewing Angle:
    //---
    sub_nodes.clear();
    xpath = "Acquisition_Angles/VIEWING_ANGLE";
    (*node)->findChildNodes(xpath, sub_nodes);
    if (sub_nodes.size() == 0)
      {
      setErrorStatus();
      if(traceDebug())
        {
        ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG:\nCould not find: " << xpath << std::endl;
        }
        return false;
      }
    theViewingAngle.push_back(sub_nodes[0]->getText().toDouble());

    //---
    // Fetch the Azimuth Angle:
    //---
    sub_nodes.clear();
    xpath = "Acquisition_Angles/AZIMUTH_ANGLE";
    (*node)->findChildNodes(xpath, sub_nodes);
    if (sub_nodes.size() == 0)
      {
      setErrorStatus();
      if(traceDebug())
        {
        ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG:\nCould not find: " << xpath << std::endl;
        }
        return false;
      }
    theAzimuthAngle.push_back(sub_nodes[0]->getText().toDouble());

    ++node;
    }

  return true;
}

bool ossimPleiadesDimapSupportData::parseQualityAssessment(ossimRefPtr<ossimXmlDocument> xmlDocument)
{
  return true;
}

bool ossimPleiadesDimapSupportData::parseDatasetSources(ossimRefPtr<ossimXmlDocument> xmlDocument)
{
  static const char MODULE[] = "ossimPleiadesDimapSupportData::parseDatasetSources";
  ossimString xpath, nodeValue;
  vector<ossimRefPtr<ossimXmlNode> > xml_nodes;

  //---
  // Fetch the mission index (1A ou 1B) ?
  // and generate theSensorID
  //---
  xpath = "/Dataset_Sources/Source_Identification/Strip_Source/MISSION";
  xpath = theXmlDocumentRoot + xpath;
  if (!readOneXmlNode(xmlDocument, xpath, nodeValue))
    {
    return false;
    }

  if (nodeValue != "PHR")
    {
    setErrorStatus();
    if (traceDebug())
      {
      ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG:\nNot a PHR mission!"<< std::endl;
      }
    return false;
    }

  //---
  // Fetch the mission index (1A ou 1B) ?
  // and generate theSensorID
  //---
  xpath = "/Dataset_Sources/Source_Identification/Strip_Source/MISSION_INDEX";
  xpath = theXmlDocumentRoot + xpath;
  if (!readOneXmlNode(xmlDocument, xpath, nodeValue))
    {
    return false;
    }

  if (nodeValue == "1A")
    theSensorID = "PHR 1A";
  else if (nodeValue == "1B")
    theSensorID = "PHR 1B";
  else
    {
    setErrorStatus();
    if (traceDebug())
      {
      ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG:\nNot a valid sensorID!"<< std::endl;
      }
    return false;
    }

  //---
  // Fetch the Instrument:
  //---
  xpath = "/Dataset_Sources/Source_Identification/Strip_Source/INSTRUMENT";
  xpath = theXmlDocumentRoot + xpath;
  if (!readOneXmlNode(xmlDocument, xpath, theInstrument))
    {
    return false;
    }

  //---
  // Fetch the Instrument Index:
  //---
  xpath = "/Dataset_Sources/Source_Identification/Strip_Source/INSTRUMENT_INDEX";
  xpath = theXmlDocumentRoot + xpath;
  if (!readOneXmlNode(xmlDocument, xpath, theInstrumentIndex))
    {
    return false;
    }

  //---
  // Fetch the Imaging Date:
  //---
  xpath = "/Dataset_Sources/Source_Identification/Strip_Source/IMAGING_DATE";
  xpath = theXmlDocumentRoot + xpath;
  if (!readOneXmlNode(xmlDocument, xpath, theFirstLineImagingDate))
    {
    return false;
    }

   //---
   // Fetch the Imaging Time:
   //---
  xpath = "/Dataset_Sources/Source_Identification/Strip_Source/IMAGING_TIME";
  xpath = theXmlDocumentRoot + xpath;
  if (!readOneXmlNode(xmlDocument, xpath, theFirstLineImagingTime))
    {
    return false;
    }

  theAcquisitionDate = theFirstLineImagingDate + "T" + theFirstLineImagingTime;

  return true;
}

}
