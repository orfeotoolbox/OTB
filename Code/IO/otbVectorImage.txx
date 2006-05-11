/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - C.Ruffel
  Language  :   C++
  Date      :   09 mai 2006
  Version   :   
  Role      :   Création d'une otb::VectorImage
  $Id:  $

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
   return ( this->ImageBase::GetProjectionRef( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension> 
std::string VectorImage<TPixel, VImageDimension>::GetGCPProjection( void )
{
   return ( this->ImageBase::GetGCPProjection( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension>   
unsigned int VectorImage<TPixel, VImageDimension>::GetGCPCount( void  ) 
{
   return ( this->ImageBase::GetGCPCount( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension>   
OTB_GCP & VectorImage<TPixel, VImageDimension>::GetGCPs ( unsigned int GCPnum )
{
   return ( this->ImageBase::GetGCPs( this->GetMetaDataDictionary(), GCPnum ) );	
}
    
template <class TPixel, unsigned int VImageDimension>   
std::string VectorImage<TPixel, VImageDimension>::GetGCPId( unsigned int GCPnum  ) 
{
   return ( this->ImageBase::GetGCPId( this->GetMetaDataDictionary(), GCPnum ) );	
}
  
template <class TPixel, unsigned int VImageDimension>   
std::string VectorImage<TPixel, VImageDimension>::GetGCPInfo( unsigned int GCPnum )
{
   return ( this->ImageBase::GetGCPInfo( this->GetMetaDataDictionary(), GCPnum ) );	
}
  
template <class TPixel, unsigned int VImageDimension>   
double VectorImage<TPixel, VImageDimension>::GetGCPRow( unsigned int GCPnum ) 
{
   return ( this->ImageBase::GetGCPRow( this->GetMetaDataDictionary(), GCPnum ) );	
}
  
template <class TPixel, unsigned int VImageDimension>   
double VectorImage<TPixel, VImageDimension>::GetGCPCol( unsigned int GCPnum )	
{
   return ( this->ImageBase::GetGCPCol( this->GetMetaDataDictionary(), GCPnum ) );	
}
  
template <class TPixel, unsigned int VImageDimension>   
double VectorImage<TPixel, VImageDimension>::GetGCPX( unsigned int GCPnum ) 
{	
   return ( this->ImageBase::GetGCPX( this->GetMetaDataDictionary(), GCPnum ) );	
}
 
template <class TPixel, unsigned int VImageDimension>   
double VectorImage<TPixel, VImageDimension>::GetGCPY( unsigned int GCPnum ) 
{
   return ( this->ImageBase::GetGCPY( this->GetMetaDataDictionary(), GCPnum ) );	
}
  
template <class TPixel, unsigned int VImageDimension>   
double VectorImage<TPixel, VImageDimension>::GetGCPZ( unsigned int GCPnum ) 
{
   return ( this->ImageBase::GetGCPZ( this->GetMetaDataDictionary(), GCPnum ) );	
}

template <class TPixel, unsigned int VImageDimension>   
ImageBase::VectorType VectorImage<TPixel, VImageDimension>::GetGeoTransform( void ) 
{
   return ( this->ImageBase::GetGeoTransform( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension>   
ImageBase::VectorType VectorImage<TPixel, VImageDimension>::GetUpperLeftCorner( void )
{
   return ( this->ImageBase::GetUpperLeftCorner( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension>   
ImageBase::VectorType VectorImage<TPixel, VImageDimension>::GetUpperRightCorner( void )
{
   return ( this->ImageBase::GetUpperRightCorner( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension>   
ImageBase::VectorType VectorImage<TPixel, VImageDimension>::GetLowerLeftCorner( void )
{
   return ( this->ImageBase::GetLowerLeftCorner( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension>   
ImageBase::VectorType VectorImage<TPixel, VImageDimension>::GetLowerRightCorner( void )
{
   return ( this->ImageBase::GetLowerRightCorner( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension> 
void 
VectorImage<TPixel, VImageDimension>::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  
  this->ImageBase::PrintSelf( os, indent, this->GetMetaDataDictionary() );
  
}


} // end namespace otb
#endif
