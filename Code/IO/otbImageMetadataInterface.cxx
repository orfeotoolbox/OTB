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

#include "itkMetaDataObject.h"

#include "otbImageMetadataInterface.h"
#include "otbImageKeywordlist.h"
#include "otbVectorDataKeywordlist.h"


namespace otb
{


ImageMetadataInterface::ImageMetadataInterface()
{

}

std::string ImageMetadataInterface::GetProjectionRef( const MetaDataDictionaryType & dict ) const
{
  std::string metadata;

  if (dict.HasKey(MetaDataKey::ProjectionRefKey))
  {
    itk::ExposeMetaData<std::string>(dict, static_cast<std::string>(MetaDataKey::ProjectionRefKey), metadata);
    return ( metadata );
  }
  else
    return ("");
}

std::string ImageMetadataInterface::GetGCPProjection( const MetaDataDictionaryType & dict ) const
{
  std::string metadata;

  if (dict.HasKey(MetaDataKey::GCPProjectionKey))
  {
    itk::ExposeMetaData<std::string>(dict, static_cast<std::string>(MetaDataKey::GCPProjectionKey), metadata);
    return ( metadata );
  }
  else
    return ("");
}

unsigned int ImageMetadataInterface::GetGCPCount( const MetaDataDictionaryType & dict) const
{
  unsigned int GCPCount = 0;

  if (dict.HasKey(MetaDataKey::GCPCountKey))
  {
    itk::ExposeMetaData<unsigned int>(dict, MetaDataKey::GCPCountKey, GCPCount);
  }

  return (GCPCount);
}


OTB_GCP & ImageMetadataInterface::GetGCPs( MetaDataDictionaryType & dict, unsigned int GCPnum )
{
  std::string key;

  ::itk::OStringStream lStream;
  lStream << MetaDataKey::GCPParametersKey << GCPnum;
  key = lStream.str();

  if (dict.HasKey(key))
  {

    itk::ExposeMetaData<OTB_GCP>(dict, key, m_GCP);
  }
  return ( m_GCP );

}

std::string ImageMetadataInterface::GetGCPId( const MetaDataDictionaryType & dict, unsigned int GCPnum ) const
{
  std::string key;

  ::itk::OStringStream lStream;
  lStream << MetaDataKey::GCPParametersKey << GCPnum;
  key = lStream.str();

  if (dict.HasKey(key))
  {
    OTB_GCP gcp;
    itk::ExposeMetaData<OTB_GCP>(dict, key, gcp);
    return ( gcp.m_Id );
  }
  else
    return ("");
}

std::string ImageMetadataInterface::GetGCPInfo( const MetaDataDictionaryType & dict, unsigned int GCPnum ) const
{
  std::string key;

  ::itk::OStringStream lStream;
  lStream << MetaDataKey::GCPParametersKey << GCPnum;
  key = lStream.str();

  if (dict.HasKey(key))
  {
    OTB_GCP gcp;
    itk::ExposeMetaData<OTB_GCP>(dict, key, gcp);
    return ( gcp.m_Info );
  }
  else
    return ("");
}

double ImageMetadataInterface::GetGCPRow( const MetaDataDictionaryType & dict, unsigned int GCPnum ) const
{
  std::string key;

  ::itk::OStringStream lStream;
  lStream << MetaDataKey::GCPParametersKey << GCPnum;
  key = lStream.str();


  if (dict.HasKey(key))
  {
    OTB_GCP gcp;
    itk::ExposeMetaData<OTB_GCP>(dict, key, gcp);
    return ( gcp.m_GCPRow );
  }
  else
    return (0);
}

double ImageMetadataInterface::GetGCPCol( const MetaDataDictionaryType & dict, unsigned int GCPnum ) const
{
  std::string key;

  ::itk::OStringStream lStream;
  lStream << MetaDataKey::GCPParametersKey << GCPnum;
  key = lStream.str();


  if (dict.HasKey(key))
  {
    OTB_GCP gcp;
    itk::ExposeMetaData<OTB_GCP>(dict, key, gcp);
    return ( gcp.m_GCPCol );
  }
  else
    return (0);
}

double ImageMetadataInterface::GetGCPX( const MetaDataDictionaryType & dict, unsigned int GCPnum ) const
{
  std::string key;

  ::itk::OStringStream lStream;
  lStream << MetaDataKey::GCPParametersKey << GCPnum;
  key = lStream.str();

  if (dict.HasKey(key))
  {
    OTB_GCP gcp;
    itk::ExposeMetaData<OTB_GCP>(dict, key, gcp);
    return ( gcp.m_GCPX );
  }
  else
    return (0);
}

double ImageMetadataInterface::GetGCPY( const MetaDataDictionaryType & dict, unsigned int GCPnum ) const
{
  std::string key;

  ::itk::OStringStream lStream;
  lStream << MetaDataKey::GCPParametersKey << GCPnum;
  key = lStream.str();

  if (dict.HasKey(key))
  {
    OTB_GCP gcp;
    itk::ExposeMetaData<OTB_GCP>(dict, key, gcp);
    return ( gcp.m_GCPY );
  }
  else
    return (0);
}

double ImageMetadataInterface::GetGCPZ( const MetaDataDictionaryType & dict, unsigned int GCPnum ) const
{
  std::string key;

  ::itk::OStringStream lStream;
  lStream << MetaDataKey::GCPParametersKey << GCPnum;
  key = lStream.str();

  if (dict.HasKey(key))
  {
    OTB_GCP gcp;
    itk::ExposeMetaData<OTB_GCP>(dict, key, gcp);
    return ( gcp.m_GCPZ );
  }
  else
    return (0);
}

ImageMetadataInterface::VectorType ImageMetadataInterface::GetGeoTransform( const MetaDataDictionaryType & dict ) const
{
  VectorType adfGeoTransform;

  if (dict.HasKey(MetaDataKey::GeoTransformKey))
  {
    itk::ExposeMetaData<VectorType>(dict, MetaDataKey::GeoTransformKey, adfGeoTransform);
  }
  return ( adfGeoTransform );
}

ImageMetadataInterface::VectorType ImageMetadataInterface::GetUpperLeftCorner( const MetaDataDictionaryType & dict ) const
{
  VectorType UpperLeftCorner;

  if (dict.HasKey(MetaDataKey::UpperLeftCornerKey))
  {
    itk::ExposeMetaData<VectorType>(dict, MetaDataKey::UpperLeftCornerKey, UpperLeftCorner);
  }
  return ( UpperLeftCorner );
}

ImageMetadataInterface::VectorType ImageMetadataInterface::GetUpperRightCorner( const MetaDataDictionaryType & dict ) const
{
  VectorType UpperRightCorner;

  if (dict.HasKey(MetaDataKey::UpperRightCornerKey))
  {
    itk::ExposeMetaData<VectorType>(dict, MetaDataKey::UpperRightCornerKey, UpperRightCorner);
  }
  return ( UpperRightCorner );
}

ImageMetadataInterface::VectorType ImageMetadataInterface::GetLowerLeftCorner( const MetaDataDictionaryType & dict ) const
{
  VectorType LowerLeftCorner;

  if (dict.HasKey(MetaDataKey::LowerLeftCornerKey))
  {
    itk::ExposeMetaData<VectorType>(dict, MetaDataKey::LowerLeftCornerKey, LowerLeftCorner);
  }
  return ( LowerLeftCorner );
}

ImageMetadataInterface::VectorType ImageMetadataInterface::GetLowerRightCorner( const MetaDataDictionaryType & dict ) const
{
  VectorType LowerRightCorner;

  if (dict.HasKey(MetaDataKey::LowerRightCornerKey))
  {
    itk::ExposeMetaData<VectorType>(dict, MetaDataKey::LowerRightCornerKey, LowerRightCorner);
  }
  return ( LowerRightCorner );
}

ImageMetadataInterface::ImageKeywordlistType ImageMetadataInterface::GetImageKeywordlist( MetaDataDictionaryType & dict )
{
  ImageKeywordlistType ImageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, ImageKeywordlist);
  }
  return ( ImageKeywordlist );
}

const ImageMetadataInterface::ImageKeywordlistType ImageMetadataInterface::GetImageKeywordlist(const MetaDataDictionaryType & dict ) const
{
  ImageKeywordlistType ImageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, ImageKeywordlist);
  }
  return ( ImageKeywordlist );
}


ImageMetadataInterface::VariableLengthVectorType
    ImageMetadataInterface::GetPhysicalBias( const MetaDataDictionaryType & dict ) const
{
  if(IsSpot(dict))
  {
    return GetSpotPhysicalBias(dict);
  }

  if(IsIkonos(dict))
  {
    return GetIkonosPhysicalBias(dict);
  }

  VariableLengthVectorType output(1);
  output.Fill(0);
  return output;
}

ImageMetadataInterface::VariableLengthVectorType
    ImageMetadataInterface::GetPhysicalGain( const MetaDataDictionaryType & dict ) const
{
  if(IsSpot(dict))
  {
    return GetSpotPhysicalGain(dict);
  }

  if(IsIkonos(dict))
  {
    return GetIkonosPhysicalGain(dict);
  }

  VariableLengthVectorType output(1);
  output.Fill(1);
  return output;
}



ImageMetadataInterface::VariableLengthVectorType
    ImageMetadataInterface::GetSolarIrradiance( const MetaDataDictionaryType & dict ) const
{
  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

 VariableLengthVectorType outputValuesVariableLengthVector;
 ossimKeywordlist kwl;
 imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  if(IsSpot(dict)) // and QB?
  {
    std::string key= "support_data.solar_irradiance";
    ossimString keywordString = kwl.find(key.c_str());
    ossimString separatorList = " ";
    std::vector<ossimString> keywordStrings = keywordString.split(separatorList);
    std::vector<double> outputValues;
    for (unsigned int i=0; i < keywordStrings.size(); ++i)
      {
	if (!keywordStrings[i].empty())
	  {
	    outputValues.push_back(keywordStrings[i].toDouble());
	  }
      }
    
    
    outputValuesVariableLengthVector.SetSize(outputValues.size());
    outputValuesVariableLengthVector.Fill(0);
    //In the case of SPOT, the bands are in a different order:
    // XS3, XS2. XS1, SWIR in the tif file.
    if(IsSpot(dict))
      {
	//assert(outputValues.size() == 4);//Valid for Spot 4 and 5
	if(outputValues.size() != 4)
	  itkGenericExceptionMacro(<<"Invalid Solar Irradiance");
	outputValuesVariableLengthVector[0]=outputValues[2];
	outputValuesVariableLengthVector[1]=outputValues[1];
	outputValuesVariableLengthVector[2]=outputValues[0];
	outputValuesVariableLengthVector[3]=outputValues[3];
      }
    else
      {
	for(unsigned int i=0; i<outputValues.size(); ++i)
	  {
	    outputValuesVariableLengthVector[i]=outputValues[i];
	  }
      }
  }
  else if(IsIkonos(dict))
    {
      outputValuesVariableLengthVector.SetSize(1);
      // values from geoeye
      std::string key= "support_data.band_name";
      ossimString keywordString = kwl.find(key.c_str());
      if(keywordString=="Pan")
	{
	  outputValuesVariableLengthVector[0] = 1375.8;
	}
      else if(keywordString=="Blue")
	{
	  outputValuesVariableLengthVector[0] = 1930.9;
	}
      else if(keywordString=="Green")
	{
	  outputValuesVariableLengthVector[0] = 1854.8;
	}
      else if(keywordString=="Red")
	{
	  outputValuesVariableLengthVector[0] = 1556.5;
	}
      else if(keywordString=="NIR")
	{
	  outputValuesVariableLengthVector[0] = 1156.9;
	}
    }


  return outputValuesVariableLengthVector;

}

double ImageMetadataInterface::GetSunElevation( const MetaDataDictionaryType & dict ) const
{
  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);
  std::string key= "support_data.elevation_angle";
  ossimString keywordString = kwl.find(key.c_str());

  return keywordString.toDouble();
}

double ImageMetadataInterface::GetSunAzimuth( const MetaDataDictionaryType & dict ) const
{
  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);
  std::string key= "support_data.azimuth_angle";
  ossimString keywordString = kwl.find(key.c_str());

  return keywordString.toDouble();
}

int ImageMetadataInterface::GetDay( const MetaDataDictionaryType & dict ) const
{
  //The image date in the ossim metadata has the form: 2007-10-03T03:17:16.973000
  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);
  
  std::string key;
  ossimString separatorList;
  if(IsSpot(dict))
  {
    key = "support_data.image_date";
    separatorList = "-T";
  }

  if(IsIkonos(dict))
  {
    key = "support_data.production_date";
    separatorList = "/";
  }

  ossimString keywordString = kwl.find(key.c_str());
  //ossimString separatorList = "-T";
  //std::string key= "support_data.image_date";
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);
  //assert(keywordStrings.size() > 2);
  if(keywordStrings.size() <= 2)
    itkExceptionMacro(<<"Invalid Day");

  return keywordStrings[2].toInt();
}


int ImageMetadataInterface::GetMonth( const MetaDataDictionaryType & dict ) const
{
  //The image date in the ossim metadata has the form: 2007-10-03T03:17:16.973000
  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;
  if(IsSpot(dict))
  {
    key = "support_data.image_date";
    separatorList = "-T";
  }

  if(IsIkonos(dict))
  {
    key = "support_data.production_date";
    separatorList = "/";
  }

  ossimString keywordString = kwl.find(key.c_str());
  //ossimString separatorList = "-T";
  //std::string key= "support_data.image_date";
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  //assert(keywordStrings.size() > 2);
  if(keywordStrings.size() <= 2)
    itkExceptionMacro(<<"Invalid Month");

  return keywordStrings[1].toInt();
}

int ImageMetadataInterface::GetYear( const MetaDataDictionaryType & dict ) const
{
std::cout<<"ImageMetadataInterface::GetYear"<<std::endl;
  //The image date in the ossim metadata has the form: 2007-10-03T03:17:16.973000
  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }
 
  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;
  if(IsSpot(dict))
  {
    key = "support_data.image_date";
    separatorList = "-T";
  }

  if(IsIkonos(dict))
  {
    key = "support_data.production_date";
    separatorList = "/";
  }

  ossimString keywordString = kwl.find(key.c_str());
  //ossimString separatorList = "-T";
  //std::string key= "support_data.image_date";
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);
  //assert(keywordStrings.size() > 2);
  if(  keywordStrings.size() <= 2 )
    itkExceptionMacro("Invalid Year");

  ossimString year = keywordStrings[0];
// For Ikonos 2002 is 02
 if(IsIkonos(dict))
  {
      year = "20"+year; 
  }

  return year.toInt();
  //return keywordStrings[0].toInt();
}

std::string ImageMetadataInterface::GetSensorID( const MetaDataDictionaryType & dict ) const
{
  ImageKeywordlistType ImageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, ImageKeywordlist);
  }
  ossimKeywordlist kwl;
  ImageKeywordlist.convertToOSSIMKeywordlist(kwl);
  std::string key= "sensor";
  ossimString keywordString = kwl.find(key.c_str());
  std::string output(keywordString.chars());

//std::cout<< "sensorID : "<<output<<std::endl;

  return output;
}

unsigned int ImageMetadataInterface::GetNumberOfBands( const MetaDataDictionaryType & dict ) const
{
  ImageKeywordlistType ImageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, ImageKeywordlist);
  }
  ossimKeywordlist kwl;
  ImageKeywordlist.convertToOSSIMKeywordlist(kwl);
  std::string key= "support_data.number_bands";
  ossimString keywordString = kwl.find(key.c_str());
  return keywordString.toUInt32();
}

std::vector<std::string> ImageMetadataInterface::GetBandName( const MetaDataDictionaryType & dict ) const
{
  ImageKeywordlistType ImageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, ImageKeywordlist);
  }
  ossimKeywordlist kwl;
  ImageKeywordlist.convertToOSSIMKeywordlist(kwl);
  std::string key= "support_data.band_name";
  ossimString keywordString = kwl.find(key.c_str());
  ossimString separatorList = " ";
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);
  std::vector<std::string> outputValues;
  for (unsigned int i=0; i < keywordStrings.size(); ++i)
  {
    if (!keywordStrings[i].empty())
    {
      outputValues.push_back(keywordStrings[i].chars());
    }
  }

  return outputValues;
}

bool ImageMetadataInterface::IsSpot( const MetaDataDictionaryType & dict ) const
{
  std::string sensorID = GetSensorID(dict);
  if (sensorID.find("Spot") != std::string::npos)
    return true;
  else
    return false;
}

bool ImageMetadataInterface::IsIkonos( const MetaDataDictionaryType & dict ) const
{
  std::string sensorID = GetSensorID(dict);
  if (sensorID.find("IKONOS-1") != std::string::npos || sensorID.find("IKONOS-2") != std::string::npos)
    return true;
  else
    return false;
}

bool ImageMetadataInterface::IsQuickbird( const MetaDataDictionaryType & dict ) const
{
  std::string sensorID = GetSensorID(dict);
  if (sensorID.find("QB") != std::string::npos)
    return true;
  else
    return false;
}

ImageMetadataInterface::VariableLengthVectorType
    ImageMetadataInterface::GetSpotPhysicalBias( const MetaDataDictionaryType & dict ) const
{
  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }
  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);
  std::string key= "support_data.physical_bias";
  ossimString keywordString = kwl.find(key.c_str());
  ossimString separatorList = " ";
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);
  std::vector<double> outputValues;
  for (unsigned int i=0; i < keywordStrings.size(); ++i)
  {
    if (!keywordStrings[i].empty())
    {
      outputValues.push_back(keywordStrings[i].toDouble());
    }
  }

  VariableLengthVectorType outputValuesVariableLengthVector;
  outputValuesVariableLengthVector.SetSize(outputValues.size());
  outputValuesVariableLengthVector.Fill(0);
  //In the case of SPOT, the bands are in a different order:
  // XS3, XS2. XS1, SWIR in the tif file.

  //assert(outputValues.size() == 4);//Valid for Spot 4 and 5
   if(  outputValues.size() != 4 )
     itkExceptionMacro("Invalid Physical Bias");

  outputValuesVariableLengthVector[0]=outputValues[2];
  outputValuesVariableLengthVector[1]=outputValues[1];
  outputValuesVariableLengthVector[2]=outputValues[0];
  outputValuesVariableLengthVector[3]=outputValues[3];


  return outputValuesVariableLengthVector;
}

ImageMetadataInterface::VariableLengthVectorType
    ImageMetadataInterface::GetSpotPhysicalGain( const MetaDataDictionaryType & dict ) const
{
  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

//   otbMsgDevMacro( << "  --- ImageKeywordlist: " << imageKeywordlist);
  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);
//   otbMsgDevMaquitcro( << "  --- ossimKeywordlist: " << kwl);
  std::string key= "support_data.physical_gain";
  ossimString keywordString = kwl.find(key.c_str());
  ossimString separatorList = " ";
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);
  std::vector<double> outputValues;
  for (unsigned int i=0; i < keywordStrings.size(); ++i)
  {
    if (!keywordStrings[i].empty())
    {
      outputValues.push_back(keywordStrings[i].toDouble());
    }
  }

  VariableLengthVectorType outputValuesVariableLengthVector;
  outputValuesVariableLengthVector.SetSize(outputValues.size());
  outputValuesVariableLengthVector.Fill(0);
  //In the case of SPOT, the bands are in a different order:
  // XS3, XS2. XS1, SWIR in the tif file.

  //assert(outputValues.size() == 4);//Valid for Spot 4 and 5
  if(  outputValues.size() != 4 )
    itkExceptionMacro("Invalid Physical Gain");
  outputValuesVariableLengthVector[0]=outputValues[2];
  outputValuesVariableLengthVector[1]=outputValues[1];
  outputValuesVariableLengthVector[2]=outputValues[0];
  outputValuesVariableLengthVector[3]=outputValues[3];


  return outputValuesVariableLengthVector;

}



/** CHECK THE BIAS : 0 0 0 0 !!! */
ImageMetadataInterface::VariableLengthVectorType
    ImageMetadataInterface::GetIkonosPhysicalBias( const MetaDataDictionaryType & dict ) const
{
  VariableLengthVectorType outputValuesVariableLengthVector;
  outputValuesVariableLengthVector.SetSize(GetNumberOfBands(dict));
  outputValuesVariableLengthVector.Fill(0.0);

  return outputValuesVariableLengthVector;
}

ImageMetadataInterface::VariableLengthVectorType
    ImageMetadataInterface::GetIkonosPhysicalGain( const MetaDataDictionaryType & dict ) const
{


  //Values are different pre/post 2001-01-22 production date, find out where we are
  ImageKeywordlistType ImageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, ImageKeywordlist);
  }
  ossimKeywordlist kwl;
  ImageKeywordlist.convertToOSSIMKeywordlist(kwl);
  std::string key= "support_data.production_date";
  ossimString keywordString = kwl.find(key.c_str());
  std::string output(keywordString.chars());

  //The Ikonos production date has the format MM/DD/YY
  ossimString separatorList = "/";
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);
  if (keywordStrings.size() < 3)
  {
    itkGenericExceptionMacro(<<"Could not retrieve the production date for Ikonos");
  }

  int productionYear = keywordStrings[2].toInt();
  int productionMonth = keywordStrings[0].toInt();
  int productionDay = keywordStrings[1].toInt();
  bool isPost20010122 = false;
  if ((productionYear > 2) || (productionYear < 99)) isPost20010122 = true;
  else
  {
    if (productionYear == 2)
    {
      if (productionMonth > 1) isPost20010122 = true;
      else
        if (productionDay >= 22) isPost20010122 = true;
    }
  }

    //Value computed from
  // http://www.geoeye.com/CorpSite/assets/docs/technical-papers/2009/IKONOS_Esun_Calculations.pdf
  // to get the equivalent of the SPOT alpha
  VariableLengthVectorType gain;
  gain.SetSize(5);
  if (isPost20010122)
  {
    gain[0] = 6.48830;//Pan
    gain[1] = 5.19064;//Blue
    gain[2] = 6.44122;//Green
    gain[3] = 6.24442;//Red
    gain[4] = 8.04222;//NIR
  }
  else
  {
    gain[0] = 6.48830;//Pan
    gain[1] = 4.51329;//Blue
    gain[2] = 5.75014;//Green
    gain[3] = 5.52720;//Red
    gain[4] = 7.11684;//NIR
  }


  std::vector<std::string> bandName = GetBandName(dict);

  VariableLengthVectorType outputValuesVariableLengthVector;
  unsigned int numBands = GetNumberOfBands(dict);
  outputValuesVariableLengthVector.SetSize(numBands);
  for(unsigned int i=0; i<numBands; ++i)
  {
    if (bandName[i].find("Pan") != std::string::npos)
      outputValuesVariableLengthVector[i]=gain[0];
    if (bandName[i].find("Blue") != std::string::npos)
      outputValuesVariableLengthVector[i]=gain[1];
    if (bandName[i].find("Green") != std::string::npos)
      outputValuesVariableLengthVector[i]=gain[2];
    if (bandName[i].find("Red") != std::string::npos)
      outputValuesVariableLengthVector[i]=gain[3];
    if (bandName[i].find("NIR") != std::string::npos)
      outputValuesVariableLengthVector[i]=gain[4];
  }



  return outputValuesVariableLengthVector;
}

void
ImageMetadataInterface::PrintSelf(std::ostream& os, itk::Indent indent, const MetaDataDictionaryType & dict) const
{

  std::vector<std::string> keys = dict.GetKeys();
  std::string svalue;
  unsigned int ivalue(0);
  VectorType vvalue;
  double dvalue = 0.;
  OTB_GCP gcpvalue;
  ImageKeywordlist ossimvalue;
  VectorDataKeywordlist vectorDataKeywordlistValue;
  unsigned int i(0);

  // Copy of the const metadata dictionary in a metadata dictionary to be used
  // by the ExposeMetaData method

  MetaDataDictionaryType dict2 = dict;
  MetaDataKey key;

  for (unsigned int itkey=0; itkey<keys.size(); ++itkey)
  {


    switch ( key.GetKeyType(keys[itkey]) )
    {
    case MetaDataKey::TSTRING:
      itk::ExposeMetaData<std::string>(dict2, keys[itkey], svalue);
      os << indent << "---> " << keys[itkey] << " = " << svalue << std::endl;
      break;

    case MetaDataKey::TENTIER:
      itk::ExposeMetaData<unsigned int>(dict2, keys[itkey], ivalue);
      os << indent << "---> " << keys[itkey] << " = " << ivalue << std::endl;
      break;

    case MetaDataKey::TVECTOR:
      itk::ExposeMetaData<VectorType>(dict2, keys[itkey], vvalue);

      for (i = 0; i < vvalue.size(); ++i )
      {
        os << indent << "---> " << keys[itkey] << "[" << i <<"] = "<< vvalue[i]<< std::endl;
      }
      vvalue.clear();

      break;

    case MetaDataKey::TDOUBLE:
      itk::ExposeMetaData<double>(dict2, keys[itkey], dvalue);
      os << indent << "---> " << keys[itkey] << " = " << dvalue << std::endl;
      break;

    case MetaDataKey::TOTB_GCP:
      itk::ExposeMetaData<OTB_GCP>(dict2, keys[itkey], gcpvalue);

      os << indent << "---> " << keys[itkey] << std::endl;
      gcpvalue.Print(os);
      break;
    case MetaDataKey::TOSSIMKEYWORDLIST:
      itk::ExposeMetaData<ImageKeywordlist>(dict2, keys[itkey], ossimvalue);

      os << indent << "---> " << keys[itkey] << std::endl;
      ossimvalue.Print(os);
      // ossimvalue.Print(os);
      break;
    case MetaDataKey::TVECTORDATAKEYWORDLIST:
      itk::ExposeMetaData<VectorDataKeywordlist>(dict2, keys[itkey], vectorDataKeywordlistValue);

      os << indent << "---> " << keys[itkey] << std::endl;
      vectorDataKeywordlistValue.Print(os);
      break;
    default:
      break;
    }

  }

}

} // end namespace otb
