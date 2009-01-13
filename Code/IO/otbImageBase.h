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
#ifndef __otbImageBase_h
#define __otbImageBase_h

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkMetaDataDictionary.h"

#include "otbMetaDataKey.h"
#include "otbImageKeywordlist.h"

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

  typedef itk::MetaDataDictionary   MetaDataDictionaryType;

  typedef MetaDataKey::VectorType  VectorType;

  typedef ImageKeywordlist              ImageKeywordlistType;


  /** Get the projection coordinate system of the image. */

  std::string GetProjectionRef( const MetaDataDictionaryType & dict ) const;
  virtual std::string GetProjectionRef( void ) const = 0;

  /** Get the GCP projection coordinates of the image. */

  std::string GetGCPProjection( const MetaDataDictionaryType & dict ) const;
  virtual std::string GetGCPProjection( void ) const = 0;

  unsigned int GetGCPCount( const MetaDataDictionaryType & dict ) const;
  virtual unsigned int GetGCPCount( void ) const = 0;

  OTB_GCP & GetGCPs( MetaDataDictionaryType & dict, unsigned int GCPnum );
  virtual OTB_GCP & GetGCPs( unsigned int GCPnum ) = 0;

  std::string GetGCPId( const MetaDataDictionaryType & dict, unsigned int GCPnum ) const;
  virtual std::string GetGCPId( unsigned int GCPnum ) const = 0;

  std::string GetGCPInfo( const MetaDataDictionaryType & dict, unsigned int GCPnum ) const;
  virtual std::string GetGCPInfo( unsigned int GCPnum ) const = 0;

  double GetGCPRow( const MetaDataDictionaryType & dict, unsigned int GCPnum ) const;
  virtual double GetGCPRow( unsigned int GCPnum ) const = 0;

  double GetGCPCol( const MetaDataDictionaryType & dict, unsigned int GCPnum ) const;
  virtual double GetGCPCol( unsigned int GCPnum ) const = 0;

  double GetGCPX( const MetaDataDictionaryType & dict, unsigned int GCPnum ) const;
  virtual double GetGCPX( unsigned int GCPnum ) const = 0;

  double GetGCPY( const MetaDataDictionaryType & dict, unsigned int GCPnum ) const;
  virtual double GetGCPY( unsigned int GCPnum ) const = 0;

  double GetGCPZ( const MetaDataDictionaryType & dict, unsigned int GCPnum ) const;
  virtual double GetGCPZ( unsigned int GCPnum ) const = 0;

  /** Get the six coefficients of affine geoTtransform. */

  VectorType GetGeoTransform( const MetaDataDictionaryType & dict ) const;
  virtual VectorType GetGeoTransform( void ) const = 0;

  /** Get image corners. */

  VectorType GetUpperLeftCorner( const MetaDataDictionaryType & dict ) const;
  virtual VectorType GetUpperLeftCorner() const = 0;

  VectorType GetUpperRightCorner( const MetaDataDictionaryType & dict ) const;
  virtual VectorType GetUpperRightCorner() const = 0;

  VectorType GetLowerLeftCorner( const MetaDataDictionaryType & dict ) const;
  virtual VectorType GetLowerLeftCorner() const = 0;

  VectorType GetLowerRightCorner( const MetaDataDictionaryType & dict ) const;
  virtual VectorType GetLowerRightCorner() const = 0;

  /** Get the ImageKeywordlist */
  ImageKeywordlistType GetImageKeywordlist( MetaDataDictionaryType & dict );
  const ImageKeywordlistType GetImageKeywordlist(const MetaDataDictionaryType & dict ) const;

  virtual ImageKeywordlistType GetImageKeywordlist() = 0;
  virtual const ImageKeywordlistType GetImageKeywordlist(void) const = 0;

  void PrintSelf(std::ostream& os, itk::Indent indent, const MetaDataDictionaryType & dict) const;


protected:
  ImageBase();
  virtual ~ImageBase(){};

private:
  ImageBase(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  OTB_GCP m_GCP;

};


} // end namespace otb

#endif
