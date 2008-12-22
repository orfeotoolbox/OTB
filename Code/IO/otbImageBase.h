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

  typedef itk::MetaDataDictionary 	MetaDataDictionaryType;

  typedef MetaDataKey::VectorType	VectorType;

  typedef ImageKeywordlist              ImageKeywordlistType;

  /** Get the projection coordinate system of the image. */

  static std::string GetProjectionRef( MetaDataDictionaryType & dict );
  virtual std::string GetProjectionRef( void ) = 0;

  /** Get the GCP projection coordinates of the image. */

  static std::string GetGCPProjection( MetaDataDictionaryType & dict );
  virtual std::string GetGCPProjection( void ) = 0;

  static unsigned int GetGCPCount( MetaDataDictionaryType & dict );
  virtual unsigned int GetGCPCount( void ) = 0;

  OTB_GCP & GetGCPs(MetaDataDictionaryType & dict, unsigned int GCPnum );
  virtual OTB_GCP & GetGCPs( unsigned int GCPnum ) = 0;

  static std::string GetGCPId( MetaDataDictionaryType & dict, unsigned int GCPnum );
  virtual std::string GetGCPId( unsigned int GCPnum ) = 0;

  static std::string GetGCPInfo( MetaDataDictionaryType & dict, unsigned int GCPnum );
  virtual std::string GetGCPInfo( unsigned int GCPnum ) = 0;

  static double GetGCPRow( MetaDataDictionaryType & dict, unsigned int GCPnum );
  virtual double GetGCPRow( unsigned int GCPnum ) = 0;

  static double GetGCPCol( MetaDataDictionaryType & dict, unsigned int GCPnum );
  virtual double GetGCPCol( unsigned int GCPnum ) = 0;

  static double GetGCPX( MetaDataDictionaryType & dict, unsigned int GCPnum );
  virtual double GetGCPX( unsigned int GCPnum ) = 0;

  static double GetGCPY( MetaDataDictionaryType & dict, unsigned int GCPnum );
  virtual double GetGCPY( unsigned int GCPnum ) = 0;

  static double GetGCPZ( MetaDataDictionaryType & dict, unsigned int GCPnum );
  virtual double GetGCPZ( unsigned int GCPnum ) = 0;

  /** Get the six coefficients of affine geoTtransform. */

  static VectorType GetGeoTransform( MetaDataDictionaryType & dict );
  virtual VectorType GetGeoTransform( void ) = 0;

  /** Get image corners. */

  static VectorType GetUpperLeftCorner( MetaDataDictionaryType & dict );
  virtual VectorType GetUpperLeftCorner() = 0;

  static VectorType GetUpperRightCorner( MetaDataDictionaryType & dict );
  virtual VectorType GetUpperRightCorner() = 0;

  static VectorType GetLowerLeftCorner( MetaDataDictionaryType & dict );
  virtual VectorType GetLowerLeftCorner() = 0;

  static VectorType GetLowerRightCorner( MetaDataDictionaryType & dict );
  virtual VectorType GetLowerRightCorner() = 0;

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
