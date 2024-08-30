/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbImageMetadataInterfaceBase_h
#define otbImageMetadataInterfaceBase_h

#include <string>

#include "itkMetaDataDictionary.h"
#include "otbMetaDataKey.h"
#include "itkImageBase.h"
#include "otbImageMetadata.h"
#include "otbMetadataSupplierInterface.h"

#include "OTBMetadataExport.h"

namespace otb
{

/** \class ImageMetadataInterfaceBase
 *
 * \brief Base class for captor metadata reading.
 *
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT ImageMetadataInterfaceBase : public itk::Object
{
public:
  typedef ImageMetadataInterfaceBase    Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageMetadataInterfaceBase, itk::Object);

  typedef itk::ImageBase<2>                     ImageType;
  typedef itk::MetaDataDictionary               MetaDataDictionaryType;
  typedef MetaDataKey::VectorType               VectorType;
  typedef MetaDataKey::VariableLengthVectorType VariableLengthVectorType;
  typedef unsigned int                          UnsignedIntType;
  typedef std::vector<std::string>              StringVectorType;
  typedef std::vector<unsigned int>             UIntVectorType;


  /** Set the image used to get the metadata */
  void SetImage(ImageType* image);

  /** Set the MetadataDictionary  */
  void SetMetaDataDictionary(const MetaDataDictionaryType& dict);

  /** Get the MetadataDictionary  */
  const MetaDataDictionaryType& GetMetaDataDictionary() const;

  /** Set the MetadataSupplierInterface */
  void SetMetadataSupplierInterface(const MetadataSupplierInterface&);

  /** Get the projection coordinate system of the image. */
  std::string GetProjectionRef() const;

  /** Get the GCP projection coordinates of the image. */
  std::string GetGCPProjection() const;
  //  otbMetadataGetMacro(GCPProjection, std::string);

  UnsignedIntType GetGCPCount() const;
  //  otbMetadataGetMacro(GCPCount, unsigned int);

  GCP& GetGCPs(unsigned int GCPnum);
  // otbMetadataGetGCPnumMacro(GCPs, GCP&, GCPnum, unsigned int);

  std::string GetGCPId(unsigned int GCPnum) const;
  //  otbMetadataGetGCPnumMacro(GCPId, std::string, GCPnum, unsigned int);

  std::string GetGCPInfo(unsigned int GCPnum) const;
  //  otbMetadataGetGCPnumMacro(GCPInfo, std::string, GCPnum, unsigned int);

  double GetGCPRow(unsigned int GCPnum) const;
  //  otbMetadataGetGCPnumMacro(GCPRow, double, GCPnum, unsigned int);

  double GetGCPCol(unsigned int GCPnum) const;
  //  otbMetadataGetGCPnumMacro(GCPCol, double, GCPnum, unsigned int);

  double GetGCPX(unsigned int GCPnum) const;
  //  otbMetadataGetGCPnumMacro(GCPX, double, GCPnum, unsigned int);

  double GetGCPY(unsigned int GCPnum) const;
  //  otbMetadataGetGCPnumMacro(GCPY, double, GCPnum, unsigned int);

  double GetGCPZ(unsigned int GCPnum) const;
  //  otbMetadataGetGCPnumMacro(GCPZ, double, GCPnum, unsigned int);

  /** Get the six coefficients of affine geoTtransform. */

  VectorType GetGeoTransform() const;
  //  otbMetadataGetMacro(GeoTransform, VectorType);

  /** Get image corners. */

  VectorType GetUpperLeftCorner() const;
  //  otbMetadataGetMacro(UpperLeftCorner, VectorType);

  VectorType GetUpperRightCorner() const;
  //  otbMetadataGetMacro(UpperRightCorner, VectorType);

  VectorType GetLowerLeftCorner() const;
  //  otbMetadataGetMacro(LowerLeftCorner, VectorType);

  VectorType GetLowerRightCorner() const;
  //  otbMetadataGetMacro(LowerRightCorner, VectorType);

  /** Parses the metadata using the MetadataSupplierInterface and fill the ImageMetadata object
   * Raises "MissingMetadataException" if parsing fails.
   */
  virtual void Parse(ImageMetadata &)
  {
    otbGenericExceptionMacro(MissingMetadataException,<<"Metadata parsing not implemented")
  }

  const std::string& Fetch(MDStr key, ImageMetadata & imd, const char *path, int band=-1);
  bool CheckFetch(MDStr key, ImageMetadata & imd, const char *path, int band=-1);

  const double& Fetch(MDNum key, ImageMetadata & imd, const char *path, int band=-1);
  bool CheckFetch(MDNum key, ImageMetadata & imd, const char *path, int band=-1);

  const MetaData::TimePoint& Fetch(MDTime key, ImageMetadata & imd, const char *path, int band=-1);
  bool CheckFetch(MDTime key, ImageMetadata & imd, const char *path, int band=-1);

  const std::string& Fetch(std::string key, ImageMetadata & imd, const char *path, int band=-1);
  bool CheckFetch(std::string key, ImageMetadata & imd, const char *path, int band=-1);

  const boost::any& FetchRPC(ImageMetadata& imd, const double lineOffset = 0.0, const double sampleOffset = 0.0);

protected:
  ImageMetadataInterfaceBase() = default;
  ~ImageMetadataInterfaceBase() override = default;

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  MetaDataDictionaryType m_MetaDataDictionary;

  const MetadataSupplierInterface * m_MetadataSupplierInterface = nullptr;

private:
  ImageMetadataInterfaceBase(const Self&) = delete;
  void operator=(const Self&) = delete;

  GCP m_GCP;
};

} // end namespace otb

#endif
