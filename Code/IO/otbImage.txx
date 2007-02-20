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

template <class TPixel, unsigned int VImageDimension,  unsigned int foo> 
Image<TPixel,VImageDimension,  foo>::Image()
{
}

template <class TPixel, unsigned int VImageDimension,  unsigned int foo> 
std::string Image<TPixel, VImageDimension,  foo>::GetProjectionRef( void )
{
   return ( this->ImageBase::GetProjectionRef( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension,  unsigned int foo> 
std::string Image<TPixel, VImageDimension,  foo>::GetGCPProjection( void )
{
   return ( this->ImageBase::GetGCPProjection( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension,  unsigned int foo>   
unsigned int Image<TPixel, VImageDimension,  foo>::GetGCPCount( void  ) 
{
   return ( this->ImageBase::GetGCPCount( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension,  unsigned int foo>   
OTB_GCP & Image<TPixel, VImageDimension,  foo>::GetGCPs ( unsigned int GCPnum )
{
   return ( this->ImageBase::GetGCPs( this->GetMetaDataDictionary(), GCPnum ) );	
}
    
template <class TPixel, unsigned int VImageDimension,  unsigned int foo>   
std::string Image<TPixel, VImageDimension,  foo>::GetGCPId( unsigned int GCPnum  ) 
{
   return ( this->ImageBase::GetGCPId( this->GetMetaDataDictionary(), GCPnum ) );	
}
  
template <class TPixel, unsigned int VImageDimension,  unsigned int foo>   
std::string Image<TPixel, VImageDimension,  foo>::GetGCPInfo( unsigned int GCPnum )
{
   return ( this->ImageBase::GetGCPInfo( this->GetMetaDataDictionary(), GCPnum ) );	
}
  
template <class TPixel, unsigned int VImageDimension,  unsigned int foo>   
double Image<TPixel, VImageDimension,  foo>::GetGCPRow( unsigned int GCPnum ) 
{
   return ( this->ImageBase::GetGCPRow( this->GetMetaDataDictionary(), GCPnum ) );	
}
  
template <class TPixel, unsigned int VImageDimension,  unsigned int foo>   
double Image<TPixel, VImageDimension,  foo>::GetGCPCol( unsigned int GCPnum )	
{
   return ( this->ImageBase::GetGCPCol( this->GetMetaDataDictionary(), GCPnum ) );	
}
  
template <class TPixel, unsigned int VImageDimension,  unsigned int foo>   
double Image<TPixel, VImageDimension,  foo>::GetGCPX( unsigned int GCPnum ) 
{	
   return ( this->ImageBase::GetGCPX( this->GetMetaDataDictionary(), GCPnum ) );	
}
 
template <class TPixel, unsigned int VImageDimension,  unsigned int foo>   
double Image<TPixel, VImageDimension,  foo>::GetGCPY( unsigned int GCPnum ) 
{
   return ( this->ImageBase::GetGCPY( this->GetMetaDataDictionary(), GCPnum ) );	
}
  
template <class TPixel, unsigned int VImageDimension,  unsigned int foo>   
double Image<TPixel, VImageDimension,  foo>::GetGCPZ( unsigned int GCPnum ) 
{
   return ( this->ImageBase::GetGCPZ( this->GetMetaDataDictionary(), GCPnum ) );	
}

template <class TPixel, unsigned int VImageDimension,  unsigned int foo>   
ImageBase::VectorType Image<TPixel, VImageDimension,  foo>::GetGeoTransform( void ) 
{
   return ( this->ImageBase::GetGeoTransform( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension,  unsigned int foo>   
ImageBase::VectorType Image<TPixel, VImageDimension,  foo>::GetUpperLeftCorner( void )
{
   return ( this->ImageBase::GetUpperLeftCorner( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension,  unsigned int foo>   
ImageBase::VectorType Image<TPixel, VImageDimension,  foo>::GetUpperRightCorner( void )
{
   return ( this->ImageBase::GetUpperRightCorner( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension,  unsigned int foo>   
ImageBase::VectorType Image<TPixel, VImageDimension,  foo>::GetLowerLeftCorner( void )
{
   return ( this->ImageBase::GetLowerLeftCorner( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension,  unsigned int foo>   
ImageBase::VectorType Image<TPixel, VImageDimension,  foo>::GetLowerRightCorner( void )
{
   return ( this->ImageBase::GetLowerRightCorner( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension, unsigned int foo> 
void 
Image<TPixel, VImageDimension,  foo>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  
  this->ImageBase::PrintSelf( os, indent, this->GetMetaDataDictionary() );
  
}


} // end namespace otb
#endif
