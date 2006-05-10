/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - C.Ruffel
  Language  :   C++
  Date      :   21 avril 2006
  Version   :   
  Role      :   Création d'une otb::ImageBase
  $Id$

=========================================================================*/
#ifndef __otbImageBase_h
#define __otbImageBase_h

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkImageBase.h"
#include "itkIndent.h"

#include "otbMetaDataKey.h"
#include <gdal.h>

#include <iostream>
#include <string>

namespace otb
{
/** \class ImageBase
 *
 * \brief Creation of an "otb" ImageBase that gets metadata.
 *
 */
class ITK_EXPORT ImageBase : public MetaDataKey
{
public:

  typedef ImageBase Self;

  typedef itk::MetaDataDictionary 	MetaDataDictionaryType;  
  
  typedef MetaDataKey::VectorType	VectorType;
  
  typedef itk::Indent			Indent;	

  /** Get the projection coordinate system of the image. */
  
  std::string GetProjectionRef( MetaDataDictionaryType & dict );
  virtual std::string GetProjectionRef( void ) = 0;
  
  /** Get the GCP projection coordinates of the image. */
  
  std::string GetGCPProjection( MetaDataDictionaryType & dict );
  virtual std::string GetGCPProjection( void ) = 0;

  unsigned int GetGCPCount( MetaDataDictionaryType & dict );
  virtual unsigned int GetGCPCount( void ) = 0;

  OTB_GCP & GetGCPs(MetaDataDictionaryType & dict, unsigned int GCPnum );
  virtual OTB_GCP & GetGCPs( unsigned int GCPnum ) = 0;
    
  std::string GetGCPId( MetaDataDictionaryType & dict, unsigned int GCPnum );
  virtual std::string GetGCPId( unsigned int GCPnum ) = 0;
  
  std::string GetGCPInfo( MetaDataDictionaryType & dict, unsigned int GCPnum );
  virtual std::string GetGCPInfo( unsigned int GCPnum ) = 0;
  
  double GetGCPRow( MetaDataDictionaryType & dict, unsigned int GCPnum );
  virtual double GetGCPRow( unsigned int GCPnum ) = 0;
  
  double GetGCPCol( MetaDataDictionaryType & dict, unsigned int GCPnum );
  virtual double GetGCPCol( unsigned int GCPnum ) = 0;
  
  double GetGCPX( MetaDataDictionaryType & dict, unsigned int GCPnum );
  virtual double GetGCPX( unsigned int GCPnum ) = 0;
  
  double GetGCPY( MetaDataDictionaryType & dict, unsigned int GCPnum );
  virtual double GetGCPY( unsigned int GCPnum ) = 0;
  
  double GetGCPZ( MetaDataDictionaryType & dict, unsigned int GCPnum );
  virtual double GetGCPZ( unsigned int GCPnum ) = 0;
  
  /** Get the six coefficients of affine geoTtransform. */
   
  VectorType GetGeoTransform( MetaDataDictionaryType & dict );
  virtual VectorType GetGeoTransform( void ) = 0;
 
  /** Get image corners. */  
  
  VectorType GetUpperLeftCorner( MetaDataDictionaryType & dict );
  virtual VectorType GetUpperLeftCorner() = 0; 
  
  VectorType GetUpperRightCorner( MetaDataDictionaryType & dict );
  virtual VectorType GetUpperRightCorner() = 0;      
  
  VectorType GetLowerLeftCorner( MetaDataDictionaryType & dict );
  virtual VectorType GetLowerLeftCorner() = 0; 
  
  VectorType GetLowerRightCorner( MetaDataDictionaryType & dict );
  virtual VectorType GetLowerRightCorner() = 0; 
  
  void PrintSelf(std::ostream& os, Indent indent, const MetaDataDictionaryType & dict) const;
                 

protected:
  ImageBase();
  virtual ~ImageBase(){};
    
private:
  ImageBase(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
  OTB_GCP m_GCP;
  
};
  
  
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageBase.txx"
#endif

#endif
