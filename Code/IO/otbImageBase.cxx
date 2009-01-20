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

#include "itkMetaDataObject.h"

#include "otbImageBase.h"
#include "otbImageKeywordlist.h"

namespace otb
{


ImageBase::ImageBase()
{

}

std::string ImageBase::GetProjectionRef( const MetaDataDictionaryType & dict ) const
{
  std::string metadata;

  if(dict.HasKey(MetaDataKey::ProjectionRefKey))
     {
     itk::ExposeMetaData<std::string>(dict, static_cast<std::string>(MetaDataKey::ProjectionRefKey), metadata);
     return ( metadata );
     }
  else
     return ("");
}

std::string ImageBase::GetGCPProjection( const MetaDataDictionaryType & dict ) const
{
  std::string metadata;

  if(dict.HasKey(MetaDataKey::GCPProjectionKey))
     {
     itk::ExposeMetaData<std::string>(dict, static_cast<std::string>(MetaDataKey::GCPProjectionKey), metadata);
     return ( metadata );
     }
  else
     return ("");
}

unsigned int ImageBase::GetGCPCount( const MetaDataDictionaryType & dict) const
{
  unsigned int GCPCount = 0;

  if(dict.HasKey(MetaDataKey::GCPCountKey))
     {
     itk::ExposeMetaData<unsigned int>(dict, MetaDataKey::GCPCountKey, GCPCount);
     }

  return (GCPCount);
}


OTB_GCP & ImageBase::GetGCPs( MetaDataDictionaryType & dict, unsigned int GCPnum )
{
  std::string key;

  ::itk::OStringStream lStream;
  lStream << MetaDataKey::GCPParametersKey << GCPnum;
  key = lStream.str();

  if(dict.HasKey(key))
     {

     itk::ExposeMetaData<OTB_GCP>(dict, key, m_GCP);
     }
  return ( m_GCP );

}

std::string ImageBase::GetGCPId( const MetaDataDictionaryType & dict, unsigned int GCPnum ) const
{
  std::string key;

  ::itk::OStringStream lStream;
  lStream << MetaDataKey::GCPParametersKey << GCPnum;
  key = lStream.str();

  if(dict.HasKey(key))
     {
       OTB_GCP gcp;
       itk::ExposeMetaData<OTB_GCP>(dict, key, gcp);
       return ( gcp.m_Id );
     }
  else
     return ("");
}

std::string ImageBase::GetGCPInfo( const MetaDataDictionaryType & dict, unsigned int GCPnum ) const
{
  std::string key;

  ::itk::OStringStream lStream;
  lStream << MetaDataKey::GCPParametersKey << GCPnum;
  key = lStream.str();

  if(dict.HasKey(key))
     {
       OTB_GCP gcp;
     itk::ExposeMetaData<OTB_GCP>(dict, key, gcp);
     return ( gcp.m_Info );
     }
  else
     return ("");
}

double ImageBase::GetGCPRow( const MetaDataDictionaryType & dict, unsigned int GCPnum ) const
{
  std::string key;

  ::itk::OStringStream lStream;
  lStream << MetaDataKey::GCPParametersKey << GCPnum;
  key = lStream.str();


  if(dict.HasKey(key))
     {
       OTB_GCP gcp;
     itk::ExposeMetaData<OTB_GCP>(dict, key, gcp);
     return ( gcp.m_GCPRow );
     }
  else
     return (0);
}

double ImageBase::GetGCPCol( const MetaDataDictionaryType & dict, unsigned int GCPnum ) const
{
  std::string key;

  ::itk::OStringStream lStream;
  lStream << MetaDataKey::GCPParametersKey << GCPnum;
  key = lStream.str();


  if(dict.HasKey(key))
     {
       OTB_GCP gcp;
     itk::ExposeMetaData<OTB_GCP>(dict, key, gcp);
     return ( gcp.m_GCPCol );
     }
  else
     return (0);
}

double ImageBase::GetGCPX( const MetaDataDictionaryType & dict, unsigned int GCPnum ) const
{
  std::string key;

  ::itk::OStringStream lStream;
  lStream << MetaDataKey::GCPParametersKey << GCPnum;
  key = lStream.str();

  if(dict.HasKey(key))
     {
       OTB_GCP gcp;
     itk::ExposeMetaData<OTB_GCP>(dict, key, gcp);
     return ( gcp.m_GCPX );
     }
  else
     return (0);
}

double ImageBase::GetGCPY( const MetaDataDictionaryType & dict, unsigned int GCPnum ) const
{
  std::string key;

  ::itk::OStringStream lStream;
  lStream << MetaDataKey::GCPParametersKey << GCPnum;
  key = lStream.str();

  if(dict.HasKey(key))
     {
       OTB_GCP gcp;
     itk::ExposeMetaData<OTB_GCP>(dict, key, gcp);
     return ( gcp.m_GCPY );
     }
  else
     return (0);
}

double ImageBase::GetGCPZ( const MetaDataDictionaryType & dict, unsigned int GCPnum ) const
{
  std::string key;

  ::itk::OStringStream lStream;
  lStream << MetaDataKey::GCPParametersKey << GCPnum;
  key = lStream.str();

  if(dict.HasKey(key))
     {
       OTB_GCP gcp;
     itk::ExposeMetaData<OTB_GCP>(dict, key, gcp);
     return ( gcp.m_GCPZ );
     }
  else
     return (0);
}

ImageBase::VectorType ImageBase::GetGeoTransform( const MetaDataDictionaryType & dict ) const
{
  VectorType adfGeoTransform;

  if(dict.HasKey(MetaDataKey::GeoTransformKey))
     {
     itk::ExposeMetaData<VectorType>(dict, MetaDataKey::GeoTransformKey, adfGeoTransform);
     }
  return ( adfGeoTransform );
}

ImageBase::VectorType ImageBase::GetUpperLeftCorner( const MetaDataDictionaryType & dict ) const
{
  VectorType UpperLeftCorner;

  if(dict.HasKey(MetaDataKey::UpperLeftCornerKey))
     {
     itk::ExposeMetaData<VectorType>(dict, MetaDataKey::UpperLeftCornerKey, UpperLeftCorner);
     }
   return ( UpperLeftCorner );
}

ImageBase::VectorType ImageBase::GetUpperRightCorner( const MetaDataDictionaryType & dict ) const
{
  VectorType UpperRightCorner;

  if(dict.HasKey(MetaDataKey::UpperRightCornerKey))
     {
     itk::ExposeMetaData<VectorType>(dict, MetaDataKey::UpperRightCornerKey, UpperRightCorner);
     }
   return ( UpperRightCorner );
}

ImageBase::VectorType ImageBase::GetLowerLeftCorner( const MetaDataDictionaryType & dict ) const
{
  VectorType LowerLeftCorner;

  if(dict.HasKey(MetaDataKey::LowerLeftCornerKey))
     {
     itk::ExposeMetaData<VectorType>(dict, MetaDataKey::LowerLeftCornerKey, LowerLeftCorner);
     }
   return ( LowerLeftCorner );
}

ImageBase::VectorType ImageBase::GetLowerRightCorner( const MetaDataDictionaryType & dict ) const
{
  VectorType LowerRightCorner;

  if(dict.HasKey(MetaDataKey::LowerRightCornerKey))
     {
     itk::ExposeMetaData<VectorType>(dict, MetaDataKey::LowerRightCornerKey, LowerRightCorner);
     }
   return ( LowerRightCorner );
}

ImageBase::ImageKeywordlistType ImageBase::GetImageKeywordlist( MetaDataDictionaryType & dict )
{
  ImageKeywordlistType ImageKeywordlist;

  if(dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
     {
     itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, ImageKeywordlist);
     }
   return ( ImageKeywordlist );
}

const ImageBase::ImageKeywordlistType ImageBase::GetImageKeywordlist(const MetaDataDictionaryType & dict ) const
{
  ImageKeywordlistType ImageKeywordlist;

  if(dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, ImageKeywordlist);
  }
  return ( ImageKeywordlist );
}

void
ImageBase::PrintSelf(std::ostream& os, itk::Indent indent, const MetaDataDictionaryType & dict) const
{

  std::vector<std::string> keys = dict.GetKeys();
  std::string svalue;
  unsigned int ivalue(0);
  VectorType vvalue;
  double dvalue;
  OTB_GCP gcpvalue;
  ImageKeywordlist ossimvalue;
  unsigned int i(0);

  // Copy of the const metadata dictionary in a metadata dictionary to be used
  // by the ExposeMetaData method

  MetaDataDictionaryType dict2 = dict;
  MetaDataKey key;

  for (unsigned int itkey=0; itkey<keys.size(); itkey++)
     {


     switch( key.GetKeyType(keys[itkey]) )
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

           for(i = 0; i < vvalue.size(); i++ )
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
