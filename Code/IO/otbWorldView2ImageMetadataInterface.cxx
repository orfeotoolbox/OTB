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

#include "otbMacro.h"

#include "otbWorldView2ImageMetadataInterface.h"
#include "itkMetaDataObject.h"
#include "base/ossimKeywordlist.h"

namespace otb
{

WorldView2ImageMetadataInterface
::WorldView2ImageMetadataInterface()
{
}

bool
WorldView2ImageMetadataInterface::CanRead() const
{
  std::string sensorID = GetSensorID();
  if (sensorID.find("WV02") != std::string::npos) return true;
  else return false;
}

WorldView2ImageMetadataInterface::VariableLengthVectorType
WorldView2ImageMetadataInterface::GetSolarIrradiance() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no WorldView2 Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  VariableLengthVectorType outputValuesVariableLengthVector;
  ossimKeywordlist         kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string keyBId = "support_data.band_id";
  ossimString keywordStringBId = kwl.find(keyBId.c_str());
  if (keywordStringBId == ossimString("P"))
    {
    outputValuesVariableLengthVector.SetSize(1);
    outputValuesVariableLengthVector.Fill(1381.79);
    }
  else if (keywordStringBId == ossimString("Multi"))
    {
    outputValuesVariableLengthVector.SetSize(4);
    outputValuesVariableLengthVector[0] = 1924.59;
    outputValuesVariableLengthVector[1] = 1843.08;
    outputValuesVariableLengthVector[2] = 1574.77;
    outputValuesVariableLengthVector[3] = 1113.71;
    }
  else
    {
    itkExceptionMacro(<< "Invalid bandID " << keywordStringBId);
    }

  return outputValuesVariableLengthVector;
}

int
WorldView2ImageMetadataInterface::GetDay() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no WorldView2 Image");
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
  key = "support_data.tlc_date";
  separatorList = "-T";

  ossimString              keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  std::cout << "WorldView2ImageMetadataInterface::GetDay" << keywordString << std::endl;

  if (keywordStrings.size() <= 2) itkExceptionMacro(<< "Invalid Day");

  ossimString day = keywordStrings[2];

  return day.toInt();
}

int
WorldView2ImageMetadataInterface::GetMonth() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no WorldView2 Image");
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
  key = "support_data.tlc_date";
  separatorList = "-T";

  ossimString              keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if (keywordStrings.size() <= 2) itkExceptionMacro(<< "Invalid Month");

  ossimString month = keywordStrings[1];

  return month.toInt();
}

int
WorldView2ImageMetadataInterface::GetYear() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no WorldView2 Image");
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
  key = "support_data.tlc_date";
  separatorList = "-T";

  ossimString              keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if (keywordStrings.size() <= 2) itkExceptionMacro("Invalid Year");

  ossimString year = keywordStrings[0];

  return year.toInt();
}

int
WorldView2ImageMetadataInterface::GetHour() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no WorldView2 Image");
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
  key = "support_data.tlc_date";
  separatorList = "-T:";

  ossimString              keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if (keywordStrings.size() <= 2) itkExceptionMacro("Invalid Hour");

  ossimString hour = keywordStrings[3];

  return hour.toInt();
}

int
WorldView2ImageMetadataInterface::GetMinute() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no WorldView2 Image");
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
  key = "support_data.tlc_date";
  separatorList = "-T:";
  ossimString keywordString = kwl.find(key.c_str());

  if (keywordString == ossimString("Unknown"))
    {
    itkExceptionMacro("Unknown date")
    }

  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if (keywordStrings.size() <= 2) itkExceptionMacro("Invalid Minute");

  ossimString minute = keywordStrings[4];

  return minute.toInt();
}

int
WorldView2ImageMetadataInterface::GetProductionDay() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no WorldView2 Image");
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
  key = "support_data.generation_date";
  separatorList = "-T";

  ossimString              keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if (keywordStrings.size() <= 2) itkExceptionMacro(<< "Invalid Day");

  ossimString day = keywordStrings[2];

  return day.toInt();
}

int
WorldView2ImageMetadataInterface::GetProductionMonth() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no WorldView2 Image");
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
  key = "support_data.generation_date";
  separatorList = "-T";

  ossimString              keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if (keywordStrings.size() <= 2) itkExceptionMacro(<< "Invalid Month");

  ossimString month = keywordStrings[1];

  return month.toInt();
}

int
WorldView2ImageMetadataInterface::GetProductionYear() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no WorldView2 Image");
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
  key = "support_data.generation_date";
  separatorList = "-T";

  ossimString              keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if (keywordStrings.size() <= 2) itkExceptionMacro("Invalid Year");

  ossimString year = keywordStrings[0];

  return year.toInt();
}

WorldView2ImageMetadataInterface::VariableLengthVectorType
WorldView2ImageMetadataInterface
::GetPhysicalBias() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no WorldView2 Image");
    }
  ImageKeywordlistType ImageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, ImageKeywordlist);
    }
  ossimKeywordlist kwl;
  ImageKeywordlist.convertToOSSIMKeywordlist(kwl);

  VariableLengthVectorType outputValuesVariableLengthVector;
  std::string              keyBId = "support_data.band_id";
  ossimString              keywordStringBId = kwl.find(keyBId.c_str());
  if (keywordStringBId == ossimString("P"))
    {
    outputValuesVariableLengthVector.SetSize(1);
    outputValuesVariableLengthVector.Fill(0.0);
    }
  else if (keywordStringBId == ossimString("Multi"))
    {
	  ossimString keywordStringBandNameList = kwl.find("support_data.band_name_list");
	  std::vector<ossimString> bandNameList = keywordStringBandNameList.split(" ");

	  outputValuesVariableLengthVector.SetSize(bandNameList.size());
	  outputValuesVariableLengthVector.Fill(0.0);
    }
  else
    {
    itkExceptionMacro(<< "Invalid bandID " << keywordStringBId);
    }

  return outputValuesVariableLengthVector;
}

WorldView2ImageMetadataInterface::VariableLengthVectorType
WorldView2ImageMetadataInterface
::GetPhysicalGain() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no WorldView2 Image");
    }
  ImageKeywordlistType ImageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, ImageKeywordlist);
    }
  ossimKeywordlist kwl;
  ImageKeywordlist.convertToOSSIMKeywordlist(kwl);

  ossimString keywordStringBitsPerPixel = kwl.find("support_data.bits_per_pixel");
  int         bitsPerPixel = keywordStringBitsPerPixel.toInt();
  if (bitsPerPixel != 16)
    {
    itkExceptionMacro(<< "Invalid bitsPerPixel " << bitsPerPixel);
    }

  std::string keyBId = "support_data.band_id";
  ossimString keywordStringBId = kwl.find(keyBId.c_str());
  if (keywordStringBId != ossimString("P") && keywordStringBId != ossimString("Multi"))
    {
    itkExceptionMacro(<< "Invalid bandID " << keywordStringBId);
    }

  ossimString keywordStringTDILevel = kwl.find("support_data.TDI_level");
  int         TDILevel = keywordStringTDILevel.toInt();
  if ( (keywordStringBId == ossimString("P") && TDILevel >64) ||
		  (keywordStringBId == ossimString("P") && TDILevel < 8) )
    {
    itkExceptionMacro(<< "Invalid TDILevel " << TDILevel);
    }

  ossimString keywordStringBandNameList = kwl.find("support_data.band_name_list");
  std::vector<ossimString> bandNameList = keywordStringBandNameList.split(" ");

  VariableLengthVectorType outputValuesVariableLengthVector;
  outputValuesVariableLengthVector.SetSize(bandNameList.size());

  if (keywordStringBId == ossimString("P"))
    {
    ossimString keywordStringAbsCalFactor = kwl.find("support_data.absCalFactor");
    outputValuesVariableLengthVector[0] = keywordStringAbsCalFactor.toDouble();
    }
  else
    {
	  for(unsigned int i = 0 ; i < bandNameList.size(); ++i)
	  {
		  ossimString keywordString = "band_"+bandNameList[i]+".abscalfactor";
		  ossimString keywordStringAbsCalFactor = kwl.find(keywordString);
		  outputValuesVariableLengthVector[i] = keywordStringAbsCalFactor.toDouble();
	  }
    }

/* PIO : To be confirmed !!!
 if (keywordStringBId == ossimString("P"))
    {
    outputValuesVariableLengthVector[0] = 1.0 / outputValuesVariableLengthVector[0];
    }
  else
    {
    outputValuesVariableLengthVector[0] = 1.000 / outputValuesVariableLengthVector[0];
    outputValuesVariableLengthVector[1] = 1.000 / outputValuesVariableLengthVector[1];
    outputValuesVariableLengthVector[2] = 1.000 / outputValuesVariableLengthVector[2];
    outputValuesVariableLengthVector[3] = 1.000 / outputValuesVariableLengthVector[3];
    }
*/
  return outputValuesVariableLengthVector;
}

double
WorldView2ImageMetadataInterface::GetSatElevation() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no WorldView2 Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);
  std::string key = "support_data.sat_elevation_angle";
  ossimString keywordString = kwl.find(key.c_str());

  return keywordString.toDouble();
}

double
WorldView2ImageMetadataInterface::GetSatAzimuth() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no WorldView2 Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);
  std::string key = "support_data.sat_azimuth_angle";
  ossimString keywordString = kwl.find(key.c_str());

  return keywordString.toDouble();
}

WorldView2ImageMetadataInterface::VariableLengthVectorType
WorldView2ImageMetadataInterface
::GetFirstWavelengths() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no WorldView2 Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  VariableLengthVectorType wavel(1);
  wavel.Fill(0.);

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);
  std::string key = "support_data.band_id";
  ossimString keywordStringBId = kwl.find(key.c_str());

  if (keywordStringBId != ossimString("P") && keywordStringBId != ossimString("Multi"))
    {
    itkExceptionMacro(<< "Invalid bandID " << keywordStringBId);
    }

  // Panchromatic case
  if (keywordStringBId == ossimString("P"))
    {
    wavel.SetSize(1);
    wavel.Fill(0.464);
    }
  else
    {
	  ossimString keywordStringBandNameList = kwl.find("support_data.band_name_list");
	  std::vector<ossimString> bandNameList = keywordStringBandNameList.split(" ");

	  wavel.SetSize(bandNameList.size());
	  for(unsigned int i = 0 ; i < bandNameList.size(); ++i)
	    {
	    double waveValue = 0.0;
	    if(bandNameList[i] == "C")
	      {
	      waveValue = 0.401;
	      }
	    else if(bandNameList[i] == "B")
	      {
	      waveValue = 0.447;
	      }
	    else if(bandNameList[i] == "G")
	      {
	      waveValue = 0.511;
	      }
	    else if(bandNameList[i] == "Y")
	      {
	      waveValue = 0.588;
	      }
	    else if(bandNameList[i] == "R")
	      {
	      waveValue=0.630;
	      }
	    else if(bandNameList[i] == "RE")
	      {
	      waveValue = 0.704;
	      }
	    else if(bandNameList[i] == "N")
	      {
	      waveValue = 0.772;
	      }
	    else if(bandNameList[i] == "N2")
	      {
	      waveValue = 0.862;
	      }
	    wavel[i] = waveValue;
	    }
    }

  return wavel;
}

WorldView2ImageMetadataInterface::VariableLengthVectorType
WorldView2ImageMetadataInterface
::GetLastWavelengths() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no WorldView2 Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  VariableLengthVectorType wavel(1);
  wavel.Fill(0.);

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);
  std::string key = "support_data.band_id";
  ossimString keywordStringBId = kwl.find(key.c_str());

  if (keywordStringBId != ossimString("P") && keywordStringBId != ossimString("Multi"))
    {
    itkExceptionMacro(<< "Invalid bandID " << keywordStringBId);
    }

  // Panchromatic case
  if (keywordStringBId == ossimString("P"))
    {
    wavel.SetSize(1);
    wavel.Fill(0.801);
    }
  else
    {
	  ossimString keywordStringBandNameList = kwl.find("support_data.band_name_list");
	  std::vector<ossimString> bandNameList = keywordStringBandNameList.split(" ");

	  wavel.SetSize(bandNameList.size());
	  for(unsigned int i = 0 ; i < bandNameList.size(); ++i)
	    {
	    double waveValue = 0.0;
	    if(bandNameList[i] == "C")
	      {
	      waveValue = 0.453;
	      }
	    else if(bandNameList[i] == "B")
	      {
	      waveValue = 0.581;
	      }
	    else if(bandNameList[i] == "G")
	      {
	      waveValue = 0.581;
	      }
	    else if(bandNameList[i] == "Y")
	      {
	      waveValue = 0.627;
	      }
	    else if(bandNameList[i]="R")
	      {
	      waveValue=0.690;
	      }
	    else if(bandNameList[i] == "RE")
	      {
	      waveValue = 0.744;
	      }
	    else if(bandNameList[i] == "N")
	      {
	      waveValue = 0.890;
	      }
	    else if(bandNameList[i] == "N2")
	      {
	      waveValue = 0.954;
	      }
	    wavel[i] = waveValue;
	    }
    }

  return wavel;
}

unsigned int
WorldView2ImageMetadataInterface
::GetDefaultRBand() const
{
  // Handle both 4 bands and 8 bands wv2 products
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no WorldView2 Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }
  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  ossimString keywordStringBandNameList = kwl.find("support_data.band_name_list");
  std::vector<ossimString> bandNameList = keywordStringBandNameList.split(" ");
  if(bandNameList.size()>4)
    {
    return 4;
    }
  else
    {
    return 2;
    }
}

unsigned int
WorldView2ImageMetadataInterface
::GetDefaultGBand() const
{
  // Handle both 4 bands and 8 bands wv2 products
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no WorldView2 Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }
  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  ossimString keywordStringBandNameList = kwl.find("support_data.band_name_list");
  std::vector<ossimString> bandNameList = keywordStringBandNameList.split(" ");
  if(bandNameList.size()>4)
    {
    return 2;
    }
  else
    {
    return 1;
    }
}

unsigned int
WorldView2ImageMetadataInterface
::GetDefaultBBand() const
{
  // Handle both 4 bands and 8 bands wv2 products
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no WorldView2 Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }
  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);


  ossimString keywordStringBandNameList = kwl.find("support_data.band_name_list");
  std::vector<ossimString> bandNameList = keywordStringBandNameList.split(" ");
  if(bandNameList.size()>4)
    {
    return 1;
    }
  else
    {
    return 0;
    }
}

} // end namespace otb
