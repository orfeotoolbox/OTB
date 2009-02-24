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
#ifndef __otbImage_txx
#define __otbImage_txx

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "otbImage.h"

namespace otb
{

template <class TPixel, unsigned int VImageDimension>
Image<TPixel,VImageDimension>::Image()
{
  m_ImageMetadata = ImageMetadata::New();
}

template <class TPixel, unsigned int VImageDimension>
std::string Image<TPixel, VImageDimension>::GetProjectionRef( void ) const
{
  return ( m_ImageMetadata->GetProjectionRef( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
std::string Image<TPixel, VImageDimension>::GetGCPProjection( void ) const
{
  return ( m_ImageMetadata->GetGCPProjection( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
unsigned int Image<TPixel, VImageDimension>::GetGCPCount( void  ) const
{
  return ( m_ImageMetadata->GetGCPCount( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
OTB_GCP & Image<TPixel, VImageDimension>::GetGCPs ( unsigned int GCPnum )
{
  return (m_ImageMetadata->GetGCPs( this->GetMetaDataDictionary(), GCPnum ) );
}

template <class TPixel, unsigned int VImageDimension>
std::string Image<TPixel, VImageDimension>::GetGCPId( unsigned int GCPnum  ) const
{
  return ( m_ImageMetadata->GetGCPId( this->GetMetaDataDictionary(), GCPnum ) );
}

template <class TPixel, unsigned int VImageDimension>
std::string Image<TPixel, VImageDimension>::GetGCPInfo( unsigned int GCPnum ) const
{
  return ( m_ImageMetadata->GetGCPInfo( this->GetMetaDataDictionary(), GCPnum ) );
}

template <class TPixel, unsigned int VImageDimension>
double Image<TPixel, VImageDimension>::GetGCPRow( unsigned int GCPnum ) const
{
  return ( m_ImageMetadata->GetGCPRow( this->GetMetaDataDictionary(), GCPnum ) );
}

template <class TPixel, unsigned int VImageDimension>
double Image<TPixel, VImageDimension>::GetGCPCol( unsigned int GCPnum ) const
{
  return ( m_ImageMetadata->GetGCPCol( this->GetMetaDataDictionary(), GCPnum ) );
}

template <class TPixel, unsigned int VImageDimension>
double Image<TPixel, VImageDimension>::GetGCPX( unsigned int GCPnum ) const
{
  return ( m_ImageMetadata->GetGCPX( this->GetMetaDataDictionary(), GCPnum ) );
}

template <class TPixel, unsigned int VImageDimension>
double Image<TPixel, VImageDimension>::GetGCPY( unsigned int GCPnum ) const
{
  return ( m_ImageMetadata->GetGCPY( this->GetMetaDataDictionary(), GCPnum ) );
}

template <class TPixel, unsigned int VImageDimension>
double Image<TPixel, VImageDimension>::GetGCPZ( unsigned int GCPnum ) const
{
  return ( m_ImageMetadata->GetGCPZ( this->GetMetaDataDictionary(), GCPnum ) );
}

template <class TPixel, unsigned int VImageDimension>
ImageMetadata::VectorType Image<TPixel, VImageDimension>::GetGeoTransform( void ) const
{
  return ( m_ImageMetadata->GetGeoTransform( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
ImageMetadata::VectorType Image<TPixel, VImageDimension>::GetUpperLeftCorner( void ) const
{
  return ( m_ImageMetadata->GetUpperLeftCorner( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
ImageMetadata::VectorType Image<TPixel, VImageDimension>::GetUpperRightCorner( void ) const
{
  return ( m_ImageMetadata->GetUpperRightCorner( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
ImageMetadata::VectorType Image<TPixel, VImageDimension>::GetLowerLeftCorner( void ) const
{
  return ( m_ImageMetadata->GetLowerLeftCorner( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
ImageMetadata::VectorType Image<TPixel, VImageDimension>::GetLowerRightCorner( void ) const
{
  return ( m_ImageMetadata->GetLowerRightCorner( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
ImageMetadata::ImageKeywordlistType Image<TPixel, VImageDimension>::GetImageKeywordlist( void )
{
  return ( m_ImageMetadata->GetImageKeywordlist( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
const ImageMetadata::ImageKeywordlistType Image<TPixel, VImageDimension>::GetImageKeywordlist( void ) const
{
  return ( m_ImageMetadata->GetImageKeywordlist( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
void
Image<TPixel, VImageDimension>
::CopyInformation(const itk::DataObject * data)
{
  Superclass::CopyInformation(data);
  this->itk::Object::SetMetaDataDictionary(data->GetMetaDataDictionary());
}

template <class TPixel, unsigned int VImageDimension>
void
Image<TPixel, VImageDimension>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  m_ImageMetadata->PrintSelf( os, indent, this->GetMetaDataDictionary() );

}


} // end namespace otb
#endif
