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

template <class TPixel, unsigned int VImageDimension,  unsigned int toto> 
Image<TPixel,VImageDimension,  toto>::Image()
{
}

template <class TPixel, unsigned int VImageDimension,  unsigned int toto> 
std::string Image<TPixel, VImageDimension,  toto>::GetProjectionRef( void )
{
   return ( this->ImageBase::GetProjectionRef( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension,  unsigned int toto> 
std::string Image<TPixel, VImageDimension,  toto>::GetGCPProjection( void )
{
   return ( this->ImageBase::GetGCPProjection( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension,  unsigned int toto>   
unsigned int Image<TPixel, VImageDimension,  toto>::GetGCPCount( void  ) 
{
   return ( this->ImageBase::GetGCPCount( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension,  unsigned int toto>   
OTB_GCP & Image<TPixel, VImageDimension,  toto>::GetGCPs ( unsigned int GCPnum )
{
   return ( this->ImageBase::GetGCPs( this->GetMetaDataDictionary(), GCPnum ) );	
}
    
template <class TPixel, unsigned int VImageDimension,  unsigned int toto>   
std::string Image<TPixel, VImageDimension,  toto>::GetGCPId( unsigned int GCPnum  ) 
{
   return ( this->ImageBase::GetGCPId( this->GetMetaDataDictionary(), GCPnum ) );	
}
  
template <class TPixel, unsigned int VImageDimension,  unsigned int toto>   
std::string Image<TPixel, VImageDimension,  toto>::GetGCPInfo( unsigned int GCPnum )
{
   return ( this->ImageBase::GetGCPInfo( this->GetMetaDataDictionary(), GCPnum ) );	
}
  
template <class TPixel, unsigned int VImageDimension,  unsigned int toto>   
double Image<TPixel, VImageDimension,  toto>::GetGCPRow( unsigned int GCPnum ) 
{
   return ( this->ImageBase::GetGCPRow( this->GetMetaDataDictionary(), GCPnum ) );	
}
  
template <class TPixel, unsigned int VImageDimension,  unsigned int toto>   
double Image<TPixel, VImageDimension,  toto>::GetGCPCol( unsigned int GCPnum )	
{
   return ( this->ImageBase::GetGCPCol( this->GetMetaDataDictionary(), GCPnum ) );	
}
  
template <class TPixel, unsigned int VImageDimension,  unsigned int toto>   
double Image<TPixel, VImageDimension,  toto>::GetGCPX( unsigned int GCPnum ) 
{	
   return ( this->ImageBase::GetGCPX( this->GetMetaDataDictionary(), GCPnum ) );	
}
 
template <class TPixel, unsigned int VImageDimension,  unsigned int toto>   
double Image<TPixel, VImageDimension,  toto>::GetGCPY( unsigned int GCPnum ) 
{
   return ( this->ImageBase::GetGCPY( this->GetMetaDataDictionary(), GCPnum ) );	
}
  
template <class TPixel, unsigned int VImageDimension,  unsigned int toto>   
double Image<TPixel, VImageDimension,  toto>::GetGCPZ( unsigned int GCPnum ) 
{
   return ( this->ImageBase::GetGCPZ( this->GetMetaDataDictionary(), GCPnum ) );	
}

template <class TPixel, unsigned int VImageDimension,  unsigned int toto>   
ImageBase::VectorType Image<TPixel, VImageDimension,  toto>::GetGeoTransform( void ) 
{
   return ( this->ImageBase::GetGeoTransform( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension,  unsigned int toto>   
ImageBase::VectorType Image<TPixel, VImageDimension,  toto>::GetUpperLeftCorner( void )
{
   return ( this->ImageBase::GetUpperLeftCorner( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension,  unsigned int toto>   
ImageBase::VectorType Image<TPixel, VImageDimension,  toto>::GetUpperRightCorner( void )
{
   return ( this->ImageBase::GetUpperRightCorner( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension,  unsigned int toto>   
ImageBase::VectorType Image<TPixel, VImageDimension,  toto>::GetLowerLeftCorner( void )
{
   return ( this->ImageBase::GetLowerLeftCorner( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension,  unsigned int toto>   
ImageBase::VectorType Image<TPixel, VImageDimension,  toto>::GetLowerRightCorner( void )
{
   return ( this->ImageBase::GetLowerRightCorner( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension,  unsigned int toto> 
void 
Image<TPixel, VImageDimension,  toto>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  
  this->ImageBase::PrintSelf( os, indent, this->GetMetaDataDictionary() );
  
}


} // end namespace otb
#endif
