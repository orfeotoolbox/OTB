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

#include "otbImageMetadataInterfaceBase.h"

#include "otbMacro.h"
#include "otbImageKeywordlist.h"
#include "itkMetaDataObject.h"
#include "itksys/SystemTools.hxx"

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

  std::ostringstream lStream;
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

  std::ostringstream lStream;
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

  std::ostringstream lStream;
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

  std::ostringstream lStream;
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

  std::ostringstream lStream;
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

  std::ostringstream lStream;
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

  std::ostringstream lStream;
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

  std::ostringstream lStream;
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
  ImageKeywordlistType imageKeywordlist;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }
  return (imageKeywordlist);
}

const ImageMetadataInterfaceBase::ImageKeywordlistType
ImageMetadataInterfaceBase::GetImageKeywordlist() const
{
  ImageKeywordlistType imageKeywordlist;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }
  return (imageKeywordlist);
}


std::string
ImageMetadataInterfaceBase::GetSensorID() const
{
  ImageKeywordlistType imageKeywordlist;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }
  if (!imageKeywordlist.HasKey("sensor"))
    {
    return "";
    }

  return imageKeywordlist.GetMetadataByKey("sensor");
}

unsigned int
ImageMetadataInterfaceBase::GetNumberOfBands() const
{
  ImageKeywordlistType imageKeywordlist;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  if (!imageKeywordlist.HasKey("support_data.number_bands"))
    {
    return 0;
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.number_bands");
  unsigned int value = atoi(valueString.c_str());
  return value;
}

std::vector<std::string>
ImageMetadataInterfaceBase::GetBandName() const
{
  ImageKeywordlistType imageKeywordlist;
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  std::vector<std::string> outputValues;
  if (!imageKeywordlist.HasKey("support_data.band_name"))
    {
    return outputValues;
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.band_name");
  itksys::SystemTools::Split(valueString.c_str(), outputValues, '/');

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

  if (imageKeywordlist.HasKey("meters_per_pixel_x"))
    {
    std::string valueString = imageKeywordlist.GetMetadataByKey("meters_per_pixel_x");
    double value = atof(valueString.c_str());
    return value;
    }

  if (imageKeywordlist.HasKey("pixel_spacing"))
    {
    std::string valueString = imageKeywordlist.GetMetadataByKey("pixel_spacing");
    double value = atof(valueString.c_str());
    return value;
    }

  return 0;
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

  if (imageKeywordlist.HasKey("meters_per_pixel_y"))
    {
    std::string valueString = imageKeywordlist.GetMetadataByKey("meters_per_pixel_y");
    double value = atof(valueString.c_str());
    return value;
    }

  if (imageKeywordlist.HasKey("pixel_spacing"))
    {
    std::string valueString = imageKeywordlist.GetMetadataByKey("pixel_spacing");
    double value = atof(valueString.c_str());
    return value;
    }

  return 0;

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
  ImageKeywordlist         kwl;
// an extra dependency just for printing is a bad idea.
//  VectorDataKeywordlist    vectorDataKeywordlistValue;
  unsigned int             i(0);

  // Copy of the const metadata dictionary in a metadata dictionary to be used
  // by the ExposeMetaData method

  MetaDataDictionaryType dict2 = dict;

  for (unsigned int itkey = 0; itkey < keys.size(); ++itkey)
    {

    switch (MetaDataKey::GetKeyType(keys[itkey]))
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
        itk::ExposeMetaData<ImageKeywordlist>(dict2, keys[itkey], kwl);

        os << indent << "---> " << keys[itkey] << std::endl;
        kwl.Print(os);
        break;
//      case MetaDataKey::TVECTORDATAKEYWORDLIST:
//        itk::ExposeMetaData<VectorDataKeywordlist>(dict2, keys[itkey], vectorDataKeywordlistValue);
//
//        os << indent << "---> " << keys[itkey] << std::endl;
//        vectorDataKeywordlistValue.Print(os);
//        break;
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

  bool canRead = this->CanRead();

  os << indent << "Initialized: " << (canRead ? true : false) << std::endl;
  if (canRead)
    {
    std::vector<unsigned int> defaultDisplay = this->GetDefaultDisplay();
    os << indent << "Default RGB Display: ["
                 << defaultDisplay[0] << ", "
                 << defaultDisplay[1] << ", "
                 << defaultDisplay[2] << "]" << std::endl;
    os << indent << "ProjectionRef:   " <<  this->GetProjectionRef() << std::endl;
    os << indent << "GCPProjection:   " << this->GetGCPProjection( ) << std::endl;
    os << indent << "GCPCount:        " << this->GetGCPCount( ) << std::endl;
    for(unsigned int gcpIdx = 0; gcpIdx  < this->GetGCPCount(); ++ gcpIdx)
    {
      //os << indent << "GCPs:            " << this->GetGCPs(gcpIdx) << std::endl;
      os << indent << "GCPId:           " << this->GetGCPId(gcpIdx) << std::endl;
      os << indent << "GCPInfo:         " << this->GetGCPInfo(gcpIdx) << std::endl;
    }
    //os << indent << "GeoTransform:    " << this->GetGeoTransform( ) << std::endl;
    //os << indent << "UpperLeftCorner: " << this->GetUpperLeftCorner( ) << std::endl;
    //os << indent << "UpperRightCorner:" << this->GetUpperRightCorner( ) << std::endl;
    //os << indent << "LowerLeftCorner: " << this->GetLowerLeftCorner( ) << std::endl;
    //os << indent << "LowerRightCorner:" << this->GetLowerRightCorner( ) << std::endl;
    //os << indent << "ImageKeywordlist:" << this->GetImageKeywordlist( ) << std::endl;
    os << indent << "SensorID:        " << this->GetSensorID( ) << std::endl;
    os << indent << "NumberOfBands:   " << this->GetNumberOfBands( ) << std::endl;
    //os << indent << "BandName:        " << this->GetBandName( ) << std::endl;
    os << indent << "XPixelSpacing:   " << this->GetXPixelSpacing( ) << std::endl;
    os << indent << "YPixelSpacing:   " << this->GetYPixelSpacing( ) << std::endl;
    os << indent << "Day:             " << this->GetDay( ) << std::endl;
    os << indent << "Month:           " << this->GetMonth( ) << std::endl;
    os << indent << "Year:            " << this->GetYear( ) << std::endl;
    os << indent << "Hour:            " << this->GetHour( ) << std::endl;
    os << indent << "Minute:          " << this->GetMinute( ) << std::endl;
    os << indent << "ProductionDay:   " << this->GetProductionDay( ) << std::endl;
    os << indent << "ProductionMonth: " << this->GetProductionMonth( ) << std::endl;
    os << indent << "ProductionYear:  " << this->GetProductionYear( ) << std::endl;
    }
}


} // end namespace otb
