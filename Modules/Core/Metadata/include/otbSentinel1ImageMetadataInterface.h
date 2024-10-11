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

#ifndef otbSentinel1ImageMetadataInterface_h
#define otbSentinel1ImageMetadataInterface_h

#include "otbSarImageMetadataInterface.h"
#include "otbXMLMetadataSupplier.h"

namespace otb
{
/** \class Sentinel1ImageMetadataInterface
 *
 * \brief Creation of an "otb" Sentinel1ImageMetadataInterface that gets metadata.
 *
 *
 * \ingroup OTBMetadata
 */

class OTBMetadata_EXPORT Sentinel1ImageMetadataInterface : public SarImageMetadataInterface
{
public:
  typedef Sentinel1ImageMetadataInterface Self;
  typedef SarImageMetadataInterface       Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);


  /** Run-time type information (and related methods). */
  itkTypeMacro(Sentinel1ImageMetadataInterface, SarImageMetadataInterface);

  typedef Superclass::ImageType                ImageType;
  typedef Superclass::MetaDataDictionaryType   MetaDataDictionaryType;
  typedef Superclass::VectorType               VectorType;
  typedef Superclass::VariableLengthVectorType VariableLengthVectorType;
  typedef Superclass::RealType                 RealType;
  typedef Superclass::LookupDataPointerType    LookupDataPointerType;

  double GetCenterIncidenceAngle(const MetadataSupplierInterface&) const override;

  /*get lookup data for calculating backscatter */
  bool HasCalibrationLookupDataFlag(const MetadataSupplierInterface&) const override;
  bool CreateCalibrationLookupData(SARCalib&, const ImageMetadata&, const MetadataSupplierInterface&, const bool) const override;

  /* Read the Sar parameters and GCPs from the annotation xml file, this method can be used 
    to instantiate a otb::SarModel */
  void ReadSarParamAndGCPs(const XMLMetadataSupplier &,
                           SARParam &,
                           Projection::GCPParam &);

  void ParseGdal(ImageMetadata &) override;

  void ParseGeom(ImageMetadata &) override;

  void Parse(ImageMetadata &) override;

protected:
  /* class ctor */
  Sentinel1ImageMetadataInterface() = default;

  /* class dtor */
  ~Sentinel1ImageMetadataInterface() override = default;

  /* Fetch the AzimuthFmRate metadata */
  std::vector<AzimuthFmRate> GetAzimuthFmRate(const XMLMetadataSupplier&) const;

  /* Fetch the DopplerCentroid metadata */
  std::vector<DopplerCentroid> GetDopplerCentroid(const XMLMetadataSupplier&) const;

  /* Fetch the Orbits metadata */
  std::vector<Orbit> GetOrbits(const XMLMetadataSupplier&) const;

  /* Fetch the burst records */
  std::vector<BurstRecord> GetBurstRecords(const XMLMetadataSupplier&, const MetaData::Duration & azimuthTimeInterval) const;

  /* Fetch coordinate conversion records (Sr0/Gr0) */
  std::vector<CoordinateConversionRecord> GetCoordinateConversionRecord(const XMLMetadataSupplier &xmlMS,
                                                                        const std::string & rg0_path,
                                                                        const std::string & coeffs_path) const;

  /* Compute the mean terrain elevation */
  double getBandTerrainHeight(const XMLMetadataSupplier&) const;

  /* create the thermal denoising LUT */
  bool CreateThermalNoiseLookupData(SARCalib& sarCalib,
                                                                    const ImageMetadata& imd,
                                                                    const MetadataSupplierInterface& mds,
                                                                    const bool geom) const;

private:
  Sentinel1ImageMetadataInterface(const Self&) = delete;
  void operator=(const Self&) = delete;
};




} // end namespace otb

#endif
