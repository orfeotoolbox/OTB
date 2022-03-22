/*
 * Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "otbSarImageMetadataInterface.h"
#include "otbSentinel1ImageMetadataInterface.h"

#include "otbMacro.h"
#include "itkMetaDataObject.h"
#include "otbXMLMetadataSupplier.h"

#include "otbSentinel1CalibrationLookupData.h"
#include "otbSentinel1ThermalNoiseLookupData.h"

#include "itksys/SystemTools.hxx"
#include "otbSpatialReference.h"

// useful constants
#include <otbMath.h>
#include <iomanip>

namespace otb
{

bool Sentinel1ImageMetadataInterface::HasCalibrationLookupDataFlag(const MetadataSupplierInterface &mds) const
{
  return mds.GetAs<bool>(true, "support_data.calibration_lookup_flag");
}

bool Sentinel1ImageMetadataInterface::CreateCalibrationLookupData(SARCalib& sarCalib,
                                                                  const ImageMetadata& imd,
                                                                  const MetadataSupplierInterface& mds,
                                                                  const bool geom) const
{
  if(SarImageMetadataInterface::CreateCalibrationLookupData(sarCalib, imd, mds, geom))
    return true;

  std::stringstream prefix;
  prefix << "calibration.";
  if(!geom)
    prefix << "CalibrationVectorList.";
  const std::string sPrefixBase = prefix.str();

  auto firstLineTime = sarCalib.calibrationStartTime;
  auto lastLineTime = sarCalib.calibrationStopTime;
  otbMsgDevMacro(<< "calibration.startTime: " << std::setprecision(16) << firstLineTime);
  otbMsgDevMacro(<< "calibration.stopTime : " << std::setprecision(16) << lastLineTime);

  int numOfLines = 0;
  if(imd.Has(MDNum::NumberOfLines))
    numOfLines = imd[MDNum::NumberOfLines];
  else
  {
    for(auto const& band: imd.Bands)
    {
      if(band.Has(MDNum::NumberOfLines))
      {
        numOfLines = band[MDNum::NumberOfLines];
        break;
      }
    }
  }
  otbMsgDevMacro(<< "numOfLines   : " << numOfLines);

  const int count = mds.GetAs<int>(sPrefixBase + "count");
  std::vector<Sentinel1CalibrationStruct> sigmaCalibrationVectorList(count);
  std::vector<Sentinel1CalibrationStruct> betaCalibrationVectorList(count);
  std::vector<Sentinel1CalibrationStruct> gammaCalibrationVectorList(count);
  std::vector<Sentinel1CalibrationStruct> dnCalibrationVectorList(count);
  double                                  lastMJD = 0;

  for (int i = 0; i < count; i++)
  {
    Sentinel1CalibrationStruct sigmaCalibrationVector;

    prefix.str("");
    if(geom)
      prefix << sPrefixBase << "calibrationVector[" << i << "].";
    else
      prefix << sPrefixBase << "calibrationVector_" << i+1 << ".";
    const std::string sPrefix = prefix.str();

    sigmaCalibrationVector.line = mds.GetAs<int>(sPrefix + "line");

    // TODO: don't manipulate doubles, but ModifiedJulianDate for a better type
    // safety
    MetaData::TimePoint tmpDate;
    std::istringstream(mds.GetAs<std::string>(sPrefix + "azimuthTime")) >> tmpDate;
    sigmaCalibrationVector.timeMJD = tmpDate.GetModifiedJulianDay();
    sigmaCalibrationVector.deltaMJD = sigmaCalibrationVector.timeMJD - lastMJD;
    lastMJD                    = sigmaCalibrationVector.timeMJD;

    otbMsgDevMacro(<< sPrefix << "line   : " << sigmaCalibrationVector.line << " ;\t" << sPrefix << "timeMJD: " << std::setprecision(16) << sigmaCalibrationVector.timeMJD
                   << " (" << tmpDate << ")");

    sigmaCalibrationVector.pixels = mds.GetAsVector<int>(sPrefix + "pixel");

    // prepare deltaPixels vector
    int prev_pixels = 0;
    sigmaCalibrationVector.deltaPixels.resize(sigmaCalibrationVector.pixels.size());
    for (std::size_t p = 0, N = sigmaCalibrationVector.pixels.size(); p != N; ++p)
    {
      sigmaCalibrationVector.deltaPixels[p] = (sigmaCalibrationVector.pixels[p] - prev_pixels);
      prev_pixels                      = sigmaCalibrationVector.pixels[p];
    }

    Sentinel1CalibrationStruct betaCalibrationVector = sigmaCalibrationVector;
    Sentinel1CalibrationStruct gammaCalibrationVector = sigmaCalibrationVector;
    Sentinel1CalibrationStruct dnCalibrationVector = sigmaCalibrationVector;

    sigmaCalibrationVector.vect = mds.GetAsVector<float>(sPrefix + "sigmaNought");
    betaCalibrationVector.vect = mds.GetAsVector<float>(sPrefix + "betaNought");
    gammaCalibrationVector.vect = mds.GetAsVector<float>(sPrefix + "gamma");
    dnCalibrationVector.vect = mds.GetAsVector<float>(sPrefix + "dn");

    sigmaCalibrationVectorList[i] = sigmaCalibrationVector;
    betaCalibrationVectorList[i] = betaCalibrationVector;
    gammaCalibrationVectorList[i] = gammaCalibrationVector;
    dnCalibrationVectorList[i] = dnCalibrationVector;
  }

  Sentinel1CalibrationLookupData::Pointer sigmaSarLut = Sentinel1CalibrationLookupData::New();
  sigmaSarLut->InitParameters(SarCalibrationLookupData::SIGMA, firstLineTime.GetModifiedJulianDay(),
                              lastLineTime.GetModifiedJulianDay(), numOfLines, count, sigmaCalibrationVectorList);
  sarCalib.calibrationLookupData[SarCalibrationLookupData::SIGMA] = sigmaSarLut;

  Sentinel1CalibrationLookupData::Pointer betaSarLut = Sentinel1CalibrationLookupData::New();
  betaSarLut->InitParameters(SarCalibrationLookupData::BETA, firstLineTime.GetModifiedJulianDay(),
                             lastLineTime.GetModifiedJulianDay(), numOfLines, count, betaCalibrationVectorList);
  sarCalib.calibrationLookupData[SarCalibrationLookupData::BETA] = betaSarLut;

  Sentinel1CalibrationLookupData::Pointer gammaSarLut = Sentinel1CalibrationLookupData::New();
  gammaSarLut->InitParameters(SarCalibrationLookupData::GAMMA, firstLineTime.GetModifiedJulianDay(),
                              lastLineTime.GetModifiedJulianDay(), numOfLines, count, gammaCalibrationVectorList);
  sarCalib.calibrationLookupData[SarCalibrationLookupData::GAMMA] = gammaSarLut;

  Sentinel1CalibrationLookupData::Pointer dnSarLut = Sentinel1CalibrationLookupData::New();
  dnSarLut->InitParameters(SarCalibrationLookupData::DN, firstLineTime.GetModifiedJulianDay(),
                           lastLineTime.GetModifiedJulianDay(), numOfLines, count, dnCalibrationVectorList);
  sarCalib.calibrationLookupData[SarCalibrationLookupData::DN] = dnSarLut;

  return true;
}

struct NoiseVectorLists
{
  std::vector<Sentinel1CalibrationStruct>  rangeVectorList;
  std::vector<Sentinel1AzimuthNoiseStruct> azimuthVectorList;
};

NoiseVectorLists ReadNoiseVectorListsFromGeom(const MetadataSupplierInterface& mds)
{
  NoiseVectorLists output;

  double lastMJD = 0;
  
  // rangeCount, the number of vectors, is not stored in old geoms, so we iterate until the last vector
  // by testing the presence of the pixel_count field
  for (int listId = 0 ; mds.GetAs<std::string>("", "noise.noiseVector[" + std::to_string(listId) + "].pixel_count").size() ; ++listId)
  {
    // Path: noise.noiseVector[<listId>].{azimuthTime,line,noiseLut,pixel,pixel_count}
    const std::string prefix = "noise.noiseVector[" + std::to_string(listId) + "].";
    Sentinel1CalibrationStruct rangeNoiseVector;
    rangeNoiseVector.timeMJD = MetaData::ReadFormattedDate(mds.GetAs<std::string>(prefix+ "azimuthTime")).GetModifiedJulianDay();
    rangeNoiseVector.deltaMJD = rangeNoiseVector.timeMJD - lastMJD;

    lastMJD = rangeNoiseVector.timeMJD;

    rangeNoiseVector.line = mds.GetAs<int>(prefix + "line");
    Utils::ConvertStringToVector(mds.GetAs<std::string>( prefix + "pixel"), 
                                 rangeNoiseVector.pixels, prefix + "pixel");
    Utils::ConvertStringToVector(mds.GetAs<std::string>( prefix + "noiseLut"), 
                                 rangeNoiseVector.vect, prefix + "noiseLut");

    int prevPixel = 0;
    for (const auto & pixel: rangeNoiseVector.pixels)
    {
      rangeNoiseVector.deltaPixels.push_back(pixel - prevPixel);
      prevPixel = pixel;
    }
    output.rangeVectorList.push_back(rangeNoiseVector);
  }

  auto azimuthCount = mds.GetAs<int>(0, "noise.azimuthCount");
  for (int listId = 0; listId < azimuthCount; ++listId)
  {
    const std::string prefix = "noise.noiseAzimuthVector[" + std::to_string(listId) + "].";
    Sentinel1AzimuthNoiseStruct azimuthNoiseVector;
    azimuthNoiseVector.firstAzimuthLine = mds.GetAs<int>(prefix + "firstAzimuthLine");
    azimuthNoiseVector.firstRangeSample = mds.GetAs<int>(prefix + "firstRangeSample");
    azimuthNoiseVector.lastAzimuthLine = mds.GetAs<int>(prefix + "lastAzimuthLine");
    azimuthNoiseVector.lastRangeSample = mds.GetAs<int>(prefix + "lastRangeSample");

    Utils::ConvertStringToVector(mds.GetAs<std::string>( prefix + "line"), 
                               azimuthNoiseVector.lines, prefix + "line");
    Utils::ConvertStringToVector(mds.GetAs<std::string>( prefix + "noiseAzimuthLut"), 
                               azimuthNoiseVector.vect, prefix + "noiseAzimuthLut");
    output.azimuthVectorList.push_back(azimuthNoiseVector);
  }
  return output;
}

NoiseVectorLists ReadNoiseVectorListsFromXML(const MetadataSupplierInterface& mds)
{
  NoiseVectorLists output;

  std::string rangeNoisePrefix = "noise.noiseVectorList.";
  std::string rangeVectorName = "noiseVector_";
  std::string rangeLUTName = "noiseLut";

  // Number of entries in the vector
  int rangeCount = mds.GetAs<int>(0, rangeNoisePrefix + "count");

  // Starting with IPF 2.9.0, the noiseLut field has been renamed into noiseRangeVectorList, and the 
  // noiseAzimuthVectorList has been added. 
  // Ref: MPC-0392 DI-MPC-TN Issue 1.1 2017,Nov.28 Thermal Denoising of Products Generated by the S-1 IPF
  if (!rangeCount)
  {
    rangeNoisePrefix =  "noise.noiseRangeVectorList.";
    rangeVectorName = "noiseRangeVector_";
    rangeLUTName = "noiseRangeLut";

    rangeCount = mds.GetAs<int>(rangeNoisePrefix + "count");
  }

  double lastMJD = 0;
  for (int i = 0; i < rangeCount; i++)
  {
    const auto prefix = rangeNoisePrefix + rangeVectorName + std::to_string(i+1) + ".";
    Sentinel1CalibrationStruct rangeNoiseVector;

    rangeNoiseVector.timeMJD = MetaData::ReadFormattedDate(mds.GetAs<std::string>(prefix + "azimuthTime")).GetModifiedJulianDay();
    rangeNoiseVector.deltaMJD = rangeNoiseVector.timeMJD - lastMJD;
    lastMJD = rangeNoiseVector.timeMJD;
    rangeNoiseVector.line = mds.GetAs<int>(prefix + "line");
    Utils::ConvertStringToVector(mds.GetAs<std::string>( prefix + "pixel"), 
                                 rangeNoiseVector.pixels, prefix + "pixel");
    Utils::ConvertStringToVector(mds.GetAs<std::string>( prefix + rangeLUTName), 
                                 rangeNoiseVector.vect, prefix + rangeLUTName);

    int prevPixel = 0;
    for (const auto & pixel: rangeNoiseVector.pixels)
    {
      rangeNoiseVector.deltaPixels.push_back(pixel - prevPixel);
      prevPixel = pixel;
    }
    output.rangeVectorList.push_back(rangeNoiseVector);
  }

  std::string azimuthNoisePrefix = "noise.noiseAzimuthVectorList.";
  std::string azimuthVectorName = "noiseAzimuthVector";
  std::string azimuthLUTName = "noiseAzimuthLut";

  const int azimuthCount = mds.GetAs<int>(0, azimuthNoisePrefix + "count");

  for (int i = 0; i < azimuthCount; i++)
  {
    const auto prefix = azimuthCount == 1
                      ? azimuthNoisePrefix + azimuthVectorName + "."
                      : azimuthNoisePrefix + azimuthVectorName + "_" + std::to_string(i+1) + ".";

    Sentinel1AzimuthNoiseStruct azimuthNoiseVector;
    azimuthNoiseVector.firstAzimuthLine = mds.GetAs<int>(prefix + "firstAzimuthLine");
    azimuthNoiseVector.firstRangeSample = mds.GetAs<int>(prefix + "firstRangeSample");
    azimuthNoiseVector.lastAzimuthLine = mds.GetAs<int>(prefix + "lastAzimuthLine");
    azimuthNoiseVector.lastRangeSample = mds.GetAs<int>(prefix + "lastRangeSample");

    Utils::ConvertStringToVector(mds.GetAs<std::string>( prefix + "line"), 
                               azimuthNoiseVector.lines, prefix + "line");
    Utils::ConvertStringToVector(mds.GetAs<std::string>( prefix + azimuthLUTName), 
                               azimuthNoiseVector.vect, prefix + azimuthLUTName);
    output.azimuthVectorList.push_back(azimuthNoiseVector);

  }

  return output;
}

bool Sentinel1ImageMetadataInterface::CreateThermalNoiseLookupData(SARCalib& sarCalib,
                                                                  const ImageMetadata& imd,
                                                                  const MetadataSupplierInterface& mds,
                                                                  const bool geom) const
{
  auto firstLineTime = sarCalib.calibrationStartTime.GetModifiedJulianDay();
  auto lastLineTime = sarCalib.calibrationStopTime.GetModifiedJulianDay();

  int numOfLines = 0;
  if(imd.Has(MDNum::NumberOfLines))
  {
    numOfLines = imd[MDNum::NumberOfLines];
  }
  else
  {
    otbGenericExceptionMacro(MissingMetadataException,
           << "the NumberOfLines metadata is missing from the input metadata dictionary");
  }
  NoiseVectorLists vectorLists;

  if (geom)
  {
    vectorLists = ReadNoiseVectorListsFromGeom(mds);
  }
  else
  {
    vectorLists = ReadNoiseVectorListsFromXML(mds);
  }

  std::vector<Sentinel1CalibrationStruct> rangeNoiseVectorList;
  std::vector<Sentinel1AzimuthNoiseStruct> azimuthNoiseVectorList;

  Sentinel1ThermalNoiseLookupData::Pointer noiseSarLut = Sentinel1ThermalNoiseLookupData::New();
  noiseSarLut->InitParameters(firstLineTime,
                             lastLineTime,
                             numOfLines,
                             vectorLists.rangeVectorList,
                             vectorLists.azimuthVectorList);

  sarCalib.calibrationLookupData[SarCalibrationLookupData::NOISE] = noiseSarLut;

  return true;
}

double Sentinel1ImageMetadataInterface::GetCenterIncidenceAngle(const MetadataSupplierInterface &) const
{
  return 0;
}

std::vector<AzimuthFmRate> Sentinel1ImageMetadataInterface::GetAzimuthFmRate(const XMLMetadataSupplier &xmlMS) const
{
  std::vector<AzimuthFmRate> azimuthFmRateVector;
  // Number of entries in the vector
  int listCount = xmlMS.GetAs<int>("product.generalAnnotation.azimuthFmRateList.count");
  // This streams wild hold the iteration number
  std::ostringstream oss;
  for (int listId = 1 ; listId <= listCount ; ++listId)
  {
    oss.str("");
    oss << listId;
    // Base path to the data, that depends on the iteration number
    const std::string path_root = listCount == 1 ? "product.generalAnnotation.azimuthFmRateList.azimuthFmRate"
                                                 : "product.generalAnnotation.azimuthFmRateList.azimuthFmRate_" + oss.str();

    AzimuthFmRate afr;
    std::istringstream(xmlMS.GetAs<std::string>(path_root + ".azimuthTime")) >> afr.azimuthTime;
    afr.t0 = xmlMS.GetAs<double>(path_root + ".t0");
    afr.azimuthFmRatePolynomial = xmlMS.GetAsVector<double>(path_root + ".azimuthFmRatePolynomial",
        ' ', xmlMS.GetAs<int>(path_root + ".azimuthFmRatePolynomial.count"));
    azimuthFmRateVector.push_back(afr);
  }
  return azimuthFmRateVector;
}

std::vector<DopplerCentroid> Sentinel1ImageMetadataInterface::GetDopplerCentroid(const XMLMetadataSupplier &xmlMS) const
{
  std::vector<DopplerCentroid> dopplerCentroidVector;
  // Number of entries in the vector
  int listCount = xmlMS.GetAs<int>("product.dopplerCentroid.dcEstimateList.count");
  // This streams wild hold the iteration number
  std::ostringstream oss;
  for (int listId = 1 ; listId <= listCount ; ++listId)
  {
    oss.str("");
    oss << listId;
    // Base path to the data, that depends on the iteration number
    const std::string path_root = listCount == 1 ? "product.dopplerCentroid.dcEstimateList.dcEstimate"
                                                 : "product.dopplerCentroid.dcEstimateList.dcEstimate_" + oss.str();

    DopplerCentroid dopplerCent;
    std::istringstream(xmlMS.GetAs<std::string>(path_root + ".azimuthTime")) >> dopplerCent.azimuthTime;
    dopplerCent.t0 = xmlMS.GetAs<double>(path_root + ".t0");
    dopplerCent.dopCoef = xmlMS.GetAsVector<double>(path_root + ".dataDcPolynomial",
                  ' ', xmlMS.GetAs<int>(path_root + ".dataDcPolynomial.count"));
    dopplerCent.geoDopCoef = xmlMS.GetAsVector<double>(path_root + ".geometryDcPolynomial",
                  ' ', xmlMS.GetAs<int>(path_root + ".geometryDcPolynomial.count"));
    dopplerCentroidVector.push_back(dopplerCent);
  }
  return dopplerCentroidVector;
}

std::vector<Orbit> Sentinel1ImageMetadataInterface::GetOrbits(const XMLMetadataSupplier &xmlMS) const
{
  std::vector<Orbit> orbitVector;
  // Number of entries in the vector
  int listCount = xmlMS.GetAs<int>("product.generalAnnotation.orbitList.count");
  // This streams wild hold the iteration number
  std::ostringstream oss;
  for (int listId = 1 ; listId <= listCount ; ++listId)
  {
    oss.str("");
    oss << listId;
    // Base path to the data, that depends on the iteration number
    const std::string path_root = listCount == 1 ? "product.generalAnnotation.orbitList.orbit"
                                                 : "product.generalAnnotation.orbitList.orbit_" + oss.str();

    Orbit orbit;

    orbit.time = MetaData::ReadFormattedDate(xmlMS.GetAs<std::string>(path_root + ".time"));

    orbit.position[0] = xmlMS.GetAs<double>(path_root + ".position.x");
    orbit.position[1] = xmlMS.GetAs<double>(path_root + ".position.y");
    orbit.position[2] = xmlMS.GetAs<double>(path_root + ".position.z");
    orbit.velocity[0] = xmlMS.GetAs<double>(path_root + ".velocity.x");
    orbit.velocity[1] = xmlMS.GetAs<double>(path_root + ".velocity.y");
    orbit.velocity[2] = xmlMS.GetAs<double>(path_root + ".velocity.z");
    orbitVector.push_back(orbit);
  }
  return orbitVector;
}

std::vector<BurstRecord> Sentinel1ImageMetadataInterface::GetBurstRecords(const XMLMetadataSupplier &xmlMS, const MetaData::Duration& azimuthTimeInterval) const
{
  std::vector<BurstRecord> burstRecords;

  const std::string prefix = "product.swathTiming.";
  // number of burst records
  int numberOfBursts = xmlMS.GetAs<int>(0, prefix + "burstList.count");

  if (!numberOfBursts)
  {
    BurstRecord record;

    record.azimuthStartTime = MetaData::ReadFormattedDate(
              xmlMS.GetAs<std::string>("product.imageAnnotation.imageInformation.productFirstLineUtcTime"));
    record.azimuthStopTime = MetaData::ReadFormattedDate(
              xmlMS.GetAs<std::string>("product.imageAnnotation.imageInformation.productLastLineUtcTime"));

    record.startLine = 0;
    record.endLine = xmlMS.GetAs<int>("product.imageAnnotation.imageInformation.numberOfLines") - 1;
    record.startSample = 0;
    record.endSample = xmlMS.GetAs<int>("product.imageAnnotation.imageInformation.numberOfSamples") - 1;

    record.azimuthAnxTime = 0.;

    burstRecords.push_back(record);
  }
  else
  {
    const auto linesPerBurst = xmlMS.GetAs<int>(prefix + "linesPerBurst"); 
    const auto samplesPerBurst = xmlMS.GetAs<int>(prefix + "samplesPerBurst");

    for (int i = 0; i<numberOfBursts; i++)
    {
      const std::string burstPath = prefix + "burstList.burst_" + std::to_string(i+1) + ".";

      BurstRecord record;

      auto azimuthTime = MetaData::ReadFormattedDate(xmlMS.GetAs<std::string>(burstPath + "azimuthTime"));

      auto firstValidSamples = xmlMS.GetAsVector<int>(burstPath + "firstValidSample");
      int firstValidSample = 0, lastValidSample = samplesPerBurst - 1;
      int firstValid = 0, lastValid = firstValidSamples.size();
      int currentIndex = 0;
      bool firstIndexFound = false;
      for (const auto & elem : firstValidSamples)
      {
        if (elem != -1)
        {
          if (!firstIndexFound)
          {
            firstIndexFound = true;
            firstValid = currentIndex;
          }

          lastValid = currentIndex;

          if (elem < samplesPerBurst && elem > firstValidSample)
          {
            firstValidSample = elem;
          }
        }

        currentIndex++;
      }
      for (const auto & elem : xmlMS.GetAsVector<int>(burstPath + "lastValidSample"))
      {
        if (elem != -1 && elem < lastValidSample)
        {
          lastValidSample = elem;
        }
      }

      //TODO: in OssimPlugins lastValid is the first sample index after the last valid one. Is this correct ?
      lastValid += 1;

      record.startLine = i*linesPerBurst + firstValid;
      record.endLine = i*linesPerBurst + lastValid;
      record.startSample = firstValidSample;
      record.endSample = lastValidSample;
      record.azimuthAnxTime = xmlMS.GetAs<double>(burstPath + "azimuthAnxTime");

      record.azimuthStartTime = azimuthTime + firstValid * azimuthTimeInterval;
      record.azimuthStopTime  = azimuthTime + lastValid * azimuthTimeInterval;

      burstRecords.push_back(record);
    }
  }

  return burstRecords;
}


std::vector<CoordinateConversionRecord> Sentinel1ImageMetadataInterface::GetCoordinateConversionRecord(const XMLMetadataSupplier &xmlMS,
                                                                                                      const std::string & rg0_path,
                                                                                                      const std::string & coeffs_path) const
{
  std::vector<CoordinateConversionRecord> records;

  const std::string prefixPath = "product.coordinateConversion.coordinateConversionList.";

  auto listCount = xmlMS.GetAs<int>(prefixPath + "count");

  for (int i = 0; i < listCount; i++)
  {
    CoordinateConversionRecord coordinateConversionRecord;
    const std::string recordPath = prefixPath +"coordinateConversion_" + std::to_string(i+1) + ".";

    coordinateConversionRecord.azimuthTime = MetaData::ReadFormattedDate(
                                xmlMS.GetAs<std::string>(recordPath + "azimuthTime"));


    coordinateConversionRecord.rg0 = xmlMS.GetAs<double>(recordPath + rg0_path);
    coordinateConversionRecord.coeffs = xmlMS.GetAsVector<double>(recordPath + coeffs_path, ' ');

    records.push_back(coordinateConversionRecord);
  }

  return records;
}

double Sentinel1ImageMetadataInterface::getBandTerrainHeight(const XMLMetadataSupplier &xmlMS) const
{
  double heightSum = 0.0;
  // Number of entries in the vector
  int listCount = xmlMS.GetAs<int>("product.generalAnnotation.terrainHeightList.count");
  // This streams wild hold the iteration number
  std::ostringstream oss;
  for (int listId = 1 ; listId <= listCount ; ++listId)
  {
    oss.str("");
    oss << listId;
    // Base path to the data, that depends on the iteration number
    std::string path_root = "product.generalAnnotation.terrainHeightList.terrainHeight_" + oss.str();
    heightSum += xmlMS.GetAs<double>(path_root + ".value");
  }
  return heightSum / (double)listCount;
}

void ReadGCP(const XMLMetadataSupplier &AnnotationMS, 
              Projection::GCPParam &gcp, 
              SARParam& sarParam)
{
  std::stringstream ss;

  // Get the ellipsoid and semi-major, semi-minor axes
  if(AnnotationMS.HasValue("product.imageAnnotation.processingInformation.ellipsoidName"))
  {
    auto ellipsoidID = AnnotationMS.GetAs<std::string>("product.imageAnnotation.processingInformation.ellipsoidName");
    auto minor_axis = AnnotationMS.GetAs<double>(0, "product.imageAnnotation.processingInformation.ellipsoidSemiMinorAxis");
    auto major_axis = AnnotationMS.GetAs<double>(0, "product.imageAnnotation.processingInformation.ellipsoidSemiMajorAxis");
    if(ellipsoidID.empty() || minor_axis == 0 || major_axis == 0)
    {
      otbGenericExceptionMacro(MissingMetadataException, << "Cannot read GCP's spatial reference");
    }
    else if(ellipsoidID == "WGS84")
    {
      gcp.GCPProjection = SpatialReference::FromWGS84().ToWkt();
    }
    else
    {
      gcp.GCPProjection = SpatialReference::FromGeogCS("", "", ellipsoidID, major_axis,
                                                       major_axis/(major_axis - minor_axis)).ToWkt();
    }
  }

  auto GCPCount = AnnotationMS.GetAs<unsigned int>(0, "product.geolocationGrid.geolocationGridPointList.count");
  // Count the gcps if the given count value is invalid
  if(GCPCount == 0)
    GCPCount = AnnotationMS.GetNumberOf("product.geolocationGrid.geolocationGridPointList.geolocationGridPoint");
  // Put some reasonable limits of the number of gcps
  if(GCPCount > 5000)
    GCPCount = 5000;

  for(unsigned int i = 1 ; i <= GCPCount ; ++i)
  {
    ss.str("");
    ss << "product.geolocationGrid.geolocationGridPointList.geolocationGridPoint_" << i << ".";
    const auto azimuthTime = MetaData::ReadFormattedDate(AnnotationMS.GetAs<std::string>(ss.str() + "azimuthTime"));
    double line = 0.;
    // In TOPSAR products, GCPs are weird (they fall in black lines
    // between burst. This code allows moving them to a valid area of
    // the image.
    if (sarParam.burstRecords.size() >= 2)
    {
      otb::MetaData::TimePoint acqStart;
      unsigned long acqStartLine = 0;

      auto doesContain = [&azimuthTime](const BurstRecord & br)
        {return br.azimuthStartTime <= azimuthTime && azimuthTime < br.azimuthStopTime;};

      const auto bIt = std::find_if(sarParam.burstRecords.crbegin(), sarParam.burstRecords.crend(), doesContain);

      if (bIt != sarParam.burstRecords.crend())
      {
        acqStart = bIt->azimuthStartTime;
        acqStartLine = bIt->startLine;
      }
      else if(azimuthTime < sarParam.burstRecords.front().azimuthStartTime)
      {
        acqStart = sarParam.burstRecords.front().azimuthStartTime;
        acqStartLine = sarParam.burstRecords.front().startLine;
      }
      else if (azimuthTime >= sarParam.burstRecords.front().azimuthStopTime)
      {
        acqStart = sarParam.burstRecords.back().azimuthStartTime;
        acqStartLine = sarParam.burstRecords.back().startLine;
      }

      auto timeSinceStart = azimuthTime - acqStart;

      line = timeSinceStart/sarParam.azimuthTimeInterval + acqStartLine;
    }
    else
    {
      line = AnnotationMS.GetAs<double>(ss.str() + "line");
    }

    gcp.GCPs.emplace_back(std::to_string(i),                                  // id
                          "",                                                 // info
                          AnnotationMS.GetAs<double>(ss.str() + "pixel"),     // col
                          line,                                               // row
                          AnnotationMS.GetAs<double>(ss.str() + "longitude"), // px
                          AnnotationMS.GetAs<double>(ss.str() + "latitude"),  // py
                          AnnotationMS.GetAs<double>(ss.str() + "height"));   // pz

    GCPTime time;
    time.azimuthTime = azimuthTime;
    time.slantRangeTime = AnnotationMS.GetAs<double>(ss.str() + "slantRangeTime"); 
    sarParam.gcpTimes[std::to_string(i)] = time;
  }
}

void Sentinel1ImageMetadataInterface::ReadSarParamAndGCPs(const XMLMetadataSupplier &AnnotationMS,
                                                          SARParam& sarParam,
                                                          Projection::GCPParam &gcp)
{
  std::unordered_map<std::string, GCPTime> gcpTimes;

  sarParam.azimuthFmRates = this->GetAzimuthFmRate(AnnotationMS);
  sarParam.dopplerCentroids = this->GetDopplerCentroid(AnnotationMS);
  sarParam.orbits = this->GetOrbits(AnnotationMS);

  sarParam.slantRangeToGroundRangeRecords = this->GetCoordinateConversionRecord(AnnotationMS, "sr0", "srgrCoefficients");
  sarParam.groundRangeToSlantRangeRecords = this->GetCoordinateConversionRecord(AnnotationMS, "gr0", "grsrCoefficients");

  sarParam.azimuthTimeInterval = MetaData::Duration::Seconds(
    AnnotationMS.GetAs<double>("product.imageAnnotation.imageInformation.azimuthTimeInterval")
    );

  sarParam.burstRecords = this->GetBurstRecords(AnnotationMS, sarParam.azimuthTimeInterval);

  sarParam.nearRangeTime = AnnotationMS.GetAs<double>("product.imageAnnotation.imageInformation.slantRangeTime");
  sarParam.rangeSamplingRate = AnnotationMS.GetAs<double>("product.generalAnnotation.productInformation.rangeSamplingRate");
  sarParam.rangeResolution = AnnotationMS.GetAs<double>("product.imageAnnotation.imageInformation.rangePixelSpacing");

  sarParam.numberOfLinesPerBurst = AnnotationMS.GetAs<unsigned long>("product.swathTiming.linesPerBurst");
  sarParam.numberOfSamplesPerBurst = AnnotationMS.GetAs<unsigned long>("product.swathTiming.samplesPerBurst");

  const std::string swathProcParams = "product.imageAnnotation.processingInformation.swathProcParamsList.swathProcParams.";

  sarParam.azimuthBandwidth = AnnotationMS.GetAs<double>(0., swathProcParams + "azimuthProcessing.processingBandwidth");
  sarParam.rangeBandwidth = AnnotationMS.GetAs<double>(0., swathProcParams + "rangeProcessing.processingBandwidth");
  
  // AzimuthSteeringRate for TOPSAR (IW) products
  if (sarParam.burstRecords.size() >= 2)
  {
    sarParam.azimuthSteeringRate = AnnotationMS.GetAs<double>("product.generalAnnotation.productInformation.azimuthSteeringRate");
  }

  sarParam.rightLookingFlag = true;

  // Fetch the GCP
  ReadGCP(AnnotationMS, gcp, sarParam);
}

void Sentinel1ImageMetadataInterface::ParseGdal(ImageMetadata & imd)
{
  auto imageFilePath = m_MetadataSupplierInterface->GetResourceFile();

  // If path is relative, find the full path to make sure we can retrieve the parent directory.
  if (!itksys::SystemTools::FileIsFullPath(imageFilePath))
  {
    imageFilePath = itksys::SystemTools::CollapseFullPath(imageFilePath);
  }

  auto imageFineName = itksys::SystemTools::GetFilenameWithoutExtension(imageFilePath);

  auto pos = imageFineName.find('-');
  imd.Add(MDStr::Instrument, itksys::SystemTools::UpperCase(imageFineName.substr(0, pos)));

  ++pos;
  auto pos2 = imageFineName.find('-', pos);
  imd.Add(MDStr::Swath, itksys::SystemTools::UpperCase(imageFineName.substr(pos, pos2-pos)));

  pos = pos2+1;
  pos2 = imageFineName.find('-', pos+1);
  imd.Add(MDStr::ProductType, itksys::SystemTools::UpperCase(imageFineName.substr(pos, pos2-pos)));

  pos = pos2+1;
  pos2 = imageFineName.find('-', pos+1);
  imd.Add(MDStr::Polarization, itksys::SystemTools::UpperCase(imageFineName.substr(pos, pos2-pos)));

  imd.Add(MDStr::Mode, imd[MDStr::Swath]);

  // Manifest file
  std::string ManifestFilePath = itksys::SystemTools::GetParentDirectory(itksys::SystemTools::GetParentDirectory(imageFilePath)) + "/manifest.safe";
  if (!itksys::SystemTools::FileExists(ManifestFilePath))
  {
    ManifestFilePath = itksys::SystemTools::GetParentDirectory(imageFilePath) + "/manifest.safe";
    if (!itksys::SystemTools::FileExists(ManifestFilePath))
    {
      otbGenericExceptionMacro(MissingMetadataException,
                               << "Cannot find the Sentinel-1 'manifest.safe' file");
    }
  }
  XMLMetadataSupplier ManifestMS(ManifestFilePath);

  imd.Add(MDStr::SensorID, ManifestMS.GetFirstAs<std::string>("xfdu:XFDU.metadataSection.metadataObject_#.metadataWrap.xmlData.safe:platform.safe:familyName") 
                         + ManifestMS.GetFirstAs<std::string>("xfdu:XFDU.metadataSection.metadataObject_#.metadataWrap.xmlData.safe:platform.safe:number"));

  imd.Add(MDTime::ProductionDate,
    MetaData::ReadFormattedDate(ManifestMS.GetFirstAs<std::string>("xfdu:XFDU.metadataSection.metadataObject_#.metadataWrap.xmlData.safe:processing.start")));
  imd.Add(MDTime::AcquisitionDate,
    MetaData::ReadFormattedDate(ManifestMS.GetFirstAs<std::string>("xfdu:XFDU.metadataSection.metadataObject_#.metadataWrap.xmlData.safe:acquisitionPeriod.safe:startTime")));
  imd.Add(MDStr::BeamMode,
    ManifestMS.GetFirstAs<std::string>(
      "xfdu:XFDU.metadataSection.metadataObject_#.metadataWrap.xmlData.safe:platform.safe:instrument.safe:extension.s1sarl1:instrumentMode.s1sarl1:mode"
    ));
  imd.Add(MDStr::BeamSwath,
    ManifestMS.GetFirstAs<std::string>(
      "xfdu:XFDU.metadataSection.metadataObject_#.metadataWrap.xmlData.safe:platform.safe:instrument.safe:extension.s1sarl1:instrumentMode.s1sarl1:swath"
    ));
  auto FIorganisation = ManifestMS.GetFirstAs<std::string>("xfdu:XFDU.metadataSection.metadataObject_#.metadataWrap.xmlData.safe:processing.safe:facility.organisation");
  auto FIsoftware = ManifestMS.GetFirstAs<std::string>("xfdu:XFDU.metadataSection.metadataObject_#.metadataWrap.xmlData.safe:processing.safe:facility.safe:software.name");
  auto FIversion = ManifestMS.GetFirstAs<std::string>("xfdu:XFDU.metadataSection.metadataObject_#.metadataWrap.xmlData.safe:processing.safe:facility.safe:software.version");
  imd.Add("FACILITY_IDENTIFIER", FIorganisation + " " + FIsoftware + " " + FIversion);

  // Annotation file
  auto AnnotationFileName = imageFineName + ".xml";
  std::string AnnotationFilePath =
      itksys::SystemTools::GetParentDirectory(itksys::SystemTools::GetParentDirectory(imageFilePath)) + "/annotation/" + AnnotationFileName;
  if (!itksys::SystemTools::FileExists(AnnotationFilePath))
  {
    otbGenericExceptionMacro(MissingMetadataException,<<"Missing Annotation file for image '"<<ManifestFilePath<<"'");
  }
  XMLMetadataSupplier AnnotationMS(AnnotationFilePath);

  imd.Add(MDTime::AcquisitionStartTime, MetaData::ReadFormattedDate(AnnotationMS.GetAs<std::string>("product.adsHeader.startTime")));
  imd.Add(MDTime::AcquisitionStopTime, MetaData::ReadFormattedDate(AnnotationMS.GetAs<std::string>("product.adsHeader.stopTime")));
  imd.Add(MDNum::LineSpacing, AnnotationMS.GetAs<double>("product.imageAnnotation.imageInformation.azimuthPixelSpacing"));
  imd.Add(MDStr::Mission, AnnotationMS.GetAs<std::string>("product.adsHeader.missionId"));
  imd.Add(MDStr::OrbitDirection, itksys::SystemTools::UpperCase(AnnotationMS.GetAs<std::string>("product.generalAnnotation.productInformation.pass")));
  imd.Add(MDNum::OrbitNumber, AnnotationMS.GetAs<double>("product.adsHeader.absoluteOrbitNumber"));
  imd.Add(MDNum::PixelSpacing, AnnotationMS.GetAs<double>("product.imageAnnotation.imageInformation.rangePixelSpacing"));

  Projection::GCPParam gcp;
  SARParam sarParam;
  ReadSarParamAndGCPs(AnnotationMS, sarParam, gcp);

  imd.Add(MDGeom::GCP, gcp);

  imd.Add(MDNum::NumberOfLines, AnnotationMS.GetAs<int>("product.imageAnnotation.imageInformation.numberOfLines"));
  imd.Add(MDNum::NumberOfColumns, AnnotationMS.GetAs<int>("product.imageAnnotation.imageInformation.numberOfSamples"));
  imd.Add(MDNum::AverageSceneHeight, this->getBandTerrainHeight(AnnotationFilePath));
  imd.Add(MDNum::RadarFrequency, AnnotationMS.GetAs<double>("product.generalAnnotation.productInformation.radarFrequency"));
  imd.Add(MDNum::PRF, AnnotationMS.GetAs<double>("product.imageAnnotation.imageInformation.azimuthFrequency"));

  // Calibration file
  std::string CalibrationFilePath =
      itksys::SystemTools::GetFilenamePath(AnnotationFilePath) + "/calibration/calibration-" + itksys::SystemTools::GetFilenameName(AnnotationFilePath);
  if (!itksys::SystemTools::FileExists(CalibrationFilePath))
  {
    otbGenericExceptionMacro(MissingMetadataException,<<"Missing Calibration file for image '"<<ManifestFilePath<<"'");
  }
  XMLMetadataSupplier CalibrationMS(CalibrationFilePath);
  imd.Add(MDNum::AbsoluteCalibrationConstant, CalibrationMS.GetAs<double>("calibration.calibrationInformation.absoluteCalibrationConstant"));

  // Noise file
  std::string NoiseFilePath =
      itksys::SystemTools::GetFilenamePath(AnnotationFilePath) + "/calibration/noise-" + itksys::SystemTools::GetFilenameName(AnnotationFilePath);
  if (!itksys::SystemTools::FileExists(NoiseFilePath))
  {
    otbGenericExceptionMacro(MissingMetadataException,<<"Missing Noise file for image '"<<ManifestFilePath<<"'");
  }

  imd.Add(MDGeom::SAR, sarParam);

  SARCalib sarCalib;
  std::istringstream(CalibrationMS.GetAs<std::string>("calibration.adsHeader.startTime")) >> sarCalib.calibrationStartTime;
  std::istringstream(CalibrationMS.GetAs<std::string>("calibration.adsHeader.stopTime")) >> sarCalib.calibrationStopTime;
  LoadRadiometricCalibrationData(sarCalib, *m_MetadataSupplierInterface, imd);
  CreateCalibrationLookupData(sarCalib, imd, CalibrationMS, false);

  XMLMetadataSupplier NoiseMS(NoiseFilePath);
  CreateThermalNoiseLookupData(sarCalib, imd, NoiseMS, false);
  imd.Add(MDGeom::SARCalib, sarCalib);
}

void Sentinel1ImageMetadataInterface::ParseGeom(ImageMetadata & imd)
{
  Fetch(MDTime::AcquisitionStartTime, imd, "support_data.first_line_time");
  Fetch(MDTime::AcquisitionStopTime, imd, "support_data.last_line_time");
  Fetch(MDNum::LineSpacing, imd, "support_data.azimuth_spacing");
  Fetch(MDNum::PixelSpacing, imd, "support_data.range_spacing");
  Fetch(MDStr::SensorID, imd, "sensor");
  Fetch(MDStr::Mode, imd, "header.swath");
  Fetch(MDNum::NumberOfLines, imd, "number_lines");
  Fetch(MDNum::NumberOfColumns, imd, "number_samples");
  Fetch(MDNum::AverageSceneHeight, imd, "support_data.avg_scene_height");
  Fetch(MDNum::RadarFrequency, imd, "support_data.radar_frequency");
  Fetch(MDNum::PRF, imd, "support_data.pulse_repetition_frequency");
  Fetch(MDNum::AbsoluteCalibrationConstant, imd, "calibration.absoluteCalibrationConstant");
  
  // Manifest data may not be present in geom file, but support data should be present in that case
  CheckFetch(MDStr::BeamMode, imd, "manifest_data.acquisition_mode") || CheckFetch(MDStr::BeamMode, imd, "support_data.acquisition_mode");
  CheckFetch(MDStr::BeamSwath, imd, "manifest_data.swath") || CheckFetch(MDStr::BeamSwath, imd, "support_data.swath");
  CheckFetch(MDStr::Instrument, imd, "manifest_data.instrument") || CheckFetch(MDStr::Instrument, imd, "support_data.instrument");
  CheckFetch(MDStr::Mission, imd, "manifest_data.instrument") || CheckFetch(MDStr::Mission, imd, "support_data.instrument");
  CheckFetch(MDStr::OrbitDirection, imd, "manifest_data.orbit_pass") || CheckFetch(MDStr::OrbitDirection, imd, "support_data.orbit_pass");
  CheckFetch(MDNum::OrbitNumber, imd, "manifest_data.abs_orbit") || CheckFetch(MDNum::OrbitNumber, imd, "support_data.abs_orbit");
  CheckFetch(MDStr::ProductType, imd, "manifest_data.product_type") || CheckFetch(MDStr::ProductType, imd, "support_data.product_type");
  CheckFetch(MDTime::ProductionDate, imd, "manifest_data.date") || CheckFetch(MDTime::ProductionDate, imd, "support_data.date");
  CheckFetch(MDTime::AcquisitionDate, imd, "manifest_data.image_date") || CheckFetch(MDTime::AcquisitionDate, imd, "support_data.image_date");
  CheckFetch("FACILITY_IDENTIFIER", imd, "manifest_data.Processing_system_identifier") || CheckFetch("FACILITY_IDENTIFIER", imd, "support_data.Processing_system_identifier");
  CheckFetch(MDStr::Swath, imd, "manifest_data.swath") || CheckFetch(MDStr::Swath, imd, "support_data.swath");

  CheckFetch(MDStr::Polarization, imd, "calibration.polarisation") || CheckFetch(MDStr::Polarization, imd, "header.polarisation");

  SARParam sarParam;
  auto hasSAR = GetSAR(sarParam);
  if (hasSAR)
  {
    // Old metadata (from geom) does not contain support_data.look_side for S1 products
    // Right looking flag is always true for S1 products => hard coded value, here
    sarParam.rightLookingFlag = true;
    imd.Add(MDGeom::SAR, sarParam);
  }

  SARCalib sarCalib;
  std::istringstream(m_MetadataSupplierInterface->GetAs<std::string>("calibration.startTime")) >> sarCalib.calibrationStartTime;
  std::istringstream(m_MetadataSupplierInterface->GetAs<std::string>("calibration.stopTime")) >> sarCalib.calibrationStopTime;
  LoadRadiometricCalibrationData(sarCalib, *m_MetadataSupplierInterface, imd);
  CreateCalibrationLookupData(sarCalib, imd, *m_MetadataSupplierInterface, true);
  CreateThermalNoiseLookupData(sarCalib, imd, *m_MetadataSupplierInterface, true);
  imd.Add(MDGeom::SARCalib, sarCalib);
}

void Sentinel1ImageMetadataInterface::Parse(ImageMetadata & imd)
{
  // Try to fetch the metadata from GDAL Metadata Supplier
  if (m_MetadataSupplierInterface->GetAs<std::string>("", "MISSION_ID").substr(0, 2) == "S1"
      || m_MetadataSupplierInterface->GetAs<std::string>("", "TIFFTAG_IMAGEDESCRIPTION").substr(0, 10) == "Sentinel-1")
    this->ParseGdal(imd);
  // Try to fetch the metadata from GEOM file
  else if (m_MetadataSupplierInterface->GetAs<std::string>("", "sensor") == "SENTINEL-1A" || m_MetadataSupplierInterface->GetAs<std::string>("", "sensor") == "SENTINEL-1B")
    this->ParseGeom(imd);
  // Failed to fetch the metadata
  else
    otbGenericExceptionMacro(MissingMetadataException,
           << "Not a Sentinel1 product");

  // Default display
  imd.Add(MDNum::RedDisplayChannel, 0);
  imd.Add(MDNum::GreenDisplayChannel, 0);
  imd.Add(MDNum::BlueDisplayChannel, 0);
}

} // end namespace otb
