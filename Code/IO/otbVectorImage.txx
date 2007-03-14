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

template <class TPixel, unsigned int VImageDimension, unsigned int foo> 
VectorImage<TPixel,VImageDimension, foo>::VectorImage()
{
}

template <class TPixel, unsigned int VImageDimension, unsigned int foo> 
std::string VectorImage<TPixel, VImageDimension, foo>::GetProjectionRef( void )
{
   return ( ImageBase::GetProjectionRef( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension, unsigned int foo> 
std::string VectorImage<TPixel, VImageDimension, foo>::GetGCPProjection( void )
{
   return ( ImageBase::GetGCPProjection( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension, unsigned int foo>   
unsigned int VectorImage<TPixel, VImageDimension, foo>::GetGCPCount( void  ) 
{
   return ( ImageBase::GetGCPCount( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension, unsigned int foo>   
OTB_GCP & VectorImage<TPixel, VImageDimension, foo>::GetGCPs ( unsigned int GCPnum )
{
   return ( this->ImageBase::GetGCPs( this->GetMetaDataDictionary(), GCPnum ) );	
}
    
template <class TPixel, unsigned int VImageDimension, unsigned int foo>   
std::string VectorImage<TPixel, VImageDimension, foo>::GetGCPId( unsigned int GCPnum  ) 
{
   return ( ImageBase::GetGCPId( this->GetMetaDataDictionary(), GCPnum ) );	
}
  
template <class TPixel, unsigned int VImageDimension, unsigned int foo>   
std::string VectorImage<TPixel, VImageDimension, foo>::GetGCPInfo( unsigned int GCPnum )
{
   return ( ImageBase::GetGCPInfo( this->GetMetaDataDictionary(), GCPnum ) );	
}
  
template <class TPixel, unsigned int VImageDimension, unsigned int foo>   
double VectorImage<TPixel, VImageDimension, foo>::GetGCPRow( unsigned int GCPnum ) 
{
   return ( ImageBase::GetGCPRow( this->GetMetaDataDictionary(), GCPnum ) );	
}
  
template <class TPixel, unsigned int VImageDimension, unsigned int foo>   
double VectorImage<TPixel, VImageDimension, foo>::GetGCPCol( unsigned int GCPnum )	
{
   return ( ImageBase::GetGCPCol( this->GetMetaDataDictionary(), GCPnum ) );	
}
  
template <class TPixel, unsigned int VImageDimension, unsigned int foo>   
double VectorImage<TPixel, VImageDimension, foo>::GetGCPX( unsigned int GCPnum ) 
{	
   return ( ImageBase::GetGCPX( this->GetMetaDataDictionary(), GCPnum ) );	
}
 
template <class TPixel, unsigned int VImageDimension, unsigned int foo>   
double VectorImage<TPixel, VImageDimension, foo>::GetGCPY( unsigned int GCPnum ) 
{
   return ( ImageBase::GetGCPY( this->GetMetaDataDictionary(), GCPnum ) );	
}
  
template <class TPixel, unsigned int VImageDimension, unsigned int foo>   
double VectorImage<TPixel, VImageDimension, foo>::GetGCPZ( unsigned int GCPnum ) 
{
   return ( ImageBase::GetGCPZ( this->GetMetaDataDictionary(), GCPnum ) );	
}

template <class TPixel, unsigned int VImageDimension, unsigned int foo>   
ImageBase::VectorType VectorImage<TPixel, VImageDimension, foo>::GetGeoTransform( void ) 
{
   return ( ImageBase::GetGeoTransform( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension, unsigned int foo>   
ImageBase::VectorType VectorImage<TPixel, VImageDimension, foo>::GetUpperLeftCorner( void )
{
   return ( ImageBase::GetUpperLeftCorner( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension, unsigned int foo>   
ImageBase::VectorType VectorImage<TPixel, VImageDimension, foo>::GetUpperRightCorner( void )
{
   return ( ImageBase::GetUpperRightCorner( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension, unsigned int foo>   
ImageBase::VectorType VectorImage<TPixel, VImageDimension, foo>::GetLowerLeftCorner( void )
{
   return ( ImageBase::GetLowerLeftCorner( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension, unsigned int foo>   
ImageBase::VectorType VectorImage<TPixel, VImageDimension, foo>::GetLowerRightCorner( void )
{
   return ( ImageBase::GetLowerRightCorner( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension, unsigned int foo> 
void 
VectorImage<TPixel, VImageDimension,  foo>
::CopyInformation(const itk::DataObject * data)
{
  Superclass::CopyInformation(data);
  this->itk::Object::SetMetaDataDictionary(data->GetMetaDataDictionary());
}

template <class TPixel, unsigned int VImageDimension, unsigned int foo> 
void 
VectorImage<TPixel, VImageDimension, foo>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  
  this->ImageBase::PrintSelf( os, indent, this->GetMetaDataDictionary() );
  
}


} // end namespace otb
#endif
