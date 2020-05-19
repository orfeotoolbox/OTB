/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbOpticalImageMetadataInterface_h
#define otbOpticalImageMetadataInterface_h

#include <string>

#include "otbImageMetadataInterfaceBase.h"
#include "otbFilterFunctionValues.h"
#include "otbObjectList.h"

namespace otb
{
/** \class OpticalImageMetadataInterface
 *
 * \brief Class for Optical captor metadata reading.
 *
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT OpticalImageMetadataInterface : public ImageMetadataInterfaceBase
{
public:
  typedef OpticalImageMetadataInterface Self;
  typedef ImageMetadataInterfaceBase    Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(OpticalImageMetadataInterface, ImageMetadataInterfaceBase);

  typedef Superclass::ImageType                ImageType;
  typedef Superclass::MetaDataDictionaryType   MetaDataDictionaryType;
  typedef Superclass::VectorType               VectorType;
  typedef Superclass::VariableLengthVectorType VariableLengthVectorType;
  typedef Superclass::ImageKeywordlistType     ImageKeywordlistType;

  typedef FilterFunctionValues                              FilterFunctionValuesType;
  typedef otb::ObjectList<FilterFunctionValuesType>         InternalWavelengthSpectralBandVectorType;
  typedef InternalWavelengthSpectralBandVectorType::Pointer WavelengthSpectralBandVectorType;

  /** Get the sun elevation from the ossim metadata */
  double GetSunElevation() const;

  /** Get the sun azimuth from the ossim metadata */
  double GetSunAzimuth() const;

  /** Get the sat elevation from the ossim metadata */
  virtual double GetSatElevation() const = 0;

  /** Get the sat azimuth from the ossim metadata */
  virtual double GetSatAzimuth() const = 0;

  /** Get the radiometric bias from the ossim metadata */
  virtual VariableLengthVectorType GetPhysicalBias() const = 0;

  /** Get the radiometric gain from the ossim metadata */
  virtual VariableLengthVectorType GetPhysicalGain() const = 0;

  /** Get the solar irradiance from the ossim metadata */
  virtual VariableLengthVectorType GetSolarIrradiance() const = 0;

  /** Get the first wavelength for the spectral band definition */
  virtual VariableLengthVectorType GetFirstWavelengths() const = 0;

  /** Get the last wavelength for the spectral band definition */
  virtual VariableLengthVectorType GetLastWavelengths() const = 0;

  /** Get the enhanced band names */
  std::vector<std::string> GetEnhancedBandNames() const override = 0;

  /** This method is to handle the permutation of the spectral band by some image provider
   * in most cases, this method won't change the value, but for SPOT data, the bands are set up as
   *  2 1 0 3 in the tiff file, this method which is overloaded for SPOT enables to retrieve the
   *  proper band. */
  virtual unsigned int BandIndexToWavelengthPosition(unsigned int i) const;

  /** Vector that contains the filter function value in 6S format (step of 0.0025 micro m).
   * There values a computed by 6S. */
  virtual WavelengthSpectralBandVectorType GetSpectralSensitivity() const = 0;

    /** Reads into the MetaDataDictionary to find an OSSIM ImageKeywordlist,
   * then translate it into ImageMetadata. Handles most optical sensors.
   * Returns true if succeed. */
  bool ConvertImageKeywordlistToImageMetadata() override;

protected:
  OpticalImageMetadataInterface();
  ~OpticalImageMetadataInterface() override
  {
  }

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;


private:
  OpticalImageMetadataInterface(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#endif
