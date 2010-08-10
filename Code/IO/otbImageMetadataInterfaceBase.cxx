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

#include "otbImageMetadataInterfaceBase.h"

#include "itkMetaDataObject.h"
#include "otbVectorDataKeywordlist.h"
#include "base/ossimKeywordlist.h"

namespace otb
{

ImageMetadataInterfaceBase
::ImageMetadataInterfaceBase()
{
}

std::string
ImageMetadataInterfaceBase::GetProjectionRef() const
{
  std::string metadata;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  if (dict.HasKey(MetaDataKey::ProjectionRefKey))
    {
    itk::ExposeMetaData<std::string>(dict, static_cast<std::string>(MetaDataKey::ProjectionRefKey), metadata);
    return (metadata);
    }
  else return ("");
}

std::string
ImageMetadataInterfaceBase::GetGCPProjection() const
{
  std::string metadata;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  if (dict.HasKey(MetaDataKey::GCPProjectionKey))
    {
    itk::ExposeMetaData<std::string>(dict, static_cast<std::string>(MetaDataKey::GCPProjectionKey), metadata);
    return (metadata);
    }
  else return ("");
}

unsigned int
ImageMetadataInterfaceBase::GetGCPCount() const
{
  unsigned int GCPCount = 0;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  if (dict.HasKey(MetaDataKey::GCPCountKey))
    {
    itk::ExposeMetaData<unsigned int>(dict, MetaDataKey::GCPCountKey, GCPCount);
    }

  return (GCPCount);
}

OTB_GCP&
ImageMetadataInterfaceBase::GetGCPs(unsigned int GCPnum)
{
  std::string key;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  ::itk::OStringStream lStream;
  lStream << MetaDataKey::GCPParametersKey << GCPnum;
  key = lStream.str();

  if (dict.HasKey(key))
    {

    itk::ExposeMetaData<OTB_GCP>(dict, key, m_GCP);
    }
  return (m_GCP);
}

std::string
ImageMetadataInterfaceBase::GetGCPId(unsigned int GCPnum) const
{
  std::string key;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  ::itk::OStringStream lStream;
  lStream << MetaDataKey::GCPParametersKey << GCPnum;
  key = lStream.str();

  if (dict.HasKey(key))
    {
    OTB_GCP gcp;
    itk::ExposeMetaData<OTB_GCP>(dict, key, gcp);
    return (gcp.m_Id);
    }
  else return ("");
}

std::string
ImageMetadataInterfaceBase::GetGCPInfo(unsigned int GCPnum) const
{
  std::string key;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  ::itk::OStringStream lStream;
  lStream << MetaDataKey::GCPParametersKey << GCPnum;
  key = lStream.str();

  if (dict.HasKey(key))
    {
    OTB_GCP gcp;
    itk::ExposeMetaData<OTB_GCP>(dict, key, gcp);
    return (gcp.m_Info);
    }
  else return ("");
}

double
ImageMetadataInterfaceBase::GetGCPRow(unsigned int GCPnum) const
{
  std::string key;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  ::itk::OStringStream lStream;
  lStream << MetaDataKey::GCPParametersKey << GCPnum;
  key = lStream.str();

  if (dict.HasKey(key))
    {
    OTB_GCP gcp;
    itk::ExposeMetaData<OTB_GCP>(dict, key, gcp);
    return (gcp.m_GCPRow);
    }
  else return (0);
}

double
ImageMetadataInterfaceBase::GetGCPCol(unsigned int GCPnum) const
{
  std::string key;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  ::itk::OStringStream lStream;
  lStream << MetaDataKey::GCPParametersKey << GCPnum;
  key = lStream.str();

  if (dict.HasKey(key))
    {
    OTB_GCP gcp;
    itk::ExposeMetaData<OTB_GCP>(dict, key, gcp);
    return (gcp.m_GCPCol);
    }
  else return (0);
}

double
ImageMetadataInterfaceBase::GetGCPX(unsigned int GCPnum) const
{
  std::string key;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  ::itk::OStringStream lStream;
  lStream << MetaDataKey::GCPParametersKey << GCPnum;
  key = lStream.str();

  if (dict.HasKey(key))
    {
    OTB_GCP gcp;
    itk::ExposeMetaData<OTB_GCP>(dict, key, gcp);
    return (gcp.m_GCPX);
    }
  else return (0);
}

double
ImageMetadataInterfaceBase::GetGCPY(unsigned int GCPnum) const
{
  std::string key;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  ::itk::OStringStream lStream;
  lStream << MetaDataKey::GCPParametersKey << GCPnum;
  key = lStream.str();

  if (dict.HasKey(key))
    {
    OTB_GCP gcp;
    itk::ExposeMetaData<OTB_GCP>(dict, key, gcp);
    return (gcp.m_GCPY);
    }
  else return (0);
}

double
ImageMetadataInterfaceBase::GetGCPZ(unsigned int GCPnum) const
{
  std::string key;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  ::itk::OStringStream lStream;
  lStream << MetaDataKey::GCPParametersKey << GCPnum;
  key = lStream.str();

  if (dict.HasKey(key))
    {
    OTB_GCP gcp;
    itk::ExposeMetaData<OTB_GCP>(dict, key, gcp);
    return (gcp.m_GCPZ);
    }
  else return (0);
}

ImageMetadataInterfaceBase::VectorType
ImageMetadataInterfaceBase::GetGeoTransform() const
{
  VectorType adfGeoTransform;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  if (dict.HasKey(MetaDataKey::GeoTransformKey))
    {
    itk::ExposeMetaData<VectorType>(dict, MetaDataKey::GeoTransformKey, adfGeoTransform);
    }
  return (adfGeoTransform);
}

ImageMetadataInterfaceBase::VectorType
ImageMetadataInterfaceBase::GetUpperLeftCorner() const
{
  VectorType UpperLeftCorner;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  if (dict.HasKey(MetaDataKey::UpperLeftCornerKey))
    {
    itk::ExposeMetaData<VectorType>(dict, MetaDataKey::UpperLeftCornerKey, UpperLeftCorner);
    }
  return (UpperLeftCorner);
}

ImageMetadataInterfaceBase::VectorType
ImageMetadataInterfaceBase::GetUpperRightCorner() const
{
  VectorType UpperRightCorner;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  if (dict.HasKey(MetaDataKey::UpperRightCornerKey))
    {
    itk::ExposeMetaData<VectorType>(dict, MetaDataKey::UpperRightCornerKey, UpperRightCorner);
    }
  return (UpperRightCorner);
}

ImageMetadataInterfaceBase::VectorType
ImageMetadataInterfaceBase::GetLowerLeftCorner() const
{
  VectorType LowerLeftCorner;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  if (dict.HasKey(MetaDataKey::LowerLeftCornerKey))
    {
    itk::ExposeMetaData<VectorType>(dict, MetaDataKey::LowerLeftCornerKey, LowerLeftCorner);
    }
  return (LowerLeftCorner);
}

ImageMetadataInterfaceBase::VectorType
ImageMetadataInterfaceBase::GetLowerRightCorner() const
{
  VectorType LowerRightCorner;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  if (dict.HasKey(MetaDataKey::LowerRightCornerKey))
    {
    itk::ExposeMetaData<VectorType>(dict, MetaDataKey::LowerRightCornerKey, LowerRightCorner);
    }
  return (LowerRightCorner);
}

ImageMetadataInterfaceBase::ImageKeywordlistType
ImageMetadataInterfaceBase::GetImageKeywordlist()
{
  ImageKeywordlistType ImageKeywordlist;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, ImageKeywordlist);
    }
  return (ImageKeywordlist);
}

const ImageMetadataInterfaceBase::ImageKeywordlistType
ImageMetadataInterfaceBase::GetImageKeywordlist() const
{
  ImageKeywordlistType ImageKeywordlist;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, ImageKeywordlist);
    }
  return (ImageKeywordlist);
}



std::string
ImageMetadataInterfaceBase::GetSensorID() const
{
  ImageKeywordlistType ImageKeywordlist;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

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

unsigned int
ImageMetadataInterfaceBase::GetNumberOfBands() const
{
  ImageKeywordlistType ImageKeywordlist;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, ImageKeywordlist);
    }
  ossimKeywordlist kwl;
  ImageKeywordlist.convertToOSSIMKeywordlist(kwl);
  std::string key = "support_data.number_bands";
  ossimString keywordString = kwl.find(key.c_str());
  return keywordString.toUInt32();
}

std::vector<std::string>
ImageMetadataInterfaceBase::GetBandName() const
{
  ImageKeywordlistType ImageKeywordlist;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, ImageKeywordlist);
    }
  ossimKeywordlist kwl;
  ImageKeywordlist.convertToOSSIMKeywordlist(kwl);
  std::string              key = "support_data.band_name";
  ossimString              keywordString = kwl.find(key.c_str());
  ossimString              separatorList = " /";
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);
  std::vector<std::string> outputValues;
  for (unsigned int i = 0; i < keywordStrings.size(); ++i)
    {
    if (!keywordStrings[i].empty())
      {
      outputValues.push_back(keywordStrings[i].chars());
      }
    }

  return outputValues;
}

double
ImageMetadataInterfaceBase::GetXPixelSpacing() const
{
  ImageKeywordlistType imageKeywordlist;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);
  std::string key = "pixel_spacing";
  ossimString keywordString = kwl.find(key.c_str());

  return keywordString.toDouble();
}

double
ImageMetadataInterfaceBase::GetYPixelSpacing() const
{
  ImageKeywordlistType imageKeywordlist;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);
  std::string key = "pixel_spacing";
  ossimString keywordString = kwl.find(key.c_str());

  return keywordString.toDouble();
}


void
ImageMetadataInterfaceBase::PrintSelf(std::ostream& os, itk::Indent indent, const MetaDataDictionaryType& dict) const
{

  std::vector<std::string> keys = dict.GetKeys();
  std::string              svalue;
  unsigned int             ivalue(0);
  VectorType               vvalue;
  double                   dvalue = 0.;
  OTB_GCP                  gcpvalue;
  ImageKeywordlist         ossimvalue;
  VectorDataKeywordlist    vectorDataKeywordlistValue;
  unsigned int             i(0);

  // Copy of the const metadata dictionary in a metadata dictionary to be used
  // by the ExposeMetaData method

  MetaDataDictionaryType dict2 = dict;
  MetaDataKey            key;

  for (unsigned int itkey = 0; itkey < keys.size(); ++itkey)
    {

    switch (key.GetKeyType(keys[itkey]))
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

        for (i = 0; i < vvalue.size(); ++i)
          {
          os << indent << "---> " << keys[itkey] << "[" << i << "] = " << vvalue[i] << std::endl;
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


void
ImageMetadataInterfaceBase
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "DefaultRBand :      "  << this->GetDefaultRBand() << std::endl;
  os << indent << "DefaultGBand :      "  << this->GetDefaultGBand() << std::endl;
  os << indent << "DefaultBBand :      "  << this->GetDefaultBBand() << std::endl;
  os << indent << "GetProjectionRef:   " <<  this->GetProjectionRef() << std::endl;
  os << indent << "GetGCPProjection:   " << this->GetGCPProjection( ) << std::endl;
  os << indent << "GetGCPCount:        " << this->GetGCPCount( ) << std::endl;
  for(unsigned int gcpIdx = 0 ; gcpIdx  < this->GetGCPCount() ; ++ gcpIdx)
  {
	  //os << indent << "GetGCPs:            " << this->GetGCPs(gcpIdx) << std::endl;
	  os << indent << "GetGCPId:           " << this->GetGCPId(gcpIdx) << std::endl;
	  os << indent << "GetGCPInfo:         " << this->GetGCPInfo(gcpIdx) << std::endl;
  }
  //os << indent << "GetGeoTransform:    " << this->GetGeoTransform( ) << std::endl;
  //os << indent << "GetUpperLeftCorner: " << this->GetUpperLeftCorner( ) << std::endl;
  //os << indent << "GetUpperRightCorner:" << this->GetUpperRightCorner( ) << std::endl;
  //os << indent << "GetLowerLeftCorner: " << this->GetLowerLeftCorner( ) << std::endl;
  //os << indent << "GetLowerRightCorner:" << this->GetLowerRightCorner( ) << std::endl;
  //os << indent << "GetImageKeywordlist:" << this->GetImageKeywordlist( ) << std::endl;
  os << indent << "GetSensorID:        " << this->GetSensorID( ) << std::endl;
  os << indent << "GetNumberOfBands:   " << this->GetNumberOfBands( ) << std::endl;
  //os << indent << "GetBandName:        " << this->GetBandName( ) << std::endl;
  os << indent << "GetXPixelSpacing:   " << this->GetXPixelSpacing( ) << std::endl;
  os << indent << "GetYPixelSpacing:   " << this->GetYPixelSpacing( ) << std::endl;
  os << indent << "GetDay:             " << this->GetDay( ) << std::endl;
  os << indent << "GetMonth:           " << this->GetMonth( ) << std::endl;
  os << indent << "GetYear:            " << this->GetYear( ) << std::endl;
  os << indent << "GetHour:            " << this->GetHour( ) << std::endl;
  os << indent << "GetMinute:          " << this->GetMinute( ) << std::endl;
  os << indent << "GetProductionDay:   " << this->GetProductionDay( ) << std::endl;
  os << indent << "GetProductionMonth: " << this->GetProductionMonth( ) << std::endl;
  os << indent << "GetProductionYear:  " << this->GetProductionYear( ) << std::endl;

}


} // end namespace otb
