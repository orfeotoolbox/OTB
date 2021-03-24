/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#include "otbMath.h"
#include "itkMetaDataObject.h"
#include "otbImageKeywordlist.h"
#include "otbSARMetadata.h"

namespace otb
{

SarImageMetadataInterface::SarImageMetadataInterface()
{
}

const std::string SarImageMetadataInterface::GetProductType() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
  {
    itkExceptionMacro(<< "Invalid Metadata");
  }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  if (imageKeywordlist.HasKey("support_data.product_type"))
  {
    const std::string product_type = imageKeywordlist.GetMetadataByKey("support_data.product_type");
    return product_type;
  }
  return "";
}

const std::string SarImageMetadataInterface::GetAcquisitionMode() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
  {
    itkExceptionMacro(<< "Invalid Metadata");
  }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  if (imageKeywordlist.HasKey("support_data.acquisition_mode"))
  {
    const std::string acquisition_mode = imageKeywordlist.GetMetadataByKey("support_data.acquisition_mode");
    return acquisition_mode;
  }
  return "";
}

void SarImageMetadataInterface::CreateCalibrationLookupData(const short itkNotUsed(t))
{
}

const SarImageMetadataInterface::LookupDataPointerType SarImageMetadataInterface::GetCalibrationLookupData(const short type)
{
  if (HasCalibrationLookupDataFlag())
  {
    CreateCalibrationLookupData(type);
  }

  return m_SarLut;
}

bool SarImageMetadataInterface::HasCalibrationLookupDataFlag() const
{
  const ImageKeywordlist& imageKeywordlist = this->GetImageKeywordlist();
  /* checking if the key exist is more than enough */
  return imageKeywordlist.HasKey("support_data.calibration_lookup_flag");
}

SarImageMetadataInterface::RealType SarImageMetadataInterface::GetRadiometricCalibrationScale() const
{
  return static_cast<SarImageMetadataInterface::RealType>(1.0);
}

SarImageMetadataInterface::PointSetPointer SarImageMetadataInterface::GetConstantValuePointSet(const RealType& value) const
{
  PointSetPointer pointSet = PointSetType::New();

  PointType p0;

  pointSet->Initialize();

  p0[0] = static_cast<unsigned int>(0);
  p0[1] = static_cast<unsigned int>(0);
  pointSet->SetPoint(0, p0);
  pointSet->SetPointData(0, value);

  return pointSet;
}

SarImageMetadataInterface::PointSetPointer SarImageMetadataInterface::GetRadiometricCalibrationNoise() const
{
  return SarImageMetadataInterface::GetConstantValuePointSet(0.0);
}

SarImageMetadataInterface::PointSetPointer SarImageMetadataInterface::GetRadiometricCalibrationAntennaPatternNewGain() const
{
  return SarImageMetadataInterface::GetConstantValuePointSet(1.0);
}


SarImageMetadataInterface::PointSetPointer SarImageMetadataInterface::GetRadiometricCalibrationAntennaPatternOldGain() const
{
  return SarImageMetadataInterface::GetConstantValuePointSet(1.0);
}


SarImageMetadataInterface::PointSetPointer SarImageMetadataInterface::GetRadiometricCalibrationIncidenceAngle() const
{
  return SarImageMetadataInterface::GetConstantValuePointSet(CONST_PI_2);
}


SarImageMetadataInterface::PointSetPointer SarImageMetadataInterface::GetRadiometricCalibrationRangeSpreadLoss() const
{
  return SarImageMetadataInterface::GetConstantValuePointSet(1.0);
}


SarImageMetadataInterface::IndexType SarImageMetadataInterface::GetConstantPolynomialDegree() const
{
  IndexType polynomialDegree;
  polynomialDegree[0] = 0;
  polynomialDegree[1] = 0;
  return polynomialDegree;
}

double SarImageMetadataInterface::GetRescalingFactor() const
{
  return 1.0;
}
SarImageMetadataInterface::IndexType SarImageMetadataInterface::GetRadiometricCalibrationNoisePolynomialDegree() const
{
  return SarImageMetadataInterface::GetConstantPolynomialDegree();
}

SarImageMetadataInterface::IndexType SarImageMetadataInterface::GetRadiometricCalibrationAntennaPatternNewGainPolynomialDegree() const
{
  return SarImageMetadataInterface::GetConstantPolynomialDegree();
}

SarImageMetadataInterface::IndexType SarImageMetadataInterface::GetRadiometricCalibrationAntennaPatternOldGainPolynomialDegree() const
{
  return SarImageMetadataInterface::GetConstantPolynomialDegree();
}


SarImageMetadataInterface::IndexType SarImageMetadataInterface::GetRadiometricCalibrationIncidenceAnglePolynomialDegree() const
{
  return SarImageMetadataInterface::GetConstantPolynomialDegree();
}


SarImageMetadataInterface::IndexType SarImageMetadataInterface::GetRadiometricCalibrationRangeSpreadLossPolynomialDegree() const
{
  return SarImageMetadataInterface::GetConstantPolynomialDegree();
}

std::vector<AzimuthFmRate> SarImageMetadataInterface::GetAzimuthFmRateGeom(const MetadataSupplierInterface & mds) const
{
  std::vector<AzimuthFmRate> azimuthFmRateVector;
  // Number of entries in the vector
  int listCount = mds.GetAs<int>("azimuthFmRate.azi_fm_rate_coef_nb_list");
  // This streams wild hold the iteration number
  std::ostringstream oss;
  for (int listId = 1 ; listId <= listCount ; ++listId)
  {
    oss.str("");
    oss << listId;
    // Base path to the data, that depends on the iteration number
    std::string path_root = "azimuthFmRate.azi_fm_rate_coef_list" + oss.str();
    AzimuthFmRate afr;
    std::istringstream(mds.GetAs<std::string>(path_root + ".azi_fm_rate_coef_time")) >> afr.azimuthTime;
    afr.t0 = mds.GetAs<double>(path_root + ".slant_range_time");
    std::vector<double> polynom(3);
    for (int polyId = 1 ; polyId < 4 ; ++polyId)
      polynom.push_back(mds.GetAs<double>(path_root+"."+std::to_string(polyId)+".azi_fm_rate_coef"));
    afr.azimuthFmRatePolynomial = std::move(polynom);
    azimuthFmRateVector.push_back(std::move(afr));
  }
  return azimuthFmRateVector;
}

std::vector<DopplerCentroid> SarImageMetadataInterface::GetDopplerCentroidGeom(const MetadataSupplierInterface & mds) const
{
  std::vector<DopplerCentroid> dopplerCentroidVector;
  // Path: dopplerCentroid.dop_coef_list<listId>.{dop_coef_time,slant_range_time,{1,2,3}.dop_coef}
  // This streams wild hold the iteration number
  std::ostringstream oss;
  for (int listId = 1 ;
       mds.GetAs<std::string>("", std::string("dopplerCentroid.dop_coef_list")+std::to_string(listId)+std::string(".slant_range_time")) != "" ;
       ++listId)
  {
    oss.str("");
    oss << listId;
    // Base path to the data, that depends on the iteration number
    std::string path_root = "dopplerCentroid.dop_coef_list" + oss.str();
    DopplerCentroid dopplerCent;
    std::istringstream(mds.GetAs<std::string>(path_root + ".dop_coef_time")) >> dopplerCent.azimuthTime;
    dopplerCent.t0 = mds.GetAs<double>(path_root + ".slant_range_time");
    dopplerCentroidVector.push_back(std::move(dopplerCent));
  }
  return dopplerCentroidVector;
}

std::vector<Orbit> SarImageMetadataInterface::GetOrbitsGeom(const MetadataSupplierInterface & mds) const
{
  std::vector<Orbit> orbitVector;
  // Number of entries in the vector
  int listCount = mds.GetAs<int>("orbitList.nb_orbits");
  // This streams wild hold the iteration number
  std::ostringstream oss;
  for (int listId = 0 ; listId <= listCount - 1 ; ++listId)
  {
    oss.str("");
    oss << listId;
    // Base path to the data, that depends on the iteration number
    std::string path_root = "orbitList.orbit[" + oss.str() + "]";
    Orbit orbit;
    std::istringstream(mds.GetAs<std::string>(path_root + ".time")) >> orbit.time;
    orbit.posX = mds.GetAs<double>(path_root + ".x_pos");
    orbit.posY = mds.GetAs<double>(path_root + ".y_pos");
    orbit.posZ = mds.GetAs<double>(path_root + ".z_pos");
    orbit.velX = mds.GetAs<double>(path_root + ".x_vel");
    orbit.velY = mds.GetAs<double>(path_root + ".y_vel");
    orbit.velZ = mds.GetAs<double>(path_root + ".z_vel");
    orbitVector.push_back(std::move(orbit));
  }
  return orbitVector;
}

std::vector<CalibrationVector> SarImageMetadataInterface::GetCalibrationVectorGeom(const MetadataSupplierInterface & mds) const
{
  std::vector<CalibrationVector> calibrationVector;
  // Number of entries in the vector
  int listCount = mds.GetAs<int>("calibration.count");
  // This streams wild hold the iteration number
  std::ostringstream oss;
  for (int listId = 0 ; listId <= listCount - 1 ; ++listId)
  {
    oss.str("");
    oss << listId;
    // Base path to the data, that depends on the iteration number
    std::string path_root = "calibration.calibrationVector[" + oss.str() + "]";

    CalibrationVector calVect;
    std::istringstream(mds.GetAs<std::string>(path_root + ".azimuthTime")) >> calVect.azimuthTime;
    calVect.line = mds.GetAs<int>(path_root + ".line");

    // Same axe for all LUTs
    MetaData::LUTAxis ax1;
    ax1.Size = mds.GetAs<int>(path_root + ".pixel_count");
    ax1.Values = mds.GetAsVector<double>(path_root + ".pixel", ' ', ax1.Size);

    MetaData::LUT1D sigmaNoughtLut;
    sigmaNoughtLut.Axis[0] = ax1;
    sigmaNoughtLut.Array = mds.GetAsVector<double>(path_root + ".sigmaNought", ' ', ax1.Size);
    calVect.sigmaNought = std::move(sigmaNoughtLut);

    MetaData::LUT1D betaNoughtLut;
    betaNoughtLut.Axis[0] = ax1;
    betaNoughtLut.Array = mds.GetAsVector<double>(path_root + ".betaNought", ' ', ax1.Size);
    calVect.betaNought = std::move(betaNoughtLut);

    MetaData::LUT1D gammaLut;
    gammaLut.Axis[0] = ax1;
    gammaLut.Array = mds.GetAsVector<double>(path_root + ".gamma", ' ', ax1.Size);
    calVect.gamma = std::move(gammaLut);

    MetaData::LUT1D dnLut;
    dnLut.Axis[0] = ax1;
    dnLut.Array = mds.GetAsVector<double>(path_root + ".dn", ' ', ax1.Size);
    calVect.dn = std::move(dnLut);

    calibrationVector.push_back(std::move(calVect));
  }
  return calibrationVector;
}

std::vector<SARNoise> SarImageMetadataInterface::GetNoiseVectorGeom(const MetadataSupplierInterface & mds) const
{
  std::vector<SARNoise> noiseVector;
  // This streams wild hold the iteration number
  std::ostringstream oss;
  // Path: noise.noiseVector[<listId>].{azimuthTime,line,noiseLut,pixel,pixel_count}
  for (int listId = 0 ; mds.GetAs<std::string>("", std::string("noise.noiseVector[")+std::to_string(listId)+std::string("].pixel_count")) != "" ; ++listId)
  {
    oss.str("");
    oss << listId;
    // Base path to the data, that depends on the iteration number
    std::string path_root = "noise.noiseVector[" + oss.str() + "]";
    SARNoise noiseVect;
    std::istringstream(mds.GetAs<std::string>(path_root + ".azimuthTime")) >> noiseVect.azimuthTime;
    MetaData::LUT1D noiseLut;
    MetaData::LUTAxis ax1;
    ax1.Size = mds.GetAs<int>(path_root + ".pixel_count");
    ax1.Values = mds.GetAsVector<double>(path_root + ".pixel", ' ', ax1.Size);
    noiseLut.Axis[0] = ax1;
    noiseLut.Array = mds.GetAsVector<double>(path_root + ".noiseLut", ' ', ax1.Size);
    noiseVect.noiseLut = std::move(noiseLut);
    noiseVector.push_back(std::move(noiseVect));
  }
  return noiseVector;
}

bool SarImageMetadataInterface::GetSAR(const MetadataSupplierInterface & mds, SARParam & sarParam) const
{
  bool hasValue;
  mds.GetMetadataValue("calibration.count", hasValue);
  if (!hasValue)
    return false;
  
  sarParam.azimuthFmRates = this->GetAzimuthFmRateGeom(mds);
  sarParam.dopplerCentroids = this->GetDopplerCentroidGeom(mds);
  sarParam.orbits = this->GetOrbitsGeom(mds);
  sarParam.calibrationVectors = this->GetCalibrationVectorGeom(mds);
  sarParam.noiseVector = this->GetNoiseVectorGeom(mds);
  std::istringstream(mds.GetAs<std::string>("calibration.startTime")) >> sarParam.calibrationStartTime;
  std::istringstream(mds.GetAs<std::string>("calibration.stopTime")) >> sarParam.calibrationStopTime;
  return true;
}

void SarImageMetadataInterface::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  if (this->CanRead())
  {
    os << indent << "GetRadiometricCalibrationScale:                 " << this->GetRadiometricCalibrationScale() << std::endl;
    os << indent << "GetRadiometricCalibrationNoise:                 " << this->GetRadiometricCalibrationNoise() << std::endl;
    os << indent << "GetRadiometricCalibrationAntennaPatternNewGain: " << this->GetRadiometricCalibrationAntennaPatternNewGain() << std::endl;
    os << indent << "GetRadiometricCalibrationAntennaPatternOldGain: " << this->GetRadiometricCalibrationAntennaPatternOldGain() << std::endl;
    os << indent << "GetRadiometricCalibrationIncidenceAngle:        " << this->GetRadiometricCalibrationIncidenceAngle() << std::endl;
    os << indent << "GetRadiometricCalibrationRangeSpreadLoss:       " << this->GetRadiometricCalibrationRangeSpreadLoss() << std::endl;
    os << indent << "GetConstantPolynomialDegree:                    " << this->GetConstantPolynomialDegree() << std::endl;
    os << indent << "GetRadiometricCalibrationNoisePolynomialDegree: " << this->GetRadiometricCalibrationNoisePolynomialDegree() << std::endl;
    os << indent << "GetRadiometricCalibrationAntennaPatternNewGainPolynomialDegree: " << this->GetRadiometricCalibrationAntennaPatternNewGainPolynomialDegree()
       << std::endl;
    os << indent << "GetRadiometricCalibrationAntennaPatternOldGainPolynomialDegree: " << this->GetRadiometricCalibrationAntennaPatternOldGainPolynomialDegree()
       << std::endl;
    os << indent << "GetRadiometricCalibrationIncidenceAnglePolynomialDegree:        " << this->GetRadiometricCalibrationIncidenceAnglePolynomialDegree()
       << std::endl;
    os << indent << "GetRadiometricCalibrationRangeSpreadLossPolynomialDegree:       " << this->GetRadiometricCalibrationRangeSpreadLossPolynomialDegree()
       << std::endl;
    os << indent << "GetPRF:                  " << this->GetPRF() << std::endl;
    os << indent << "GetRSF:                  " << this->GetRSF() << std::endl;
    os << indent << "GetRadarFrequency:       " << this->GetRadarFrequency() << std::endl;
    os << indent << "GetCenterIncidenceAngle: " << this->GetCenterIncidenceAngle() << std::endl;
  }
}

bool SarImageMetadataInterface::ConvertImageKeywordlistToImageMetadata()
{
  // TODO
  return false;
}

} // end namespace otb
