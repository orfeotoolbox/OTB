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


#include "otbTerraSarXSarImageMetadataInterface.h"

#include "otbStringUtils.h"
#include "otbMath.h"
#include "itkIndex.h"
#include "itkMetaDataObject.h"
#include "otbSARMetadata.h"
#include "otbSpatialReference.h"

#include <boost/filesystem.hpp>
//TODO C++ 17 : use std::optional instead
#include <boost/optional.hpp>

#include "itksys/SystemTools.hxx"
#include "itksys/RegularExpression.hxx"

#include "otb_tinyxml.h"


namespace otb
{

namespace ExtractXMLFiles {

// Adapted from boost filesystem documentation : https://www.boost.org/doc/libs/1_53_0/libs/filesystem/doc/reference.html
std::vector<std::string> GetXMLFilesInDirectory(const std::string & directoryPath,
                                                bool getSubDirectories=true)
{
  std::vector<std::string> fileList;
  if ( !boost::filesystem::exists( directoryPath) )
  {
    return fileList;
  }
  else if (!boost::filesystem::is_directory(directoryPath))
  {
    fileList.push_back(directoryPath);
    return fileList;
  }
  // End iterator : default construction yields past-the-end
  for ( const auto & item : boost::make_iterator_range(boost::filesystem::directory_iterator(directoryPath), {}) )
  {
    if ( boost::filesystem::is_directory(item.status()) && getSubDirectories )
    {
      auto subDirList = GetXMLFilesInDirectory(item.path().string());
      fileList.insert(fileList.end(), subDirList.begin(), subDirList.end());
    }
    else
    {
      //Extract only XML files
      std::string extension = boost::filesystem::extension(item);
      if (extension ==".xml")
        fileList.push_back(item.path().string());
    }
  }

  return fileList;
}

std::string GetResourceFile(const std::string & directoryPath, std::string pattern, bool getSubDirectories=true)
{
  auto xmlFiles = GetXMLFilesInDirectory(directoryPath, getSubDirectories);
  itksys::RegularExpression reg;
  reg.compile(pattern);
  for (const auto & file : xmlFiles)
  {
    if (reg.find(boost::filesystem::path(file).filename().string()))
      return file ;
  }
  return std::string("");
}

}

unsigned int TerraSarXSarImageMetadataInterface::GetNumberOfNoiseRecords(const MetadataSupplierInterface& mds, const unsigned int polLayer) const
{
  if(polLayer == 0)
  {
    if(mds.HasValue("level1Product.noise.numberOfNoiseRecords"))
      return mds.GetAs<unsigned int>(std::numeric_limits<unsigned int>::quiet_NaN(), "level1Product.noise.numberOfNoiseRecords");
    else
      return mds.GetAs<unsigned int>(std::numeric_limits<unsigned int>::quiet_NaN(), "noise.numberOfNoiseRecords");
  }
  else
  {
    std::stringstream oss;
    oss << "level1Product.noise_" << polLayer << ".numberOfNoiseRecords";
    return mds.GetAs<unsigned int>(std::numeric_limits<unsigned int>::quiet_NaN(), oss.str());
  }
}

unsigned int TerraSarXSarImageMetadataInterface::GetNoisePolynomialDegrees(const unsigned int noiseRecord,
                                                                       const MetadataSupplierInterface& mds,
                                                                       const unsigned int polLayer) const
{
  std::ostringstream oss;
  if(polLayer == 0)
  {
    oss << "level1Product.noise.imageNoise_" << noiseRecord << ".noiseEstimate.polynomialDegree";
    if(mds.HasValue(oss.str()))
      return mds.GetAs<unsigned int>(oss.str());
    oss.str("");
    oss << "noise[" << (noiseRecord-1) << "]imageNoise.noiseEstimate.polynomialDegree";
    if(mds.HasValue(oss.str()))
      return mds.GetAs<unsigned int>(oss.str());
  }
  else
  {
    oss << "level1Product.noise_" << polLayer << ".imageNoise_" << noiseRecord << ".noiseEstimate.polynomialDegree";
    if(mds.HasValue(oss.str()))
      return mds.GetAs<unsigned int>(oss.str());
  }
  return std::numeric_limits<unsigned int>::quiet_NaN(); // Invalid value
}

TerraSarXSarImageMetadataInterface::DoubleVectorType
TerraSarXSarImageMetadataInterface::GetNoisePolynomialCoefficients(const unsigned int noiseRecord,
                                                               const MetadataSupplierInterface& mds,
                                                               const unsigned int polLayer) const
{
  unsigned int polDegs = this->GetNoisePolynomialDegrees(noiseRecord, mds, polLayer);

  DoubleVectorType   polCoef;
  polCoef.clear();

  std::ostringstream oss, oss2 , oss3;
  int shift = 0;
  if(polLayer == 0)
  {
    if (mds.HasValue("level1Product.noise.polLayer"))
    {
      oss << "level1Product.noise.imageNoise_";
      oss2 << ".noiseEstimate.coefficient_";
    }
    else
    {
      oss << "noise[";
      oss2 << "]imageNoise.noiseEstimate.coefficient[";
      oss3 << "]";
      shift = 1;
    }
  }
  else
  {
    oss << "level1Product.noise_" << polLayer << ".imageNoise_";
    oss2 << ".noiseEstimate.coefficient_";
  }
  const std::string path_start = oss.str();
  const std::string path_middle = oss2.str();
  const std::string path_end = oss3.str();
  const unsigned int const_shift = shift;

  // set <= condition because degree N means N+1 coeff
  for (unsigned int j = 1; j <= polDegs+1; ++j)
  {
    oss.str("");
    oss << path_start << (noiseRecord - const_shift) << path_middle << (j - const_shift) << path_end;
    polCoef.push_back(mds.GetAs<double>(oss.str()));

  }

  return polCoef;
}

double TerraSarXSarImageMetadataInterface::GetNoiseTimeUTC(const unsigned int noiseRecord,
                                                       const MetadataSupplierInterface& mds,
                                                       const unsigned int polLayer) const
{
  std::ostringstream oss;
  MetaData::TimePoint time, defaultTime;
  std::istringstream("2000-01-01T00:0:00.000000Z") >> defaultTime;

  if(polLayer == 0)
  {
    oss << "level1Product.noise.imageNoise_" << noiseRecord << ".timeUTC";
    if (mds.HasValue(oss.str()))
      std::istringstream(mds.GetAs<std::string>(oss.str())) >> time;
    oss.str("");
    oss << "noise[" << (noiseRecord-1) << "]imageNoise.timeUTC";
    std::istringstream(mds.GetAs<std::string>("2000-01-01T00:0:00.000000Z", oss.str())) >> time;
  }
  else
  {
    oss << "level1Product.noise_" << polLayer << ".imageNoise_" << noiseRecord << ".timeUTC";
    std::istringstream(mds.GetAs<std::string>("2000-01-01T00:0:00.000000Z", oss.str())) >> time;
  }
  if(time != defaultTime)
    return time.GetJulianDay();
  return std::numeric_limits<double>::quiet_NaN(); // Invalid value
}

double TerraSarXSarImageMetadataInterface::ConvertStringTimeUTCToJulianDay(const std::string& value) const
{
  std::vector<std::string> splitDate;
  boost::split(splitDate, value, boost::is_any_of("-T:Z"));

  int    year  = std::stoi(splitDate[0].c_str());
  int    month = std::stoi(splitDate[1].c_str());
  int    day   = std::stoi(splitDate[2].c_str());
  int    hour  = std::stoi(splitDate[3].c_str());
  int    minu  = std::stoi(splitDate[4].c_str());
  double sec   = std::stod(splitDate[5].c_str());

  // Conversion to julian day
  // according to http://en.wikipedia.org/wiki/Julian_day
  // division are integer divisions:
  int a = (14 - month) / 12;
  int y = year + 4800 - a;
  int m = month + 12 * a - 3;

  double julianDay = day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;

  // now, the division are NOT integer
  julianDay += hour / 24. + minu / 1440. + sec / 86400.;

  return julianDay;
}


double TerraSarXSarImageMetadataInterface::GetNoiseReferencePoint(const unsigned int noiseRecord,
                                                              const MetadataSupplierInterface& mds,
                                                              const unsigned int polLayer) const
{
  std::ostringstream oss;
  if(polLayer == 0)
  {
    oss << "level1Product.noise.imageNoise_" << noiseRecord << ".noiseEstimate.referencePoint";
    if(mds.HasValue(oss.str()))
      return mds.GetAs<double>(std::numeric_limits<double>::min(), oss.str());
    oss.str();
    oss << "noise[" << (noiseRecord+1) << "]imageNoise.noiseEstimate.referencePoint";
    return mds.GetAs<double>(std::numeric_limits<double>::min(), oss.str());
  }
  oss << "level1Product.noise_" << polLayer << ".imageNoise_" << noiseRecord << ".noiseEstimate.referencePoint";
  return mds.GetAs<double>(std::numeric_limits<double>::min(), oss.str());
}

unsigned int TerraSarXSarImageMetadataInterface::GetNumberOfCornerIncidenceAngles(const MetadataSupplierInterface& mds) const
{
  auto ret = mds.GetAs<unsigned int>(0, "sceneCoord.numberOfSceneCornerCoord");
  if (ret > 0)
    return ret;
  return mds.GetNumberOf("level1Product.productInfo.sceneInfo.sceneCornerCoord");;
}

double TerraSarXSarImageMetadataInterface::GetCenterIncidenceAngle(const MetadataSupplierInterface& mds) const
{
  if(mds.HasValue("level1Product.productInfo.sceneInfo.sceneCenterCoord.incidenceAngle"))
    return mds.GetAs<double>("level1Product.productInfo.sceneInfo.sceneCenterCoord.incidenceAngle");
  return mds.GetAs<double>("sceneCoord.sceneCenterCoord.incidenceAngle");
}

TerraSarXSarImageMetadataInterface::IndexType
TerraSarXSarImageMetadataInterface::GetCenterIncidenceAngleIndex(const MetadataSupplierInterface& mds) const
{
  TerraSarXSarImageMetadataInterface::IndexType it;
  if(mds.HasValue("level1Product.productInfo.sceneInfo.sceneCenterCoord.refRow"))
    it[1] = mds.GetAs<int>("level1Product.productInfo.sceneInfo.sceneCenterCoord.refRow");
  else
    it[1] = mds.GetAs<int>("sceneCoord.sceneCenterCoord.refRow");
  if(mds.HasValue("level1Product.productInfo.sceneInfo.sceneCenterCoord.refColumn"))
    it[0] = mds.GetAs<int>(std::numeric_limits<int>::min(), "level1Product.productInfo.sceneInfo.sceneCenterCoord.refColumn");
  else
    it[0] = mds.GetAs<int>("sceneCoord.sceneCenterCoord.refColumn");
  return it;
}

TerraSarXSarImageMetadataInterface::DoubleVectorType
TerraSarXSarImageMetadataInterface::GetCornersIncidenceAngles(const MetadataSupplierInterface& mds) const
{
  TerraSarXSarImageMetadataInterface::DoubleVectorType dv;
  std::ostringstream oss;
  auto nbAngles = this->GetNumberOfCornerIncidenceAngles(mds);
  for (unsigned int i = 0; i < nbAngles; ++i)
  {
    oss.str("");
    oss << "level1Product.productInfo.sceneInfo.sceneCornerCoord_" << (i+1) << ".incidenceAngle";
    if(mds.HasValue(oss.str()))
      dv.push_back(mds.GetAs<double>(oss.str()));
    else
    {
      oss.str("");
      oss << "sceneCoord.sceneCornerCoord[" << i << "].incidenceAngle";
      dv.push_back(mds.GetAs<double>(oss.str()));
    }
  }
  return dv;
}

TerraSarXSarImageMetadataInterface::IndexVectorType
TerraSarXSarImageMetadataInterface::GetCornersIncidenceAnglesIndex(const MetadataSupplierInterface & imd) const
{
  TerraSarXSarImageMetadataInterface::IndexVectorType iv;
  std::ostringstream oss;

  unsigned int nbAngles = this->GetNumberOfCornerIncidenceAngles(imd);
  for (unsigned int i = 0; i < nbAngles; ++i)
  {
    TerraSarXSarImageMetadataInterface::IndexType it;

    oss.str("");
    oss << "level1Product.productInfo.sceneInfo.sceneCornerCoord_" << (i+1) << ".refRow";
    if(!imd.HasValue(oss.str()))
    {
      oss.str("");
      oss << "sceneCoord.sceneCornerCoord[" << i << "].refRow";
    }
    it[1] = imd.GetAs<int>(oss.str());

    oss.str("");
    oss << "level1Product.productInfo.sceneInfo.sceneCornerCoord_" << (i+1) << ".refColumn";
    if(!imd.HasValue(oss.str()))
    {
      oss.str("");
      oss << "sceneCoord.sceneCornerCoord[" << i << "].refColumn";
    }
    it[0] = imd.GetAs<int>(oss.str());

    iv.push_back(std::move(it));
  }
  return iv;
}


double TerraSarXSarImageMetadataInterface::Horner(std::vector<double>& coefficients, const double tauMinusTauRef) const
{
  std::vector<double>::reverse_iterator coefIt = coefficients.rbegin();
  double                                res    = *(coefIt);
  ++coefIt;

  while (coefIt < coefficients.rend())
  {
    // Cumulate polynomial
    res = res * tauMinusTauRef + (*coefIt);
    ++coefIt;
  }
  return res;
}


TerraSarXSarImageMetadataInterface::PointSetPointer
TerraSarXSarImageMetadataInterface::GetRadiometricCalibrationNoise(const MetadataSupplierInterface& mds,
                                                               const ImageMetadata& imd, const std::string& bandName) const
{
  // Retrieve the polarisation layer
  unsigned int polLayer = 0;
  auto numberOfCalFactor = mds.GetNumberOf("level1Product.calibration.calibrationConstant");
  if(bandName != "")
  {
    std::stringstream oss;
    for(unsigned int idx = 1 ; idx <= numberOfCalFactor ; ++idx)
    {
      oss.str("");
      oss << "level1Product.noise_" << idx << ".polLayer";
      if(mds.GetAs<std::string>("", oss.str()) == bandName)
      {
        polLayer = idx;
        break;
      }
    }
  }

  assert(imd.Has(MDNum::NumberOfLines));
  assert(imd.Has(MDNum::NumberOfColumns));
  auto numberOfRows = static_cast<unsigned int>(imd[MDNum::NumberOfLines]);
  auto numberOfCols = static_cast<unsigned int>(imd[MDNum::NumberOfColumns]);

  double startTime = imd[MDTime::AcquisitionStartTime].GetJulianDay();
  double stopTime  = imd[MDTime::AcquisitionStopTime].GetJulianDay();

  RealType firstRangeTime = imd[MDNum::RangeTimeFirstPixel];
  RealType lastRangeTime  = imd[MDNum::RangeTimeLastPixel];

  PointSetPointer points = PointSetType::New();

  points->Initialize();
  unsigned int noPoint = 0;

  PointType p0;

  unsigned int numberOfNoiseRecords = this->GetNumberOfNoiseRecords(mds, polLayer);

  for (unsigned int noiseRecord = 1; noiseRecord <= numberOfNoiseRecords; ++noiseRecord)
  {
    double   currentNoiseTime   = this->GetNoiseTimeUTC(noiseRecord, mds, polLayer);
    RealType AzimutAcquisition  = (currentNoiseTime - startTime) * numberOfRows / (stopTime - startTime);
    RealType referencePointTime = this->GetNoiseReferencePoint(noiseRecord, mds, polLayer);

    std::vector<RealType> polynomialCoefficient;
    polynomialCoefficient = this->GetNoisePolynomialCoefficients(noiseRecord, mds, polLayer);

    p0[0] = AzimutAcquisition;

    for (unsigned int col = 0; col < numberOfCols; ++col)
    {
      RealType rangeTime      = col * (lastRangeTime - firstRangeTime) / (numberOfCols) + firstRangeTime;
      RealType tauMinusTauRef = rangeTime - referencePointTime;
      RealType value          = this->Horner(polynomialCoefficient, tauMinusTauRef);

      p0[1] = col;

      points->SetPoint(noPoint, p0);
      points->SetPointData(noPoint, value);
      ++noPoint;
    }
  }
  return points;
}

SarImageMetadataInterface::ArrayIndexType TerraSarXSarImageMetadataInterface::GetRadiometricCalibrationNoisePolynomialDegree() const
{
  return {2, 2};
}

TerraSarXSarImageMetadataInterface::PointSetPointer
TerraSarXSarImageMetadataInterface::GetRadiometricCalibrationIncidenceAngle(const MetadataSupplierInterface& imd) const
{
  PointSetPointer points = PointSetType::New();
  PointType       p0;

  double    centerIncidenceAngleValue = this->GetCenterIncidenceAngle(imd);
  IndexType centerIncidenceAngleIndex = this->GetCenterIncidenceAngleIndex(imd);

  DoubleVectorType cornerIncidenceAngleValue = this->GetCornersIncidenceAngles(imd);
  IndexVectorType  cornerIncidenceAngleIndex = this->GetCornersIncidenceAnglesIndex(imd);
  points->Initialize();
  unsigned int noPoint = 0;

  p0[0] = centerIncidenceAngleIndex[0];
  p0[1] = centerIncidenceAngleIndex[1];

  points->SetPoint(noPoint, p0);
  points->SetPointData(noPoint, centerIncidenceAngleValue * CONST_PI_180);
  ++noPoint;

  for (unsigned int i = 0; i < cornerIncidenceAngleIndex.size(); ++i)
  {

    p0[0] = cornerIncidenceAngleIndex.at(i)[0];
    p0[1] = cornerIncidenceAngleIndex.at(i)[1];

    points->SetPoint(noPoint, p0);
    points->SetPointData(noPoint, cornerIncidenceAngleValue[i] * CONST_PI_180);
    ++noPoint;
  }
  return points;
}

TerraSarXSarImageMetadataInterface::ArrayIndexType TerraSarXSarImageMetadataInterface::GetRadiometricCalibrationIncidenceAnglePolynomialDegree() const
{
  return {2, 1};
}

void TerraSarXSarImageMetadataInterface::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

InfoSceneCoord TerraSarXSarImageMetadataInterface::GetSceneCoord(const MetadataSupplierInterface& supplier, const std::string& path)
{
  InfoSceneCoord output;
  output.referenceRow = supplier.GetAs<unsigned long>(path + ".refRow");
  output.referenceColumn = supplier.GetAs<unsigned long>(path + ".refColumn");
  output.latitude = supplier.GetAs<double>(path + ".lat");
  output.longitude = supplier.GetAs<double>(path + ".lon");
  output.rangeTime = supplier.GetAs<double>(path + ".rangeTime");
  output.incidenceAngle = supplier.GetAs<double>(path + ".incidenceAngle");
  std::istringstream iss(supplier.GetAs<std::string>(path + ".azimuthTimeUTC"));
  if (!(iss >> output.azimuthTime))
  {
    otbGenericExceptionMacro(itk::ExceptionObject,
           << "Unable to decode " << supplier.GetAs<std::string>(path + ".azimuthTimeUTC"));
  }
  return output;
}

void ReadGeorefGCP(const otb::MetaData::TimePoint & azimuthTimeStart, const std::string & geoRefXmlFileName, ImageMetadata & imd, SARParam & param)
{
  Projection::GCPParam gcp;

  // Open the xml file
  TiXmlDocument doc(geoRefXmlFileName);
  if (!doc.LoadFile())
  {
    otbGenericExceptionMacro(MissingMetadataException, << "Can't open file " << geoRefXmlFileName);
  }

  TiXmlHandle   hDoc(&doc);
  auto sphereNode = hDoc.FirstChild("geoReference")
                                 .FirstChild("referenceFrames")
                                 .FirstChild("sphere").ToNode();
  if (sphereNode)
  {
    const std::string ellipsoidID = sphereNode->FirstChildElement("ellipsoidID")->GetText();
    const auto minor_axis = std::stod(sphereNode->FirstChildElement("semiMajorAxis")->GetText());
    const auto major_axis = std::stod(sphereNode->FirstChildElement("semiMinorAxis")->GetText());
    
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
  else
  {
    otbGenericExceptionMacro(MissingMetadataException, << "Cannot find the geoReference/referenceFrames/sphere node in " << geoRefXmlFileName);
  }

  auto gridNode = hDoc.FirstChild("geoReference")
                      .FirstChild("geolocationGrid").ToNode();

  if (gridNode)
  {
    // GCP id starts at 1
    unsigned int i = 1;
    for(TiXmlElement* gcpElem = gridNode->FirstChildElement("gridPoint")
        ; gcpElem != NULL
        ; gcpElem = gcpElem->NextSiblingElement("gridPoint"), i++)
    {
      const std::string id = std::to_string(i);
      gcp.GCPs.emplace_back(id,                                                          // id
                            "",                                                          // info
                            std::stod(gcpElem->FirstChildElement("col")->GetText()),     // col
                            std::stod(gcpElem->FirstChildElement("row")->GetText()),     // row
                            std::stod(gcpElem->FirstChildElement("lon")->GetText()),     // px
                            std::stod(gcpElem->FirstChildElement("lat")->GetText()),     // py
                            std::stod(gcpElem->FirstChildElement("height")->GetText())); // pz

      GCPTime time;
      auto deltaAz = MetaData::Duration::Seconds(std::stod(gcpElem->FirstChildElement("t")->GetText()));

      time.azimuthTime = azimuthTimeStart + deltaAz;
      time.slantRangeTime = param.nearRangeTime + std::stod(gcpElem->FirstChildElement("tau")->GetText()); 

      param.gcpTimes[id] = time;
    }
  }
  else
  {
    otbGenericExceptionMacro(MissingMetadataException, << "Cannot find the geoReference/geolocationGrid node in " << geoRefXmlFileName);
  }

  imd.Add(MDGeom::GCP, gcp);
}

void ReadSARSensorModel(const XMLMetadataSupplier & xmlMS,
                        const std::string& polarization,
                        SARParam & param)
{
  // Number of entries in the vector
  int listCount = xmlMS.GetAs<int>("level1Product.platform.orbit.orbitHeader.numStateVectors");

  const std::string dateFormat = "%Y-%m-%dT%H:%M:%S";

  // This streams wild hold the iteration number
  std::ostringstream oss;
  for (int listId = 1 ; listId <= listCount ; ++listId)
  {
    oss.str("");
    oss << listId;
    // Base path to the data, that depends on the iteration number
    std::string path_root = "level1Product.platform.orbit.stateVec_" + oss.str();
    Orbit orbit;
    
    orbit.time = MetaData::ReadFormattedDate(xmlMS.GetAs<std::string>(path_root + ".timeUTC"), dateFormat);

    orbit.position[0] = xmlMS.GetAs<double>(path_root + ".posX");
    orbit.position[1] = xmlMS.GetAs<double>(path_root + ".posY");
    orbit.position[2] = xmlMS.GetAs<double>(path_root + ".posZ");
    orbit.velocity[0] = xmlMS.GetAs<double>(path_root + ".velX");
    orbit.velocity[1] = xmlMS.GetAs<double>(path_root + ".velY");
    orbit.velocity[2] = xmlMS.GetAs<double>(path_root + ".velZ");
    param.orbits.push_back(orbit);
  }

  param.nearRangeTime = xmlMS.GetAs<double>("level1Product.productInfo.sceneInfo.rangeTime.firstPixel");

  if(xmlMS.HasValue("level1Product.instrument.settings.RSF"))
    param.rangeSamplingRate = xmlMS.GetAs<double>("level1Product.instrument.settings.RSF");
  else
  {
    // Retrieve the polarisation layer
    auto nbLayers = xmlMS.GetAs<unsigned int>("level1Product.productInfo.imageDataInfo.numberOfLayers");
    for(unsigned int band = 1 ; band <= nbLayers ; ++band)
    {
      oss.str("");
      oss << "level1Product.instrument.settings_" << band << ".polLayer";
      if(xmlMS.GetAs<std::string>(oss.str()) == polarization)
      {
        oss.str("");
        oss << "level1Product.instrument.settings_" << band << ".RSF";
        param.rangeSamplingRate = xmlMS.GetAs<double>(oss.str());
        break;
      }
    }
  }

  const auto azimuthTimeStart = MetaData::ReadFormattedDate(
      xmlMS.GetAs<std::string>("level1Product.productInfo.sceneInfo.start.timeUTC"), dateFormat);

  const auto azimuthTimeStop = MetaData::ReadFormattedDate(
      xmlMS.GetAs<std::string>("level1Product.productInfo.sceneInfo.stop.timeUTC"), dateFormat);

  const auto td = azimuthTimeStop - azimuthTimeStart;
  assert(td > MetaData::Duration::Seconds(0));

  const auto numberOfRows = xmlMS.GetAs<double>("level1Product.productInfo.imageDataInfo.imageRaster.numberOfRows");

  param.azimuthTimeInterval = td / numberOfRows;

  auto lookSide =  xmlMS.GetAs<std::string>("level1Product.productInfo.acquisitionInfo.lookDirection");

  if (lookSide != "RIGHT" && lookSide != "LEFT")
  {
    otbGenericExceptionMacro(MissingMetadataException, "Not an expected look side (only RIGHT or LEFT expected)");
  }

  param.rightLookingFlag = lookSide == "RIGHT";

  //For Terrasar-X only 1 burst is supported for now
  BurstRecord burstRecord;

  burstRecord.azimuthStartTime = azimuthTimeStart;
  burstRecord.azimuthStopTime = azimuthTimeStop;

  burstRecord.startLine = 0;
  burstRecord.endLine = numberOfRows - 1;

  burstRecord.startSample = 0;
  burstRecord.endSample = xmlMS.GetAs<double>("level1Product.productInfo.imageDataInfo.imageRaster.numberOfColumns") - 1;
  burstRecord.azimuthAnxTime = 0.;

  param.burstRecords.push_back(burstRecord);

  // Read Doppler centroid
  const auto xmlFileName = xmlMS.GetResourceFile();

  TiXmlDocument doc(xmlFileName);
  if (!doc.LoadFile())
  {
    otbGenericExceptionMacro(MissingMetadataException, << "Can't open file " << xmlFileName);
  }

  TiXmlHandle   hDoc(&doc);
  auto processingElem = hDoc.FirstChild("level1Product")
                            .FirstChild("processing").ToElement();

  auto centroidElem = processingElem->FirstChildElement("doppler")->FirstChildElement("dopplerCentroid");

  // Retrieve the polarisation layer
  for( ;centroidElem; centroidElem = centroidElem->NextSiblingElement("dopplerCentroid"))
  {
    if (centroidElem->FirstChildElement("polLayer")->GetText() == polarization)
      break;
  }
  if (!centroidElem)
  {
    otbGenericExceptionMacro(MissingMetadataException, << "Can't find the doppler centroid in the product metadata.");
  }

  for(auto dopplerEstimateElem = centroidElem->FirstChildElement("dopplerEstimate"); 
      dopplerEstimateElem; 
      dopplerEstimateElem = dopplerEstimateElem->NextSiblingElement("dopplerEstimate"))
  {
    DopplerCentroid centroid;

    centroid.azimuthTime = MetaData::ReadFormattedDate(dopplerEstimateElem->FirstChildElement("timeUTC")->GetText());

    auto combinedDopplerElem = dopplerEstimateElem->FirstChildElement("combinedDoppler");

    if (!combinedDopplerElem)
    {
      otbGenericExceptionMacro(MissingMetadataException, << "Can't find the combined doppler in the product metadata.");
    }

    centroid.t0 =  std::stod(combinedDopplerElem->FirstChildElement("referencePoint")->GetText());

    unsigned int polynomialDegree = std::stoi(combinedDopplerElem->FirstChildElement("polynomialDegree")->GetText());

    centroid.dopCoef.resize(polynomialDegree + 1);

    for(auto coefficientElem = combinedDopplerElem->FirstChildElement("coefficient"); 
      coefficientElem; 
      coefficientElem = coefficientElem->NextSiblingElement("coefficient"))
    {
      unsigned int exponent = 0;

      // operator <= is used because a polynomial of degree N has N+1 elements
      if (coefficientElem->QueryUnsignedAttribute("exponent", &exponent) == TIXML_SUCCESS && exponent <= polynomialDegree)
      {
        centroid.dopCoef[exponent] = std::stod(coefficientElem->GetText());
      }
    }

    param.dopplerCentroids.push_back(centroid);
  }

  // Read Azimuth FM rate
  for(auto dopplerRateElem = processingElem->FirstChildElement("geometry")->FirstChildElement("dopplerRate"); 
      dopplerRateElem; 
      dopplerRateElem = dopplerRateElem->NextSiblingElement("dopplerRate"))
  {
    AzimuthFmRate rate;

    rate.azimuthTime = MetaData::ReadFormattedDate(dopplerRateElem->FirstChildElement("timeUTC")->GetText());

    auto dopplerRatePolynomialElem = dopplerRateElem->FirstChildElement("dopplerRatePolynomial");

    if (!dopplerRatePolynomialElem)
    {
      otbGenericExceptionMacro(MissingMetadataException, << "Can't find the doppler rate polynomial in the product metadata.");
    }

    rate.t0 =  std::stod(dopplerRatePolynomialElem->FirstChildElement("referencePoint")->GetText());

    unsigned int polynomialDegree = std::stoi(dopplerRatePolynomialElem->FirstChildElement("polynomialDegree")->GetText());

    rate.azimuthFmRatePolynomial.resize(polynomialDegree + 1);

    for(auto coefficientElem = dopplerRatePolynomialElem->FirstChildElement("coefficient"); 
      coefficientElem; 
      coefficientElem = coefficientElem->NextSiblingElement("coefficient"))
    {
      unsigned int exponent = 0;

      // operator <= is used because a polynomial of degree N has N+1 elements
      if (coefficientElem->QueryUnsignedAttribute("exponent", &exponent) == TIXML_SUCCESS && exponent <= polynomialDegree)
      {
        rate.azimuthFmRatePolynomial[exponent] = std::stod(coefficientElem->GetText());
      }
    }

    param.azimuthFmRates.push_back(rate);
  }

  // Azimuth and range Bandwidth
  param.azimuthBandwidth = std::stod(processingElem->FirstChildElement("processingParameter")
                                                   ->FirstChildElement("totalProcessedAzimuthBandwidth")
                                                   ->GetText());
  param.rangeBandwidth = std::stod(processingElem->FirstChildElement("processingParameter")
                                                ->FirstChildElement("totalProcessedRangeBandwidth")
                                                ->GetText());
}


void TerraSarXSarImageMetadataInterface::ParseGdal(ImageMetadata &imd)
{
  std::stringstream oss;
  // Main XML file
  std::string MainDirectory = itksys::SystemTools::GetParentDirectory(
        itksys::SystemTools::GetParentDirectory(m_MetadataSupplierInterface->GetResourceFile("")));
  std::string MainFilePath =
      ExtractXMLFiles::GetResourceFile(MainDirectory, "(T[SD]X|PAZ)1_SAR__.*\\.xml", false);
  if (MainFilePath.empty())
  {
    otbGenericExceptionMacro(MissingMetadataException,
           << "Cannot find the TerraSar main XML file");
  }

  XMLMetadataSupplier MainXMLFileMetadataSupplier(MainFilePath);

  imd.Add(MDNum::NumberOfLines, MainXMLFileMetadataSupplier.GetAs<int>("level1Product.productInfo.imageDataInfo.imageRaster.numberOfRows"));
  imd.Add(MDNum::NumberOfColumns, MainXMLFileMetadataSupplier.GetAs<int>("level1Product.productInfo.imageDataInfo.imageRaster.numberOfColumns"));
  imd.Add(MDStr::OrbitDirection, MainXMLFileMetadataSupplier.GetAs<std::string>("level1Product.productInfo.missionInfo.orbitDirection"));
  imd.Add(MDNum::OrbitNumber, MainXMLFileMetadataSupplier.GetAs<double>("level1Product.productInfo.missionInfo.absOrbit"));
  imd.Add(MDNum::LineSpacing, MainXMLFileMetadataSupplier.GetAs<double>("level1Product.productSpecific.complexImageInfo.projectedSpacingAzimuth"));
  imd.Add(MDNum::PixelSpacing, MainXMLFileMetadataSupplier.GetAs<double>("level1Product.productSpecific.complexImageInfo.projectedSpacingRange.slantRange"));
  imd.Add(MDStr::Mission, MainXMLFileMetadataSupplier.GetAs<std::string>("level1Product.generalHeader.mission"));
  imd.Add(MDStr::SensorID, MainXMLFileMetadataSupplier.GetAs<std::string>("level1Product.generalHeader.mission"));

  // This sensorID corresponds to the sensorID read in OTB 7.3 and below, e.g. MGD_SE_SM_S, but OTB 7.4 read productVariant, i.e. MGD in this case
  const std::string productVariant = MainXMLFileMetadataSupplier.GetAs<std::string>("level1Product.productInfo.productVariantInfo.productVariant");
  imd.Add(MDStr::ProductType, MainXMLFileMetadataSupplier.GetAs<std::string>("level1Product.productInfo.productVariantInfo.productType"));
  imd.Add(MDStr::Mode, MainXMLFileMetadataSupplier.GetAs<std::string>("level1Product.productInfo.acquisitionInfo.imagingMode"));
  imd.Add(MDNum::RadarFrequency, MainXMLFileMetadataSupplier.GetAs<double>("level1Product.instrument.radarParameters.centerFrequency"));
  imd.Add(MDTime::AcquisitionStartTime, MetaData::ReadFormattedDate(MainXMLFileMetadataSupplier.GetFirstAs<std::string>("level1Product.productInfo.sceneInfo.start.timeUTC")));
  imd.Add(MDTime::AcquisitionStopTime, MetaData::ReadFormattedDate(MainXMLFileMetadataSupplier.GetFirstAs<std::string>("level1Product.productInfo.sceneInfo.stop.timeUTC")));
  imd.Add(MDNum::RangeTimeFirstPixel, MainXMLFileMetadataSupplier.GetFirstAs<double>("level1Product.productInfo.sceneInfo.rangeTime.firstPixel"));
  imd.Add(MDNum::RangeTimeLastPixel, MainXMLFileMetadataSupplier.GetFirstAs<double>("level1Product.productInfo.sceneInfo.rangeTime.lastPixel"));
  imd.Add(MDNum::PRF, MainXMLFileMetadataSupplier.GetAs<double>("level1Product.productSpecific.complexImageInfo.commonPRF"));
  imd.Add(MDNum::RSF, MainXMLFileMetadataSupplier.GetAs<double>("level1Product.productSpecific.complexImageInfo.commonRSF"));

  imd.Add(MDStr::Instrument, "TSX-SAR");

  // Polarisation
  auto polarization =
      itksys::SystemTools::GetFilenameName(m_MetadataSupplierInterface->GetResourceFile("")).substr(6, 2);
  imd.Add(MDStr::Polarization, polarization);

  auto numberOfCalFactor = MainXMLFileMetadataSupplier.GetNumberOf("level1Product.calibration.calibrationConstant");
  if(numberOfCalFactor == 1)
  {
    imd.Add(MDNum::CalFactor, MainXMLFileMetadataSupplier.GetAs<double>("level1Product.calibration.calibrationConstant.calFactor"));
    imd.Add(MDNum::CalScale, MainXMLFileMetadataSupplier.GetAs<double>("level1Product.calibration.calibrationConstant.calFactor"));
  }
  else
  {
    // Retrieve the polarisation layer
    for(unsigned int band = 1 ; band <= numberOfCalFactor ; ++band)
    {
      oss.str("");
      oss << "level1Product.calibration.calibrationConstant_" << band << ".polLayer";
      if(MainXMLFileMetadataSupplier.GetAs<std::string>(oss.str()) == polarization)
      {
        oss.str("");
        oss << "level1Product.calibration.calibrationConstant_" << band << ".calFactor";
        imd.Add(MDNum::CalFactor, MainXMLFileMetadataSupplier.GetAs<double>(oss.str()));
        imd.Add(MDNum::CalScale, MainXMLFileMetadataSupplier.GetAs<double>(oss.str()));
      }
    }
  }

  SARCalib sarCalib;
  std::istringstream("1970-01-01T00:00:00.000000") >> sarCalib.calibrationStartTime;
  std::istringstream("1970-01-01T00:00:00.000000") >> sarCalib.calibrationStopTime;

  LoadRadiometricCalibrationData(sarCalib, MainXMLFileMetadataSupplier, imd, polarization);
  sarCalib.calibrationLookupFlag = false;
  imd.Add(MDGeom::SARCalib, sarCalib);

  // Open the georef file containing GCPs
  XMLMetadataSupplier GCPXMLFileMS(MainDirectory + "/ANNOTATION/GEOREF.xml");

  SARParam sarParam;

  ReadSARSensorModel(MainXMLFileMetadataSupplier, polarization, sarParam);

  // Fetch the GCP (only for SSC products)
  if (productVariant == "SSC")
  {
    ReadGeorefGCP(MetaData::ReadFormattedDate(MainXMLFileMetadataSupplier.GetAs<std::string>("level1Product.productInfo.sceneInfo.start.timeUTC")), 
                  MainDirectory + "/ANNOTATION/GEOREF.xml",
                  imd,
                  sarParam);
  }

  // Scene coordinate
  sarParam.centerSceneCoord = GetSceneCoord(MainXMLFileMetadataSupplier, "level1Product.productInfo.sceneInfo.sceneCenterCoord");
  for (unsigned int i = 1 ; i <= MainXMLFileMetadataSupplier.GetNumberOf("level1Product.productInfo.sceneInfo.sceneCornerCoord") ; ++i)
  {
    oss.str("");
    oss << "level1Product.productInfo.sceneInfo.sceneCornerCoord_" << i;
    InfoSceneCoord isc = GetSceneCoord(MainXMLFileMetadataSupplier, oss.str());
    if (isc.referenceRow == 1 && isc.referenceColumn > 1)
      sarParam.urSceneCoord = isc;
    else if (isc.referenceRow == imd[MDNum::NumberOfLines] && isc.referenceColumn > 1)
      sarParam.lrSceneCoord = isc;
    else if (isc.referenceRow == imd[MDNum::NumberOfLines] && isc.referenceColumn == 1)
      sarParam.llSceneCoord = isc;
    else
      sarParam.ulSceneCoord = isc;
  }

  imd.Add(MDGeom::SAR, sarParam);
}

void TerraSarXSarImageMetadataInterface::ParseGeom(ImageMetadata & imd)
{
  Fetch(MDNum::LineSpacing, imd, "meters_per_pixel_y");
  Fetch(MDNum::PixelSpacing, imd, "meters_per_pixel_x");
  if (!CheckFetch(MDStr::Mode, imd, "acquisitionInfo.imagingMode"))  // TerraSAR-X geoms (OTB < 7.4)
    Fetch(MDStr::Mode, imd, "support_data.acquisition_mode");       // TerraSAR-X geoms (OTB >= 7.4)
  Fetch(MDStr::Mission, imd, "sensor");
  imd.Add(MDStr::Instrument, "TSX-SAR");
  if (!CheckFetch(MDStr::ProductType, imd, "product_type"))            // TerraSAR-X geoms (OTB < 7.4)
    Fetch(MDStr::ProductType, imd, "support_data.product_type");  // TerraSAR-X geoms (OTB >= 7.4)
  if (!CheckFetch(MDTime::AcquisitionStartTime, imd, "azimuth_start_time"))         // TerraSAR-X geoms (OTB < 7.4)
    Fetch(MDTime::AcquisitionStartTime, imd, "support_data.first_line_time");  // TerraSAR-X geoms (OTB >= 7.4)
  if (!CheckFetch(MDTime::AcquisitionStopTime, imd, "azimuth_stop_time"))          // TerraSAR-X geoms (OTB < 7.4)
    Fetch(MDTime::AcquisitionStopTime, imd, "support_data.last_line_time");  // TerraSAR-X geoms (OTB >= 7.4)
  if (!CheckFetch(MDNum::PRF, imd, "sensor_params.prf"))                // TerraSAR-X geoms (OTB < 7.4)
    Fetch(MDNum::PRF, imd, "support_data.pulse_repetition_frequency");  // TerraSAR-X geoms (OTB >= 7.4)
  Fetch(MDNum::CalFactor, imd, "calibration.calibrationConstant.calFactor");
  Fetch(MDNum::CalScale, imd, "calibration.calibrationConstant.calFactor");
  if (!CheckFetch(MDStr::Polarization, imd, "acquisitionInfo.polarisationList[0]"))  // TerraSAR-X geoms (OTB < 7.4)
    Fetch(MDStr::Polarization, imd, "header.polarisation");                          // TerraSAR-X geoms (OTB >= 7.4)
  Fetch(MDNum::NumberOfLines, imd, "number_lines");
  Fetch(MDNum::NumberOfColumns, imd, "number_samples");
  if (!CheckFetch(MDNum::RadarFrequency, imd, "radarFrequency"))        // TerraSAR-X geoms (OTB < 7.4)
    Fetch(MDNum::RadarFrequency, imd, "support_data.radar_frequency");  // TerraSAR-X geoms (OTB >= 7.4)
  Fetch(MDStr::OrbitDirection, imd, "support_data.orbit_pass");
  Fetch(MDNum::OrbitNumber, imd, "support_data.abs_orbit");
  if (!CheckFetch(MDNum::RangeTimeFirstPixel, imd, "support_data.slant_range_to_first_pixel"))  // TerraSAR-X geoms (OTB >= 7.4)
    Fetch(MDNum::RangeTimeFirstPixel, imd, "range_first_time");                                // TerraSAR-X geoms (OTB < 7.4)
  if (!CheckFetch(MDNum::RangeTimeLastPixel, imd, "support_data.slant_range_to_last_pixel"))  // TerraSAR-X geoms (OTB >= 7.4)
    Fetch(MDNum::RangeTimeLastPixel, imd, "range_last_time");                                // TerraSAR-X geoms (OTB < 7.4)
  if (!CheckFetch(MDNum::RSF, imd, "support_data.range_sampling_rate"))  // TerraSAR-X geoms (OTB >= 7.4)
    Fetch(MDNum::RSF, imd, "sensor_params.sampling_frequency");          // TerraSAR-X geoms (OTB < 7.4)
  Fetch(MDStr::SensorID, imd, "sensor");
  imd.Add(MDStr::Instrument, "TSX-SAR");

  //SAR Parameters
  SARParam sarParam;
  sarParam.orbits = this->GetOrbitsGeom();
  sarParam.azimuthFmRates = this->GetAzimuthFmRateGeom();
  sarParam.dopplerCentroids = this->GetDopplerCentroidGeom();

  // Right looking flag is always true for TSX products
  sarParam.rightLookingFlag = true;

  const std::string supportDataPrefix = "support_data.";
  sarParam.rangeSamplingRate = m_MetadataSupplierInterface->GetAs<double>(
                                supportDataPrefix + "range_sampling_rate");

  sarParam.nearRangeTime = m_MetadataSupplierInterface->GetAs<double>(
                                supportDataPrefix + "slant_range_to_first_pixel");

  sarParam.rangeResolution = m_MetadataSupplierInterface->GetAs<double>(
                                supportDataPrefix + "range_spacing");

  sarParam.azimuthTimeInterval = MetaData::Duration::Seconds(m_MetadataSupplierInterface->GetAs<double>(
                                supportDataPrefix + "line_time_interval") );


  //For Terrasar-X only 1 burst is supported for now
  BurstRecord burstRecord;

  burstRecord.azimuthStartTime = imd[MDTime::AcquisitionStartTime];
  burstRecord.azimuthStopTime = imd[MDTime::AcquisitionStopTime];

  burstRecord.startLine = 0;
  burstRecord.endLine = static_cast<int>(imd[MDNum::NumberOfLines]) - 1;

  burstRecord.startSample = 0;
  burstRecord.endSample = static_cast<int>(imd[MDNum::NumberOfColumns]) - 1;
  burstRecord.azimuthAnxTime = 0.;

  sarParam.burstRecords.push_back(burstRecord);

  // Scene coordinate
  sarParam.centerSceneCoord = GetSceneCoord(*m_MetadataSupplierInterface, "sceneCoord.sceneCenterCoord");
  for (unsigned int i = 0 ; i < m_MetadataSupplierInterface->GetAs<unsigned int>("sceneCoord.numberOfSceneCornerCoord") ; ++i)
  {
    std::stringstream oss;
    oss.str("");
    oss << "sceneCoord.sceneCornerCoord[" << i << "]";
    InfoSceneCoord isc = GetSceneCoord(*m_MetadataSupplierInterface, oss.str());
    if (isc.referenceRow == 1 && isc.referenceColumn > 1)
      sarParam.urSceneCoord = isc;
    else if (isc.referenceRow == imd[MDNum::NumberOfLines] && isc.referenceColumn > 1)
      sarParam.lrSceneCoord = isc;
    else if (isc.referenceRow == imd[MDNum::NumberOfLines] && isc.referenceColumn == 1)
      sarParam.llSceneCoord = isc;
    else
      sarParam.ulSceneCoord = isc;
  }

  imd.Add(MDGeom::SAR, sarParam);

  SARCalib sarCalib;
  LoadRadiometricCalibrationData(sarCalib, *m_MetadataSupplierInterface, imd);
  sarCalib.calibrationLookupFlag = false;
  imd.Add(MDGeom::SARCalib, sarCalib);
}

void TerraSarXSarImageMetadataInterface::Parse(ImageMetadata & imd)
{
  // Try to fetch the metadata from GEOM file
  if (m_MetadataSupplierInterface->GetAs<std::string>("", "sensor") == "TSX-1" ||
      m_MetadataSupplierInterface->GetAs<std::string>("", "sensor") == "PAZ-1" ||
      m_MetadataSupplierInterface->GetAs<std::string>("", "sensor") == "TDX-1")
    this->ParseGeom(imd);
  // Try to fetch the metadata from GDAL Metadata Supplier
  else
    this->ParseGdal(imd);

  // Default display
  imd.Add(MDNum::RedDisplayChannel, 0);
  imd.Add(MDNum::GreenDisplayChannel, 0);
  imd.Add(MDNum::BlueDisplayChannel, 0);
}


} // end namespace otb
