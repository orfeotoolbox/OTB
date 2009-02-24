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
#ifndef __otbImageMetadata_h
#define __otbImageMetadata_h

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkMetaDataDictionary.h"

#include "otbMetaDataKey.h"
#include "otbImageKeywordlist.h"

#include <string>

namespace otb
{
/** \class ImageMetadata
 *
 * \brief Creation of an "otb" ImageMetadata that gets metadata.
 *
 */
class ITK_EXPORT ImageMetadata : public itk::Object
{
public:

  typedef ImageMetadata Self;
  typedef itk::Object Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageMetadata, itk::Object);


  typedef itk::MetaDataDictionary   MetaDataDictionaryType;

  typedef MetaDataKey::VectorType  VectorType;

  typedef ImageKeywordlist              ImageKeywordlistType;


  /** Get the projection coordinate system of the image. */
  std::string GetProjectionRef( const MetaDataDictionaryType & dict ) const;

  /** Get the GCP projection coordinates of the image. */
  std::string GetGCPProjection( const MetaDataDictionaryType & dict ) const;

  unsigned int GetGCPCount( const MetaDataDictionaryType & dict ) const;

  OTB_GCP & GetGCPs( MetaDataDictionaryType & dict, unsigned int GCPnum );

  std::string GetGCPId( const MetaDataDictionaryType & dict, unsigned int GCPnum ) const;

  std::string GetGCPInfo( const MetaDataDictionaryType & dict, unsigned int GCPnum ) const;

  double GetGCPRow( const MetaDataDictionaryType & dict, unsigned int GCPnum ) const;

  double GetGCPCol( const MetaDataDictionaryType & dict, unsigned int GCPnum ) const;

  double GetGCPX( const MetaDataDictionaryType & dict, unsigned int GCPnum ) const;

  double GetGCPY( const MetaDataDictionaryType & dict, unsigned int GCPnum ) const;

  double GetGCPZ( const MetaDataDictionaryType & dict, unsigned int GCPnum ) const;

  /** Get the six coefficients of affine geoTtransform. */

  VectorType GetGeoTransform( const MetaDataDictionaryType & dict ) const;

  /** Get image corners. */

  VectorType GetUpperLeftCorner( const MetaDataDictionaryType & dict ) const;

  VectorType GetUpperRightCorner( const MetaDataDictionaryType & dict ) const;

  VectorType GetLowerLeftCorner( const MetaDataDictionaryType & dict ) const;

  VectorType GetLowerRightCorner( const MetaDataDictionaryType & dict ) const;

  /** Get the ImageKeywordlist */
  ImageKeywordlistType GetImageKeywordlist( MetaDataDictionaryType & dict );
  const ImageKeywordlistType GetImageKeywordlist(const MetaDataDictionaryType & dict ) const;

  void PrintSelf(std::ostream& os, itk::Indent indent, const MetaDataDictionaryType & dict) const;


protected:
  ImageMetadata();
  virtual ~ImageMetadata() {};

private:
  ImageMetadata(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  OTB_GCP m_GCP;

};


} // end namespace otb

#endif
