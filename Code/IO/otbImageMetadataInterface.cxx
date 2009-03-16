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

#include "itkMetaDataObject.h"

#include "otbImageMetadataInterface.h"
#include "otbImageKeywordlist.h"

#include "otbMacro.h"

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
  for (int i=0; i < keywordStrings.size(); ++i)
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
  if(IsSpot(dict))
  {
    assert(outputValues.size() == 4);//Valid for Spot 4 and 5
    outputValuesVariableLengthVector[0]=outputValues[2];
    outputValuesVariableLengthVector[1]=outputValues[1];
    outputValuesVariableLengthVector[2]=outputValues[0];
    outputValuesVariableLengthVector[3]=outputValues[3];
  }
  else
  {
    for(int i=0; i<outputValues.size(); ++i)
    {
      outputValuesVariableLengthVector[i]=outputValues[i];
    }
  }

  return outputValuesVariableLengthVector;
}

ImageMetadataInterface::VariableLengthVectorType
    ImageMetadataInterface::GetPhysicalGain( const MetaDataDictionaryType & dict ) const
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
  for (int i=0; i < keywordStrings.size(); ++i)
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
  if(IsSpot(dict))
  {
    assert(outputValues.size() == 4);//Valid for Spot 4 and 5
    outputValuesVariableLengthVector[0]=outputValues[2];
    outputValuesVariableLengthVector[1]=outputValues[1];
    outputValuesVariableLengthVector[2]=outputValues[0];
    outputValuesVariableLengthVector[3]=outputValues[3];
  }
  else
  {
    for(int i=0; i<outputValues.size(); ++i)
    {
      outputValuesVariableLengthVector[i]=outputValues[i];
    }
  }

  return outputValuesVariableLengthVector;

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
  return output;
}

bool ImageMetadataInterface::IsSpot( const MetaDataDictionaryType & dict ) const
{
  std::string sensorID = GetSensorID(dict);
  if (sensorID.find("Spot") != std::string::npos)
    return true;
  else
    return false;
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
  unsigned int i(0);

  // Copy of the const metadata dictionary in a metadata dictionary to be used
  // by the ExposeMetaData method

  MetaDataDictionaryType dict2 = dict;
  MetaDataKey key;

  for (unsigned int itkey=0; itkey<keys.size(); itkey++)
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

      for (i = 0; i < vvalue.size(); i++ )
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
    }

  }

}

} // end namespace otb
