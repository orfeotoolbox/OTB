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



#include "otbMacro.h"
#include "otbMath.h"

#include "otbTerraSarImageMetadataInterface.h"

#include "itkMetaDataObject.h"
#include "otbVectorDataKeywordlist.h"
#include "base/ossimDate.h"
#include "base/ossimKeywordlist.h"
#include "itkIndex.h"


namespace otb
{

TerraSarImageMetadataInterface
::TerraSarImageMetadataInterface()
{
}

std::string
TerraSarImageMetadataInterface::GetSensorID() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
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
TerraSarImageMetadataInterface::CanRead() const
{
  std::string sensorID = GetSensorID();
  if (sensorID.find("TSX") != std::string::npos) return true;
  else return false;
}

int
TerraSarImageMetadataInterface::GetDay() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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
TerraSarImageMetadataInterface::GetMonth() const
{
 const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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
TerraSarImageMetadataInterface::GetYear() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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
TerraSarImageMetadataInterface::GetHour() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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
TerraSarImageMetadataInterface::GetMinute() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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
TerraSarImageMetadataInterface::GetProductionDay() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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
TerraSarImageMetadataInterface::GetProductionMonth() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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
TerraSarImageMetadataInterface::GetProductionYear() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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
TerraSarImageMetadataInterface::GetCalibrationFactor() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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
TerraSarImageMetadataInterface::GetNoise() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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
TerraSarImageMetadataInterface::GetNumberOfNoiseRecords() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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

unsigned int
TerraSarImageMetadataInterface::GetNoisePolynomialDegrees(unsigned int noiseRecord) const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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

  unsigned int     polDeg;
  itk::OStringStream oss;

  oss.str("");
  oss << "noise[" << noiseRecord << "]imageNoise.noiseEstimate.polynomialDegree";
  ossimString tempVal = kwl.find(oss.str().c_str());
  polDeg = static_cast<unsigned int>(tempVal.toInt());

  return polDeg;
}


TerraSarImageMetadataInterface::UIntVectorType
TerraSarImageMetadataInterface::GetNoisePolynomialDegrees() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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

  unsigned int nbRec = this->GetNumberOfNoiseRecords();
  for (unsigned int i = 0; i < nbRec; i++)
    {
    oss.str("");
    oss << "noise[" << i << "]imageNoise.noiseEstimate.polynomialDegree";
    ossimString tempVal = kwl.find(oss.str().c_str());
    polDeg.push_back(static_cast<unsigned int>(tempVal.toInt()));
    }

  return polDeg;
}


TerraSarImageMetadataInterface::DoubleVectorType
TerraSarImageMetadataInterface
::GetNoisePolynomialCoefficients(unsigned int noiseRecord) const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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

  unsigned int   polDegs = this->GetNoisePolynomialDegrees(noiseRecord);

  DoubleVectorType       polCoef;
  itk::OStringStream     oss;

  polCoef.clear();
  // set <= condition because degree N means N+1 coeff
  for (unsigned int j = 0; j <= polDegs; j++)
  {
      oss.str("");
      oss << "noise[" << noiseRecord << "]imageNoise.noiseEstimate.coefficient[" << j << "]";
      ossimString tempVal = kwl.find(oss.str().c_str());
      polCoef.push_back(static_cast<double>(tempVal.toDouble()));
  }

  return polCoef;
}


TerraSarImageMetadataInterface::DoubleVectorVectorType
TerraSarImageMetadataInterface::GetNoisePolynomialCoefficientsList() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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

  unsigned int   nbRec = this->GetNumberOfNoiseRecords();
  UIntVectorType polDegs = this->GetNoisePolynomialDegrees();

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

double
TerraSarImageMetadataInterface
::GetNoiseTimeUTC(unsigned int noiseRecord) const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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

  oss.str("");
  oss << "noise[" << noiseRecord << "]imageNoise.timeUTC";
  ossimString tempVal = kwl.find(oss.str().c_str());

  double julianDay;
  julianDay = ConvertStringTimeUTCToJulianDay(tempVal);

  return julianDay;
}

double
TerraSarImageMetadataInterface
::ConvertStringTimeUTCToJulianDay(ossimString& value) const
{
  ossimString              separatorList = "-T:Z";
  std::vector<ossimString> splittedDate;

  int          year, month, day, hour, minu, sec;
  double       secDec, julianDay;
    splittedDate = value.split(separatorList);

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

    return julianDay;
}

TerraSarImageMetadataInterface::DoubleVectorType
TerraSarImageMetadataInterface::GetNoiseTimeUTCList() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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
  unsigned int nbRec = this->GetNumberOfNoiseRecords();
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
TerraSarImageMetadataInterface::GetNoiseValidityRangeMaxList() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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

  unsigned int nbRec = this->GetNumberOfNoiseRecords();
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
TerraSarImageMetadataInterface::GetNoiseValidityRangeMinList() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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

  unsigned int nbRec = this->GetNumberOfNoiseRecords();
  for (unsigned int i = 0; i < nbRec; i++)
    {
    oss.str("");
    oss << "noise[" << i << "]imageNoise.noiseEstimate.validityRangeMin";
    ossimString tempVal = kwl.find(oss.str().c_str());

    minList.push_back(tempVal.toDouble());
    }

  return minList;
}


double
TerraSarImageMetadataInterface
::GetNoiseReferencePoint(unsigned int noiseRecord) const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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

  double refPoint;
  itk::OStringStream oss;

  oss.str("");
  oss << "noise[" << noiseRecord << "]imageNoise.noiseEstimate.referencePoint";
  ossimString tempVal = kwl.find(oss.str().c_str());
  refPoint = tempVal.toDouble();

  return refPoint;
}


TerraSarImageMetadataInterface::DoubleVectorType
TerraSarImageMetadataInterface::GetNoiseReferencePointList() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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

  unsigned int nbRec = this->GetNumberOfNoiseRecords();
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
TerraSarImageMetadataInterface::GetRadarFrequency() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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
TerraSarImageMetadataInterface::GetPRF() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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

double
TerraSarImageMetadataInterface::GetRSF() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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
  
  ossimString tempVal = kwl.find("sensor_params.sampling_frequency");
  double      freq = tempVal.toDouble();
  
  return freq;
}

ossimplugins::SceneCoord*
TerraSarImageMetadataInterface::GetSceneCoord() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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

  ossimplugins::SceneCoord* sceneCoord = new ossimplugins::SceneCoord();
  sceneCoord->loadState(kwl, "");

  return sceneCoord;
}

unsigned int
TerraSarImageMetadataInterface::GetNumberOfCornerIncidenceAngles() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);
  std::string key = "sceneCoord.numberOfSceneCornerCoord";
  ossimString nbCornerAngles = kwl.find(key.c_str());

  return static_cast<unsigned int>(nbCornerAngles.toInt());
}

double
TerraSarImageMetadataInterface::GetMeanIncidenceAngles() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  ImageKeywordlistType imageKeywordlist;

  double sum = 0.;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  itk::OStringStream oss;

  std::string key = "sceneCoord.sceneCenterCoord.incidenceAngle";
  ossimString centerIncidenceAngle = kwl.find(key.c_str());

  sum += centerIncidenceAngle.toDouble();

  unsigned int nbAngles = this->GetNumberOfCornerIncidenceAngles();
  for (unsigned int i = 0; i < nbAngles; i++)
    {
    oss.str("");
    oss << "sceneCoord.sceneCornerCoord[" << i << "].incidenceAngle";
    ossimString tempVal = kwl.find(oss.str().c_str());

    sum += tempVal.toDouble();
    }

  return (sum / (nbAngles + 1));
}

double
TerraSarImageMetadataInterface::GetCenterIncidenceAngle() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);
  std::string key = "sceneCoord.sceneCenterCoord.incidenceAngle";
  ossimString tempVal = kwl.find(key.c_str());

  return tempVal.toDouble();
}

TerraSarImageMetadataInterface::IndexType
TerraSarImageMetadataInterface::GetCenterIncidenceAngleIndex() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  TerraSarImageMetadataInterface::IndexType it;

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);
  std::string key = "sceneCoord.sceneCenterCoord.refRow";
  ossimString tempVal = kwl.find(key.c_str());

  it[1] = tempVal.toInt();

  key = "sceneCoord.sceneCenterCoord.refColumn";
  tempVal = kwl.find(key.c_str());

  it[0] = tempVal.toInt();

  return it;
}

TerraSarImageMetadataInterface::DoubleVectorType
TerraSarImageMetadataInterface::GetCornersIncidenceAngles() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  ImageKeywordlistType imageKeywordlist;

  TerraSarImageMetadataInterface::DoubleVectorType dv;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  itk::OStringStream oss;

  unsigned int nbAngles = this->GetNumberOfCornerIncidenceAngles();
  for (unsigned int i = 0; i < nbAngles; i++)
    {
    oss.str("");
    oss << "sceneCoord.sceneCornerCoord[" << i << "].incidenceAngle";
    ossimString tempVal = kwl.find(oss.str().c_str());

    dv.push_back(tempVal.toDouble());
    }


  return dv;
}

TerraSarImageMetadataInterface::IndexVectorType
TerraSarImageMetadataInterface::GetCornersIncidenceAnglesIndex() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

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

  unsigned int nbAngles = this->GetNumberOfCornerIncidenceAngles();
  for (unsigned int i = 0; i < nbAngles; i++)
    {
    TerraSarImageMetadataInterface::IndexType it;

    oss.str("");
    oss << "sceneCoord.sceneCornerCoord[" << i << "].refRow";
    ossimString tempVal = kwl.find(oss.str().c_str());

    it[1] = tempVal.toInt();

    oss2.str("");
    oss2 << "sceneCoord.sceneCornerCoord[" << i << "].refColumn";
    tempVal = kwl.find(oss2.str().c_str());

    it[0] = tempVal.toInt();

    iv.push_back(it);
    }

  return iv;
}


double
TerraSarImageMetadataInterface
::Horner(std::vector<double>& coefficients, const double tauMinusTauRef) const
{
  std::vector<double>::reverse_iterator coefIt = coefficients.rbegin();
  double res = *(coefIt);
  ++coefIt;

  while (coefIt < coefficients.rend())
    {
    // Cumulate polynomial
    res = res * tauMinusTauRef + (*coefIt);
    ++coefIt;
    }
  return res;
}


TerraSarImageMetadataInterface::PointSetPointer
TerraSarImageMetadataInterface
::GetRadiometricCalibrationNoise() const
{
  PointSetPointer points = PointSetType::New();

  IndexVectorType cornerIndex = this->GetCornersIncidenceAnglesIndex();
  unsigned int numberOfRows = 0;
  unsigned int numberOfCols = 0;

  for (unsigned int i = 0; i < cornerIndex.size(); ++i)
    {
    IndexType index;
    index = cornerIndex[i];
    unsigned int noRow = index[0];
    unsigned int noCol = index[1];
    if (noRow > numberOfRows)
      {
      numberOfRows = noRow;
      }
    if (noCol > numberOfCols)
      {
      numberOfCols = noCol;
      }
    }

  double startTime = this->GetStartTimeUTC();
  double stopTime = this->GetStopTimeUTC();
  RealType firstRangeTime = this->GetRangeTimeFirstPixel();
  RealType lastRangeTime = this->GetRangeTimeLastPixel();

  points->Initialize();
  unsigned int noPoint = 0;

  PointType p0;

  unsigned int numberOfNoiseRecords = this->GetNumberOfNoiseRecords();

  for (unsigned int noiseRecord = 0; noiseRecord < numberOfNoiseRecords; ++noiseRecord)
    {
    double currentNoiseTime = this->GetNoiseTimeUTC(noiseRecord);
    RealType AzimutAcquisition = (currentNoiseTime - startTime) * numberOfRows / (stopTime - startTime);
    RealType referencePointTime = this->GetNoiseReferencePoint(noiseRecord);

    std::vector<RealType> polynomialCoefficient;
    polynomialCoefficient = this->GetNoisePolynomialCoefficients(noiseRecord);

    p0[0] = AzimutAcquisition;

    for (unsigned int col = 0; col < numberOfCols; ++col)
      {
      RealType rangeTime = col * (lastRangeTime - firstRangeTime) / (numberOfCols) + firstRangeTime;
      RealType tauMinusTauRef = rangeTime - referencePointTime;
      RealType value = this->Horner(polynomialCoefficient, tauMinusTauRef);

      p0[1] = col;
      points->SetPoint(noPoint, p0);
      points->SetPointData(noPoint, value);
      ++noPoint;

      }
    }
  return points;
}

TerraSarImageMetadataInterface::IndexType
TerraSarImageMetadataInterface
::GetRadiometricCalibrationNoisePolynomialDegree() const
{
  IndexType polynomSize;
  polynomSize[0] = 2;
  polynomSize[1] = 2;

  return polynomSize;
}

double
TerraSarImageMetadataInterface
::GetStartTimeUTC() const
{
    const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
    if (!this->CanRead())
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
  key = "azimuth_start_time";

  ossimString  referenceUTC = kwl.find(key.c_str());

  double julianDay;
  julianDay = ConvertStringTimeUTCToJulianDay(referenceUTC);

    return julianDay;
}

double
TerraSarImageMetadataInterface
::GetStopTimeUTC() const
{
    const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
    if (!this->CanRead())
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
  key = "azimuth_stop_time";

  ossimString  referenceUTC = kwl.find(key.c_str());

  double julianDay;
  julianDay = ConvertStringTimeUTCToJulianDay(referenceUTC);

    return julianDay;
}

TerraSarImageMetadataInterface::RealType
TerraSarImageMetadataInterface
::GetRangeTimeFirstPixel() const
{
    const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
    if (!this->CanRead())
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
  key = "range_first_time";

  ossimString  rangeTimeTmp = kwl.find(key.c_str());

  double rangeTime = rangeTimeTmp.toDouble();

  return rangeTime;
}

TerraSarImageMetadataInterface::RealType
TerraSarImageMetadataInterface
::GetRangeTimeLastPixel() const
{
    const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
    if (!this->CanRead())
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
  key = "range_last_time";

  ossimString  rangeTimeTmp = kwl.find(key.c_str());

  double rangeTime = rangeTimeTmp.toDouble();

  return rangeTime;
}

TerraSarImageMetadataInterface::RealType
TerraSarImageMetadataInterface
::GetRadiometricCalibrationScale() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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
  key = "calibration.calibrationConstant.calFactor";

  ossimString calFactorValue = kwl.find(key.c_str());

  return calFactorValue.toDouble();
}

TerraSarImageMetadataInterface::PointSetPointer
TerraSarImageMetadataInterface
::GetRadiometricCalibrationIncidenceAngle() const
{
  PointSetPointer points = PointSetType::New();
  PointType p0;

  double centerIncidenceAngleValue = this->GetCenterIncidenceAngle();
  IndexType centerIncidenceAngleIndex = this->GetCenterIncidenceAngleIndex();

  DoubleVectorType cornerIncidenceAngleValue = this->GetCornersIncidenceAngles();
  IndexVectorType cornerIncidenceAngleIndex = this->GetCornersIncidenceAnglesIndex();
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

TerraSarImageMetadataInterface::IndexType
TerraSarImageMetadataInterface
::GetRadiometricCalibrationIncidenceAnglePolynomialDegree() const
{
  IndexType polynomSize;
  polynomSize[0] = 2;
  polynomSize[1] = 1;

  return polynomSize;
}


  /** Get the R, G, B channel */
std::vector<unsigned int>
TerraSarImageMetadataInterface
::GetDefaultDisplay() const
{
  std::vector<unsigned int> rgb(3);
  rgb[0] = 0;
  rgb[1] = 0;
  rgb[2] = 0;
  return rgb;
}

void
TerraSarImageMetadataInterface
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


} // end namespace otb
