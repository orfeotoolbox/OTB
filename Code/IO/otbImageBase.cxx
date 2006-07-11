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


namespace otb
{


ImageBase::ImageBase()
{
	
}

std::string ImageBase::GetProjectionRef(MetaDataDictionaryType & dict )
{
  std::string metadata;
  
  if(dict.HasKey(MetaDataKey::m_ProjectionRefKey))
     {
     itk::ExposeMetaData<std::string>(dict, static_cast<std::string>(MetaDataKey::m_ProjectionRefKey), metadata);
     return ( metadata );
     }
  else
     return ("");
}

std::string ImageBase::GetGCPProjection(MetaDataDictionaryType & dict )
{
  std::string metadata;
  
  if(dict.HasKey(MetaDataKey::m_GCPProjectionKey))
     {
     itk::ExposeMetaData<std::string>(dict, static_cast<std::string>(MetaDataKey::m_GCPProjectionKey), metadata);
     return ( metadata );
     }
  else
     return ("");
}

unsigned int ImageBase::GetGCPCount(MetaDataDictionaryType & dict)
{
  unsigned int GCPCount = 0;
  
  if(dict.HasKey(MetaDataKey::m_GCPCountKey))
     {
     itk::ExposeMetaData<unsigned int>(dict, MetaDataKey::m_GCPCountKey, GCPCount);
     }

  return (GCPCount);
}


OTB_GCP & ImageBase::GetGCPs(MetaDataDictionaryType & dict, unsigned int GCPnum )
{
  std::string key;
  
  ::itk::OStringStream lStream;
  lStream << MetaDataKey::m_GCPParametersKey << GCPnum;
  key = lStream.str(); 
  
  if(dict.HasKey(key))
     {
     itk::ExposeMetaData<OTB_GCP>(dict, key, m_GCP);
     }
  return ( m_GCP );	
	
}

std::string ImageBase::GetGCPId(MetaDataDictionaryType & dict, unsigned int GCPnum )
{
  std::string key;
  
  ::itk::OStringStream lStream;
  lStream << MetaDataKey::m_GCPParametersKey << GCPnum;
  key = lStream.str(); 
  
  if(dict.HasKey(key))
     {
     itk::ExposeMetaData<OTB_GCP>(dict, key, m_GCP);
     return ( m_GCP.m_Id );
     }
  else
     return ("");
}

std::string ImageBase::GetGCPInfo(MetaDataDictionaryType & dict, unsigned int GCPnum )
{
  std::string key;
  
  ::itk::OStringStream lStream;
  lStream << MetaDataKey::m_GCPParametersKey << GCPnum;
  key = lStream.str(); 
  
  if(dict.HasKey(key))
     {
     itk::ExposeMetaData<OTB_GCP>(dict, key, m_GCP);
     return ( m_GCP.m_Info );
     }
  else
     return ("");
}

double ImageBase::GetGCPRow(MetaDataDictionaryType & dict, unsigned int GCPnum )
{
  std::string key;
  
  ::itk::OStringStream lStream;
  lStream << MetaDataKey::m_GCPParametersKey << GCPnum;
  key = lStream.str(); 
  

  if(dict.HasKey(key))
     {
     itk::ExposeMetaData<OTB_GCP>(dict, key, m_GCP);
     return ( m_GCP.m_GCPRow );
     }
  else
     return (0);
}

double ImageBase::GetGCPCol(MetaDataDictionaryType & dict, unsigned int GCPnum )
{
  std::string key;
  
  ::itk::OStringStream lStream;
  lStream << MetaDataKey::m_GCPParametersKey << GCPnum;
  key = lStream.str(); 
  

  if(dict.HasKey(key))
     {
     itk::ExposeMetaData<OTB_GCP>(dict, key, m_GCP);
     return ( m_GCP.m_GCPCol );
     }
  else
     return (0);
}

double ImageBase::GetGCPX(MetaDataDictionaryType & dict, unsigned int GCPnum )
{
  std::string key;
  
  ::itk::OStringStream lStream;
  lStream << MetaDataKey::m_GCPParametersKey << GCPnum;
  key = lStream.str(); 
      
  if(dict.HasKey(key))
     {
     itk::ExposeMetaData<OTB_GCP>(dict, key, m_GCP);
     return ( m_GCP.m_GCPX );
     }
  else
     return (0);
}

double ImageBase::GetGCPY(MetaDataDictionaryType & dict, unsigned int GCPnum )
{
  std::string key;
  
  ::itk::OStringStream lStream;
  lStream << MetaDataKey::m_GCPParametersKey << GCPnum;
  key = lStream.str(); 
  
  if(dict.HasKey(key))
     {
     itk::ExposeMetaData<OTB_GCP>(dict, key, m_GCP);
     return ( m_GCP.m_GCPY );
     }
  else
     return (0);
}

double ImageBase::GetGCPZ(MetaDataDictionaryType & dict, unsigned int GCPnum )
{
  std::string key;
  
  ::itk::OStringStream lStream;
  lStream << MetaDataKey::m_GCPParametersKey << GCPnum;
  key = lStream.str(); 
     
  if(dict.HasKey(key))
     {
     itk::ExposeMetaData<OTB_GCP>(dict, key, m_GCP);
     return ( m_GCP.m_GCPZ );
     }
  else
     return (0);
}

ImageBase::VectorType ImageBase::GetGeoTransform( MetaDataDictionaryType & dict )
{
  VectorType adfGeoTransform;
  
  if(dict.HasKey(MetaDataKey::m_GeoTransformKey))
     {
     itk::ExposeMetaData<VectorType>(dict, MetaDataKey::m_GeoTransformKey, adfGeoTransform);
     }
  return ( adfGeoTransform );
}

ImageBase::VectorType ImageBase::GetUpperLeftCorner( MetaDataDictionaryType & dict )
{
  VectorType UpperLeftCorner;
  
  if(dict.HasKey(MetaDataKey::m_UpperLeftCornerKey))
     {
     itk::ExposeMetaData<VectorType>(dict, MetaDataKey::m_UpperLeftCornerKey, UpperLeftCorner);     
     }
   return ( UpperLeftCorner );
}  

ImageBase::VectorType ImageBase::GetUpperRightCorner( MetaDataDictionaryType & dict )
{
  VectorType UpperRightCorner;
  
  if(dict.HasKey(MetaDataKey::m_UpperRightCornerKey))
     {
     itk::ExposeMetaData<VectorType>(dict, MetaDataKey::m_UpperRightCornerKey, UpperRightCorner);     
     }
   return ( UpperRightCorner );
}  

ImageBase::VectorType ImageBase::GetLowerLeftCorner( MetaDataDictionaryType & dict )
{
  VectorType LowerLeftCorner;
  
  if(dict.HasKey(MetaDataKey::m_LowerLeftCornerKey))
     {
     itk::ExposeMetaData<VectorType>(dict, MetaDataKey::m_LowerLeftCornerKey, LowerLeftCorner);     
     }
   return ( LowerLeftCorner );
}  

ImageBase::VectorType ImageBase::GetLowerRightCorner( MetaDataDictionaryType & dict )
{
  VectorType LowerRightCorner;
  
  if(dict.HasKey(MetaDataKey::m_LowerRightCornerKey))
     {
     itk::ExposeMetaData<VectorType>(dict, MetaDataKey::m_LowerRightCornerKey, LowerRightCorner);     
     }
   return ( LowerRightCorner );
}   

void
ImageBase::PrintSelf(std::ostream& os, itk::Indent indent, const MetaDataDictionaryType & dict) const
{ 

  std::vector<std::string> keys = dict.GetKeys();
  std::string svalue;
  unsigned int ivalue;
  VectorType vvalue;
  double dvalue;
  OTB_GCP gcpvalue;
  unsigned int i ;
  
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
           
           for(i = 0 ; i < vvalue.size() ; i++ )
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
        }

     }
  
}    
  
} // end namespace otb
