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

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include <typeinfo>
#include <cassert>

#include "otbMacro.h"

#include "otbTerraSarImageMetadataInterface.h"

#include "itkMetaDataObject.h"
#include "otbVectorDataKeywordlist.h"
#include "base/ossimDate.h"

namespace otb
{

TerraSarImageMetadataInterface
::TerraSarImageMetadataInterface()
{
}

std::string
TerraSarImageMetadataInterface::GetSensorID(const MetaDataDictionaryType& dict) const
{
  ImageKeywordlistType ImageKeywordlist;
  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, ImageKeywordlist);
    }

  ossimKeywordlist kwl;
  ImageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key = "sensor";
  ossimString keywordString = kwl.find(key.c_str());
  std::string output(keywordString.chars());

  return output;
}

bool
TerraSarImageMetadataInterface::CanRead(const MetaDataDictionaryType& dict) const
{
  std::string sensorID = GetSensorID(dict);
  if (sensorID.find("TSX") != std::string::npos) return true;
  else return false;
}

int
TerraSarImageMetadataInterface::GetDay(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no TerraSar Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;
  key = "azimuth_start_time";
  separatorList = "-T";

  ossimString              keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if (keywordStrings.size() <= 2) itkExceptionMacro(<< "Invalid Day");

  ossimString day = keywordStrings[2];

  return day.toInt();
}

int
TerraSarImageMetadataInterface::GetMonth(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no TerraSar Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;
  key = "azimuth_start_time";
  separatorList = "-T";

  ossimString              keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if (keywordStrings.size() <= 2) itkExceptionMacro(<< "Invalid Month");

  ossimString month = keywordStrings[1];

  return month.toInt();
}

int
TerraSarImageMetadataInterface::GetYear(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no TerraSar Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;
  key = "azimuth_start_time";
  separatorList = "-T";

  ossimString              keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if (keywordStrings.size() <= 2) itkExceptionMacro("Invalid Year");

  ossimString year = keywordStrings[0];

  return year.toInt();

}

int
TerraSarImageMetadataInterface::GetHour(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no TerraSar Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;
  key = "azimuth_start_time";
  separatorList = "-T:";

  ossimString              keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if (keywordStrings.size() <= 4) itkExceptionMacro("Invalid Hour");

  ossimString hour = keywordStrings[3];

  return hour.toInt();
}

int
TerraSarImageMetadataInterface::GetMinute(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no TerraSar Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;
  key = "azimuth_start_time";
  separatorList = "-T:";

  ossimString              keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);
  if (keywordStrings.size() <= 5) itkExceptionMacro("Invalid Minute");

  ossimString minute = keywordStrings[4];

  return minute.toInt();
}

int
TerraSarImageMetadataInterface::GetProductionDay(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no TerraSar Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;
  key = "generation_time";
  separatorList = "-T:";

  ossimString              keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if (keywordStrings.size() <= 2) itkExceptionMacro(<< "Invalid Production Day");

  ossimString day = keywordStrings[2];

  return day.toInt();
}

int
TerraSarImageMetadataInterface::GetProductionMonth(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no TerraSar Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;
  key = "generation_time";
  separatorList = "-T";

  ossimString              keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if (keywordStrings.size() <= 2) itkExceptionMacro(<< "Invalid Production Month");

  ossimString month = keywordStrings[1];

  return month.toInt();
}

int
TerraSarImageMetadataInterface::GetProductionYear(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no TerraSar Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;
  key = "generation_time";
  separatorList = "-T";

  ossimString              keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if (keywordStrings.size() <= 2) itkExceptionMacro("Invalid Production Year");

  ossimString year = keywordStrings[0];

  return year.toInt();
}

double
TerraSarImageMetadataInterface::GetCalibrationFactor(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no TerraSar Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key = "calibration.calibrationConstant.calFactor";
  ossimString calFac = kwl.find(key.c_str());

  return calFac.toDouble();
}

ossimplugins::Noise *
TerraSarImageMetadataInterface::GetNoise(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no TerraSar Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  ossimplugins::Noise * noise = new ossimplugins::Noise();
  noise->loadState(kwl, "");

  return noise;
}

unsigned int
TerraSarImageMetadataInterface::GetNumberOfNoiseRecords(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no TerraSar Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);
  std::string key = "noise.numberOfNoiseRecords";
  ossimString nbRec = kwl.find(key.c_str());

  return static_cast<unsigned int>(nbRec.toInt());
}

TerraSarImageMetadataInterface::UIntVectorType
TerraSarImageMetadataInterface::GetNoisePolynomialDegrees(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no TerraSar Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  UIntVectorType     polDeg;
  itk::OStringStream oss;

  unsigned int nbRec = this->GetNumberOfNoiseRecords(dict);
  for (unsigned int i = 0; i < nbRec; i++)
    {
    oss.str("");
    oss << "noise[" << i << "]imageNoise.noiseEstimate.polynomialDegree";
    ossimString tempVal = kwl.find(oss.str().c_str());
    polDeg.push_back(static_cast<unsigned int>(tempVal.toInt()));
    }

  return polDeg;
}

TerraSarImageMetadataInterface::DoubleVectorVectorType
TerraSarImageMetadataInterface::GetNoisePolynomialCoefficientsList(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no TerraSar Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  unsigned int   nbRec = this->GetNumberOfNoiseRecords(dict);
  UIntVectorType polDegs = this->GetNoisePolynomialDegrees(dict);

  DoubleVectorVectorType polCoefList;
  DoubleVectorType       polCoef;
  itk::OStringStream     oss;

  for (unsigned int i = 0; i < nbRec; i++)
    {
    polCoef.clear();
    // set <= condition because degree N means N+1 coeff
    for (unsigned int j = 0; j <= polDegs.size(); j++)
      {
      oss.str("");
      oss << "noise[" << i << "]imageNoise.noiseEstimate.coefficient[" << j << "]";
      ossimString tempVal = kwl.find(oss.str().c_str());
      polCoef.push_back(static_cast<double>(tempVal.toDouble()));
      }
    polCoefList.push_back(polCoef);
    }

  return polCoefList;
}

TerraSarImageMetadataInterface::DoubleVectorType
TerraSarImageMetadataInterface::GetNoiseTimeUTCList(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no TerraSar Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  DoubleVectorType         timeList;
  itk::OStringStream       oss;
  ossimString              separatorList = "-T:Z";
  std::vector<ossimString> splittedDate;

  int          year, month, day, hour, minu, sec;
  double       secDec, julianDay;
  unsigned int nbRec = this->GetNumberOfNoiseRecords(dict);
  for (unsigned int i = 0; i < nbRec; i++)
    {
    oss.str("");
    oss << "noise[" << i << "]imageNoise.timeUTC";
    ossimString tempVal = kwl.find(oss.str().c_str());
    splittedDate = tempVal.split(separatorList);

    year = splittedDate[0].toInt();
    month = splittedDate[1].toInt();
    day = splittedDate[2].toInt();
    hour = splittedDate[3].toInt();
    minu = splittedDate[4].toInt();
    double secFull = splittedDate[5].toDouble();
    sec = static_cast<int>(vcl_floor(secFull));
    // store the decimal second value
    secDec = secFull - vcl_floor(secFull);
    ossimDate myDate(month, day, year);
    myDate.setHour(hour);
    myDate.setMin(minu);
    myDate.setSec(sec);

    julianDay = myDate.getJulian();

    // add the decimal second to the julian day (1s <-> 1/24*60*60 = 1/86400 julienDay)
    julianDay += secDec / 86400;

    timeList.push_back(julianDay);
    }

  return timeList;
}

TerraSarImageMetadataInterface::DoubleVectorType
TerraSarImageMetadataInterface::GetNoiseValidityRangeMaxList(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no TerraSar Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  DoubleVectorType   maxList;
  itk::OStringStream oss;

  unsigned int nbRec = this->GetNumberOfNoiseRecords(dict);
  for (unsigned int i = 0; i < nbRec; i++)
    {
    oss.str("");
    oss << "noise[" << i << "]imageNoise.noiseEstimate.validityRangeMax";
    ossimString tempVal = kwl.find(oss.str().c_str());
    maxList.push_back(tempVal.toDouble());
    }

  return maxList;
}

TerraSarImageMetadataInterface::DoubleVectorType
TerraSarImageMetadataInterface::GetNoiseValidityRangeMinList(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no TerraSar Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  DoubleVectorType   minList;
  itk::OStringStream oss;

  unsigned int nbRec = this->GetNumberOfNoiseRecords(dict);
  for (unsigned int i = 0; i < nbRec; i++)
    {
    oss.str("");
    oss << "noise[" << i << "]imageNoise.noiseEstimate.validityRangeMin";
    ossimString tempVal = kwl.find(oss.str().c_str());

    minList.push_back(tempVal.toDouble());
    }

  return minList;
}

TerraSarImageMetadataInterface::DoubleVectorType
TerraSarImageMetadataInterface::GetNoiseReferencePointList(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no TerraSar Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  DoubleVectorType   refPointList;
  itk::OStringStream oss;

  unsigned int nbRec = this->GetNumberOfNoiseRecords(dict);
  for (unsigned int i = 0; i < nbRec; i++)
    {
    oss.str("");
    oss << "noise[" << i << "]imageNoise.noiseEstimate.referencePoint";
    ossimString tempVal = kwl.find(oss.str().c_str());

    refPointList.push_back(tempVal.toDouble());
    }

  return refPointList;
}

double
TerraSarImageMetadataInterface::GetRadarFrequency(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no TerraSar Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  ossimString tempVal = kwl.find("radarFrequency");
  double      freq = tempVal.toDouble();

  return freq;
}

double
TerraSarImageMetadataInterface::GetPRF(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no TerraSar Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  ossimString tempVal = kwl.find("sensor_params.prf");
  double      freq = tempVal.toDouble();

  return freq;
}

ossimplugins::IncidenceAngles*
TerraSarImageMetadataInterface::GetIncidenceAngles(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no TerraSar Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  ossimplugins::IncidenceAngles* incidenceAngles = new ossimplugins::IncidenceAngles();
  incidenceAngles->loadState(kwl, "");

  return incidenceAngles;
}

unsigned int
TerraSarImageMetadataInterface::GetNumberOfCornerIncidenceAngles(const MetaDataDictionaryType& dict) const
{
  ossimplugins::IncidenceAngles * ia = this->GetIncidenceAngles(dict);

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);
  std::string key = "incidenceAngles.numberOfCornerIncidenceAngles";
  ossimString nbCornerAngles = kwl.find(key.c_str());

  delete ia;

  return static_cast<unsigned int>(nbCornerAngles.toInt());
}

double
TerraSarImageMetadataInterface::GetMeanIncidenceAngles(const MetaDataDictionaryType& dict) const
{
  ossimplugins::IncidenceAngles * ia = this->GetIncidenceAngles(dict);

  ImageKeywordlistType imageKeywordlist;

  double sum = 0.;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  itk::OStringStream oss;

  std::string key = "incidenceAngles.centerInfoIncidenceAngle.infoIncidenceAngle.incidenceAngle";
  ossimString centerIncidenceAngle = kwl.find(key.c_str());

  sum += centerIncidenceAngle.toDouble();

  unsigned int nbAngles = this->GetNumberOfCornerIncidenceAngles(dict);
  for (unsigned int i = 0; i < nbAngles; i++)
    {
    oss.str("");
    oss << "incidenceAngles.cornersInfoIncidenceAngle[" << i << "].infoIncidenceAngle.incidenceAngle";
    ossimString tempVal = kwl.find(oss.str().c_str());

    sum += tempVal.toDouble();
    }

  delete ia;

  return (sum / (nbAngles + 1));
}

double
TerraSarImageMetadataInterface::GetCenterIncidenceAngle(const MetaDataDictionaryType& dict) const
{
  ossimplugins::IncidenceAngles * ia = this->GetIncidenceAngles(dict);

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);
  std::string key = "incidenceAngles.centerInfoIncidenceAngle.infoIncidenceAngle.incidenceAngle";
  ossimString tempVal = kwl.find(key.c_str());

  delete ia;

  return tempVal.toDouble();
}

TerraSarImageMetadataInterface::IndexType
TerraSarImageMetadataInterface::GetCenterIncidenceAngleIndex(const MetaDataDictionaryType& dict) const
{
  ossimplugins::IncidenceAngles * ia = this->GetIncidenceAngles(dict);

  TerraSarImageMetadataInterface::IndexType it;

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);
  std::string key = "incidenceAngles.centerInfoIncidenceAngle.infoIncidenceAngle.refRow";
  ossimString tempVal = kwl.find(key.c_str());

  it[0] = tempVal.toInt();

  key = "incidenceAngles.centerInfoIncidenceAngle.infoIncidenceAngle.refColumn";
  tempVal = kwl.find(key.c_str());

  it[1] = tempVal.toInt();

  delete ia;

  return it;
}

TerraSarImageMetadataInterface::DoubleVectorType
TerraSarImageMetadataInterface::GetCornersIncidenceAngles(const MetaDataDictionaryType& dict) const
{
  ossimplugins::IncidenceAngles * ia = this->GetIncidenceAngles(dict);

  ImageKeywordlistType imageKeywordlist;

  TerraSarImageMetadataInterface::DoubleVectorType dv;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  itk::OStringStream oss;

  unsigned int nbAngles = this->GetNumberOfCornerIncidenceAngles(dict);
  for (unsigned int i = 0; i < nbAngles; i++)
    {
    oss.str("");
    oss << "incidenceAngles.cornersInfoIncidenceAngle[" << i << "].infoIncidenceAngle.incidenceAngle";
    ossimString tempVal = kwl.find(oss.str().c_str());

    dv.push_back(tempVal.toDouble());
    }

  delete ia;

  return dv;
}

TerraSarImageMetadataInterface::IndexVectorType
TerraSarImageMetadataInterface::GetCornersIncidenceAnglesIndex(const MetaDataDictionaryType& dict) const
{
  ossimplugins::IncidenceAngles * ia = this->GetIncidenceAngles(dict);

  ImageKeywordlistType imageKeywordlist;

  TerraSarImageMetadataInterface::IndexVectorType iv;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  itk::OStringStream oss;
  itk::OStringStream oss2;

  unsigned int nbAngles = this->GetNumberOfCornerIncidenceAngles(dict);
  for (unsigned int i = 0; i < nbAngles; i++)
    {
    TerraSarImageMetadataInterface::IndexType it;

    oss.str("");
    oss << "incidenceAngles.cornersInfoIncidenceAngle[" << i << "].infoIncidenceAngle.refRow";
    ossimString tempVal = kwl.find(oss.str().c_str());

    it[0] = tempVal.toInt();

    oss2.str("");
    oss2 << "incidenceAngles.cornersInfoIncidenceAngle[" << i << "].infoIncidenceAngle.refColumn";
    tempVal = kwl.find(oss2.str().c_str());

    it[1] = tempVal.toInt();

    iv.push_back(it);
    }

  delete ia;

  return iv;

}

} // end namespace otb
