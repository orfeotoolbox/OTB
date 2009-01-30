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
}

template <class TPixel, unsigned int VImageDimension>
std::string Image<TPixel, VImageDimension>::GetProjectionRef( void ) const
{
  return ( this->ImageBase::GetProjectionRef( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
std::string Image<TPixel, VImageDimension>::GetGCPProjection( void ) const
{
  return ( this->ImageBase::GetGCPProjection( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
unsigned int Image<TPixel, VImageDimension>::GetGCPCount( void  ) const
{
  return ( this->ImageBase::GetGCPCount( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
OTB_GCP & Image<TPixel, VImageDimension>::GetGCPs ( unsigned int GCPnum )
{
  return (this->ImageBase::GetGCPs( this->GetMetaDataDictionary(), GCPnum ) );
}

template <class TPixel, unsigned int VImageDimension>
std::string Image<TPixel, VImageDimension>::GetGCPId( unsigned int GCPnum  ) const
{
  return ( this->ImageBase::GetGCPId( this->GetMetaDataDictionary(), GCPnum ) );
}

template <class TPixel, unsigned int VImageDimension>
std::string Image<TPixel, VImageDimension>::GetGCPInfo( unsigned int GCPnum ) const
{
  return ( this->ImageBase::GetGCPInfo( this->GetMetaDataDictionary(), GCPnum ) );
}

template <class TPixel, unsigned int VImageDimension>
double Image<TPixel, VImageDimension>::GetGCPRow( unsigned int GCPnum ) const
{
  return ( this->ImageBase::GetGCPRow( this->GetMetaDataDictionary(), GCPnum ) );
}

template <class TPixel, unsigned int VImageDimension>
double Image<TPixel, VImageDimension>::GetGCPCol( unsigned int GCPnum ) const
{
  return ( this->ImageBase::GetGCPCol( this->GetMetaDataDictionary(), GCPnum ) );
}

template <class TPixel, unsigned int VImageDimension>
double Image<TPixel, VImageDimension>::GetGCPX( unsigned int GCPnum ) const
{
  return ( this->ImageBase::GetGCPX( this->GetMetaDataDictionary(), GCPnum ) );
}

template <class TPixel, unsigned int VImageDimension>
double Image<TPixel, VImageDimension>::GetGCPY( unsigned int GCPnum ) const
{
  return ( this->ImageBase::GetGCPY( this->GetMetaDataDictionary(), GCPnum ) );
}

template <class TPixel, unsigned int VImageDimension>
double Image<TPixel, VImageDimension>::GetGCPZ( unsigned int GCPnum ) const
{
  return ( this->ImageBase::GetGCPZ( this->GetMetaDataDictionary(), GCPnum ) );
}

template <class TPixel, unsigned int VImageDimension>
ImageBase::VectorType Image<TPixel, VImageDimension>::GetGeoTransform( void ) const
{
  return ( this->ImageBase::GetGeoTransform( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
ImageBase::VectorType Image<TPixel, VImageDimension>::GetUpperLeftCorner( void ) const
{
  return ( this->ImageBase::GetUpperLeftCorner( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
ImageBase::VectorType Image<TPixel, VImageDimension>::GetUpperRightCorner( void ) const
{
  return ( this->ImageBase::GetUpperRightCorner( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
ImageBase::VectorType Image<TPixel, VImageDimension>::GetLowerLeftCorner( void ) const
{
  return ( this->ImageBase::GetLowerLeftCorner( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
ImageBase::VectorType Image<TPixel, VImageDimension>::GetLowerRightCorner( void ) const
{
  return ( this->ImageBase::GetLowerRightCorner( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
ImageBase::ImageKeywordlistType Image<TPixel, VImageDimension>::GetImageKeywordlist( void )
{
  return ( this->ImageBase::GetImageKeywordlist( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
const ImageBase::ImageKeywordlistType Image<TPixel, VImageDimension>::GetImageKeywordlist( void ) const
{
  return ( this->ImageBase::GetImageKeywordlist( this->GetMetaDataDictionary() ) );
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

  this->ImageBase::PrintSelf( os, indent, this->GetMetaDataDictionary() );

}


} // end namespace otb
#endif
