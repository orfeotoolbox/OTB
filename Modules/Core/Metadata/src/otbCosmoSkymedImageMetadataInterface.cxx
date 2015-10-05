/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "otbSarImageMetadataInterface.h"
#include "otbCosmoSkymedImageMetadataInterface.h"
#include "otbMacro.h"
//#include "otbImageKeywordlist.h"

//useful constants
#include "otbMath.h"


#include <iomanip>
#include <iostream>

namespace otb
{

CosmoSkymedImageMetadataInterface
::CosmoSkymedImageMetadataInterface()
{
}

bool
CosmoSkymedImageMetadataInterface::CanRead() const
{
  std::string sensorID = this->GetSensorID();

  if (sensorID.find("CSK") != std::string::npos)
    {
    return true;
    }
  else
    return false;
}

std::string
CosmoSkymedImageMetadataInterface::GetSensorID() const
{

   std::string sensorId;
   GetValueFromMetadataDictionary("Mission_ID", sensorId);
   return sensorId;
}
std::string
CosmoSkymedImageMetadataInterface::GetImageID() const
{
  std::string imageId;
  GetValueFromMetadataDictionary("Programmed_Image_ID", imageId);
  return imageId;
}

CosmoSkymedImageMetadataInterface::PointSetPointer
CosmoSkymedImageMetadataInterface
::GetRadiometricCalibrationIncidenceAngle() const
{
  PointSetPointer points = PointSetType::New();
  double refIncidenceAngle = 1.0;
  std::string refIncidenceAngleCompensation = "";

  GetValueFromMetadataDictionary("Incidence_Angle_Compensation_Geometry", refIncidenceAngleCompensation);
  if (!refIncidenceAngleCompensation.empty())
    {
    GetValueFromMetadataDictionary<double>("Reference_Incidence_Angle", refIncidenceAngle);
    refIncidenceAngle *=  CONST_PI_180;
    }

  points->Initialize();
  PointType p0;
  p0[0] = 0; p0[0] = 0;
  points->SetPoint(0, p0);
  points->SetPointData(0, refIncidenceAngle);
  return points;
}

CosmoSkymedImageMetadataInterface::PointSetPointer
CosmoSkymedImageMetadataInterface
::GetRadiometricCalibrationRangeSpreadLoss() const
{
  PointSetPointer points = PointSetType::New();
  double rangeSpreadLoss = 0.0;
  int referenceSlantRange = 1.0;
  int referenceSlantRangeExponent = 1.0;
  std::string rangeSpreadLossCompensation = "";

  GetValueFromMetadataDictionary("Range_Spreading_Loss_Compensation_Geometry", rangeSpreadLossCompensation);
  if (!rangeSpreadLossCompensation.empty())
    {
    GetValueFromMetadataDictionary<int>("Reference_Slant_Range", referenceSlantRange);
    GetValueFromMetadataDictionary<int>("Reference_Slant_Range_Exponent", referenceSlantRangeExponent);
    rangeSpreadLoss = vcl_pow(referenceSlantRange, 2 * referenceSlantRangeExponent);
    }

  points->Initialize();
  PointType p0;
  p0[0] = 0; p0[0] = 0;
  points->SetPoint(0, p0);
  points->SetPointData(0, rangeSpreadLoss);
  return points;
}

double
CosmoSkymedImageMetadataInterface
::GetRescalingFactor() const
{
  std::string calibrationConstantCompensation = "";
  double rescalingFactor = 1.0;
  double calibrationConstant = 1.0;
  GetValueFromMetadataDictionary("Calibration_Constant_Compensation_Flag", calibrationConstantCompensation);
    if (calibrationConstantCompensation.empty())
    {
    std::string prefix = "S01";
    unsigned int datasetIndex = 1;
    GetValueFromMetadataDictionary<unsigned int>(MetaDataKey::SubDatasetIndex, datasetIndex);
    std::string datasetName;
    std::stringstream strm;
    strm << "SUBDATASET_" <<  datasetIndex+1 << "_NAME";
    GetValueFromMetadataDictionary(strm.str().c_str(), datasetName);
    size_t start = datasetName.find("://");
    if( start  !=std::string::npos )
      {
      datasetName = datasetName.substr(start+3,datasetName.size()-1);
      size_t end = datasetName.find("/");
      if( end  !=std::string::npos )
        {
        prefix = datasetName.substr(0, end);
        }
    }
    const std::string key = prefix + "_Calibration_Constant";

    GetValueFromMetadataDictionary<double>(key, calibrationConstant);
    }

  GetValueFromMetadataDictionary<double>("Rescaling_Factor", rescalingFactor);

  rescalingFactor  *= rescalingFactor * calibrationConstant;
  return rescalingFactor;
}
void
CosmoSkymedImageMetadataInterface
::GetValueFromMetadataDictionary(std::string const key, std::string& result) const
{
  GetValueFromMetadataDictionary<std::string>(key, result);
}

void
CosmoSkymedImageMetadataInterface
::ParseDateTime(const char* key, std::vector<int>& dateFields) const
{
  if(dateFields.size() < 1 )
    {
    if (!this->CanRead())
      {
      itkExceptionMacro(<< "Invalid Metadata, not a valid ComsoSkymed product");
      }
    std::string dateTimeUTCString;
    std::string const prefix = MetaDataKey::MetadataKeyPrefix;
    GetValueFromMetadataDictionary(key, dateTimeUTCString);
    Utils::ConvertStringToVector(dateTimeUTCString, dateFields, prefix + key, " T:-.");
    }
}

int
CosmoSkymedImageMetadataInterface::GetYear() const
{
  int value = 0;
  ParseDateTime("Scene_Sensing_Start_UTC", m_AcquisitionDateFields);
  if(m_AcquisitionDateFields.size() > 0 )
    {
    value = boost::lexical_cast<int>( m_AcquisitionDateFields[0] );
    }
  else
    {
    itkExceptionMacro( << "Invalid year" );
    }
  return value;
}

int
CosmoSkymedImageMetadataInterface::GetMonth() const
{
  int value = 0;
  ParseDateTime("Scene_Sensing_Start_UTC", m_AcquisitionDateFields);
  if(m_AcquisitionDateFields.size() > 1 )
    {
    value = boost::lexical_cast<int>( m_AcquisitionDateFields[1] );
    }
  else
    {
    itkExceptionMacro( << "Invalid month" );
    }
  return value;
}

int
CosmoSkymedImageMetadataInterface::GetDay() const
{
  int value = 0;
  ParseDateTime("Scene_Sensing_Start_UTC", m_AcquisitionDateFields);
  if(m_AcquisitionDateFields.size() > 2 )
    {
    value = boost::lexical_cast<int>( m_AcquisitionDateFields[2] );
    }
  else
    {
    itkExceptionMacro( << "Invalid day" );
    }
  return value;
}

int
CosmoSkymedImageMetadataInterface::GetHour() const
{
  int value = 0;
  ParseDateTime("Scene_Sensing_Start_UTC", m_AcquisitionDateFields);
  if(m_AcquisitionDateFields.size() > 3 )
    {
    value = boost::lexical_cast<int>( m_AcquisitionDateFields[3] );
    }
  else
    {
    itkExceptionMacro( << "Invalid hour" );
    }
  return value;
}

int
CosmoSkymedImageMetadataInterface::GetMinute() const
{
  int value = 0;
  ParseDateTime("Scene_Sensing_Start_UTC", m_AcquisitionDateFields);
  if(m_AcquisitionDateFields.size() > 4 )
    {
    value = boost::lexical_cast<int>( m_AcquisitionDateFields[4] );
    }
  else
    {
    itkExceptionMacro( << "Invalid minute" );
    }
  return value;
}

int
CosmoSkymedImageMetadataInterface::GetProductionYear() const
{
  int value = 0;
  ParseDateTime("Product_Generation_UTC", m_ProductionDateFields);
  if(m_ProductionDateFields.size() > 0 )
    {
    value = boost::lexical_cast<int>( m_ProductionDateFields[0] );
    }
  else
    {
    itkExceptionMacro( << "Invalid production year" );
    }
  return value;

}

int
CosmoSkymedImageMetadataInterface::GetProductionMonth() const
{
  int value = 0;
  ParseDateTime("Product_Generation_UTC", m_ProductionDateFields);
  if(m_ProductionDateFields.size() > 1 )
    {
    value = boost::lexical_cast<int>( m_ProductionDateFields[1] );
    }
  else
    {
    itkExceptionMacro( << "Invalid production month" );
    }
  return value;
}

int
CosmoSkymedImageMetadataInterface::GetProductionDay() const
{
  int value = 0;
  ParseDateTime("Product_Generation_UTC", m_ProductionDateFields);
  if(m_ProductionDateFields.size() > 2 )
    {
    value = boost::lexical_cast<int>( m_ProductionDateFields[2] );
    }
  else
    {
    itkExceptionMacro( << "Invalid production day" );
    }
  return value;
}

CosmoSkymedImageMetadataInterface::UIntVectorType
CosmoSkymedImageMetadataInterface::GetDefaultDisplay() const
{
  UIntVectorType rgb(3);
  rgb[0] = 0;
  rgb[1] = 0;
  rgb[2] = 0;
  return rgb;
}

double
CosmoSkymedImageMetadataInterface::GetPRF() const
{
  double prf = 0;
  std::string prefix = "S01";
  unsigned int datasetIndex = 1;
  GetValueFromMetadataDictionary<unsigned int>(MetaDataKey::SubDatasetIndex, datasetIndex);
  std::string datasetName;
  std::stringstream strm;
  strm << "SUBDATASET_" <<  datasetIndex+1 << "_NAME";
  GetValueFromMetadataDictionary(strm.str().c_str(), datasetName);
  size_t start = datasetName.find("://");
  if( start  !=std::string::npos )
    {
    datasetName = datasetName.substr(start+3,datasetName.size()-1);
    size_t end = datasetName.find("/");
    if( end  !=std::string::npos )
      {
      prefix = datasetName.substr(0, end);
      }
    }

  const std::string key = prefix + "_PRF";
  GetValueFromMetadataDictionary<double>(key.c_str(), prf);
  return prf;
}

double
CosmoSkymedImageMetadataInterface::GetRadarFrequency() const
{
  double radarFrequency = 0;
  GetValueFromMetadataDictionary<double>("Radar_Frequency", radarFrequency);
  return radarFrequency;
}

double
CosmoSkymedImageMetadataInterface::GetCenterIncidenceAngle() const
{
  return 0;
}

double
CosmoSkymedImageMetadataInterface::GetRSF() const
{
  return 0;
}

} // end namespace otb
