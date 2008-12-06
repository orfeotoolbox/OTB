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
}

template <class TPixel, unsigned int VImageDimension>
std::string VectorImage<TPixel, VImageDimension>::GetProjectionRef( void )
{
   return ( ImageBase::GetProjectionRef( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
std::string VectorImage<TPixel, VImageDimension>::GetGCPProjection( void )
{
   return ( ImageBase::GetGCPProjection( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
unsigned int VectorImage<TPixel, VImageDimension>::GetGCPCount( void  )
{
   return ( ImageBase::GetGCPCount( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
OTB_GCP & VectorImage<TPixel, VImageDimension>::GetGCPs ( unsigned int GCPnum )
{
   return ( this->ImageBase::GetGCPs( this->GetMetaDataDictionary(), GCPnum ) );
}

template <class TPixel, unsigned int VImageDimension>
std::string VectorImage<TPixel, VImageDimension>::GetGCPId( unsigned int GCPnum  )
{
   return ( ImageBase::GetGCPId( this->GetMetaDataDictionary(), GCPnum ) );
}

template <class TPixel, unsigned int VImageDimension>
std::string VectorImage<TPixel, VImageDimension>::GetGCPInfo( unsigned int GCPnum )
{
   return ( ImageBase::GetGCPInfo( this->GetMetaDataDictionary(), GCPnum ) );
}

template <class TPixel, unsigned int VImageDimension>
double VectorImage<TPixel, VImageDimension>::GetGCPRow( unsigned int GCPnum )
{
   return ( ImageBase::GetGCPRow( this->GetMetaDataDictionary(), GCPnum ) );
}

template <class TPixel, unsigned int VImageDimension>
double VectorImage<TPixel, VImageDimension>::GetGCPCol( unsigned int GCPnum )
{
   return ( ImageBase::GetGCPCol( this->GetMetaDataDictionary(), GCPnum ) );
}

template <class TPixel, unsigned int VImageDimension>
double VectorImage<TPixel, VImageDimension>::GetGCPX( unsigned int GCPnum )
{
   return ( ImageBase::GetGCPX( this->GetMetaDataDictionary(), GCPnum ) );
}

template <class TPixel, unsigned int VImageDimension>
double VectorImage<TPixel, VImageDimension>::GetGCPY( unsigned int GCPnum )
{
   return ( ImageBase::GetGCPY( this->GetMetaDataDictionary(), GCPnum ) );
}

template <class TPixel, unsigned int VImageDimension>
double VectorImage<TPixel, VImageDimension>::GetGCPZ( unsigned int GCPnum )
{
   return ( ImageBase::GetGCPZ( this->GetMetaDataDictionary(), GCPnum ) );
}

template <class TPixel, unsigned int VImageDimension>
ImageBase::VectorType VectorImage<TPixel, VImageDimension>::GetGeoTransform( void )
{
   return ( ImageBase::GetGeoTransform( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
ImageBase::VectorType VectorImage<TPixel, VImageDimension>::GetUpperLeftCorner( void )
{
   return ( ImageBase::GetUpperLeftCorner( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
ImageBase::VectorType VectorImage<TPixel, VImageDimension>::GetUpperRightCorner( void )
{
   return ( ImageBase::GetUpperRightCorner( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
ImageBase::VectorType VectorImage<TPixel, VImageDimension>::GetLowerLeftCorner( void )
{
   return ( ImageBase::GetLowerLeftCorner( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
ImageBase::VectorType VectorImage<TPixel, VImageDimension>::GetLowerRightCorner( void )
{
   return ( ImageBase::GetLowerRightCorner( this->GetMetaDataDictionary() ) );
}

template <class TPixel, unsigned int VImageDimension>
ImageBase::ImageKeywordlistType VectorImage<TPixel, VImageDimension>::GetImageKeywordlist( void )
{
   return ( ImageBase::GetImageKeywordlist( this->GetMetaDataDictionary() ) );
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

  this->ImageBase::PrintSelf( os, indent, this->GetMetaDataDictionary() );

}


} // end namespace otb
#endif
