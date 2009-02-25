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
#ifndef __otbVectorImage_txx
#define __otbVectorImage_txx


#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "otbVectorImage.h"

namespace otb
{

template <class TPixel, unsigned int VImageDimension>
VectorImage<TPixel,VImageDimension>::VectorImage()
{
  m_ImageMetadataInterface = ImageMetadataInterface::New();
}

template <class TPixel, unsigned int VImageDimension>
std::string VectorImage<TPixel, VImageDimension>::GetProjectionRef( void ) const
{
  return ( m_ImageMetadataInterface->GetProjectionRef( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
std::string VectorImage<TPixel, VImageDimension>::GetGCPProjection( void ) const
{
  return ( m_ImageMetadataInterface->GetGCPProjection( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
unsigned int VectorImage<TPixel, VImageDimension>::GetGCPCount( void  ) const
{
  return ( m_ImageMetadataInterface->GetGCPCount( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
OTB_GCP & VectorImage<TPixel, VImageDimension>::GetGCPs ( unsigned int GCPnum )
{
  return ( m_ImageMetadataInterface->GetGCPs( this->GetMetaDataDictionary(), GCPnum ) );
}

template <class TPixel, unsigned int VImageDimension>
std::string VectorImage<TPixel, VImageDimension>::GetGCPId( unsigned int GCPnum  ) const
{
  return ( m_ImageMetadataInterface->GetGCPId( this->GetMetaDataDictionary(), GCPnum ) );
}

template <class TPixel, unsigned int VImageDimension>
std::string VectorImage<TPixel, VImageDimension>::GetGCPInfo( unsigned int GCPnum ) const
{
  return ( m_ImageMetadataInterface->GetGCPInfo( this->GetMetaDataDictionary(), GCPnum ) );
}

template <class TPixel, unsigned int VImageDimension>
double VectorImage<TPixel, VImageDimension>::GetGCPRow( unsigned int GCPnum ) const
{
  return ( m_ImageMetadataInterface->GetGCPRow( this->GetMetaDataDictionary(), GCPnum ) );
}

template <class TPixel, unsigned int VImageDimension>
double VectorImage<TPixel, VImageDimension>::GetGCPCol( unsigned int GCPnum ) const
{
  return ( m_ImageMetadataInterface->GetGCPCol( this->GetMetaDataDictionary(), GCPnum ) );
}

template <class TPixel, unsigned int VImageDimension>
double VectorImage<TPixel, VImageDimension>::GetGCPX( unsigned int GCPnum ) const
{
  return ( m_ImageMetadataInterface->GetGCPX( this->GetMetaDataDictionary(), GCPnum ) );
}

template <class TPixel, unsigned int VImageDimension>
double VectorImage<TPixel, VImageDimension>::GetGCPY( unsigned int GCPnum ) const
{
  return ( m_ImageMetadataInterface->GetGCPY( this->GetMetaDataDictionary(), GCPnum ) );
}

template <class TPixel, unsigned int VImageDimension>
double VectorImage<TPixel, VImageDimension>::GetGCPZ( unsigned int GCPnum ) const
{
  return ( m_ImageMetadataInterface->GetGCPZ( this->GetMetaDataDictionary(), GCPnum ) );
}

template <class TPixel, unsigned int VImageDimension>
ImageMetadataInterface::VectorType VectorImage<TPixel, VImageDimension>::GetGeoTransform( void ) const
{
  return ( m_ImageMetadataInterface->GetGeoTransform( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
ImageMetadataInterface::VectorType VectorImage<TPixel, VImageDimension>::GetUpperLeftCorner( void ) const
{
  return ( m_ImageMetadataInterface->GetUpperLeftCorner( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
ImageMetadataInterface::VectorType VectorImage<TPixel, VImageDimension>::GetUpperRightCorner( void ) const
{
  return ( m_ImageMetadataInterface->GetUpperRightCorner( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
ImageMetadataInterface::VectorType VectorImage<TPixel, VImageDimension>::GetLowerLeftCorner( void ) const
{
  return ( m_ImageMetadataInterface->GetLowerLeftCorner( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
ImageMetadataInterface::VectorType VectorImage<TPixel, VImageDimension>::GetLowerRightCorner( void ) const
{
  return ( m_ImageMetadataInterface->GetLowerRightCorner( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
ImageMetadataInterface::ImageKeywordlistType VectorImage<TPixel, VImageDimension>::GetImageKeywordlist( void )
{
  return ( m_ImageMetadataInterface->GetImageKeywordlist( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
const ImageMetadataInterface::ImageKeywordlistType VectorImage<TPixel, VImageDimension>::GetImageKeywordlist( void ) const
{
  return ( m_ImageMetadataInterface->GetImageKeywordlist( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
void
VectorImage<TPixel, VImageDimension>
::CopyInformation(const itk::DataObject * data)
{
  Superclass::CopyInformation(data);
  this->itk::Object::SetMetaDataDictionary(data->GetMetaDataDictionary());
}

template <class TPixel, unsigned int VImageDimension>
void
VectorImage<TPixel, VImageDimension>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  m_ImageMetadataInterface->PrintSelf( os, indent, this->GetMetaDataDictionary() );

}


} // end namespace otb
#endif
