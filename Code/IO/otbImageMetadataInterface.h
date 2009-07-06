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
#ifndef __otbImageMetadataInterface_h
#define __otbImageMetadataInterface_h

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkMetaDataDictionary.h"

#include "otbMetaDataKey.h"
#include "otbImageKeywordlist.h"
#include "itkImageBase.h"

#include <string>


/** */
#define otbMetadataGetMacro(name,type) \
  type Get##name () const \
  { \
    if (m_Image.IsNull()) \
    { \
      itkExceptionMacro(<<"Invalid Image, can't get the dictionary"); \
    } \
    return Get##name(m_Image->GetMetaDataDictionary()); \
  }

namespace otb
{
/** \class ImageMetadataInterface
 *
 * \brief Creation of an "otb" ImageMetadataInterface that gets metadata.
 *
 */
class ITK_EXPORT ImageMetadataInterface : public itk::Object
{
public:

  typedef ImageMetadataInterface Self;
  typedef itk::Object Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageMetadataInterface, itk::Object);

  typedef itk::ImageBase< 2 > ImageType;

  typedef itk::MetaDataDictionary   MetaDataDictionaryType;

  typedef MetaDataKey::VectorType  VectorType;
  typedef MetaDataKey::VariableLengthVectorType  VariableLengthVectorType;

  typedef ImageKeywordlist              ImageKeywordlistType;

  /** Set the image used to get the metadata */
  itkSetObjectMacro(Image,ImageType);

  /** Get the projection coordinate system of the image. */
  std::string GetProjectionRef( const MetaDataDictionaryType & dict ) const;
  otbMetadataGetMacro(ProjectionRef, std::string);

  /** Get the GCP projection coordinates of the image. */
  std::string GetGCPProjection( const MetaDataDictionaryType & dict ) const;
  otbMetadataGetMacro(GCPProjection, std::string);


  unsigned int GetGCPCount( const MetaDataDictionaryType & dict ) const;
  otbMetadataGetMacro(GCPCount, unsigned int);

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
  otbMetadataGetMacro(GeoTransform, VectorType);

  /** Get image corners. */

  VectorType GetUpperLeftCorner( const MetaDataDictionaryType & dict ) const;
  otbMetadataGetMacro(UpperLeftCorner, VectorType);

  VectorType GetUpperRightCorner( const MetaDataDictionaryType & dict ) const;
  otbMetadataGetMacro(UpperRightCorner, VectorType);

  VectorType GetLowerLeftCorner( const MetaDataDictionaryType & dict ) const;
  otbMetadataGetMacro(LowerLeftCorner, VectorType);

  VectorType GetLowerRightCorner( const MetaDataDictionaryType & dict ) const;
  otbMetadataGetMacro(LowerRightCorner, VectorType);

  /** Get the ImageKeywordlist */
  ImageKeywordlistType GetImageKeywordlist( MetaDataDictionaryType & dict );
  const ImageKeywordlistType GetImageKeywordlist(const MetaDataDictionaryType & dict ) const;

  /** Get the radiometric bias from the ossim metadata */
  VariableLengthVectorType GetPhysicalBias( const MetaDataDictionaryType & dict ) const;
  otbMetadataGetMacro(PhysicalBias, VariableLengthVectorType);

  /** Get the radiometric gain from the ossim metadata */
  VariableLengthVectorType GetPhysicalGain( const MetaDataDictionaryType & dict ) const;
  otbMetadataGetMacro(PhysicalGain, VariableLengthVectorType);

  /** Get the solar irradiance from the ossim metadata */
  VariableLengthVectorType GetSolarIrradiance( const MetaDataDictionaryType & dict ) const;
  otbMetadataGetMacro(SolarIrradiance, VariableLengthVectorType);

  /** Get the sun elevation from the ossim metadata */
  double GetSunElevation( const MetaDataDictionaryType & dict ) const;
  otbMetadataGetMacro(SunElevation, double);

  /** Get the sun azimuth from the ossim metadata */
  double GetSunAzimuth( const MetaDataDictionaryType & dict ) const;
  otbMetadataGetMacro(SunAzimuth, double);

  /** Get the imaging day from the ossim metadata */
  int GetDay( const MetaDataDictionaryType & dict ) const;
  otbMetadataGetMacro(Day, int);

  /** Get the imaging month from the ossim metadata */
  int GetMonth( const MetaDataDictionaryType & dict ) const;
  otbMetadataGetMacro(Month, int);

  /** Get the imaging month from the ossim metadata */
  int GetYear( const MetaDataDictionaryType & dict ) const;
  otbMetadataGetMacro(Year, int);

  /** Get the sensor ID from the ossim metadata */
  std::string GetSensorID(const MetaDataDictionaryType & dict ) const;
  otbMetadataGetMacro(SensorID, std::string);

  /** Get the number of bands from the ossim metadata */
  unsigned int GetNumberOfBands(const MetaDataDictionaryType & dict ) const;
  otbMetadataGetMacro(NumberOfBands, unsigned int);

  /** Get the band name from the ossim metadata */
  std::vector<std::string> GetBandName(const MetaDataDictionaryType & dict ) const;
  otbMetadataGetMacro(BandName, std::vector<std::string>);

  void PrintSelf(std::ostream& os, itk::Indent indent, const MetaDataDictionaryType & dict) const;

protected:
  ImageMetadataInterface();
  virtual ~ImageMetadataInterface() {};

  bool IsSpot( const MetaDataDictionaryType & dict) const;

  bool IsIkonos( const MetaDataDictionaryType & dict) const;

  bool IsQuickbird( const MetaDataDictionaryType & dict) const;

  VariableLengthVectorType GetSpotPhysicalBias( const MetaDataDictionaryType & dict ) const;
  VariableLengthVectorType GetSpotPhysicalGain( const MetaDataDictionaryType & dict ) const;

  VariableLengthVectorType GetIkonosPhysicalBias( const MetaDataDictionaryType & dict ) const;
  VariableLengthVectorType GetIkonosPhysicalGain( const MetaDataDictionaryType & dict ) const;

  VariableLengthVectorType GetQuickbirdPhysicalBias( const MetaDataDictionaryType & dict ) const;
  VariableLengthVectorType GetQuickbirdPhysicalGain( const MetaDataDictionaryType & dict ) const;

private:

  ImageMetadataInterface(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  OTB_GCP m_GCP;
  ImageType::Pointer m_Image;
};


} // end namespace otb

#endif
