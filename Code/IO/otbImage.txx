/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - C.Ruffel
  Language  :   C++
  Date      :   21 avril 2006
  Version   :   
  Role      :   Création d'une otb::Image
  $Id: $

=========================================================================*/


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
std::string Image<TPixel, VImageDimension>::GetProjectionRef( void )
{
   return ( this->ImageBase::GetProjectionRef( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension> 
std::string Image<TPixel, VImageDimension>::GetGCPProjection( void )
{
   return ( this->ImageBase::GetGCPProjection( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension>   
unsigned int Image<TPixel, VImageDimension>::GetGCPCount( void  ) 
{
   return ( this->ImageBase::GetGCPCount( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension>   
OTB_GCP & Image<TPixel, VImageDimension>::GetGCPs ( unsigned int GCPnum )
{
   return ( this->ImageBase::GetGCPs( this->GetMetaDataDictionary(), GCPnum ) );	
}
    
template <class TPixel, unsigned int VImageDimension>   
std::string Image<TPixel, VImageDimension>::GetGCPId( unsigned int GCPnum  ) 
{
   return ( this->ImageBase::GetGCPId( this->GetMetaDataDictionary(), GCPnum ) );	
}
  
template <class TPixel, unsigned int VImageDimension>   
std::string Image<TPixel, VImageDimension>::GetGCPInfo( unsigned int GCPnum )
{
   return ( this->ImageBase::GetGCPInfo( this->GetMetaDataDictionary(), GCPnum ) );	
}
  
template <class TPixel, unsigned int VImageDimension>   
double Image<TPixel, VImageDimension>::GetGCPRow( unsigned int GCPnum ) 
{
   return ( this->ImageBase::GetGCPRow( this->GetMetaDataDictionary(), GCPnum ) );	
}
  
template <class TPixel, unsigned int VImageDimension>   
double Image<TPixel, VImageDimension>::GetGCPCol( unsigned int GCPnum )	
{
   return ( this->ImageBase::GetGCPCol( this->GetMetaDataDictionary(), GCPnum ) );	
}
  
template <class TPixel, unsigned int VImageDimension>   
double Image<TPixel, VImageDimension>::GetGCPX( unsigned int GCPnum ) 
{	
   return ( this->ImageBase::GetGCPX( this->GetMetaDataDictionary(), GCPnum ) );	
}
 
template <class TPixel, unsigned int VImageDimension>   
double Image<TPixel, VImageDimension>::GetGCPY( unsigned int GCPnum ) 
{
   return ( this->ImageBase::GetGCPY( this->GetMetaDataDictionary(), GCPnum ) );	
}
  
template <class TPixel, unsigned int VImageDimension>   
double Image<TPixel, VImageDimension>::GetGCPZ( unsigned int GCPnum ) 
{
   return ( this->ImageBase::GetGCPZ( this->GetMetaDataDictionary(), GCPnum ) );	
}

template <class TPixel, unsigned int VImageDimension>   
ImageBase::VectorType Image<TPixel, VImageDimension>::GetGeoTransform( void ) 
{
   return ( this->ImageBase::GetGeoTransform( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension>   
ImageBase::  VectorType Image<TPixel, VImageDimension>::GetUpperLeftCorner( void )
{
   return ( this->ImageBase::GetUpperLeftCorner( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension>   
ImageBase::  VectorType Image<TPixel, VImageDimension>::GetUpperRightCorner( void )
{
   return ( this->ImageBase::GetUpperRightCorner( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension>   
ImageBase::  VectorType Image<TPixel, VImageDimension>::GetLowerLeftCorner( void )
{
   return ( this->ImageBase::GetLowerLeftCorner( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension>   
ImageBase::  VectorType Image<TPixel, VImageDimension>::GetLowerRightCorner( void )
{
   return ( this->ImageBase::GetLowerRightCorner( this->GetMetaDataDictionary() ) );	
}

template <class TPixel, unsigned int VImageDimension> 
void 
Image<TPixel, VImageDimension>::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  
  this->ImageBase::PrintSelf( os, indent, this->GetMetaDataDictionary() );
  
}


} // end namespace otb

